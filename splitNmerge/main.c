#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

#include "cli_utils.h"
#include "splitNmerge.h"
#include "../root/checker.h"


int main(int argc, char** argv)
{
    int i, status, fd_parent, elapsed_dgts = 0, pid_dgts = 0, line_length = 0;
    long elapsed = 0;
    char* time;
    struct timeval t0, t1;

    gettimeofday(&t0, 0);                                                       //Starting time of sm process.
    parse_cli_args(argc, argv);                                                 //Check in line parameters.
    fd_parent = splitNmerge_fork();                                             //Forks to sm nodes and returns parent's named pipe fd.
    for (i = 0; i < 2; i++)
        wait(&status);                                                          //Parent process will wait for children to exit.
    gettimeofday(&t1, 0);                                                       //Ending time of sm process.


    elapsed = (t1.tv_sec-t0.tv_sec)*1000000 + t1.tv_usec-t0.tv_usec;            //Turnaround time of sm.

    elapsed_dgts = count_digits(elapsed);
    pid_dgts = count_digits(getpid());
    line_length = 7 + pid_dgts + elapsed_dgts;
    time = mallocNcheck(line_length, LEAF);
    sprintf(time, "s%2d %5ld %ld\n", line_length, (long int)getpid(), elapsed); //Create a string with s for splitter/merger, %2d for line length,
                                                                                //%5ld for pid of process, %ld for turnaround time of process.
    writeNcheck(fd_parent, time, line_length, SM);
    close(fd_parent);
    free(get_datafile());
    free(get_height_str());
    free(get_substring());
    free(get_splitFile());
    free(get_fifo());
    free(get_rootPid_str());
    free(time);
    return 0;
}




