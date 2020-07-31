#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> x;
atomic<int> y;
atomic<int> cpu;
atomic<int> dum_var;

static void a(void *obj) {
	dum_var.store(0, memory_order_relaxed);
	x.store(42);
	y.load();
	cpu.store(1);
	MODEL_ASSERT(cpu.load() == 1);
}

static void b(void *obj) {
	dum_var.store(0, memory_order_relaxed);
	y.store(42);
	x.load();
	cpu = 42;
	MODEL_ASSERT(cpu.load() == 42);
}

int user_main(int argc, char **argv) {
	thrd_t t1, t2;

	atomic_init(&x, 0);
	atomic_init(&y, 0);
	atomic_init(&cpu, 0);
	atomic_init(&dum_var, 0);

	thrd_create(&t2, (thrd_start_t)&b, NULL);
	thrd_create(&t1, (thrd_start_t)&a, NULL);

	thrd_join(t2);
	thrd_join(t1);

	MODEL_ASSERT(!(x.load(memory_order_relaxed) == 1 && y.load(memory_order_relaxed) == 1));

	return 0;
}
