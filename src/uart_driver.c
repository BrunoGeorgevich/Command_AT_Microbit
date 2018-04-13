/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "uart_driver.h"

#include <zephyr.h>
#include <device.h>
#include <misc/printk.h>
#include <uart.h>

static void (*uart_callback)(uint8_t) = 0;

void DriverSetCallback(void (*callback)(uint8_t)) {
    uart_callback = callback;
}

static void uart_fifo_callback(struct device *dev)
{
    u8_t recvData;
    /* Verify uart_irq_update() */
    if (!uart_irq_update(dev)) {
        printk("retval should always be 1\n");
        return;
    }
    /* Verify uart_irq_rx_ready() */
    if (uart_irq_rx_ready(dev)) {
        /* Verify uart_fifo_read() */
        uart_fifo_read(dev, &recvData, 1);
        if(uart_callback) {
            uart_callback(recvData);
        }
    }
}

uint8_t DriverOpen() {
    struct device *uart_dev = device_get_binding("UART_0");
    if(!uart_dev) {
    printk("Problem to load uart device\n");
        return 1;
    }
    printk("UART device loaded successfully!\n");

    /* Verify uart_irq_callback_set() */
    uart_irq_callback_set(uart_dev, uart_fifo_callback);

    /* Enable Tx/Rx interrupt before using fifo */
    /* Verify uart_irq_rx_enable() */
    uart_irq_rx_enable(uart_dev);
    printk("Now you can start sending AT commands\n");
    return 0;
}

void DriverWrite(uint8_t data) {
    printk("%c", (char) data);
}
