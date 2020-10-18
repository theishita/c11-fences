/**
 * CDSChecker - mot_eg
 * BUFFER SIZE = 1
 * Modified some of the orginal file
*/

#include <model-assert.h>
#include <threads.h>
#include <atomic>
#include <iostream>

using namespace std;

atomic<int> flag_w1;
atomic<int> flag_w2;
atomic<int> flag_r;
atomic<int> num;
atomic<int> notify;
atomic<int> buffer;
atomic<int> value;

static void writer1(void* arg) {
	flag_w1.store(__LINE__, 1, memory_order_seq_cst);

	if (not(flag_w2.load(__LINE__, memory_order_relaxed))) {
		int x = value.load(__LINE__, memory_order_relaxed);
		value.store(__LINE__, x + 1, memory_order_relaxed);
		x += 1;
		buffer.store(__LINE__, x, memory_order_release);
		notify.store(__LINE__, 1, memory_order_release);
	}
}

static void writer2(void* arg) {
	flag_w2.store(__LINE__, 1, memory_order_seq_cst);

	if (not(flag_w1.load(__LINE__, memory_order_relaxed))) {
		int x = value.load(__LINE__, memory_order_relaxed);
		value.store(__LINE__, x + 1, memory_order_relaxed);
		x += 1;
		buffer.store(__LINE__, x, memory_order_release);
		notify.store(__LINE__, 1, memory_order_release);
	}
}

static void reader1(void* arg) {
	if (notify.load(__LINE__, memory_order_acquire)) {
		int x = buffer.load(__LINE__, memory_order_acquire);
		if (value.load(__LINE__, memory_order_relaxed) != 0)
			MODEL_ASSERT(x != 0);

		notify.store(__LINE__, 1, memory_order_relaxed);
	}
}

static void reader2(void* arg) {
	if (notify.load(__LINE__, memory_order_acquire)) {
		int x = buffer.load(__LINE__, memory_order_acquire);
		if (value.load(__LINE__, memory_order_relaxed) != 0)
			MODEL_ASSERT(x != 0);

		notify.store(__LINE__, 1, memory_order_relaxed);
	}
}

int user_main(int argc, char** argv) {
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
	thrd_create(&t4, (thrd_start_t)&reader2, NULL);

	thrd_join(t1);
	thrd_join(t2);
	thrd_join(t3);
	thrd_join(t4);

	return 0;
}