#include <math.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "root.h"
#include "checker.h"
#include "cli_utils.h"



Stats_t* init_splitNmerge(void)
{
    int status = 0, checker = 0, fd_child = 0, numOfrecords = 0, line_length = 0;
    int sum_leaf = 0, sum_sm = 0, time_temp = 0, pid_temp = 0;
    char* fifo, *rootPid_str, * record, * time;
    char lengthOfstr[5];
    pid_t pid;
    FILE* fp_results;
    Stats_t *stats;


    numOfrecords = count_recs();                                //Get the records number from bin file.
    split_recs(numOfrecords);                                   //Function to correctly split records across leaf nodes, by using a file.

    rootPid_str = mallocNcheck(count_digits(getpid()), ROOT);
    sprintf(rootPid_str, "%ld", (long int)getpid());            //Father's pid will be passed on to children.
    fifo = mallocNcheck(5 + count_digits(getpid()), ROOT);      //Malloc 5 + number of pid digits (5 for "fifo_").
    sprintf(fifo, "fifo_%ld", (long int)getpid());              //Create distinct named pipe names.
    checker = mkfifo(fifo, 0666);                               //Create named pipe for communication from child to parent with read and write permissions.
    check_mkfifo(checker, ROOT);                                //Check if mkfifo() function succeeded.
    pid = fork();
    check_fork(pid, ROOT);                                      //Check if fork() function succeeded.
    if (pid == 0)                                               //Child splitter/merger process.
    {
        execl("../splitNmerge/splitNmerge", "",                 //Executable is located one directory back and inside directory "splitNmerge".
                                                                //Second "" argument is for valgrind usage (valgrind will not run without it).
              "-d", get_datafile(),                             //Bin file.
              "-h", get_height_str(),                           //Height value.
              "-p", get_substring(),                            //Substring.
              "-r", "splitter",                                 //File of records to be assigned.
              "-f", fifo,                                       //Named pipe for father-child communication.
              "-t", rootPid_str,                                //Root's pid
              (char*) NULL);
        check_exec(ROOT);                                       //Check if execl() function succeeded.
    }
    else                                                        //Parent process.
    {
        fd_child = open(fifo, O_RDONLY);
        check_open(fd_child, FIFO_FILE, ROOT);                  //Check if open() function succeeded.
        fp_results = fopen("results", "w+");
        check_fopen(fp_results, RES_FILE, ROOT);                //Check if fopen() function succeeded.

        stats = mallocNcheck(sizeof(Stats_t), ROOT);
        int bytes_read = 0;
        memset(lengthOfstr, 0, 5);
                                                                //Parent will block here until there is something to read.
        while ((bytes_read = readNcheck(fd_child, lengthOfstr, 4, ROOT)) != 0)
        {                                                       //Read exactly 4 bytes.
            line_length = 0;
            if ((lengthOfstr[0] == ' ') || ((lengthOfstr[0] >= '1') && (lengthOfstr[0] <= '9')))
            {                                                   //Line to be read, contains an actual record.
                stats->results++;
                line_length = atoi(lengthOfstr);                //First 4 bytes read from fd_child, contain the length of the record.
                record = mallocNcheck(line_length, ROOT);
                memcpy(record, lengthOfstr, 4);
                readNcheck(fd_child, record + strlen(record), line_length - 4, ROOT);
                                                                //Read from fd_child record_length bytes - 4 (4 bytes where already read).
                fprintf(fp_results, "%s", &(record[4]));        //Write all records(without first 4 bytes containing length info) to results file.
                free(record);
            }
            else if (lengthOfstr[0] == 'f')                     //Line to be read, contains the turnaround time of a leaf.
            {                                                   //e.g:f14 21666 554\n->(f14 = leaf node sends 14 bytes) (21666 = pid of leaf) (554 = turnaround time).
                line_length = atoi(&(lengthOfstr[1]));          //Get line length.
                time = mallocNcheck(line_length, ROOT);
                memcpy(time, lengthOfstr, 4);
                readNcheck(fd_child, time + strlen(time), line_length - 4, ROOT);
                pid_temp = atoi(&(time[3]));                    //Get pid of leaf.
                time_temp = atoi(&(time[10]));                  //Get turnaround time of leaf.
                if (stats->min_leaf == 0 || time_temp < stats->min_leaf)
                {                                               //Find min leaf and store turnaround time and pid.
                    stats->min_leaf = time_temp;
                    stats->min_leaf_pid = pid_temp;
                    stats->max_leaf = time_temp;
                    stats->max_leaf_pid = pid_temp;
                }
                else if (time_temp > stats->max_leaf)           //Find max leaf and store turnaround time and pid.
                {
                    stats->max_leaf = time_temp;
                    stats->max_leaf_pid = pid_temp;
                }
                sum_leaf = sum_leaf + time_temp;                //Sum leaf times to calculate average of leafs.
                free(time);
            }
            else if (lengthOfstr[0] == 's')                     //Line to be read, contains the turnaround time of a splitter/merger.
            {                                                   //e.g:s15 21665 2465\n->(s15 = sm node sends 15 bytes) (21665 = pid of sm) (2465 = turnaround time).
                line_length = atoi(&(lengthOfstr[1]));          //Get line length.
                time = mallocNcheck(line_length, ROOT);
                memcpy(time, lengthOfstr, 4);
                readNcheck(fd_child, time + strlen(time), line_length - 4, ROOT);
                pid_temp = atoi(&(time[3]));                    //Get pid of sm.
                time_temp = atoi(&(time[10]));                  //Get turnaround time of sm.
                if (stats->min_sm == 0 || time_temp < stats->min_sm)
                {                                               //Find min sm and store turnaround time and pid.
                    stats->min_sm = time_temp;
                    stats->min_sm_pid = pid_temp;
                    stats->max_sm = time_temp;
                    stats->max_sm_pid = pid_temp;
                }
                else if (time_temp > stats->max_sm)             //Find max sm and store turnaround time and pid.
                {
                    stats->max_sm = time_temp;
                    stats->max_sm_pid = pid_temp;
                }
                sum_sm = sum_sm + time_temp;                    //Sum sm times to calculate average of sms.
                free(time);
            }

            memset(lengthOfstr, 0, 5);
        }
        wait(&status);                                          //Parent process will wait for child to exit.
    }
    fclose(fp_results);
    close(fd_child);
    checker = unlink(fifo);                                     //Remove named pipe.
    check_unlink(checker, ROOT);                                //Check if unlink() function succeeded.
    free(fifo);
    free(rootPid_str);

    stats->leaf_average = sum_leaf/pow(2, get_height());        //Calculate average turnaround time of leafs.
    stats->sm_average = sum_sm/(pow(2, get_height()) - 1);      //Calculate average turnaround time of sms.
    return stats;
}

