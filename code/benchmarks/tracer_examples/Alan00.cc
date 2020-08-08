// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/Alan00.litmus

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> vars0;
atomic<int> vars1;
atomic<int> vars2;
atomic<int> vars3;
atomic<int> vars4; 
atomic<int> atom_2_r6_2; 
atomic<int> atom_2_r7_0; 
atomic<int> atom_2_r8_1; 
atomic<int> atom_3_r6_1; 
atomic<int> atom_3_r7_0; 
atomic<int> atom_3_r8_1; 
atomic<int> __fence_var;

static void t0(void *arg){
label_1:;
  vars0.store(1, memory_order_release);
  atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
  vars1.store(1, memory_order_release);
  atomic_fetch_add_explicit(&__fence_var, 0, memory_order_acq_rel);
  vars2.store(1, memory_order_release);

}

static void t1(void *arg){
label_2:;
  vars3.store(2, memory_order_release);

}

static void t2(void *arg){
label_3:;
  int v2_r6 = vars3.load(memory_order_acquire);
  int v3_r7 = v2_r6 ^ v2_r6;

  int sum = 0+v3_r7;
  int v6_r7;
  if(sum == 0) {
    v6_r7 = vars0.load(memory_order_acquire);
  }
  else if(sum == 1) {
    v6_r7 = vars1.load(memory_order_acquire);
  }
  else if(sum == 2) {
    v6_r7 = vars2.load(memory_order_acquire);
  }
  else if(sum == 3) {
    v6_r7 = vars3.load(memory_order_acquire);
  }
  else if(sum == 4) {
    v6_r7 = vars4.load(memory_order_acquire);
  }

  int v8_r8 = vars1.load(memory_order_acquire);
  int v9_r9 = v8_r8 ^ v8_r8;

  sum = 4+v9_r9;
  if(sum == 0) {
    vars0.store(1, memory_order_release);
  }
  else if(sum == 1) {
    vars1.store(1, memory_order_release);
  }
  else if(sum == 2) {
    vars2.store(1, memory_order_release);
  }
  else if(sum == 3) {
    vars3.store(1, memory_order_release);
  }
  else if(sum == 4) {
    vars4.store(1, memory_order_release);
  }

  int v33 = (v2_r6 == 2);
  atom_2_r6_2.store(v33, memory_order_release);
  int v34 = (v6_r7 == 0);
  atom_2_r7_0.store(v34, memory_order_release);
  int v35 = (v8_r8 == 1);
  atom_2_r8_1.store(v35, memory_order_release);

}

static void t3(void *arg){
label_4:;
  int v11_r6 = vars4.load(memory_order_acquire);
  int v12_r7 = v11_r6 ^ v11_r6;

  int sum = 1+v12_r7;
  int v15_r7;
  if(sum == 0) {
    v15_r7 = vars0.load(memory_order_acquire);
  }
  else if(sum == 1) {
    v15_r7 = vars1.load(memory_order_acquire);
  }
  else if(sum == 2) {
    v15_r7 = vars2.load(memory_order_acquire);
  }
  else if(sum == 3) {
    v15_r7 = vars3.load(memory_order_acquire);
  }
  else if(sum == 4) {
    v15_r7 = vars4.load(memory_order_acquire);
  }

  int v17_r8 = vars2.load(memory_order_acquire);
  int v18_r9 = v17_r8 ^ v17_r8;

  sum = 3+v18_r9;
  if(sum == 0) {
    vars0.store(1, memory_order_release);
  }
  else if(sum == 1) {
    vars1.store(1, memory_order_release);
  }
  else if(sum == 2) {
    vars2.store(1, memory_order_release);
  }
  else if(sum == 3) {
    vars3.store(1, memory_order_release);
  }
  else if(sum == 4) {
    vars4.store(1, memory_order_release);
  }

  int v36 = (v11_r6 == 1);
  atom_3_r6_1.store(v36, memory_order_release);
  int v37 = (v15_r7 == 0);
  atom_3_r7_0.store(v37, memory_order_release);
  int v38 = (v17_r8 == 1);
  atom_3_r8_1.store(v38, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 
  thrd_t thr3; 

  atomic_init(&vars4, 0);
  atomic_init(&vars3, 0);
  atomic_init(&vars2, 0);
  atomic_init(&vars1, 0);
  atomic_init(&vars0, 0);
  atomic_init(&atom_2_r6_2, 0); 
  atomic_init(&atom_2_r7_0, 0); 
  atomic_init(&atom_2_r8_1, 0); 
  atomic_init(&atom_3_r6_1, 0); 
  atomic_init(&atom_3_r7_0, 0); 
  atomic_init(&atom_3_r8_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);
  thrd_create(&thr3, t3, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);
  thrd_join(thr3);

  int v19 = vars3.load(memory_order_acquire);
  int v20 = (v19 == 2);
  int v21 = atom_2_r6_2.load(memory_order_acquire);
  int v22 = atom_2_r7_0.load(memory_order_acquire);
  int v23 = atom_2_r8_1.load(memory_order_acquire);
  int v24 = atom_3_r6_1.load(memory_order_acquire);
  int v25 = atom_3_r7_0.load(memory_order_acquire);
  int v26 = atom_3_r8_1.load(memory_order_acquire);
  int v27_conj = v25 & v26;
  int v28_conj = v24 & v27_conj;
  int v29_conj = v23 & v28_conj;
  int v30_conj = v22 & v29_conj;
  int v31_conj = v21 & v30_conj;
  int v32_conj = v20 & v31_conj;
  if (v32_conj == 1) MODEL_ASSERT(0);
  return 0;
}