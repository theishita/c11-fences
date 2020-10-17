/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[5]; 
atomic<int> atom_1_r1_1; 
atomic<int> atom_1_r12_0; 

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
  atomic_store_explicit(__LINE__, &vars[2], 1, memory_order_release);
  int v5_r5 = atomic_load_explicit(__LINE__, &vars[2], memory_order_acquire);
  int v6_r6 = v5_r5 ^ v5_r5;
  int v7_r6 = v6_r6 + 1;
  atomic_store_explicit(__LINE__, &vars[3], v7_r6, memory_order_release);
  int v9_r8 = atomic_load_explicit(__LINE__, &vars[3], memory_order_acquire);
  int v10_r9 = v9_r8 ^ v9_r8;
  int v13_r10 = atomic_load_explicit(__LINE__, &vars[4+v10_r9], memory_order_acquire);
  int v14_cmpeq = (v13_r10 == v13_r10);
  if (v14_cmpeq)  goto lbl_LC01; else goto lbl_LC01;
lbl_LC01:;
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v16_r12 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v20 = (v2_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r1_1, v20, memory_order_release);
  int v21 = (v16_r12 == 0);
  atomic_store_explicit(__LINE__, &atom_1_r12_0, v21, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[4], 0);
  atomic_init(&vars[3], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&vars[2], 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_1_r12_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v17 = atomic_load_explicit(__LINE__, &atom_1_r1_1, memory_order_acquire);
  int v18 = atomic_load_explicit(__LINE__, &atom_1_r12_0, memory_order_acquire);
  int v19_conj = v17 & v18;
  if (v19_conj == 1) MODEL_ASSERT(0);
  return 0;
}
