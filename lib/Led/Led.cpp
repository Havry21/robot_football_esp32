#include "Led.h"

Led::Led(/* args */)
{
    led = Adafruit_NeoPixel(1, 48, NEO_GRB + NEO_KHZ800);
    led.begin();
    led.show();
    xTaskCreate(&blink_task, "blink_task", 2000, this, 5, NULL);
}

void blink_task(void* pvParameter)
{
    auto ptr = (Led*) pvParameter;
    while (1)
    {
        ptr->thread();
    }
}

void Led::thread()
{
    static auto timer = millis();
    switch (mode)
    {
        case NONE:
            led.clear();
            break;
        case BLINK:
        {
            static bool toogleState = false;
            auto timeout = 300;
            if (millis() - timer > timeout)
            {
                if (toogleState)
                    led.setPixelColor(0, color);
                else
                    led.clear();
                toogleState = ~toogleState;
                timer = millis();
                needUpdate = true;
            }
        }
        break;
        case MONO:
            led.setPixelColor(0, color);
            break;
        default:
            break;
    }
    if (needUpdate)
    {
        led.show();
        needUpdate = false;
    }
    delay(100);
}