#include "UdpReceiver.h"
#include "Decoder.h"
#include <time.h>
#include <functional>
#include <SDL.h>
#include <iostream>

#define PORT 5000
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

class StreamerClient
{
private:
    UdpReceiver m_receiver;
	Decoder m_decoder;
    const int WIDTH = 800;
    const int HEIGHT = 600;

public:
    StreamerClient();
    ~StreamerClient();
	void start();
	void receiveDecode(char* buffer, std::function<void(AVFrame* frame)> callback);
};

StreamerClient::StreamerClient()
{
    m_receiver = UdpReceiver();
	m_decoder = Decoder();

	try {
		m_receiver.init(PORT);
	}
	catch (std::system_error err) {
		printf(err.what());
	}

	m_decoder.setupContexts(WIDTH, HEIGHT);
	printf("ffmpeg is set up \n");
}

StreamerClient::~StreamerClient()
{
	m_receiver.closeSock();
}

void StreamerClient::receiveDecode(char* buffer, std::function<void(AVFrame* frame)> callback)
{
	double ptime = clock() / (double)CLOCKS_PER_SEC;
	try {
		auto ret = m_receiver.receive(buffer,&ptime);
		printf("received a message! \n");
		if (ret < 0) 
		{
			return;
		}

		m_decoder.decode(buffer, ret, callback);
	}

	catch (std::system_error err) {
		printf(err.what());
		return;
	}
	catch (std::runtime_error err) {
		printf(err.what());
		return;
	}
}

void StreamerClient::start()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	auto win = SDL_CreateWindow("Stream Client",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		0
	);

	bool running = true;

	auto renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	auto streamTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);
	char buffer[65535];


	while (running) {

		receiveDecode(buffer, [&](AVFrame * frame) {
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

			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, streamTexture, NULL, NULL);
			SDL_RenderPresent(renderer);
			});

		SDL_Event e;
		if (SDL_PollEvent(&e)) {
			if (e.key.keysym.sym == SDLK_ESCAPE) {
				running = false;
			}
		}
	}

	SDL_DestroyTexture(streamTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);

	SDL_Quit();	
}
