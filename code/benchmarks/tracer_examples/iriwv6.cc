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
  int v4_r2 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v40 = (v2_r1 == 0);
  atomic_store_explicit(__LINE__, &atom_0_r1_0, v40, memory_order_release);
  int v41 = (v4_r2 == 0);
  atomic_store_explicit(__LINE__, &atom_0_r2_0, v41, memory_order_release);
  int v44 = (v4_r2 == 2);
  atomic_store_explicit(__LINE__, &atom_0_r2_2, v44, memory_order_release);
  int v45 = (v2_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_0_r1_1, v45, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v6_r2 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v8_r1 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v42 = (v8_r1 == 0);
  atomic_store_explicit(__LINE__, &atom_1_r1_0, v42, memory_order_release);
  int v43 = (v6_r2 == 2);
  atomic_store_explicit(__LINE__, &atom_1_r2_2, v43, memory_order_release);

}

void t2(void *arg){
label_3:;
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);
  atomic_store_explicit(__LINE__, &vars[1], 2, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
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

  int v9 = atomic_load_explicit(__LINE__, &atom_0_r1_0, memory_order_acquire);
  int v10 = atomic_load_explicit(__LINE__, &atom_0_r2_0, memory_order_acquire);
  int v11 = atomic_load_explicit(__LINE__, &atom_1_r1_0, memory_order_acquire);
  int v12 = atomic_load_explicit(__LINE__, &atom_1_r2_2, memory_order_acquire);
  int v13_conj = v11 & v12;
  int v14_conj = v10 & v13_conj;
  int v15_conj = v9 & v14_conj;
  int v16 = atomic_load_explicit(__LINE__, &atom_0_r1_0, memory_order_acquire);
  int v17 = atomic_load_explicit(__LINE__, &atom_0_r2_2, memory_order_acquire);
  int v18 = atomic_load_explicit(__LINE__, &atom_1_r1_0, memory_order_acquire);
  int v19 = atomic_load_explicit(__LINE__, &atom_1_r2_2, memory_order_acquire);
  int v20_conj = v18 & v19;
  int v21_conj = v17 & v20_conj;
  int v22_conj = v16 & v21_conj;
  int v23 = atomic_load_explicit(__LINE__, &atom_0_r1_1, memory_order_acquire);
  int v24 = atomic_load_explicit(__LINE__, &atom_0_r2_0, memory_order_acquire);
  int v25 = atomic_load_explicit(__LINE__, &atom_1_r1_0, memory_order_acquire);
  int v26 = atomic_load_explicit(__LINE__, &atom_1_r2_2, memory_order_acquire);
  int v27_conj = v25 & v26;
  int v28_conj = v24 & v27_conj;
  int v29_conj = v23 & v28_conj;
  int v30 = atomic_load_explicit(__LINE__, &atom_0_r1_1, memory_order_acquire);
  int v31 = atomic_load_explicit(__LINE__, &atom_0_r2_2, memory_order_acquire);
  int v32 = atomic_load_explicit(__LINE__, &atom_1_r1_0, memory_order_acquire);
  int v33 = atomic_load_explicit(__LINE__, &atom_1_r2_2, memory_order_acquire);
  int v34_conj = v32 & v33;
  int v35_conj = v31 & v34_conj;
  int v36_conj = v30 & v35_conj;
  int v37_disj = v29_conj | v36_conj;
  int v38_disj = v22_conj | v37_disj;
  int v39_disj = v15_conj | v38_disj;
  if (v39_disj == 1) MODEL_ASSERT(0);
  return 0;
}
