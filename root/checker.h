#ifndef _CHECKER_H_
#define _CHECKER_H_

#define ROOT -26
#define SM -27
#define LEAF -28
#define RES_FILE -29
#define BIN_FILE -30
#define SPLIT_FILE -31
#define FIFO_FILE_PARENT -32
#define FIFO_FILE -33
#define ARGC_CHECK -34
#define HEIGHT_CHECK -35

#define MIN_ARGS_ROOT 7
#define MAX_ARGS_ROOT 8
#define ARGS_LEAF 11
#define ARGS_SM 13





void* mallocNcheck(size_t incoming_size, int caller);

int readNcheck(int fd_child, char* buffer, int full_size, int caller);

int writeNcheck(int fd_parent, char* buffer, int full_size, int caller);

void check_fork(pid_t pid_check, int caller);

void check_open(int fd_in, int which_file, int caller);

void check_fopen(FILE* fp_in , int which_file, int caller);

void check_mkfifo(int value_check, int caller);

void check_unlink(int value_check, int caller);

void check_exec(int caller);

void parseNcheck(int parse_value, int function, int caller);

#endif /* _CHECKER_H_ */
