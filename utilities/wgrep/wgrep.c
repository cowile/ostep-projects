#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const char *CANNOT_OPEN_ERR = "wgrep: cannot open file\n";
const char *USAGE_ERR = "wgrep: searchterm [file ...]\n";

void search_file(FILE *f, char *term)
{
	char *line = NULL;
	size_t n = 0;

	while(getline(&line, &n, f) != -1)
	{
		if(strstr(line, term) != NULL)
		{
			fputs(line, stdout);
		}
	}
}

int main(int argc, char **argv)
{
	char *path;
	char *search_term;
	FILE *f;

	if(argc == 1)
	{
		/* This is incorrect. Error messages should go
		 * to stderr, but do what the test wants.
		 */
		fprintf(stdout, USAGE_ERR);
		return EXIT_FAILURE;
	}

	search_term = argv[1];
	
	if(argc == 2)
	{
		search_file(stdin, search_term);
	}
	
	for(int i = 2; i < argc; i++)
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

		search_file(f, search_term);
		fclose(f);
	}

	return EXIT_SUCCESS;
}
