## Fault injection tests

Firmware: v2.0 · Board: NUCLEO-F446RE · I2C: 400 kHz

### Method

- Every scenario is repeated **N = 10** times.
- Statuses (`OK`, `SENSOR_ERROR`, `BUS_ERROR`) exist only for the two sensors (BMP280, MPU6050). All other tasks are covered by the watchdog.

### Results

| #   | Fault                                       | How it is injected                                          | Expected behaviour                                                                                                                                             | Result |
| --- | ------------------------------------------- | ----------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------ |
| 1   | BMP280 disconnected (SDA/SCL)               | Pull both wires at runtime, reconnect after 2 s             | Sensor status becomes `SENSOR_ERROR` on page 3; BMP values are marked invalid on page 1 (no stale or garbage numbers); gyro and OLED unaffected; no IWDG reset | Passed |
| 2   | MPU6050 disconnected (SDA/SCL)              | Same as #1                                                  | Same as #1 for the gyro                                                                                                                                        | Passed |
| 3   | Sensor VCC removed, SDA/SCL still connected | Disconnect only the VCC wire of the sensor (BMP, then gyro) | Same as #1/#2, and the OLED stays clean                                                                                                                        | Passed |
| 4   | Boot without sensors                        | Power up with BMP280 / MPU6050 / both unplugged             | System boots, no IWDG reset (failed init must not eat the IWDG window), missing sensors show `SENSOR_ERROR`, everything else works                             | Passed |
| 5   | Button bounce and hold                      | 20 fast presses; hold for 5 s                               | Exactly one page switch per press; holding switches only once. Measured bounce time: 30 +- 1 ms, debounce window: 30 +- 1 ms                                   | Passed |
| 6   | Task stops pinging                          | Test build: `for (;;);` inside `bmp_task`                   | `watchdog_task` stops refreshing the IWDG, reset within the IWDG timeout; after reboot page 3 shows `RC: IWDG` and `TASK: bmpTASK`                             | Passed |
| 7   | Stack overflow                              | `EXAMPLE_TEST_MODE 1`                                       | Hook fires at the next context switch; cause and task name saved in `.noinit`; reset; page 3 shows the cause and `exampleTASK`                                 | Passed |
| 8   | Heap exhaustion                             | `EXAMPLE_TEST_MODE 3`                                       | Malloc hook, then reset; page 3 shows the correct cause (not the stack-overflow one)                                                                           | Passed |
| 9   | `configASSERT` failure                      | Force `configASSERT(0)` in a test task                      | Reset; file and line survive the reset (`.noinit`) and are shown on page 3                                                                                     | Passed |
| 10  | Reset cause                                 | Cold power-up; NRST button                                  | `PO` after power-up, `PIN` after NRST                                                                                                                          | Passed |
| 11  | OLED disconnected                           | Pull the display at runtime, reconnect after 2 s            | Sensors keep sampling, no reset; after reconnect the display is re-initialised and redrawn                                                                     | Passed |
