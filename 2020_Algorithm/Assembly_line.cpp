// 2018007974_김서연_12838

#include <stdio.h>
#include <stdlib.h>

void assembly_line(int** a, int** t, int* e, int* x,
                   int n, int** s, int** l)
{

    s[1][1] = e[1] + a[1][1];
    s[2][1] = e[2] + a[2][1];

    for ( int i = 2; i < n + 1; i++ )
    {
        if ( s[1][i - 1] <= s[2][i - 1] + t[2][i - 1] )
        {
            s[1][i] = s[1][i - 1] + a[1][i];
            l[1][i] = 1;
        }
        else
        {
            s[1][i] = s[2][i - 1] + t[2][i - 1] + a[1][i];
            l[1][i] = 2;
        }

        if ( s[2][i - 1] <= s[1][i - 1] + t[1][i - 1] )
        {
            s[2][i] = s[2][i - 1] + a[2][i];
            l[2][i] = 2;
        }
        else
        {
            s[2][i] = s[1][i - 1] + t[1][i - 1] + a[2][i];
            l[2][i] = 1;
        }
    }
    
    if ( s[1][n] + x[1] <= s[2][n] + x[2] )
    {
        s[0][0] = s[1][n] + x[1];
        l[0][0] = 1;
    }
    else
    {
        s[0][0] = s[2][n] + x[2];
        l[0][0] = 2;
    }

}

void printStaitons(int** l, int n)
{

    int* result = (int*)malloc(sizeof(int)*(n+1));
    int i = l[0][0];
    result[n] = i;

    for ( int j = n; j > 1; j-- )
    {
        i = l[i][j];
        result[j-1] = i;
    }

    for ( int i = 1; i < n + 1; i++ )
        printf("%d %d\n", result[i], i);

}

int main()
{

    int n;
    scanf("%d", &n);

    // index 0 is not used
    int* e = (int*)malloc(sizeof(int) * 3);
    int* x = (int*)malloc(sizeof(int) * 3);
    scanf("%d %d %d %d", &e[1], &e[2], &x[1], &x[2]);

    // index 0 is not used
    int** a = (int**)malloc(sizeof(int*)*(3));
    for ( int i = 1; i < 3; i++ )
        a[i] = (int*)malloc(sizeof(int)*(n + 1));

    for ( int i = 1; i < 3; i++ )
        for ( int j = 1; j < n + 1; j++ )
            scanf("%d", &a[i][j]);

    // index 0 is not used
    int** t = (int**)malloc(sizeof(int*)*(3));
    for ( int i = 1; i < 3; i++ )
        t[i] = (int*)malloc(sizeof(int)*n);

    for ( int i = 1; i < 3; i++ )
        for ( int j = 1; j < n; j++ )
            scanf("%d", &t[i][j]);

    // index 0 = s*(s[0][0]) / l*(l[0][0])
    int** s = (int**)malloc(sizeof(int*)*3);
    for ( int i = 0; i < 3; i++ )
        s[i] = (int*)malloc(sizeof(int)*(n + 1));
    int** l = (int**)malloc(sizeof(int*)*3);
    for ( int i = 0; i < 3; i++ )
        l[i] = (int*)malloc(sizeof(int)*(n + 1));


    assembly_line(a, t, e, x, n, s, l);

    printf("%d\n", s[0][0]);
    printStaitons(l, n);

    return 0;

}
