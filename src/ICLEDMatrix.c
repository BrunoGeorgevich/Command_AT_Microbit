//
// Created by matheuswhite on 19/03/18.
//

#include "ICLEDMatrix.h"

static bool __is_initialized = false;
static struct mb_image __heart = MB_IMAGE({ 0, 1, 0, 1, 0 },
                                        { 1, 1, 1, 1, 1 },
                                        { 1, 1, 1, 1, 1 },
                                        { 0, 1, 1, 1, 0 },
                                        { 0, 0, 1, 0, 0 });
static struct mb_display *__display;
static uint8_t __time_active;

int IC_LED_matrix_init(void)
{
    if (__is_initialized) {
        printk("[LED Matrix|Error|%d] Module already initialized!\n",
               __LINE__);
        return -ENOT_INIT;
    }

    printk("[LED Matrix|Error|%d] Initializing module...\n", __LINE__);
    __display = mb_display_get();
    __is_initialized = true;
    __time_active = 5;
    printk("[LED Matrix|Error|%d] Module initialized!\n", __LINE__);

    return 0;
}

int IC_LED_matrix_draw_heart(uint8_t **data, uint8_t size)
{
    if (!__is_initialized) {
        printk("[LED Matrix|Error|%d] Module not initialized!\n", __LINE__);
        return -ENOT_INIT;
    }

    if (size != 0) {
        printk("[LED Matrix|Error|%d] Size wrong!\n", __LINE__);
        return -ESIZE_WRONG;
    }

    printk("[LED Matrix|Error|%d] Drawing a heart and showing for %d seconds."
           "\n", __LINE__, __time_active);
    mb_display_image(__display, MB_DISPLAY_MODE_SINGLE,
                     K_SECONDS(__time_active), &__heart, 1);
    k_sleep(K_SECONDS(__time_active));

    return 0;
}

int IC_LED_matrix_set_time_active(uint8_t **data, uint8_t size)
{
    if (size != 1) {
        printk("[LED Matrix|Error|%d] Size wrong!\n", __LINE__);
        return -ESIZE_WRONG;
    }

    uint8_t seconds = *(data[0]);

    printk("[LED Matrix|%d] Old time active: %d, new time active: %d!\n",
           __LINE__, __time_active, seconds);

    __time_active = seconds;

    return 0;
}

int IC_LED_matrix_draw_hours(uint8_t **data, uint8_t size)
{
    if (!__is_initialized) {
        printk("[LED Matrix|Error|%d] Module not initialized!\n", __LINE__);
        return -ENOT_INIT;
    }

    if (size != 2) {
        printk("[LED Matrix|Error|%d] Size wrong!\n", __LINE__);
        return -ESIZE_WRONG;
    }

    uint8_t hour = *(data[0]);
    uint8_t min = *(data[1]);

    if (hour > 23 || min > 59) {
        printk("[LED Matrix|Error|%d] Format not supported!\n", __LINE__);
        return -EHOUR_INVALID;
    }

    char output[6];
    snprintk(output, 6, "%02d:%02d", hour, min);

    printk("[LED Matrix|Error|%d] Drawing %s.\n", __LINE__, output);

    mb_display_print(__display, MB_DISPLAY_MODE_SINGLE, K_SECONDS(1), output);
    k_sleep(K_SECONDS(5));

    return 0;
}

int IC_LED_matrix_draw_string(uint8_t **data, uint8_t size)
{
    if (!__is_initialized) {
        printk("[LED Matrix|Error|%d] Module not initialized!\n", __LINE__);
        return -ENOT_INIT;
    }

    if (size != 1) {
        printk("[LED Matrix|Error|%d] Size wrong!\n", __LINE__);
        return -ESIZE_WRONG;
    }

    char *str = (char *)(data[0]);

    for (size_t i = 0; i < strlen(str); ++i) {
        if (str[i] < 0x20 || str[i] > 0x7E) {
            printk("[LED Matrix|Error|%d] Format not supported!\n", __LINE__);
            return -ENOT_ASCII;
        }
    }

    printk("[LED Matrix|Error|%d] Drawing %s.\n", __LINE__, str);

    mb_display_print(__display, MB_DISPLAY_MODE_SINGLE, K_SECONDS(1), str);
    k_sleep(K_SECONDS(1)*strlen(str));

    return 0;
}

int IC_LED_matrix_draw_name(uint8_t **data, uint8_t size)
{
    if (!__is_initialized) {
        printk("[LED Matrix|Error|%d] Module not initialized!\n", __LINE__);
        return -ENOT_INIT;
    }

    if (size != 2) {
        printk("[LED Matrix|Error|%d] Size wrong!\n", __LINE__);
        return -ESIZE_WRONG;
    }

    char *name = (char *)(data[0]);
    char *surname = (char *)(data[1]);

    for (size_t i = 0; i < strlen(name); ++i) {
        if (name[i] < 0x20 || name[i] > 0x7E) {
            printk("[LED Matrix|Error|%d] Format not supported!\n", __LINE__);
            return -ENOT_ASCII;
        }
    }

    for (size_t i = 0; i < strlen(surname); ++i) {
        if (surname[i] < 0x20 || surname[i] > 0x7E) {
            printk("[LED Matrix|Error|%d] Format not supported!\n", __LINE__);
            return -ENOT_ASCII;
        }
    }

    size_t length = strlen(name)+strlen(surname)+2;
    char output[length];
    snprintk(output, length, "%s-%s", name, surname);

    printk("[LED Matrix|Error|%d] Drawing %s.\n", __LINE__, output);

    mb_display_print(__display, MB_DISPLAY_MODE_SINGLE, K_SECONDS(1), output);
    k_sleep(K_SECONDS(1)*(length-1));

    return 0;
}

int IC_LED_matrix_draw_date(uint8_t **data, uint8_t size)
{
    if (!__is_initialized) {
        printk("[LED Matrix|Error|%d] Module not initialized!\n", __LINE__);
        return -ENOT_INIT;
    }

    if (size != 4) {
        printk("[LED Matrix|Error|%d] Size wrong!\n", __LINE__);
        return -ESIZE_WRONG;
    }

    uint8_t day = *(data[0]);
    char *month = (char *)(data[1]);
    uint16_t year = ((*(data[3])) << 8) | (*(data[2]));

    if (strlen(month) != 3 || day > 31) {
        printk("[LED Matrix|Error|%d] Format not supported!\n", __LINE__);
        return -EDATE_FMT_WRONG;
    }

    for (size_t i = 0; i < strlen(month); ++i) {
        if (month[i] < 0x20 || month[i] > 0x7E) {
            printk("[LED Matrix|Error|%d] Format not supported!\n", __LINE__);
            return -ENOT_ASCII;
        }
    }

    char output[12];
    snprintk(output, 12, "%02d-%s-%04d", day, month, year);

    printk("[LED Matrix|Error|%d] Drawing %s.\n", __LINE__, output);

    mb_display_print(__display, MB_DISPLAY_MODE_SINGLE, K_SECONDS(1), output);
    k_sleep(K_SECONDS(11));

    return 0;
}
