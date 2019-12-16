#define _CRT_SECURE_NO_WARNINGS
#define MAX_HEAP 200

#include <stdio.h>
#include <stdlib.h>

typedef struct node { //트리 노드 구조체
	int key;	//트리 key
	struct node *left;	//트리 자식 왼쪽 노드
	struct node *right;	//트리 자식 오른쪽 노드
	struct node *next;	//큐 다음 노드
	struct node *previous;	//큐 이전 노드
	struct node *parent;	//트리 부모 노드
}element;

typedef struct queue_ori {
	struct node *head;	//큐 head
	struct node *tail;	//큐 tail
}queue;

void push(element *item);
int pop();

int count = 0;
element *head;	//트리의 head
queue *inqueue;	//트리 push or pop할때 우선순위 큐

int main(int argc, char* argv[]) {

	int key;
	int input[MAX_HEAP];
	int i, j;
	element item;
	element *item2;

	FILE *f;


	if (argc != 2) {
		fprintf(stderr, "argc error!\n");
		exit(EXIT_FAILURE);
	}

	f = fopen(argv[1], "w"); //file open


	inqueue = (queue*)calloc(1,sizeof(queue));//queue 구조체 동적할당

	scanf("%d", &key);
	for (i = 1; i <= key; i++) {	//트리 입력과 동시에 초기화
		scanf("%d", &j);
		item2 = (element*)calloc(1,sizeof(element));
		item2->key = j;
		push(item2);
	}
	while (1) { //메뉴 실행
		printf("Select menu and input the number (1: insert, 2: delete): ");
		scanf("%d", &key);
		switch (key) {
		case 1:
			scanf("%d", &j);
			item2 = (element*)calloc(1,sizeof(element));
			item2->key = j;
			push(item2);
			break;
		case 2:
			i = pop();
			break;
		default:
			printf("error\n");
		}

		if (count != 0) {
			item2 = head;
			for (i = 1, j = 1; i <= count; i++) {//heap 출력 함수
				printf("%3d", item2->key);
				fprintf(f, "%3d", item2->key);
				if (i == j) {
					printf("\n");
					fprintf(f, "\n");
					j = 2 * j + 1;
				}
				item2 = item2->next;
			}
			printf("\n");
			fprintf(f, "\n\n");
		}
		else {
			printf("\n");
			fprintf(f, "\n\n");
		}
	}
	return 0;
}

void push(element *item) { //heap에서 push 하는 함수
	int i,key;
	element *temp;

	if (count == 0) {
		head = item;
		inqueue->head = item;
		inqueue->tail = item;
		count++;
		return;
	}
	else {
		if (inqueue->head->left == NULL) {	//linked list 구현 및 sort 를 위한 링크 구현
			item->previous = inqueue->tail;
			item->parent = inqueue->head;
			inqueue->head->left = item;
			inqueue->tail->next = item;
			inqueue->tail = item;
		}
		else {
			item->previous = inqueue->tail;
			item->parent = inqueue->head;
			inqueue->head->right = item;
			inqueue->head = inqueue->head->next;
			inqueue->tail->next = item;
			inqueue->tail = item;
		}
		count++;
	}

	key = item->key;
	temp = item;
	while (1) {
		if ((temp->parent == NULL) || (key >= temp->parent->key)) {	//heqp 트리 sort 구현
			break;
		}
		key = temp->key;
		temp->key = temp->parent->key;
		temp->parent->key = key;
		temp = temp->parent;
	}
}

int pop() {	//heap에서 pop 하는 함수
	int parent, child;
	int key, temp;
	int comp1 = 0, comp2 = 0;
	element *etemp;

	count--;
	if (count <= -1) {
		fprintf(stderr, "The heap is empty. \n");
		exit(EXIT_FAILURE);
	}
	else if(count == 0) {
		return 0;
	}

	key = head->key;

	head->key = inqueue->tail->key;//우선순위 가장낮은 큐의 key를 루트 노드로 이동
	if (inqueue->tail->parent->right == NULL) {	//이동된 노드의 부모 노드 link 할당해제
		inqueue->tail->parent->left = NULL;
	}
	else {
		inqueue->tail->parent->right = NULL;
		inqueue->head = inqueue->head->previous;	//우선순위 큐 head 이동
	}
	inqueue->tail->previous->next = NULL;	//큐에서 우선순위 낮은 큐를 삭제 및 tail 이동
	inqueue->tail = inqueue->tail->previous;

	temp = head->key;
	etemp = head;

	while (1) {
		if (etemp->left == NULL && etemp->right == NULL) break;	//root 노드인지 검사

		if (etemp->right != NULL) {//right 노드가 있는지 확인후 대소 검사
			if ((etemp->left->key >= etemp->key) && (etemp->right->key >= etemp->key)) break;//sort 완료 검사
			if (etemp->left->key >= etemp->right->key) {//right 자식 노드랑 swap
				temp = etemp->key;
				etemp->key = etemp->right->key;
				etemp->right->key = temp;
				etemp = etemp->right;
			}
			else {
				temp = etemp->key;//left 자식 노드랑 swap
				etemp->key = etemp->left->key;
				etemp->left->key = temp;
				etemp = etemp->left;
			}
		}
		else{
			if (etemp->key <= etemp->left) {//left 자식 노드랑 swap
				temp = etemp->key;
				etemp->key = etemp->left->key;
				etemp->left->key = temp;
				etemp = etemp->left;
			}
		}
	}
	printf("\n\ncount  =  %d\n\n", count);
	
	return key;
}