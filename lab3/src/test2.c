#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    pid_t child_pid = fork();
    if (child_pid == 0){
        execl("sequential_min_max", "sequential_min_max", "10", "9", NULL);
    }
    else 
        exit(0);
}