// 2018007974_김서연_12838

#include <stdio.h>
#include <stdlib.h>

int size = 0;
int idx = 0;

void swap(int* a, int* b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void maxHeapify(int arr[], int i, int len)
{
    int max, l, r;
    l = 2 * i;
    r = 2 * i + 1;

    if ( r <= len && arr[r] > arr[i] )
        max = r;
    else
        max = i;

    if ( l <= len && arr[l] > arr[max] )
        max = l;

    if ( max != i )
    {
        swap(&arr[i], &arr[max]);
        maxHeapify(arr, max, len);
    }
}

int extractMax(int* arr)
{
    if ( size < 1 ) return -1;

    int max = arr[1];
    arr[1] = arr[size];
    size--;

    maxHeapify(arr, 1, size);

    return max;
}

void heapIncreasesKey(int* arr, int i, int key)
{
    arr[i] = key;

    while ( i > 1 && arr[i / 2] <= arr[i] )
    {
        swap(&arr[i], &arr[i / 2]);
        i /= 2;
    }
}

void heapDecreasesKey(int* arr, int i, int key)
{
    arr[i] = key;

    int max, l, r;
    l = 2 * i;
    r = 2 * i + 1;

    if ( r <= size && arr[r] > arr[i] )
        max = r;
    else
        max = i;

    if ( l <= size && arr[l] > arr[max] )
        max = l;

    if ( max != i )
    {
        swap(&arr[i], &arr[max]);
        maxHeapify(arr, max, size);
    }
}
void maxheapInsert(int* arr, int key)
{
    if ( size == 0 )
    {
        size++;
        arr[1] = key;
        return;
    }

    size++;
    heapIncreasesKey(arr, size, key);
}


int main()
{
    int cmd, i, key, max;
    int* arr = (int*)malloc(sizeof(int) * 100001);
    int* extracted = (int*)malloc(sizeof(int) * 100000);

    while ( true )
    {
        scanf("%d", &cmd);

        if ( cmd == 0 )
        {
            // 결과 출력 후 프로그램 종료

            // 첫 번째 줄: extract된 수들 출력
            for ( int i = 0; i < idx; i++ ) printf("%d ", extracted[i]);
            printf("\n");
            // 두 번째 줄: 현재 heap 출력
            for ( int i = 1; i <= size; i++ ) printf("%d ", arr[i]);

            return 0;
        }
        else if ( cmd == 1 )
        {
            // 다음 입력되는 수를 삽입
            scanf("%d", &key);
            maxheapInsert(arr, key);

        }
        else if ( cmd == 2 )
        {
            // extract max
            max = extractMax(arr);
            if ( max != -1 )
                extracted[idx++] = max;

        }
        else if ( cmd == 3 )
        {
            // 두 번째 값의 인덱스 안의 값을 세 번째 값으로 바꿈
            scanf("%d %d", &i, &key);
            if(arr[i] < key )
                heapIncreasesKey(arr, i, key);
            else
                heapDecreasesKey(arr, i, key);

        }

    }

}
