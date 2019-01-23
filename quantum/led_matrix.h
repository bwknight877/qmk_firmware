/* Copyright 2018 Clueboard
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#include <stdint.h>
#include <stdbool.h>
#include "color.h"
#include "quantum.h"

typedef struct Point {
	uint8_t x;
	uint8_t y;
} __attribute__((packed)) Point;

typedef struct led_matrix {
	union {
		uint8_t raw;
		struct {
			uint8_t row:4; // 16 max
			uint8_t col:4; // 16 max
		};
	} matrix_co;
	Point point;
	uint8_t modifier:1;
} __attribute__((packed)) led_matrix;


extern const led_matrix g_leds[DRIVER_LED_TOTAL];

typedef struct
{
	HSV color;
	uint8_t index;
} rgb_indicator;

typedef union {
  uint32_t raw;
  struct {
    bool     enable  :1;
    uint8_t  mode    :6;
    uint16_t hue     :9;
    uint8_t  sat     :8;
    uint8_t  val     :8;
    uint8_t  speed   :8;//EECONFIG needs to be increased to support this
  };
} led_config_t;

enum led_matrix_effects {
	  LED_MATRIX_SOLID_COLOR = 1,
	  // All new effects go above this line
    LED_MATRIX_EFFECT_MAX
};

void rgb_matrix_set_color( int index, uint8_t red, uint8_t green, uint8_t blue );
void rgb_matrix_set_color_all( uint8_t red, uint8_t green, uint8_t blue );

// This runs after another backlight effect and replaces
// colors already set
void led_matrix_indicators(void);
void led_matrix_indicators_kb(void);
void led_matrix_indicators_user(void);

void led_matrix_init(void);
void led_matrix_setup_drivers(void);

void led_matrix_set_suspend_state(bool state);
void led_matrix_set_indicator_state(uint8_t state);


void led_matrix_task(void);

// This should not be called from an interrupt
// (eg. from a timer interrupt).
// Call this while idle (in between matrix scans).
// If the buffer is dirty, it will update the driver with the buffer.
void led_matrix_update_pwm_buffers(void);

bool process_led_matrix(uint16_t keycode, keyrecord_t *record);

uint32_t led_matrix_get_tick(void);

void led_matrix_toggle(void);
void led_matrix_enable(void);
void led_matrix_enable_noeeprom(void);
void led_matrix_disable(void);
void led_matrix_disable_noeeprom(void);
void led_matrix_step(void);
void led_matrix_set_value(uint8_t val);
void led_matrix_set_value_noeeprom(uint8_t val);
void led_matrix_step_reverse(void);
void led_matrix_increase_val(void);
void led_matrix_decrease_val(void);
void led_matrix_increase_speed(void);
void led_matrix_decrease_speed(void);
void led_matrix_mode(uint8_t mode);
void led_matrix_mode_noeeprom(uint8_t mode);
uint8_t led_matrix_get_mode(void);

#ifndef BACKLIGHT_ENABLE
#define backlight_toggle() backlight_matrix_toggle()
#define backlight_enable() backlight_matrix_enable()
#define backlight_enable_noeeprom() backlight_matrix_enable_noeeprom()
#define backlight_disable() backlight_matrix_disable()
#define backlight_disable_noeeprom() backlight_matrix_disable_noeeprom()
#define backlight_step() backlight_matrix_step()
#define backlight_set_value(val) backlight_matrix_set_value(val)
#define backlight_set_value_noeeprom(val) backlight_matrix_set_value_noeeprom(val)
#define backlight_step_reverse() backlight_matrix_step_reverse()
#define backlight_increase_val() backlight_matrix_increase_val()
#define backlight_decrease_val() backlight_matrix_decrease_val()
#define backlight_increase_speed() backlight_matrix_increase_speed()
#define backlight_decrease_speed() backlight_matrix_decrease_speed()
#define backlight_mode(mode) backlight_matrix_mode(mode)
#define backlight_mode_noeeprom(mode) backlight_matrix_mode_noeeprom(mode)
#define backlight_get_mode() backlight_matrix_get_mode()
#endif

typedef struct {
    /* Perform any initialisation required for the other driver functions to work. */
    void (*init)(void);

    /* Set the brightness of a single LED in the buffer. */
    void (*set_value)(int index, uint8_t value);
    /* Set the brightness of all LEDS on the keyboard in the buffer. */
    void (*set_value_all)(uint8_t value);
    /* Flush any buffered changes to the hardware. */
    void (*flush)(void);
} led_matrix_driver_t;

extern const led_matrix_driver_t led_matrix_driver;

#endif
