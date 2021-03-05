#ifdef __x86_64__

#include <stdio.h>
#include <stdlib.h>

int input[6] = {0, 1, 2, 3, -1, -2};
int input_len = 6;
int heap[32] = {0};
int len = 0;

void putint(int a) { printf("%d", a); }

#else

#include "venus.h"

extern int input[];
extern int input_len;
extern int heap[];
extern int len;

#endif

static int parent(int index) { return (index - 1) / 2; }

static int left(int index) { return index * 2 + 1; }

static int right(int index) { return index * 2 + 2; }

static void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

static void insert(int key) {
    if (len == 32) {
        exit(-1);
    }

    int index = len;
    heap[index] = key;
    len += 1;

    /* Up heapify */
    while (index > 0 && heap[parent(index)] > heap[index]) {
        swap(&heap[parent(index)], &heap[index]);
        index = parent(index);
    }
}

static void down_heapify(int index) {
    int l = left(index);
    int r = right(index);
    int min = index;

    if (l < len && heap[l] < heap[min]) {
        min = l;
    }

    if (r < len && heap[r] < heap[min]) {
        min = r;
    }

    if (min != index) {
        swap(&heap[index], &heap[min]);
        down_heapify(min);
    }
}

static void pop() {
    if (len == 0) {
        exit(-1);
    }

    putint(heap[0]);
    putchar('\n');

    len -= 1;
    heap[0] = heap[len];

    down_heapify(0);
}

static void print_heap() {
    for (int i = 0; i < len; i++) {
        putint(heap[i]);
        putchar(' ');
    }
    putchar('\n');
}

int main() {
    for (int i = 0; i < input_len; i++) {
        switch (input[i]) {
        case -1:
            pop();
            break;
        case -2:
            print_heap();
            break;
        default:
            insert(input[i]);
        }
    }

    exit(0);
}
