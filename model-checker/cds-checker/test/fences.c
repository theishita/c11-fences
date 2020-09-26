#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>

#include "librace.h"

atomic_int x;
atomic_int y;

static void a(void *obj)
{
	atomic_store_explicit(__LINE__, &x, 1, memory_order_relaxed);
	atomic_store_explicit(__LINE__, &x, 2, memory_order_relaxed);
	atomic_thread_fence(memory_order_seq_cst);
	printf("Thread A reads: %d\n", atomic_load_explicit(__LINE__, &y, memory_order_relaxed));
}

static void b(void *obj)
{
	atomic_store_explicit(__LINE__, &y, 1, memory_order_relaxed);
	atomic_store_explicit(__LINE__, &y, 2, memory_order_relaxed);
	atomic_thread_fence(memory_order_seq_cst);
	printf("Thread B reads: %d\n", atomic_load_explicit(__LINE__, &x, memory_order_relaxed));
}

int user_main(int argc, char **argv)
{
	thrd_t t1, t2;

	atomic_init(&x, 0);
	atomic_init(&y, 0);

	printf("Main thread: creating 2 threads\n");
	thrd_create(&t1, (thrd_start_t)&a, NULL);
	thrd_create(&t2, (thrd_start_t)&b, NULL);

	thrd_join(t1);
	thrd_join(t2);
	printf("Main thread is finishing\n");

	return 0;
}
