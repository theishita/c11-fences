// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/MP0129.litmus

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> vars0;
atomic<int> vars1;
atomic<int> vars2;
atomic<int> atom_1_r1_1; 
atomic<int> atom_1_r7_1; 

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
  int v4_r3 = vars2.load(memory_order_acquire);
  int v5_cmpeq = (v4_r3 == v4_r3);
  if (v5_cmpeq)  goto lbl_LC00; else goto lbl_LC00;
lbl_LC00:;
  vars0.store(1, memory_order_release);
  int v7_r7 = vars0.load(memory_order_acquire);
  int v14 = (v2_r1 == 1);
  atom_1_r1_1.store(v14, memory_order_release);
  int v15 = (v7_r7 == 1);
  atom_1_r7_1.store(v15, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars2, 0);
  atomic_init(&vars1, 0);
  atomic_init(&vars0, 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_1_r7_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v8 = vars0.load(memory_order_acquire);
  int v9 = (v8 == 2);
  int v10 = atom_1_r1_1.load(memory_order_acquire);
  int v11 = atom_1_r7_1.load(memory_order_acquire);
  int v12_conj = v10 & v11;
  int v13_conj = v9 & v12_conj;
  if (v13_conj == 1) MODEL_ASSERT(0);
  return 0;
}