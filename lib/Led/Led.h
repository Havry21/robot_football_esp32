#pragma once

#include <Adafruit_NeoPixel.h>

enum BlinkMode
{
    NONE,
    BLINK,
    MONO,
};

void blink_task(void* pvParameter);

class Led
{
public:
    static Led* GetInstance(void)
    {
        static Led m_instance;
        return &m_instance;
    }

    void set(BlinkMode _mode, uint32_t _color)
    {
        if (mode != _mode && _color != color)
        {
            mode = _mode;
            color = _color;
            needUpdate = true;
        }
    };
    void thread();

private:
    Led();
    ~Led() = default;
    Adafruit_NeoPixel led;
    uint32_t color;
    BlinkMode mode;
    bool needUpdate = false;
};
