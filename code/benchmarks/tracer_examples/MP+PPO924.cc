/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[2]; 
atomic<int> atom_1_r1_1; 
atomic<int> atom_1_r9_1; 

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
  int v4_r3 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  atomic_store_explicit(__LINE__, &vars[1], 2, memory_order_release);
  int v6_r5 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v7_r6 = v6_r5 ^ v6_r5;
  int v8_r6 = v7_r6 + 1;
  atomic_store_explicit(__LINE__, &vars[0], v8_r6, memory_order_release);
  int v10_r8 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v12_r9 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v22 = (v2_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r1_1, v22, memory_order_release);
  int v23 = (v12_r9 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r9_1, v23, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[0], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_1_r9_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v13 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v14 = (v13 == 2);
  int v15 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v16 = (v15 == 2);
  int v17 = atomic_load_explicit(__LINE__, &atom_1_r1_1, memory_order_acquire);
  int v18 = atomic_load_explicit(__LINE__, &atom_1_r9_1, memory_order_acquire);
  int v19_conj = v17 & v18;
  int v20_conj = v16 & v19_conj;
  int v21_conj = v14 & v20_conj;
  if (v21_conj == 1) MODEL_ASSERT(0);
  return 0;
}
