#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "main.h"

pid_t pID;

const int command_size = 256;

int main(void)
{
	char command[command_size];

	while (1) {
		strcpy(command, "\0");
		printf("maiterm > ");
		fgets(command, sizeof(command), stdin);
		remove_substring(command, "\n");

		if (strcmp(command, "") == 0)
			continue;

		if (strcmp(command, "exit") == 0)
			break;

		printf("exit %d\n", exec_command(command));
	}

	_exit(0);
}

void signal_handler(int sig)
{

	if (pID != 0)
		kill(pID, SIGKILL);

}

pid_t exec_command(char *command)
{
	int waitforpid = strstr(command, " &") == NULL;

	if (!waitforpid)
		remove_substring(command, " &");

	pID = fork();
	int status = 0;

	if (pID == 0) {
		signal(SIGINT, SIG_IGN);

		int res = execvp(command, split_command(command));

		if (res == -1)
			printf("command not found\n");

	} else if (pID < 0) {
		printf("Failed to fork\n");
		_exit(1);
	} else {
		if (waitforpid)
			signal(SIGINT, signal_handler);
		else
			signal(SIGINT, SIG_IGN);
		if (waitforpid)
			waitpid(pID, &status, 0);
	}

	return pID;
}

char **split_command(char *command)
{

	char **res = NULL;
	char *p = strtok(command, " ");
	int n_spaces = 0, i;

	while (p) {
		res = realloc(res, sizeof(char *) * ++n_spaces);

		if (res == NULL) {
			free(res);
			_exit(-1);
		}

		res[n_spaces-1] = p;

		p = strtok(NULL, " ");
	}

	res = realloc(res, sizeof(char *) * (n_spaces+1));
	if (res == NULL) {
		free(res);
		_exit(-1);
	}
	res[n_spaces] = NULL;

	return res;
}

void remove_substring(char *s, const char *toremove)
{
	s = strstr(s, toremove);
	memmove(s, s+strlen(toremove), 1+strlen(s+strlen(toremove)));
}
