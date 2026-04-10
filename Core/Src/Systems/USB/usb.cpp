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

size_t USBTransport::write(const uint8_t* data, size_t len)
{
    size_t written = 0;

    for (size_t i = 0; i < len; i++) {
        if (isFull()) break;

        buffer[head] = data[i];
        head = (head + 1) % BUFFER_SIZE;
        written++;
    }

    return written;
}

void USBTransport::writeString(const char* str)
{
    write(reinterpret_cast<const uint8_t*>(str), strlen(str));
}

void USBTransport::printf(const char* fmt, ...)
{
    char temp[512];

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

        uint8_t temp[128]; //TinyUSB automatic packet split
        uint32_t count = 0;

        while (!isEmpty() && count < sizeof(temp) && count < available) {
            temp[count++] = buffer[tail];
            tail = (tail + 1) % BUFFER_SIZE;
        }

        tud_cdc_n_write(0, temp, count);
    }

    // if (count < 64) {
        tud_cdc_write_flush();
    // }
}