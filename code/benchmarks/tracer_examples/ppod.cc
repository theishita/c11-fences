/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[2]; 
atomic<int> atom_0_r1_2; 
atomic<int> atom_0_r2_0; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  int v2_r1 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  atomic_store_explicit(__LINE__, &vars[0], 3, memory_order_release);
  int v4_r4 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v5_r10 = v4_r4 ^ v4_r4;
  int v8_r2 = atomic_load_explicit(__LINE__, &vars[1+v5_r10], memory_order_acquire);
  int v12 = (v2_r1 == 2);
  atomic_store_explicit(__LINE__, &atom_0_r1_2, v12, memory_order_release);
  int v13 = (v8_r2 == 0);
  atomic_store_explicit(__LINE__, &atom_0_r2_0, v13, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(__LINE__, &vars[1], 1, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__LINE__, &vars[0], 2, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_0_r1_2, 0); 
  atomic_init(&atom_0_r2_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v9 = atomic_load_explicit(__LINE__, &atom_0_r1_2, memory_order_acquire);
  int v10 = atomic_load_explicit(__LINE__, &atom_0_r2_0, memory_order_acquire);
  int v11_conj = v9 & v10;
  if (v11_conj == 1) MODEL_ASSERT(0);
  return 0;
}
