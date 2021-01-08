/* PROCESS
 *   create window/renderer
 *   create square
 *   loop
 *     clear screen
 *     draw square
 *     display
 *     move square
 *
 *   NOTE: removing vsync and running the program with a high fps will cause screen tearing.
 *     EX: change SDL_RENDERER_PRESENTVSYNC to 0 for example on screen tearing (look at corners of square)
*/

#include <psp2/kernel/processmgr.h>
#include <SDL2/SDL.h>

int main()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return -1;

	SDL_Window* window = SDL_CreateWindow( "loop", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 960, 544, SDL_WINDOW_SHOWN );

	if (window == nullptr)
		return -1;

	SDL_Renderer* renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_PRESENTVSYNC );

	if (renderer == nullptr)
		return -1;
	
	SDL_Rect square;
	square.x = 0;
	square.y = 50;
	square.h = 100;
	square.w = 100;
	
	bool run = true;
	
	while (run)
	{
		SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
		SDL_RenderClear( renderer );
		
		SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
		SDL_RenderFillRect( renderer, &square );

		SDL_RenderPresent( renderer );
		
		square.x = square.x + 1;

		if (square.x >= 860)
			run = false;
	}

	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
	
	SDL_Quit();
	sceKernelExitProcess(0);

	return 0;
}
