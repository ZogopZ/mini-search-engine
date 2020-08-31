#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cli_utils.h"
#include "../root/checker.h"



int height;
int arg_height;
char* height_str;

int arg_datafile;
char* datafile;

int arg_substring;
char* substring;

int arg_splitFile;
char* splitFile;

int arg_fifo;
char* fifo;

int rootPid;
int arg_rootPid;
char* rootPid_str;

void parse_cli_args(int argc, char** argv)
{
    int i;

    parseNcheck(argc, ARGC_CHECK, SM);                          //Check if given arguments are 13.
    for (i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-h") == 0)                         //Search arguments for flag -h.
        {
            arg_height = i + 1;
            height = atoi(argv[arg_height]);
            height_str = mallocNcheck(strlen(argv[arg_height]), SM);
            strcpy(height_str, argv[arg_height]);
        }
        else if (strcmp(argv[i], "-d") == 0)                    //Search arguments for flag -d.
        {
            arg_datafile = i + 1;                               //The argument following flag -d is the datafile.
            datafile = mallocNcheck(strlen(argv[arg_datafile]), SM);
            strcpy(datafile, argv[arg_datafile]);
        }
        else if (strcmp(argv[i], "-p") == 0)                    //Search arguments for flag -p.
        {
            arg_substring = i + 1;                              //The argument following flag -p is the substring.
            substring = mallocNcheck(strlen(argv[arg_substring]), SM);
            strcpy(substring, argv[arg_substring]);
        }
        else if (strcmp(argv[i], "-r") == 0)                    //Search arguments for flag -r.
        {
            arg_splitFile = i + 1;                              //The argument following flag -r is the file containing the records to be assigned.
            splitFile = mallocNcheck(strlen(argv[arg_splitFile]), SM);
            strcpy(splitFile, argv[arg_splitFile]);
        }
        else if (strcmp(argv[i], "-f") == 0)                    //Check for existence of flag -f.
        {
            arg_fifo = i + 1;                                   //The argument following flag -f is the fifo file created by parent for cummunication.
            fifo = mallocNcheck(strlen(argv[arg_fifo]), SM);
            strcpy(fifo, argv[arg_fifo]);
        }
        else if (strcmp(argv[i], "-t") == 0)                    //Check for existence of flag -t.
        {
            arg_rootPid = i + 1;                                //The argument following flag -t is root's pid.
            rootPid = atoi(argv[arg_rootPid]);
            rootPid_str = mallocNcheck(strlen(argv[arg_rootPid]), SM);
            strcpy(rootPid_str, argv[arg_rootPid]);
        }
    }
}

int get_height(void)                                            /*FUNCTIONS THAT RETURN ARGUMENTS OF MAIN*/
{
    return (height);
}
char* get_height_str(void)
{
    return (height_str);
}
int get_arg_datafile(void)
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
int get_arg_fifo(void)
{
    return (arg_fifo);
}
char* get_fifo(void)
{
    return (fifo);
}
int get_rootPid(void)
{
    return (rootPid);
}
char* get_rootPid_str(void)
{
    return (rootPid_str);
}
