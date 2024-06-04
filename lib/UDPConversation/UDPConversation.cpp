#include "UDPConversation.h"

UDPConversation::UDPConversation(uint8_t _addr, Commmand_t _command)
{
    addr = _addr;
    command = _command;
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("WiFi connected.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    // if (udpBroadCast.listen(m_broadCastPort))
    // {
    //     Serial.print("Init udp listen");
    //     udpBroadCast.onPacket(parsePacket);
    // }
    // else
    // {
    //     Serial.print("Error in udp listen");
    // }

    if (udpLocal.listen(m_localPort))
    {
        Serial.print("Init udp listen");
        udpLocal.onPacket(parsePacket);
    }
    else
    {
        Serial.print("Error in udp listen");
    }
    xTaskCreate(&UDPConversation::thread, "wifi_connection", 4000, NULL, 5, NULL);
}

void UDPConversation::parsePacket(AsyncUDPPacket packet)
{

    // Serial.print("UDP Packet Type: ");
    // Serial.print(packet.isBroadcast()   ? "Broadcast"
    //              : packet.isMulticast() ? "Multicast"
    //                                     : "Unicast");
    // Serial.print(", From: ");
    // Serial.print(packet.remoteIP());
    // Serial.print(":");
    // Serial.print(packet.remotePort());
    // Serial.print(", To: ");
    // Serial.print(packet.localIP());
    // Serial.print(":");
    // Serial.print(packet.localPort());
    // Serial.print(", Length: ");
    // Serial.print(packet.length());
    // Serial.print(", Data: ");
    // Serial.write(packet.data(), packet.length());
    // Serial.println();
    // packet.printf("Got %u bytes of data", packet.length());
    // remoteIP = packet.remoteIP();

    // recive msg1
    if (packet.isBroadcast())
    {
        uint8_t _command = *packet.data();
        if (_command == command)
        {
            Serial.println("Recive msg1");
            raspIP = packet.remoteIP();
            msg2Data _msg2;
            _msg2.address = 2;
            _msg2.betteryPrecent = 100;
            _msg2.signal = 0x2;
            udpBroadCast.writeTo((uint8_t*) &_msg2, sizeof(_msg2), raspIP, m_localPort);
        }
    }
    else
    {
        // recive msg3
        if (raspIP == packet.remoteIP() && packet.length() == sizeof(msg3))
        {
            Serial.println("Recive msg3");
            memcpy(&msg3, packet.data(), sizeof(msg3));
        }
        if (packet.length() == 1 && *packet.data() == 3)
        {
            Serial.println("Recive msg4");
            msg2Data _msg2;
            _msg2.address = 2;
            _msg2.betteryPrecent = 100;
            _msg2.signal = 0x2;
            udpBroadCast.writeTo((uint8_t*) &_msg2, sizeof(_msg2), raspIP, 5005);
        }
    }
}

int16_t UDPConversation::getMotorSpeed(size_t motoId)
{
    return msg3.motorSpeed[motoId];
}

void UDPConversation::thread(void* pvParameter)
{
    while (1)
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            Serial.println("Lost connection");
        }
        else
        {
            Serial.println("Wifi connected!");
        }
        delay(2000);
    }
}
