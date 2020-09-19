#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

const char *PROMPT = "wish> ";
const char *ERROR = "An error has occurred\n";
const char REDIRECT_OUTPUT = '>';
const char PARALLEL = '&';

char *SEARCH_PATH[] = {
	"/bin"
};

void shell_exit(int exit_code)
{
	exit(exit_code);
}

int shell_chdir(char *path)
{
	return chdir(path);
}

int shell_path(char paths)
{
	return 0;
}

int main(int argc, char **argv)
{
	return EXIT_SUCCESS;
}
