#include "StreamerClient/StreamerClient.h"

int main(int argc, char const *argv[])
{
    StreamerClient client = StreamerClient();
    client.receiveAndEncode();
    return 0;
}
