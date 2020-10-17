/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[3]; 
atomic<int> atom_1_r1_1; 
atomic<int> atom_1_r10_1; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, &vars[0], 2, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__LINE__, &vars[1], 1, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v2_r1 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v3_r3 = v2_r1 ^ v2_r1;
  atomic_store_explicit(__LINE__, &vars[2+v3_r3], 1, memory_order_release);
  int v5_r6 = atomic_load_explicit(__LINE__, &vars[2], memory_order_acquire);
  int v7_r7 = atomic_load_explicit(__LINE__, &vars[2], memory_order_acquire);
  int v8_r8 = v7_r7 ^ v7_r7;
  int v9_r8 = v8_r8 + 1;
  atomic_store_explicit(__LINE__, &vars[0], v9_r8, memory_order_release);
  int v11_r10 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v18 = (v2_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r1_1, v18, memory_order_release);
  int v19 = (v11_r10 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r10_1, v19, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[0], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[2], 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_1_r10_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v12 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v13 = (v12 == 2);
  int v14 = atomic_load_explicit(__LINE__, &atom_1_r1_1, memory_order_acquire);
  int v15 = atomic_load_explicit(__LINE__, &atom_1_r10_1, memory_order_acquire);
  int v16_conj = v14 & v15;
  int v17_conj = v13 & v16_conj;
  if (v17_conj == 1) MODEL_ASSERT(0);
  return 0;
}
