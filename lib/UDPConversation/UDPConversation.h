#pragma once
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "AsyncUDP.h"
#include <WiFi.h>

typedef enum
{
    RED_C,
    BLUE_C
} Commmand_t;

struct msg3Data
{
    int16_t motorSpeed[3];
    uint8_t kickerState;
};

struct msg2Data
{
    uint8_t signal = 0x2;
    uint8_t address;
    uint8_t betteryPrecent;
};

class UDPConversation
{
public:
    UDPConversation(uint8_t _addr, Commmand_t _command);
    ~UDPConversation() {};

    static void parsePacket(AsyncUDPPacket packet);
    static void thread(void* pvParameter);
    int16_t getMotorSpeed(size_t motoId);
    bool getKickState() { return msg3.kickerState; };
    void wifiAct(void* pvParameter);

private:
    const char* ssid = "MAS";
    const char* password = "getthefuckoff";

    uint8_t addr = 0;
    static inline msg3Data msg3;
    static inline Commmand_t command;
    static inline IPAddress remoteIP;
    static inline IPAddress raspIP;
    static inline AsyncUDP udpBroadCast;
    static inline AsyncUDP udpLocal;

    // const uint32_t m_broadCastPort = 5006;
    static const inline uint32_t m_localPort = 5005;
};