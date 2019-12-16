#include <stdio.h>
#include <stdlib.h>
#define BLACK 'B'
#define RED 'R'

typedef struct _node {
	int key;
	char color;
	struct _node *leftChild;
	struct _node *rightChild;
	struct _node *parent;
}node;
typedef node* nodePointer;

int size; // input file의 개수
FILE *fr, *fw; // file pointer

int color_count = 0;
int rotate_count_ll = 0;
int rotate_count_rr = 0;

int rotate_count_real_ll = 0;
int rotate_count_real_rr = 0;


void inorder(nodePointer node); // inorder 출력
void preorder(nodePointer node); // preorder 출력
void postorder(nodePointer node);
void file_input(); // 파일입력과 동시에 red black tree 구성
void node_init(nodePointer *root, int key); // node를 Binary Search Tree 정의에 대한 위치에 삽입
nodePointer node_grand(nodePointer *root, nodePointer node); // 해당 노드에대한 Grand 노드 Search
nodePointer node_uncle(nodePointer *root, nodePointer node); // 해당 노드에대한 Uncle 노드 Search
void treeCheck(nodePointer *root, nodePointer node); // node를 추가후 red black tree 정의에 대한 확인

void leftRotation(nodePointer *root, nodePointer node); // Left Rotation
void rightRotation(nodePointer *root, nodePointer node); // Right rotation

int main(int argc, char *argv[]) {

	if (argc != 3) {
		fprintf(stderr, "argc error\n!");
		exit(-1);
	}
	fr = fopen(argv[1], "r");
	fw = fopen(argv[2], "w");

	file_input(); // 파일입력과 동시에 red black tree 구성 

	return 0;
}

void inorder(nodePointer node) {	//inorder 출력
	if (node == NULL) return;
	inorder(node->leftChild);
	printf("%d(%c) ", node->key, node->color);
	fprintf(fw, "%d(%c) ", node->key, node->color);
	inorder(node->rightChild);

}

void preorder(nodePointer node) {	//preorder 출력
	if (node == NULL) return;

	printf("%d(%c) ", node->key, node->color);
	fprintf(fw, "%d(%c) ", node->key, node->color);
	preorder(node->leftChild);
	preorder(node->rightChild);
}

void postorder(nodePointer node) {	//preorder 출력
	if (node == NULL) return;

	postorder(node->leftChild);
	postorder(node->rightChild);
	printf("%d(%c) ", node->key, node->color);
	fprintf(fw, "%d(%c) ", node->key, node->color);
}

void file_input() {
	int i, key;
	nodePointer *temp;

	temp = NULL;

	fscanf(fr, "%d", &size); 

	for (i = 0; i < size; i++) {
		fscanf(fr, "%d", &key); 
		node_init(&temp, key); // node의 binary search tree의 정의에 맞는 위치에 삽입

		printf("Insert %d\n", key);   
		fprintf(fw, "Insert %d\n", key);
		printf("Inorder : ");      
		fprintf(fw, "Inorder : ");
		inorder(temp);
		printf("\nPreorder : ");   
		fprintf(fw, "\nPreorder : ");
		preorder(temp);
		printf("\nPostorder : ");
		fprintf(fw, "\nPostorder : ");
		postorder(temp);
		printf("\n\n");            
		fprintf(fw, "\n\n");
	}
	printf("count = %d\n", color_count);
	printf("count_ll = %d\n", rotate_count_real_ll);
	printf("count_rr = %d\n", rotate_count_real_rr);
}


void node_init(nodePointer *root, int key) { // tree에 node node_init
	
	nodePointer node, temp, parent;
	
	node = calloc(1, sizeof(struct _node));
	node->key = key;
	node->color = RED;

	if (!*root) { // root가 null일 경우 root에 할당
		*root = node;
		(*root)->color = BLACK; 
	}
	else {
		temp = *root;
		parent = NULL;

		while (temp != NULL) { // 새로운 노드의 들어갈 위치를 Search
			parent = temp;
			if (temp->key > node->key) {
				temp = temp->leftChild;
			}
			else {
				temp = temp->rightChild;
			}
		}

		if (parent->key > node->key) { // 해당 위치에 노드 삽입
			parent->leftChild = node;
		}
		else {
			parent->rightChild = node;
		}
		node->parent = parent;

		treeCheck(root, node); // new node에 대한 tree 확인 및 Rotation
	}
}

