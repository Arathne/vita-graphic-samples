/* plays mp4 video without sound
 *
 * credits to SonicMastr
*/

/* important: video player crashes randomly. Possible that it has something to do with pbuf or threads
*/

#include <psp2/kernel/processmgr.h>
#include <psp2/sysmodule.h>
#include <psp2/avplayer.h>
#include <psp2/gxm.h>
#include <psp2/audioout.h> 
#include <psp2/display.h> 

#include <vita2d.h>

#include <utils.h>
#include <iostream>
#include <cstring>

#define PCM_BUFFER 4096

SceAvPlayerHandle playerHandle;

SceInt32 loadAudioThread (SceSize args, void* argp)
{
	uint32_t audioSampleRate = 48000;
	uint16_t channelCount = 2;
	
	SceUID audioPort = sceAudioOutOpenPort(SCE_AUDIO_OUT_PORT_TYPE_BGM, (PCM_BUFFER/channelCount/sizeof(int16_t)), audioSampleRate, SCE_AUDIO_OUT_MODE_STEREO);
	
	SceAvPlayerFrameInfo audioInfo;
	memset(&audioInfo, 0, sizeof(SceAvPlayerFrameInfo));

	while (sceAvPlayerIsActive(playerHandle))
	{
		if (sceAvPlayerGetAudioData(playerHandle, &audioInfo))
		{
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
	
	sceAudioOutReleasePort(audioPort);
	
	return sceKernelExitThread(0);
}


int main()
{ 
	// initialize vita2d
	vita2d_init();
        vita2d_set_clear_color(RGBA8(0x40, 0x40, 0x40, 0xFF));
	vita2d_set_vblank_wait(true);
	vita2d_pgf* pgf = vita2d_load_default_pgf();
	
	vita2d_texture* frameTexture = (vita2d_texture*)malloc(sizeof(vita2d_texture));;
	frameTexture -> palette_UID = 0;

	// initialize player
	sceSysmoduleLoadModule(SCE_SYSMODULE_AVPLAYER);
	
	SceAvPlayerInitData playerInit;
	memset(&playerInit, 0, sizeof(SceAvPlayerInitData));

        playerInit.memoryReplacement.allocate = Allocate;
        playerInit.memoryReplacement.deallocate = Deallocate;
        playerInit.memoryReplacement.allocateTexture = AllocateTexture;
        playerInit.memoryReplacement.deallocateTexture = DeallocateTexture;

        playerInit.basePriority = 0xA0;
        playerInit.numOutputVideoFrameBuffers = 1;
        playerInit.autoStart = true;
        playerInit.debugLevel = 3;
	
	playerHandle = sceAvPlayerInit(&playerInit);

	sceAvPlayerSetLooping(playerHandle, false);
	sceAvPlayerAddSource(playerHandle, "app0:WakeUp.mp4");
	
	SceAvPlayerFrameInfo frameInfo;
	memset(&frameInfo, 0, sizeof(SceAvPlayerFrameInfo));
	
	// start sound
	SceUID audioThread = sceKernelCreateThread("AudioOutput", loadAudioThread, 0x10000100, 0x4000, 0, 0, nullptr);
	sceKernelStartThread(audioThread, 0, nullptr);

	// draw video frames
	bool drawTexture = false;
	
	while (sceAvPlayerIsActive(playerHandle)) 
	{
		vita2d_start_drawing();
		vita2d_clear_screen();

		if (sceAvPlayerGetVideoData(playerHandle, &frameInfo)) 
		{
			sceGxmTextureInitLinear(
				&frameTexture->gxm_tex,
				frameInfo.pData,
				SCE_GXM_TEXTURE_FORMAT_YVU420P2_CSC1,
				frameInfo.details.video.width,
				frameInfo.details.video.height, 
				0
			);
			
			drawTexture = true;
		}
		
		if (drawTexture)
		{
			float scaleX = 960.0f / ((frameInfo.details.video.width/16)*16);
			float scaleY = 544.0f / ((frameInfo.details.video.height/9)*9);
			
    			vita2d_draw_texture_scale(frameTexture, 0, 0, scaleX, scaleY);
		}
		
		uint64_t time = sceAvPlayerCurrentTime(playerHandle);
		vita2d_pgf_draw_text(pgf, 30, 30, RGBA8(0, 255, 0, 255), 1.0f, std::to_string(time).c_str());

		vita2d_end_drawing();
		vita2d_swap_buffers();
	}
	
	vita2d_fini();
	vita2d_free_texture(frameTexture);
	
	sceKernelDeleteThread(audioThread);
	sceAvPlayerClose(playerHandle);

	sceKernelExitProcess(0);
}
