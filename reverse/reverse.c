#include <errno.h>
#include <error.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

const char *SAME_FILE_ERR = "reverse: input and output file must differ\n";
const char *INVALID_FILE_ERR = "reverse: cannot open file '%s'\n";
const char *USE_ERR = "usage: reverse <input> <output>\n";

struct stack {
	size_t cap;
	size_t top;
	char **data;
};

void push(struct stack *s, char *datum)
{
	if(s->data == NULL || s->top == s->cap)
	{
		if(s->data == NULL)
		{
			s->cap = 1;
			s->top = 0;
		}

		if(s->top == s->cap)
		{
			s->cap *= 2;
		}
		
		if((s->data = reallocarray(s->data, s->cap, sizeof(char*))) == NULL)
		{
			error(EXIT_FAILURE, errno, "error allocating memory");
		}
	}

	s->data[s->top] = datum;
	s->top++;
}

char *pop(struct stack *s)
{
	s->top--;
	return s->data[s->top];
}

/* While the instructions say not to assume anything about the length
 * of the input file, the OSTEP example code stores all lines in
 * memory. This assumes the input file fits in memory. Some files may
 * not, but this code will make the same assumption.
 */

/* The instructions also fail to account for the case that the last
 * line does not have a newline. Tho OSTEP example code ignores this
 * case, so this code will do the same.
 */
void io_copy(FILE *i, FILE *o)
{
	struct stack lines = {};
	char *line = NULL;
	size_t n = 0;

	while(getline(&line, &n, i) != -1)
	{
		push(&lines, line);
		line = NULL;
		n = 0;
	}

	if(errno != 0)
	{
		error(EXIT_FAILURE, errno, "error reading input");
	}

	while(lines.top != 0)
	{
		line = pop(&lines);
		fprintf(o, "%s", line);
	}

	if(errno != 0)
	{
		error(EXIT_FAILURE, errno, "error writing output");
	}
}

int main(int argc, char **argv)
{
	FILE *in = stdin;
	FILE *out = stdout;
	char *in_path = "/dev/stdin";
	char *out_path = "/dev/stdout";
	struct stat in_stat = {};
	struct stat out_stat = {};

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

	/* This is the only reliable way to tell since different
	 * pathnames may still be the same file.
	 */
	if(in_stat.st_ino != 0 && in_stat.st_ino == out_stat.st_ino)
	{
		fprintf(stderr, SAME_FILE_ERR);
		return EXIT_FAILURE;
	}

	io_copy(in, out);

	fclose(out);
	fclose(in);

	return EXIT_SUCCESS;
}
