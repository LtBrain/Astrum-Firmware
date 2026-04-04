#pragma once

#include <cstdint>
#include <cstddef>
#include <stdint.h>

class USBTransport {
public:
    // Initialize transport
    static void init();

    // Must be called frequently in main loop
    static void task();

    // Check if USB CDC is connected
    static bool connected();

    // Write raw binary data
    static void write(const uint8_t* data, size_t len);

    // Write null-terminated string
    static void writeString(const char* str);

    // printf-style formatted output
    static void printf(const char* fmt, ...);

private:
    static constexpr size_t BUFFER_SIZE = 4096;

    static uint8_t buffer[BUFFER_SIZE];
    static volatile size_t head;
    static volatile size_t tail;

    static bool isFull();
    static bool isEmpty();
};