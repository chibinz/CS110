#include "venus.h"
#define CAPACITY 8

int input[8] = {8, 7, 6, 5, 4, 3, 2, 1};
int heap[8] = {0};
int len = 0;

int parent(int index) { return (index - 1) / 2; }

int left(int index) { return index * 2 + 1; }

int right(int index) { return index * 2 + 2; }

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void insert(int key) {
    int index = len;
    heap[index] = key;

    /* Up heapify */
    while (index > 0 && heap[parent(index)] > heap[index]) {
        swap(&heap[parent(index)], &heap[index]);
        index = parent(index);
    }

    len += 1;
}

void down_heapify(int index) {
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

int pop() {
    int index = 0;
    int ret = heap[0];
    len -= 1;

    swap(&heap[0], &heap[len]);
    down_heapify(0);

    return ret;
}

void print_heap() {
    for (int i = 0; i < len; i++) {
        print_int(heap[i]);
    }
}

int main(void) {
    for (int i = 0; i < CAPACITY; i++) {
        insert(input[i]);
    }

    print_int(pop());

    print_heap();

    exit(0);
}
