#include <errno.h>
#include <error.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

const char *CMD_SEP = "&\n";
const char *ARG_SEP = " \f\r\t\v";
const char *PROMPT = "wish> ";
const char *ERROR = "An error has occurred\n";

char **SEARCH_PATHS = NULL;
size_t SEARCH_PATHS_LEN = 0;

void update_search_paths(char **paths, size_t paths_len)
{
}

char *absolute_path(char *prog_name)
{
	return NULL;
}

void run_builtin(char **args, size_t args_len)
{
	if(strcmp(args[0], "exit") == 0 && args_len == 1)
	{
		exit(EXIT_SUCCESS);
	}
	else if(strcmp(args[0], "cd") == 0 && args_len == 2)
	{
		chdir(args[1]);
		if(errno != 0)
		{
			fprintf(stderr, ERROR);
			errno = 0;
		}
	}
	else if(strcmp(args[0], "path") == 0)
	{
		update_search_paths(args + 1, args_len - 1);
	}
	else
	{
		fprintf(stderr, ERROR);
	}
}

void run_program(char *name, char **args, size_t args_len, FILE *redirect)
{
}

void run_cmd(char **args, size_t args_len, FILE *redirect)
{
	char *exec_path = absolute_path(args[0]);

	if(exec_path == NULL)
	{
		run_builtin(args, args_len);
	}
	else
	{
		run_program(exec_path, args, args_len, redirect);
	}
}

void exec_cmd(char *cmd, size_t cmd_len)
{
	/* The maximum possible number of arguments occurs when one
	 * character arguments alternate with space. Example: "a a a"
	 * has 5 characters and three arguments. 5 / 2 + 1 = 3
	 */
	char *args[cmd_len / 2 + 1];
	size_t args_len = 0;
	char *arg = strsep(&cmd, ARG_SEP);
	bool open_redirect = false;
	FILE *redirect_file = NULL;

	while(arg != NULL)
	{
		/* If there is arbitrary space, argument may be
		 * empty. Do nothing.
		 */
		if(*arg == '\0')
		{
		}
		else if(open_redirect)
		{
			open_redirect = false;
			if((redirect_file = fopen(arg, "w")) == NULL)
			{
				fprintf(stderr, ERROR);
				exit(EXIT_FAILURE);
			}
		}
		/* Redirection can possibly occur with or without
		 * space separating the operator. An argument with a
		 * redirection operator needs special handling.
		 */
		else if(strchr(arg, '>') == NULL)
		{
			/* No redirection. Easy. */
			args[args_len] = arg;
			args_len++;
		}
		else
		{
			open_redirect = true;
			arg = strsep(&cmd, ">");

			/* Possibly an argument before the redirect operator. */
			if(*arg != '\0')
			{
				args[args_len] = arg;
				args_len++;
			}
		}

		arg = strsep(&cmd, ARG_SEP);
	}

	run_cmd(args, args_len, redirect_file);
}

/* For the scope given, take advantage of the fact that the only time
 * we ever need to wait for children commands when we have finished
 * processing the line.
 */
void exec_line(char *line, size_t len)
{
	char *cmd = strsep(&line, CMD_SEP);

	while(cmd != NULL)
	{
		/* Since line is updated to point after the null byte,
		 * length of command string is given by line - cmd
		 * - 1.
		 */
		exec_cmd(cmd, line - cmd - 1);
		cmd = strsep(&line, "&\n");
	}

	/* Now wait for all children to finish. */
	while(wait(NULL) != ECHILD);
}

int main(int argc, char **argv)
{
	FILE *input = stdin;
	char *line = NULL;
	size_t n = 0;
	ssize_t line_len;

	if(argc > 2)
	{
		fprintf(stderr, ERROR);
		return EXIT_FAILURE;
	}

	if(argc == 2 && (input = fopen(argv[1], "r")) == NULL)
	{
		fprintf(stderr, ERROR);
		return EXIT_FAILURE;
	}

	while((line_len = getline(&line, &n, input)) != -1)
	{
		exec_line(line, line_len);
	}

	free(line);
	fclose(input);
	return EXIT_SUCCESS;
}
