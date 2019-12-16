#define _CRT_SECURE_NO_WARNINGS

#define M 3	//default 값 = 3
#define Fail 1
#define Success 2
#define Insert 3

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int node_counter = 1;

typedef struct _node * nodepointer;
typedef struct _node {
	int id; // 노드의 식별자 번호
	int n;  //value 의 갯수
	int keys[M - 1]; //value 배열
	nodepointer p[M]; //pointer 배열
	nodepointer parent;
}node;
nodepointer root;

FILE *fr, *fw;	//FILE input, output 포인터

int ins(nodepointer ptr, int key, int *upKey, nodepointer *newnode);	//M=3을 사용하여 Btree를 만족시키게 계속 Check
int searchPos(int key, int *key_arr, int n); //노드 안에 있는 keys 배열에서 받은 value값의 insert 위치확인
void insert_tree(int key);	//key를 받아 tree에 서 해당위치에 삽입
void preorder(nodepointer root, nodepointer parent);  //preorder 출력함수

int main(int argc, char *argv[]) {
	int num, i, tmp;

	root = NULL;

	if (argc != 3) {	//argc error check
		fprintf(stderr, "argc error!");
	}
	
	fr = fopen(argv[1], "r");
	fw = fopen(argv[2], "w");

	fscanf(fr, "%d", &num);	
	for (i = 0; i < num; i++) {	//input을 받으면서 트리에 삽입하면서 preorder출력
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

	value = ins(root, key, &upKey, &newnode); //키값을 받아 트리를 탐색하며 인서트할 위치를 선정
	
	if (value == Insert)
	{
		uproot = root; //노드 
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
	return pos; //삽입할 위치 리턴
}

int ins(nodepointer ptr, int key, int *upKey, nodepointer *newnode)
{
	nodepointer newPtr, lastPtr; //이전 방문 노드의 포인터
	int newKey, lastKey; //이전 방문 노드의 키
	int pos, i, n, splitPos;
	int value;

	if (ptr == NULL) //null 노드라면 삽입
	{
		*newnode = NULL;
		*upKey = key;
		return Insert;
	}
	n = ptr->n; //현재 노드의 데이터 수 저장
	pos = searchPos(key, ptr->keys, n); 

	value = ins(ptr->p[pos], key, &newKey, &newPtr);
	if (value != Insert) //null 노드에 Insert 되면 재귀함수 종료
		return value;

	if (n < M - 1) //노드에서 키 개수가 최대가 아닐때
	{
		pos = searchPos(newKey, ptr->keys, n);

		for (i = n; i > pos; i--)
		{
			ptr->keys[i] = ptr->keys[i - 1];
			ptr->p[i + 1] = ptr->p[i];
		}//새 키를 삽입하기 위해 현재 위치의 키와 포인터를 오른쪽으로 1칸 밈

		ptr->keys[pos] = newKey;
		ptr->p[pos + 1] = newPtr;//현재 위치에 새로운 키 삽입
		++ptr->n; //키 갯수 수정
		return Success; //삽입 성공
	}
	if (pos == M - 1) //노드의 데이터가 꽉 차있고 넣을 위치가 마지막이라면
	{
		lastKey = newKey; //삽입할 키를 저장
		lastPtr = newPtr; //삽입할 노드의 포인터
	}else{  //현재 노드가 꽉 차있고 넣을 위치가 마지막이 아니라면
		lastKey = ptr->keys[M - 2]; 
		lastPtr = ptr->p[M - 1]; 
		for (i = M - 2; i > pos; i--)
		{
			ptr->keys[i] = ptr->keys[i - 1];
			ptr->p[i + 1] = ptr->p[i];
		}                     
		ptr->keys[pos] = newKey;
		ptr->p[pos + 1] = newPtr; //지정한 위치에 데이터 삽입
	}
	splitPos = (M - 1) / 2; //데이터 어레이 정 가운데
	*upKey = ptr->keys[splitPos];

	*newnode = (nodepointer)calloc(1,sizeof(node));//새 노드 생성
	ptr->n = splitPos; //현재 위치의 노드의 데이터 수를 반으로 줄임
	(*newnode)->n = M - 1 - splitPos;//생성한 노드의 데이터 수를 m절반으로 설정
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

