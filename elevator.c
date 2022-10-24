/* File: elevator.c */

#include <stdio.h>
#include <stdlib.h>
#include "solve.h"
#define ERROR -1

int main(void)
{
    int i, nrid, nst, *dests, LowestCost;

    printf("Please enter\n1) the number of riders\n2) the maximum number of stops\n");
    scanf("%d %d", &nrid, &nst);
    dests= malloc(nrid * sizeof(int));
    if (dests == NULL) {
        printf("Memory allocation failed\n");
        return ERROR;
    }
    printf("Please enter the riders' destinations\n");
    for (i=0 ; i < nrid ; i++)                                                 /* fill the array of the destinations */
        scanf("%d", dests+i);
    LowestCost= solve(nrid, nst, dests);
    free(dests);
    printf("Cost is: %d\n", LowestCost);
    return 0;
}
