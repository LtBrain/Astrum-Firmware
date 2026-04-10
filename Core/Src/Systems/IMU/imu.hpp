#pragma  once

#include <cstdint>
#include <cstddef>

class IMU {
public:

    enum AccRange {
        RANGE_2G  = 0, 
        RANGE_4G  = 1,
        RANGE_8G  = 2,
        RANGE_16G = 3
    };
    enum GyroRange {
        GYRO_RANGE_125DPS  = 0,
        GYRO_RANGE_250DPS  = 1,
        GYRO_RANGE_500DPS  = 2,
        GYRO_RANGE_1000DPS = 3,
        GYRO_RANGE_2000DPS = 4
    };
    enum AccRate {
        ACC_RATE_OFF       = 0,
        ACC_RATE_12Hz5     = 1,
        ACC_RATE_26Hz      = 2,
        ACC_RATE_52Hz      = 3,
        ACC_RATE_104Hz     = 4,
        ACC_RATE_208Hz     = 5,
        ACC_RATE_416Hz     = 6,
        ACC_RATE_833Hz     = 7,
        ACC_RATE_1666Hz    = 8,
        ACC_RATE_3332Hz    = 9,
        ACC_RATE_6667Hz    = 10,
        ACC_RATE_1Hz6      = 11
    };
    enum GyroRate {
        GYRO_RATE_OFF       = 0,
        GYRO_RATE_12Hz5     = 1,
        GYRO_RATE_26Hz      = 2,
        GYRO_RATE_52Hz      = 3,
        GYRO_RATE_104Hz     = 4,
        GYRO_RATE_208Hz     = 5,
        GYRO_RATE_416Hz     = 6,
        GYRO_RATE_833Hz     = 7,
        GYRO_RATE_1666Hz    = 8,
        GYRO_RATE_3332Hz    = 9,
        GYRO_RATE_6667Hz    = 10,
    };

    // Initialize LSM6DSR Accelerometer
    
    static void accInit();
    
    // Initialize LSM6DSR Gyroscope
    static void gyroInit();

    // Initialize IMU
    static void init();

    // Check if IMU is attached
    static bool connected();

    // -------------------------------------------------

    // Set accelerometer range
    static void setAccRange(AccRange range);

    // Set accelerometer data rate
    static void setAccRate(AccRate rate);

    // Set gyroscope range
    static void setGyroRange(GyroRange range);

    // Set gyroscope data rate
    static void setGyroRate(GyroRate rate);

    // -------------------------------------------------

    // Read accelerometer raw data
    static void readRawAcc(float &x, float &y, float &z);

    // Read gyroscope raw data
    static void readRawGyro(float &x, float &y, float &z);

    // Read temperature sensor
    static float readTemp();
};