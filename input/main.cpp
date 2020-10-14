/* PROCESS
 *   initialize window/renderer/font
 *   initialize input detection
 *   loop
 *     update text when button pressed
 *     clear
 *     draw
 *     display  
 *   
 *   NOTE: updating text every frame can have a performance impact
*/

#include <psp2/kernel/processmgr.h>
#include <psp2/ctrl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

TTF_Font* font;
SDL_Rect rectangle;
SDL_Texture* texture;

SDL_Window* window;
SDL_Renderer* renderer;

SDL_Color white = {255, 255, 255};

void updateText( const char* text );


int main()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return -1;
	
	window = SDL_CreateWindow( "input", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 960, 544, SDL_WINDOW_SHOWN );

	if (window == nullptr)
		return -1;

	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_PRESENTVSYNC );

	if (renderer == nullptr)
		return -1;

	if (TTF_Init() < 0)
		return -1;

	font = TTF_OpenFont("app0:/assets/texterius.ttf", 30);
	
	sceCtrlSetSamplingMode( SCE_CTRL_MODE_DIGITAL );
	SceCtrlData control;

	updateText( "press cross/square/triangle/circle" );
	
	bool run = true;
	while (run)
	{
		sceCtrlPeekBufferPositive( 0, &control, 1 );
		
		if (control.buttons == SCE_CTRL_CROSS)
			updateText("cross");
		else if (control.buttons == SCE_CTRL_SQUARE)
			updateText("square");
		else if (control.buttons == SCE_CTRL_TRIANGLE)
			updateText("triangle");
		else if (control.buttons == SCE_CTRL_CIRCLE)
			updateText("circle");
		else if (control.buttons == SCE_CTRL_RTRIGGER)
			run = false;

		SDL_RenderClear( renderer );
		SDL_RenderCopy( renderer, texture, nullptr, &rectangle );
		SDL_RenderPresent( renderer );
	}
	
	SDL_DestroyTexture( texture );
	TTF_CloseFont( font );
	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
	
	SDL_Quit();
	sceKernelExitProcess(0);

	return 0;
}

void updateText( const char* text )
{
	if( texture != nullptr )
		SDL_DestroyTexture( texture );

	SDL_Surface* surface = TTF_RenderText_Solid( font, text, white );
	texture = SDL_CreateTextureFromSurface( renderer, surface );
	
	rectangle.x = 480-(surface->w/2);
	rectangle.y = 272-(surface->h/2);
	rectangle.w = surface-> w;
	rectangle.h = surface-> h;
	
	SDL_FreeSurface( surface );
}
