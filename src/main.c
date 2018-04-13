/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * AUTHOR : BRUNO GEORGEVICH FERREIRA
 */

/* CUSTOM LIBS */

#include "uart_driver.h"
#include "ICLEDMatrix.h"
#include "CommandAT.h"

/***************/

int main() {

    CommandATCreateCommand(
                "AT",
                0,
                0,
                CommandAT
                );

    CommandATCreateCommand(
                "LIST",
                0,
                0,
                CommandATPrintCommands
                );

    CommandATCreateCommand(
                "SUM",
                (0 | (INTEGER << BIT0) | (INTEGER << BIT1)),
                2,
                CommandATSum
                );

    CommandATCreateCommand(
                "HEART",
                0,
                0,
                IC_LED_matrix_draw_heart
                );

    CommandATCreateCommand(
                "TIME",
                (0 | (INTEGER << BIT0)),
                1,
                IC_LED_matrix_set_time_active
                );

    CommandATCreateCommand(
                "HOURS",
                (0 | (INTEGER << BIT0) | (INTEGER << BIT1)),
                2,
                IC_LED_matrix_draw_hours
                );

    CommandATCreateCommand(
                "NAME",
                (0 | (STRING << BIT0) | (STRING << BIT1)),
                2,
                IC_LED_matrix_draw_name
                );

    CommandATCreateCommand(
                "DATE",
                (0 | (INTEGER << BIT0) | (STRING << BIT1) | (INTEGER << BIT2) | (INTEGER << BIT3)),
                4,
                IC_LED_matrix_draw_date
                );

    printk("INIT LED PANEL : %d\n", (IC_LED_matrix_init() == 0) ? "OK" : "FAIL");

    DriverOpen();
    DriverSetCallback(CommandATCommandParser);
    return 0;
}
