#include <errno.h>
#include <error.h>
#include <stdlib.h>
#include <stdio.h>

const char *USAGE_ERR = "wzip: file1 [file2 ...]\n";

/* Pack the structure so it is exactly 5 bytes. */
#pragma pack(1)
struct run_length_block
{
	int n;
	char c;
};

/* We need to pass in the state of the current block since compression
 * continues across files. That is is not obvious from the
 * instructions.
 */
void compress(FILE *f, struct run_length_block *state)
{
	char c = 0;

	while(fread(&c, sizeof(c), 1, f) == 1)
	{
		if(state->c != c && state->n != 0)
		{
			fwrite(state, sizeof(*state), 1, stdout);
			state->n = 0;
		}
		state->c = c;
		state->n++;
	}
}

int main(int argc, char **argv)
{
	char *path = NULL;
	FILE *f = NULL;
	struct run_length_block rlb = {};

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

		compress(f, &rlb);
	}

	if(rlb.n != 0)
	{
		fwrite(&rlb, sizeof(rlb), 1, stdout);
	}
	
	return EXIT_SUCCESS;
}
