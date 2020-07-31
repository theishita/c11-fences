#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> data;
atomic<int> dum_var;

static void t1(void *arg) {
	int d = data.load(memory_order_seq_cst);
	d += 1;
	dum_var.store(0, memory_order_relaxed);
	data.store(d, memory_order_seq_cst);
}

static void t2(void *arg) {
	int d = data.load(memory_order_seq_cst);
	d += 2;
	dum_var.store(0, memory_order_relaxed);
	data.store(d, memory_order_seq_cst);
}

static void t3(void *arg) {
	MODEL_ASSERT(data.load(memory_order_seq_cst) < 3);
}

int user_main(int argc, char **argv) {
	thrd_t id1, id2, id3;

	atomic_init(&data, 0);
	atomic_init(&dum_var, 0);

	thrd_create(&id1, (thrd_start_t)&t1, NULL);
	thrd_create(&id2, (thrd_start_t)&t2, NULL);
	thrd_create(&id3, (thrd_start_t)&t3, NULL);

	thrd_join(id1);
	thrd_join(id2);
	thrd_join(id3);

	return 0;
}