#include "UDP/UdpReceiver.h"

int main(int argc, char const *argv[])
{
    /* code */
    auto receiver = UdpReceiver();
    char *data;
    double *ptime = new double(8);

    receiver.receive(data, 5, ptime);

    delete ptime;

    return 0;
}
