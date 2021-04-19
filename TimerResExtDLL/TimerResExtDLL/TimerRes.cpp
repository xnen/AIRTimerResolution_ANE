#include "pch.h"
#include "TimerRes.h"
#include "FlashRuntimeExtensions.h"
#include <windows.h>
#include <winternl.h>

typedef NTSTATUS(NTAPI* ModifyTimerResolution) (ULONG RequestedResolution, BOOLEAN Set, PULONG ActualResolution);
typedef NTSTATUS(NTAPI* QueryTimerResolution)  (PULONG Min, PULONG Max, PULONG Current);

class TimerRes
{

public:
	/*
	 * Attempt to update timer resolution to target_res (100ns units)
	 * target_res is clamped between the system's minimum resolution and maximum resolution.
	 */
	bool update_resolution(ULONG target_res) {
		const HINSTANCE hLibrary = LoadLibrary(L"NTDLL.dll");
		if (hLibrary == nullptr) { return false; }

		ModifyTimerResolution modifyTimer = (ModifyTimerResolution) GetProcAddress (hLibrary, "NtSetTimerResolution");
		QueryTimerResolution  queryTimer  = (QueryTimerResolution) GetProcAddress  (hLibrary, "NtQueryTimerResolution");
		
		if (modifyTimer == nullptr || queryTimer == nullptr) {
			return false;
		}

		ULONG minRes, maxRes, curRes;

		// Query Timer parameters
		NTSTATUS query_status = queryTimer(&minRes, &maxRes, &curRes);
		if (query_status == STATUS_ACCESS_VIOLATION) {
			return false;
		}

		ULONG clamped_target = min(minRes, max(maxRes, target_res));

		if (curRes <= clamped_target) {
			return true;
		}

		ULONG beforeRes = curRes;

		// Attempt to set to target
		NTSTATUS modify_status = modifyTimer(clamped_target, TRUE, &curRes);
		if (modify_status == STATUS_ACCESS_VIOLATION) {
			return false;
		}

		return beforeRes >= curRes;
	}
};