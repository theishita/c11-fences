// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/alpha2.litmus

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> vars0;
atomic<int> atom_1_r1_2; 
atomic<int> atom_1_r2_3; 

atomic<int> __fence_var;

static void t0(void *arg){
label_1:;
  vars0.store(2, memory_order_release);

}

static void t1(void *arg){
label_2:;
  vars0.store(3, memory_order_release);
  int v2_r1 = vars0.load(memory_order_acquire);
  int v4_r2 = vars0.load(memory_order_acquire);
  int v8 = (v2_r1 == 2);
  atom_1_r1_2.store(v8, memory_order_release);
  int v9 = (v4_r2 == 3);
  atom_1_r2_3.store(v9, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars0, 1);
  atomic_init(&atom_1_r1_2, 0); 
  atomic_init(&atom_1_r2_3, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v5 = atom_1_r1_2.load(memory_order_acquire);
  int v6 = atom_1_r2_3.load(memory_order_acquire);
  int v7_conj = v5 & v6;
  if (v7_conj == 1) MODEL_ASSERT(0);
  return 0;
}