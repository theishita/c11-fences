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
atomic<int> atom_2_r1_2; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, &vars[0], 2, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__LINE__, &vars[1], 1, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v2_r5 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v18 = (v2_r5 == 2);
  atomic_store_explicit(__LINE__, &atom_0_r5_2, v18, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(__LINE__, &vars[1], 2, memory_order_release);
  int v4_r3 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  atomic_store_explicit(__LINE__, &vars[1], 4, memory_order_release);
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);
  int v19 = (v4_r3 == 2);
  atomic_store_explicit(__LINE__, &atom_1_r3_2, v19, memory_order_release);

}

void t2(void *arg){
label_3:;
  int v6_r1 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  atomic_store_explicit(__LINE__, &vars[1], 3, memory_order_release);
  int v20 = (v6_r1 == 2);
  atomic_store_explicit(__LINE__, &atom_2_r1_2, v20, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_0_r5_2, 0); 
  atomic_init(&atom_1_r3_2, 0); 
  atomic_init(&atom_2_r1_2, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v7 = atomic_load_explicit(__LINE__, &atom_0_r5_2, memory_order_acquire);
  int v8 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v9 = (v8 == 2);
  int v10 = atomic_load_explicit(__LINE__, &atom_1_r3_2, memory_order_acquire);
  int v11 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v12 = (v11 == 4);
  int v13 = atomic_load_explicit(__LINE__, &atom_2_r1_2, memory_order_acquire);
  int v14_conj = v12 & v13;
  int v15_conj = v10 & v14_conj;
  int v16_conj = v9 & v15_conj;
  int v17_conj = v7 & v16_conj;
  if (v17_conj == 1) MODEL_ASSERT(0);
  return 0;
}
