#ifndef MAIN_H
#define MAIN_H

#include <sys/types.h>

char **split_command(char *command);
pid_t exec_command(char *command);
void remove_substring(char *s, const char *toremove);
void signal_handler(int);

#endif
