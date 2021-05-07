#include "UdpReceiver.h"
#include <iostream>

UdpReceiver::UdpReceiver() {}

int UdpReceiver::receive(char *buffer, int len, double *ptime)
{
    std::cout << "receive package!";
    return 0;
}