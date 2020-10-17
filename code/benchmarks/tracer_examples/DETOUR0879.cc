/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[2]; 
atomic<int> atom_1_r6_2; 
atomic<int> atom_1_r7_2; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, &vars[0], 3, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__LINE__, &vars[1], 1, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(__LINE__, &vars[1], 2, memory_order_release);
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);
  atomic_store_explicit(__LINE__, &vars[0], 2, memory_order_release);
  int v2_r6 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v4_r7 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v14 = (v2_r6 == 2);
  atomic_store_explicit(__LINE__, &atom_1_r6_2, v14, memory_order_release);
  int v15 = (v4_r7 == 2);
  atomic_store_explicit(__LINE__, &atom_1_r7_2, v15, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_1_r6_2, 0); 
  atomic_init(&atom_1_r7_2, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v5 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v6 = (v5 == 2);
  int v7 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v8 = (v7 == 3);
  int v9 = atomic_load_explicit(__LINE__, &atom_1_r6_2, memory_order_acquire);
  int v10 = atomic_load_explicit(__LINE__, &atom_1_r7_2, memory_order_acquire);
  int v11_conj = v9 & v10;
  int v12_conj = v8 & v11_conj;
  int v13_conj = v6 & v12_conj;
  if (v13_conj == 1) MODEL_ASSERT(0);
  return 0;
}
