#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSTR 9999999
#define MAXPAT 2999

char string[MAXSTR];
char pat[MAXPAT];
int failure[MAXPAT];
char tmp[MAXSTR];
char tmppat[MAXPAT];
char tmpstring[MAXSTR];

int pmatch(char*, char*);
void fail(char*);


int main() {
	FILE* fstr = fopen("string.txt", "r");
	if (fstr == NULL) {
		fprintf(stderr, "The string file does not exist.\n");
		return 1;
	}
	fgets(string, MAXSTR, fstr);
	int len = strlen(string);
	int clo_str = fclose(fstr);

	FILE* fpat = fopen("pattern.txt", "r");
	if (fstr == NULL) {
		fprintf(stderr, "The pattern file does not exist.\n");
		return 1;
	}
	fgets(pat, MAXPAT, fpat);
	int clo_pat = fclose(fpat);

	strcpy(tmppat, pat);
	strcpy(tmpstring, string);
	
	int t1, pind, count = 0;
	strcpy(tmp, string);
	fail(pat);
	int* ind = malloc(strlen(string) * sizeof(int));
	for (t1 = 0; t1 < strlen(string); t1++) {
		ind[t1] = 0;
	}
	
	pind = pmatch(string, pat);
	FILE* result = fopen("result_kmp.txt", "w");
	if (pind == -1) {
		fprintf(result, "0\n");
		int clo_result = fclose(result);
		return 1;
	}
	count++;
	ind[pind] = 1;
	
	int tmpind = pind + strlen(pat);
	int plen = strlen(pat);
	int slen = strlen(string);
	strncpy(tmp, string + pind + 1, slen - pind - 1);
	
	strcpy(pat, tmppat);
	strcpy(string, tmpstring);

	while (1) {
		tmpind = pind + 1;
		pind = pmatch(tmp, pat);
		if (pind == -1) {
			break;
		}
		pind = pind + tmpind;
		ind[pind] = 1;
		count++;
		
		strncpy(tmp, string + pind + 1, string + slen);
		strcpy(pat, tmppat);
		strcpy(string, tmpstring);
	}
	
	
	
	fprintf(result, "%d\n", count);
	for (t1 = 0; t1 < len; t1++) {
		if (ind[t1] == 1) {
			fprintf(result, "%d ", t1);
		}
	}
	int clo_result = fclose(result);
	printf("Program complete. Result saved to result_kmp.txt\n");

	free(ind);
	return 0;
}

void fail(char* pat) {
	int i, j, n = strlen(pat);
	failure[0] = -1;
	for (j = 1; j < n; j++) {
		i = failure[j - 1];
		while ((pat[j] != pat[i + 1]) && (i >= 0)) i = failure[i];
		if (pat[j] == pat[i + 1]) failure[j] = i + 1;
		else failure[j] = -1;
	}
}

int pmatch(char* string, char* pat) {
	int i = 0, j = 0;
	int lens = strlen(string);
	int lenp = strlen(pat);
	while (i < lens && j < lenp) {
		if (string[i] == pat[j]) { i++; j++; }
		else if (j == 0) i++;
		else j = failure[j - 1] + 1;
	}
	
	return ((j == lenp) ? (i - lenp) : -1);
}
