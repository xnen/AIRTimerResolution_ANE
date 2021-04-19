// dllmain.cpp : Defines the entry point for the DLL application.

#include "pch.h"
#include "FlashRuntimeExtensions.h"
#include <Windows.h>
#include "TimerRes.cpp"

extern "C" {
	// Update timer resolution to a requested value (in microseconds)
	__declspec(dllexport) FREObject UpdateResolution(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		if (argc != 1) return NULL;
		uint32_t target_res;

		FREResult castResult = FREGetObjectAsUint32(argv[0], &target_res);

		if (castResult != FRE_OK) {
			return NULL;
		}

		// Convert to 100ns units for Nt Resolution Funcs
		target_res *= 10;

		FREObject result;

		FRENewObjectFromBool(
			TimerRes().update_resolution((unsigned long) target_res), 
			&result
		);

		return result;
	}

	__declspec(dllexport) void TimerRes_Destroy(void* ctx) {
		return;
	}

	__declspec(dllexport) void TimerRes_Init(void* extData, const uint8_t* ctxType, FREContext ctx, uint32_t* numFunctionsToSet, const FRENamedFunction** functionsToSet) {
		static FRENamedFunction extArr[] = {
			{ 
				(const uint8_t*) "TimerRes_Update", nullptr, UpdateResolution 
			},
		};

		*numFunctionsToSet = sizeof(extArr) / sizeof(extArr[0]);
		*functionsToSet = extArr;
	}

	__declspec(dllexport) void TimerResExt_Init(void** extDataToSet, FREContextInitializer* ctxInitializerToSet, FREContextFinalizer* ctxFinalizerToSet) {
		extDataToSet = NULL;
		*ctxInitializerToSet = &TimerRes_Init;
		*ctxFinalizerToSet = &TimerRes_Destroy;
	}
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

