#ifndef __TCPSERVER_APPLICATION_HPP_
#define __TCPSERVER_APPLICATION_HPP_

#include <cstdint>



class TCP_ServerApplication
{
public:
    class IObserver
    {
    public:
        virtual void OnTcpConnected(void) = 0;
    };

    static void Init(int port);
    static void BindObserver(IObserver *observer);
    static int SendData(uint8_t *buf, int len);
    static void Handle(void);
private:

    static void Notify(void);

    static int listenfd, connfd;
    static IObserver *Observer;
};

#endif