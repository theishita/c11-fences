/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[1]; 
atomic<int> atom_2_r1_1; 
atomic<int> atom_2_r2_2; 
atomic<int> atom_2_r1_2; 
atomic<int> atom_2_r2_1; 
atomic<int> atom_2_r2_0; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(__LINE__, &vars[0], 2, memory_order_release);

}

void t2(void *arg){
label_3:;
  int v2_r1 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v4_r2 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v24 = (v2_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_2_r1_1, v24, memory_order_release);
  int v25 = (v4_r2 == 2);
  atomic_store_explicit(__LINE__, &atom_2_r2_2, v25, memory_order_release);
  int v26 = (v2_r1 == 2);
  atomic_store_explicit(__LINE__, &atom_2_r1_2, v26, memory_order_release);
  int v27 = (v4_r2 == 1);
  atomic_store_explicit(__LINE__, &atom_2_r2_1, v27, memory_order_release);
  int v28 = (v4_r2 == 0);
  atomic_store_explicit(__LINE__, &atom_2_r2_0, v28, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars[0], 0);
  atomic_init(&atom_2_r1_1, 0); 
  atomic_init(&atom_2_r2_2, 0); 
  atomic_init(&atom_2_r1_2, 0); 
  atomic_init(&atom_2_r2_1, 0); 
  atomic_init(&atom_2_r2_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v5 = atomic_load_explicit(__LINE__, &atom_2_r1_1, memory_order_acquire);
  int v6 = atomic_load_explicit(__LINE__, &atom_2_r2_2, memory_order_acquire);
  int v7 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v8 = (v7 == 1);
  int v9_conj = v6 & v8;
  int v10_conj = v5 & v9_conj;
  int v11 = atomic_load_explicit(__LINE__, &atom_2_r1_2, memory_order_acquire);
  int v12 = atomic_load_explicit(__LINE__, &atom_2_r2_1, memory_order_acquire);
  int v13 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v14 = (v13 == 2);
  int v15_conj = v12 & v14;
  int v16_conj = v11 & v15_conj;
  int v17 = atomic_load_explicit(__LINE__, &atom_2_r1_1, memory_order_acquire);
  int v18 = atomic_load_explicit(__LINE__, &atom_2_r1_2, memory_order_acquire);
  int v19_disj = v17 | v18;
  int v20 = atomic_load_explicit(__LINE__, &atom_2_r2_0, memory_order_acquire);
  int v21_conj = v19_disj & v20;
  int v22_disj = v16_conj | v21_conj;
  int v23_disj = v10_conj | v22_disj;
  if (v23_disj == 1) MODEL_ASSERT(0);
  return 0;
}
