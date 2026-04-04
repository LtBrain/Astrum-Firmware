#include "app.hpp"
#include "tusb.h"
#include "main.h"   // for HAL_GetTick()

void app_init()
{
    // Put any startup code here (optional)
}

void app_loop()
{
    // This runs every loop (like your old while loop)

    static uint32_t lastTime = 0;

    if (HAL_GetTick() - lastTime >= 1000) // every 1 second
    {
        lastTime = HAL_GetTick();

        // Minimal test: send something over USB
        if (tud_cdc_connected())
        {
            const char* msg = "Hello from app.cpp\r\n";
            tud_cdc_write(msg, strlen(msg));
            tud_cdc_write_flush();
        }
    }
}