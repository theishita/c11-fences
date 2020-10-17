/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[3]; 
atomic<int> atom_0_r5_2; 
atomic<int> atom_1_r3_3; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, &vars[0], 2, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__LINE__, &vars[1], 1, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v2_r5 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v11 = (v2_r5 == 2);
  atomic_store_explicit(__LINE__, &atom_0_r5_2, v11, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(__LINE__, &vars[1], 2, memory_order_release);
  int v4_r3 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  atomic_store_explicit(__LINE__, &vars[2], 1, memory_order_release);
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);
  int v12 = (v4_r3 == 3);
  atomic_store_explicit(__LINE__, &atom_1_r3_3, v12, memory_order_release);

}

void t2(void *arg){
label_3:;
  atomic_store_explicit(__LINE__, &vars[1], 3, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&vars[2], 0);
  atomic_init(&atom_0_r5_2, 0); 
  atomic_init(&atom_1_r3_3, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v5 = atomic_load_explicit(__LINE__, &atom_0_r5_2, memory_order_acquire);
  int v6 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v7 = (v6 == 2);
  int v8 = atomic_load_explicit(__LINE__, &atom_1_r3_3, memory_order_acquire);
  int v9_conj = v7 & v8;
  int v10_conj = v5 & v9_conj;
  if (v10_conj == 1) MODEL_ASSERT(0);
  return 0;
}
