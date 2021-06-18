#include "StreamerClient.h"
#include <SDL.h>
#undef main

int main(int argc, char const *argv[])
{
    StreamerClient client = StreamerClient();
	SDL_Init(SDL_INIT_EVERYTHING);
    client.receiveAndEncode();
    return 0;
}
