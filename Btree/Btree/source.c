#define _CRT_SECURE_NO_WARNINGS

#define M 3	//default �� = 3
#define Fail 1
#define Success 2
#define Insert 3

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int node_counter = 1;

typedef struct _node * nodepointer;
typedef struct _node {
	int id; // ����� �ĺ��� ��ȣ
	int n;  //value �� ����
	int keys[M - 1]; //value �迭
	nodepointer p[M]; //pointer �迭
	nodepointer parent;
}node;
nodepointer root;

FILE *fr, *fw;	//FILE input, output ������

int ins(nodepointer ptr, int key, int *upKey, nodepointer *newnode);	//M=3�� ����Ͽ� Btree�� ������Ű�� ��� Check
int searchPos(int key, int *key_arr, int n); //��� �ȿ� �ִ� keys �迭���� ���� value���� insert ��ġȮ��
void insert_tree(int key);	//key�� �޾� tree�� �� �ش���ġ�� ����
void preorder(nodepointer root, nodepointer parent);  //preorder ����Լ�

int main(int argc, char *argv[]) {
	int num, i, tmp;

	root = NULL;

	if (argc != 3) {	//argc error check
		fprintf(stderr, "argc error!");
	}
	
	fr = fopen(argv[1], "r");
	fw = fopen(argv[2], "w");

	fscanf(fr, "%d", &num);	
	for (i = 0; i < num; i++) {	//input�� �����鼭 Ʈ���� �����ϸ鼭 preorder���
		fscanf(fr, "%d", &tmp);
		printf("\nInserting %d\n", tmp);
		fprintf(fw, "\nInserting %d\n", tmp);
		insert_tree(tmp);
		preorder(root, NULL);
	}
	fclose(fr);
	fclose(fw);

	return 0;
}

void insert_tree(int key)
{
	nodepointer newnode, uproot;
	int upKey;
	int value;

	value = ins(root, key, &upKey, &newnode); //Ű���� �޾� Ʈ���� Ž���ϸ� �μ�Ʈ�� ��ġ�� ����
	
	if (value == Insert)
	{
		uproot = root; //��� 
		root = (nodepointer)malloc(sizeof(node));
		root->n = 1;
		root->keys[0] = upKey;
		root->p[0] = uproot;
		root->p[1] = newnode;
		root->parent = NULL;
		root->p[2] = NULL;
		root->id = node_counter++;
	}
}

int searchPos(int key, int *key_arr, int n)
{
	int pos = 0;
	while (pos < n && key > key_arr[pos])
		pos++;
	return pos; //������ ��ġ ����
}

int ins(nodepointer ptr, int key, int *upKey, nodepointer *newnode)
{
	nodepointer newPtr, lastPtr; //���� �湮 ����� ������
	int newKey, lastKey; //���� �湮 ����� Ű
	int pos, i, n, splitPos;
	int value;

	if (ptr == NULL) //null ����� ����
	{
		*newnode = NULL;
		*upKey = key;
		return Insert;
	}
	n = ptr->n; //���� ����� ������ �� ����
	pos = searchPos(key, ptr->keys, n); 

	value = ins(ptr->p[pos], key, &newKey, &newPtr);
	if (value != Insert) //null ��忡 Insert �Ǹ� ����Լ� ����
		return value;

	if (n < M - 1) //��忡�� Ű ������ �ִ밡 �ƴҶ�
	{
		pos = searchPos(newKey, ptr->keys, n);

		for (i = n; i > pos; i--)
		{
			ptr->keys[i] = ptr->keys[i - 1];
			ptr->p[i + 1] = ptr->p[i];
		}//�� Ű�� �����ϱ� ���� ���� ��ġ�� Ű�� �����͸� ���������� 1ĭ ��

		ptr->keys[pos] = newKey;
		ptr->p[pos + 1] = newPtr;//���� ��ġ�� ���ο� Ű ����
		++ptr->n; //Ű ���� ����
		return Success; //���� ����
	}
	if (pos == M - 1) //����� �����Ͱ� �� ���ְ� ���� ��ġ�� �������̶��
	{
		lastKey = newKey; //������ Ű�� ����
		lastPtr = newPtr; //������ ����� ������
	}else{  //���� ��尡 �� ���ְ� ���� ��ġ�� �������� �ƴ϶��
		lastKey = ptr->keys[M - 2]; 
		lastPtr = ptr->p[M - 1]; 
		for (i = M - 2; i > pos; i--)
		{
			ptr->keys[i] = ptr->keys[i - 1];
			ptr->p[i + 1] = ptr->p[i];
		}                     
		ptr->keys[pos] = newKey;
		ptr->p[pos + 1] = newPtr; //������ ��ġ�� ������ ����
	}
	splitPos = (M - 1) / 2; //������ ��� �� ���
	*upKey = ptr->keys[splitPos];

	*newnode = (nodepointer)calloc(1,sizeof(node));//�� ��� ����
	ptr->n = splitPos; //���� ��ġ�� ����� ������ ���� ������ ����
	(*newnode)->n = M - 1 - splitPos;//������ ����� ������ ���� m�������� ����
	(*newnode)->id = node_counter++;
	for (i = 0; i < (*newnode)->n; i++){
		(*newnode)->p[i] = ptr->p[i + splitPos + 1];
		if (i < (*newnode)->n - 1) {
			(*newnode)->keys[i] = ptr->keys[i + splitPos + 1];
		}
		else {
			(*newnode)->keys[i] = lastKey;
		}
	}
	(*newnode)->p[(*newnode)->n] = lastPtr;
	return Insert;
}

void preorder(nodepointer node, nodepointer parent) {

	if (node == NULL) return;
	if (node->n == 2)
	{
		fprintf(fw, "[%d][p%2d] : n = %d, (%d) ", node->id, parent == NULL ? -1 : parent->id, node->n, ((node->p[1] == NULL) && (node->p[2] == NULL)) ? -1 : node->p[0]->id);
		printf("[%d][p%2d] : n = %d, (%d) ", node->id, parent == NULL ? -1 : parent->id, node->n, ((node->p[1] == NULL) && (node->p[2] == NULL)) ? -1 : node->p[0]->id);
		for (int i = 0; i < node->n; i++) {
			printf("(%d,%d) ", node->keys[i], node->p[i + 1] == NULL ? -1 : node->p[i + 1]->id);
			fprintf(fw, "(%d,%d) ", node->keys[i], node->p[i + 1] == NULL ? -1 : node->p[i + 1]->id);
		}
		printf("\n");
		fprintf(fw, "\n");
		preorder(node->p[0], node);
		preorder(node->p[1], node);
		preorder(node->p[2], node);

	}
	else
	{
		fprintf(fw, "[%d][p%2d] : n = %d, (%d) ", node->id, parent == NULL ? -1 : parent->id, node->n, ((node->p[1] == NULL) && (node->p[2] == NULL)) ? -1 : node->p[0]->id);	//
		printf("[%d][p%2d] : n = %d, (%d) ", node->id, parent == NULL ? -1 : parent->id, node->n, ((node->p[1] == NULL) && (node->p[2] == NULL)) ? -1 : node->p[0]->id);
		for (int i = 0; i < node->n; i++) {
			printf("(%d,%d) ", node->keys[i], node->p[i + 1] == NULL ? -1 : node->p[i + 1]->id);
			fprintf(fw, "(%d,%d) ", node->keys[i], node->p[i + 1] == NULL ? -1 : node->p[i + 1]->id);
		}
		printf("\n");
		fprintf(fw, "\n");
		preorder(node->p[0], node);
		preorder(node->p[1], node);
	}
}

