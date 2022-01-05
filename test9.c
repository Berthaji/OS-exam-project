#include <stdio.h>
#include <stdlib.h>

int * missilesCount;

int main()
{
    missilesCount =  (int*) malloc (sizeof(int) * 1);

    *missilesCount = 145;
    printf("%d", *missilesCount);

    return 0;
}
