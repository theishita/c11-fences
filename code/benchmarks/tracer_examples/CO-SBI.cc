/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[1]; 
atomic<int> atom_0_r2_2; 
atomic<int> atom_0_r1_2; 
atomic<int> atom_1_r2_1; 
atomic<int> atom_1_r1_1; 
atomic<int> atom_0_r2_1; 
atomic<int> atom_1_r2_2; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);
  int v2_r1 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v4_r2 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v24 = (v4_r2 == 2);
  atomic_store_explicit(__LINE__, &atom_0_r2_2, v24, memory_order_release);
  int v25 = (v2_r1 == 2);
  atomic_store_explicit(__LINE__, &atom_0_r1_2, v25, memory_order_release);
  int v28 = (v4_r2 == 1);
  atomic_store_explicit(__LINE__, &atom_0_r2_1, v28, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(__LINE__, &vars[0], 2, memory_order_release);
  int v6_r1 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v8_r2 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v26 = (v8_r2 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r2_1, v26, memory_order_release);
  int v27 = (v6_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r1_1, v27, memory_order_release);
  int v29 = (v8_r2 == 2);
  atomic_store_explicit(__LINE__, &atom_1_r2_2, v29, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[0], 0);
  atomic_init(&atom_0_r2_2, 0); 
  atomic_init(&atom_0_r1_2, 0); 
  atomic_init(&atom_1_r2_1, 0); 
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_0_r2_1, 0); 
  atomic_init(&atom_1_r2_2, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v9 = atomic_load_explicit(__LINE__, &atom_0_r2_2, memory_order_acquire);
  int v10 = atomic_load_explicit(__LINE__, &atom_0_r1_2, memory_order_acquire);
  int v11_disj = v9 | v10;
  int v12 = atomic_load_explicit(__LINE__, &atom_1_r2_1, memory_order_acquire);
  int v13 = atomic_load_explicit(__LINE__, &atom_1_r1_1, memory_order_acquire);
  int v14_disj = v12 | v13;
  int v15_conj = v11_disj & v14_disj;
  int v16 = atomic_load_explicit(__LINE__, &atom_0_r1_2, memory_order_acquire);
  int v17 = atomic_load_explicit(__LINE__, &atom_0_r2_1, memory_order_acquire);
  int v18_conj = v16 & v17;
  int v19 = atomic_load_explicit(__LINE__, &atom_1_r1_1, memory_order_acquire);
  int v20 = atomic_load_explicit(__LINE__, &atom_1_r2_2, memory_order_acquire);
  int v21_conj = v19 & v20;
  int v22_disj = v18_conj | v21_conj;
  int v23_disj = v15_conj | v22_disj;
  if (v23_disj == 1) MODEL_ASSERT(0);
  return 0;
}
