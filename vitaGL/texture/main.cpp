/* draw texture with glBegin()
*/

#include <psp2/kernel/processmgr.h>
#include <vitaGL.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main()
{
	stbi_set_flip_vertically_on_load(true);
	vglInit(0x800000);

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0, 960, 0, 544, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	int width, height, channels;
	unsigned char* image = stbi_load("app0:face.png", &width, &height, &channels, 4);

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glEnable(GL_TEXTURE_2D);
	
	stbi_image_free(image);

	while (true)
	{
		vglStartRendering();
		
		glClear(GL_COLOR_BUFFER_BIT);
		
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBegin(GL_TRIANGLES);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		
		glTexCoord2f(0.5f, 1.0f);
		glVertex3f(480.0f, 544.0f, 0.0f);
		
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(960.0f, 0.0f, 0.0f);
		glEnd();

		vglStopRendering();
	}

	vglEnd();
	sceKernelExitProcess(0);
}

