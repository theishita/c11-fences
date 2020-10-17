/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[1]; 
atomic<int> atom_0_r1_0; 
atomic<int> atom_1_r3_1; 
atomic<int> atom_1_r1_1; 
atomic<int> atom_1_r1_0; 
atomic<int> atom_1_r3_0; 
atomic<int> atom_0_r1_2; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  int v2_r1 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);
  int v32 = (v2_r1 == 0);
  atomic_store_explicit(__LINE__, &atom_0_r1_0, v32, memory_order_release);
  int v37 = (v2_r1 == 2);
  atomic_store_explicit(__LINE__, &atom_0_r1_2, v37, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v4_r1 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v6_r3 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  atomic_store_explicit(__LINE__, &vars[0], 2, memory_order_release);
  int v33 = (v6_r3 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r3_1, v33, memory_order_release);
  int v34 = (v4_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r1_1, v34, memory_order_release);
  int v35 = (v4_r1 == 0);
  atomic_store_explicit(__LINE__, &atom_1_r1_0, v35, memory_order_release);
  int v36 = (v6_r3 == 0);
  atomic_store_explicit(__LINE__, &atom_1_r3_0, v36, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[0], 0);
  atomic_init(&atom_0_r1_0, 0); 
  atomic_init(&atom_1_r3_1, 0); 
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_1_r1_0, 0); 
  atomic_init(&atom_1_r3_0, 0); 
  atomic_init(&atom_0_r1_2, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v7 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v8 = (v7 == 2);
  int v9 = atomic_load_explicit(__LINE__, &atom_0_r1_0, memory_order_acquire);
  int v10 = atomic_load_explicit(__LINE__, &atom_1_r3_1, memory_order_acquire);
  int v11 = atomic_load_explicit(__LINE__, &atom_1_r1_1, memory_order_acquire);
  int v12 = atomic_load_explicit(__LINE__, &atom_1_r1_0, memory_order_acquire);
  int v13_disj = v11 | v12;
  int v14_conj = v10 & v13_disj;
  int v15 = atomic_load_explicit(__LINE__, &atom_1_r3_0, memory_order_acquire);
  int v16 = atomic_load_explicit(__LINE__, &atom_1_r1_0, memory_order_acquire);
  int v17_conj = v15 & v16;
  int v18_disj = v14_conj | v17_conj;
  int v19_conj = v9 & v18_disj;
  int v20_conj = v8 & v19_conj;
  int v21 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v22 = (v21 == 1);
  int v23 = atomic_load_explicit(__LINE__, &atom_1_r3_0, memory_order_acquire);
  int v24 = atomic_load_explicit(__LINE__, &atom_1_r1_0, memory_order_acquire);
  int v25 = atomic_load_explicit(__LINE__, &atom_0_r1_2, memory_order_acquire);
  int v26 = atomic_load_explicit(__LINE__, &atom_0_r1_0, memory_order_acquire);
  int v27_disj = v25 | v26;
  int v28_conj = v24 & v27_disj;
  int v29_conj = v23 & v28_conj;
  int v30_conj = v22 & v29_conj;
  int v31_disj = v20_conj | v30_conj;
  if (v31_disj == 0) MODEL_ASSERT(0);
  return 0;
}
