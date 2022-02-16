// 2018007974_김서연_12838

#include <stdio.h>

int p[101];
int m[101][101];
int s[101][101];

void matrixChain(int n)
{
    for ( int i = 1; i <= n; i++ )
        m[i][i] = 0;

    int j, q;
    for ( int l = 2; l <= n; l++ )
    {
        for ( int i = 1; i <= n - l + 1; i++ )
        {
            j = i + l - 1;
            m[i][j] = 1000000;
            for ( int k = i; k <= j - 1; k++ )
            {
                q = m[i][k] + m[k + 1][j] + p[i - 1] * p[k] * p[j];
                if ( q < m[i][j] )
                {
                    m[i][j] = q;
                    s[i][j] = k;
                }
            }
        }
    }

}

void printParens(int i, int j)
{

    if ( i == j )
    {
        printf("%d ", i);
        return;
    }

    printf("( ");
    printParens(i, s[i][j]);
    printParens(s[i][j] + 1, j);
    printf(") ");

}

int main()
{

    int n;
    scanf("%d", &n);

    for ( int i = 0; i <= n; i++ )
        scanf("%d", &p[i]);

    matrixChain(n);

    printf("%d\n", m[1][n]);
    printParens(1, n);

    return 0;

}
