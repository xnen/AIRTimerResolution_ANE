package io.improt.timerres
{
	import flash.display.Sprite;
	import flash.events.EventDispatcher;
	import flash.external.ExtensionContext;
	import flash.system.fscommand;
	
	public class TimerResANE extends Sprite {
		
		private static var aneInst:TimerRes;
		
		public function TimerResANE() {
			super();
		}
		
		public static function IsInitialized() : Boolean {
			return aneInst != null;
		}
		
		public static function Init() : Boolean {
			aneInst = new TimerRes();
			return aneInst.Init();
		}
		
	    /*
		 * Try to update Window TimerResolution to specified value (in microseconds, 1000 = 1.0 ms)
		 * Uses lowest available if requested is below the available lowest.
		 * 
		 * @return false if unable to update resolution.
		 *   (No NTDLL.dll, no NtSetTimerResolution/NtQueryTimerResolution available)
		 */
		public static function UpdateResolution(target:uint) : Boolean {
			if (!IsInitialized()) {
				trace("WARN UpdateResolution() called before Init");
				return false;
			}
			
			var o:Object = aneInst.SetResolution(target);
			
			if (o == null) {
				return false;
			}
			
			return o as Boolean;
		}
		
		public static function dispose() : void {
			if (!IsInitialized()) {
				return;
			}
			
			aneInst.dispose();
			aneInst = null;
		}
	}
	
}