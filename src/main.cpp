#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include "sdkconfig.h"

#include <Arduino.h>
#include <WiFi.h>
#include "AsyncUDP.h"

#include "UDPConversation.h"

class Motor
{
public:
    Motor(uint8_t _pwnPin, std::pair<uint8_t, uint8_t> _dirPin)
        : pwmPin(_pwnPin), dirPin(_dirPin) {};

    void setSpeed(int16_t sp)
    {
        if (sp > 256 && sp < -256)
        {
            return;
        }

        bool sign = sp > 0;
        digitalWrite(dirPin.first, sign);
        digitalWrite(dirPin.second, ~sign);
        analogWrite(pwmPin, std::abs(sp));
    };

private:
    const uint8_t pwmPin = 0;
    const std::pair<uint8_t, uint8_t> dirPin;
};

void kick(bool state)
{
    static auto prevState = state;
    if (state && ~prevState)
    {
        digitalWrite(47, HIGH);
        delay(2);
        digitalWrite(47, LOW);
        prevState = state;
    }
};

uint8_t adrr = 2;
Commmand_t command = adrr > 5 ? RED_C : BLUE_C;
auto conversetion = UDPConversation(adrr, command);
std::array<Motor, 3> motors {{{3, {5, 6}}, {16, {15, 7}}, {8, {18, 17}}}};

void setup()
{
    Serial.begin(115200);
}

void loop()
{
    for (size_t i = 0; i < 3; i++)
    {
        motors[i].setSpeed(conversetion.getMotorSpeed(i));
    }
    kick(conversetion.getKickState());
    delay(10);
}