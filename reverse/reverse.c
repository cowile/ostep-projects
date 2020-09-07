#include <errno.h>
#include <error.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>

const char *SAME_FILE_ERR = "reverse: input and output file must differ\n";
const char *INVALID_FILE_ERR = "reverse: cannot open file '%s'\n";
const char *USE_ERR = "usage: reverse <input> <output>\n";

int main(int argc, char **argv)
{
	FILE *in = stdin;
	FILE *out = stdout;
	char *in_path = "/dev/stdin";
	char *out_path = "/dev/stdout";
	struct stat in_stat = {0};
	struct stat out_stat = {0};

	if(argc > 3)
	{
		fprintf(stderr, USE_ERR);
		return EXIT_FAILURE;
	}

	if(argc > 2)
	{
		out_path = argv[2];
		if((out = fopen(out_path, "w")) == NULL)
		{
			fprintf(stderr, INVALID_FILE_ERR, out_path);
			return EXIT_FAILURE;
		}
		if(stat(out_path, &out_stat) == -1)
		{
			error(EXIT_FAILURE, errno, "%s", out_path);
		}
	}

	if(argc > 1)
	{
		in_path = argv[1];
		if((in = fopen(in_path, "r")) == NULL)
		{
			fprintf(stderr, INVALID_FILE_ERR, in_path);
			return EXIT_FAILURE;
		}
		if(stat(in_path, &in_stat) == -1)
		{
			error(EXIT_FAILURE, errno, "%s", in_path);
		}
	}

	if(in_stat.st_ino == out_stat.st_ino)
	{
		fprintf(stderr, SAME_FILE_ERR);
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}
