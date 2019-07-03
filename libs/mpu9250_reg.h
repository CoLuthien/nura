/*
 * mpu9250 registers for internal library use
 *
 * Copyright 2016 Ryan Kurte
 */

// MPU9250 Register Enumeration
enum mpu9250_reg{

    // Self test registers
    SELF_TEST_X_GYRO = 0x00,
    SELF_TEST_Y_GYRO = 0x01,
    SELF_TEST_Z_GYRO = 0x02,
    SELF_TEST_X_ACCEL = 0x0D,
    SELF_TEST_Y_ACCEL = 0x0E,
    SELF_TEST_Z_ACCEL = 0x0F,

    // Gyro offset registers
    XG_OFFSET_H = 0x13,
    XG_OFFSET_L = 0x14,
    YG_OFFSET_H = 0x15,
    YG_OFFSET_L = 0x16,
    ZG_OFFSET_H = 0x17,
    ZG_OFFSET_L = 0x18,

    // Configuration Registers
    SMPLRT_DIV = 0x19,      //!< Sample rate divisor
    CONFIG = 0x1A,          //!< General Configuration
    GYRO_CONFIG = 0x1B,     //!< Gyro configuration
    ACCEL_CONFIG_1 = 0x1C,  //!< Accel configuration 1
    ACCEL_CONFIG_2 = 0x1D,  //!< Accel configuration 2
    LP_ACCEL_ODR = 0x1E,    //!< Low Power accel control
    WOM_THR = 0x1F,         //!< Wake on Movement threshold
    FIFO_EN = 0x23,         //!< FIFO output config, sets readings to be stored in fifo

    // I2C Slave configuration registers
    I2C_MST_CTRL = 0x24,    //!< I2C Master control

    I2C_SLV0_ADDR = 0x25,
    I2C_SLV0_REG = 0x26,
    I2C_SLV0_CTRL = 0x27,
    
    I2C_SLV1_ADDR = 0x28,
    I2C_SLV1_REG = 0x29,
    I2C_SLV1_CTRL = 0x2A,
    
    I2C_SLV2_ADDR = 0x2B,
    I2C_SLV2_REG = 0x2C,
    I2C_SLV2_CTRL = 0x2D,
    
    I2C_SLV3_ADDR = 0x2E,
    I2C_SLV3_REG = 0x2F,
    I2C_SLV3_CTRL = 0x30,
    
    I2C_SLV4_ADDR = 0x31,
    I2C_SLV4_REG = 0x32,
    I2C_SLV4_DO = 0x33,
    I2C_SLV4_CTRL = 0x34,
    I2C_SLV4_DI = 0x35,

    I2C_MST_STATUS = 0x36,  //!< I2C Master Status

    // Interrupt control registers
    INT_PIN_CFG = 0x37,     //!< Interrupt pin configuration
    INT_ENABLE = 0x38,      //!< Interrupt Enable mask
    INT_STATUS = 0x3A,      //!< Interrupt Status

    // Accelerator output registers
    ACCEL_XOUT_H = 0x3B,
    ACCEL_XOUT_L = 0x3C,
    ACCEL_YOUT_H = 0x3D,
    ACCEL_YOUT_L = 0x3E,
    ACCEL_ZOUT_H = 0x3F,
    ACCEL_ZOUT_L = 0x40,

    // Temperature output registers
    TEMP_OUT_H = 0x41,
    TEMP_OUT_L = 0x42,

    // Gyro output registers
    GYRO_XOUT_H = 0x43,
    GYRO_XOUT_L = 0x44,
    GYRO_YOUT_H = 0x45,
    GYRO_YOUT_L = 0x46,
    GYRO_ZOUT_H = 0x47,
    GYRO_ZOUT_L = 0x48,

