/* File: solve.c */
#include <stdio.h>
#include <stdlib.h>
#include "fw.h"
#define ERROR -1
/* #define REC */
/* #define MEM */
/* #define DP  */

#ifdef REC
int solve(int nrid, int nst, int *dests)                                                                /* Recursive */
{
    int nfl, MinCost, temp, LastStop, i;

    /* Find the maximum floor(nfl) of the destinations' array */
    nfl= *dests;                                               /* Initialize nfl with the first element of the array */
    for (i=1; i < nrid ; i++) {                                              /* For every element of the array check */
        if (*(dests+i) > nfl)                                  /* If the current element is greater than the maximum */
            nfl= *(dests+i);
    }
    /* Implementation of the recursive type */
    MinCost= 2147483647 ;                                       /* 2^31-1 the highest value that a 4byte int can handle
                                                                               (it is used to represent the infinity)*/
    for(i=0; i <= nfl ; i++) {            /* Find the minimum cost with last stop at the 1st, 2nd, ..., nfl th floor */
        temp= M(i, nst, dests, nrid);
        if (temp < MinCost) {
            MinCost= temp;
            LastStop= i;
        }
    }
    printf("\nLast stop at floor: %d\n", LastStop);
    return MinCost;
}
#endif

#ifdef MEM
int solve(int nrid, int nst, int *dests)                                              /* Recursive with memorization */
{
    int nfl, MinCost, temp, LastStop, i, i2, **memorizeM;

    /* Find the maximum floor(nfl) of the destinations' array */
    nfl= *dests;                                               /* Initialize nfl with the first element of the array */
    for (i=1; i < nrid ; i++) {                                              /* For every element of the array check */
        if (*(dests+i) > nfl)                                  /* If the current element is greater than the maximum */
            nfl= *(dests+i);
    }
    /* Create the array memorizeM [nfl+1][nst+1] */
    memorizeM= malloc((nfl+1) * sizeof(int *));
    if (memorizeM == NULL) {
        printf("Memory allocation failed\n");
        return ERROR;
    }
    for (i=0; i < nfl+1 ; i++) {
        *(memorizeM+i)= malloc((nst+1) * sizeof(int));
        if (*(memorizeM+i) == NULL) {
            printf("Memory allocation failed\n");
            return ERROR;
        }
    }
    /* Initialize the array memorizeM with the value -1 */
    for (i=0; i < nfl+1 ; i++) {
        for (i2=0; i2 < nst+1 ; i2++)
            *(*(memorizeM+i)+i2)= -1;
    }
    /* Implementation of the recursive type */
    MinCost= 2147483647 ;                                     /* 2^31-1 the highest value that a 4byte int can handle*/
    for(i=0; i <= nfl ; i++) {            /* Find the minimum cost with last stop at the 1st, 2nd, ..., nfl th floor */
        temp= M(i, nst, dests, nrid, memorizeM);
        if (temp < MinCost) {
            MinCost= temp;
            LastStop= i;                                          /* Save the floor in which the minimum cost was found
                                                                             (this is the last stop of the elevator) */
        }
    }
    printf("\nLast stop at floor: %d\n", LastStop);
    free(memorizeM);
    return MinCost;
}
#endif

