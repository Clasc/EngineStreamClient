#include "UdpReceiver.h"
#include <iostream>
#include "Decoder.h"

#define PORT 5000
#define ADDRESS "127.0.0.1"

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
    void receiveAndEncode();
};

StreamerClient::StreamerClient()
{
    m_receiver = UdpReceiver();
	m_decoder = Decoder();
}

StreamerClient::~StreamerClient()
{
}

void StreamerClient::receiveAndEncode()
{
	char message[1000];
    double ptime;

	m_receiver.init(PORT, ADDRESS);
	m_decoder.setupContexts(WIDTH, HEIGHT);
	printf("ffmpeg is set up \n");
	
	while (true) {
		m_receiver.receive(message, &ptime);
		printf("received a message! \n");
		m_decoder.decode(message);
		auto pos = 0;
	}

    m_receiver.closeSock();
    return;
}
