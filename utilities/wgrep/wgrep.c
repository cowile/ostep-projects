#include <errno.h>
#include <error.h>
#include <regex.h>
#include <stdlib.h>
#include <stdio.h>

const char *CANNOT_OPEN_ERR = "wgrep: cannot open file\n";
const size_t REG_BUF_SIZE = 128;

int main(int argc, char **argv)
{
	char *pat_string;
	char *path;
	FILE *f;
	regex_t pat;
	int reg_err = 0;
	char reg_err_buf[REG_BUF_SIZE];

	if(argc > 1)
	{
		pat_string = argv[1];
		if((reg_err = regcomp(&pat, pat_string, 0)) != 0)
		{
			regerror(reg_err, &pat, reg_err_buf, REG_BUF_SIZE);
			error(EXIT_FAILURE, 0, "error regex compile: %s", reg_err_buf);
		}
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

		/* search_file(f); */
	}

	return EXIT_SUCCESS;
}
