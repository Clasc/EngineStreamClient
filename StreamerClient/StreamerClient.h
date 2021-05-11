#include "../UDP/UdpReceiver.h"
#include "../Encoder/Encoder.h"
#include <iostream>

#define PORT 5000
class StreamerClient
{
private:
    UdpReceiver m_receiver;
    Encoder m_encoder;
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
    m_encoder = Encoder();
}

StreamerClient::~StreamerClient()
{
}

void StreamerClient::receiveAndEncode()
{
    uint8_t *data;
    char *message = new char(10);

    double *ptime;

    m_receiver.init(PORT);
    m_receiver.receive(message, ptime);
    std::cout << message << std::endl;

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
