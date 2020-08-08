// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/S0022.litmus

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> vars0;
atomic<int> vars1;
atomic<int> atom_1_r1_1; 

atomic<int> __fence_var;

static void t0(void *arg){
label_1:;
  vars0.store(2, memory_order_release);
  int v2_r3 = vars0.load(memory_order_acquire);
  int v3_cmpeq = (v2_r3 == v2_r3);
  if (v3_cmpeq)  goto lbl_LC00; else goto lbl_LC00;
lbl_LC00:;
  vars1.store(1, memory_order_release);

}

static void t1(void *arg){
label_2:;
  int v5_r1 = vars1.load(memory_order_acquire);
  int v6_r3 = v5_r1 ^ v5_r1;
  int v7_r3 = v6_r3 + 1;
  vars0.store(v7_r3, memory_order_release);
  int v12 = (v5_r1 == 1);
  atom_1_r1_1.store(v12, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars1, 0);
  atomic_init(&vars0, 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v8 = vars0.load(memory_order_acquire);
  int v9 = (v8 == 2);
  int v10 = atom_1_r1_1.load(memory_order_acquire);
  int v11_conj = v9 & v10;
  if (v11_conj == 1) MODEL_ASSERT(0);
  return 0;
}