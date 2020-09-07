#include <errno.h>
#include <error.h>
#include <stdlib.h>
#include <stdio.h>

const char *USAGE_ERR = "wunzip: file1 [file2 ...]\n";

/* Pack the structure so it is exactly 5 bytes. */
#pragma pack(1)
struct run_length_block
{
	int n;
	char c;
};

void uncompress(FILE *f)
{}

int main(int argc, char **argv)
{
	char *path = NULL;
	FILE *f = NULL;
	
	if(argc == 1)
	{
		/* Should be stderr, but tests are wrong. */
		fprintf(stdout, USAGE_ERR);
		return EXIT_FAILURE;
	}

	for(int i = 1; i < argc; i++)
	{
		path = argv[i];
		if((f = fopen(path, "r")) == NULL)
		{
			error(EXIT_FAILURE, errno, "%s", path);
		}

		/* compress(f, &rlb); */
	}

	return EXIT_SUCCESS;
}
