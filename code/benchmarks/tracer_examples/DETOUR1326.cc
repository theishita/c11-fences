/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[2]; 
atomic<int> atom_0_r5_2; 
atomic<int> atom_1_r3_2; 
atomic<int> atom_1_r5_3; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, &vars[0], 2, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__LINE__, &vars[1], 1, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v2_r5 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v15 = (v2_r5 == 2);
  atomic_store_explicit(__LINE__, &atom_0_r5_2, v15, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(__LINE__, &vars[1], 2, memory_order_release);
  int v4_r3 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  atomic_store_explicit(__LINE__, &vars[1], 3, memory_order_release);
  int v6_r5 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);
  int v16 = (v4_r3 == 2);
  atomic_store_explicit(__LINE__, &atom_1_r3_2, v16, memory_order_release);
  int v17 = (v6_r5 == 3);
  atomic_store_explicit(__LINE__, &atom_1_r5_3, v17, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[0], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&atom_0_r5_2, 0); 
  atomic_init(&atom_1_r3_2, 0); 
  atomic_init(&atom_1_r5_3, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v7 = atomic_load_explicit(__LINE__, &atom_0_r5_2, memory_order_acquire);
  int v8 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v9 = (v8 == 2);
  int v10 = atomic_load_explicit(__LINE__, &atom_1_r3_2, memory_order_acquire);
  int v11 = atomic_load_explicit(__LINE__, &atom_1_r5_3, memory_order_acquire);
  int v12_conj = v10 & v11;
  int v13_conj = v9 & v12_conj;
  int v14_conj = v7 & v13_conj;
  if (v14_conj == 1) MODEL_ASSERT(0);
  return 0;
}
