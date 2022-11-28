#include <sys/trapcounter.h>
#include "common.h"

int main(int argc, char *argv[])
{
    int num_traps = trap_counter();

    printf("The number of traps hit since last clear is: %u\n", num_traps);

    return 0;
}
