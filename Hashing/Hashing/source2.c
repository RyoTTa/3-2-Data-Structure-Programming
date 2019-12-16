#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>


#define TABLE_SIZE 26 //hasing table ������
#define MAX_SIZE 500 //pair ���� �ִ� ������ 
typedef struct node_origin { //�ؽ��� ���� Ű �� �� ����ü
	char key;
	char value;
	struct node_origin* next;
}node;

node *table[TABLE_SIZE];	//�ؽ� ���̺�
node pair[MAX_SIZE];	//pair ���̺�
int count=0;	//pair ����
FILE *fw, *fr;	//file pointer

void pair_search(node* temp) { //pair ã�� �Լ�

	node* temp2;
	int index = 0;

	index = char_to_index(temp->value);	//����� �ε��� ������ ��ȯ�Ͽ� �ش� ���̺� �˻� �غ�

	temp2 = table[index];
	while (temp2 != NULL) {	//path�� �ִ��� Ȯ�� 
		if (temp2->value == temp->key) { //path�� �ִٸ� pair ���̺� ����
			pair[count].key = temp2->key;
			pair[count++].value = temp2->value; 
			break;
		}
		temp2 = temp2->next;
	}
}

int char_to_index(char key) { //����� �ε��� ������ ��ȯ
	return key - 'A';
}

void hasing(char key, char value) { //hashing ���̺��� ����� �Լ�
	int index;
	node * temp;
	node * temp2;

	index = char_to_index(key);
	temp = calloc(1, sizeof(node));

	temp->key = key;
	temp->value = value;

	if (table[index] == NULL) { //���̺��� ����ִٸ� ù ����߰�
		table[index] = temp;
	}
	else { //���̺��� ��� ���� �ʴٸ� ������ ����� next�� �߰�
		temp2 = table[index];
		while (temp2->next != NULL) {
			temp2 = temp2->next;
		}
		temp2->next = temp;
	}
	pair_search(temp);

}

int main(int argc, char *argv[]) {	

	int i;
	int vertex, edge; //vertex�� edge ����
	int start, end;	//fscanf�� ���� ����
	
	if (argc != 3) { // argc error
		fputs("Usage : program input output", stderr);
		exit(1);
	}

	if ((fr = fopen(argv[1], "r")) == NULL) { // input file open error
		fputs("Input File open error", stderr);
		exit(1);
	}

	if ((fw = fopen(argv[2], "w")) == NULL) { // output file open error
		fputs("Output File open error", stderr);
		exit(1);
	}
	
	fscanf(fr, "%d %d\n", &vertex, &edge); //vertex, edge �� �޾ƿ���
	
	for (i = 0; i < edge; i++) {	//���Ϸκ��� �д°Ͱ� ���ÿ� �ؽ� ���̺� �����
		fscanf(fr, "%c %c\n", &start, &end);
		hasing(start, end);
	}

	printf("count : %d\n", count);	//���� �� ȭ�� ���
	fprintf(fw,"count : %d\n", count);
	for (i = 0; i < count; i++) {
		printf("%c  %c \n", pair[i].key, pair[i].value);
		fprintf(fw, "%c %c\n", pair[i].key, pair[i].value);
	}
	return 0;
}