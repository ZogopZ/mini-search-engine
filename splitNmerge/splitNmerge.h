#ifndef _SPLITNMERGE_H_
#define _SPLITNMERGE_H_

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

int splitNmerge_fork(void);

void leafNode_fork(void);

int count_digits(int number);

#endif /* _SPLITNMERGE_H_ */
