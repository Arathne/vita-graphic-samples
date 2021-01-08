/* draws triangle using vertex buffer object
 * NOTE: no idea why one of the vertices is colored
*/

#include <psp2/kernel/processmgr.h>
#include <vitaGL.h>

float vertices[] = {
	  0.0f,   0.0f, 0.0f,
	480.0f, 544.0f, 0.0f,
	960.0f,   0.0f, 0.0f
};

unsigned int VBO;

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
	
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), &vertices, GL_STATIC_DRAW);
	
	while (true)
	{
		vglStartRendering();
		glEnableClientState(GL_VERTEX_ARRAY);
		
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		glDisableClientState(GL_VERTEX_ARRAY);
		vglStopRendering();
	}

	vglEnd();
	sceKernelExitProcess(0);
}
