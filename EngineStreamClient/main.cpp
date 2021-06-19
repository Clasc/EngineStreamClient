#include "StreamerClient.h"
#include <SDL.h>

#undef main

int main(int argc, char const *argv[])
{
    StreamerClient client = StreamerClient();
	client.init();
	SDL_Window* win = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Texture* bitmapTex = NULL;
	SDL_Surface* bitmapSurface = NULL;
	int posX = 100, posY = 100, width = 320, height = 240;

	SDL_Init(SDL_INIT_VIDEO);

	win = SDL_CreateWindow("Hello World", 
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height, 
		0
	);

	bool running = true;

	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	auto streamTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_UYVY, SDL_TEXTUREACCESS_STREAMING, width, height);
	char buffer[100000];


	while (running) {
		client.receiveAndEncode(buffer);
		int pitch;
		auto success = SDL_LockTexture(streamTexture, NULL, (void**) buffer, &pitch);
		
		if (success == -1) {
			printf("error filling streamtexture");
			SDL_UnlockTexture(streamTexture);
			continue;
		}

		SDL_UnlockTexture(streamTexture);
		SDL_Event e;
		if (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				running = false;
			}
		}
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, bitmapTex, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyTexture(bitmapTex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);

	SDL_Quit();
    return 0;
}
