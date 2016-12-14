#ifndef HALE_UTIL_H
#define HALE_UTIL_H

#include <inttypes.h>
#include <stdio.h>

#include "config.h"

#define HANDLE_UNRECOVERABLE_ERROR(x) handleUnrecoverableError(__func__, x)

#define PRINT_MSG(msg) fprintf(stdout, "%s: %s\n", __func__, msg)
#define PRINT_MSG_ARG(msg,arg) fprintf(stdout, "%s: %s: %s\n", __func__, msg, arg)
#define PRINT_MSG_INT(msg,arg) fprintf(stdout, "%s: %s: %d\n", __func__, msg, arg)
#define PRINT_MSG_LONG(msg,arg) fprintf(stdout, "%s: %s: %ld\n", __func__, msg, arg)

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

//We'll check for NULL pointers... unless we want to go fast and break things
#ifndef GO_FAST_AND_BREAK_THINGS
#define CHECK_NULL_PTR(ptr,var) {if(ptr == NULL){fprintf(stdout, "%s: <%s> is NULL\n", __func__, var); HANDLE_UNRECOVERABLE_ERROR(HALE_BAD_INPUT);}}
#else
#define CHECK_NULL_PTR(ptr,var) {}
#endif


void handleUnrecoverableError(const char* func, HALE_status_t err_code);

//Generates a random number between the provided min and max values
//This could be implementation-dependent, so bring out into its own function
uint8_t getRandom8(uint16_t min, uint16_t max);

#endif
