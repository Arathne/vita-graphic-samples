/* creates a thread while passing parameters into it
 *
 * possible to use sceKernelCreateThread and sceKernelStartThread, but I couldn't find a way to pass parameters into a thread with it. Not ideal for object oriented programming (C++).
*/

#include <psp2/kernel/processmgr.h>
#include <vita2d.h>
#include <pthread.h>
#include <string>

std::string out1 = "-";
std::string out2 = "-";

void* thread_func (void* arg)
{
	out2 = std::string((const char*) arg) + std::string(" ") + std::to_string(2);

	pthread_exit(nullptr);
	return nullptr;
}

int main()
{
	vita2d_init();
	vita2d_set_clear_color(RGBA8(0x40, 0x40, 0x40, 0xFF));

	vita2d_pgf* pgf = vita2d_load_default_pgf();
	
	const char* message = "HELLO WORLD";

	out1 = std::string(message);

	pthread_t thread;
	pthread_create(&thread, nullptr, thread_func, (void*) message);

	while (true)
	{
		vita2d_start_drawing();
		vita2d_clear_screen();
		
		vita2d_pgf_draw_text(pgf, 30, 30, RGBA8(255,255,255,255), 1.0f, out1.c_str());
		vita2d_pgf_draw_text(pgf, 30, 60, RGBA8(255,255,255,255), 1.0f, out2.c_str());

		vita2d_end_drawing();
		vita2d_swap_buffers();
	}

	vita2d_fini();
	sceKernelExitProcess(0);
}