    // External sensor output registers
    EXT_SENS_DATA_00 = 0x49,
    EXT_SENS_DATA_01 = 0x4A,
    EXT_SENS_DATA_02 = 0x4B,
    EXT_SENS_DATA_03 = 0x4C,
    EXT_SENS_DATA_04 = 0x4D,
    EXT_SENS_DATA_05 = 0x4E,
    EXT_SENS_DATA_06 = 0x4F,
    EXT_SENS_DATA_07 = 0x50,
    EXT_SENS_DATA_08 = 0x51,
    EXT_SENS_DATA_09 = 0x52,
    EXT_SENS_DATA_10 = 0x53,
    EXT_SENS_DATA_11 = 0x54,
    EXT_SENS_DATA_12 = 0x55,
    EXT_SENS_DATA_13 = 0x56,
    EXT_SENS_DATA_14 = 0x57,
    EXT_SENS_DATA_15 = 0x58,
    EXT_SENS_DATA_16 = 0x59,
    EXT_SENS_DATA_17 = 0x5A,
    EXT_SENS_DATA_18 = 0x5B,
    EXT_SENS_DATA_19 = 0x5C,
    EXT_SENS_DATA_20 = 0x5D,
    EXT_SENS_DATA_21 = 0x5E,
    EXT_SENS_DATA_22 = 0x5F,
    EXT_SENS_DATA_23 = 0x60,

    // I2C Slave Data Out
    I2C_SLV0_DO = 0x63,
    I2C_SLV1_DO = 0x64,
    I2C_SLV2_DO = 0x65,
    I2C_SLV3_DO = 0x66,

    // Configuration registers
    I2C_MST_DELAY_CTRL = 0x67,  //!< I2C Master Delay Control
    SIGNAL_PATH_RESET = 0x68,   //!< Signal Path Reset
    MOT_DETECT_CTRL = 0x69,     //!< Motion Detect Control (see ACCEL_INTEL_CTRL)
    USER_CTRL = 0x6A,           //!< User control, sets FIFO and I2C modes

    // Power management registers
    PWR_MGMT_1 = 0x6B,          //!< Power management 1
    PWR_MGMT_2 = 0x6C,          //!< Power management 2

    // FIFO status registers
    FIFO_COUNTH = 0x72,         //!< Current fifo count, high byte
    FIFO_COUNTL = 0x73,         //!< Current fifo count, low byte
    FIFO_R_W = 0x74,            //!< Register to read/write from/to the FIFO

    WHO_AM_I = 0x75,            //!< Device ID check, always 0x71

    // Accelerometer offsets
    // Full scale compensation: 15 bit 0.98-mg steps
    XA_OFFSET_H = 0x77,
    XA_OFFSET_L = 0x78,
    YA_OFFSET_H = 0x7A,
    YA_OFFSET_L = 0x7B,
    ZA_OFFSET_H = 0x7D,
    ZA_OFFSET_L = 0x7E

};

enum mpu9250_compass_reg_e {
    // Read Only
    MPU9250_COMPASS_REG_WIA     = 0x00,     //!< Device id, always 0x48
    MPU9250_COMPASS_REG_INFO    = 0x01,     //!< Device information
    MPU9250_COMPASS_REG_ST1     = 0x02,     //!< Device status 1 (lowest bit indicates data ready)
    MPU9250_COMPASS_REG_HXL     = 0x03,     //!< X axis LSB
    MPU9250_COMPASS_REG_HXH     = 0x04,     //!< X axis MSB
    MPU9250_COMPASS_REG_HYL     = 0x05,     //!< Y axis LSB
    MPU9250_COMPASS_REG_HYH     = 0x06,     //!< Y axis MSB
    MPU9250_COMPASS_REG_HZL     = 0x07,     //!< Z axis LSB
    MPU9250_COMPASS_REG_HZH     = 0x08,     //!< Z axis MSB
    MPU9250_COMPASS_REG_ST2     = 0x09,     //!< Device status 2 (magnetic overflow, output bit setting)_data
    // Read / Write
    MPU9250_COMPASS_REG_CNTL1   = 0x0A,     
    MPU9250_COMPASS_REG_CNTL2   = 0x0B,
    MPU9250_COMPASS_REG_ASTC    = 0x0C,
    MPU9250_COMPASS_REG_TS1     = 0x0D,
    MPU9250_COMPASS_REG_TS2     = 0x0E,
    MPU9250_COMPASS_REG_I2CDIS  = 0x0F,
    // Read Only
    MPU9250_COMPASS_REG_ASAX    = 0x10,
    MPU9250_COMPASS_REG_ASAy    = 0x11,
    MPU9250_COMPASS_REG_ASAZ    = 0x12
};