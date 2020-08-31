#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#include "leaf.h"
#include "cli_utils.h"
#include "../root/checker.h"

char* find_range(void)
{
    FILE* fp_split;
    static char line[20];

    fp_split = fopen(get_splitFile(), "r+");                    //Open file splitter for reading and writing.
    check_fopen(fp_split, SPLIT_FILE, LEAF);                    //Check if fopen() function succeeded.
    while (fgets(line, sizeof(line), fp_split) != NULL)         //Get a line from file splitter.
    {
        if (line[0] == '0')                                     //If the first character of a line is 0 the line is not yet assigned to a leaf.
        {
            fseek(fp_split, -strlen(line), SEEK_CUR);           //Go back to the beginning of the line.
            fprintf(fp_split, "%d", 1);                         //Replace 0 with 1. Line with records is assigned to leaf.
            break;
        }
        memset(line, 0, strlen(line));                          //Reinitialize buffer line to 0.

    }
    fclose(fp_split);                                           //Close splitter file.
    return line;
}

Assist_t* create_context(char range[20])
{
    int start = 0, end = 0, numOfrecords = 0;
    FILE* fp_bin;
    char* token = NULL, * token1 = NULL, * token2 = NULL, * context_line = NULL;
    Assist_t *assist;
    MyRecord_t rec;

    token = strtok(range, " \n");                               //Skip the first token.
    while ((token = strtok(NULL, " \n\0")) != NULL)
    {
        if (token1 == NULL)
        {
            token1 = token;
            start = atoi(token1);                               //Second token is the first record to be assigned to leaf.
        }
        else if (token2 == NULL)
        {
            token2 = token;
            end = atoi(token2);                                 //Third token is the last record to be assigned to leaf.
        }
    }
    numOfrecords = end - start + 1;                             //Number of records to be assigned to leaf.
    assist = mallocNcheck(sizeof(Assist_t) + (numOfrecords*sizeof(MyRecord_t)), LEAF);
    assist->context_length = (numOfrecords*sizeof(MyRecord_t));
    context_line = mallocNcheck(sizeof(MyRecord_t), LEAF);
    fp_bin = fopen(get_datafile(), "rb");                       //Open datafile for binary reading.
    check_fopen(fp_bin, BIN_FILE, LEAF);                        //Check if fopen() function succeeded.
    fseek(fp_bin, (start)*sizeof(MyRecord_t), SEEK_SET);        //Seek the first record to be assigned to leaf from the start.
    for (int i = 0; i < numOfrecords; i++)                      //Repeat for the number of records that are assigned to leaf.
    {
        memset(&rec, 0, sizeof(rec));
        fread(&rec, sizeof(rec), 1, fp_bin);                    //Read a record from datafile.
        sprintf(context_line, "%ld %s %s %s %d %s %s %-9.2f\n", //Print the record to the context_line buffer.
                rec.custid, rec.LastName, rec.FirstName,
                rec.Street, rec.HouseID, rec.City, rec.postcode,
                rec.amount);
                                                                //The length of each line contains 4 bytes and a record.
        int context_line_length = (int)strlen(context_line) + 4;
        sprintf(assist->context + strlen(assist->context), "%3d %s",
                context_line_length, context_line);             //For each repetition append to context buffer the length of a record and the record itself.
        memset(context_line, 0, context_line_length);

    }
    free(context_line);
    fclose(fp_bin);
    return assist;
}

void searchNsend(Assist_t* assist, int fd_parent)
{
    long unsigned int chars = 0, line_start = 0;
    char* substring = get_substring();
    char line_length_str[5];
    int line_length;

    for (long unsigned int i = 4; i < assist->context_length; i++)
    {                                                           //Skip first 4 characters of each line, containing additional information about line length.
        if (assist->context[i] == '\n')
        {
            line_start = i + 1;                                 //Store where each record is written, insite context buffer.
            i = i + 4;                                          //Skip first 4 characters of each line, containing additional information about line length.
            continue;
        }
        for (long unsigned int j = 0; j < strlen(substring); j++)
        {                                                       //Step through all substring characters.
            if (substring[j] == assist->context[i+j])           //Check if a substring's character is matched to a context's character.
            {
                chars++;
                if (chars == strlen(substring))                 //All substring's characters have a match.
                {
                    memset(line_length_str, 0, 5);
                    strncpy(line_length_str, &(assist->context[line_start]), 4);
                    line_length = atoi(line_length_str);        //The first 4 characters of each line contain the line length.
                    writeNcheck(fd_parent, &(assist->context[line_start]), line_length, LEAF);
                    i = line_start - 1 + (long int)line_length - 1;
                    chars = 0;
                }
            }
            else
            {
                chars = 0;
                break;
            }

        }
    }
}

int count_digits(int number)
{
    int count = 0;

    while(number != 0)
    {
        number = number/10;
        count++;
    }
    return(count);
}




