/* PROCESS:
 *    initialize
 *    create window
 *    create renderer
 *    select color for drawing
 *    clear the screen with specified color
 *    update screen
 *    wait 3 seconds
 *    shutdown
*/

#include <psp2/kernel/processmgr.h>
#include <SDL2/SDL.h>

int main()
{
	if( SDL_Init(SDL_INIT_VIDEO) < 0 )
		return -1;

	SDL_Window* window = SDL_CreateWindow( "basic", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 960, 544, SDL_WINDOW_SHOWN );
	
	if (window == nullptr)
		return -1;
	
	SDL_Renderer* renderer = SDL_CreateRenderer( window, -1, 0 );
	
	if (renderer == nullptr)
		return -1;
	
	SDL_SetRenderDrawColor( renderer, 255, 100, 0, 255 );
	SDL_RenderClear( renderer );
	SDL_RenderPresent( renderer );

	SDL_Delay(10000);
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	sceKernelExitProcess(0);
	return 0;
}
