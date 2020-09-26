#include <stdio.h>
#include <threads.h>
#include <atomic>

std::atomic_int x;
std::atomic_int y;

static void a(void *obj)
{
	x.store(__LINE__, 1, std::memory_order_relaxed);
	printf("y: %d\n", y.load(__LINE__, std::memory_order_relaxed));
}

static void b(void *obj)
{
	y.store(__LINE__, 1, std::memory_order_relaxed);
	printf("x: %d\n", x.load(__LINE__, std::memory_order_relaxed));
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
	printf("Main thread is finished\n");

	return 0;
}
