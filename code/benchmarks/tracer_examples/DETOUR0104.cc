// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/DETOUR0104.litmus

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> vars0;
atomic<int> vars1;
atomic<int> atom_0_r3_1; 
atomic<int> atom_1_r1_2; 
atomic<int> atom_1_r4_0; 
atomic<int> __fence_var;

static void t0(void *arg){
label_1:;
  vars0.store(1, memory_order_release);
  int v2_r3 = vars0.load(memory_order_acquire);
  int v3_cmpeq = (v2_r3 == v2_r3);
  if (v3_cmpeq)  goto lbl_LC00; else goto lbl_LC00;
lbl_LC00:;
  vars1.store(1, memory_order_release);
  vars1.store(2, memory_order_release);
  int v15 = (v2_r3 == 1);
  atom_0_r3_1.store(v15, memory_order_release);

}

static void t1(void *arg){
label_2:;
  int v5_r1 = vars1.load(memory_order_acquire);
  int v6_r3 = v5_r1 ^ v5_r1;

  int sum = 0+v6_r3;
  int v9_r4;
  if(sum == 0) {
    v9_r4 = vars0.load(memory_order_acquire);
  }
  else if(sum == 1) {
    v9_r4 = vars1.load(memory_order_acquire);
  }

  int v16 = (v5_r1 == 2);
  atom_1_r1_2.store(v16, memory_order_release);
  int v17 = (v9_r4 == 0);
  atom_1_r4_0.store(v17, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars1, 0);
  atomic_init(&vars0, 0);
  atomic_init(&atom_0_r3_1, 0); 
  atomic_init(&atom_1_r1_2, 0); 
  atomic_init(&atom_1_r4_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v10 = atom_0_r3_1.load(memory_order_acquire);
  int v11 = atom_1_r1_2.load(memory_order_acquire);
  int v12 = atom_1_r4_0.load(memory_order_acquire);
  int v13_conj = v11 & v12;
  int v14_conj = v10 & v13_conj;
  if (v14_conj == 1) MODEL_ASSERT(0);
  return 0;
}