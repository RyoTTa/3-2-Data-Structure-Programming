#include <stdio.h>
#include <stdlib.h>

typedef struct element {
	unsigned key;
}element;
typedef struct patriciaTree * patricia;
typedef struct patriciaTree {
	int id;
	int bitNumber;
	element data;
	patricia leftChild, rightChild;
}patriciaTree;

patricia root;
int count = 0;
patricia queue[999] = {NULL,};


patricia search(patricia t, unsigned k);
void insert(patricia *t, element theElement);
int bit(unsigned key, int bitNumber);
void node_print(patricia t);
void print_binary(unsigned input);

void main() {
	
	int i, j, key;
	FILE *fp;
	element temp;

	root = NULL;
	fp = fopen("input.txt", "r");

	fscanf(fp, "%d\n", &j);
	for (i = 0; i < j; i++) {
		fscanf(fp, "%d", &key);
		temp.key = key;
		insert(&root, temp);
		printf("Inserting : ");
		print_binary(key);
		printf("\n");
		node_print(root);
	}
}

void node_print(patricia t) {

	for (int i = 0; i < count; i++) {
		if (i == 0) {
			printf("[%d] [10진수 : %d]", i, queue[i]->data.key);
			print_binary(queue[i]->data.key);
			printf("(%d) [%d,%d]\n", queue[i]->bitNumber, queue[i]->leftChild->id, -1);
			continue;
		}
		printf("[%d] [10진수 : %d]", i, queue[i]->data.key);
		print_binary(queue[i]->data.key);
		printf("(%d) [%d,%d]\n", queue[i]->bitNumber, queue[i]->leftChild->id, queue[i]->rightChild->id);

	}
}

void print_binary(unsigned input) {
	int mask;


	for (int i = 3; i >= 0; i--) { 
		mask = 1 << i; 
		printf("%d", input & mask ? 1 : 0); 
		if (i % 8 == 0) printf(" "); 
	}


}

// 탐색 함수 주어진 key 값을 찾는다
patricia search(patricia t, unsigned k)
{
	patricia currentNode, nextNode;

	// 루트가 비어있으면 에러처리
	if (!t) {
		printf("root is null!\n");
		return;
	}

	// 다음 노드 가지고와서
	nextNode = t->leftChild;
	currentNode = t;

	// 찾을때까지 가서 비교한번 후 반환
	while (nextNode->bitNumber > currentNode->bitNumber) {
		// 다음 노드 이동
		currentNode = nextNode;
		nextNode = (bit(k, nextNode->bitNumber)) ?
			nextNode->rightChild : nextNode->leftChild;
	}
	// 찾은 노드 반환
	return nextNode;
}

void insert(patricia *t, element theElement)
{ /* insert theElement into the Patricia tree *t  */
	patricia current, parent, lastNode, newNode;
	int i;

	// 트리가 비어있다면
	if (!(*t)) {
		// 할당 후 첫 노드
		*t = (patricia)calloc(1, sizeof(patriciaTree));
		(*t)->bitNumber = 0; (*t)->data = theElement;
		(*t)->leftChild = *t; // lch는 자기 자신
		(*t)->id = count;
		queue[count] = (*t);
		count++;
		return;
	}

	// 만약 값이 있을 경우
	lastNode = search(*t, theElement.key);
	if (theElement.key == lastNode->data.key) {
		printf("해당 키가 이미 존재합니다!\n");
		return;
	}

	// 처음으로 다른 비트값의 위치 찾아오기
	for (i = 1; bit(theElement.key, i) == bit(lastNode->data.key, i); i++);

	// 탐색을 위한 세팅
	current = (*t)->leftChild;
	parent = *t;

	// 탐색
	while (current->bitNumber > parent->bitNumber && current->bitNumber < i) {
		parent = current;
		current = (bit(theElement.key, current->bitNumber)) ?
			current->rightChild : current->leftChild;
	}
	// 새로운 노드를 할당한 후
	newNode = (patricia)calloc(1, sizeof(patriciaTree));
	newNode->id = count;
	newNode->data = theElement;
	newNode->bitNumber = i; // 위에서 체크한 값
	newNode->leftChild = (bit(theElement.key, i)) ? current : newNode;
	newNode->rightChild = (bit(theElement.key, i)) ? newNode : current;
	queue[count] = newNode;
	count++;

	// 어디 연결할지 체크 후 연결
	if (current == parent->leftChild)
		parent->leftChild = newNode;
	else
		parent->rightChild = newNode;
}

// 해당 key 값에서 bitNumber번 째 값 반환
int bit(unsigned key, int bitNumber) {

	unsigned a = 1 << 4 - 1; // 1000 
	unsigned b = a >> bitNumber - 1; // 몇번쨰 비트인가

	// 해당 bit값 반환
	return key & b;
}
