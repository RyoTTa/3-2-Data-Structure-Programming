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

int size; // input file�� ����
FILE *fr, *fw; // file pointer

int color_count = 0;
int rotate_count_ll = 0;
int rotate_count_rr = 0;

int rotate_count_real_ll = 0;
int rotate_count_real_rr = 0;


void inorder(nodePointer node); // inorder ���
void preorder(nodePointer node); // preorder ���
void postorder(nodePointer node);
void file_input(); // �����Է°� ���ÿ� red black tree ����
void node_init(nodePointer *root, int key); // node�� Binary Search Tree ���ǿ� ���� ��ġ�� ����
nodePointer node_grand(nodePointer *root, nodePointer node); // �ش� ��忡���� Grand ��� Search
nodePointer node_uncle(nodePointer *root, nodePointer node); // �ش� ��忡���� Uncle ��� Search
void treeCheck(nodePointer *root, nodePointer node); // node�� �߰��� red black tree ���ǿ� ���� Ȯ��

void leftRotation(nodePointer *root, nodePointer node); // Left Rotation
void rightRotation(nodePointer *root, nodePointer node); // Right rotation

int main(int argc, char *argv[]) {

	if (argc != 3) {
		fprintf(stderr, "argc error\n!");
		exit(-1);
	}
	fr = fopen(argv[1], "r");
	fw = fopen(argv[2], "w");

	file_input(); // �����Է°� ���ÿ� red black tree ���� 

	return 0;
}

void inorder(nodePointer node) {	//inorder ���
	if (node == NULL) return;
	inorder(node->leftChild);
	printf("%d(%c) ", node->key, node->color);
	fprintf(fw, "%d(%c) ", node->key, node->color);
	inorder(node->rightChild);

}

void preorder(nodePointer node) {	//preorder ���
	if (node == NULL) return;

	printf("%d(%c) ", node->key, node->color);
	fprintf(fw, "%d(%c) ", node->key, node->color);
	preorder(node->leftChild);
	preorder(node->rightChild);
}

void postorder(nodePointer node) {	//preorder ���
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
		node_init(&temp, key); // node�� binary search tree�� ���ǿ� �´� ��ġ�� ����

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


void node_init(nodePointer *root, int key) { // tree�� node node_init
	
	nodePointer node, temp, parent;
	
	node = calloc(1, sizeof(struct _node));
	node->key = key;
	node->color = RED;

	if (!*root) { // root�� null�� ��� root�� �Ҵ�
		*root = node;
		(*root)->color = BLACK; 
	}
	else {
		temp = *root;
		parent = NULL;

		while (temp != NULL) { // ���ο� ����� �� ��ġ�� Search
			parent = temp;
			if (temp->key > node->key) {
				temp = temp->leftChild;
			}
			else {
				temp = temp->rightChild;
			}
		}

		if (parent->key > node->key) { // �ش� ��ġ�� ��� ����
			parent->leftChild = node;
		}
		else {
			parent->rightChild = node;
		}
		node->parent = parent;

		treeCheck(root, node); // new node�� ���� tree Ȯ�� �� Rotation
	}
}

void treeCheck(nodePointer *root, nodePointer node) { // red black ���ǿ� �°� tree ����   

	nodePointer uncle, grand, temp;

	temp = node;
	int check = 0;

	while (temp != (*root) && temp->parent->color == RED) { // root�� �ƴϰ�, parent�� red�� ��츸
		uncle = node_uncle(root, temp); 
		grand = node_grand(root, temp); 
		check = 0; // LR �� RL ȸ���� �ߴ��� Ȯ���ϴ� ����

		if (uncle != NULL && uncle->color == RED) { // uncle�� Null�� �ƴϰ� Color�� red��� r Ÿ�� �浹�̹Ƿ� Color�� ��ȯ��Ų��.
			grand->color = RED;
			temp->parent->color = BLACK;
			uncle->color = BLACK;
			temp = grand;
			color_count++;
		}
		else {
			if ((temp == temp->parent->rightChild) && (temp->parent == grand->leftChild)) { // LR�����϶� LL���·� Rotation ��Ų��.
				leftRotation(root, temp->parent);
				temp = temp->leftChild;
				check = 1;
				rotate_count_ll++;
			}
			else if ((temp == temp->parent->leftChild) && (temp->parent == grand->rightChild)) { // RL�����϶� RR���·� Rotation ��Ų��.
				rightRotation(root, temp->parent);
				temp = temp->rightChild;
				check = 1;
				rotate_count_rr++;
			}

			temp->parent->color = BLACK; // �̸� Parent�� ���� Black���� ��ü
			grand->color = RED; // �̸� Grand�� ���� Red�� ��ü

			if (temp == temp->parent->leftChild) { // LL, RRȭ�� tree�� ���ǿ� �°� �ٽ� Rotation
				rightRotation(root, grand);
				rotate_count_real_ll++;
			}
			else {
				leftRotation(root, grand);
				rotate_count_real_rr++;
			}

			if ((*root)->parent) // root�� parent�� ���� ���
				*root = (*root)->parent; // root�� root�� parent�� ����

			if (check == 0) // LR �Ǵ� RL ȸ���� ���� �ʾ��� ���
				temp = temp->parent;

		}
	}
	(*root)->color = BLACK; // root�� ������ �׻� Black
}

node *node_grand(nodePointer *root, nodePointer node) { // node�� Grand Parent�� ã�� �Լ�
	if (node->parent != NULL && node->parent->parent != NULL)
		return node->parent->parent;
	else
		return NULL;
}

node *node_uncle(nodePointer *root, nodePointer node) { // node�� uncle�� ã�� �Լ�
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