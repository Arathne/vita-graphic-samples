/* separate arrays to draw triangle with colors
*/

#include <psp2/kernel/processmgr.h>
#include <vitaGL.h>

float vertices[] = {
	  0.0f,   0.0f,
	480.0f, 544.0f,
	960.0f,   0.0f
};

float color[] = {
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f
};

int main()
{
	vglInit(0x800000);
	vglWaitVblankStart(GL_TRUE);

	glClearColor(0.5, 0.5, 0.5, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0, 960, 0, 544, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	while (true)
	{
		vglStartRendering();
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		glVertexPointer(2, GL_FLOAT, 2*sizeof(float), vertices);
		glColorPointer(3, GL_FLOAT, 3*sizeof(float), color);
		
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		vglStopRendering();
	}

	vglEnd();
	sceKernelExitProcess(0);
}
