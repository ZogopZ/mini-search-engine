#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

#include "checker.h"

void* mallocNcheck(size_t incoming_size, int caller)
{
    void *ptr;

    ptr = malloc(incoming_size + 1);                            //+1 for terminating character.
    if (ptr == NULL && incoming_size > 0)                       //malloc() function failed.
    {
        if (caller == ROOT)
            perror("---|MALLOC FAIL ROOT| ");
        else if (caller == SM)
            perror("---|MALLOC FAIL SPLITTER/MERGER| ");
        else if (caller == LEAF)
            perror("---|MALLOC FAIL LEAF| ");
        exit(EXIT_FAILURE);
    }
    memset(ptr, 0, incoming_size + 1);                          //Every allocated string will have a null terminating character.
    return ptr;                                                 //Every pointer is initialized to NULL.
}

int readNcheck(int fd_child, char* buffer, int full_size, int caller)
{
    int bytes_read = 0;
    int byte_checker = 0;
    int bytes_append = 0;

    while (((bytes_read = read(fd_child, &(buffer[bytes_append]), full_size)) > 0)
           && (byte_checker <= full_size))                      //Repeat until the asked size from fd is read in the buffer.
    {
        if (bytes_read == -1)                                   //read() function failed.
        {
            if (caller == ROOT)
                perror("---|READ FAIL ROOT| ");
            else if (caller == SM)
                perror("---|READ FAIL SPLITTER/MERGER| ");
            else if (caller == LEAF)
                perror("---|READ FAIL LEAF| ");
            exit(EXIT_FAILURE);
        }
        byte_checker = byte_checker + bytes_read;
        bytes_append = bytes_append + bytes_read;
        if (byte_checker == full_size)                          //The asked size was read to the buffer.
            break;
        else if (byte_checker < full_size)                      //Read was incomplete... Read again.
        {
            full_size = full_size - byte_checker;
            byte_checker = 0;
        }
    }
    return bytes_append;                                        //Return sum of bytes read from fd (All bytes asked from calling function).
}

int writeNcheck(int fd_parent, char* buffer, int full_size, int caller)
{
    int bytes_wrote = 0;
    int byte_checker = 0;
    int bytes_whence = 0;

    while (((bytes_wrote = write(fd_parent, &(buffer[bytes_whence]), full_size)) > 0)
           && (byte_checker <= full_size))                      //Repeat until the asked size from buffer is written to fd.
    {
        if (bytes_wrote == -1)                                  //write() function failed.
        {
            if (caller == SM)
                perror("---|WRITE FAIL SPLITTER/MERGER| ");
            else if (caller == LEAF)
                perror("---|WRITE FAIL LEAF| ");
            exit(EXIT_FAILURE);
        }
        byte_checker = byte_checker + bytes_wrote;
        bytes_whence = bytes_whence + bytes_wrote;
        if (byte_checker == full_size)                          //The asked size was written to fd.
            break;
        else if (byte_checker < full_size)                      //Asked size was not written to fd. Write again size remaining.
        {
            full_size = full_size - byte_checker;
            byte_checker = 0;
        }
    }
    return bytes_whence;                                        //Return sum of bytes wrote to fd (All bytes asked from calling function).
}

void check_fork(pid_t pid_check, int caller)
{
    if (pid_check == -1)                                        //fork() function failed.
    {
        if (caller == ROOT)
            perror("---|FORK FAIL ROOT| ");
        else if (caller == SM)
            perror("---|FORK FAIL SPLITTER/MERGER| ");
        else if (caller == LEAF)
            perror("---|FORK FAIL LEAF| ");
        exit(EXIT_FAILURE);
    }
    return;
}

void check_open(int fd_in, int which_file, int caller)
{
    if ((which_file == FIFO_FILE) && (fd_in == -1))             //open() function failed.
    {
        if (caller == ROOT)
            perror("---|OPEN FAIL FIFO-FILE ROOT| ");
        else if (caller == SM)
            perror("---|OPEN FAIL FIFO-FILE SPLITTER/MERGER| ");
        else if (caller == LEAF)
            perror("---|OPEN FAIL FIFO-FILE LEAF| ");
        exit(EXIT_FAILURE);
    }
    return;
}

void check_fopen(FILE* fp_in, int which_file, int caller)
{
    if ((which_file == BIN_FILE) && (fp_in == NULL))            //fopen() function failed for binary file.
    {
        if (caller == ROOT)
            perror("---|FOPEN FAIL BIN-FILE ROOT| ");
        else if (caller == LEAF)
            perror("---|FOPEN FAIL BIN-FILE LEAF| ");
        exit(EXIT_FAILURE);
    }
    else if ((which_file == SPLIT_FILE) && (fp_in == NULL))     //fopen() function failed for splitter file..
    {
        if (caller == ROOT)
            perror("---|FOPEN FAIL SPLIT-FILE ROOT| ");
        else if (caller == LEAF)
            perror("---|FOPEN FAIL SPLIT-FILE LEAF| ");
        exit(EXIT_FAILURE);
    }
    return;
}

void check_mkfifo(int value_check, int caller)
{
    if (value_check < 0 )                                       //mkfifo() function failed.
    {
        if (caller == ROOT)
            perror("---|CREATE FAIL FIFO-FILE ROOT| ");
        else if (caller == SM)
            perror("---|CREATE FAIL FIFO-FILE SPLITTER/MERGER| ");
        exit(EXIT_FAILURE);
    }
    return;
}

void check_unlink(int value_check, int caller)
{
    if (value_check != 0 )                                      //unlink() function failed.
    {
        if (caller == ROOT)
            perror("---|UNLINK FAIL ROOT| ");
        else if (caller == SM)
            perror("---|UNLINK FAIL SPLITTER/MERGER| ");
        exit(EXIT_FAILURE);
    }
    return;
}

void check_exec(int caller)
{
    if (caller == ROOT)
        perror("---|EXEC FAIL ROOT| ");
    else if (caller == SM)
        perror("---|EXEC FAIL SPLITTER/MERGER| ");
    exit(EXIT_FAILURE);
    return;
}

void parseNcheck(int parse_value, int function, int caller)
{
    if (caller == ROOT)
    {
        if ((function == ARGC_CHECK) && (parse_value < MIN_ARGS_ROOT || parse_value > MAX_ARGS_ROOT))
        {                                                           //Check if given arguments for root are less than 7 or more than 8.
            printf("---|PARSE FAIL ROOT: Number of arguments is incorrect. Aborting...|\n");
            exit(EXIT_FAILURE);
        }
        else if ((function == HEIGHT_CHECK) && ((parse_value < 1) || (parse_value > 5)))
        {                                                           //Check if height argument, following flag -h, has a valid value.
            printf("---|PARSE FAIL ROOT: Height value is incorrect. ");
            printf("Valid height values are 1, 2, 3, 4 or 5. Aborting...|\n");
            exit(EXIT_FAILURE);
        }
    }
    else if ((caller == SM) && (parse_value != ARGS_SM))            //Check if given arguments for sm are exactly 13.
    {
        printf("---|PARSE FAIL SLPITTER/MERGER: Number of arguments is incorrect. Aborting...|\n");
        exit(EXIT_FAILURE);
    }
    else if ((caller == LEAF) && (parse_value != ARGS_LEAF))        //Check if given arguments for leaf are exactly 11.
    {
        printf("---|PARSE FAIL LEAF: Number of arguments is incorrect. Aborting...|\n");
        exit(EXIT_FAILURE);
    }
    return;
}
