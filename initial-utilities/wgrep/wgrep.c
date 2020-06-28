#include <stdio.h>
#include <string.h>

void grep(char *term, FILE *fp) {
	char *line = NULL;
	size_t linecap;
	while (getline(&line, &linecap, fp) > 0) {
		if (strstr(line, term) != NULL) {
			printf("%s", line);
		}
	}
}

int main(int argc, char *argv[]) {
	// Check argument counts
	if (argc <= 1) {
		printf("wgrep: searchterm [file ...]\n");
		return 1;
	}

	char *term = argv[1];

	// Grep from stdin
	if (argc == 2) {
		grep(term, stdin);
		return 0;
	}

	// Grep in each file provided
	for(int i = 2; i < argc; i++) {
		FILE *fp = fopen(argv[i], "r");
		if (fp == NULL) {
			printf("wgrep: cannot open file\n");
			return 1;
		}
		grep(term, fp);
		fclose(fp);
	}
	return 0;
}

