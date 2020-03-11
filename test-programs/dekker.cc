#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> flag1;
atomic<int> flag2;
atomic<int> turn;
atomic<int> x;

#define LOOP 5

static void t1(void *arg)
{
	flag1.store(1, memory_order_seq_cst);
	for(int k=0;k<LOOP;k++) { if(flag2.load(memory_order_relaxed) >= 1) {
		if(turn.load(memory_order_relaxed) != 0) {
			flag1.store(0,memory_order_relaxed);
			while(turn.load(memory_order_relaxed) != 0) {};
			flag1.store(1,memory_order_relaxed); }	}  }
	x.store(0,memory_order_relaxed);
	MODEL_ASSERT(x.load(memory_order_relaxed) <= 0);
	turn.store(1,memory_order_relaxed);
	flag1.store(0,memory_order_relaxed);

}

static void t2(void *arg)
{
	flag2.store(1, memory_order_seq_cst);
	for(int k=0;k<LOOP;k++) { if(flag1.load(memory_order_relaxed) >= 1) {
		if(turn.load(memory_order_relaxed) != 1) {
			flag2.store(0,memory_order_relaxed);
			while(turn.load(memory_order_relaxed) != 1) {};
			flag2.store(1,memory_order_relaxed); }	}  }
	x.store(1,memory_order_relaxed);
	MODEL_ASSERT(x.load(memory_order_relaxed) >= 1);
	turn.store(1,memory_order_relaxed);
	flag2.store(0,memory_order_relaxed);
}




int user_main(int argc, char **argv)
{
	thrd_t id1, id2;

    atomic_init(&flag1, 0);
    atomic_init(&flag2, 0);
    atomic_init(&turn, 0);
	
	// MODEL_ASSERT(turn.load(memory_order_relaxed)>=0 && turn.load(memory_order_relaxed)<=1);

	thrd_create(&id1, (thrd_start_t)&t1, NULL);
    thrd_create(&id2, (thrd_start_t)&t2, NULL);

    thrd_join(id1);
    thrd_join(id2);

  
	return 0;
}
