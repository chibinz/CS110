#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Infinity distance */
#define INF 0xffffffff

/* Number of vertices, index of source vertex, index of target vertex, number of threads*/
unsigned n, s, t, cheer, nt;

/* Pointer to adjacency matrix (cost matrix) */
unsigned **c;

unsigned *dis, *pred;
bool *visited;

unsigned min[4], ind[4], id[4];
pthread_t thread[4];

/* Set to INF when maxing out */
unsigned saturating_add(unsigned a, unsigned b)
{
    unsigned c = a + b;

    return c < a ? INF : c;
}

/* Fast IO */
unsigned getuint()
{
    unsigned a = 0;
    char c = 0;

    /* Skip non digit characters */
    while (c < 33)
        c = getchar_unlocked();

    /* Cycle through digit characters */
    while (c > 33)
    {
        a = a * 10 + c - '0';

        /* getchar_unlocked is not thread safe but we are not allowed to
            use multithreaded io, so it won't bother */
        c = getchar_unlocked();
    }

    return a;
}

/* Allocate memory for adjacency matrix */
void alloc_graph()
{
    /* Allocate pointer array */
    c = malloc(sizeof(unsigned *) * n);

    /* Allocate cost array */
    for (unsigned i = 0; i < n; i++)
        c[i] = malloc(sizeof(unsigned) * n);
}

/* Free allocated memory for adjacency matrix */
void free_graph()
{
    for (unsigned i = 0; i < n; i++)
        free(c[i]);

    free(c);
}

/* Read n * n matrix from stand input into c */
void read_graph()
{
    for (unsigned i = 0; i < n; i++)
        for (unsigned j = 0; j < n; j++)
        {
            unsigned temp = getuint();

            /* Initialize to infinity */
            c[i][j] = temp == 0 && i != j ? INF : temp;
        }
}

/* Print shortest from source s to target t */
void print_route(unsigned *pre, unsigned v)
{
    /* Use recursion to avoid using a stack to reverse input */
    if (v == s)
        return;
    else
    {
        /* Post printing */
        print_route(pre, pre[v]);
        printf("%u\n", pre[v]);
    }
}

/* Iterate through dist to find nearest node on the frontier */
void *parallel_nearest(void *tid)
{
    unsigned id = *(int *)tid;

    min[id] = INF;
    ind[id] = 0;

    /* Start from id and increment by number of thread so there will
        be no race condition */
    for (unsigned i = id; i < n; i += nt)
        if (!visited[i] && dis[i] < min[id])
            min[id] = dis[i], ind[id] = i;

    return NULL;
}

/* Find nearest not yet visited node */
static unsigned nearest()
{
    unsigned m = INF, idx = 0;

    /* Search min in parallel */
    for (int i = 0; i < nt; i++)
        pthread_create(&thread[i], NULL, parallel_nearest, &id[i]);

    for (int i = 0; i < nt; i++)
        pthread_join(thread[i], NULL);

    /* Iterate through dist to find nearest node on the frontier */
    for(unsigned i = 0; i < nt; i++)
        if(min[i] < m)
            m = min[i], idx = ind[i];

    return idx;
}

/* Update distance to source if new path is shorter */
static void relax(unsigned u, unsigned v)
{
    /* Set temp to INF at overflow */
    unsigned temp = saturating_add(dis[u], c[u][v]);
    if (temp < dis[v])
    {
        /* Update distance to source if new path is shorter */
        dis[v] = temp;
        pred[v] = u;
    }
}

/* Sequential dijkstra algorithm */
void dijkstra()
{
    /* Distance from source to all nodes, initialized to infinity */
    dis = malloc(sizeof(unsigned) * n);
    pred = malloc(sizeof(unsigned) * n);
    visited = calloc(n, sizeof(bool));

    /* Initialize distance from source to other nodes as infinity, itself as 0 */
    memset(dis, 0xff, sizeof(unsigned) * n);
    dis[s] = 0;

    for (unsigned i = 0; i < 4; i++)
        id[i] = i;

    /* Loop */
    for (unsigned i = 0; i < n - 1; i++)
    {
        /* Find nearest not yet visited node */
        unsigned u = nearest();

        /* Add node to visited */
        visited[u] = true;

        /* Terminal node is found, exit early */
        if (u == t)
            break;

        /* Update node u's neighbors */
        for (unsigned v = 0; v < n; v++)
            relax(u, v);
    }

    /* Print distance of shortest path from s to t */
    printf("%u\n", dis[t]);

    /* Print route from s to t */
    print_route(pred, t);
    printf("%u\n", t);

    /* Free allocated memory */
    free(dis), free(pred), free(visited);
}

/* Calculate the shortest distance given the input. */
void dist(int max_threads)
{
    /* Read n, s, t */
    n = getuint();
    s = getuint();
    t = getuint();

    nt = max_threads > 4 ? 4 : max_threads;

    /* Graph Initialzation */
    alloc_graph();
    read_graph();

    dijkstra();

    /* Graph destruction */
    free_graph();
}
