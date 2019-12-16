#define _CRT_SECURE_NO_WARNINGS
#define MAX_SIZE 900

#include <stdio.h>
#include <stdlib.h>

typedef struct node_ori {
	char key;	//노드의 값
	int num;	//인접 노드 수
	struct node_ori * next;	//다음 인접 노드
}node;

node *list;	//인접 리스트 
node stack[MAX_SIZE];	//dfs 탐색 스택
int stack_top = 0;	//스택 탑 
int count = 0;	//총 path 갯수
char source, dest; //시작, 도착 노드 
FILE *fw, *fr;	//read write file pointer

void node_insert(char s_edge, node *edge);	//노드 추가
void node_set();	//인접 리스트 초기화
void dfs(node node);	//재귀 dfs 함수 사용 
int nodeToindex(node node);	//node 값을 통해 list에서의 index 얻는 함수
void print();	//destination 노드 도착시 프린트

int main(int argc, char* argv[]) {

	int vertex, edge;
	char s_edge, d_edge;
	node *temp;
	int i;

	if (argc != 3) {
		fprintf(stderr, "argc error\n");
		exit(1);
	}
	fr = fopen(argv[1], "r");
	fw = fopen(argv[2], "w");

	fscanf(fr, "%d %d %c %c\n", &vertex, &edge, &source, &dest);

	list = (node *)calloc(26, sizeof(node));	//26개의 인접 리스트 동적할당

	node_set();	//인접 리스트 초기화

	for(i=0;i<edge;i++) {	//파일로 부터 입력값 읽는거와 동시에 인접리스트 저장
		temp = (node*)calloc(1, sizeof(node));
		fscanf(fr, "%c %c\n", &s_edge, &d_edge);
		temp->key = d_edge;
		node_insert(s_edge, temp);
	}
	fclose(fr);

	dfs(list[source- 'A']);	//dfs를 통해 simple path search

	fprintf(fw, "total route : %d\n", count);// 총 path 갯수 출력
	printf("total route : %d\n", count);

	return 0;
}
void node_insert(char s_edge,node *edge) { //인접 리스트 추가
	int i,index=0;
	node *temp;

	for (i = 0; i < 26; i++) {	//source 인접리스트 search
		if ('A' + i == s_edge) {
			index = i;
		}
	}
	temp = &list[index];
	list[index].num++;
	while (1) {
		if (temp->next == NULL) {	//인접리스트 마지막 노드 next 이후 추가
			temp->next = edge;
			break;
		}
		else {
			temp = temp->next;
		}
	}
}
void node_set() {	//인접 리스트 초기화
	int i = 0;

	for (i = 0; i < 26; i++) {
		list[i].key = 'A' + i;
	}
}
int nodeToindex(node node) {	//node 값을 통해 list에서의 index 얻는 함수

	int i = 0;

	for (i = 0; i < 26; i++) {
		if (node.key == 'A' + i) {
			return i; //node에 해당하는 인접리스트 index 리턴
		}
	}
	return -1;
}

void dfs(node node2) {
	int i;
	node *temp;

	temp = &node2;

	if (temp->key == dest) { // destination에 도착하였을 때 
		print();
		fprintf(fw, "%c\n", dest);
		printf("%c\n", dest);
		return 0;
	}

	for (i = 1; i <= stack_top; i++) {		//중복 노드 탐색 금지
		if (stack[i].key == temp->key) {
			return 0;
		}
	}
	stack[++stack_top] = *temp;	//스택 push

	for (i = 0; i < node2.num; i++) {
		temp = temp->next;

		dfs(list[nodeToindex(*(temp))]); // next 노드 탐색
	}
	stack_top--; //스택 pop
}

void print() { //destination 노드 도착시 프린트
	int i;
	for (i = 1; i <= stack_top; i++) {
		printf("%c -> ", stack[i].key); 
		fprintf(fw, "%c -> ", stack[i].key);
	}
	count++;
}
