#include "UdpReceiver.h"
#include "Decoder.h"
#include <time.h>
#include <iostream>

#define PORT 5000

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
	void init();
	void receiveAndEncode(char* buffer, AVFrame* frame);
};

StreamerClient::StreamerClient()
{
    m_receiver = UdpReceiver();
	m_decoder = Decoder();
}

StreamerClient::~StreamerClient()
{
	m_receiver.closeSock();
}

void StreamerClient::init() {
	try {
		m_receiver.init(PORT);
	}
	catch (std::system_error err) {
		printf(err.what());
	}

	m_decoder.setupContexts(WIDTH, HEIGHT);
	printf("ffmpeg is set up \n");
};

void StreamerClient::receiveAndEncode(char* buffer, AVFrame* frame)
{
	double ptime = clock() / (double)CLOCKS_PER_SEC;
	try {
		auto ret = m_receiver.receive(buffer,&ptime);
		printf("received a message! \n");
		if (ret < 0) 
		{
			return;
		}

		auto frame = av_frame_alloc();
		if (!frame)
		{
			fprintf(stderr, "Cannot alloc frame\n");
			exit(1);
		}

		m_decoder.decode(buffer, frame);
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
