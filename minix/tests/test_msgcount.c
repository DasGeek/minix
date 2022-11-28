#include <sys/msgcounter.h>
#include "common.h"

int main(int argc, char *argv[])
{
    int num_msgs = msg_counter();

    printf("The number of messages received since last clear is: %u\n", num_msgs);

    return 0;
}
