// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/AddrRW.litmus

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> vars0;
atomic<int> vars1;
atomic<int> vars2;
atomic<int> atom_1_r1_1; 
atomic<int> atom_1_r6_1; 
atomic<int> __fence_var;

static void t0(void *arg){
label_1:;
  vars0.store(2, memory_order_release);
  atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
  vars1.store(1, memory_order_release);

}

static void t1(void *arg){
label_2:;
  int v2_r1 = vars1.load(memory_order_acquire);
  int v3_r3 = v2_r1 ^ v2_r1;

  int sum = 2+v3_r3;
  if(sum == 0) {
    vars0.store(1, memory_order_release);
  }
  else if(sum == 1) {
    vars1.store(1, memory_order_release);
  }
  else if(sum == 2) {
    vars2.store(1, memory_order_release);
  }

  int v5_r6 = vars2.load(memory_order_acquire);
  vars0.store(1, memory_order_release);
  int v12 = (v2_r1 == 1);
  atom_1_r1_1.store(v12, memory_order_release);
  int v13 = (v5_r6 == 1);
  atom_1_r6_1.store(v13, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars2, 0);
  atomic_init(&vars1, 0);
  atomic_init(&vars0, 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_1_r6_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v6 = vars0.load(memory_order_acquire);
  int v7 = (v6 == 2);
  int v8 = atom_1_r1_1.load(memory_order_acquire);
  int v9 = atom_1_r6_1.load(memory_order_acquire);
  int v10_conj = v8 & v9;
  int v11_conj = v7 & v10_conj;
  if (v11_conj == 1) MODEL_ASSERT(0);
  return 0;
}