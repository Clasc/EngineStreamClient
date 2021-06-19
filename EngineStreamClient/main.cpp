#include "StreamerClient.h"

#undef main

int main(int argc, char const *argv[])
{
    StreamerClient client = StreamerClient();
	client.init();
	client.start();
    return 0;
}
