#include "venus.h"

extern int input[];
extern int input_len;
extern int heap[];
extern int len;

static int parent(int index) { return (index - 1) / 2; }

static int left(int index) { return index * 2 + 1; }

static int right(int index) { return index * 2 + 2; }

static void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

static void insert(int key) {
    int index = len;
    heap[index] = key;

    /* Up heapify */
    while (index > 0 && heap[parent(index)] > heap[index]) {
        swap(&heap[parent(index)], &heap[index]);
        index = parent(index);
    }

    len += 1;
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

int main(void) {
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
