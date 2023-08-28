#ifndef __TCP_CONTROLLER_HPP_
#define __TCP_CONTROLLER_HPP_

#include "TCP_ServerApplication.hpp"
#include "Model.hpp"

class TCP_Controller : public TCP_ServerApplication::IObserver
{
private:
    static constexpr uint32_t SEND_BUL_LEN = 1536;
    Model &ModelRef;
    char SendBuf[SEND_BUL_LEN];
public:
    TCP_Controller(Model &model)
        :ModelRef(model)
    {

    }
private:
    // Метод получения пакета со значениями
    void GetPacket(char *buf, uint32_t len);
    void OnTcpConnected(TCP_ServerApplication *obj) override;
};

#endif