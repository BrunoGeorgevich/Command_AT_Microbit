/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * AUTHOR : BRUNO GEORGEVICH FERREIRA
 */

#include "CommandAT.h"


/* GLOBAL VARIABLES */

uint8_t  string_params_index = 0;
char string_params_list[MAX_NUM_OF_PARAMS_COMMAND*MAX_NUM_OF_COMMANDS][SIZE_PARAM_DATA_STRING];

uint8_t int_params_index = 0;
uint8_t int_params_list[MAX_NUM_OF_PARAMS_COMMAND*MAX_NUM_OF_COMMANDS];

uint8_t commands_index = 0;
Command commands_list[MAX_NUM_OF_COMMANDS];

uint8_t command_parser_data_index = 0;
char command_parser_data[MAX_SIZE_STRING_DATA];

char substring_buffer[LIMIT_OF_SUBSTRING_BUFFER];
char split_buffer[LIMIT_OF_SPLIT_BUFFER];
char param_data[MAX_SIZE_PARAM_DATA];

/********************/

void CommandATCreateCommand(char *op_name, char param_types, char num_of_params, int (*method)(uint8_t **, uint8_t)) {
    if(commands_index == MAX_NUM_OF_COMMANDS - 1) {
        printk("HAS NO MORE SPACE FOR COMMANDS, ONLY SUPPORTS %d\n", MAX_NUM_OF_COMMANDS);
        return;
    }
    strcpy(commands_list[commands_index].op_name, op_name);
    commands_list[commands_index].num_of_params = num_of_params;
    commands_list[commands_index].param_types = param_types;
    commands_list[commands_index].method = method;

    commands_index++;
}

void CommandATCleanCommandDataArray(char *data, int data_size) {
    if(data == NULL) return;
    memset(data, 0, data_size);
}

int CommandATSearchCommand(char *command_data) {
    for(int i = 0; i < commands_index; i++) {
        if(strcmp(commands_list[i].op_name, command_data) == 0) {
            return i;
        }
    }

    return -1;
}

Command *CommandATGetLastCommand() {
    return &commands_list[commands_index];
}

uint8_t *CommandATAddStringParamToList(char *data) {
    strcpy(string_params_list[string_params_index], data);
    string_params_index++;
    return string_params_list[string_params_index - 1];
}

uint8_t *CommandATAddIntegerParamToList(int data) {
    int_params_list[int_params_index] = data;
    int_params_index++;
    return &int_params_list[int_params_index - 1];
}

char *CommandATGetSubstring(char *string, int init, int end) {

    if(end-init <= 0) {
        return NULL;
    } else if(end > strlen(string)) {
        return NULL;
    }

    for(int index = init; index <= end; index++) {
        if(index == end) {
            substring_buffer[index - init] = '\0';
        } else {
            substring_buffer[index - init] = string[index];
        }
    }

    return substring_buffer;
}

char *CommandATSplit(char *string, int index, char delimiter) {
    if(string == NULL) string = split_buffer;
    int str_len = strlen(string);

    for(int i = 0; i < str_len; i++) {
        split_buffer[i] = string[i];
    }
    split_buffer[str_len] = '\0';

    int init = 0;
    int end = str_len;
    int enter_quotes = 0;
    for(int i = 0; i < str_len; i++) {
        if(split_buffer[i] == '\"' && delimiter != '\"') {
            enter_quotes = !enter_quotes;
        }
        else if(split_buffer[i] == delimiter && !enter_quotes) {
            if(index > 0) {
                index--;
                init = i + 1;
            } else {
                end = i;
                break;
            }
        }
    }
    if(index > 0) {
        //        printk("INDEX :: %d <-> STR :: %s\n", index, CommandATGetSubstring(split_buffer, init, end));
        return NULL;
    }

    return CommandATGetSubstring(split_buffer, init, end);
}

void CommandATInvoker(char *command_data) {
    char *command_name = CommandATSplit(command_data, 1, '+'); //Only to remove AT+
    command_name = CommandATSplit(command_name,0,'=');
    if(command_name == NULL) {
        printk("Command is not valid! Use 'LIST' to show all Commands\n");
        return;
    }

    printk("\nCOMMAND NAME :: %s\n", command_name);

    int command_index = CommandATSearchCommand(command_name);
    if(command_index == -1) {
        printk("Command is not valid! Use 'LIST' to show all Commands\n");
        return;
    }

    Command *current_command = &commands_list[command_index];

    uint8_t params_size = 0;
    uint8_t *params[current_command->num_of_params];

    while(params_size < current_command->num_of_params) {
        char param_str[MAX_SIZE_PARAM_DATA];
        strcpy(param_str, CommandATSplit(command_data,1,'='));

        memset(param_data, 0, MAX_SIZE_PARAM_DATA);
        char *params_splitted_data = CommandATSplit(param_str, params_size, ',');

        if(params_splitted_data == NULL) {
            printk("PARAM [%d] WAS NULL\n", params_size);
            break;
        }

        strcpy(param_data, params_splitted_data);

        if(params_size == MAX_NUM_OF_PARAMS_COMMAND - 1) {
            printk("ONLY SUPPORT %d PARAMETERS\n", MAX_NUM_OF_PARAMS_COMMAND);
            break;
        }
        if(param_data[0] == '\"') {
            char *data = CommandATSplit(params_splitted_data,1,'\"');
            params[params_size] = CommandATAddStringParamToList(data);
            printk("COMMAND PARAMETER [%d][STRING] :: %s\n",params_size, data);
        } else {
            params[params_size] = CommandATAddIntegerParamToList(atoi(param_data));
            printk("COMMAND PARAMETER [%d][INT] :: %d\n",params_size, atoi(param_data));
        }
        params_size++;
    }

    printk("COMMAND RETURN :: %s\n", (current_command->method(params, params_size) == 0) ? "Ok" : "Fail");
}

void CommandATCommandParser(uint8_t data) {
    // CHAR 13 IS \n
    if(data == 13) {
        CommandATInvoker(command_parser_data);
        command_parser_data_index = 0;
        CommandATCleanCommandDataArray(command_parser_data, MAX_SIZE_STRING_DATA);
    } else {
        printk("%c", data);
        command_parser_data[command_parser_data_index] = data;
        command_parser_data_index++;
    }
}


int CommandAT(uint8_t **data, uint8_t size)
{
    UNUSED(data); UNUSED(size);
    return kOK;
}

int CommandATPrintCommands(uint8_t **data, uint8_t size){
    UNUSED(data); UNUSED(size);
    for(int k = 0; k < commands_index; k++) {
        Command *command = &commands_list[k];
        printk("\nCOMMAND {\n");
        printk("\tOP_NAME     = %s\n", command->op_name);
        printk("\tPARAM_TYPES = %d\n", command->param_types);
        printk("\tNUM_PARAMS  = %d\n", command->num_of_params);
        printk("}\n");
    }
    return kOK;
}

int CommandATSum(uint8_t **data, uint8_t size) {
    if(size != 2) {
        printk("SUM only supports 2 parameters\n");
        return kFAIL;
    }

    int a = *CAST(data[0], uint8_t *);
    int b = *CAST(data[1], uint8_t *);
    printk("SUM IS :: %d\n", a+b);
    return kOK;
}
