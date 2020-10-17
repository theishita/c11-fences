/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[2]; 
atomic<int> atom_1_r1_1; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, &vars[0], 2, memory_order_release);
  int v2_r3 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v4_r4 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v5_r5 = v4_r4 ^ v4_r4;
  atomic_store_explicit(__LINE__, &vars[1+v5_r5], 1, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v7_r1 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v8_r3 = v7_r1 ^ v7_r1;
  int v9_r3 = v8_r3 + 1;
  atomic_store_explicit(__LINE__, &vars[0], v9_r3, memory_order_release);
  int v14 = (v7_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r1_1, v14, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v10 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v11 = (v10 == 2);
  int v12 = atomic_load_explicit(__LINE__, &atom_1_r1_1, memory_order_acquire);
  int v13_conj = v11 & v12;
  if (v13_conj == 1) MODEL_ASSERT(0);
  return 0;
}
