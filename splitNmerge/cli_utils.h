#ifndef _CLI_UTILS_H_
#define _CLI_UTILS_H_

void parse_cli_args(int argc, char **argv);

int get_height(void);

char* get_height_str(void);

int get_arg_datafile(void);

char* get_datafile(void);

int get_arg_substring(void);

char* get_substring(void);

char* get_splitFile(void);

char* get_fifo(void);

int get_rootPid(void);

char* get_rootPid_str(void);

#endif /* _CLI_UTILS_H_ */
