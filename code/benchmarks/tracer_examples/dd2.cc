/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[2]; 
atomic<int> atom_2_r2_1; 
atomic<int> atom_3_r2_1; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, &vars[0], 2, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(__LINE__, &vars[1], 2, memory_order_release);

}

void t2(void *arg){
label_3:;
  int v2_r2 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);
  int v8 = (v2_r2 == 1);
  atomic_store_explicit(__LINE__, &atom_2_r2_1, v8, memory_order_release);

}

void t3(void *arg){
label_4:;
  int v4_r2 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__LINE__, &vars[1], 1, memory_order_release);
  int v9 = (v4_r2 == 1);
  atomic_store_explicit(__LINE__, &atom_3_r2_1, v9, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 
  thrd_t thr3; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_2_r2_1, 0); 
  atomic_init(&atom_3_r2_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);
  thrd_create(&thr3, t3, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);
  thrd_join(thr3);

  int v5 = atomic_load_explicit(__LINE__, &atom_2_r2_1, memory_order_acquire);
  int v6 = atomic_load_explicit(__LINE__, &atom_3_r2_1, memory_order_acquire);
  int v7_conj = v5 & v6;
  if (v7_conj == 1) MODEL_ASSERT(0);
  return 0;
}
