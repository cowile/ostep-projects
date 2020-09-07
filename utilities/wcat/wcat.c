#include <errno.h>
#include <error.h>
#include <stdlib.h>
#include <stdio.h>

const char *CANNOT_OPEN_ERR = "wcat: cannot open file\n";
const size_t BUFFER_SIZE = 1024;

void write_file(FILE *f)
{
	char buffer[BUFFER_SIZE];

	while(fgets(buffer, BUFFER_SIZE, f) != NULL)
	{
		fputs(buffer, stdout);
		if(errno != 0)
		{
			error(EXIT_FAILURE, errno, "error writing");
		}
	}

	if(errno != 0)
	{
		error(EXIT_FAILURE, errno, "error reading file");
	}
}

int main(int argc, char **argv)
{
	char *path;
	FILE *f;
	
	for(int i = 1; i < argc; i++)
	{
		path = argv[i];
		if((f = fopen(path, "r")) == NULL)
		{
			/* This is incorrect. Error messages should go
			 * to stderr, but do what the test wants.
			 */
			fprintf(stdout, CANNOT_OPEN_ERR);
			return EXIT_FAILURE;
		}

		write_file(f);
	}

	return EXIT_SUCCESS;
}
