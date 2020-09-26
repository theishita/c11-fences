// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/MP+PPO420.litmus

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> vars0;
atomic<int> vars1;
atomic<int> vars2;
atomic<int> atom_1_r1_1; 
atomic<int> atom_1_r9_0; 

atomic<int> __fence_var;

static void t0(void *arg){
label_1:;
  vars0.store(__LINE__, 1, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  vars1.store(__LINE__, 1, memory_order_release);

}

static void t1(void *arg){
label_2:;
  int v2_r1 = vars1.load(__LINE__, memory_order_acquire);
  int v4_r3 = vars1.load(__LINE__, memory_order_acquire);
  int v5_cmpeq = (v4_r3 == v4_r3);
  if (v5_cmpeq)  goto lbl_LC00; else goto lbl_LC00;
lbl_LC00:;
  vars2.store(__LINE__, 1, memory_order_release);
  int v7_r6 = vars2.load(__LINE__, memory_order_acquire);
  vars2.store(__LINE__, 2, memory_order_release);
  int v9_r8 = vars2.load(__LINE__, memory_order_acquire);
  int v10_cmpeq = (v9_r8 == v9_r8);
  if (v10_cmpeq)  goto lbl_LC01; else goto lbl_LC01;
lbl_LC01:;
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v12_r9 = vars0.load(__LINE__, memory_order_acquire);
  int v19 = (v2_r1 == 1);
  atom_1_r1_1.store(__LINE__, v19, memory_order_release);
  int v20 = (v12_r9 == 0);
  atom_1_r9_0.store(__LINE__, v20, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars2, 0);
  atomic_init(&vars1, 0);
  atomic_init(&vars0, 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_1_r9_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v13 = vars2.load(__LINE__, memory_order_acquire);
  int v14 = (v13 == 2);
  int v15 = atom_1_r1_1.load(__LINE__, memory_order_acquire);
  int v16 = atom_1_r9_0.load(__LINE__, memory_order_acquire);
  int v17_conj = v15 & v16;
  int v18_conj = v14 & v17_conj;
  if (v18_conj == 1) MODEL_ASSERT(0);
  return 0;
}