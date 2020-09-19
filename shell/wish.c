#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

const char *PROMPT = "wish> ";
const char *ERROR = "An error has occurred\n";

char **SEARCH_PATHS;
size_t SEARCH_PATHS_LEN;

void shell_exit(int exit_code)
{
	exit(exit_code);
}

void shell_chdir(char *path)
{
	if(chdir(path) == -1)
	{
		fprintf(stdout, ERROR);
	}
}

void shell_path(char **paths, size_t n)
{
	for(size_t i = 0; i < SEARCH_PATHS_LEN; i++)
	{
		free(SEARCH_PATHS[i]);
	}

	free(SEARCH_PATHS);
	SEARCH_PATHS_LEN = n;
	paths = calloc(n, sizeof(char *));

	for(size_t i = 0; i < n; i++)
	{
		SEARCH_PATHS[i] = strdup(paths[i]);
	}
}

char *absolute_path(char *name)
{
	char *path = NULL;
	int path_len = 0;

	if(name == NULL || name[0] == '\0')
	{
		return NULL;
	}

	if(name[0] == '/')
	{
		return name;
	}
	
	for(size_t i = 0; i < SEARCH_PATHS_LEN; i++)
	{
		path_len = strlen(SEARCH_PATHS[i]) + strlen(name) + 1;
		path = calloc(path_len + 1, sizeof(char));
		snprintf(path, path_len + 1, "%s/%s", SEARCH_PATHS[i], name);
		
		if(access(path, X_OK) == 0)
		{
			return strdup(path);
		}

		free(path);
	}
	
	return NULL;
}

void execute_command(char **args, size_t n)
{
	char *abs_path = NULL;

	if(strcmp(args[0], "exit") && n == 1)
	{
		shell_exit(EXIT_SUCCESS);
	}
	else if(strcmp(args[0], "chdir") && n == 2)
	{
		shell_chdir(args[1]);
	}
	else if(strcmp(args[0], "path"))
	{
		shell_path(args + 1, n - 1);
	}
	else if((abs_path = absolute_path(args[0])) != NULL)
	{
		if(fork() == 0)
		{
			execv(abs_path, args);
		}
	}
	else
	{
		fprintf(stdout, ERROR);
	}
}

int main(int argc, char **argv)
{
	char *line = NULL;
	char *line_pos = NULL;
	char *token = NULL;
	size_t n = 0;
	ssize_t line_len = 0;

	FILE *input = stdin;

	char **args = NULL;
	size_t args_len = 0;

	if(argc > 2)
	{
		fprintf(stdout, ERROR);
		return EXIT_FAILURE;
	}

	if(argc == 2)
	{
		if((input = fopen(argv[1], "r")) == NULL)
		{
			fprintf(stdout, ERROR);
			return EXIT_FAILURE;
		}
	}

	while((line_len = getline(&line, &n, input)) != -1)
	{
		line_pos = line;
		token = line_pos;
		/* Maximum possible length of argument list occurs
		 * with one character arguments every other
		 * position. Example: "a a a" has three arguments and
		 * five characters. 5 / 2 + 1 = 3. */
		args = calloc(line_len / 2 + 1, sizeof(char *));
		while(token != NULL)
		{
			if(*token == '\n' || *token == ';' || *token == '&')
			{
				execute_command(args, args_len);
				while(*token != '&' && wait(NULL) != ECHILD);
			}
			else
			{
				args[args_len] = token;
				args_len++;
			}

			token = strsep(&line_pos, " \f\r\t\v");
		}

		free(args);
	}

	free(line);
	fclose(input);
	return EXIT_SUCCESS;
}