void split_recs(int numOfrecords)
{
    int i, start, end, surplus;
    int height = get_height();
    int leafs = pow(2, height);
    FILE* fp_split;



    fp_split = fopen("splitter", "w+");                         //Open splitter file for writing and reading. Create it if it doesn't exist or truncate.
    check_fopen(fp_split, SPLIT_FILE, ROOT);                    //Check if fopen function succeeded.

    if (get_skew() == INVALID)
    {
        surplus = numOfrecords%leafs;                           //Use to assign all records to leaf nodes.
        start = 0;
        end = -1;
        for (i = 0; i < leafs; i++)
        {
            if (surplus == 0)                                   //There is no surplus.
                end = end + numOfrecords/leafs;                 //Each leaf node is assigned the same amount of records.
            else if (surplus > 0 )                              //There is surplus.
            {
                end = end + numOfrecords/leafs + 1;             //Assign each leaf node, one more record.
                surplus--;
            }
            fprintf(fp_split, "%d %d %d\n", 0, start, end);     //Write the results to splitter file.
            start = end + 1;
        }
    }
    else if (get_skew() == VALID)
    {
        int rec_sum = 0;
        int leaf_sum = 0;
        for (i = 0; i < leafs; i++)
            leaf_sum = leaf_sum + i + 1;                        //Calculate the sum of 1 + 2 + 3 + ... + 2^height.
        for (i = 0; i < leafs; i++)
            rec_sum = rec_sum + (i+1)*numOfrecords/leaf_sum;    //Calculate total number of records to be initially assigned. Without surplus calculation.
        surplus = numOfrecords - rec_sum;                       //Calculate surplus.
        start = 0;
        end = -1;
        for (i = 0; i < leafs; i++)
        {
            if (surplus == 0)                                   //There is no surplus.
                end = end + (i+1)*numOfrecords/leaf_sum;        //Each leaf node is assigned k * i * (1/1+2+3+...+n) records.
            else if (surplus > 0)                               //There is surplus.
            {
                end = end + (i+1)*numOfrecords/leaf_sum + 1;    //Assign each leaf node, one more record.
                surplus--;
            }
            fprintf(fp_split, "%d %d %d\n", 0, start, end);     //Write the results to splitter file.
            start = end + 1;
        }
    }
    fclose(fp_split);
    return;
}

