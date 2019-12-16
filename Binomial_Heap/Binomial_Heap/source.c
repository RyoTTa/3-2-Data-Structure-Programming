#include<stdio.h>
#include<stdlib.h>

#define MAX_LIST 100

typedef struct node_t {
	struct node_t* child;	//자식노드 포인터
	struct node_t* link_next;	//링크노드 포인터
	int data;	//노드의 값
	int degree;	//자식의 수
}node;

node* nodeListA[MAX_LIST];	//A BHeap의 루트 노드 set
node* nodeListB[MAX_LIST];	//B BHeap의 루트 노드 set
int nodeListCountA = 0;	//A Heap의 루토노드 개수
int nodeListCountB = 0; //A Heap의 루토노드 개수

FILE *fr, *fw;

void fileInput();	//input file로 부터 read와 동시에 nodepush
void nodePush(node* nodeList[], node * newnode, int *nodeListCount);	//node set에 노드 추가 함수
node* nodeMerge(node* A, node* B);	//degree가 같은 node merge 함수
void nodeCheck(node* nodeList[], int *nodeListCount);	//node set에서 degree같은 노드들 확인 후 merge
void nodeSort(node* nodeList[], int *nodeListCount);	//node set에서 degree순으로 sort
void nodePrint(node* node);		//node set의 level order
void nodePop(node* nodeList[],int *nodeListCount);	//node set에서 min값 혹은 max값 출력

int main(int argc, char* argv[]) {

	if (argc != 3) {	//argc error
		fprintf(stderr, "argc error!!\n");
		exit(1);
	}
	fr = fopen(argv[1], "r");
	fw = fopen(argv[2], "w");
	fileInput();
	
	return 0;
}

void fileInput() {	//input file로 부터 read와 동시에 nodepush
	
	node* temp;
	int key, value;

	fscanf(fr,"%d\n", &key);
	printf("Building BHeap of A:\n");
	fprintf(fw,"Building BHeap of A:\n");
	for (int i = 0; i < key; i++) {	//A BHeap 초기화
		temp = calloc(1, sizeof(node));
		fscanf(fr,"%d", &value);
		printf("Inserting %d\n", value);
		fprintf(fw,"Inserting %d\n", value);
		temp->data = value;		//node 초기화
		temp->degree = 0;
		nodePush(nodeListA,temp,&nodeListCountA);
		for (int j = 0; j < nodeListCountA; j++) {	//A BHeap의 level order
			printf("B[%d]: ", nodeListA[j]->degree);
			fprintf(fw,"B[%d]: ", nodeListA[j]->degree);
			nodePrint(nodeListA[j]);
			printf("\n");
			fprintf(fw,"\n");
		}
	}

	fscanf(fr, "\n");
	fscanf(fr, "%d\n", &key);
	printf("\n\nBuilding BHeap of B:\n");
	fprintf(fw,"\n\nBuilding BHeap of B:\n");
	for (int i = 0; i < key; i++) {	//B BHeap 초기화
		temp = calloc(1, sizeof(node));
		fscanf(fr, "%d", &value);
		printf("Inserting %d\n", value);
		fprintf(fw,"Inserting %d\n", value);
		temp->data = value;	//node 초기화
		temp->degree = 0;
		nodePush(nodeListB, temp, &nodeListCountB);
		for (int j = 0; j < nodeListCountB; j++) {	//B BHeap의 level order
			printf("B[%d]: ", nodeListB[j]->degree);
			fprintf(fw,"B[%d]: ", nodeListB[j]->degree);
			nodePrint(nodeListB[j]);
			printf("\n");
			fprintf(fw, "\n");
		}
	}
	printf("\n\nMerging two BHeap of A and B:\n");
	fprintf(fw,"\n\nMerging two BHeap of A and B:\n");
	for (int i = 0; i < nodeListCountB; i++) {	//A, B BHeap의 Merge
		nodePush(nodeListA, nodeListB[i], &nodeListCountA);
	}
	for (int j = 0; j < nodeListCountA; j++) {	//Merge BHeap의 level order
		printf("B[%d]: ", nodeListA[j]->degree);
		fprintf(fw,"B[%d]: ", nodeListA[j]->degree);
		nodePrint(nodeListA[j]);
		printf("\n");
		fprintf(fw,"\n");
	}
	printf("\n\n");
	fprintf(fw,"\n\n");
	nodePop(nodeListA, &nodeListCountA);	//min 또는 max 값 delete

	printf("\n\nBHeap of A after two pop:\n");
	fprintf(fw,"\n\nBHeap of A after two pop:\n");	
	for (int j = 0; j < nodeListCountA; j++) {	//delete 2번한 BHeap의 level order
		printf("B[%d]: ", nodeListA[j]->degree);
		fprintf(fw,"B[%d]: ", nodeListA[j]->degree);
		nodePrint(nodeListA[j]);
		printf("\n");
		fprintf(fw,"\n");
	}
}

