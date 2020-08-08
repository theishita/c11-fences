// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/c6.litmus

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> vars0;
atomic<int> vars1;
atomic<int> vars2;
atomic<int> vars3;
atomic<int> __fence_var;

static void t0(void *arg){
label_1:;
  vars1.store(2, memory_order_release);
  int v2_r2 = vars1.load(memory_order_acquire);
  atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
  int v4_r1 = vars0.load(memory_order_acquire);
  vars2.store(v4_r1, memory_order_release);

}

static void t1(void *arg){
label_2:;
  vars0.store(1, memory_order_release);
  atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
  int v6_r2 = vars1.load(memory_order_acquire);
  vars3.store(v6_r2, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars3, 0);
  atomic_init(&vars2, 0);
  atomic_init(&vars1, 0);
  atomic_init(&vars0, 0);
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v7 = vars2.load(memory_order_acquire);
  int v8 = (v7 == 0);
  int v9 = vars3.load(memory_order_acquire);
  int v10 = (v9 == 0);
  int v11_conj = v8 & v10;
  if (v11_conj == 1) MODEL_ASSERT(0);
  return 0;
}