/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[2]; 
atomic<int> atom_1_r1_2; 
atomic<int> atom_2_r1_1; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, __LINE__, &vars[0], 2, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v2_r1 = atomic_load_explicit(__LINE__, __LINE__, &vars[0], memory_order_acquire);
  atomic_fetch_add_explicit(__LINE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__LINE__, __LINE__, &vars[1], 1, memory_order_release);
  int v11 = (v2_r1 == 2);
  atomic_store_explicit(__LINE__, __LINE__, &atom_1_r1_2, v11, memory_order_release);

}

void t2(void *arg){
label_3:;
  int v4_r1 = atomic_load_explicit(__LINE__, __LINE__, &vars[1], memory_order_acquire);
  atomic_store_explicit(__LINE__, __LINE__, &vars[0], 1, memory_order_release);
  int v12 = (v4_r1 == 1);
  atomic_store_explicit(__LINE__, __LINE__, &atom_2_r1_1, v12, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_1_r1_2, 0); 
  atomic_init(&atom_2_r1_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v5 = atomic_load_explicit(__LINE__, __LINE__, &vars[0], memory_order_acquire);
  int v6 = (v5 == 2);
  int v7 = atomic_load_explicit(__LINE__, __LINE__, &atom_1_r1_2, memory_order_acquire);
  int v8 = atomic_load_explicit(__LINE__, __LINE__, &atom_2_r1_1, memory_order_acquire);
  int v9_conj = v7 & v8;
  int v10_conj = v6 & v9_conj;
  if (v10_conj == 1) MODEL_ASSERT(0);
  return 0;
}
