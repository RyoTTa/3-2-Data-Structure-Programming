#define _CRT_SECURE_NO_WARNINGS
#define MAX_HEAP 200

#include <stdio.h>
#include <stdlib.h>

typedef struct node { //Ʈ�� ��� ����ü
	int key;	//Ʈ�� key
	struct node *left;	//Ʈ�� �ڽ� ���� ���
	struct node *right;	//Ʈ�� �ڽ� ������ ���
	struct node *next;	//ť ���� ���
	struct node *previous;	//ť ���� ���
	struct node *parent;	//Ʈ�� �θ� ���
}element;

typedef struct queue_ori {
	struct node *head;	//ť head
	struct node *tail;	//ť tail
}queue;

void push(element *item);
int pop();

int count = 0;
element *head;	//Ʈ���� head
queue *inqueue;	//Ʈ�� push or pop�Ҷ� �켱���� ť

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


	inqueue = (queue*)calloc(1,sizeof(queue));//queue ����ü �����Ҵ�

	scanf("%d", &key);
	for (i = 1; i <= key; i++) {	//Ʈ�� �Է°� ���ÿ� �ʱ�ȭ
		scanf("%d", &j);
		item2 = (element*)calloc(1,sizeof(element));
		item2->key = j;
		push(item2);
	}
	while (1) { //�޴� ����
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
			for (i = 1, j = 1; i <= count; i++) {//heap ��� �Լ�
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

void push(element *item) { //heap���� push �ϴ� �Լ�
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
		if (inqueue->head->left == NULL) {	//linked list ���� �� sort �� ���� ��ũ ����
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
		if ((temp->parent == NULL) || (key >= temp->parent->key)) {	//heqp Ʈ�� sort ����
			break;
		}
		key = temp->key;
		temp->key = temp->parent->key;
		temp->parent->key = key;
		temp = temp->parent;
	}
}

int pop() {	//heap���� pop �ϴ� �Լ�
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

	head->key = inqueue->tail->key;//�켱���� ���峷�� ť�� key�� ��Ʈ ���� �̵�
	if (inqueue->tail->parent->right == NULL) {	//�̵��� ����� �θ� ��� link �Ҵ�����
		inqueue->tail->parent->left = NULL;
	}
	else {
		inqueue->tail->parent->right = NULL;
		inqueue->head = inqueue->head->previous;	//�켱���� ť head �̵�
	}
	inqueue->tail->previous->next = NULL;	//ť���� �켱���� ���� ť�� ���� �� tail �̵�
	inqueue->tail = inqueue->tail->previous;

	temp = head->key;
	etemp = head;

	while (1) {
		if (etemp->left == NULL && etemp->right == NULL) break;	//root ������� �˻�

		if (etemp->right != NULL) {//right ��尡 �ִ��� Ȯ���� ��� �˻�
			if ((etemp->left->key >= etemp->key) && (etemp->right->key >= etemp->key)) break;//sort �Ϸ� �˻�
			if (etemp->left->key >= etemp->right->key) {//right �ڽ� ���� swap
				temp = etemp->key;
				etemp->key = etemp->right->key;
				etemp->right->key = temp;
				etemp = etemp->right;
			}
			else {
				temp = etemp->key;//left �ڽ� ���� swap
				etemp->key = etemp->left->key;
				etemp->left->key = temp;
				etemp = etemp->left;
			}
		}
		else{
			if (etemp->key <= etemp->left) {//left �ڽ� ���� swap
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