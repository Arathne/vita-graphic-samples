/* PROCESS
 *   initialize window/renderer
 *   load image into surface
 *   make surface into texture (texture stores image in GPU instead of RAM)
 *   create a rectangle at center of screen
 *   link texture to rectangle
 *   clear screen
 *   draw square
 *   display
*/

#include <psp2/kernel/processmgr.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

int main()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return -1;
	
	SDL_Window* window = SDL_CreateWindow( "image", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 960, 544, SDL_WINDOW_SHOWN );

	if (window == nullptr)
		return -1;

	SDL_Renderer* renderer = SDL_CreateRenderer( window, -1, 0 );

	if (renderer == nullptr)
		return -1;
	
	IMG_Init(IMG_INIT_PNG);
	
	SDL_Surface* surface = IMG_Load( "app0:/assets/epicface.png");
	SDL_Texture* texture = SDL_CreateTextureFromSurface( renderer, surface );
	
	SDL_Rect square;
	square.x = 380;
	square.y = 170;
	square.w = 200;
	square.h = 200;
	
	SDL_RenderClear( renderer );
	SDL_RenderCopy( renderer, texture, nullptr, &square );
	SDL_RenderPresent( renderer );

	SDL_Delay(10000);
	
	SDL_DestroyTexture( texture );
	SDL_FreeSurface( surface );
	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );

	sceKernelExitProcess(0);
	
	return 0;
}
