/* draw texture from png
*/

#include <psp2/kernel/processmgr.h>
#include <vita2d.h>

int main()
{
	vita2d_init();
	vita2d_set_clear_color(RGBA8(0x40, 0x40, 0x40, 0xFF));
	
	vita2d_texture* image = vita2d_load_PNG_file("app0:face.png");

	while (true)
	{
		vita2d_start_drawing();
		vita2d_clear_screen();
		
		vita2d_draw_texture(image, 50, 50);

		vita2d_end_drawing();
		vita2d_swap_buffers();
	}

	vita2d_fini();
	sceKernelExitProcess(0);
}
