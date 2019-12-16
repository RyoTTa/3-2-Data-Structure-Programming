#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#define MAX 11

int c[MAX][MAX];
int w[MAX][MAX];
int r[MAX][MAX];
int KnuthMin(int i, int j);
void main()
{
	//int p[11] = { 0,1,3,5,1,8,2,3,8,2,4 };
	//int q[11] = { 2,2,2,3,9,4,6,2,7,3,5 };
	int n = 4;
	int N = 5;

	int p[5] = { 0,7,5,2,6 };
	int q[5] = { 5,4,3,6,9 };

	for (int i = 0; i < n; i++) {
		w[i][i] = q[i]; r[i][i] = c[i][i] = 0; // 초기화
		w[i][i + 1] = q[i] + q[i + 1] + p[i + 1]; // 한 노드를 갖는 최적 트리
		r[i][i + 1] = i + 1;
		c[i][i + 1] = w[i][i + 1];
	}
	w[n][n] = q[n]; r[n][n] = c[n][n] = 0;
	for (int m = 2; m <= n; m++) // m개의 노드를 갖는 최적 트리 탐색
		for (int i = 0; i <= n - m; i++)
		{
			int j = i + m;
			w[i][j] = w[i][j - 1] + p[j] + q[j];
			int k = KnuthMin(i, j);
			// KnuthMin은 범위 [r[i, j-1], r[i+1, j]] 사이의 값 k를 반환한다.
			// 이때 c[i, k-1] + c[k, j] 는 최소가 된다.
			c[i][j] = w[i][j] + c[i][k - 1] + c[k][j]; // 식 (10.3)
			r[i][j] = k;
		}
	printf("w : \n");
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			printf("%2d ", w[i][j]);
		}
		puts("");
	}
	printf("c : \n");
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			printf("%2d ", c[i][j]);
		}
		puts("");
	}
	printf("r : \n");
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			printf("%2d ", r[i][j]);
		}
		puts("");
	}
}

int KnuthMin(int i, int j)
{
	int min = 9999;
	int index;
	for (int k = i + 1; k <= j; k++)
	{
		if (c[i][k - 1] + c[k][j] < min)
		{
			min = c[i][k - 1] + c[k][j];
			index = k;
		}
	}
	return index;
}
//optimalBst 계산