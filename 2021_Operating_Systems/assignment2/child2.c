#include <stdio.h>
#include <unistd.h>

int main(){

    FILE *f = fopen("test.txt", "r");
    if (f == NULL)
        printf("child2 read empty file.\n");

    int num;
    for (int i=0; i<6; i++){
    
        sleep(2);

        fscanf(f, "%d", &num);
        printf("child2 read %d.\n", num);

       }

    fclose(f);

    return 0;

}


