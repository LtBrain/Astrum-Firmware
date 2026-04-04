#include "app.hpp"
#include "USB/usb.hpp"
#include "system_stm32h5xx.h"

extern "C" {
#include "tusb.h"
#include "main.h"   
}

// STARTUP CODE
void app_init()
{
    // Startup USB
    USBTransport::init();
}

// Runs at whatever frequency the CPU is at 
void app_loop()
{
    USBTransport::task();

    static uint32_t lastTime = 0;
    static uint32_t counter = 0;
    counter++;

    if (HAL_GetTick() - lastTime >= 1000)
    {
        lastTime = HAL_GetTick();

        if (USBTransport::connected())
        {
            USBTransport::printf("Loop/sec: %lu, CPU: %lu\r\n", counter, SystemCoreClock);
            counter = 0;
        }
    }
}