// 2018007974_김서연_12838

#include <stdio.h>
#include <stdlib.h>

int size;

void maxHeapify(int arr[], int i, int len)
{

    int max, l, r;
    l = 2 * i;
    r = 2 * i + 1;

    if ( l <= len && arr[l] > arr[i] )
        max = l;
    else
        max = i;

    if ( r <= len && arr[r] > arr[max] )
        max = r;
   
   // arr[i]랑 arr[max]를 swap
    int tmp;
    if ( max != i )
    {
        tmp = arr[i];
        arr[i] = arr[max];
        arr[max] = tmp;

        maxHeapify(arr, max, len);
    }
    
}

void buildMaxheap(int* arr)
{
    for ( int i = size / 2; i >= 1; i-- )
        maxHeapify(arr, i, size);
    
}

int extractMax(int* arr)
{
    if ( size < 1 ) return -1;

    int max = arr[1];
    arr[1] = arr[size];
    size -= 1;

    maxHeapify(arr, 1, size);

    return max;
}

int main()
{
    int n, k;
    scanf("%d %d", &n, &k);
    size = n;

    int* arr = (int*)malloc(sizeof(int)*n);
    for ( int i = 1; i <= n; i++ ) scanf("%d", &arr[i]);
   
    buildMaxheap(arr);

    size = n;
    int max;
    // extract max k번 하고 나서 결과 출력
    for ( int i = 0; i < k; i++ )
    {
        max = extractMax(arr);
        printf("%d ", max);
    }

    printf("\n");
    for ( int i = 1; i <= size; i++ ) printf("%d ", arr[i]);

    return 0;
}
