#include <errno.h>
#include <error.h>
#include <regex.h>
#include <stdlib.h>
#include <stdio.h>

const char *CANNOT_OPEN_ERR = "wgrep: cannot open file\n";
const char *USAGE_ERR = "wgrep: searchterm [file ...]\n";
const size_t REG_BUF_SIZE = 128;

void search_file(FILE *f, regex_t *pat)
{
	char *line = NULL;
	size_t n = 0;
	int reg_err = 0;

	while(getline(&line, &n, f) != -1)
	{
		if((reg_err = regexec(pat, line, 0, NULL, 0)) == 0)
		{
			fputs(line, stdout);
		}

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
	char *pat_string;
	char *path;
	FILE *f;
	regex_t pat;
	int reg_err = 0;
	char reg_err_buf[REG_BUF_SIZE];

	if(argc == 1)
	{
		/* This is incorrect. Error messages should go
		 * to stderr, but do what the test wants.
		 */
		fprintf(stdout, USAGE_ERR);
		return EXIT_FAILURE;
	}

	if(argc > 1)
	{
		pat_string = argv[1];
		if((reg_err = regcomp(&pat, pat_string, 0)) != 0)
		{
			regerror(reg_err, &pat, reg_err_buf, REG_BUF_SIZE);
			error(EXIT_FAILURE, 0, "error regex compile: %s", reg_err_buf);
		}
	}

	if(argc == 2)
	{
		search_file(stdin, &pat);
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

		search_file(f, &pat);
	}

	return EXIT_SUCCESS;
}
