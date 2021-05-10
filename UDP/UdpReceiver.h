
extern "C"
{
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
}

#ifndef UDP_RECEIVER_H
#define UDP_RECEIVER_H

class UdpReceiver
{
public:
	int sock;
	struct sockaddr_in addr;
	char *recbuffer;
	bool leftover;

	UdpReceiver();
	~UdpReceiver() { delete recbuffer; };
	void init(int port);
	int receive(char *buffer, int len, double *ptime);
	int receive(char *buffer, int len, char *tag, double *ptime);
	void closeSock();
};
#endif