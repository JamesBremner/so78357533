#include "cTask.h"

main()
{
    // generate problem
    theClub.generate2();

    // check feasibility
    theClub.check();

    // allocate matches using max flow algorithm
    theClub.maxflow();

    theClub.display();

    return 0;
}
