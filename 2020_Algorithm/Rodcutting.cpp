// 2018007974_김서연_12838

#include <stdio.h>
#include <stdlib.h>

void rodCutting(int* p, int* r, int* s, int len)
{
    
    for ( int j = 1; j < len + 1; j++ )
    {
        int q = -1;
        for ( int i = 1; i < j + 1; i++ )
        {
            if ( q < p[i] + r[j - i] )
            {
                q = p[i] + r[j - i];
                s[j] = i;
            }
            r[j] = q;
        }
    }

}

void printSolution(int* p, int* r, int* s, int len)
{
    while ( len > 0 )
    {
        printf("%d ", s[len]);
        len -= s[len];
    }
    printf("\n");
}

int main()
{
    int len;
    scanf("%d", &len);

    // index 0 is not used
    int* p = (int*)malloc(sizeof(int)*(len + 1));
    p[0] = 0;
    for ( int i = 1; i < len + 1; i++ )
        scanf("%d", &p[i]);

    int* r = (int*)malloc(sizeof(int)*(len + 1));
    int* s = (int*)malloc(sizeof(int)*(len + 1));
    r[0] = 0;

    rodCutting(p, r, s, len);

    printf("%d\n", r[len]);
    printSolution(p, r, s, len);

    return 0;

}
