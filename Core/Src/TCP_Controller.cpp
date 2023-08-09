#include "TCP_Controller.hpp"
#include <cstring>
#include <cstdio>



void TCP_Controller::OnTcpConnected(void)
{
    ModelRef.GetTextData(SendBuf, sizeof(SendBuf));
    ModelRef.SendTextData(SendBuf, strlen(SendBuf));
}