#ifdef DP
int solve(int nrid, int nst, int *dests)                                                      /* Dynamic programming */
{
    int nfl, MinCost, MaxCost, temp, LastStop, i, j, **memorizeM, column, line,
        *Stops, **Min_k, SemifinalStop, PreviousStop, counter, RemainingStops;

    /* Find the maximum floor(nfl) of the destinations' array */
    nfl= *dests;                                               /* Initialize nfl with the first element of the array */
    for (i=1; i < nrid ; i++) {                                              /* For every element of the array check */
        if (*(dests+i) > nfl)                                  /* If the current element is greater than the maximum */
            nfl= *(dests+i);
    }
    /* Create the array memorizeM [nfl+1][nst+1] */
    memorizeM= malloc((nfl+1) * sizeof(int *));
    if (memorizeM == NULL) {
        printf("Memory allocation failed\n");
        return ERROR;
    }
    for (i=0; i < nfl+1 ; i++) {
        *(memorizeM+i)= malloc((nst+1) * sizeof(int));
        if (*(memorizeM+i) == NULL) {
            printf("Memory allocation failed\n");
            return ERROR;
        }
    }
    /* Calculate the maximum cost (all riders use the stairs) */
    MaxCost= 0;
    for (counter=0 ; counter < nrid ; counter++)
        MaxCost+= *(dests+counter);
    /* Fill the first column of the array memorizeM with the maximum cost */
    for (column=0, line=0 ; line < nfl+1 ; line++)     /* No elevator movement(because maximum number of stops is 0) */
        *(*(memorizeM+line)+column)= MaxCost;
    /* Fill the first line of the array memorizeM with the maximum cost */
    for (line=0, column=0 ; column < nst+1 ; column++)                      /* No elevator movement(because the highest
                                                                                           stop is the ground floor) */
        *(*(memorizeM+line)+column)= MaxCost;
    /* Create the array Min_k[nfl+1][nst+1] */
    Min_k= malloc((nfl+1) * sizeof(int *));
    if (Min_k == NULL) {
        printf("Memory allocation failed\n");
        return ERROR;
    }
    for (i=0; i < nfl+1 ; i++) {
        *(Min_k+i)= malloc((nst+1) * sizeof(int));
        if (*(Min_k+i) == NULL) {
            printf("Memory allocation failed\n");
            return ERROR;
        }
    }
    /* Initialize the first column with 0 (when there is no elevator's movement
    the floor with the minimum k in j-1 stops is the ground floor) */
    for (column=0, line=0 ; line < nfl+1 ; line++)
        *(*(Min_k+line)+column)= 0;
    /* Fill the rest memorizeM array buttom-up */
    for (column=1 ; column < nst+1 ; column++) {
        for (line=1; line < nfl+1 ; line++)
            *(*(memorizeM+line)+column)= M(line, column, dests, nrid, memorizeM, Min_k);
    }
    /* Print on the screen the array memorizeM (inversed) */
    for (column=0 ; column < nst+1 ; column++) {
        printf("\n");
        for (line=0 ; line < nfl+1 ; line++)
            printf("%3d  ", *(*(memorizeM+line)+column));
    }
    printf("\n");
    /* Find the last stop of the elevator */
    MinCost= *((*memorizeM)+nst);             /* Initialize the MinCost with the element memorizeM[0][nst](=MaxCost) */
    LastStop= 0;                                                         /* Initialize last stop at the ground floor */
    for(line=1; line < nfl+1 ; line++) {
        if (*(*(memorizeM+line)+nst) < MinCost) {      /* If in any floor (=line) the cost is lower than the minimum */
            MinCost= *(*(memorizeM+line)+nst);                                  /* then this is the new minimum cost */
            LastStop= line;                                                /* The floor where the minimum cost is found
                                                                           (the elevator makes it's last stop there) */
        }
    }
    /* Create the array Stops[nst] */
    if (nst != 0) {
        Stops= malloc(nst * sizeof(int));
        if (Stops == NULL) {
            printf("Memory allocation failed\n");
            return ERROR;
        }
    }
    /* Find all the stops of the elevator from the semilast
    until the first and store them in array Stops[nst] */
    if (nst != 0) {                             /* under the condition that the elevator will make at least one stop */
        SemifinalStop= *(*(Min_k + LastStop)+nst) ;                               /* The floor of the semifinal stop is
                                                                                 the content of Min_k[LastStop][nst] */
        PreviousStop= SemifinalStop;
        *(Stops+(nst-1))= LastStop;            /* Store the last stop in the array Stops[nst] (in it's last element) */
        *(Stops+(nst-2))= SemifinalStop;                                        /* Store the semilast stop in the array
                                                                               Stops[nst] (in it's semilast element) */
        /* Find all the other stops (apart from the last and the semilast)
        that the elevator makes and store them in array Stops[nst] */
        RemainingStops= nst-2;                    /* The number of stops that are left to be done (according to nst) */
        column= nst;                                /* The column of the matrix Min_k in which the semilast stop was */
        for (line=LastStop-1 ; line >= 0 && RemainingStops > 0 ; line--) {    /* Check the matrix line by line top-down
                                                                                      from line LastStop-1 until line 0
                                                                           (if there are no more stops left to be done,
                                                                                                     stop the check) */
            if ( *(*(Min_k+line)+column) < PreviousStop ) {                   /* When a lower floor (=line) is found */
                PreviousStop= *(*(Min_k+line)+(--column));                                     /* Go one column left */
                *(Stops+(--RemainingStops))= PreviousStop; /* Store every stop that is found in the array Stops[nst] */
            }
        }
    }
    /* Print on the screen all the stops that the elevator makes */
    if (nst != 0) {                                                /* under the condition that there is at least one */
        printf("Elevator stops are: ");
        for (j=0 ; j < nst ; j++) {                                                 /* From the first until the last */
            if ( *(Stops+j) != 0)                                                     /* Print only the actual stops */
                printf("%d ", *(Stops+j));
        }
        printf("\n");
    }
    else                                                                         /* When the elevator makes no stops */
        printf("No elevator stops\n");
    /* Free the malloced matrixes */
    for (i=0 ; i < nfl+1 ; i++) {
        free(*(memorizeM+i));
        free(*(Min_k+i));
    }
    free(memorizeM);
    free(Min_k);
    if (nst != 0)                                               /* If nst==0 then the array Stops doesn't even exist */
        free(Stops);

    return MinCost;
}
#endif
