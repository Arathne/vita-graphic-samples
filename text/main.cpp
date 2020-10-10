/* PROCESS
 *  create window/renderer
 *  initialize SDL_ttf
 *  create font
 *  make font into a texture
 *  render rectangle with font
 */

#include <psp2/kernel/processmgr.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

int main()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return -1;

	SDL_Window* window = SDL_CreateWindow( "text", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 960, 544, SDL_WINDOW_SHOWN );

	if (window == nullptr)
		return -1;

	SDL_Renderer* renderer = SDL_CreateRenderer( window, -1, 0 );

	if (renderer == nullptr)
		return -1;
	
	if (TTF_Init() < 0)
		return -1;

	TTF_Font* font = TTF_OpenFont("app0:/assets/texterius.ttf", 100);	
	SDL_Color white = {255, 255, 255};
	
	SDL_Surface* surface = TTF_RenderText_Solid( font, "Hello World!", white );
	SDL_Texture* texture = SDL_CreateTextureFromSurface( renderer, surface );

	SDL_Rect rectangle;
	rectangle.x = 0;
	rectangle.y = 0;
	rectangle.h = 100;
	rectangle.w = 500;
	
	SDL_RenderClear( renderer );
	SDL_RenderCopy( renderer, texture, nullptr, &rectangle );
	SDL_RenderPresent( renderer );
	
	SDL_Delay(10000);
	
	SDL_DestroyTexture( texture );
	SDL_FreeSurface( surface );
	TTF_CloseFont( font );
	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
	
	SDL_Quit();
	sceKernelExitProcess(0);
	
	return 0;
}
