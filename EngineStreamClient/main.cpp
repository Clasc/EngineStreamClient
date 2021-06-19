#include "StreamerClient.h"
#include <SDL.h>

#undef main

int main(int argc, char const *argv[])
{
    StreamerClient client = StreamerClient();
	client.init();

	
	int width = 640, height = 480;

	SDL_Init(SDL_INIT_EVERYTHING);

	auto win = SDL_CreateWindow("Stream Client", 
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height, 
		0
	);

	bool running = true;

	auto renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	auto streamTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_UYVY, SDL_TEXTUREACCESS_STREAMING, width, height);
	char buffer[65535];


	while (running) {

		client.receiveDecode(buffer, [&](AVFrame * frame) {
			if (frame == NULL) 
			{
				printf("frame is null, continue to next frame\n\n");
				return;
			}	

			auto success = SDL_UpdateYUVTexture(
				streamTexture,
				NULL,
				frame->data[0],
				frame->linesize[0],
				frame->data[1],
				frame->linesize[1],
				frame->data[2],
				frame->linesize[2]
			);

			if (success == -1) {
				printf("error filling streamtexture\n\n");
				return;
			}
		});

		SDL_Event e;
		if (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				running = false;
			}
		}
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, streamTexture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyTexture(streamTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);

	SDL_Quit();
    return 0;
}