void nodePrint(node* node) {
	
	if (node->link_next != NULL) {	//degree가 가장 낮은 노드부터 출력
		nodePrint(node->link_next);
	}
	printf("%d(%d)", node->data, node->degree);
	fprintf(fw,"%d(%d)", node->data, node->degree);	
	if (node->child != NULL) {	//child가 있다면 노드 출력
		nodePrint(node->child);
	}
}

void nodePush(node* nodeList[],node * newnode,int *nodeListCount) {
	
	int degreeCount = 0;
	int check=0;
	node* swap_temp;

	if ((*nodeListCount) == 0) {	//List가 Null일때 초기화
		nodeList[*nodeListCount] = newnode;
		(*nodeListCount)++;
		return;
	}

	for (int i = 0; i < (*nodeListCount); i++) {
		if (nodeList[i]->degree == newnode->degree) {	//같은 degree가 존재할때 Merge
			nodeList[i] = nodeMerge(nodeList[i], newnode);
			nodeCheck(nodeList,nodeListCount);
			nodeSort(nodeList, nodeListCount);
			check = 1;
		}
	}
	if (check == 0) {	//같은 degree가 존재하지 않을경우 뒤에 넣고 Sort
		nodeList[*nodeListCount] = newnode;
		(*nodeListCount)++;
		nodeSort(nodeList,nodeListCount);
	}

}

void nodeSort(node* nodeList[], int *nodeListCount) {

	int i, j;
	int least;
	node* temp;

	for (i = 0; i < (*nodeListCount); i++) {	//node set의 degree로 Sort
		least = i;
		for (j = i + 1; j < (*nodeListCount); j++) {
			if (nodeList[j]->degree < nodeList[least]->degree)
				least = j;
		}
		if (i != least) {
			temp = nodeList[i];
			nodeList[i] = nodeList[least];
			nodeList[least] = temp;
		}
	}
}

node* nodeMerge(node* A, node* B) {	//같은 degree node 의 Merge 함수

	node* swap_temp;

	if (A->data >= B->data) {	//B의 데이터가 작을때 
		if (B->child != NULL) {	//만약 B의 child가 있다면 A의 link_next에 B의 child 접속
			swap_temp = B->child;
			A->link_next = swap_temp;
		}
		B->degree++;
		B->child = A;

		return B;
	}
	else if(A->data < B->data){	//A의 데이터가 작을때
		if (A->child != NULL) {	//만약 A의 child가 있다면 B의 link_next에 A의 child 접속
			swap_temp = A->child;
			B->link_next = swap_temp;
		}
		A->degree++;
		A->child = B;

		return A;
	}
}

void nodeCheck(node* nodeList[], int *nodeListCount) { //nodeList에 degree가 같은 노드가 존재하면 Merge
	
	int i, j, k;

	for (i = 0; i < (*nodeListCount); i++) {
		for (j = 0; j < (*nodeListCount); j++) {
			if ((nodeList[i]->degree == nodeList[j]->degree) && (i != j)) {	//degree가 같은 노드가 존재한다면 두 노드를 merge
				nodeList[i] = nodeMerge(nodeList[i], nodeList[j]);
				for (k = j; k < (*nodeListCount); k++) {
					nodeList[k] = nodeList[k + 1];
				}
				(*nodeListCount)--;
				i = 0;
				j = 0;
				nodeSort(nodeList, nodeListCount);	//다시 sort
			}
		}
	}
}

void nodePop(node* nodeList[], int *nodeListCount) {

	int least = 0;
	int i, j, k;
	node *temp, *temp_child, *child[MAX_LIST];

	for (i = 0; i < (*nodeListCount); i++) {	//min 또는 max값의 index 확인
		if (nodeList[least]->data > nodeList[i]->data) {
			least = i;
		}
	}
	temp = nodeList[least];
	printf("Push : %d\n", temp->data);
	fprintf(fw,"Push : %d\n", temp->data);
	for (k = least; k < (*nodeListCount); k++) {	//출력 이후 set에서 노드 삭제
		nodeList[k] = nodeList[k + 1];
	}
	(*nodeListCount)--;

	if (temp->child != NULL) {
		temp_child = temp->child;
		for (i = 0; i < temp->degree; i++) {
			child[i] = temp_child;
			temp_child = temp_child->link_next;
			child[i]->link_next = NULL;
		}
		for (i = 0; i < temp->degree; i++) {	//node push
			nodePush(nodeList, child[i], nodeListCount);
		}
	}
	free(temp);
}