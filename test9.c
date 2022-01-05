#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

int * missilesCount;

int main()
{
    missilesCount =  (int*) malloc (sizeof(int) * 1);

    *missilesCount = 100;

    while(true){
        sleep(1);
        *missilesCount += 2;
        printf("%d\n", *missilesCount);
    }

    return 0;
}
