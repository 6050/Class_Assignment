// 2018007974_김서연_12838

#include <stdio.h>
#include <stdlib.h>

int main()
{
    int n, m;
    scanf("%d %d", &n, &m);

    int* count = (int*)malloc(sizeof(int)*100001);
    for ( int i = 1; i <= 100000; i++ ) count[i] = 0;

    int key;
    for ( int i = 0; i < n+m; i++ )
    {
        scanf("%d", &key);
        count[key]++;
    }

    int res = 0;
    for ( int i = 1; i <= 100000; i++ )
        if ( count[i] >= 2 ) res++;

    printf("%d", res);

    return 0;

}
