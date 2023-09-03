#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char str[9999999];
char pat[2999];

int main() {
	FILE* fstr = fopen("string.txt", "r");
	if (fstr == NULL) {
		fprintf(stderr, "The string file does not exist.\n");
		return 1;
	}
	fgets(str, 9999999, fstr);
	int clo_str = fclose(fstr);

	FILE* fpat = fopen("pattern.txt", "r");
	if (fstr == NULL) {
		fprintf(stderr, "The pattern file does not exist.\n");
		return 1;
	}
	fgets(pat, 2999, fpat);
	int clo_pat = fclose(fpat);
	

	int count, t1, t2;
	count = 0;
	int* ind = malloc(strlen(str) * sizeof(int));
	for (t1 = 0; t1 < strlen(str); t1++) {
		ind[t1] = 0;
	}

	for (t1 = 0; t1 < strlen(str) - strlen(pat) + 1; t1++) {
		char* tmp = malloc((strlen(pat) + 1) * sizeof(char));
		for (t2 = 0; t2 < strlen(pat); t2++) {
			tmp[t2] = str[t1 + t2];
		}
		tmp[strlen(pat)] = '\0'; 
		if (strcmp(tmp, pat) == 0) {
			count++;
			ind[t1] = 1;
		}
		free(tmp);
	}

	FILE* result = fopen("result_naive.txt", "w");
	fprintf(result, "%d\n", count);
	for (t1 = 0; t1 < strlen(str); t1++) {
		if (ind[t1] == 1) {
			fprintf(result, "%d ", t1);
		}
	}
	int clo_result = fclose(result);
	printf("Program complete. Result saved to result_naive.txt\n");

	free(ind);
	return 0;
}
