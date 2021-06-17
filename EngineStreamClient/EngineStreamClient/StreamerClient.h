#include "UdpReceiver.h"
#include <iostream>

#define PORT 5000
#define ADDRESS "127.0.0.1"

class StreamerClient
{
private:
    UdpReceiver m_receiver;
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
}

StreamerClient::~StreamerClient()
{
}

void StreamerClient::receiveAndEncode()
{
	char message[1000];
    double ptime;

	m_receiver.init(PORT, ADDRESS);

	while (true) {
		m_receiver.receive(message, &ptime);
		printf("received a message! \n");
		printf(message);
	}

    m_receiver.closeSock();
    return;
    // m_encoder.setupContexts(WIDTH, HEIGHT);

    // auto filename = "out/media/stream.mpeg";
    // FILE *file = fopen(filename, "wb");
    // if (!file)
    // {
    //     fprintf(stderr, "could not open %s\n", filename);
    //     return nullptr;
    // }
    // auto pos = 0;
    // m_encoder.saveImageBufferToFile(data, file, pos);
}
