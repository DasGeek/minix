#include "common.h"
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Invalid number of arguments. Expected args are:\n\tint pid - PID of process to change priority\n\tint prio - Value by which to change the priority\n");
    }

    nicer_to( atoi(argv[1]), atoi(argv[2]) );

    return 0;
}
