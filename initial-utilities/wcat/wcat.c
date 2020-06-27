#include <stdio.h>

int main(int argc, char *argv[]) {
	for (int i = 1; i < argc; i++) {
		FILE *fp = fopen(argv[i], "r");
		if (fp == NULL) {
			printf("wcat: cannot open file\n");
			return 1;
		}

		char *line = NULL;
		size_t linecap;
		while (getline(&line, &linecap, fp) > 0) {
			printf("%s", line);
		}

		fclose(fp);
	}

	return 0;
}
