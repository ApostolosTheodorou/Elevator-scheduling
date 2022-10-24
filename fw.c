/* File: fw.c */

#include <stdio.h>
/* #define REC */ 
/* #define MEM */
/* #define DP  */
/* #define BF  */

/* Function: fw. It calculates the total cost (how many floors the riders
 have to go upstairs or downstairs) for the riders with destinations d,
 where d is between floors a and b (a<d<=b) */
int fw(int a, int b, int *dests, int nrid)
{
    int totalCost, i;

    totalCost= 0;
    for (i=0 ; i < nrid ; i++) {
        if (*(dests+i) > a && *(dests+i) <= b)
            totalCost+= *(dests+i)-a < b- *(dests+i) ? *(dests+i)-a : b- *(dests+i);
    }
    return totalCost;
}

/* Function: M. It calculates the minimum cost so that all riders are served
 with exactly j stops, with the highest stop being at floor i */

#ifdef REC
int M(int i, int j, int *dests, int nrid)
{
    int k, min, temp, cost;

    if (j == 0) {                                               /* Elevator does not move. All riders use the stairs */
        cost= 0;
        for (i=0 ; i < nrid ; i++)                                   /* The cost will be the sum of the destinations */
            cost+= *(dests+i);
        return cost;
    }
    else {
        min= 2147483647;                                        /* 2^31-1 the highest value that a 4byte int can handle
                                                                               (it is used to represent the infinity)*/
        for (k=0; k <= i ; k++) {
            temp= M(k, j-1 , dests, nrid) - fw(k, 2147483647, dests, nrid) +
                  fw(k, i, dests, nrid) + fw(i, 2147483647, dests, nrid);
            if (temp < min)
                min= temp;
        }
        return min;
    }
}
#endif

#ifdef MEM
int M(int i, int j, int *dests, int nrid, int **memorizeM)
{
    int k, min, temp, cost, counter;

    if (*(*(memorizeM+i)+j) != -1)                                       /* If the value has already been calculated */
        return *(*(memorizeM+i)+j);
    else {                                                     /* If it is the first time that a value is calculated */
        if (j == 0) {                                           /* Elevator does not move. All riders use the stairs */
            cost= 0;
            for (counter=0 ; counter < nrid ; counter++)                 /* The cost will be the sum of destinations */
                cost+= *(dests+counter);
            *(*(memorizeM+i))= cost;                                           /* Store the cost that was calculated */
            return cost;
        }
        else {
            min= 2147483647;                                    /* 2^31-1 the highest value that a 4byte int can handle
                                                                               (it is used to represent the infinity)*/
            for (k=0; k <= i ; k++) {
                temp= M(k, j-1 , dests, nrid, memorizeM) - fw(k, 2147483647, dests, nrid) +
                fw(k, i, dests, nrid) + fw(i, 2147483647, dests, nrid);
                if (temp < min)
                    min= temp;
            }
            *(*(memorizeM+i)+j)= min;                                          /* Store the cost that was calculated */
            return min;
        }
    }
}
#endif

#ifdef DP
int M(int i, int j, int *dests, int nrid, int **memorizeM, int **Min_k)
{
    int k, min, temp, MinIndex;

    min= *((*memorizeM)+j);
    MinIndex= 0;
    for (k=0; k <= i ; k++) {
        temp= *(*(memorizeM+k)+j-1) - fw(k, 2147483647, dests, nrid) +
        fw(k, i, dests, nrid) + fw(i, 2147483647, dests, nrid);
        if (temp < min) {
            min= temp;
            MinIndex= k;
        }
    }
    *(*(Min_k + i)+j)= MinIndex;
    return min;
}
#endif
