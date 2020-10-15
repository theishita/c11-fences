/**
 * Watts - ib700wdt_01
*/

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

#define LOOP 5

atomic<char> expect_close;
atomic<bool> ibwdt_lock;

char wdt_start_port;
char write_user_buf;
char wdt_stop_port;
unsigned long ibwdt_is_open;

static void writer(void *arg) {
	wdt_start_port = 0;
}

static void closer(void *arg) {
	write_user_buf = 'V';
    expect_close.store(__LINE__, 0, memory_order_relaxed);
    if (write_user_buf == 'V') {
	    expect_close.store(__LINE__, 42, memory_order_relaxed);
	}
	wdt_start_port = 0;

	if(expect_close.load(__LINE__, memory_order_relaxed) == 42) {
		wdt_stop_port = 0;
	}
	else {
		MODEL_ASSERT(0);
	}
	ibwdt_is_open = 0;
    expect_close.store(__LINE__, 0, memory_order_relaxed);
}

int user_main(int argc, char **argv) {
	thrd_t t1, t2;

	atomic_init(&expect_close, 0);
	atomic_init(&ibwdt_lock, false);

	thrd_create(&t1, (thrd_start_t)&writer, NULL);
	thrd_create(&t2, (thrd_start_t)&closer, NULL);

	return 0;
}
