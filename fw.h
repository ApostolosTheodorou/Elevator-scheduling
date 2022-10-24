int fw(int , int , int *, int);

/* #define REC */
/* #define MEM */
/* #define DP  */

#ifdef REC
int M(int, int, int *, int);
#endif

#ifdef MEM
int M(int, int, int *, int, int **);
#endif

#ifdef DP
int M(int i, int j, int *dests, int nrid, int **memorizeM, int **Min_k);
#endif
