#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cli_utils.h"
#include "../root/checker.h"


int arg_datafile;
char* datafile;

int arg_substring;
char* substring;

int arg_splitFile;
char* splitFile;

int arg_fifo;
char* fifo;

int arg_rootPid;
int rootPid;

void parse_cli_args(int argc, char** argv)
{
    int i;

    parseNcheck(argc, ARGC_CHECK, LEAF);                        //Check if given arguments are 11.
    for (i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-d") == 0)                         //Search arguments for flag -d.
        {
            arg_datafile = i + 1;                               //The argument following flag -d is the datafile.
            datafile = mallocNcheck(strlen(argv[arg_datafile]), LEAF);
            strcpy(datafile, argv[arg_datafile]);
        }
        else if (strcmp(argv[i], "-p") == 0)                    //Search arguments for flag -p.
        {
            arg_substring = i + 1;                              //The argument following flag -p is the substring.
            substring = mallocNcheck(strlen(argv[arg_substring]), LEAF);
            strcpy(substring, argv[arg_substring]);
        }
        else if (strcmp(argv[i], "-r") == 0)                    //Search arguments for flag -r.
        {
            arg_splitFile = i + 1;                              //The argument following flag -r is the file containing the records to be assigned.
            splitFile = mallocNcheck(strlen(argv[arg_splitFile]), LEAF);
            strcpy(splitFile, argv[arg_splitFile]);
        }
        else if (strcmp(argv[i], "-f") == 0)                    //Check for existence of flag -f.
        {
            arg_fifo = i + 1;                                   //The argument following flag -f is the fifo file created by parent for cummunication.
            fifo = mallocNcheck(strlen(argv[arg_fifo]), LEAF);
            strcpy(fifo, argv[arg_fifo]);
        }
        else if (strcmp(argv[i], "-t") == 0)                    //Check for existence of flag -t.
        {
            arg_rootPid = i + 1;                                //The argument following flag -t is root's pid.
            rootPid = atoi(argv[arg_rootPid]);
        }
    }
}


int get_arg_datafile(void)                                      /*FUNCTIONS THAT RETURN ARGUMENTS OF MAIN*/
{
    return (arg_datafile);
}
char* get_datafile(void)
{
    return (datafile);
}
int get_arg_substring(void)
{
    return (arg_substring);
}
char* get_substring(void)
{
    return (substring);
}
char* get_splitFile(void)
{
    return (splitFile);
}
char* get_fifo(void)
{
    return (fifo);
}
int get_rootPid(void)
{
    return (rootPid);
}
