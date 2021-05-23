/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[2]; 
atomic<int> atom_1_r1_2; 
atomic<int> atom_1_r2_0; 
atomic<int> atom_1_r2_2; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__LINE__, &vars[1], 1, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v2_r1 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v3_r9 = v2_r1 ^ v2_r1;
  int v6_r2 = atomic_load_explicit(__LINE__, &vars[0+v3_r9], memory_order_acquire);
  int v36 = (v2_r1 == 2);
  atomic_store_explicit(__LINE__, &atom_1_r1_2, v36, memory_order_release);
  int v37 = (v6_r2 == 0);
  atomic_store_explicit(__LINE__, &atom_1_r2_0, v37, memory_order_release);
  int v38 = (v6_r2 == 2);
  atomic_store_explicit(__LINE__, &atom_1_r2_2, v38, memory_order_release);

}

void t2(void *arg){
label_3:;
  atomic_store_explicit(__LINE__, &vars[0], 2, memory_order_release);

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

  atomic_init(&vars[0], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&atom_1_r1_2, 0); 
  atomic_init(&atom_1_r2_0, 0); 
  atomic_init(&atom_1_r2_2, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);
  thrd_create(&thr3, t3, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);
  thrd_join(thr3);

  int v7 = atomic_load_explicit(__LINE__, &atom_1_r1_2, memory_order_acquire);
  int v8 = atomic_load_explicit(__LINE__, &atom_1_r2_0, memory_order_acquire);
  int v9 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v10 = (v9 == 1);
  int v11 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v12 = (v11 == 2);
  int v13_conj = v10 & v12;
  int v14_conj = v8 & v13_conj;
  int v15_conj = v7 & v14_conj;
  int v16 = atomic_load_explicit(__LINE__, &atom_1_r1_2, memory_order_acquire);
  int v17 = atomic_load_explicit(__LINE__, &atom_1_r2_0, memory_order_acquire);
  int v18 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v19 = (v18 == 2);
  int v20 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v21 = (v20 == 2);
  int v22_conj = v19 & v21;
  int v23_conj = v17 & v22_conj;
  int v24_conj = v16 & v23_conj;
  int v25 = atomic_load_explicit(__LINE__, &atom_1_r1_2, memory_order_acquire);
  int v26 = atomic_load_explicit(__LINE__, &atom_1_r2_2, memory_order_acquire);
  int v27 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v28 = (v27 == 1);
  int v29 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v30 = (v29 == 2);
  int v31_conj = v28 & v30;
  int v32_conj = v26 & v31_conj;
  int v33_conj = v25 & v32_conj;
  int v34_disj = v24_conj | v33_conj;
  int v35_disj = v15_conj | v34_disj;
  if (v35_disj == 1) MODEL_ASSERT(0);
  return 0;
}
