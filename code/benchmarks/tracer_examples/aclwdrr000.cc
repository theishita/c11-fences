// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/aclwdrr000.litmus

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> vars0;
atomic<int> vars1;
atomic<int> vars2;
atomic<int> atom_3_r1_2; 
atomic<int> atom_3_r3_0; 
atomic<int> __fence_var;

static void t0(void *arg){
label_1:;
  vars0.store(1, memory_order_release);
  atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
  vars1.store(1, memory_order_release);

}

static void t1(void *arg){
label_2:;
  vars1.store(2, memory_order_release);
  atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
  vars2.store(1, memory_order_release);

}

static void t2(void *arg){
label_3:;
  vars2.store(2, memory_order_release);

}

static void t3(void *arg){
label_4:;
  int v2_r1 = vars2.load(memory_order_acquire);
  atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
  int v4_r3 = vars0.load(memory_order_acquire);
  int v14 = (v2_r1 == 2);
  atom_3_r1_2.store(v14, memory_order_release);
  int v15 = (v4_r3 == 0);
  atom_3_r3_0.store(v15, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 
  thrd_t thr3; 

  atomic_init(&vars2, 0);
  atomic_init(&vars1, 0);
  atomic_init(&vars0, 0);
  atomic_init(&atom_3_r1_2, 0); 
  atomic_init(&atom_3_r3_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);
  thrd_create(&thr3, t3, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);
  thrd_join(thr3);

  int v5 = vars1.load(memory_order_acquire);
  int v6 = (v5 == 2);
  int v7 = vars2.load(memory_order_acquire);
  int v8 = (v7 == 2);
  int v9 = atom_3_r1_2.load(memory_order_acquire);
  int v10 = atom_3_r3_0.load(memory_order_acquire);
  int v11_conj = v9 & v10;
  int v12_conj = v8 & v11_conj;
  int v13_conj = v6 & v12_conj;
  if (v13_conj == 1) MODEL_ASSERT(0);
  return 0;
}