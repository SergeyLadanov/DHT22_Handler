#ifndef __MQTT_CLIENT_HPP_
#define __MQTT_CLIENT_HPP_


#include "TLS_Client.hpp"
#include <pthread.h>

class MQTT_Client : public TLS_Client::IObserver
{
public:
    class IObserver
    {
    public:
        virtual void MQTT_OnConnected(MQTT_Client *obj) = 0;
        virtual void MQTT_OnDisconnected(MQTT_Client *obj) = 0;
        virtual void MQTT_OnReceived(MQTT_Client *obj, char *topic,  char *payload, uint16_t len) = 0;
        virtual void MQTT_PollConnection(MQTT_Client *obj) = 0;
    };
private:

    typedef enum
    {
        STATE_NO_CON = 0,
        STATE_CONN_ACK
    }ConnStates;

    struct DataStruct
    {
        TLS_Client Tcp;
        bool KeepLooping = false;
        char Host[32];
        uint16_t Port = 0;
        pthread_mutex_t Mutex;
    };

    ConnStates State = STATE_NO_CON;
    char Username[32];
    char Password[32];
    uint8_t UseLwt = 0;
    char WillTopic[32];
    char WillMsg[32];
    uint8_t WillRetained = 1;
    uint8_t WillQos = 0;
    pthread_t KeepConnectionTask;
    DataStruct Data;
    IObserver *Observer = nullptr;
    uint16_t KeepAlive = 120;
    uint32_t PingSendDelay = 0;
    char Id[32];
public:
    MQTT_Client(const char *id = nullptr)
    {
        if (id)
        {
            snprintf(Id, sizeof(Id), id);
        }
        
        Data.Tcp.BindObserver(this);
    }

    void SetId(const char *id);
    void SetKeepAlive(uint16_t val);
    bool Subscribe(char *topic, int msgid = 1);
    bool Publish(char *topic, unsigned char retained, char* payload);
    bool Publish(char *topic, unsigned char retained, char* payload, int payloadlen);
    bool Begin(const char *host, uint16_t port, const char *username, const char *password, const char* willTopic = nullptr, uint8_t willQos = 0, uint8_t willRetain = 0, const char* willMessage = nullptr);
    void BindObserver(IObserver *obj);
    void Stop(void);
private:
    void OnTcpReceived(TLS_Client *obj, uint8_t *buf, uint32_t len) override;
    void OnTcpConnected(TLS_Client *obj) override;
    void OnTcpDisconnected(TLS_Client *obj) override;
    void TcpPollConnectionl(TLS_Client *obj) override;
};


#endif