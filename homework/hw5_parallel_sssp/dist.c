#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"

/* Calculate the shortest distance given the input. */
void dist(int max_threads)
{
    n = getuint();
    s = getuint();
    t = getuint();

    alloc_graph();
    read_graph();

    dijkstra();

    free_graph();
}
