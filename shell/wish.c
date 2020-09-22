#include <errno.h>
#include <error.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

const char *REDIR_SEP = ">";
const char *CMD_SEP = "&\n";
const char *ARG_SEP = " \f\r\t\v";
const char *PROMPT = "wish> ";
const char *ERROR = "An error has occurred\n";
char *DEFAULT_PATHS[] = {
	"/bin"
};

char **SEARCH_PATHS = NULL;
size_t SEARCH_PATHS_LEN = 0;

void update_search_paths(char **paths, size_t paths_len)
{
	for(size_t i = 0; i < SEARCH_PATHS_LEN; i++)
	{
		free(SEARCH_PATHS[i]);
	}

	free(SEARCH_PATHS);
	SEARCH_PATHS = calloc(paths_len, sizeof(char *));
	SEARCH_PATHS_LEN = paths_len;

	for(size_t i = 0; i < paths_len; i++)
	{
		SEARCH_PATHS[i] = strdup(paths[i]);
	}
}

char *absolute_path(char *prog_name)
{
	/* Can probably get away with this assumption. */
	char prog_path[4096];

	/* Start with a slash means we already have the absolute path. */
	if(prog_name[0] == '/')
	{
		return prog_name;
	}

	for(size_t i = 0; i < SEARCH_PATHS_LEN; i++)
	{
		snprintf(prog_path, 4096, "%s/%s", SEARCH_PATHS[i], prog_name);

		if(access(prog_path, X_OK) == 0)
		{
			return strdup(prog_path);
		}
	}

	return NULL;
}

bool is_builtin(char *name)
{
	return
		strcmp(name, "exit") == 0 ||
		strcmp(name, "cd") == 0 ||
		strcmp(name, "path") == 0;
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

void run_program(char *name, char **args, size_t args_len, char *redirect)
{
	pid_t p = fork();

	if(p < 0)
	{
		fprintf(stderr, ERROR);
	}
	else if(p == 0)
	{
		if(redirect != NULL)
		{
			freopen(redirect, "w", stdout);
			freopen(redirect, "w", stderr);
		}
		execv(name, args);
		
		if(errno != 0)
		{
			fprintf(stderr, ERROR);
			errno = 0;
		}
	}
}

void run_cmd(char **args, size_t args_len, char *redirect)
{
	char *exec_path = absolute_path(args[0]);

	if(is_builtin(args[0]))
	{
		run_builtin(args, args_len);
	}
	else if(exec_path != NULL)
	{
		run_program(exec_path, args, args_len, redirect);
	}
	else
	{
		fprintf(stderr, ERROR);
	}

	free(exec_path);
}

void arg_list(char *cmd, size_t cmd_len, size_t *argc, char ***argv)
{
	/* The maximum possible number of arguments occurs when one
	 * character arguments alternate with space. Example: "a a a"
	 * has 5 characters and three arguments. 5 / 2 + 1 = 3
	 */
	*argv = calloc(cmd_len / 2 + 1, sizeof(char *));
	char *arg = strsep(&cmd, ARG_SEP);

	while(arg != NULL)
	{
		if(*arg != '\0')
		{
			(*argv)[*argc] = arg;
			(*argc)++;
		}

		arg = strsep(&cmd, ARG_SEP);
	}
}

void redir_file(char *after_marker, char **filename)
{
	char *red = strsep(&after_marker, ARG_SEP);
	bool found_filename = false;

	while(red != NULL)
	{
	        if(*red != '\0')
		{
			if(found_filename)
			{
				*filename = NULL;
			}
			else
			{
				found_filename = true;
				*filename = red;
			}
		}

		red = strsep(&after_marker, ARG_SEP);
	}
}

void exec_cmd(char *redir, size_t len)
{
	char *cmd = strsep(&redir, REDIR_SEP);
	char **argv = NULL;
	size_t argc = 0;
	char *filename = NULL;
	bool is_redirect = (redir != NULL);

	arg_list(cmd, strlen(cmd), &argc, &argv);
	redir_file(redir, &filename);

	if((argc == 0 && is_redirect) || (is_redirect && filename == NULL))
	{
		fprintf(stderr, ERROR);
	}
	else if(argc != 0)
	{
		run_cmd(argv, argc, filename);
	}

	free(argv);
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
		if(*cmd != '\0')
		{
			exec_cmd(cmd, strlen(cmd));
		}

		cmd = strsep(&line, "&\n");
	}

	/* Now wait for all children to finish. */
	errno = 0;
	while(errno != ECHILD)
	{
		wait(NULL);
	}
}

int main(int argc, char **argv)
{
	FILE *input = stdin;
	char *line = NULL;
	size_t n = 0;
	ssize_t line_len;

	update_search_paths(DEFAULT_PATHS, sizeof(DEFAULT_PATHS)/sizeof(DEFAULT_PATHS[0]));
	
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

	if(input == stdin)
	{
		printf(PROMPT);
	}

	while((line_len = getline(&line, &n, input)) != -1)
	{
		exec_line(line, line_len);

		if(input == stdin)
		{
			printf(PROMPT);
		}
	}

	free(line);
	fclose(input);
	return EXIT_SUCCESS;
}
