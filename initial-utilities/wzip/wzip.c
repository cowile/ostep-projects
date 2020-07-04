#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	if (argc <= 1) {
		printf("wzip: file1 [file2 ...]\n");
		return 1;
	}

	for (int i = 1; i < argc; i++) {
		char *filename = argv[i];
		FILE *fp = fopen(filename, "r");
		if (fp == NULL) {
			printf("cannot open file\n");
			return 1;
		}

		// Running current c and count
		char currc = '0';
		int count = 0;

		char c = '0';
		while (fread(&c, sizeof(char), 1, fp) > 0) {
			if (count > 0 && currc != c) {
				fwrite(&count, sizeof(int), 1, stdout);
				fwrite(&currc, sizeof(char), 1, stdout);
				count = 0;
			}
			currc = c;
			count += 1;
		}

		if (count > 0) {
			// Write out remaining running items
			fwrite(&count, sizeof(int), 1, stdout);
			fwrite(&currc, sizeof(char), 1, stdout);
		}

		fclose(fp);
	}
	return 0;
}
