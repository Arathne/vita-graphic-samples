/* launching separate game/app
 *
 * change gameid to any game installed in vita
 * easiest way to launch a psp game is to create a psp bubble using Adrenaline Bubbles Manager and then update 'gameid' with bubble titleid.
*/

#include <psp2/kernel/processmgr.h>
#include <psp2/appmgr.h>
#include <string>

int main(){
	std::string gameid = "PCSE01253"; //pic-a-pix color
	
	std::string uri = std::string("psgm:play?titleid=") + gameid;
	sceAppMgrLaunchAppByUri(0x20000, uri.c_str());

	sceKernelExitProcess(0);
}
