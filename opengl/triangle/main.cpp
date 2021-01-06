// draws a triangle using opengl

#include <psp2/kernel/processmgr.h>
#include <vitaGL.h>

int main(){
	
	vglInit(0x800000);
	
	glClearColor(0.5, 0.5, 0.5, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glOrtho(0, 960, 0, 544, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	while (true)
	{
		vglStartRendering();
		
		glClear(GL_COLOR_BUFFER_BIT);
	
		glBegin(GL_TRIANGLES);
		glVertex3f(   0,   0, 0);
		glVertex3f( 480, 544, 0);
		glVertex3f( 960,   0, 0);
		glEnd();
		
		vglStopRendering();
	}

	vglEnd();
	sceKernelExitProcess(0);
}
