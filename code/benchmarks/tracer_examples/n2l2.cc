/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[2]; 
atomic<int> atom_0_r1_1; 
atomic<int> atom_0_r2_0; 
atomic<int> atom_0_r1_2; 
atomic<int> atom_1_r1_2; 
atomic<int> atom_1_r2_0; 
atomic<int> atom_1_r1_1; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  int v2_r1 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v3_r9 = v2_r1 ^ v2_r1;
  int v6_r2 = atomic_load_explicit(__LINE__, &vars[1+v3_r9], memory_order_acquire);
  int v28 = (v2_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_0_r1_1, v28, memory_order_release);
  int v29 = (v6_r2 == 0);
  atomic_store_explicit(__LINE__, &atom_0_r2_0, v29, memory_order_release);
  int v30 = (v2_r1 == 2);
  atomic_store_explicit(__LINE__, &atom_0_r1_2, v30, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v8_r1 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v9_r9 = v8_r1 ^ v8_r1;
  int v12_r2 = atomic_load_explicit(__LINE__, &vars[1+v9_r9], memory_order_acquire);
  int v31 = (v8_r1 == 2);
  atomic_store_explicit(__LINE__, &atom_1_r1_2, v31, memory_order_release);
  int v32 = (v12_r2 == 0);
  atomic_store_explicit(__LINE__, &atom_1_r2_0, v32, memory_order_release);
  int v33 = (v8_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r1_1, v33, memory_order_release);

}

void t2(void *arg){
label_3:;
  atomic_store_explicit(__LINE__, &vars[1], 2, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__LINE__, &vars[0], 2, memory_order_release);

}

void t3(void *arg){
label_4:;
  atomic_store_explicit(__LINE__, &vars[1], 1, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 
  thrd_t thr3; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_0_r1_1, 0); 
  atomic_init(&atom_0_r2_0, 0); 
  atomic_init(&atom_0_r1_2, 0); 
  atomic_init(&atom_1_r1_2, 0); 
  atomic_init(&atom_1_r2_0, 0); 
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);
  thrd_create(&thr3, t3, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);
  thrd_join(thr3);

  int v13 = atomic_load_explicit(__LINE__, &atom_0_r1_1, memory_order_acquire);
  int v14 = atomic_load_explicit(__LINE__, &atom_0_r2_0, memory_order_acquire);
  int v15_conj = v13 & v14;
  int v16 = atomic_load_explicit(__LINE__, &atom_0_r1_2, memory_order_acquire);
  int v17 = atomic_load_explicit(__LINE__, &atom_0_r2_0, memory_order_acquire);
  int v18_conj = v16 & v17;
  int v19 = atomic_load_explicit(__LINE__, &atom_1_r1_2, memory_order_acquire);
  int v20 = atomic_load_explicit(__LINE__, &atom_1_r2_0, memory_order_acquire);
  int v21_conj = v19 & v20;
  int v22 = atomic_load_explicit(__LINE__, &atom_1_r1_1, memory_order_acquire);
  int v23 = atomic_load_explicit(__LINE__, &atom_1_r2_0, memory_order_acquire);
  int v24_conj = v22 & v23;
  int v25_disj = v21_conj | v24_conj;
  int v26_disj = v18_conj | v25_disj;
  int v27_disj = v15_conj | v26_disj;
  if (v27_disj == 1) MODEL_ASSERT(0);
  return 0;
}
