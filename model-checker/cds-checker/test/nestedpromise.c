#include <stdlib.h>
#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>

#include "librace.h"
#include "model-assert.h"

atomic_int x;
atomic_int y;
atomic_int z;
static void a(void *obj)
{
	(void)atomic_load_explicit(__LINE__, &z, memory_order_relaxed); // this is only for schedule control
	int t1=atomic_load_explicit(__LINE__, &x, memory_order_relaxed);
	atomic_store_explicit(__LINE__, &y, 1, memory_order_relaxed);
	printf("t1=%d\n",t1);
}

static void b(void *obj)
{
	int t2=atomic_load_explicit(__LINE__, &y, memory_order_relaxed);
	atomic_store_explicit(__LINE__, &x, t2, memory_order_relaxed);
}

int user_main(int argc, char **argv)
{
	thrd_t t1, t2;


	atomic_init(&x, 0);
	atomic_init(&y, 0);
	atomic_init(&z, 0);
	thrd_create(&t1, (thrd_start_t)&a, NULL);
	thrd_create(&t2, (thrd_start_t)&b, NULL);

	thrd_join(t1);
	thrd_join(t2);


	return 0;
}
