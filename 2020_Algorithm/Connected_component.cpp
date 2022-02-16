// 2018007974_±è¼­¿¬_12838

#include <stdio.h>
#include <stdlib.h>

// num of vertices
int v; 
// index 0 is not used
//int parent[1001], rank[1001], check[1001];
int *parent, *rank, *check;

int findSet(int x) {

	if (x == parent[x])
		return x;

	return parent[x] = findSet(parent[x]);

}

void Union(int x, int y) {

	x = findSet(x);
	y = findSet(y);

	if (x == y) return;

	if (rank[x] > rank[y]) {

		parent[y] = x;
	
	}
	else {

		parent[x] = y;
		if (rank[x] == rank[y]) rank[y]++;

	}
}

int main() {

	int e;
	scanf("%d %d", &v, &e);

	parent = (int*)malloc(sizeof(int) * (v + 1));
	rank = (int*)malloc(sizeof(int) * (v + 1));
	check = (int*)malloc(sizeof(int) * (v + 1));

	for (int i = 1; i <= v; i++) {
		parent[i] = i;
		rank[i] = i;
		check[i] = 0;
	}

	int x, y;
	for (int i = 0; i < e; i++) {
		
		scanf("%d %d", &x, &y);
		Union(x, y);

	}

	int idx, result = 0;
	for (int i = 1; i <= v; i++) {
		
		idx = i;
		if (check[i] == 0) {

			while (parent[idx] != idx) {

				check[idx] = 1;
				idx = parent[idx];

			}
			if (check[parent[idx]] == 0) result++;
			check[parent[idx]] = 1;
			
		
		}

	}

	free(parent);
	free(rank);
	free(check);

	printf("%d", result);

	return 0;
}