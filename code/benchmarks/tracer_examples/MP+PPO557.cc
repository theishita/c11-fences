/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[3]; 
atomic<int> atom_1_r1_1; 
atomic<int> atom_1_r12_1; 

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
  int v6_r4 = atomic_load_explicit(__LINE__, &vars[2+v3_r3], memory_order_acquire);
  atomic_store_explicit(__LINE__, &vars[2], 1, memory_order_release);
  atomic_store_explicit(__LINE__, &vars[2], 2, memory_order_release);
  int v8_r8 = atomic_load_explicit(__LINE__, &vars[2], memory_order_acquire);
  int v9_r9 = v8_r8 ^ v8_r8;
  atomic_store_explicit(__LINE__, &vars[0+v9_r9], 1, memory_order_release);
  int v11_r12 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v21 = (v2_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r1_1, v21, memory_order_release);
  int v22 = (v11_r12 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r12_1, v22, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_1_r12_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v12 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v13 = (v12 == 2);
  int v14 = atomic_load_explicit(__LINE__, &vars[2], memory_order_acquire);
  int v15 = (v14 == 2);
  int v16 = atomic_load_explicit(__LINE__, &atom_1_r1_1, memory_order_acquire);
  int v17 = atomic_load_explicit(__LINE__, &atom_1_r12_1, memory_order_acquire);
  int v18_conj = v16 & v17;
  int v19_conj = v15 & v18_conj;
  int v20_conj = v13 & v19_conj;
  if (v20_conj == 1) MODEL_ASSERT(0);
  return 0;
}
