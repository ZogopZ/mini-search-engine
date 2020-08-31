#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>

#include "root.h"
#include "checker.h"
#include "cli_utils.h"

int signals_caught;

void got_signal()
{
    signal(SIGUSR2, got_signal);
    signals_caught++;
}

int main(int argc, char** argv)
{
    signal(SIGUSR2, got_signal);                                //Signal handler for SIGUSR2.

    struct timeval t0, t1;
    Stats_t *stats;

    gettimeofday(&t0, 0);                                       //Starting time of root.
    parse_cli_args(argc, argv);                                 //Check in line parameters.
    stats = init_splitNmerge();                                 //Create first splitter/merger node and wait for results.
    init_sort();                                                //Create sort node to sort results and print them to tty.
    gettimeofday(&t1, 0);                                       //Ending time of root.

    stats->turnaround_time = (t1.tv_sec-t0.tv_sec)*1000000 + t1.tv_usec-t0.tv_usec;
                                                                //Turnaround time of query.
    print_stats(stats, signals_caught);                         //Stats printer.

    free(get_datafile());
    free(get_height_str());
    free(get_substring());
    free(stats);
}




