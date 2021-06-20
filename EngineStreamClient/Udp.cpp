#include "Udp.h"
#include <time.h>

typedef struct RTHeader
{
    double time;
    unsigned long packetnum;
    unsigned char fragments;
    unsigned char fragnum;
} RTHeader_t;

Udp::Udp()
{
	leftover = false;
	WSADATA wsaData;
	auto wVersionRequested = MAKEWORD(2, 2);
	auto err = WSAStartup(wVersionRequested, &wsaData);

	if (err != 0)
	{
		printf("WSAStartup failed with error: %d\n", err);
		return;
	}

    sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
}

void Udp::init(const char* address, int port)
{
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(address);
    addr.sin_port = htons(port);
    
	int ret = bind(sock, (const sockaddr *)&addr, sizeof(addr));
	if (ret < 0) {
		throw std::system_error(WSAGetLastError(), std::system_category(), "Bind failed");
	}

    printf("Binding port %d return %d\n", port, ret);
}

int Udp::receive(char* buffer, double* ptime) {
	return receive(buffer, "", ptime);
}

int Udp::receive(char* buffer,const char* tag, double* ptime) {
	struct sockaddr_in si_other;
	socklen_t slen = sizeof(si_other);

	RTHeader_t* pheader = (RTHeader_t*)recbuffer;

	bool goon = true;
	int bytes = 0;
	int packetnum = -1;
	int fragments = -1;
	int fragnum = -1;
	int nextfrag = 1;

	while (goon) {

		int ret = 0;

		if (!leftover) {
			ret = recvfrom(sock, recbuffer, 65000, 0, (sockaddr*)& si_other, &slen);
		}

		leftover = false;

		//printf("%s UDP Packet %ld Size %d Fragment %d of %d Nextfrag %d\n", tag, pheader->packetnum, ret, pheader->fragnum, pheader->fragments, nextfrag );

		if (ret <= sizeof(RTHeader_t)) {
			printf("Fragment %d not larger than %d", pheader->fragnum, sizeof(RTHeader_t));
			throw std::system_error(WSAGetLastError(), std::system_category(), "receive failed");
		}

		if (packetnum == -1) {						//first fragment of the new packet
			packetnum = pheader->packetnum;
		}

		if (packetnum != pheader->packetnum) {		//last fragments lost
			printf("Last Frag %d lost", nextfrag);
			leftover = true;
			throw std::runtime_error("Last Frag lost");
		}

		//printf("%s UDP Packet %ld Size %d Fragment %d of %d Nextfrag %d\n", tag, pheader->packetnum, ret, pheader->fragnum, pheader->fragments, nextfrag );

		if (nextfrag != pheader->fragnum) {			//a fragment is missing
			printf("Fragment %d lost\n", nextfrag);
			throw std::runtime_error("A Fragment was lost");
		}

		nextfrag++;

		memcpy(buffer + bytes, recbuffer + sizeof(RTHeader_t), ret - sizeof(RTHeader_t));
		bytes += ret - sizeof(RTHeader_t);

		if (pheader->fragments == pheader->fragnum) goon = false;		//last fragment

		packetnum = pheader->packetnum;
		fragments = pheader->fragments;
		fragnum = pheader->fragnum;

		*ptime = pheader->time;

	}

	leftover = false;
	return bytes;
}

int Udp::send(char* buffer, int len)
{	
	packetnum++;

	if (len > 65000)
	{
		return 0;
	}
	//if( packetnum%100==50) return -1;		//test for packet loss

	int maxpacketsize = 1400;

	RTHeader_t header;
	header.time = clock() / (double)CLOCKS_PER_SEC;
	header.packetnum = packetnum;
	header.fragments = len / maxpacketsize;
	header.fragnum = 1;

	int left = len - header.fragments * maxpacketsize;

	if (left > 0)
	{
		header.fragments++;
	}

	int ret, bytes = 0;
	for (int i = 0; i < header.fragments; i++)
	{
		memcpy(sendbuffer, &header, sizeof(header));
		memcpy(sendbuffer + sizeof(header), buffer + bytes, min(maxpacketsize, len - bytes));

		ret = sendto(sock, sendbuffer, min(maxpacketsize, len - bytes) + sizeof(header), 0, (const struct sockaddr*) & addr, sizeof(addr));

		if (ret == -1)
		{
			printf("error in sending to socket: /i", sock);
			return ret;
		}

		else
		{
			ret = ret - sizeof(header);
		}
		bytes += ret;
		header.fragnum++;
	}

	return bytes;
}

void Udp::closeSock()
{
	if (sock)
	{
		closesocket(sock);
	}
	WSACleanup();
}
