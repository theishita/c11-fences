/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[2]; 
atomic<int> atom_0_r1_0; 
atomic<int> atom_0_r2_0; 
atomic<int> atom_1_r1_0; 
atomic<int> atom_1_r2_2; 
atomic<int> atom_0_r2_2; 
atomic<int> atom_0_r1_1; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  int v2_r1 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v3_r3 = v2_r1 ^ v2_r1;
  int v6_r2 = atomic_load_explicit(__LINE__, &vars[1+v3_r3], memory_order_acquire);
  int v47 = (v2_r1 == 0);
  atomic_store_explicit(__LINE__, &atom_0_r1_0, v47, memory_order_release);
  int v48 = (v6_r2 == 0);
  atomic_store_explicit(__LINE__, &atom_0_r2_0, v48, memory_order_release);
  int v51 = (v6_r2 == 2);
  atomic_store_explicit(__LINE__, &atom_0_r2_2, v51, memory_order_release);
  int v52 = (v2_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_0_r1_1, v52, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v8_r2 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v9_r3 = v8_r2 ^ v8_r2;
  int v12_r1 = atomic_load_explicit(__LINE__, &vars[0+v9_r3], memory_order_acquire);
  int v49 = (v12_r1 == 0);
  atomic_store_explicit(__LINE__, &atom_1_r1_0, v49, memory_order_release);
  int v50 = (v8_r2 == 2);
  atomic_store_explicit(__LINE__, &atom_1_r2_2, v50, memory_order_release);

}

void t2(void *arg){
label_3:;
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);
  int v14_r1 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v15_r3 = v14_r1 ^ v14_r1;
  atomic_store_explicit(__LINE__, &vars[1+v15_r3], 2, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars[0], 0);
  atomic_init(&vars[1], 0);
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

  int v16 = atomic_load_explicit(__LINE__, &atom_0_r1_0, memory_order_acquire);
  int v17 = atomic_load_explicit(__LINE__, &atom_0_r2_0, memory_order_acquire);
  int v18 = atomic_load_explicit(__LINE__, &atom_1_r1_0, memory_order_acquire);
  int v19 = atomic_load_explicit(__LINE__, &atom_1_r2_2, memory_order_acquire);
  int v20_conj = v18 & v19;
  int v21_conj = v17 & v20_conj;
  int v22_conj = v16 & v21_conj;
  int v23 = atomic_load_explicit(__LINE__, &atom_0_r1_0, memory_order_acquire);
  int v24 = atomic_load_explicit(__LINE__, &atom_0_r2_2, memory_order_acquire);
  int v25 = atomic_load_explicit(__LINE__, &atom_1_r1_0, memory_order_acquire);
  int v26 = atomic_load_explicit(__LINE__, &atom_1_r2_2, memory_order_acquire);
  int v27_conj = v25 & v26;
  int v28_conj = v24 & v27_conj;
  int v29_conj = v23 & v28_conj;
  int v30 = atomic_load_explicit(__LINE__, &atom_0_r1_1, memory_order_acquire);
  int v31 = atomic_load_explicit(__LINE__, &atom_0_r2_0, memory_order_acquire);
  int v32 = atomic_load_explicit(__LINE__, &atom_1_r1_0, memory_order_acquire);
  int v33 = atomic_load_explicit(__LINE__, &atom_1_r2_2, memory_order_acquire);
  int v34_conj = v32 & v33;
  int v35_conj = v31 & v34_conj;
  int v36_conj = v30 & v35_conj;
  int v37 = atomic_load_explicit(__LINE__, &atom_0_r1_1, memory_order_acquire);
  int v38 = atomic_load_explicit(__LINE__, &atom_0_r2_2, memory_order_acquire);
  int v39 = atomic_load_explicit(__LINE__, &atom_1_r1_0, memory_order_acquire);
  int v40 = atomic_load_explicit(__LINE__, &atom_1_r2_2, memory_order_acquire);
  int v41_conj = v39 & v40;
  int v42_conj = v38 & v41_conj;
  int v43_conj = v37 & v42_conj;
  int v44_disj = v36_conj | v43_conj;
  int v45_disj = v29_conj | v44_disj;
  int v46_disj = v22_conj | v45_disj;
  if (v46_disj == 1) MODEL_ASSERT(0);
  return 0;
}
