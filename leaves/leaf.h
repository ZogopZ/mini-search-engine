#ifndef _TOOLS_H_
#define _TOOLS_H_

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

typedef struct
{
    long unsigned int context_length;
    char context[0];
} Assist_t;

char* find_range(void);

Assist_t* create_context(char range[20]);

void searchNsend(Assist_t *assist, int fd_parent);

int count_digits(int number);

#endif /* _TOOLS_H_ */
