# LCD_I2C
Liquid Crystal Display (HD44780) library in C with injected I2C API to decouple from a required MCU.

## I2C Requirements
The library is based on the use of a [PCF8574](https://www.nxp.com/docs/en/data-sheet/PCF8574_PCF8574A.pdf) 8-bit I/O I2C expander.

The library requires the following I2C functions to be provided at time of initialization:
  * Send start condition and 7-bit target device address
  * Send one byte of data
  * Send stop condition
  * Delay timer measured in microseconds

Any I2C setup must have already been completed (i.e. initalized) before initializing the LCD.

## How to Use LCD_I2C
  1. Download the .h and .c files to your project
  2. Include the LCD_I2C header in your project
  
      ```C
        #include "lcd_i2c.h"
	  ```
      
  3. Initialize your I2C interface on your MCU
  
      ```C
	    my_mcu_i2c_init();

  4. Create an ```I2C_Interface``` struct and assign the appropriate functions from your mcu
  
      ```C
      	struct I2C_Interface i2c = {
		      .send_one_byte_of_data = mcu_transmit_byte_function,
		      .send_start_condition_and_7bit_address = mcu_begin_transmission_function,
		      .send_stop_condition = mcu_end_transmission_function,
		      .micro_sec_timer = mcu_microsecond_delay_timer_function
          };

  5. Call LCD_I2C.init and pass in the i2c struct and an optional device address (default is 0x27)
  
  	  ```C
     	LCD_I2C.init(i2c);

	or

        LCD_I2C.init(i2c, 0x28);

  6. Start using the LCD_I2C library. Note: The LCD initializes to the display, cursors, and backlight off, so you need to turn them on to see any written characters.

  	  ```C
		LCD_I2C.write('H');
		LCD_I2C.write('i');
		LCD_I2C.write('!');
		LCD_I2C.turn_on_backlight();
		LCD_I2C.show_display();
