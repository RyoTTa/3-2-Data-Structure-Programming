#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>


#define TABLE_SIZE 26 //hasing table 사이즈
#define MAX_SIZE 500 //pair 저장 최대 사이즈 
typedef struct node_origin { //해싱을 위한 키 및 값 구조체
	char key;
	char value;
	struct node_origin* next;
}node;

node *table[TABLE_SIZE];	//해싱 테이블
node pair[MAX_SIZE];	//pair 테이블
int count=0;	//pair 개수
FILE *fw, *fr;	//file pointer

void pair_search(node* temp) { //pair 찾는 함수

	node* temp2;
	int index = 0;

	index = char_to_index(temp->value);	//밸류를 인덱스 값으로 변환하여 해당 테이블 검색 준비

	temp2 = table[index];
	while (temp2 != NULL) {	//path가 있는지 확인 
		if (temp2->value == temp->key) { //path가 있다면 pair 테이블에 저장
			pair[count].key = temp2->key;
			pair[count++].value = temp2->value; 
			break;
		}
		temp2 = temp2->next;
	}
}

int char_to_index(char key) { //밸류를 인덱스 값으로 변환
	return key - 'A';
}

void hasing(char key, char value) { //hashing 테이블을 만드는 함수
	int index;
	node * temp;
	node * temp2;

	index = char_to_index(key);
	temp = calloc(1, sizeof(node));

	temp->key = key;
	temp->value = value;

	if (table[index] == NULL) { //테이블이 비어있다면 첫 노드추가
		table[index] = temp;
	}
	else { //테이블이 비어 있지 않다면 마지막 노드의 next에 추가
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
	int vertex, edge; //vertex와 edge 개수
	int start, end;	//fscanf를 위한 변수
	
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
	
	fscanf(fr, "%d %d\n", &vertex, &edge); //vertex, edge 을 받아오기
	
	for (i = 0; i < edge; i++) {	//파일로부터 읽는것과 동시에 해싱 테이블 만들기
		fscanf(fr, "%c %c\n", &start, &end);
		hasing(start, end);
	}

	printf("count : %d\n", count);	//파일 및 화면 출력
	fprintf(fw,"count : %d\n", count);
	for (i = 0; i < count; i++) {
		printf("%c  %c \n", pair[i].key, pair[i].value);
		fprintf(fw, "%c %c\n", pair[i].key, pair[i].value);
	}
	return 0;
}