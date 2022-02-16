#include <stdio.h>
#include <stdlib.h>

void InsertionSort(int* arr, int len)
{
    int i, j, key;
    for ( j = 2; j <= len; j++ )
    {
        key = arr[j];
        i = j - 1;

        while ( i > 0 && arr[i] < key )
        {
            arr[i + 1] = arr[i];
            i--;
        }
       
        arr[i + 1] = key;
    }

    for ( i = 1; i < len + 1; i++ )
        printf("%d\n", arr[i]);

}

int main()
{
    int size;
    scanf("%d", &size);

    int* arr = (int*)malloc(sizeof(int)*size);
    
    for ( int i = 1; i < size + 1; i++ )
        scanf("%d", &arr[i]);

    InsertionSort(arr, size);

    return 0;

}
