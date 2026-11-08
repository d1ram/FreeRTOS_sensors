#include "stm32f4xx_hal.h"

HAL_StatusTypeDef MPU6050_init(I2C_HandleTypeDef *bus);                // Initialize the MPU
HAL_StatusTypeDef MPU6050_Read_Accel(float *Ax, float *Ay, float *Az); // Read MPU Accelerator
HAL_StatusTypeDef MPU6050_Read_Gyro(float *Gx, float *Gy, float *Gz);  // Read MPU Gyroscope