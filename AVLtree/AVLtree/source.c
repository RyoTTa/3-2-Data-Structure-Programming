#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TRUE 1
#define FALSE 0

typedef   struct {	//Data의 구조체
	int key;
} element;

typedef struct _node *treePointer;

struct _node {   // min leftistTree 구조체 선언
	treePointer leftChild;
	element data;
	treePointer rightChild;
	int bf;
	int height;
}node; 

void print_post(treePointer a);
void print_pre(treePointer a);	//preorder로 노드 출력 함수
void print_in(treePointer a);	//inorder로 노드 출력 함수
void leftRotation(treePointer *parent, int *unbalanced);	//Left Rotation을 위한 함수
void rightRotation(treePointer *parent, int *unbalanced); //Right Rotation을 위한 함수
void avlInsert(treePointer *parent, int x, int *unbalanced);	//AVL tree Insert 함수

treePointer root;	//root 노드 포인터
int unbalanced;	//balanced or unbalanced 확인 
FILE *fr, *fw;	//file 포인터


int main(int argc, char *argv[]) {

	int size_a, i;
	int tmp;
	if (argc != 3){ //argc error
		fprintf(stderr, "argc error!!\n");
		exit(1);
	}
	fr = fopen(argv[1], "r");
	fw = fopen(argv[2], "w");

	fscanf(fr, "%d", &size_a); //input file로부터 사이즈 입력
	for (i = 0; i < size_a; i++) {
		fscanf(fr, "%d", &tmp);
		avlInsert(&root, tmp, &unbalanced);
		printf("AVL Tree : \nPreorder : ");
		fprintf(fw, "AVL Tree : \nPreorder : ");
		print_pre(root);
		printf("\ninorder : ");
		fprintf(fw, "\ninorder : ");
		print_in(root); 
		printf("\npostorder : ");
		fprintf(fw, "\npostorder : ");
		print_post(root);
		printf("\n");
		fprintf(fw, "\n");
	}
	fclose(fr);
	fclose(fw);
}

void leftRotation(treePointer *parent, int *unbalanced)
{
	treePointer grandChild, child;
	child = (*parent)->leftChild;

	if (child->bf == 1) {  //LL 회전
		(*parent)->leftChild = child->rightChild;
		child->rightChild = *parent;
		(*parent)->bf = 0;
		(*parent) = child;
	}
	else {  //LR 회전
		grandChild = child->rightChild;
		child->rightChild = grandChild->leftChild;
		grandChild->leftChild = child;
		(*parent)->leftChild = grandChild->rightChild;
		grandChild->rightChild = *parent;

		switch (grandChild->bf) {
		case 1://1일 경우, 자식노드 균형, 부모노드 -1
			(*parent)->bf = -1;
			child->bf = 0;
			break;
		case 0://0일 경우, 부모, 자식 0
			(*parent)->bf = child->bf = 0;
			break;
		case -1://-1의 경우, 부모 균형, 자식노드 1
			(*parent)->bf = 0;
			child->bf = 1;
			break;
		}
		*parent = grandChild;
	}

	(*parent)->bf = 0;
	*unbalanced = FALSE;
}
void rightRotation(treePointer *parent, int *unbalanced)
{
	treePointer grandChild, child;
	child = (*parent)->rightChild;

	if (child->bf == -1) {//RR 회전
		(*parent)->rightChild = child->leftChild;
		child->leftChild = *parent;
		(*parent)->bf = 0;
		(*parent) = child;
	}
	else {//RL 회전
		grandChild = child->leftChild;
		child->leftChild = grandChild->rightChild;
		grandChild->rightChild = child;
		(*parent)->rightChild = grandChild->leftChild;
		grandChild->leftChild = *parent;

		switch (grandChild->bf) {
		case 1://1일 경우, 부모 균형, 자식 -1
			(*parent)->bf = 0;
			child->bf = -1;
			break;
		case 0://0일 경우, 모두 0
			(*parent)->bf = child->bf = 0;
			break;
		case -1://-1의 경우, 부모 1, 자식 균형
			(*parent)->bf = 1;
			child->bf = 0;
			break;
		}
		*parent = grandChild;
	}

	(*parent)->bf = 0;
	*unbalanced = FALSE;
}
void avlInsert(treePointer *parent, int x, int *unbalanced)
{
	if (!*parent) { //자식이 없다면 현재위치에 새 노드 삽입
		*unbalanced = TRUE;
		*parent = (treePointer)malloc(sizeof(node));
		(*parent)->leftChild = (*parent)->rightChild = NULL;
		(*parent)->bf = 0; (*parent)->data.key = x;
	}
	else if (x < (*parent)->data.key) { //Input data가 현재 노드의 data보다 작다면
		avlInsert(&(*parent)->leftChild, x, unbalanced); //자식이 존재하면 재귀
		if (*unbalanced)
			switch ((*parent)->bf) { //노드 삽입후 bf값을 수정, blance가 무너지면 로테이션 실행
			case -1: (*parent)->bf = 0;
				*unbalanced = FALSE; break;
			case 0: (*parent)->bf = 1; break;
			case 1:
				leftRotation(parent, unbalanced);
			}
	}
	else if (x > (*parent)->data.key) { //Input data가 현재 노드의 data보다 크다면
		avlInsert(&((*parent)->rightChild), x, unbalanced);//자식이 존재하면 재귀
		if (*unbalanced)
			switch ((*parent)->bf) { //노드 삽입후 bf값을 수정, blance가 무너지면 로테이션 실행
			case 1: (*parent)->bf = 0;
				*unbalanced = FALSE; break;
			case 0: (*parent)->bf = -1; break;
			case -1:
				rightRotation(parent, unbalanced);
			}
	}
	else {
		*unbalanced = FALSE;
		printf("The key is already in the tree");
	}
}
void print_pre(treePointer a)//out file에 preorder 노드 출력
{
	if (a != NULL) {
		printf("%d(%d) ", a->data.key, a->bf);
		fprintf(fw, "%d(%d) ", a->data.key, a->bf);
		print_pre(a->leftChild);
		print_pre(a->rightChild);
	}
}
void print_in(treePointer a)//out file에 inorder 노드 출력
{
	if (a != NULL) {
		print_in(a->leftChild);
		printf("%d(%d) ", a->data.key, a->bf);
		fprintf(fw, "%d(%d) ", a->data.key, a->bf);
		print_in(a->rightChild);
	}
}

void print_post(treePointer a)//out file에 inorder 노드 출력
{
	if (a != NULL) {
		print_post(a->leftChild);
		print_post(a->rightChild);
		printf("%d(%d) ", a->data.key, a->bf);
		fprintf(fw, "%d(%d) ", a->data.key, a->bf);
	}
}