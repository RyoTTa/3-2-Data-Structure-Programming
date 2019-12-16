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
			printf("[%d] [10���� : %d]", i, queue[i]->data.key);
			print_binary(queue[i]->data.key);
			printf("(%d) [%d,%d]\n", queue[i]->bitNumber, queue[i]->leftChild->id, -1);
			continue;
		}
		printf("[%d] [10���� : %d]", i, queue[i]->data.key);
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

// Ž�� �Լ� �־��� key ���� ã�´�
patricia search(patricia t, unsigned k)
{
	patricia currentNode, nextNode;

	// ��Ʈ�� ��������� ����ó��
	if (!t) {
		printf("root is null!\n");
		return;
	}

	// ���� ��� ������ͼ�
	nextNode = t->leftChild;
	currentNode = t;

	// ã�������� ���� ���ѹ� �� ��ȯ
	while (nextNode->bitNumber > currentNode->bitNumber) {
		// ���� ��� �̵�
		currentNode = nextNode;
		nextNode = (bit(k, nextNode->bitNumber)) ?
			nextNode->rightChild : nextNode->leftChild;
	}
	// ã�� ��� ��ȯ
	return nextNode;
}

void insert(patricia *t, element theElement)
{ /* insert theElement into the Patricia tree *t  */
	patricia current, parent, lastNode, newNode;
	int i;

	// Ʈ���� ����ִٸ�
	if (!(*t)) {
		// �Ҵ� �� ù ���
		*t = (patricia)calloc(1, sizeof(patriciaTree));
		(*t)->bitNumber = 0; (*t)->data = theElement;
		(*t)->leftChild = *t; // lch�� �ڱ� �ڽ�
		(*t)->id = count;
		queue[count] = (*t);
		count++;
		return;
	}

	// ���� ���� ���� ���
	lastNode = search(*t, theElement.key);
	if (theElement.key == lastNode->data.key) {
		printf("�ش� Ű�� �̹� �����մϴ�!\n");
		return;
	}

	// ó������ �ٸ� ��Ʈ���� ��ġ ã�ƿ���
	for (i = 1; bit(theElement.key, i) == bit(lastNode->data.key, i); i++);

	// Ž���� ���� ����
	current = (*t)->leftChild;
	parent = *t;

	// Ž��
	while (current->bitNumber > parent->bitNumber && current->bitNumber < i) {
		parent = current;
		current = (bit(theElement.key, current->bitNumber)) ?
			current->rightChild : current->leftChild;
	}
	// ���ο� ��带 �Ҵ��� ��
	newNode = (patricia)calloc(1, sizeof(patriciaTree));
	newNode->id = count;
	newNode->data = theElement;
	newNode->bitNumber = i; // ������ üũ�� ��
	newNode->leftChild = (bit(theElement.key, i)) ? current : newNode;
	newNode->rightChild = (bit(theElement.key, i)) ? newNode : current;
	queue[count] = newNode;
	count++;

	// ��� �������� üũ �� ����
	if (current == parent->leftChild)
		parent->leftChild = newNode;
	else
		parent->rightChild = newNode;
}

// �ش� key ������ bitNumber�� ° �� ��ȯ
int bit(unsigned key, int bitNumber) {

	unsigned a = 1 << 4 - 1; // 1000 
	unsigned b = a >> bitNumber - 1; // ����� ��Ʈ�ΰ�

	// �ش� bit�� ��ȯ
	return key & b;
}
