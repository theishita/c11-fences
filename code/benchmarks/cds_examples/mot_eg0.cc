#include <atomic>
#include <threads.h>
#include <stdio.h>

#include "model-assert.h"

#define SIZE 1

using namespace std;

atomic<int> flag_w1;
atomic<int> flag_w2;
atomic<int> flag_r;
atomic<int> num;
atomic<int> notify;
atomic<int> buffer;		// size of buffer can be changed
atomic<int> value;

static void writer1(void* arg) {
	flag_w1.store(1, memory_order_seq_cst);

	if (not (flag_w2.load(memory_order_relaxed))) {
		int x = value.load(memory_order_relaxed);
		value.store(x+1, memory_order_relaxed);
		x += 1;
		buffer.store(x, memory_order_release);
		notify.store(1, memory_order_release);
	}
}

static void writer2(void* arg) {
	flag_w2.store(1, memory_order_seq_cst);

	if (not (flag_w1.load(memory_order_relaxed))) {
		int x = value.load(memory_order_relaxed);
		value.store(x+1, memory_order_relaxed);
		x += 1;
		buffer.store(x, memory_order_release);
		notify.store(1, memory_order_release);
	}
}

static void reader1(void* arg) {
	if (notify.load(memory_order_acquire)){
		int x = buffer.load(memory_order_acquire);
		if (value.load(memory_order_relaxed) != 0)
			MODEL_ASSERT(x != 0);

        notify.store(1, memory_order_relaxed);
	}
}

static void reader2(void* arg) {
	if (notify.load(memory_order_acquire)){
		int x = buffer.load(memory_order_acquire);
		if (value.load(memory_order_relaxed) != 0)
			MODEL_ASSERT(x != 0);

        notify.store(1, memory_order_relaxed);
	}
}

int user_main(int argc, char **argv) {
	thrd_t t1, t2, t3, t4;

	atomic_init(&flag_w1, 0);
	atomic_init(&flag_w2, 0);
	atomic_init(&flag_r, 0);
	atomic_init(&num, 0);
	atomic_init(&notify, 0);
	atomic_init(&buffer, 0);
	atomic_init(&value, 0);

	thrd_create(&t1, (thrd_start_t)&writer1, NULL);
	thrd_create(&t2, (thrd_start_t)&writer2, NULL);
	thrd_create(&t3, (thrd_start_t)&reader1, NULL);
	thrd_create(&t4, (thrd_start_t)&reader2, NULL);		// reader 1, reader 2 are the same

	thrd_join(t1);
	thrd_join(t2);
	thrd_join(t3);
	thrd_join(t4);

	return 0;
}
