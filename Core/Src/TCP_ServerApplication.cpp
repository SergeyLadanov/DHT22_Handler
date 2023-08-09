#include "TCP_ServerApplication.hpp"


#if defined(_WIN32) || defined(_WIN64)//Windows includes
#include <winsock.h>
#else
#include <sys/socket.h> //Add support for sockets
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif



int TCP_ServerApplication::listenfd = 0;
int TCP_ServerApplication::connfd = 0;
TCP_ServerApplication::IObserver *TCP_ServerApplication::Observer = nullptr;



void TCP_ServerApplication::Init(int port)
{
    struct sockaddr_in serv_addr;
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	serv_addr.sin_port = htons(port);

	bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	listen(listenfd, 10);
}


void TCP_ServerApplication::Handle(void)
{

}

void TCP_ServerApplication::BindObserver(IObserver *observer)
{

}

int TCP_ServerApplication::SendData(uint8_t *buf, int len)
{

}


void TCP_ServerApplication::Notify(void)
{

}