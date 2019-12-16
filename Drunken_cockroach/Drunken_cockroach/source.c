#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {

	int i, j, k;
	int **mat, m, n;		//mat : 매트릭스 , m,n : 매트릭스 크기	, 
	int row, column;	//벌레 위치
	int randi, randj;
	unsigned long visit = 0, result[1000] = { 0, }, result2 = 0;	//visit : 완료 확인, result[] = 총 횟수 배열
	int imove[8] = { -1,0,1,1,1,0,-1,-1 };
	int jmove[8] = { 1,1,1,0,-1,-1,-1,0 };
	FILE *fp = fopen(argv[3], "w");

	if (argc != 4) {		//argc 확인
		printf("[Error] argc\n");
		return 0;
	}
	m = atoi(argv[1]);
	n = atoi(argv[2]);
	if (m <= 0 || n <= 0) {	//m, n 확인
		printf("[Error] m or n < 0");
		return 0;
	}

	mat = (int**)calloc(m, sizeof(int*));
	for (i = 0; i < n; i++) {
		*(mat + i) = (int *)calloc(n, sizeof(int));
	}

	srand(time(NULL));

	for (i = 0; i < 1000; i++) {
		for (k = 0; k < m; k++) {
			for (j = 0; j < n; j++) {
				mat[k][j] = 0;
			}
		}
		visit = 0;
		row = m / 2;
		column = n / 2;
		mat[row][column]++;
		visit++;
		result[i]++;
		while (visit != m * n) {
			randi = rand() % 8;
			randj = rand() % 8;
			if (row + imove[randi] >= 0 && column + jmove[randj] >= 0 && row + imove[randi] < m && column + jmove[randj] < n) {
				row = row + imove[randi];
				column = column + jmove[randj];
				if (mat[row][column] == 0) {
					visit++;
				}
				mat[row][column]++;
				result[i]++;
			}
		}
		result2 = result2 + result[i];
	}

	fprintf(fp, "%d %d\n", row, column);
	printf("%d %d \n", row, column);

	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) {
			printf("%5d", mat[i][j]);
			fprintf(fp, "%5d", mat[i][j]);
		}
		printf("\n");
		fprintf(fp, "\n");
	}

	printf("%.1lf\n", (float)result2 / 1000);
	fprintf(fp, "%.1lf\n", (float)result2 / 1000);
	fclose(fp);
	
	return 0;
}