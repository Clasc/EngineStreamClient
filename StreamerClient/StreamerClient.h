#include "../UDP/UdpReceiver.h"
#include "../Encoder/Encoder.h"

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
    void *receiveAndEncode();
};

StreamerClient::StreamerClient()
{
    m_receiver = UdpReceiver();
    m_encoder = Encoder();
    m_receiver.init(8080);
}

StreamerClient::~StreamerClient()
{
    m_receiver.closeSock();
}

void *StreamerClient::receiveAndEncode()
{
    uint8_t *data;
    char *tag;
    double *ptime;

    m_receiver.receive((char *)data, WIDTH * HEIGHT, tag, ptime);
    m_encoder.setupContexts(WIDTH, HEIGHT);

    auto filename = "out/media/stream.mpeg";
    FILE *file = fopen(filename, "wb");
    if (!file)
    {
        fprintf(stderr, "could not open %s\n", filename);
        return nullptr;
    }
    auto pos = 0;
    m_encoder.saveImageBufferToFile(data, file, pos);
}
