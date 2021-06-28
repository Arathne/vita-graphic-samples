/* managing time using vitasdk
*/

#include <psp2/kernel/processmgr.h>

int main(){
	SceUInt64 startTime = sceKernelGetProcessTimeWide(); // in microseconds : 1,000,000 ms = 1 s
	
	bool lock = true;
	while (lock)
	{
		SceUInt64 currentTime = sceKernelGetProcessTimeWide();
		
		if (currentTime >= startTime+(1000000*3)) // exit loop once 3 seconds have passed
		{
			lock = false;
		}
	}
	
	sceKernelExitProcess(0);
}
