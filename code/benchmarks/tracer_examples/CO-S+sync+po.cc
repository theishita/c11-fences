/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[1]; 
atomic<int> atom_1_r0_2; 
atomic<int> atom_1_r0_1; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__LINE__, &vars[0], 2, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v2_r0 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  atomic_store_explicit(__LINE__, &vars[0], 3, memory_order_release);
  int v12 = (v2_r0 == 2);
  atomic_store_explicit(__LINE__, &atom_1_r0_2, v12, memory_order_release);
  int v13 = (v2_r0 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r0_1, v13, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[0], 0);
  atomic_init(&atom_1_r0_2, 0); 
  atomic_init(&atom_1_r0_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v3 = atomic_load_explicit(__LINE__, &atom_1_r0_2, memory_order_acquire);
  int v4 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v5 = (v4 == 2);
  int v6 = atomic_load_explicit(__LINE__, &atom_1_r0_1, memory_order_acquire);
  int v7 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v8 = (v7 == 1);
  int v9_conj = v6 & v8;
  int v10_disj = v5 | v9_conj;
  int v11_conj = v3 & v10_disj;
  if (v11_conj == 1) MODEL_ASSERT(0);
  return 0;
}
