#define _CRT_SECURE_NO_WARNINGS
#define MAX_SIZE 900

#include <stdio.h>
#include <stdlib.h>

typedef struct node_ori {
	char key;	//����� ��
	int num;	//���� ��� ��
	struct node_ori * next;	//���� ���� ���
}node;

node *list;	//���� ����Ʈ 
node stack[MAX_SIZE];	//dfs Ž�� ����
int stack_top = 0;	//���� ž 
int count = 0;	//�� path ����
char source, dest; //����, ���� ��� 
FILE *fw, *fr;	//read write file pointer

void node_insert(char s_edge, node *edge);	//��� �߰�
void node_set();	//���� ����Ʈ �ʱ�ȭ
void dfs(node node);	//��� dfs �Լ� ��� 
int nodeToindex(node node);	//node ���� ���� list������ index ��� �Լ�
void print();	//destination ��� ������ ����Ʈ

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

	list = (node *)calloc(26, sizeof(node));	//26���� ���� ����Ʈ �����Ҵ�

	node_set();	//���� ����Ʈ �ʱ�ȭ

	for(i=0;i<edge;i++) {	//���Ϸ� ���� �Է°� �д°ſ� ���ÿ� ��������Ʈ ����
		temp = (node*)calloc(1, sizeof(node));
		fscanf(fr, "%c %c\n", &s_edge, &d_edge);
		temp->key = d_edge;
		node_insert(s_edge, temp);
	}
	fclose(fr);

	dfs(list[source- 'A']);	//dfs�� ���� simple path search

	fprintf(fw, "total route : %d\n", count);// �� path ���� ���
	printf("total route : %d\n", count);

	return 0;
}
void node_insert(char s_edge,node *edge) { //���� ����Ʈ �߰�
	int i,index=0;
	node *temp;

	for (i = 0; i < 26; i++) {	//source ��������Ʈ search
		if ('A' + i == s_edge) {
			index = i;
		}
	}
	temp = &list[index];
	list[index].num++;
	while (1) {
		if (temp->next == NULL) {	//��������Ʈ ������ ��� next ���� �߰�
			temp->next = edge;
			break;
		}
		else {
			temp = temp->next;
		}
	}
}
void node_set() {	//���� ����Ʈ �ʱ�ȭ
	int i = 0;

	for (i = 0; i < 26; i++) {
		list[i].key = 'A' + i;
	}
}
int nodeToindex(node node) {	//node ���� ���� list������ index ��� �Լ�

	int i = 0;

	for (i = 0; i < 26; i++) {
		if (node.key == 'A' + i) {
			return i; //node�� �ش��ϴ� ��������Ʈ index ����
		}
	}
	return -1;
}

void dfs(node node2) {
	int i;
	node *temp;

	temp = &node2;

	if (temp->key == dest) { // destination�� �����Ͽ��� �� 
		print();
		fprintf(fw, "%c\n", dest);
		printf("%c\n", dest);
		return 0;
	}

	for (i = 1; i <= stack_top; i++) {		//�ߺ� ��� Ž�� ����
		if (stack[i].key == temp->key) {
			return 0;
		}
	}
	stack[++stack_top] = *temp;	//���� push

	for (i = 0; i < node2.num; i++) {
		temp = temp->next;

		dfs(list[nodeToindex(*(temp))]); // next ��� Ž��
	}
	stack_top--; //���� pop
}

void print() { //destination ��� ������ ����Ʈ
	int i;
	for (i = 1; i <= stack_top; i++) {
		printf("%c -> ", stack[i].key); 
		fprintf(fw, "%c -> ", stack[i].key);
	}
	count++;
}
