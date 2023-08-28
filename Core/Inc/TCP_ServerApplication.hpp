#ifndef __TCPSERVER_APPLICATION_HPP_
#define __TCPSERVER_APPLICATION_HPP_

#include <cstdint>



class TCP_ServerApplication
{
public:
    class IObserver
    {
    public:
        virtual void OnTcpConnected(TCP_ServerApplication *obj) = 0;
    };

    void Init(int port);
    void BindObserver(IObserver *observer);
    int SendData(uint8_t *buf, int len);
    void Handle(void);
private:

    void Notify(void);

    int listenfd = 0, connfd = 0;
    IObserver *Observer = nullptr;
};

#endif