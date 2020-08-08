// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/MOREDETOUR0140.litmus

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> vars0;
atomic<int> vars1;
atomic<int> atom_0_r1_1; 
atomic<int> atom_1_r1_1; 
atomic<int> atom_2_r1_1; 

atomic<int> __fence_var;

static void t0(void *arg){
label_1:;
  int v2_r1 = vars0.load(memory_order_acquire);
  int v3_r3 = v2_r1 ^ v2_r1;
  int v4_r3 = v3_r3 + 1;
  vars1.store(v4_r3, memory_order_release);
  int v20 = (v2_r1 == 1);
  atom_0_r1_1.store(v20, memory_order_release);

}

static void t1(void *arg){
label_2:;
  int v6_r1 = vars1.load(memory_order_acquire);
  vars1.store(3, memory_order_release);
  int v8_r4 = vars1.load(memory_order_acquire);
  int v9_cmpeq = (v8_r4 == v8_r4);
  if (v9_cmpeq)  goto lbl_LC00; else goto lbl_LC00;
lbl_LC00:;
  vars0.store(1, memory_order_release);
  int v21 = (v6_r1 == 1);
  atom_1_r1_1.store(v21, memory_order_release);

}

static void t2(void *arg){
label_3:;
  int v11_r1 = vars1.load(memory_order_acquire);
  vars1.store(2, memory_order_release);
  int v22 = (v11_r1 == 1);
  atom_2_r1_1.store(v22, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars1, 0);
  atomic_init(&vars0, 0);
  atomic_init(&atom_0_r1_1, 0); 
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_2_r1_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v12 = atom_0_r1_1.load(memory_order_acquire);
  int v13 = atom_1_r1_1.load(memory_order_acquire);
  int v14 = vars1.load(memory_order_acquire);
  int v15 = (v14 == 3);
  int v16 = atom_2_r1_1.load(memory_order_acquire);
  int v17_conj = v15 & v16;
  int v18_conj = v13 & v17_conj;
  int v19_conj = v12 & v18_conj;
  if (v19_conj == 1) MODEL_ASSERT(0);
  return 0;
}