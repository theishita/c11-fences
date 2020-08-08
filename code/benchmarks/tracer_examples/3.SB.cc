// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/3.SB.litmus

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> vars0;
atomic<int> vars1;
atomic<int> vars2;
atomic<int> atom_0_r3_0; 
atomic<int> atom_1_r3_0; 
atomic<int> atom_2_r3_0; 

atomic<int> __fence_var;

static void t0(void *arg){
label_1:;
  vars0.store(1, memory_order_release);
  int v2_r3 = vars1.load(memory_order_acquire);
  int v12 = (v2_r3 == 0);
  atom_0_r3_0.store(v12, memory_order_release);

}

static void t1(void *arg){
label_2:;
  vars1.store(1, memory_order_release);
  int v4_r3 = vars2.load(memory_order_acquire);
  int v13 = (v4_r3 == 0);
  atom_1_r3_0.store(v13, memory_order_release);

}

static void t2(void *arg){
label_3:;
  vars2.store(1, memory_order_release);
  int v6_r3 = vars0.load(memory_order_acquire);
  int v14 = (v6_r3 == 0);
  atom_2_r3_0.store(v14, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars2, 0);
  atomic_init(&vars1, 0);
  atomic_init(&vars0, 0);
  atomic_init(&atom_0_r3_0, 0); 
  atomic_init(&atom_1_r3_0, 0); 
  atomic_init(&atom_2_r3_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v7 = atom_0_r3_0.load(memory_order_acquire);
  int v8 = atom_1_r3_0.load(memory_order_acquire);
  int v9 = atom_2_r3_0.load(memory_order_acquire);
  int v10_conj = v8 & v9;
  int v11_conj = v7 & v10_conj;
  if (v11_conj == 1) MODEL_ASSERT(0);
  return 0;
}