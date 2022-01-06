#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

int * missilesCount;

int main()
{
    missilesCount =  (int*) malloc (sizeof(int) * 1);

    *missilesCount = -5;


    while(true){
        sleep(1);
        printf("%d\n", *missilesCount);
        *missilesCount += 1;
    }

    return 0;
}
