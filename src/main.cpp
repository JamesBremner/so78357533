#include "cTask.h"

main()
{
    // generate problem
    generate2();

    // check feasibility
    check();

    // allocate matches using max flow algorithm
    maxflow();

    return 0;
}
