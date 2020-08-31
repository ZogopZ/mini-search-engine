#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "root.h"
#include "checker.h"
#include "cli_utils.h"


int height;
int arg_height;
char* height_str;

int arg_datafile;
char* datafile;

int arg_substring;
char* substring;

int skew = INVALID;

void parse_cli_args(int argc, char** argv)
{
    int i;

    parseNcheck(argc, ARGC_CHECK, ROOT);                                //Check correct number of arguments.
    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-h") == 0)                                 //Search arguments for flag -h.
        {
            parseNcheck(height = atoi(argv[i+1]), HEIGHT_CHECK, ROOT);  //Check height argument.
            arg_height = i + 1;
            height_str = mallocNcheck(strlen(argv[arg_height]), ROOT);
            strcpy(height_str, argv[arg_height]);
        }
        else if (strcmp(argv[i], "-d") == 0)                            //Search arguments for flag -d.
        {
            arg_datafile = i + 1;                                       //The argument following flag -d is the datafile.
            datafile = mallocNcheck(strlen(argv[arg_datafile]), ROOT);
            strcpy(datafile, argv[arg_datafile]);
        }
        else if (strcmp(argv[i], "-p") == 0)                            //Search arguments for flag -p.
        {
            arg_substring = i + 1;                                      //The argument following flag -p is the substring.
            substring = mallocNcheck(strlen(argv[arg_substring]), ROOT);
            strcpy(substring, argv[arg_substring]);
        }
        else if (strcmp(argv[i], "-s") == 0)                            //Check for existence of flag -s.
        {
            skew = VALID;
        }
    }
}

int get_height(void)                                                    /*FUNCTIONS THAT RETURN ARGUMENTS OF MAIN*/
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
int get_skew(void)
{
    return (skew);
}