int count_recs(void)
{
    FILE *fp_bin;
    MyRecord_t rec;
    long lSize;
    int numOfrecords = 0;

    fp_bin = fopen(get_datafile(), "rb");                       //Open bin file for reading.
    check_fopen(fp_bin, BIN_FILE, ROOT);                        //Check if fopen() function succeeded.
    fseek(fp_bin , 0 , SEEK_END);
    lSize = ftell(fp_bin);                                      //Calculate size of bin file in bytes.
    rewind(fp_bin);
    numOfrecords = (int) lSize/sizeof(rec);                     //Calculate number of records, by dividing file size by struct size.
    fclose(fp_bin);
    return numOfrecords;
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

void init_sort(void)
{
    int status;
    pid_t pid;

    pid = fork();
    check_fork(pid, ROOT);                                      //Check if fork() function succeeded.
    if (pid == 0)                                               //Child sort process.
    {
        execlp("sort", "-g", "results", (char*) NULL);          //Use flag -g to sort the first column numerically.
        check_exec(ROOT);                                       //Check if execlp() function succeeded.
    }
    else                                                        //Parent process.
        wait(&status);                                          //Parent process will wait for child to exit.
    return;
}

void print_stats(Stats_t* stats, int signals_caught)
{
    printf("\n*************************************STATISTICS*************************************\n");
    printf("Total results: |%d|\n", stats->results);
    printf("Total signals caught: |%d|\n", signals_caught);
    printf("                               --- \n");
    printf("Minimum turnaround time of leaf node with pid:    |%d|-->|%d microseconds|\n",stats->min_leaf_pid, stats->min_leaf);
    printf("Maximum turnaround time of leaf node:             |%d|-->|%d microseconds|\n",stats->max_leaf_pid, stats->max_leaf);
    printf("Average turnaround time of leaf nodes:            |%.3f microseconds|\n", stats->leaf_average);
    printf("                               --- \n");
    printf("Minimum turnaround time of splitter/merger node:  |%d|-->|%d microseconds|\n",stats->min_sm_pid, stats->min_sm);
    printf("Maximum turnaround time of splitter/merger node:  |%d|-->|%d microseconds|\n",stats->max_sm_pid, stats->max_sm);
    printf("Average turnaround time of splitter/merger nodes: |%.3f microseconds|\n", stats->sm_average);
    printf("                               --- \n");
    printf("Turnaround time of query |%d microseconds|\n", stats->turnaround_time);
    printf("************************************************************************************\n");
    return;
}
