#ifndef SMSH
#define SMSH

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <pwd.h>
#include <limits.h>
#include <signal.h>

#include "splitter.h"

#define USER_COLOR "\033[38;5;68m"
#define PATH_COLOR "\033[38;5;120m"
#define DEFAULT_COLOR "\033[0m"

/**
 * Evaluates command line to run system commands or shell-scripts
 */
void shellEval(char** argv, size_t argc);

/**
 * Write the cwd and shell user to the screen
 */
void showPrompt();

/**
 * Reads in a line of user-input
 *
 * @param input
 * @param len
 *
 * @returns number of bytes read
 */
ssize_t getCmd(char** input);

#endif
