/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[2]; 
atomic<int> atom_0_r2_0; 
atomic<int> atom_1_r2_2; 
atomic<int> atom_2_r1_1; 
atomic<int> atom_2_r1_2; 
atomic<int> atom_2_r1_0; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v2_r2 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v33 = (v2_r2 == 0);
  atomic_store_explicit(__LINE__, &atom_0_r2_0, v33, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v4_r2 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__LINE__, &vars[0], 2, memory_order_release);
  int v34 = (v4_r2 == 2);
  atomic_store_explicit(__LINE__, &atom_1_r2_2, v34, memory_order_release);

}

void t2(void *arg){
label_3:;
  int v6_r1 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v35 = (v6_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_2_r1_1, v35, memory_order_release);
  int v36 = (v6_r1 == 2);
  atomic_store_explicit(__LINE__, &atom_2_r1_2, v36, memory_order_release);
  int v37 = (v6_r1 == 0);
  atomic_store_explicit(__LINE__, &atom_2_r1_0, v37, memory_order_release);

}

void t3(void *arg){
label_4:;
  atomic_store_explicit(__LINE__, &vars[1], 2, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 
  thrd_t thr3; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_0_r2_0, 0); 
  atomic_init(&atom_1_r2_2, 0); 
  atomic_init(&atom_2_r1_1, 0); 
  atomic_init(&atom_2_r1_2, 0); 
  atomic_init(&atom_2_r1_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);
  thrd_create(&thr3, t3, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);
  thrd_join(thr3);

  int v7 = atomic_load_explicit(__LINE__, &atom_0_r2_0, memory_order_acquire);
  int v8 = atomic_load_explicit(__LINE__, &atom_1_r2_2, memory_order_acquire);
  int v9 = atomic_load_explicit(__LINE__, &atom_2_r1_1, memory_order_acquire);
  int v10 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v11 = (v10 == 1);
  int v12_conj = v9 & v11;
  int v13_conj = v8 & v12_conj;
  int v14_conj = v7 & v13_conj;
  int v15 = atomic_load_explicit(__LINE__, &atom_0_r2_0, memory_order_acquire);
  int v16 = atomic_load_explicit(__LINE__, &atom_1_r2_2, memory_order_acquire);
  int v17 = atomic_load_explicit(__LINE__, &atom_2_r1_2, memory_order_acquire);
  int v18 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v19 = (v18 == 1);
  int v20_conj = v17 & v19;
  int v21_conj = v16 & v20_conj;
  int v22_conj = v15 & v21_conj;
  int v23 = atomic_load_explicit(__LINE__, &atom_0_r2_0, memory_order_acquire);
  int v24 = atomic_load_explicit(__LINE__, &atom_1_r2_2, memory_order_acquire);
  int v25 = atomic_load_explicit(__LINE__, &atom_2_r1_0, memory_order_acquire);
  int v26 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v27 = (v26 == 1);
  int v28_conj = v25 & v27;
  int v29_conj = v24 & v28_conj;
  int v30_conj = v23 & v29_conj;
  int v31_disj = v22_conj | v30_conj;
  int v32_disj = v14_conj | v31_disj;
  if (v32_disj == 1) MODEL_ASSERT(0);
  return 0;
}
