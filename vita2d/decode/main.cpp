/* draw single frame of a video
*/

#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/sysmem.h>
#include <psp2/videodec.h>
#include <vita2d.h>
#include <iostream>
#include <fstream>
#include <cstring>

extern "C" {
	#include <libavcodec/avcodec.h>	
};

#define DECODER_BUFFER_SIZE (92 * 1024)
#define IN_BUFFER_SIZE 534 * 1024

//void readMp4 (void);
void log (const char* info);
void log (std::string info);
//void check (std::string name, int value);

//uint8_t* buffer;
//size_t buffer_length;
//int success = 1;

const AVCodec* codec;
AVCodecParserContext* parser;
AVCodecContext* context = nullptr;
AVPacket* packet;

uint8_t inbuf[IN_BUFFER_SIZE + AV_INPUT_BUFFER_PADDING_SIZE];
size_t inbuf_size;

FILE* file;

int main()
{ 
	avcodec_register_all();
	
	packet = av_packet_alloc();
	if (!packet)
	{
		log("packet :: FAILED");
		sceKernelExitProcess(0);
	}
	else
		log("packet :: SUCCESS");
	
	memset(inbuf + IN_BUFFER_SIZE, 0, AV_INPUT_BUFFER_PADDING_SIZE);

	codec = avcodec_find_decoder(AV_CODEC_ID_MPEG4);
	if (!codec)
        {
                log("codec :: FAILED");
                sceKernelExitProcess(0);
        }
        else
                log("codec :: SUCCESS");

	parser = av_parser_init(codec->id);
	if (!parser) {
		log("parser :: FAILED");
                sceKernelExitProcess(0);
	}
	else
                log("parser :: SUCCESS");
	
	context = avcodec_alloc_context3(codec);
        if (!context) {
                log("context :: FAILED");
                sceKernelExitProcess(0);
        }
        else
                log("context :: SUCCESS");
	
	context-> width = 640;
	context-> height = 360;

	if (avcodec_open2(context, codec, nullptr) < 0) {
		log("link codec to context :: FAILED");
                sceKernelExitProcess(0);
	}
	else
		log("link codec to context :: SUCCESS");

	
	file = fopen("app0:WakeUp.mp4", "rb");
	if (avcodec_open2(context, codec, nullptr) < 0) {
                log("open file :: FAILED");
                sceKernelExitProcess(0);
        }
        else
                log("open file :: SUCCESS");

	inbuf_size = fread(inbuf, 1, IN_BUFFER_SIZE, file);


	int numOfBytesRead = av_parser_parse2(parser, context, &packet->data, &packet->size, inbuf, inbuf_size, AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);
	log("numOfBytesRead :: " + std::to_string(numOfBytesRead));

	if (numOfBytesRead < 0) {
                log("ERROR :: parsing");
		sceKernelExitProcess(0);
        }


































	sceKernelExitProcess(0);






















	//readMp4();
	
	/*vita2d_init();
	vita2d_set_clear_color(RGBA8(0x40, 0x40, 0x40, 0xFF));
	
	vita2d_texture* texture = vita2d_create_empty_texture_format(960, 544, SCE_GXM_TEXTURE_FORMAT_U8U8U8U8_ABGR);
	vita2d_start_drawing();

	SceVideodecQueryInitInfoHwAvcdec init = {
		sizeof(SceVideodecQueryInitInfoHwAvcdec),
		960,
		544,
		3,
		1
	};

	success = sceVideodecInitLibrary(SCE_VIDEODEC_TYPE_HW_AVCDEC, &init);
	check("sceVideoInitLibrary", success);

	SceAvcdecQueryDecoderInfo query = {
		init.horizontal,
		init.vertical,
		init.numOfRefFrames
	};
	
	SceAvcdecDecoderInfo decoder_info = {0};
	
	success = sceAvcdecQueryDecoderMemSize(SCE_VIDEODEC_TYPE_HW_AVCDEC, &query, &decoder_info);
	check("sceAvcdecQueryDecoderMemSize", success);

	log("Frame Memory Size: " + std::to_string(decoder_info.frameMemSize));
	
	size_t sz = (decoder_info.frameMemSize + 0xFFFFF) & ~0xFFFFF;
	SceAvcdecCtrl* ctrl = (SceAvcdecCtrl*)calloc(1, sizeof(SceAvcdecCtrl));
	ctrl->frameBuf.size = sz;

	SceUID decoderBlock = -1;
	decoderBlock = sceKernelAllocMemBlock("decoder", SCE_KERNEL_MEMBLOCK_TYPE_USER_MAIN_PHYCONT_NC_RW, sz, nullptr);
	success = sceKernelGetMemBlockBase(decoderBlock, &ctrl->frameBuf.pBuf);
	check("sceKernelGetMemBlockBase", success);

	success = sceAvcdecCreateDecoder(SCE_VIDEODEC_TYPE_HW_AVCDEC, ctrl, &query);
	check("sceAvcdecCreateDecoder", success);
	
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
	check("sceAvcdecDecode", success);

	while (true)
	{
		vita2d_start_drawing();
		
		vita2d_clear_screen();
		vita2d_draw_texture(texture, 200, 200);
		vita2d_end_drawing();
		
		vita2d_wait_rendering_done();
		vita2d_swap_buffers();
	}
	
	vita2d_free_texture(texture);
	vita2d_fini();

	sceKernelFreeMemBlock(decoderBlock);
	delete buffer;
	sceKernelExitProcess(0);*/
}

/*void readMp4 (void)
{
	std::ifstream file("app0:test.mp4", std::ios::binary);	
	file.seekg(0, std::ios::end);
	data_size = file.tellg();
	file.seekg(0, std::ios::beg);
	
	data = new uint8_t[data_size];
	file.read((char*) data, data_size);
	file.close();

	log("\nfile length: " + std::to_string(data_size));
	log("");
}*/

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

void check (std::string name, int value)
{
	if (value == 0)
		log(name + " : " + "SUCCESS" );
	else
		log(name + " : " + "FAILED" );
}
