#include "lsm6dsr_reg.h"
#include <stdint.h>

extern "C" {
#include "main.h"
#include "custom_motion_sensors.h"
#include "custom_motion_sensors_ex.h"
#include "motion_sensor.h"

#include "custom_bus.h"
#include "lsm6dsr.h"
}

#include "imu.hpp"
#include <cstdint>
#include <cmath> 
#include <complex>

void IMU::accInit() {
    CUSTOM_MOTION_SENSOR_Init(0, MOTION_ACCELERO);
}

void IMU::gyroInit() {
    CUSTOM_MOTION_SENSOR_Init(0, MOTION_GYRO);
}

void IMU::init() {
    int32_t status = CUSTOM_MOTION_SENSOR_Init(0, MOTION_ACCELERO | MOTION_GYRO);

    if (status != BSP_ERROR_NONE) {
        // USBTransport::printf("IMU INIT FAILED: %ld\r\n", status);
    }
}


bool IMU::connected() {
    uint8_t id;
    CUSTOM_MOTION_SENSOR_ReadID(0, &id);
    return (id == 0x6B);
}

void IMU::setAccRange(AccRange range) {
    int32_t accValue;
    switch (range) {
        case RANGE_4G:  accValue = LSM6DSR_4g;  break;
        case RANGE_8G:  accValue = LSM6DSR_8g;  break;
        case RANGE_16G: accValue = LSM6DSR_16g; break;
        case RANGE_2G:  
        default:        accValue = LSM6DSR_2g;  break;
    }

    CUSTOM_MOTION_SENSOR_SetFullScale(0, MOTION_ACCELERO, accValue);
}

void IMU::setGyroRange(GyroRange range) {
    int32_t gyroValue;
    switch (range) {
        case GYRO_RANGE_250DPS:  gyroValue = LSM6DSR_250dps;  break;
        case GYRO_RANGE_500DPS:  gyroValue = LSM6DSR_500dps;  break;
        case GYRO_RANGE_1000DPS: gyroValue = LSM6DSR_1000dps; break;
        case GYRO_RANGE_2000DPS: gyroValue = LSM6DSR_2000dps; break;
        case GYRO_RANGE_125DPS:  
        default:                 gyroValue = LSM6DSR_125dps;  break;
    }
    
    CUSTOM_MOTION_SENSOR_SetFullScale(0, MOTION_GYRO, gyroValue);
}

void IMU::setAccRate(AccRate rate) {
    int32_t accRate;
    switch (rate) {
        case ACC_RATE_OFF:      accRate = LSM6DSR_XL_ODR_OFF ;   break;
        case ACC_RATE_12Hz5:    accRate = LSM6DSR_XL_ODR_12Hz5;  break;
        case ACC_RATE_26Hz:     accRate = LSM6DSR_XL_ODR_26Hz;   break;
        case ACC_RATE_52Hz:     accRate = LSM6DSR_XL_ODR_52Hz;   break;
        case ACC_RATE_104Hz:    accRate = LSM6DSR_XL_ODR_104Hz;  break;
        case ACC_RATE_208Hz:    accRate = LSM6DSR_XL_ODR_208Hz;  break;
        case ACC_RATE_416Hz:    accRate = LSM6DSR_XL_ODR_416Hz;  break;
        case ACC_RATE_833Hz:    accRate = LSM6DSR_XL_ODR_833Hz;  break;
        case ACC_RATE_1666Hz:   accRate = LSM6DSR_XL_ODR_1666Hz; break;
        case ACC_RATE_3332Hz:   accRate = LSM6DSR_XL_ODR_3332Hz; break;
        case ACC_RATE_6667Hz:   accRate = LSM6DSR_XL_ODR_6667Hz; break;
        case ACC_RATE_1Hz6:     accRate = LSM6DSR_XL_ODR_1Hz6;   break;
        default:                accRate = LSM6DSR_XL_ODR_OFF;    break;
    }
    CUSTOM_MOTION_SENSOR_SetOutputDataRate(0, MOTION_ACCELERO, accRate);
}

void IMU::setGyroRate(GyroRate rate) {
    int32_t gyroRate;
    switch (rate) {
        case GYRO_RATE_OFF:      gyroRate = LSM6DSR_GY_ODR_OFF ;   break;
        case GYRO_RATE_12Hz5:    gyroRate = LSM6DSR_GY_ODR_12Hz5;  break;
        case GYRO_RATE_26Hz:     gyroRate = LSM6DSR_GY_ODR_26Hz;   break;
        case GYRO_RATE_52Hz:     gyroRate = LSM6DSR_GY_ODR_52Hz;   break;
        case GYRO_RATE_104Hz:    gyroRate = LSM6DSR_GY_ODR_104Hz;  break;
        case GYRO_RATE_208Hz:    gyroRate = LSM6DSR_GY_ODR_208Hz;  break;
        case GYRO_RATE_416Hz:    gyroRate = LSM6DSR_GY_ODR_416Hz;  break;
        case GYRO_RATE_833Hz:    gyroRate = LSM6DSR_GY_ODR_833Hz;  break;
        case GYRO_RATE_1666Hz:   gyroRate = LSM6DSR_GY_ODR_1666Hz; break;
        case GYRO_RATE_3332Hz:   gyroRate = LSM6DSR_GY_ODR_3332Hz; break;
        case GYRO_RATE_6667Hz:   gyroRate = LSM6DSR_GY_ODR_6667Hz; break;
        default:                 gyroRate = LSM6DSR_GY_ODR_OFF;    break;
    }
    CUSTOM_MOTION_SENSOR_SetOutputDataRate(0, MOTION_GYRO, gyroRate);
}

void IMU::readRawAcc(float &x, float &y, float &z) {
    CUSTOM_MOTION_SENSOR_Axes_t axes;
    CUSTOM_MOTION_SENSOR_GetAxes(0, MOTION_ACCELERO, &axes);
    x = axes.x;
    y = axes.y;
    z = axes.z;
}

void IMU::readRawGyro(float &x, float &y, float &z) {
    CUSTOM_MOTION_SENSOR_Axes_t axes;
    CUSTOM_MOTION_SENSOR_GetAxes(0, MOTION_GYRO, &axes);
    x = axes.x;
    y = axes.y;
    z = axes.z;
}

// float IMU::readTemp(stmdev_ctx_t &dev_ctx) {
//     int16_t temp_raw;
//     lsm6dsr_temperature_raw_get(&dev_ctx, &temp_raw);

//     return (temp_raw / 256.0f) + 25.0f;
// }
