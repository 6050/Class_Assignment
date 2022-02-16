// 2018007974_김서연_12838

#include <stdio.h>
#include <stdlib.h>

int main()
{
    int n, m, k;
    scanf("%d %d %d", &n, &m, &k);

    int* key = (int*)malloc(sizeof(int)*n);
    int* count = (int*)malloc(sizeof(int)*m);
    int* range = (int*)malloc(sizeof(int)*k*2);

    for ( int i = 0; i < k * 2; i += 2 ) scanf("%d %d", &range[i], &range[i + 1]);
    for ( int i = 1; i <= n; i++ ) scanf("%d", &key[i]);

    // count 배열 적절히 초기화: key 배열 counting
    for ( int i = 1; i <= m; i++ ) count[i] = 0;
    for ( int i = 1; i <= n; i++ ) count[key[i]]++;
    for ( int i = 2; i <= m; i++ ) count[i] += count[i - 1];

    for ( int i = 0; i < k * 2; i += 2 )
        printf("%d\n", count[range[i+1]] - count[range[i] - 1]);

}
