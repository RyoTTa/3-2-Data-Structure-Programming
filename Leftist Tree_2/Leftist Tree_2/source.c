#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_SIZE 1000 // queue�� size

typedef struct {
	int key;
}element;

typedef struct leftist *leftistTree;
struct leftist { // tree�� node data
	leftistTree leftChild;
	element data;
	leftistTree rightChild;
	int shortest;
};

leftistTree first; // ù ��° Ʈ��
leftistTree second; // �� ��° Ʈ��

leftistTree queue[MAX_SIZE]; // print�� ���� queue
int rear = 0;
int head = 0;

leftistTree createNode(int data); // create node
void minUnion(leftistTree *a, leftistTree *b); // union
void minMeld(leftistTree *a, leftistTree *b); // meld
void swap(leftistTree *a, leftistTree *b);

void printTree(leftistTree root); // level�� �̿��Ͽ� print
void enQueue(leftistTree node); // push
leftistTree deQueue(); // pop
int deleteMin(leftistTree *tree);

int height(leftistTree root); // tree�� root�������� ������ node������ ����

int main(int argc, char *argv[]) {
	FILE *inFile;
	FILE *outFile;
	int num, key, i;
	leftistTree node;

	if ((inFile = fopen(argv[1], "r")) == NULL) { // input file open
		fputs("Input File open error", stderr);
		exit(1);
	}
	if ((outFile = fopen(argv[2], "w")) == NULL) { // output file open
		fputs("Output File open error", stderr);
		exit(1);
	}

	fscanf(inFile, "%d", &num);

	for (i = 0; i < num; i++) {
		fscanf(inFile, "%d", &key);
		node = createNode(key);
		minMeld(&first, &node);
	}
	printf("First Tree\n");
	printTree(first);

	fscanf(inFile, "%d", &num);

	/*
	for (i = 0; i < num; i++) {
	   fscanf(inFile, "%d", &key);
	   node = createNode(key);
	   minMeld(&second, &node);
	}
	printf("Second Tree\n");
	printTree(second);

	minMeld(&first, &second); // first tree + second tree

	printf("Union Tree\n");
	printTree(first);
	*/
}

leftistTree createNode(int data) {
	leftistTree temp = (leftistTree)malloc(sizeof(struct leftist)); // �����Ҵ� ��
	temp->data.key = data; // File���� �о�� data�� ����
	temp->leftChild = NULL;
	temp->rightChild = NULL;
	temp->shortest = 1; // node �ϳ� �̹Ƿ� shortest�� 1

	return temp; // return
}

void minUnion(leftistTree *a, leftistTree *b) {
	if ((*a)->data.key > (*b)->data.key) { // root���� ��
		swap(&(*a), &(*b));
	}

	if (!(*a)->rightChild) // rightChild�� ���� ���, rightChild�� ����
		(*a)->rightChild = *b;
	else
		minUnion(&(*a)->rightChild, b); // ���� ���, rightChild�� b�� union

	if (!(*a)->leftChild) { // leftChild�� ���� ���
		(*a)->leftChild = (*a)->rightChild; // rightChild�� leftChild�� �ű�
		(*a)->rightChild = NULL;
	}
	else if ((*a)->leftChild->shortest < (*a)->rightChild->shortest) { // shortest�� �� �� ū �� �������� �ű�
		swap(&(*a)->leftChild, &(*a)->rightChild);
	}

	(*a)->shortest = (!(*a)->rightChild) ? 1 : (*a)->rightChild->shortest + 1; // shortest ���
}

void minMeld(leftistTree *a, leftistTree *b) {
	if (!*a) // root�� ���� ���
		*a = *b;
	else if (*b)
		minUnion(a, b);

	*b = NULL;
}

void swap(leftistTree *a, leftistTree *b) {
	leftistTree *temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

void printTree(leftistTree root) {
	leftistTree temp;
	int h = height(root); // tree�� root�������� ������ node������ ����
	int count = 0, level = 0; // level ����� ���� ����
	int i = 0;

	rear = 0; head = 0; // queue �ʱ�ȭ

	enQueue(root); // root push

	printf("%d level : ", level + 1);

	for (i = 1; i < pow(2, h); i++) { // tree�� null ������ ����� ũ��
		temp = deQueue(); // pop
		if (!temp) { // null���� ���
			printf("- ");
			rear += 2; // null�� child ���� null�̹Ƿ� �ڽ� �ΰ��� push
		}
		else {
			printf("%d ", temp->data.key); // Ű ���� ���� ���

			enQueue(temp->leftChild);
			enQueue(temp->rightChild);
		}
		count++;

		if (pow(2, level) == count) { // �ش� level node�� ������ŭ ����� �Ǿ��� ���
			count = 0;
			level++;
			if (level != h)
				printf("\n%d level : ", level + 1);
		}
	}

	printf("\n\n");
}

void enQueue(leftistTree node) { // push
	queue[rear++] = node;
}

leftistTree deQueue() { // pop
	return queue[head++];
}

int deleteMin(leftistTree *tree) {
	leftistTree left, right, temp;
	int min = (*tree)->data.key;

	left = (*tree)->leftChild;
	right = (*tree)->rightChild;
	*tree = left;
	minMeld(tree, &right);

	return min;
}

int height(leftistTree root) { // tree�� root�������� ������ node������ ����
	if (root == NULL)
		return 0;
	else {
		int lheight = height(root->leftChild);
		int rheight = height(root->rightChild);

		if (lheight > rheight)
			return (lheight + 1);
		else
			return (rheight + 1);
	}
}