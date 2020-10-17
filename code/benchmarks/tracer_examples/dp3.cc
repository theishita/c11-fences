/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[2]; 
atomic<int> atom_0_r1_0; 
atomic<int> atom_1_r1_0; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  int v2_r1 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v3_r9 = v2_r1 ^ v2_r1;
  atomic_store_explicit(__LINE__, &vars[1+v3_r9], 1, memory_order_release);
  int v10 = (v2_r1 == 0);
  atomic_store_explicit(__LINE__, &atom_0_r1_0, v10, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v5_r1 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v6_r9 = v5_r1 ^ v5_r1;
  atomic_store_explicit(__LINE__, &vars[0+v6_r9], 1, memory_order_release);
  int v11 = (v5_r1 == 0);
  atomic_store_explicit(__LINE__, &atom_1_r1_0, v11, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[0], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&atom_0_r1_0, 0); 
  atomic_init(&atom_1_r1_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v7 = atomic_load_explicit(__LINE__, &atom_0_r1_0, memory_order_acquire);
  int v8 = atomic_load_explicit(__LINE__, &atom_1_r1_0, memory_order_acquire);
  int v9_disj = v7 | v8;
  if (v9_disj == 0) MODEL_ASSERT(0);
  return 0;
}
