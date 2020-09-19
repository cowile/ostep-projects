#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

const char *PROMPT = "wish> ";
const char *ERROR = "An error has occurred\n";
const char REDIRECT_OUTPUT = '>';
const char PARALLEL = '&';

char **SEARCH_PATH;
size_t SEARHCH_PATH_LEN;

void shell_exit(int exit_code)
{
	exit(exit_code);
}

int shell_chdir(char *path)
{
	if(chdir(path) == -1)
	{
		fprintf(stdout, ERROR);
	}
}

int shell_path(char **paths, size_t n)
{
	for(size_t i = 0; i < n; i++)
	{
		free(SEARCH_PATH[i]);
	}
	free(SEARCH_PATH);

	SEARCH_PATH = paths;
	SEARCH_PATH_LEN = n;

	return 0;
}

char *absolute_path(char *name)
{
	char path[1024];
	for(size_t i = 0; i < SEARCH_PATH_LEN; i++)
	{
		snprintf(path, 1024, "%s/%s", SEARCH_PATH[i], name);
		if(access(path, X_OK) == 0)
		{
			return strdup(path);
		}
	}
	
	return NULL:
}

int main(int argc, char **argv)
{
	return EXIT_SUCCESS;
}
