// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/iriwv3.litmus

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> vars0;
atomic<int> vars1;
atomic<int> atom_0_r1_0; 
atomic<int> atom_0_r2_0; 
atomic<int> atom_1_r1_0; 
atomic<int> atom_1_r2_2; 
atomic<int> atom_0_r2_2; 
atomic<int> atom_0_r1_1; 
atomic<int> __fence_var;

static void t0(void *arg){
label_1:;
  int v2_r1 = vars0.load(__LINE__, memory_order_acquire);
  int v3_r3 = v2_r1 ^ v2_r1;

  int sum = 1+v3_r3;
  int v6_r2;
  if(sum == 0) {
    v6_r2 = vars0.load(__LINE__, memory_order_acquire);
  }
  else if(sum == 1) {
    v6_r2 = vars1.load(__LINE__, memory_order_acquire);
  }

  int v44 = (v2_r1 == 0);
  atom_0_r1_0.store(__LINE__, v44, memory_order_release);
  int v45 = (v6_r2 == 0);
  atom_0_r2_0.store(__LINE__, v45, memory_order_release);
  int v48 = (v6_r2 == 2);
  atom_0_r2_2.store(__LINE__, v48, memory_order_release);
  int v49 = (v2_r1 == 1);
  atom_0_r1_1.store(__LINE__, v49, memory_order_release);

}

static void t1(void *arg){
label_2:;
  int v8_r2 = vars1.load(__LINE__, memory_order_acquire);
  int v9_r3 = v8_r2 ^ v8_r2;

  int sum = 0+v9_r3;
  int v12_r1;
  if(sum == 0) {
    v12_r1 = vars0.load(__LINE__, memory_order_acquire);
  }
  else if(sum == 1) {
    v12_r1 = vars1.load(__LINE__, memory_order_acquire);
  }

  int v46 = (v12_r1 == 0);
  atom_1_r1_0.store(__LINE__, v46, memory_order_release);
  int v47 = (v8_r2 == 2);
  atom_1_r2_2.store(__LINE__, v47, memory_order_release);

}

static void t2(void *arg){
label_3:;
  vars0.store(__LINE__, 1, memory_order_release);
  vars1.store(__LINE__, 2, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars1, 0);
  atomic_init(&vars0, 0);
  atomic_init(&atom_0_r1_0, 0); 
  atomic_init(&atom_0_r2_0, 0); 
  atomic_init(&atom_1_r1_0, 0); 
  atomic_init(&atom_1_r2_2, 0); 
  atomic_init(&atom_0_r2_2, 0); 
  atomic_init(&atom_0_r1_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v13 = atom_0_r1_0.load(__LINE__, memory_order_acquire);
  int v14 = atom_0_r2_0.load(__LINE__, memory_order_acquire);
  int v15 = atom_1_r1_0.load(__LINE__, memory_order_acquire);
  int v16 = atom_1_r2_2.load(__LINE__, memory_order_acquire);
  int v17_conj = v15 & v16;
  int v18_conj = v14 & v17_conj;
  int v19_conj = v13 & v18_conj;
  int v20 = atom_0_r1_0.load(__LINE__, memory_order_acquire);
  int v21 = atom_0_r2_2.load(__LINE__, memory_order_acquire);
  int v22 = atom_1_r1_0.load(__LINE__, memory_order_acquire);
  int v23 = atom_1_r2_2.load(__LINE__, memory_order_acquire);
  int v24_conj = v22 & v23;
  int v25_conj = v21 & v24_conj;
  int v26_conj = v20 & v25_conj;
  int v27 = atom_0_r1_1.load(__LINE__, memory_order_acquire);
  int v28 = atom_0_r2_0.load(__LINE__, memory_order_acquire);
  int v29 = atom_1_r1_0.load(__LINE__, memory_order_acquire);
  int v30 = atom_1_r2_2.load(__LINE__, memory_order_acquire);
  int v31_conj = v29 & v30;
  int v32_conj = v28 & v31_conj;
  int v33_conj = v27 & v32_conj;
  int v34 = atom_0_r1_1.load(__LINE__, memory_order_acquire);
  int v35 = atom_0_r2_2.load(__LINE__, memory_order_acquire);
  int v36 = atom_1_r1_0.load(__LINE__, memory_order_acquire);
  int v37 = atom_1_r2_2.load(__LINE__, memory_order_acquire);
  int v38_conj = v36 & v37;
  int v39_conj = v35 & v38_conj;
  int v40_conj = v34 & v39_conj;
  int v41_disj = v33_conj | v40_conj;
  int v42_disj = v26_conj | v41_disj;
  int v43_disj = v19_conj | v42_disj;
  if (v43_disj == 1) MODEL_ASSERT(0);
  return 0;
}