/**
 * GenMC - names-2d-array0
*/

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

#define NUM 5

typedef struct node {
	int value;
	int next;

} node_t;

typedef struct {
	int top;
	node_t nodes[4][2];
} mystack_t;

mystack_t stack;

atomic<int> x ;

int a[2][2];

static void thread_1(void *arg) {
	stack.nodes[2][1].next = 2;
	stack.nodes[3][1].value = 3;

	if (x.load(__LINE__, memory_order_relaxed) == 42) {
		a[1][1] = 42;
		a[0][0] = 17;
	}
}

static void thread_2(void *arg) {
	a[1][0] = 3;
	stack.nodes[1][1].value = 42;
	x.store(__LINE__, 42, memory_order_relaxed);
}

int user_main(int argc, char **argv) {
    thrd_t t1, t2;

    thrd_create(&t2, (thrd_start_t)&thread_2, NULL);
    thrd_create(&t1, (thrd_start_t)&thread_1, NULL);

    thrd_join(t1);
    thrd_join(t2);

    return 0;
}
