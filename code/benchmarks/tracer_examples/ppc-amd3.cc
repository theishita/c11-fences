/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[2]; 
atomic<int> atom_0_r7_55; 
atomic<int> atom_1_r7_55; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, &vars[0], 55, memory_order_release);
  atomic_store_explicit(__LINE__, &vars[0], 66, memory_order_release);
  int v2_r7 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v8 = (v2_r7 == 55);
  atomic_store_explicit(__LINE__, &atom_0_r7_55, v8, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(__LINE__, &vars[1], 55, memory_order_release);
  atomic_store_explicit(__LINE__, &vars[1], 66, memory_order_release);
  int v4_r7 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v9 = (v4_r7 == 55);
  atomic_store_explicit(__LINE__, &atom_1_r7_55, v9, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[0], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&atom_0_r7_55, 0); 
  atomic_init(&atom_1_r7_55, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v5 = atomic_load_explicit(__LINE__, &atom_0_r7_55, memory_order_acquire);
  int v6 = atomic_load_explicit(__LINE__, &atom_1_r7_55, memory_order_acquire);
  int v7_conj = v5 & v6;
  if (v7_conj == 1) MODEL_ASSERT(0);
  return 0;
}
