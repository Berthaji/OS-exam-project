#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

int * missilesCount;

int main()
{
    missilesCount =  (int*) malloc (sizeof(int) * 1);

    *missilesCount = 1;
    bool test = true;

    while(test){
        sleep(1);
        printf("%d\n", *missilesCount);
        *missilesCount += 1;

        if(*missilesCount == 3){
            test = false;
        }
    }

    return 0;
}
