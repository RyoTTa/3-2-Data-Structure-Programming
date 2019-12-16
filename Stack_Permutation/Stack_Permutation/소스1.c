#include <stdio.h>

FILE *f;
int arr[] = { 1,2,3,4,5,6 };
int count = 0;

void swap(int *a, int *b) {
	int tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}

void print_arr(int size) {
	count++;
	for (int i = 0; i < size; i++) {
		printf("%d ", arr[i]);
		fprintf(f, "%d ", arr[i]);
	}
	printf("\n");
	fprintf(f,"\n");
}

void permutation(int n, int r, int depth) {

	if (r == depth) {
		print_arr(depth);
		return;
	}

	for (int i = depth; i < n; i++) {
		swap(&arr[i], &arr[depth]);
		permutation(n, r, depth + 1);
		swap(&arr[i], &arr[depth]);
	}
}

int main() {
	f = fopen("output.txt", "w");
	permutation(sizeof(arr) / sizeof(int), 6, 0);
	return 0;
}