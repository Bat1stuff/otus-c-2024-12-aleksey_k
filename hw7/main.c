#include <stdio.h>
#include <stdlib.h>

long data[] = {4, 8, 15, 16, 23, 42};
int data_length = sizeof(data) / sizeof(data[0]);

struct Node {
    long data;
    struct Node* next;
};

void print_int(const long value) {
    printf("%ld ", value);
    fflush(stdout);
}

long p(const long value) {
    return value & 1;
}

struct Node* add_element(long data, struct Node* next) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    if (!node) {
        abort();
    }
    node->data = data;
    node->next = next;
    return node;
}

void m(const struct Node* node, void (*func)(long)) {
    if (node == NULL) {
        return;
    }
    func(node->data);
    m(node->next, func);
}

struct Node* f(struct Node* node, struct Node* acc, long (*predicate)(long)) {
    if (!node) {
        return acc;
    }
    if (predicate(node->data)) {
        return f(node->next, add_element(node->data, acc), predicate);
    }

    return f(node->next, acc, predicate);
}

void free_list(struct Node* node) {
    while (node) {
        struct Node* temp = node;
        node = node->next;
        free(temp);
    }
}

int main() {
    struct Node* list = NULL;
    for (int i = data_length - 1; i >= 0; i--) {
        list = add_element(data[i], list);
    }

    m(list, print_int);
    puts("");

    struct Node* filtered_list = f(list, NULL, &p);

    m(filtered_list, print_int);
    puts("");

    free_list(list);
    free_list(filtered_list);

    return 0;
}