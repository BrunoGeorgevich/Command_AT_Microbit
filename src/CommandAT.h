#ifndef COMMANDAT_H
#define COMMANDAT_H

#include <zephyr.h>
#include <misc/printk.h>
#include <board.h>
#include <gpio.h>
#include <device.h>

#include <display/mb_display.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define kOK     0
#define kFAIL   1

#define BIT0    0
#define BIT1    1
#define BIT2    2
#define BIT3    3
#define BIT4    4
#define BIT5    5
#define BIT6    6
#define BIT7    7

#define STRING  1
#define INTEGER 0

#define SIZE_COMMAND_NAME_STRING    8
#define SIZE_PARAM_DATA_STRING      8

#define MAX_NUM_OF_PARAMS_COMMAND   8
#define MAX_SIZE_STRING_DATA        40
#define MAX_SIZE_PARAM_DATA         20
#define MAX_NUM_OF_COMMANDS   	    8

#define LIMIT_OF_SUBSTRING_BUFFER   40
#define LIMIT_OF_SPLIT_BUFFER       40

#define CAST(valor, tipo) ((tipo) (valor))
#define ASCII_TO_INT(x) x - '0'
#define UNUSED(x) (void)(x)

typedef struct {
    char op_name[SIZE_COMMAND_NAME_STRING];
    char param_types;
    char num_of_params;
    int  (*method)(uint8_t **, uint8_t);
//    void *command_params[MAX_NUM_OF_PARAMS_COMMAND];
} Command;

void CommandATCreateCommand(char *op_name, char param_types, char num_of_params, int  (*method)(uint8_t **data, uint8_t size));

void CommandATCleanCommandDataArray(char *data, int data_size);

int CommandATSearchCommand(char *command_data);

Command *CommandATGetLastCommand();

uint8_t *CommandATAddStringParamToList(char *data);

uint8_t *CommandATAddIntegerParamToList(int data);

char *CommandATGetSubstring(char *string, int init, int end);

char *CommandATSplit(char *string, int index, char delimiter);

void CommandATInvoker(char *command_data);

void CommandATCommandParser(uint8_t data);


/* CALLBACKS FUNCTIONS */

int CommandAT(uint8_t **data, uint8_t size);

int CommandATPrintCommands(uint8_t **data, uint8_t size);

int CommandATSum(uint8_t **data, uint8_t size);

/***********************/

#endif //COMMANDAT_H
