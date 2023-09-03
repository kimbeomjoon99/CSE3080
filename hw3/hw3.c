#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct node {
	int start;
	int end;
	int data;
	struct node* link;
}node;

int main(int argc, char* argv[]) {
	double start = (double)clock() / CLOCKS_PER_SEC;

	if (argc != 2) {
		printf("usage: ./hw3 input_filename\n");
		return 1;
	}

	FILE* input = fopen(argv[1], "r"); //argv[1]
	if (input == NULL) {
		printf("The input file does not exist.\n");
		return 1;
	}

	int vertices, edges;
	fscanf(input, "%d", &vertices);
	fscanf(input, "%d", &edges);

	int i, j;
	
	int* start_keys = (int*)malloc(sizeof(int) * edges);
	int* end_keys = (int*)malloc(sizeof(int) * edges);
	int* weight_unsorted = (int*)malloc(sizeof(int) * edges);
	int* weight_sorted = (int*)malloc(sizeof(int) * edges);
	node* min_heap = (node*)malloc(sizeof(node) * (edges + 1));
	node* vertices_sorted = (node*)malloc(sizeof(node) * edges);

	//min_heap[0].data = 0;
	for (i = 0; i < edges; i++) {
		int tmp1, tmp2, key;
		fscanf(input, "%d %d %d", &tmp1, &tmp2, &key);
		//printf("%d %d %d\n", tmp1, tmp2, key);
		start_keys[i] = tmp1;
		end_keys[i] = tmp2;
		weight_unsorted[i] = key;
		min_heap[i + 1].data = key;
		min_heap[i + 1].start = tmp1;
		min_heap[i + 1].end = tmp2;
		int p = i + 1;
		while (p != 1) {
			if (min_heap[p].data < min_heap[p / 2].data) {
				int tmp = min_heap[p / 2].data;
				min_heap[p / 2].data = min_heap[p].data;
				min_heap[p].data = tmp;
				tmp = min_heap[p / 2].start;
				min_heap[p / 2].start = min_heap[p].start;
				min_heap[p].start = tmp;
				tmp = min_heap[p / 2].end;
				min_heap[p / 2].end = min_heap[p].end;
				min_heap[p].end = tmp;
				p = p / 2;
			}
			else break;
		}
	}

	int p = edges;
	for (i = 1; i < edges + 1; i++) {
		int parent, child;
		int ans;
		node temp;
		vertices_sorted[i - 1].data = min_heap[1].data;
		vertices_sorted[i - 1].start = min_heap[1].start;
		vertices_sorted[i - 1].end = min_heap[1].end;
		temp = min_heap[p--];
		parent = 1;
		child = 2;
		while (child <= p) {
			if (child < p && (min_heap[child].data > min_heap[child + 1].data)) child++;
			if (temp.data <= min_heap[child].data) break;
			min_heap[parent] = min_heap[child];
			parent = child;
			child *= 2;
		}
		min_heap[parent] = temp;
	}

    //debug ascending sort by minimum heap
	/*
	for (i = 0; i < edges; i++) {
		printf("(%d %d  %d)\n", vertices_sorted[i].start, vertices_sorted[i].end, vertices_sorted[i].data);
	}
    */
	
	FILE* result = fopen("hw3_result.txt", "w");

	int *darr = (int*)malloc(sizeof(int) * vertices);
	for (i = 0; i < vertices; i++){
		darr[i] = -1;
	}

	int cost = 0;
	for (i = 0; i < edges; i++){
		int parent = vertices_sorted[i].start;
		int child = vertices_sorted[i].end;
		
		if (darr[parent] == -1){
			int cflag = 0;
			int croot = child;
			while(1){
				if (darr[croot] == -1) break;
				else if (darr[croot] == parent){
					cflag++;
					break;
				}
				else croot = darr[croot];
			}
			if (cflag) continue;
			if (croot != parent){
				darr[croot] = parent;
				cost += vertices_sorted[i].data;
				fprintf(result, "%d %d %d\n", vertices_sorted[i].start, vertices_sorted[i].end, vertices_sorted[i].data);
			}
			continue;
		}
		else if (darr[child] != -1){
			int pflag = 0, cflag = 0;
			int proot = parent;
			while(1){
				if (darr[proot] == -1) break;
				else if (darr[proot] == child){
					pflag++;
					break;
				}
				else proot = darr[proot];
			}
			int croot = child;
			while(1){
				if (darr[croot] == -1) break;
				else if (darr[croot] == parent){
					cflag++;
					break;
				}
				else croot = darr[croot];
			}
			if (pflag || cflag) continue;
			if (proot != croot){
				if (proot <= croot){
					darr[croot] = proot;
				}
				else {
					darr[proot] = croot;
				}
				cost += vertices_sorted[i].data;
				fprintf(result, "%d %d %d\n", vertices_sorted[i].start, vertices_sorted[i].end, vertices_sorted[i].data);
			}
			continue;
		}
		else {
			int proot = parent;
			int pflag = 0, cflag = 0;
			while(1){
				if (darr[proot] == -1) break;
				else if (darr[proot] == child){
					pflag++;
					break;
				}
				else proot = darr[proot];
			}
			int croot = child;
			while(1){
				if (darr[croot] == -1) break;
				else if (darr[croot] == parent){
					cflag++;
					break;
				}
				else croot = darr[croot];
			}
			if (pflag || cflag) continue;
			if (proot != croot){
				darr[child] = parent;
				cost += vertices_sorted[i].data;
				fprintf(result, "%d %d %d\n", vertices_sorted[i].start, vertices_sorted[i].end, vertices_sorted[i].data);
			}
			continue;
		}
	}
	fprintf(result, "%d\n", cost);
	int connected_flag = -1;
	for (i = 0; i < vertices; i++){
		if (darr[i] == -1){
			connected_flag++;
		}
	}
	if (connected_flag > 0){
		fprintf(result, "DISCONNECTED\n");
	}
	else{
		fprintf(result, "CONNECTED\n");
	}

	fclose(input);
	fclose(result);

	free(darr);
	free(start_keys);
	free(end_keys);
	free(weight_sorted);
	free(weight_unsorted);
	free(min_heap);
	free(vertices_sorted);
	
	printf("output written to hw3_result.txt\n");

	double reach = (double)clock() / CLOCKS_PER_SEC;
	printf("running time: %lf seconds\n", reach - start);

	return 0;
}