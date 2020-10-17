/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[3]; 
atomic<int> atom_1_r1_1; 
atomic<int> atom_1_r3_0; 
atomic<int> atom_1_r6_1; 
atomic<int> atom_1_r8_0; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__LINE__, &vars[1], 1, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v2_r1 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v3_cmpeq = (v2_r1 == v2_r1);
  if (v3_cmpeq)  goto lbl_LC00; else goto lbl_LC00;
lbl_LC00:;
  int v5_r3 = atomic_load_explicit(__LINE__, &vars[2], memory_order_acquire);
  atomic_store_explicit(__LINE__, &vars[2], 1, memory_order_release);
  int v7_r6 = atomic_load_explicit(__LINE__, &vars[2], memory_order_acquire);
  int v8_r7 = v7_r6 ^ v7_r6;
  int v11_r8 = atomic_load_explicit(__LINE__, &vars[0+v8_r7], memory_order_acquire);
  int v19 = (v2_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r1_1, v19, memory_order_release);
  int v20 = (v5_r3 == 0);
  atomic_store_explicit(__LINE__, &atom_1_r3_0, v20, memory_order_release);
  int v21 = (v7_r6 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r6_1, v21, memory_order_release);
  int v22 = (v11_r8 == 0);
  atomic_store_explicit(__LINE__, &atom_1_r8_0, v22, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&vars[2], 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_1_r3_0, 0); 
  atomic_init(&atom_1_r6_1, 0); 
  atomic_init(&atom_1_r8_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v12 = atomic_load_explicit(__LINE__, &atom_1_r1_1, memory_order_acquire);
  int v13 = atomic_load_explicit(__LINE__, &atom_1_r3_0, memory_order_acquire);
  int v14 = atomic_load_explicit(__LINE__, &atom_1_r6_1, memory_order_acquire);
  int v15 = atomic_load_explicit(__LINE__, &atom_1_r8_0, memory_order_acquire);
  int v16_conj = v14 & v15;
  int v17_conj = v13 & v16_conj;
  int v18_conj = v12 & v17_conj;
  if (v18_conj == 1) MODEL_ASSERT(0);
  return 0;
}
