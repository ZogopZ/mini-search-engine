#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>

#include "leaf.h"
#include "cli_utils.h"
#include "../root/checker.h"

int main(int argc, char** argv)
{
    int fd_parent, elapsed_dgts = 0, pid_dgts = 0, line_length = 0;
    long elapsed = 0;
    char* time;
    Assist_t* assist;
    struct timeval t0, t1;

    gettimeofday(&t0, 0);                                                       //Starting time of leaf process.
    parse_cli_args(argc, argv);                                                 //Check in line parameters.
    assist = create_context(find_range());                                      /*-Open splitter file and find range of records for each leaf.
                                                                                -Open bin file and copy all records assigned to each leaf, to a buffer.
                                                                                -Store information to assist struct.*/
    fd_parent = open(get_fifo(), O_WRONLY);                                     //Open named pipe inherited from parent for writing (blocking).
    check_open(fd_parent, FIFO_FILE, LEAF);                                     //Check if open() function succeeded.
    searchNsend(assist, fd_parent);                                             //Search for substring matches and write them back to parent.
    gettimeofday(&t1, 0);                                                       //Ending time of leaf process.

    elapsed = (t1.tv_sec-t0.tv_sec)*1000000 + t1.tv_usec-t0.tv_usec;            //Turnaround time of leaf.
    elapsed_dgts = count_digits(elapsed);
    pid_dgts = count_digits(getpid());
    line_length = 7 + pid_dgts + elapsed_dgts;
    time = mallocNcheck(line_length, LEAF);
    sprintf(time, "f%2d %5ld %ld\n", line_length, (long int)getpid(), elapsed); //Create a string with f for leaf node, %2d for line length,
                                                                                //%5ld for pid of process, %ld for turnaround time of process.
    writeNcheck(fd_parent, time, line_length, LEAF);
    close(fd_parent);
    free(get_datafile());
    free(get_substring());
    free(get_splitFile());
    free(get_fifo());
    free(assist);
    free(time);
    kill(get_rootPid(), SIGUSR2);                                               //Send signal to root.
    return 0;
}

