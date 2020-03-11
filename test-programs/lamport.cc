#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> x;
atomic<int> y;
atomic<int> b1;
atomic<int> b2;
atomic<int> X;

#define LOOP 5

static void t1(void *arg)
{
    for (int k = 0; k < LOOP; k++) { b1.store(1,memory_order_relaxed);
	x.store(1,memory_order_relaxed);
	if (y.load(memory_order_relaxed) != 0) {
		b1.store(0,memory_order_relaxed);
		while (y.load(memory_order_relaxed) != 0) {}; continue; }
	y.store(1,memory_order_relaxed);
	if(x.load(memory_order_relaxed) != 1) {
		b1.store(0,memory_order_relaxed);
		while(b2.load(memory_order_relaxed) >= 1) {};

	}	}
}

static void t2(void *arg)
{
	y.store(1, memory_order_relaxed);
    int temp = x.load(memory_order_relaxed);
    b.store(temp,memory_order_relaxed);
    int temp2 = z.load(memory_order_relaxed);
	c.store(temp2, memory_order_relaxed);
}

int user_main(int argc, char **argv)
{
	thrd_t id1, id2;

	thrd_create(&id1, (thrd_start_t)&t1, NULL);
    thrd_create(&id2, (thrd_start_t)&t2, NULL);

    thrd_join(id1);
    thrd_join(id2);
  
	return 0;
}