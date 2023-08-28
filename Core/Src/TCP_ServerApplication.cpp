#include "TCP_ServerApplication.hpp"
#include <cstdio>

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#if defined(_WIN32) || defined(_WIN64)//Windows includes
#include <winsock.h>
#else
#include <sys/socket.h> //Add support for sockets
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif





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
    	connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);	
		printf("TCP connected\r\n");
        Notify();
		// Закрытие соединение
		printf("Close tcp...\r\n");
		
		#if defined(_WIN32) || defined(_WIN64)//Windows includes
        closesocket(connfd);
        #else
        close(connfd);
        #endif
}

void TCP_ServerApplication::BindObserver(IObserver *observer)
{
    Observer = observer;
}

int TCP_ServerApplication::SendData(uint8_t *buf, int len)
{
    return send(connfd, (char *) buf, len, 0);
}


void TCP_ServerApplication::Notify(void)
{
    if (Observer)
    {
        Observer->OnTcpConnected(this);
    }
}