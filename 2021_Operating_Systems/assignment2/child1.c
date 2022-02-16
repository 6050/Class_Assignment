#include <stdio.h>
#include <unistd.h>

int main(){

    FILE *f = fopen("test.txt", "w");

    for(int i=0; i<12; i++){

        sleep(1);

        fprintf(f, "%d\n", i);
        fflush(f);
        printf("child1 wrote %d.\n", i);

    }

    fclose(f);

    return 0;

}


