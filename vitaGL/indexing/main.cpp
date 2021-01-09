/* 4 vertices to draw rectangle
 * NOTE: not using indexing will result in needing 6 vertices (3 for each triangle)
*/

#include <psp2/kernel/processmgr.h>
#include <vitaGL.h>

float color[] = {
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 0.0f
};

float vertices[] = {
	240.0f, 408.0f,
	720.0f, 408.0f,
	240.0f, 136.0f,
	720.0f, 136.0f
};

short unsigned int indices[] = {
	0, 1, 2,
	1, 2, 3
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
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
		
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		
		vglStopRendering();
	}

	vglEnd();
	sceKernelExitProcess(0);
}
