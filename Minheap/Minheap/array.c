#define _CRT_SECURE_NO_WARNINGS
#define MAX_HEAP 200

#include <stdio.h>
#include <stdlib.h>

typedef struct node { //트리 노드 배열 구조체
	int key;//트리 key
}element;

element heap[MAX_HEAP]; //트리 배열 


void push(element item, int *n);
element pop(int *n);

int main(int argc, char* argv[]) {

	int key;
	int input[MAX_HEAP];
	int i,j,count=0;
	element item;

	FILE *f;

	if (argc != 2) {
		fprintf(stderr, "argc error!\n");
		exit(EXIT_FAILURE);
	}

	f = fopen(argv[1], "w");

	scanf("%d", &key);
	for (i = 1; i <= key; i++) { //트리 입력과  동시에 초기화
		scanf("%d", &item.key);
		push(item, &count);
	}

	while (1) { //메뉴실행
		printf("Select menu and input the number (1: insert, 2: delete): ");
		scanf("%d", &key);
		switch (key) {
		case 1:
			scanf("%d", &item.key);
			push(item, &count);
			break;
		case 2:
			item = pop(&count);
			break;
		default:
			printf("error\n");
		}

		for (i = 1, j = 1; i <= count; i++) {//heap 출력함수
			printf("%3d", heap[i].key);
			fprintf(f,"%3d", heap[i].key);
			if (i == j) {
				printf("\n");
				fprintf(f,"\n");
				j = 2 * j + 1;
			}
		}
		printf("\n");
		fprintf(f,"\n\n");
	}

	return 0;
}

void push(element item, int *n) {
	int i;
	
	if (*n  >= MAX_HEAP) {
		fprintf(stderr, "The heap is full. \n");
		exit(EXIT_FAILURE);
	}

	i = ++(*n);
	while ((i != 1) && (item.key < heap[i / 2].key)) {
		heap[i] = heap[i / 2];
		i /= 2;
	}
	heap[i] = item;
}

element pop(int *n) {
	int parent, child;
	element item, temp;

	if (*n <= 0) {
		fprintf(stderr, "The heap is empty. \n");
		exit(EXIT_FAILURE);
	}

	item = heap[1];

	temp = heap[(*n)--];
	parent = 1;
	child = 2;
	while (child <= *n) {
		if ((child < *n) && (heap[child].key > heap[child + 1].key)) {
			child++;
		}
		if (temp.key <= heap[child].key) break;
		heap[parent] = heap[child];
		parent = child;
		child *= 2;
	}
	heap[parent] = temp;
	return item;
}