/* draw single frame of a video
*/

#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/sysmem.h>
#include <psp2/videodec.h>
#include <vita2d.h>
#include <iostream>
#include <fstream>
#include <cstring>

#define DECODER_BUFFER_SIZE (92 * 1024)

void readMp4 (void);
void log (const char* info);
void log (std::string info);

uint8_t* buffer;
size_t buffer_length;

int main()
{
	readMp4();
	
	vita2d_init();
	vita2d_set_clear_color(RGBA8(0x40, 0x40, 0x40, 0xFF));
	
	vita2d_texture* texture = vita2d_create_empty_texture_format(960, 544, SCE_GXM_TEXTURE_FORMAT_U8U8U8U8_ABGR);

	int success = 1;

	SceVideodecQueryInitInfoHwAvcdec init = {
		sizeof(SceVideodecQueryInitInfoHwAvcdec),
		960,
		544,
		3,
		1
	};

	sceVideodecInitLibrary(SCE_VIDEODEC_TYPE_HW_AVCDEC, &init);
	
	SceAvcdecQueryDecoderInfo query = {
		init.horizontal,
		init.vertical,
		init.numOfRefFrames
	};
	
	SceAvcdecDecoderInfo decoder_info = {0};
	
	sceAvcdecQueryDecoderMemSize(SCE_VIDEODEC_TYPE_HW_AVCDEC, &query, &decoder_info);
	
	log("Frame Memory Size: " + std::to_string(decoder_info.frameMemSize));
	
	SceAvcdecCtrl* ctrl = (SceAvcdecCtrl*)calloc(1, sizeof(SceAvcdecCtrl));

	size_t sz = (decoder_info.frameMemSize + 0xFFFFF) & ~0xFFFFF;
	SceUID decoderBlock = -1;
	ctrl->frameBuf.size = sz;
	decoderBlock = sceKernelAllocMemBlock("decoder", SCE_KERNEL_MEMBLOCK_TYPE_USER_MAIN_PHYCONT_NC_RW, sz, nullptr);
	sceKernelGetMemBlockBase(decoderBlock, &ctrl->frameBuf.pBuf);

	sceAvcdecCreateDecoder(SCE_VIDEODEC_TYPE_HW_AVCDEC, ctrl, &query);
	
	struct SceAvcdecPicture picture = {0};
	picture.size = sizeof(picture);
	picture.frame.pixelType = SCE_AVCDEC_PIXELFORMAT_RGBA8888;
	picture.frame.framePitch = 960;
	picture.frame.frameWidth = 960;
	picture.frame.frameHeight = 544;
	picture.frame.pPicture[0] = vita2d_texture_get_datap(texture);
	
	SceAvcdecAu au = {0};
	SceAvcdecArrayPicture array_picture = {0};
	struct SceAvcdecPicture* pictures = { &picture };
	array_picture.numOfElm = 1;
	array_picture.pPicture = &pictures;
	
	au.es.pBuf = buffer;
	au.es.size = buffer_length;
	au.dts.lower = 0xFFFFFFFF;
	au.dts.upper = 0xFFFFFFFF;
	au.pts.lower = 0xFFFFFFFF;
	au.pts.upper = 0xFFFFFFFF;

	success = sceAvcdecDecode(ctrl, &au, &array_picture);
	
	SceAvcdecPicture x = **array_picture.pPicture;
	log("pPicture.size: " + std::to_string(x.size));
	log("pPicture.timeScale: " + std::to_string(x.info.timeScale));

	if (success == 0) {
		log("SUCCESS");
	}
	else { 
		log("FAILED");
	}
	
	while (true)
	{
		vita2d_start_drawing();
		vita2d_clear_screen();

		vita2d_draw_texture(texture, 0, 0);
		
		vita2d_end_drawing();
		vita2d_swap_buffers();
	}

	vita2d_free_texture(texture);
	vita2d_fini();

	sceKernelFreeMemBlock(decoderBlock);
	delete buffer;
	sceKernelExitProcess(0);
}

void readMp4 (void)
{
	log(" ");

	std::ifstream file("app0:test.mp4", std::ios::binary);	
	file.seekg(0, std::ios::end);
	buffer_length = file.tellg();
	file.seekg(0, std::ios::beg);
	
	buffer = new uint8_t[buffer_length];
	file.read((char*) buffer, buffer_length);
	file.close();

	log("File Length: " + std::to_string(buffer_length));
}

void log (const char* info)
{
	std::ofstream logFile ("ux0:/data/Test/decode.log", std::ios_base::app);
	logFile << info << "\n";
	logFile.close();
}

void log (std::string info)
{
	log(info.c_str());
}
