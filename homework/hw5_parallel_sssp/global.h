/* Infinity distance */
#define INF 0xffffffff

/* Number of vertices, index of source vertex, index of target vertex */
unsigned n, s, t;

/* Pointer to adjacency matrix (cost matrix) */
unsigned **c;

void print_route(unsigned *pred, unsigned v);

void dijkstra();
unsigned saturating_add(unsigned a, unsigned b);
unsigned getuint();
void alloc_graph();
void free_graph();
void read_graph();
void print_graph();
void print_route(unsigned *pred, unsigned v);