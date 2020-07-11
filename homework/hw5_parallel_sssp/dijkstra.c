#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"

/* Find nearest not yet visited node */
static unsigned nearest(unsigned *dist, bool *visited)
{
    unsigned min = INF, index = 0;

    /* Iterate through dist to find nearest node on the frontier */
    for (unsigned i = 0; i < n; i++)
        if (!visited[i] && dist[i] < min)
            min = dist[i], index = i;

    return index;
}

/* Update distance to source if new path is shorter */
static void relax(unsigned *dist, unsigned *pred, unsigned u, unsigned v)
{
    /* Set temp to INF at overflow */
    unsigned temp = saturating_add(dist[u], c[u][v]);
    if (temp < dist[v])
    {
        /* Update distance to source if new path is shorter */
        dist[v] = temp;
        pred[v] = u;
    }
}

/* Sequential dijkstra algorithm */
void dijkstra()
{
    /* Distance from source to all nodes, initialized to infinity */
    unsigned *dist = malloc(sizeof(unsigned) * n);
    unsigned *pred = malloc(sizeof(unsigned) * n);
    bool *visited = calloc(n, sizeof(bool));

    /* Initialize distance from source to other nodes as infinity, itself as 0 */
    memset(dist, 0xff, sizeof(unsigned) * n);
    dist[s] = 0;

    /* Loop */
    for (unsigned i = 0; i < n - 1; i++)
    {
        /* Find nearest not yet visited node */
        unsigned u = nearest(dist, visited);

        /* Add node to visited */
        visited[u] = true;

        /* Terminal node is found, exit early */
        if (u == t)
            break;

        /* Update node u's neighbors */
        for (unsigned v = 0; v < n; v++)
            relax(dist, pred, u, v);
    }

    /* Print distance of shortest path from s to t */
    printf("%u\n", dist[t]);

    /* Print route from s to t */
    print_route(pred, t);
    printf("%u\n", t);

    /* Free allocated memory */
    free(dist), free(pred), free(visited);
}
