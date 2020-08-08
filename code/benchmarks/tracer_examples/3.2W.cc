// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/3.2W.litmus

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> vars0;
atomic<int> vars1;
atomic<int> vars2;
atomic<int> __fence_var;

static void t0(void *arg){
label_1:;
  vars0.store(2, memory_order_release);
  vars1.store(1, memory_order_release);

}

static void t1(void *arg){
label_2:;
  vars1.store(2, memory_order_release);
  vars2.store(1, memory_order_release);

}

static void t2(void *arg){
label_3:;
  vars2.store(2, memory_order_release);
  vars0.store(1, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars2, 0);
  atomic_init(&vars1, 0);
  atomic_init(&vars0, 0);
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v1 = vars0.load(memory_order_acquire);
  int v2 = (v1 == 2);
  int v3 = vars1.load(memory_order_acquire);
  int v4 = (v3 == 2);
  int v5 = vars2.load(memory_order_acquire);
  int v6 = (v5 == 2);
  int v7_conj = v4 & v6;
  int v8_conj = v2 & v7_conj;
  if (v8_conj == 1) MODEL_ASSERT(0);
  return 0;
}