#include "usb.hpp"

extern "C" {
#include "tusb.h"
}

#include <cstring>
#include <cstdarg>
#include <cstdio>

// Static member definitions
uint8_t USBTransport::buffer[BUFFER_SIZE];
volatile size_t USBTransport::head = 0;
volatile size_t USBTransport::tail = 0;

void USBTransport::init()
{
    head = 0;
    tail = 0;
}

bool USBTransport::connected()
{
    return tud_cdc_connected();
} 

bool USBTransport::isEmpty()
{
    return head == tail;
}

bool USBTransport::isFull()
{
    return ((head + 1) % BUFFER_SIZE) == tail;
}

void USBTransport::write(const uint8_t* data, size_t len)
{
    for (size_t i = 0; i < len; i++) {
        if (isFull()) {
            // Drop data if buffer is full (can be changed to overwrite policy)
            return;
        }

        buffer[head] = data[i];
        head = (head + 1) % BUFFER_SIZE;
    }
}

void USBTransport::writeString(const char* str)
{
    write(reinterpret_cast<const uint8_t*>(str), strlen(str));
}

void USBTransport::printf(const char* fmt, ...)
{
    char temp[256];

    va_list args;
    va_start(args, fmt);
    vsnprintf(temp, sizeof(temp), fmt, args);
    va_end(args);

    writeString(temp);
}

void USBTransport::task()
{
    tud_task();

    if (!connected()) return;

    while (!isEmpty()) {

        uint32_t available = tud_cdc_write_available();
        if (available == 0) break;

        uint32_t count = 0;
        uint8_t temp[64];  // match USB packet size

        while (!isEmpty() && count < sizeof(temp) && count < available) {
            temp[count++] = buffer[tail];
            tail = (tail + 1) % BUFFER_SIZE;
        }

        tud_cdc_n_write(0, temp, count);
    }

    tud_cdc_write_flush();
}