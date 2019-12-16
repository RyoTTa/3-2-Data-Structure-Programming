#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 26 // hash table�� size

typedef struct _data { // �����͸� ���� ����ü
	char key; // key ��
	char value; // value ��
	struct _data *next; // collision �߻� ��, ���ʿ� ���ο� data�� �����ϱ� ���� link
}Data;

Data *table[HASH_SIZE]; // hash table

int count = 0;
int vertex, edge; // vertex, edge�� ����
FILE *inFile; // input File
FILE *outFile; // output File

int findKey(char key); // hash table�� index�� ã��
void hashing(char start, char end); // hash table�� ����
void checkExist(Data *data); // ����� ������ �̷�����ٸ� print

int main(int argc, char *argv[]) {
	int i;
	char start, end;
	if (argc != 3) { // argc error
		fputs("Usage : program input output", stderr);
		exit(1);
	}

	if ((inFile = fopen(argv[1], "r")) == NULL) { // input file open error
		fputs("Input File open error", stderr);
		exit(1);
	}

	if ((outFile = fopen(argv[2], "w")) == NULL) { // output file open error
		fputs("Output File open error", stderr);
		exit(1);
	}

	fscanf(inFile, "%d %d\n", &vertex, &edge);

	memset(table, 0, sizeof(table)); // table �� �ʱ�ȭ

	for (i = 0; i < edge; i++) {
		fscanf(inFile, "%c %c\n", &start, &end); // input File���� read
		hashing(start, end); // read�� �����͸� hash table�� ����
	}

	printf("Pair : %d\n", count);
	fprintf(outFile, "Pair : %d\n", count);

	fclose(inFile);
	fclose(outFile);

	return 0;
}

int findKey(char key) {
	return key - 'A'; // hash table�� �����ϱ� ���� index return
}

void hashing(char start, char end) {
	int num;
	Data *temp;
	Data *data = (Data*)malloc(sizeof(Data));
	data->key = start;
	data->value = end;
	data->next = NULL;

	num = findKey(start);

	if (table[num] == NULL) { // key�� �ش��ϴ� table�� �����Ͱ� ���� ���
		table[num] = data;
	}
	else { // �����Ͱ� ���� ���
		temp = table[num];
		while (temp->next != NULL)
			temp = temp->next;

		temp->next = data; // �ش� �ε����� ���� �������� ����
	}

	checkExist(data);
}

void checkExist(Data *data) { // ������ �̷�������� Ȯ�� �� ����Ʈ
	Data *temp = table[findKey(data->value)]; // ã���� �ϴ� key ���� table���� load

	if (temp == NULL) // table�� �ش� data�� ���� ��� return
		return;

	while (1) {
		if (temp->value == data->key) { // �����Ͱ� ������ ��� print
			printf("%c %c\n", temp->key, temp->value);
			fprintf(outFile, "%c %c\n", temp->key, temp->value);
			count++;
		}

		if (temp->next == NULL) // �ش� key�� �� �̻� �����Ͱ� ���� ��� break
			break;
		temp = temp->next;
	}
}