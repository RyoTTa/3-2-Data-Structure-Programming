#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 26 // hash table의 size

typedef struct _data { // 데이터를 담을 구조체
	char key; // key 값
	char value; // value 값
	struct _data *next; // collision 발생 시, 뒤쪽에 새로운 data를 연결하기 위한 link
}Data;

Data *table[HASH_SIZE]; // hash table

int count = 0;
int vertex, edge; // vertex, edge의 개수
FILE *inFile; // input File
FILE *outFile; // output File

int findKey(char key); // hash table의 index를 찾음
void hashing(char start, char end); // hash table에 저장
void checkExist(Data *data); // 양방향 연결이 이루어졌다면 print

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

	memset(table, 0, sizeof(table)); // table 값 초기화

	for (i = 0; i < edge; i++) {
		fscanf(inFile, "%c %c\n", &start, &end); // input File에서 read
		hashing(start, end); // read한 데이터를 hash table에 삽입
	}

	printf("Pair : %d\n", count);
	fprintf(outFile, "Pair : %d\n", count);

	fclose(inFile);
	fclose(outFile);

	return 0;
}

int findKey(char key) {
	return key - 'A'; // hash table에 저장하기 위한 index return
}

void hashing(char start, char end) {
	int num;
	Data *temp;
	Data *data = (Data*)malloc(sizeof(Data));
	data->key = start;
	data->value = end;
	data->next = NULL;

	num = findKey(start);

	if (table[num] == NULL) { // key에 해당하는 table에 데이터가 없을 경우
		table[num] = data;
	}
	else { // 데이터가 있을 경우
		temp = table[num];
		while (temp->next != NULL)
			temp = temp->next;

		temp->next = data; // 해당 인덱스의 제일 마지막에 저장
	}

	checkExist(data);
}

void checkExist(Data *data) { // 연결이 이루어졌는지 확인 후 프린트
	Data *temp = table[findKey(data->value)]; // 찾고자 하는 key 값을 table에서 load

	if (temp == NULL) // table에 해당 data가 없을 경우 return
		return;

	while (1) {
		if (temp->value == data->key) { // 데이터가 존재할 경우 print
			printf("%c %c\n", temp->key, temp->value);
			fprintf(outFile, "%c %c\n", temp->key, temp->value);
			count++;
		}

		if (temp->next == NULL) // 해당 key에 더 이상 데이터가 없을 경우 break
			break;
		temp = temp->next;
	}
}