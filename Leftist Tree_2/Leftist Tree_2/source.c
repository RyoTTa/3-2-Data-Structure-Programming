#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_SIZE 1000 // queue의 size

typedef struct {
	int key;
}element;

typedef struct leftist *leftistTree;
struct leftist { // tree의 node data
	leftistTree leftChild;
	element data;
	leftistTree rightChild;
	int shortest;
};

leftistTree first; // 첫 번째 트리
leftistTree second; // 두 번째 트리

leftistTree queue[MAX_SIZE]; // print를 위한 queue
int rear = 0;
int head = 0;

leftistTree createNode(int data); // create node
void minUnion(leftistTree *a, leftistTree *b); // union
void minMeld(leftistTree *a, leftistTree *b); // meld
void swap(leftistTree *a, leftistTree *b);

void printTree(leftistTree root); // level을 이용하여 print
void enQueue(leftistTree node); // push
leftistTree deQueue(); // pop
int deleteMin(leftistTree *tree);

int height(leftistTree root); // tree의 root에서부터 마지막 node까지의 길이

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
	leftistTree temp = (leftistTree)malloc(sizeof(struct leftist)); // 동적할당 후
	temp->data.key = data; // File에서 읽어온 data를 삽입
	temp->leftChild = NULL;
	temp->rightChild = NULL;
	temp->shortest = 1; // node 하나 이므로 shortest는 1

	return temp; // return
}

void minUnion(leftistTree *a, leftistTree *b) {
	if ((*a)->data.key > (*b)->data.key) { // root끼리 비교
		swap(&(*a), &(*b));
	}

	if (!(*a)->rightChild) // rightChild가 없을 경우, rightChild에 대입
		(*a)->rightChild = *b;
	else
		minUnion(&(*a)->rightChild, b); // 있을 경우, rightChild와 b를 union

	if (!(*a)->leftChild) { // leftChild가 없을 경우
		(*a)->leftChild = (*a)->rightChild; // rightChild를 leftChild로 옮김
		(*a)->rightChild = NULL;
	}
	else if ((*a)->leftChild->shortest < (*a)->rightChild->shortest) { // shortest를 비교 후 큰 걸 왼쪽으로 옮김
		swap(&(*a)->leftChild, &(*a)->rightChild);
	}

	(*a)->shortest = (!(*a)->rightChild) ? 1 : (*a)->rightChild->shortest + 1; // shortest 계산
}

void minMeld(leftistTree *a, leftistTree *b) {
	if (!*a) // root가 없을 경우
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
	int h = height(root); // tree의 root에서부터 마지막 node까지의 길이
	int count = 0, level = 0; // level 출력을 위한 변수
	int i = 0;

	rear = 0; head = 0; // queue 초기화

	enQueue(root); // root push

	printf("%d level : ", level + 1);

	for (i = 1; i < pow(2, h); i++) { // tree의 null 값까지 계산한 크기
		temp = deQueue(); // pop
		if (!temp) { // null값인 경우
			printf("- ");
			rear += 2; // null의 child 역시 null이므로 자식 두개를 push
		}
		else {
			printf("%d ", temp->data.key); // 키 값이 있을 경우

			enQueue(temp->leftChild);
			enQueue(temp->rightChild);
		}
		count++;

		if (pow(2, level) == count) { // 해당 level node의 개수만큼 출력이 되었을 경우
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

int height(leftistTree root) { // tree의 root에서부터 마지막 node까지의 길이
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