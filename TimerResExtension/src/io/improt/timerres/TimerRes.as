package io.improt.timerres
{
	import flash.events.EventDispatcher;
	import flash.external.ExtensionContext;
	import flash.system.fscommand;
	
	public class TimerRes extends EventDispatcher {
		
		private var extContext:ExtensionContext;
		
		public function TimerRes() 
		{
			super();
		}
		
		public function Init() : Boolean 
		{
			extContext = ExtensionContext.createExtensionContext("TimerResExt", null);
			return extContext != null;
		}
		
		/*
		 * Try to update Window TimerResolution to specified value (in microseconds, 1000 = 1.0 ms)
		 * Uses lowest available if requested is below the available lowest.
		 * 
		 * @return false if unable to update resolution.
		 *   (No NTDLL.dll, no NtSetTimerResolution/NtQueryTimerResolution available)
		 */
		public function SetResolution(target:uint) : Boolean {
			return extContext.call("TimerRes_Update", target) as Boolean;
		}
		
		public function dispose():void {
			extContext.dispose();
		}
	}
	
}