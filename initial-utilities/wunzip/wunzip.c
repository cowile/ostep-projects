#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	if (argc <= 1) {
		printf("wunzip: file1 [file2 ...]\n");
		return 1;
	}

	for (int i = 1; i < argc; i++) {
		char *filename = argv[i];
		FILE *fp = fopen(filename, "r");
		if (fp == NULL) {
			printf("cannot open file\n");
			return 1;
		}

		int count = 0;
		char c = '0';
		while (fread(&count, sizeof(int), 1, fp) > 0 && fread(&c, sizeof(char), 1, fp) > 0) {
			for (int i = 0; i < count; i++) {
				fwrite(&c, sizeof(char), 1, stdout);
			}
		}
		fclose(fp);
	}
	return 0;
}
