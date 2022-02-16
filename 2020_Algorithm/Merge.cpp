// 2018007974_김서연_12838

#include <stdio.h>
#include <stdlib.h>

void Merge(int* arr, int l, int m, int r)
{
    int* newArr = (int*)malloc(sizeof(int)*(r + 1));

    int idx = l, i = l, j = m + 1;
    while ( i <= m && j <= r )
    {
        if ( arr[i] >= arr[j] )
            newArr[idx++] = arr[i++];
        else
            newArr[idx++] = arr[j++];
    }

    int k;
    if ( i > m )
        for ( k = j; k <= r; k++ ) newArr[idx++] = arr[k];
    else
        for ( k = i; k <= m; k++ ) newArr[idx++] = arr[k];

    for ( k = l; k <= r; k++ ) arr[k] = newArr[k];

    free(newArr);
}

void MergeSort(int* arr, int l, int r)
{
    if ( l < r )
    {

        int m = (l + r) / 2;
        MergeSort(arr, l, m);
        MergeSort(arr, m + 1, r);
        Merge(arr, l, m, r);

    }

}

int main()
{
    int size;
    scanf("%d", &size);

    int* arr = (int*)malloc(sizeof(int)*(size + 1));

    int i;
    for ( i = 1; i < size + 1; i++ )
        scanf("%d", &arr[i]);

    MergeSort(arr, 1, size);

    for ( i = 1; i < size + 1; i++ )
        printf("%d\n", arr[i]);

    return 0;

}
