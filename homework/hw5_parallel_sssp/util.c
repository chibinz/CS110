#include <stdio.h>
#include <stdlib.h>

#include "global.h"

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

/* Print adjacency matrix */
void print_graph()
{
    for (unsigned i = 0; i < n; i++, putchar('\n'))
        for (unsigned j = 0; j < n; j++)
            if (c[i][j] == INF)
                printf("-      ");
            else
                printf("%-6d ", c[i][j]);
}

/* Print shortest from source s to target t */
void print_route(unsigned *pred, unsigned v)
{
    /* Use recursion to avoid using a stack to reverse input */
    if (v == s)
        return;
    else
    {
        print_route(pred, pred[v]);
        printf("%u\n", pred[v]);
    }
}
