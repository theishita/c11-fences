#include <stdio.h>
#include <threads.h>
#include <atomic>

std::atomic_int x;
std::atomic_int y;

static void a(void *obj)
{
	x.store(__LINE__, 1, std::memory_order_relaxed);
	y.store(__LINE__, 1, std::memory_order_relaxed);
}

static void b(void *obj)
{
	printf("y1: %d\n", y.load(__LINE__, std::memory_order_relaxed));
	printf("x1: %d\n", x.load(__LINE__, std::memory_order_relaxed));
}

static void c(void *obj)
{
	printf("x2: %d\n", x.load(__LINE__, std::memory_order_relaxed));
	printf("y2: %d\n", y.load(__LINE__, std::memory_order_relaxed));
}

int user_main(int argc, char **argv)
{
	thrd_t t1, t2, t3;

	atomic_init(&x, 0);
	atomic_init(&y, 0);

	printf("Main thread: creating 3 threads\n");
	thrd_create(&t1, (thrd_start_t)&a, NULL);
	thrd_create(&t2, (thrd_start_t)&b, NULL);
	thrd_create(&t3, (thrd_start_t)&c, NULL);

	thrd_join(t1);
	thrd_join(t2);
	thrd_join(t3);
	printf("Main thread is finished\n");

	return 0;
}
