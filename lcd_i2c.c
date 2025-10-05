#include "lcd_i2c.h"
#include <stdbool.h>
#include <stdint.h>

static struct I2C_Interface i2c;
static uint8_t device_address = DEFAULT_DEVICE_ADDRESS;
static uint8_t current_backlight_state = BACKLIGHT_OFF;
static uint8_t current_display_state = DISPLAYOFF | CURSOROFF | BLINKOFF;
static uint8_t current_function_state = F_4BITMODE | F_2LINE | F_5x8DOTS;
static uint8_t current_entry_mode = ENTRYLEFT | ENTRYSHIFTDECREMENT;

static void write_expander(uint8_t data) {
    i2c.send_start_condition_and_7bit_address(device_address);
	i2c.send_one_byte_of_data(data | current_backlight_state);
	i2c.send_stop_condition();
}

static void pulse_enable_pin(uint8_t data) {
    write_expander(data | En);
	i2c.micro_sec_timer(1);

	write_expander(data & ~En);
	i2c.micro_sec_timer(1);
}

static void write4bits(uint8_t data) {
    write_expander(data);
    pulse_enable_pin(data);
}

static void send(uint8_t value, uint8_t mode) {
    uint8_t high_nib = value & 0xF0;
	uint8_t low_nib = (value << 4) & 0xF0;
	write4bits(high_nib | mode);
	write4bits(low_nib |mode);
}

static void command(uint8_t value) {
    send(value, 0);
}

static void clear() {
    command(CLEAR_DISPLAY);
    i2c.micro_sec_timer(50);
}

static void home() {
    command(RETURN_HOME);
    i2c.micro_sec_timer(50);
}

static void turn_off_backlight(void) {
    current_backlight_state = BACKLIGHT_OFF;
    write_expander(0);
}

static void turn_on_backlight(void) {
    current_backlight_state = BACKLIGHT_ON;
    write_expander(0);
}

static void turn_on_underline_cursor() {
    current_display_state |= CURSORON;
    command(DISPLAY_CONTROL | current_display_state);
}

static void turn_off_underline_cursor() {
    current_display_state &= ~CURSORON;
    command(DISPLAY_CONTROL | current_display_state);
}

static void turn_on_blinking_cursor() {
    current_display_state |= BLINKON;
	command(DISPLAY_CONTROL | current_display_state);
}

static void turn_off_blinking_cursor() {
    current_display_state &= ~BLINKON;
	command(DISPLAY_CONTROL | current_display_state);
}

static void turn_off_display() {
    current_display_state &= ~DISPLAYON;
    command(DISPLAY_CONTROL | current_display_state);
}

static void turn_on_display() {
    current_display_state |= DISPLAYON;
    command(DISPLAY_CONTROL | current_display_state);
}

static void scroll_left() {
    command(CURSOR_DISPLAY_SHIFT | DISPLAYMOVE | MOVELEFT);
}

static void scroll_right() {
    command(CURSOR_DISPLAY_SHIFT | DISPLAYMOVE | MOVERIGHT);
}

static void set_left_to_right() {
	current_entry_mode |= ENTRYLEFT;
	command(SET_ENTRY_MODE | current_entry_mode);
}

static void set_right_to_left() {
	current_entry_mode &= ~ENTRYLEFT;
	command(SET_ENTRY_MODE | current_entry_mode);
}

static void set_cursor_position(uint8_t row, uint8_t col) {
    command(SET_DDRAM_ADDRESS | ((col%40) + (row%4 * 0x40)));
}

// This will 'right justify' text from the cursor
void turn_on_auto_scroll(void) {
	current_entry_mode |= ENTRYSHIFTINCREMENT;
	command(SET_ENTRY_MODE | current_entry_mode);
}

// This will 'left justify' text from the cursor
void turn_off_auto_scroll(void) {
	current_entry_mode &= ~ENTRYSHIFTINCREMENT;
	command(SET_ENTRY_MODE | current_entry_mode);
}

static void write(uint8_t value) {
    send(value, Rs);
}

static void init(struct I2C_Interface i2c_interface) {
	// Follow steps in https://cdn.sparkfun.com/assets/9/5/f/7/b/HD44780.pdf
	// page 46
	i2c = i2c_interface;
	i2c.micro_sec_timer(500); // >40ms
	write4bits(0x03 << 4);
	i2c.micro_sec_timer(50); // >4.1ms
	write4bits(0x03 << 4);
	i2c.micro_sec_timer(1); // >100ns
	write4bits(0x03 << 4);
	i2c.micro_sec_timer(1);

	write4bits(0x02 << 4); // Set 4-bit mode

	command(SET_FUNCTION | current_function_state);
	clear();
	command(DISPLAY_CONTROL | current_display_state);
	clear();
	command(SET_ENTRY_MODE | current_entry_mode);
	home();
}

static void init_with_device_address(struct I2C_Interface i2c_interface, uint8_t lcd_address) {
    device_address = lcd_address;
    init(i2c_interface);
}

const struct lcd_i2c LCD_I2C = {
		.init = init,
        .init_with_device_address = init_with_device_address,
		.write = write,
		.clear = clear,
		.home = home,
		.turn_off_backlight = turn_off_backlight,
		.turn_on_backlight = turn_on_backlight,
		.turn_on_blinking_cursor = turn_on_blinking_cursor,
		.turn_off_blinking_cursor = turn_off_blinking_cursor,
		.scroll_left = scroll_left,
		.scroll_right = scroll_right,
		.set_cursor_position = set_cursor_position,
		.hide_display = turn_off_display,
		.show_display = turn_on_display,
		.turn_on_underline_cursor = turn_on_underline_cursor,
		.turn_off_underline_cursor = turn_off_underline_cursor,
		.set_left_to_right = set_left_to_right,
		.set_right_to_left = set_right_to_left,
		.turn_on_auto_scroll = turn_on_auto_scroll,
		.turn_off_auto_scroll = turn_off_auto_scroll
};