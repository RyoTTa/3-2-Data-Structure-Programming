#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TRUE 1
#define FALSE 0

typedef   struct {	//Data�� ����ü
	int key;
} element;

typedef struct _node *treePointer;

struct _node {   // min leftistTree ����ü ����
	treePointer leftChild;
	element data;
	treePointer rightChild;
	int bf;
	int height;
}node; 

void print_post(treePointer a);
void print_pre(treePointer a);	//preorder�� ��� ��� �Լ�
void print_in(treePointer a);	//inorder�� ��� ��� �Լ�
void leftRotation(treePointer *parent, int *unbalanced);	//Left Rotation�� ���� �Լ�
void rightRotation(treePointer *parent, int *unbalanced); //Right Rotation�� ���� �Լ�
void avlInsert(treePointer *parent, int x, int *unbalanced);	//AVL tree Insert �Լ�

treePointer root;	//root ��� ������
int unbalanced;	//balanced or unbalanced Ȯ�� 
FILE *fr, *fw;	//file ������


int main(int argc, char *argv[]) {

	int size_a, i;
	int tmp;
	if (argc != 3){ //argc error
		fprintf(stderr, "argc error!!\n");
		exit(1);
	}
	fr = fopen(argv[1], "r");
	fw = fopen(argv[2], "w");

	fscanf(fr, "%d", &size_a); //input file�κ��� ������ �Է�
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

	if (child->bf == 1) {  //LL ȸ��
		(*parent)->leftChild = child->rightChild;
		child->rightChild = *parent;
		(*parent)->bf = 0;
		(*parent) = child;
	}
	else {  //LR ȸ��
		grandChild = child->rightChild;
		child->rightChild = grandChild->leftChild;
		grandChild->leftChild = child;
		(*parent)->leftChild = grandChild->rightChild;
		grandChild->rightChild = *parent;

		switch (grandChild->bf) {
		case 1://1�� ���, �ڽĳ�� ����, �θ��� -1
			(*parent)->bf = -1;
			child->bf = 0;
			break;
		case 0://0�� ���, �θ�, �ڽ� 0
			(*parent)->bf = child->bf = 0;
			break;
		case -1://-1�� ���, �θ� ����, �ڽĳ�� 1
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

	if (child->bf == -1) {//RR ȸ��
		(*parent)->rightChild = child->leftChild;
		child->leftChild = *parent;
		(*parent)->bf = 0;
		(*parent) = child;
	}
	else {//RL ȸ��
		grandChild = child->leftChild;
		child->leftChild = grandChild->rightChild;
		grandChild->rightChild = child;
		(*parent)->rightChild = grandChild->leftChild;
		grandChild->leftChild = *parent;

		switch (grandChild->bf) {
		case 1://1�� ���, �θ� ����, �ڽ� -1
			(*parent)->bf = 0;
			child->bf = -1;
			break;
		case 0://0�� ���, ��� 0
			(*parent)->bf = child->bf = 0;
			break;
		case -1://-1�� ���, �θ� 1, �ڽ� ����
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
	if (!*parent) { //�ڽ��� ���ٸ� ������ġ�� �� ��� ����
		*unbalanced = TRUE;
		*parent = (treePointer)malloc(sizeof(node));
		(*parent)->leftChild = (*parent)->rightChild = NULL;
		(*parent)->bf = 0; (*parent)->data.key = x;
	}
	else if (x < (*parent)->data.key) { //Input data�� ���� ����� data���� �۴ٸ�
		avlInsert(&(*parent)->leftChild, x, unbalanced); //�ڽ��� �����ϸ� ���
		if (*unbalanced)
			switch ((*parent)->bf) { //��� ������ bf���� ����, blance�� �������� �����̼� ����
			case -1: (*parent)->bf = 0;
				*unbalanced = FALSE; break;
			case 0: (*parent)->bf = 1; break;
			case 1:
				leftRotation(parent, unbalanced);
			}
	}
	else if (x > (*parent)->data.key) { //Input data�� ���� ����� data���� ũ�ٸ�
		avlInsert(&((*parent)->rightChild), x, unbalanced);//�ڽ��� �����ϸ� ���
		if (*unbalanced)
			switch ((*parent)->bf) { //��� ������ bf���� ����, blance�� �������� �����̼� ����
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
void print_pre(treePointer a)//out file�� preorder ��� ���
{
	if (a != NULL) {
		printf("%d(%d) ", a->data.key, a->bf);
		fprintf(fw, "%d(%d) ", a->data.key, a->bf);
		print_pre(a->leftChild);
		print_pre(a->rightChild);
	}
}
void print_in(treePointer a)//out file�� inorder ��� ���
{
	if (a != NULL) {
		print_in(a->leftChild);
		printf("%d(%d) ", a->data.key, a->bf);
		fprintf(fw, "%d(%d) ", a->data.key, a->bf);
		print_in(a->rightChild);
	}
}

void print_post(treePointer a)//out file�� inorder ��� ���
{
	if (a != NULL) {
		print_post(a->leftChild);
		print_post(a->rightChild);
		printf("%d(%d) ", a->data.key, a->bf);
		fprintf(fw, "%d(%d) ", a->data.key, a->bf);
	}
}