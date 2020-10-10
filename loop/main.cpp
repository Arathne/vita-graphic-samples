#include <psp2/kernel/processmgr.h>
#include <SDL2/SDL.h>

int main()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return -1;

	SDL_Window* window = SDL_CreateWindow( "loop", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 960, 544, SDL_WINDOW_SHOWN );

	if (window == nullptr)
		return -1;

	SDL_Renderer* renderer = SDL_CreateRenderer( window, -1, 0 );

	if (renderer == nullptr)
		return -1;
	
	SDL_Rect square;
	square.x = 0;
	square.y = 50;
	square.h = 100;
	square.w = 100;
	
	SDL_Event currentEvent;
	bool run = true;
	
	while (run)
	{
		while (SDL_PollEvent(&currentEvent))
		{
			if (currentEvent.type == SDL_QUIT)
				run = false;
		}

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
