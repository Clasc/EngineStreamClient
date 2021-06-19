#include "UdpReceiver.h"
#include "Decoder.h"
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
	void receiveAndEncode(char* buffer);
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
	m_receiver.init(PORT);
	m_decoder.setupContexts(WIDTH, HEIGHT);
	printf("ffmpeg is set up \n");
};

void StreamerClient::receiveAndEncode(char* buffer)
{
    double ptime;
	m_receiver.receive(buffer, &ptime);
	printf("received a message! \n");
	m_decoder.decode(buffer);   
    return;
}
