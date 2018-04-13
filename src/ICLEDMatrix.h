//
// Created by matheuswhite on 19/03/18.
//

#ifndef PROJECT_ICLEDMATRIX_H
#define PROJECT_ICLEDMATRIX_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <zephyr.h>
#include <misc/printk.h>
#include <board.h>
#include <gpio.h>
#include <device.h>
#include <display/mb_display.h>

#define ENOT_INIT       1
#define EHOUR_INVALID   2
#define ENOT_ASCII      3
#define EDATE_FMT_WRONG 4
#define ESIZE_WRONG     5

/*!
 * @brief This function initialize the module: "LED Matrix"
 * @return kOK if the module has not been initialized. And, kFAIL otherwise.
 */
int IC_LED_matrix_init(void);

/*! 
 * @brief
 * This function draw a heart in the LED Matrix. This heart will remain in the
 * LED Matrix, by default, 5 seconds.
 *
 * AT+HEART
 *
 * @param data - An array of pointers of type uint8_t
 * @param size - The size of array 'data'.
 * @return kOK if the module has been initialized. And kFAIL, otherwise.
 */
int IC_LED_matrix_draw_heart(uint8_t **data, uint8_t size);

/*!
 * @
 * @brief
 * This function change the time the heart remains in the LED Matrix
 *
 * AT+TIME=<int>
 *
 * @param data - An array of pointers of type uint8_t
 * @param size - The size of array 'data'.
 * 1º PARAMETRO: seconds - A unsigned integer that represent the seconds in
 *                         which the heart will remain in the LED matrix.
 * @return kOK if the module has been initialized. And kFAIL, otherwise.
 */
int IC_LED_matrix_set_time_active(uint8_t **data, uint8_t size);

/*!
 * @at_command
 * @brief
 * This function will draw, in the LED Matrix, a hour and a minute. Each number
 * will be shown sequentially, and will stay in the LED Matrix for 1 seconds.
 * The hour will be followed by ":".
 *
 * AT+HOURS=<int>,<int>
 *
 * @param data - An array of pointers of type uint8_t
 * @param size - The size of array 'data'.
 * 1º PARAMETRO: hour - A unsigned integer that represent the hours. This value
 *                      must be less than 24.
 * 2º PARAMETRO: min - A unsigned integer that represent the minutes. This
 *                     value must be less than 60.
 * @return kOK if the module has been initialized and the conditions of above
 *         params has been met. And kFAIL, otherwise.
 */
int IC_LED_matrix_draw_hours(uint8_t **data, uint8_t size);

/*!
 * @brief
 * This function will draw, in the LED Matrix, an any string. Each character
 * will be shown sequentially, and will stay in the LED Matrix for 1 seconds.
 *
 * AT+STRING=<str>
 *
 * @param data - An array of pointers of type uint8_t
 * @param size - The size of array 'data'.
 * 1º PARAMETRO: str - A generic string. The characters of this string must be
 *                     between 0x20 and 0x7E.
 * @return kOK if the module has been initialized and the conditions of above
 *         params has been met. And kFAIL, otherwise.
 */
int IC_LED_matrix_draw_string(uint8_t **data, uint8_t size);

/*!
 * @brief
 * This function will draw, in the LED Matrix: a name and a surname. Each
 * character will be shown sequentially, and will stay in the LED Matrix for
 * 1 seconds. The name and surname will be separated by a hyphen.
 *
 * AT+NAME=<str>,<str>
 *
 * @param data - An array of pointers of type uint8_t
 * @param size - The size of array 'data'.
 * 1º PARAMETRO: name - A string that represent a name. The characters of this
 *                      string must be between 0x20 and 0x7E.
 * 2º PARAMETRO: surname - A string that represent a surname. The characters
 *                         of this string must be between 0x20 and 0x7E.
 * @return kOK if the module has been initialized and the conditions of above
 *         params has been met. And kFAIL, otherwise.
 */
int IC_LED_matrix_draw_name(uint8_t **data, uint8_t size);

/*!
 * @brief
 * This function will draw, in the LED Matrix, a date. Each character
 * (or number) will be shown sequentially, and will stay in the LED Matrix for
 * 1 seconds. The day, the month and the year will be separated by a hyphen.
 *
 * AT+DATE=<int>,<str>,<int>
 *
 * @param data - An array of pointers of type uint8_t
 * @param size - The size of array 'data'.
 * 1º PARAMETRO: day - A unsigned integer that represent the day of a date.
 *                     This value must be less than 32.
 * 2º PARAMETRO: month - A string that represent the month of a date. The
 *                       length of this string must be less than 3 and the
 *                       characters of this string must be between 0x20 and
 *                       0x7E.
 * 3º PARAMETRO: year - A unsigned integer that represent the first byte of a
 *                      year of a date.
 * 4º PARAMETRO: year - A unsigned integer that represent the second byte of a
 *                      year of a date.
 * @return kOK if the module has been initialized and the conditions of above
 *         params has been met. And kFAIL, otherwise.
 */
int IC_LED_matrix_draw_date(uint8_t **data, uint8_t size);

#endif //PROJECT_ICLEDMATRIX_H
