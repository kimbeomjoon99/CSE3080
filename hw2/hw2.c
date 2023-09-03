#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_HEAP 1000001
typedef struct node {
	int data;
}node;
node max_heap[MAX_HEAP];
node min_heap[MAX_HEAP];
node max_copy[MAX_HEAP];
node min_copy[MAX_HEAP];

#define MAX 100
char command[MAX];
char order[MAX];

int main(int argc, char* filename[]) {
	double start = (double)clock() / CLOCKS_PER_SEC;

	if (!(argc == 2)) {
		printf("usage: ./hw2 input_filename\n");
		return 1;
	}

	FILE* input = fopen(filename[1], "r");
	if (input == NULL) {
		printf("The input file does not exist.\n");
		return 1;
	}

	FILE* result = fopen("hw2_result.txt", "w");
	int heap_element = 1;

	int key;

	while (!feof(input)) {
		
		int read_line_flag = 0;
		if(fgets(command, MAX, input) == NULL) continue;

		int i;
		for (i = 0; i < strlen(command); i++) {
			if (command[i] == '\n') {
				command[i] = '\0';
				break;
			}
		}

		for (i = 0; i < strlen(command); i++) {
			if (command[i] == ' ') {
				read_line_flag++;
				break;
			}
		}
		
		if (read_line_flag) {
			sscanf(command, "%s %d", order, &key);
			if (heap_element == MAX_HEAP) break;
			max_heap[heap_element].data = key;
			min_heap[heap_element].data = key;
			if (heap_element == 1) {
				heap_element++;
				continue;
			}
			int p = heap_element;
			while (p != 1) {
				if (max_heap[p].data > max_heap[p / 2].data) {
					int tmp = max_heap[p / 2].data;
					max_heap[p / 2].data = max_heap[p].data;
					max_heap[p].data = tmp;
					p = p / 2;
				}
				else break;
			}
			
			p = heap_element;
			while (p != 1) {
				if (min_heap[p].data < min_heap[p / 2].data) {
					int tmp = min_heap[p / 2].data;
					min_heap[p / 2].data = min_heap[p].data;
					min_heap[p].data = tmp;
					p = p / 2;
				}
				else break;
			}
			heap_element++;
		}
		else {
			int p = heap_element - 1;
			if (!strcmp(command, "ASCEND")) {
				for (i = 1; i <= p; i++) min_copy[i].data = min_heap[i].data;

				for (i = 1; i < heap_element; i++) {
					int parent, child;
					int ans;
					node temp;
					ans = min_copy[1].data;
					temp = min_copy[p--];
					parent = 1;
					child = 2;
					while (child <= p) {
						if ((child < p) && (min_copy[child].data > min_copy[child + 1].data)) child++;
						if (temp.data <= min_copy[child].data) break;
						min_copy[parent] = min_copy[child];
						parent = child;
						child *= 2;
					}
					min_copy[parent] = temp;
					fprintf(result, "%d ", ans);
				}
				fprintf(result, "\n");
			}
			else if (!strcmp(command, "DESCEND")) {
				for (i = 1; i <= p; i++) max_copy[i].data = max_heap[i].data;
				
				for (i = 1; i < heap_element; i++) {
					int parent, child;
					int ans;
					node temp;
					ans = max_copy[1].data;
					temp = max_copy[p--];
					parent = 1;
					child = 2;
					while (child <= p) {
						if ((child < p) && (max_copy[child].data < max_copy[child + 1].data)) child++;
						if (temp.data >= max_copy[child].data) break;
						max_copy[parent] = max_copy[child];
						parent = child;
						child *= 2;
					}
					max_copy[parent] = temp;
					fprintf(result, "%d ", ans);
				}
				fprintf(result, "\n");
			}
		}
	}

	fclose(input); fclose(result);
	double end = (double)clock() / CLOCKS_PER_SEC;

	printf("output written to hw2_result.txt.\n");
	printf("running time: %lf seconds\n", end - start);

	return 0;
}
