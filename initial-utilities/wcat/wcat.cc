#include <stdio.h>
#include <string.h>
#include <iostream>

int main(int argc, char *argv[]) {
	for (int i = 1; i < argc; i++) {
		char *filename = argv[i];

		FILE *fp = fopen(filename, "r");
		if (fp == NULL) {
			std::cout << "wcat: cannot open file\n";
			return 1;
		}

		char *line = NULL;
		size_t linecap;
		while (getline(&line, &linecap, fp) > 0) {
			std::cout << line;
		}
		fclose(fp);
	}

	return 0;
}
