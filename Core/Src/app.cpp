#include "app.hpp"
#include "Systems/USB/usb.hpp"
#include "Systems/IMU/imu.hpp"

#include "system_stm32h5xx.h"

extern "C" {
#include "tusb.h"
#include "main.h"   
#include "custom_motion_sensors.h"
}

// STARTUP CODE
void app_init()
{
    // Startup USB
    USBTransport::init();

    // Startup IMU
    IMU::init();
    IMU::setAccRate(IMU::ACC_RATE_104Hz);
    IMU::setGyroRate(IMU::GYRO_RATE_104Hz);
    IMU::setAccRange(IMU::RANGE_16G);
    IMU::setGyroRange(IMU::GYRO_RANGE_2000DPS);

    CUSTOM_MOTION_SENSOR_Enable(0, MOTION_ACCELERO);
    CUSTOM_MOTION_SENSOR_Enable(0, MOTION_GYRO);
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

            float ax, ay, az;
            float gx, gy, gz;

            uint8_t id;
            CUSTOM_MOTION_SENSOR_ReadID(0, &id);

            IMU::readRawAcc(ax, ay, az);
            IMU::readRawGyro(gx, gy, gz);

            // --- Combined debug print ---
            USBTransport::printf(
            "%lu,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%u\r\n",
                counter,
                ax, ay, az,
                gx, gy, gz,
                id
            );
            counter = 0;
    }
}