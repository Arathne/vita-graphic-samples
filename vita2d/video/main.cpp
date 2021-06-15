/* plays mp4 video without sound
 *
 * credits to SonicMastr
*/

#include <psp2/kernel/processmgr.h>
#include <psp2/sysmodule.h>
#include <psp2/avplayer.h>
#include <psp2/gxm.h>
#include <psp2/audioout.h> 

#include <vita2d.h>

#include <utils.h>
#include <iostream>
#include <cstring>

#define PCM_BUFFER 4096

SceAvPlayerHandle playerHandle;
SceUID playerThread = 0;
SceUID audioThread = 0;

int audioPort = -1;
uint32_t audioSampleRate = 48000;
uint16_t channelCount = 2;

/* runs when the AvPlayerThread is created
*/
SceInt32 loadAvPlayerThread (SceSize args, void* argp)
{
	SceAvPlayerInitData playerInit;
	memset(&playerInit, 0, sizeof(SceAvPlayerInitData));

        playerInit.memoryReplacement.objectPointer = nullptr;
        playerInit.memoryReplacement.allocate = Allocate;
        playerInit.memoryReplacement.deallocate = Deallocate;
        playerInit.memoryReplacement.allocateTexture = AllocateTexture;
        playerInit.memoryReplacement.deallocateTexture = DeallocateTexture;

        playerInit.basePriority = 125;
        playerInit.numOutputVideoFrameBuffers = 6;
        playerInit.autoStart = SCE_FALSE;
        playerInit.debugLevel = 3;
	
	playerHandle = sceAvPlayerInit(&playerInit);
	sceAvPlayerAddSource(playerHandle, "app0:WakeUp.mp4");

	return sceKernelExitThread(0);
}


SceInt32 loadAudioThread (SceSize args, void* argp)
{
	SceAvPlayerFrameInfo audioInfo;
	memset(&audioInfo, 0, sizeof(SceAvPlayerFrameInfo));
	
	while (sceAvPlayerIsActive(playerHandle))
	{
		if (sceAvPlayerGetAudioData(playerHandle, &audioInfo))
		{
			// update to match video audio sample rate and channel count;
			if (audioSampleRate != audioInfo.details.audio.sampleRate || channelCount != audioInfo.details.audio.channelCount)
			{
				audioSampleRate = audioInfo.details.audio.sampleRate;
				channelCount = audioInfo.details.audio.channelCount;

				SceAudioOutMode mode = SCE_AUDIO_OUT_MODE_STEREO;
				if (channelCount == 1)
					mode = SCE_AUDIO_OUT_MODE_MONO;

				sceAudioOutSetConfig(audioPort, (PCM_BUFFER/channelCount/sizeof(int16_t)), audioSampleRate, mode);
			}

			sceAudioOutOutput(audioPort, audioInfo.pData);	
		}
	}
	
	return sceKernelExitThread(0);
}


int main()
{ 
	// initialize player
	sceSysmoduleLoadModule(SCE_SYSMODULE_AVPLAYER);
	
	SceUID main_thread_uid = sceKernelGetThreadId();
    	sceKernelChangeThreadPriority(main_thread_uid, 70);
	
	playerThread = sceKernelCreateThread("loadAVPlayer", loadAvPlayerThread, 0x10000100, 0x4000, 0, 0, nullptr);
	sceKernelStartThread(playerThread, 0, nullptr);
	
	// initialize vita2d
	vita2d_init();
        vita2d_set_clear_color(RGBA8(0x40, 0x40, 0x40, 0xFF));
	vita2d_texture* frameTexture = (vita2d_texture*) malloc(sizeof(*frameTexture));
	
	// start player
	sceAvPlayerSetLooping(playerHandle, SCE_FALSE);
	sceAvPlayerStart(playerHandle);
	SceAvPlayerFrameInfo frameInfo;
	memset(&frameInfo, 0, sizeof(SceAvPlayerFrameInfo));
	
	// start sound
	//   thread must be started after sceAvPlayerStart()
	audioThread = sceKernelCreateThread("AudioOutput", loadAudioThread, 0x10000100, 0x4000, 0, 0, nullptr);
	sceKernelStartThread(audioThread, 0, nullptr);
	audioPort = sceAudioOutOpenPort(SCE_AUDIO_OUT_PORT_TYPE_BGM, (PCM_BUFFER/channelCount/sizeof(int16_t)), audioSampleRate, SCE_AUDIO_OUT_MODE_STEREO);
	
	bool drawTexture = false;

	while (sceAvPlayerIsActive(playerHandle)) 
	{
		vita2d_start_drawing();
		vita2d_clear_screen();

		// grab frame data and place into texture
		if (sceAvPlayerGetVideoData(playerHandle, &frameInfo)) {
			sceGxmTextureInitLinear(&frameTexture->gxm_tex, frameInfo.pData, SCE_GXM_TEXTURE_FORMAT_YVU420P2_CSC1, frameInfo.details.video.width, frameInfo.details.video.height, 0);
			drawTexture = true;
		}
		
		if (drawTexture && frameTexture != nullptr)
		{
			// stretch video to entire screen
			// 16:9 ratio
			float scaleX = 960.0f / ((frameInfo.details.video.width/16)*16);
			float scaleY = 544.0f / ((frameInfo.details.video.height/9)*9);

			// draw frame of video
    			vita2d_draw_texture_scale(frameTexture, 0, 0, scaleX, scaleY);
		}

		vita2d_end_drawing();
		vita2d_swap_buffers();
	}
	
	sceAudioOutReleasePort(audioPort);
	sceKernelExitProcess(0);
}
