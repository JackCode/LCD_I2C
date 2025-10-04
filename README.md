# LCD_I2C
Liquid Crystal Display (HD44780) library in C with injected I2C API to decouple from a required MCU.

## I2C Requirements
The library requires the following I2C functions to be provided at time of initialization:
  * Send start condition and 7-bit target device address
  * Send one byte of data
  * Send stop condition
  * Delay timer measured in milliseconds

Any I2C setup must have already been completed (i.e. initalized) before initializing the LCD.
