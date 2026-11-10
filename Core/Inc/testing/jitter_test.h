#ifdef DIAG

#define MARK_BMP_HIGH() (GPIOA->BSRR = GPIO_PIN_6)
#define MARK_BMP_LOW() (GPIOA->BSRR = (uint32_t)GPIO_PIN_6 << 16)
#define MARK_GYRO_HIGH() (GPIOA->BSRR = GPIO_PIN_7)
#define MARK_GYRO_LOW() (GPIOA->BSRR = (uint32_t)GPIO_PIN_7 << 16)
#define MARK_BUS_HIGH() (GPIOA->BSRR = GPIO_PIN_8)
#define MARK_BUS_LOW() (GPIOA->BSRR = (uint32_t)GPIO_PIN_8 << 16)

#endif