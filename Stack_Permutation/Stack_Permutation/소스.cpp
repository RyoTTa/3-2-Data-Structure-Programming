#include <algorithm>
#include <stdio.h>
#include <stack>
#include <queue>
#include <stdio.h>
using namespace std;

int n, temp;
int falsecount = 0;
queue<int> q;
stack<int> s;
queue<char> ans;

int main() {

	int i;
	int arr[6];
	FILE *f = fopen("input.txt", "r");
	
	for (i = 0; i < 1; i++) { 
		//fscanf(f, "%d %d %d %d %d %d\n", &arr[0], &arr[1], &arr[2], &arr[3], &arr[4], &arr[5]);
		arr[0] = 5;
		arr[1] = 4;
		arr[2] = 3;
		arr[3] = 2;
		arr[4] = 1;
		arr[5] = 6;
		n = 6;

		for (int i = 0; i < n; i++) {
			temp = arr[i], q.push(temp); 
		}
		int l = 1;
		while (q.size()) {
			if (s.size() && s.top() == q.front()) {
				s.pop();
				q.pop();
				ans.push('-');
			}
			else if (l <= q.front()) {
				while (l <= q.front()) {
					s.push(l++);
					ans.push('+');
				}
			}
			else {
				printf("NO\n");      
				falsecount++;
				break;
			}
		}
		while (ans.size()) { 
			printf("%c\n", ans.front()), ans.pop(); 
		}
		while (q.size()) q.pop();
		while (s.size()) s.pop();
	}
	printf("false = %d\n", falsecount);
	return 0;
}
