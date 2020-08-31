#ifndef _ROOT_H_
#define _ROOT_H_

#define SIZEofBUFF 20
#define SSizeofBUFF 6

typedef struct{
    long  	custid;
    char 	FirstName[SIZEofBUFF];
    char 	LastName[SIZEofBUFF];
    char	Street[SIZEofBUFF];
    int 	HouseID;
    char	City[SIZEofBUFF];
    char	postcode[SSizeofBUFF];
    float  	amount;
} MyRecord_t;

typedef struct{
    int results;
    int min_leaf_pid;
    int min_leaf;
    int max_leaf_pid;
    int max_leaf;
    float leaf_average;
    int min_sm_pid;
    int min_sm;
    int max_sm_pid;
    int max_sm;
    float sm_average;
    int turnaround_time;
} Stats_t;

Stats_t* init_splitNmerge(void);

void split_recs(int numOfrecords);

int count_recs(void);

int count_digits(int number);

void init_sort(void);

void print_stats(Stats_t* stats, int signals_caught);

#endif /* _ROOT_H_ */
