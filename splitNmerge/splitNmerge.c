#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "cli_utils.h"
#include "splitNmerge.h"
#include "../root/checker.h"

int splitNmerge_fork(void)
{
    int i, checker, fd[2], fd_parent, line_length;
    char * new_height, * fifo, * record, * time;
    char lengthOfstr[5];
    pid_t pid;

    fd_parent = open(get_fifo(), O_WRONLY);                     //Open named pipe inherited from parent for writing (blocking).
    check_open(fd_parent, FIFO_FILE_PARENT, SM);                //Check if open function succeeded.

    for (i = 0; i < 2; i++)
    {
        fifo = mallocNcheck(7 + count_digits(getpid()), SM);    //Malloc 7 + number of pid digits (7 for "fifo_ _").
        sprintf(fifo, "fifo_%ld_%d", (long int)getpid(), i+1);  //Create distinct named pipe names.
        checker = mkfifo(fifo, 0666);                           //Create named pipe for communication from child to parent with read and write permissions.
        check_mkfifo(checker, SM);
        pid = fork();
        check_fork(pid, SM);                                    //Check if fork() function succeeded.

        if ((get_height() > 1) && (pid == 0))                   //Child process will execute sm.
        {
            new_height = mallocNcheck(1, SM);
            sprintf(new_height, "%d", get_height()-1);
            execl("../splitNmerge/splitNmerge","",              //Executable is located one directory back and inside directory "splitNmerge".
                                                                //Second "" argument is for valgrind usage (valgrind will not run without it).
                  "-d", get_datafile(),                         //Bin file.
                  "-h", new_height,                             //New height value.
                  "-p", get_substring(),                        //Substring.
                  "-r", get_splitFile(),                        //File of records to be assigned.
                  "-f", fifo,                                   //Named pipe for father-child communication.
                  "-t", get_rootPid_str(),                      //Root's pid
                  (char*) NULL);
            check_exec(SM);                                     //Check if execl() function succeeded.
        }
        else if ((get_height() == 1) && (pid == 0))             //Child process will execute leaf.
        {
            execl("../leaves/leaves","",                        //Executable is one directory back and inside directory "leaves".
                                                                //Second "" argument is for valgrind usage (valgrind will not run without it).
                  "-d", get_datafile(),                         //Bin file.
                  "-p", get_substring(),                        //Substring.
                  "-r", get_splitFile(),                        //File of records to be assigned.
                  "-f", fifo,                                   //Named pipe for father-child communication.
                  "-t", get_rootPid_str(),                      //Root's pid
                  (char*) NULL);
            check_exec(SM);                                     //Check if execl() function succeeded.
        }
        fd[i] = open(fifo, O_RDONLY);                           //Open named pipe created for communication with child for reading. (blocking).
        check_open(fd[i], FIFO_FILE, SM);                       //Check if open() function succeeded.
        int bytes_read = 0;
        memset(lengthOfstr, 0, 5);
                                                                //Parent will block here until there is something to read.
        while ((bytes_read = readNcheck(fd[i], lengthOfstr, 4, SM)) != 0)
        {                                                       //Read exactly 4 bytes.
            line_length = 0;
//            if ((lengthOfstr[0] != 'f') && (lengthOfstr[0] != 's'))
            if ((lengthOfstr[0] == ' ') || ((lengthOfstr[0] >= '1') && (lengthOfstr[0] <= '9')))
            {                                                   //Line to be read, contains an actual record.
                line_length = atoi(lengthOfstr);                //First 4 bytes read from fd_child, contain the length of the record.
                record = mallocNcheck(line_length, SM);
                memcpy(record, lengthOfstr, 4);
                readNcheck(fd[i], record + strlen(record), line_length - 4, SM);
                                                                //Read from fd_child record_length bytes - 4 (4 bytes where already read).
                writeNcheck(fd_parent, record, line_length, SM);//Write to fd_parent the record just read.
                free(record);
            }
            else if ((lengthOfstr[0] == 'f') || (lengthOfstr[0] == 's'))
            {                                                   //Line to be read, contains the turnaround time of a child.
                line_length = atoi(&(lengthOfstr[1]));
                time = mallocNcheck(line_length, SM);
                memcpy(time, lengthOfstr, 4);
                readNcheck(fd[i], time + strlen(time), line_length - 4, SM);
                writeNcheck(fd_parent, time, line_length, SM);  //Write to fd_parent the time just read.
                free(time);
            }
            memset(lengthOfstr, 0, 5);
        }
        checker = unlink(fifo);                                 //Remove child's named pipe.
        check_unlink(checker, SM);                              //Check if unlink() function succeeded.
        free(fifo);
    }
    return (fd_parent);
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
