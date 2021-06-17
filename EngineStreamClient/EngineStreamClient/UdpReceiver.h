#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define min(x, y) ((x) <= (y) ? (x) : (y))
#define max(x, y) ((x) >= (y) ? (x) : (y))

#if !defined(_WIN32)
#define SOCKET int
#endif

#if defined(_WIN32)
#define ISVALIDSOCKET(s) ((s) != INVALID_SOCKET)
#else
#define ISVALIDSOCKET(s) ((s) >= 0)
#endif

#ifndef UDP_RECEIVER_H
#define UDP_RECEIVER_H

class UdpReceiver
{
public:
	SOCKET sock;
	struct sockaddr_in addr;
	char *recbuffer;
	bool leftover;

	UdpReceiver();
	~UdpReceiver() { delete recbuffer; WSACleanup();};
	void init(int port);
	int receive(char *buffer, double *ptime);
	int receive(char *buffer, const char *tag, double *ptime);
	void closeSock();
};
#endif