void treeCheck(nodePointer *root, nodePointer node) { // red black 정의에 맞게 tree 수정   

	nodePointer uncle, grand, temp;

	temp = node;
	int check = 0;

	while (temp != (*root) && temp->parent->color == RED) { // root가 아니고, parent가 red인 경우만
		uncle = node_uncle(root, temp); 
		grand = node_grand(root, temp); 
		check = 0; // LR 및 RL 회전을 했는지 확인하는 변수

		if (uncle != NULL && uncle->color == RED) { // uncle이 Null이 아니고 Color가 red라면 r 타입 충돌이므로 Color만 변환시킨다.
			grand->color = RED;
			temp->parent->color = BLACK;
			uncle->color = BLACK;
			temp = grand;
			color_count++;
		}
		else {
			if ((temp == temp->parent->rightChild) && (temp->parent == grand->leftChild)) { // LR상태일때 LL상태로 Rotation 시킨다.
				leftRotation(root, temp->parent);
				temp = temp->leftChild;
				check = 1;
				rotate_count_ll++;
			}
			else if ((temp == temp->parent->leftChild) && (temp->parent == grand->rightChild)) { // RL상태일때 RR상태로 Rotation 시킨다.
				rightRotation(root, temp->parent);
				temp = temp->rightChild;
				check = 1;
				rotate_count_rr++;
			}

			temp->parent->color = BLACK; // 미리 Parent의 색을 Black으로 교체
			grand->color = RED; // 미리 Grand의 색을 Red로 교체

			if (temp == temp->parent->leftChild) { // LL, RR화된 tree를 정의에 맞게 다시 Rotation
				rightRotation(root, grand);
				rotate_count_real_ll++;
			}
			else {
				leftRotation(root, grand);
				rotate_count_real_rr++;
			}

			if ((*root)->parent) // root의 parent가 생긴 경우
				*root = (*root)->parent; // root를 root의 parent로 설정

			if (check == 0) // LR 또는 RL 회전을 하지 않았을 경우
				temp = temp->parent;

		}
	}
	(*root)->color = BLACK; // root의 색깔은 항상 Black
}

node *node_grand(nodePointer *root, nodePointer node) { // node의 Grand Parent를 찾는 함수
	if (node->parent != NULL && node->parent->parent != NULL)
		return node->parent->parent;
	else
		return NULL;
}

node *node_uncle(nodePointer *root, nodePointer node) { // node의 uncle을 찾는 함수
	nodePointer grand;

	grand = node_grand(root, node);

	if (node->parent == grand->leftChild)
		return grand->rightChild;
	else
		return grand->leftChild;
}

void leftRotation(nodePointer *root, nodePointer node) { // left rotation
	nodePointer temp, parent;

	temp = node->rightChild;
	parent = node->parent;

	node->rightChild = temp->leftChild;
	node->parent = temp;
	temp->leftChild = node;
	temp->parent = parent;

	if (parent != NULL) {
		if (parent->leftChild == node)
			parent->leftChild = temp;
		else
			parent->rightChild = temp;
	}
}

void rightRotation(nodePointer *root, nodePointer node) { // right rotation
	nodePointer temp, parent;

	temp = node->leftChild;
	parent = node->parent;

	node->leftChild = temp->rightChild;
	node->parent = temp;
	temp->rightChild = node;
	temp->parent = parent;

	if (parent != NULL) {
		if (parent->rightChild == node)
			parent->rightChild = temp;
		else
			parent->leftChild = temp;
	}
}