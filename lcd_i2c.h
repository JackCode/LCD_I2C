#ifndef __LCD_I2C__
#define __LCD_I2C__

#include <stdint.h>

#define DEFAULT_DEVICE_ADDRESS  0x27

/* Commands */
#define CLEAR_DISPLAY			0x01
#define RETURN_HOME				0x02
#define SET_ENTRY_MODE  		0x04
#define DISPLAY_CONTROL			0x08
#define CURSOR_DISPLAY_SHIFT	0x10
#define SET_FUNCTION			0x20
#define SET_CGRAM_ADDRESS		0x40
#define SET_DDRAM_ADDRESS		0x80

/* Entry Mode Flags */
#define ENTRYRIGHT          0x00
#define ENTRYLEFT           0x02
#define ENTRYSHIFTINCREMENT 0x01
#define ENTRYSHIFTDECREMENT 0x00

/* Display Control Flags */
#define DISPLAYON   0x04
#define DISPLAYOFF  0x00
#define CURSORON    0x02
#define CURSOROFF   0x00
#define BLINKON     0x01
#define BLINKOFF    0x00

/* Cursor and Display Shift Flags */
#define DISPLAYMOVE 0x08
#define CURSORMOVE  0x00
#define MOVERIGHT   0x04
#define MOVELEFT    0x00

/* Set Function Flags */
#define F_8BITMODE    0x10
#define F_4BITMODE    0x00
#define F_2LINE       0x08
#define F_1LINE       0x00
#define F_5x10DOTS    0x04
#define F_5x8DOTS     0x00

/* Backlight Flags */
#define BACKLIGHT_ON    0X08
#define BACKLIGHT_OFF	0X00

/* En/R/W Bits */
#define En 0b00000100  // Enable bit
#define Rw 0b00000010  // Read/Write bit
#define Rs 0b00000001  // Register select bit

typedef struct I2C_Interface {
	void (*send_start_condition_and_7bit_address)(uint8_t address);
	void (*send_one_byte_of_data)(uint8_t data);
	void (*send_stop_condition)(void);
	void (*micro_sec_timer)(uint32_t microseconds);
} I2C_Interface;

struct lcd_i2c {
	void (*init)(struct I2C_Interface);
    void (*init_with_device_address)(struct I2C_Interface, uint8_t lcd_address);
	void (*write)(uint8_t value);
	void (*clear)(void);
	void (*home) (void);
	void (*turn_off_backlight)(void);
	void (*turn_on_backlight)(void);
	void (*turn_on_blinking_cursor)(void);
	void (*turn_off_blinking_cursor)(void);
	void (*scroll_right)(void);
	void (*scroll_left)(void);
	void (*set_cursor_position)(uint8_t row, uint8_t col);
	void (*hide_display)(void);
	void (*show_display)(void);
	void (*turn_on_underline_cursor)(void);
	void (*turn_off_underline_cursor)(void);
	void (*set_left_to_right)(void);
	void (*set_right_to_left)(void);
	void (*turn_on_auto_scroll)(void);
	void (*turn_off_auto_scroll)(void);
	void (*create_custom_character)(uint8_t location, uint8_t charmap[]);
};

extern const struct lcd_i2c LCD_I2C;

#endif
