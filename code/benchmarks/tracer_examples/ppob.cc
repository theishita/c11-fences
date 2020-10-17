/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[3]; 
atomic<int> atom_0_r1_1; 
atomic<int> atom_0_r4_0; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  int v2_r1 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v3_cmpeq = (v2_r1 == 0);
  if (v3_cmpeq)  goto lbl_L00; else goto lbl_L00;
lbl_L00:;
  atomic_store_explicit(__LINE__, &vars[2], 1, memory_order_release);
  int v5_r2 = atomic_load_explicit(__LINE__, &vars[2], memory_order_acquire);
  int v6_r10 = v5_r2 ^ v5_r2;
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v9_r4 = atomic_load_explicit(__LINE__, &vars[1+v6_r10], memory_order_acquire);
  int v13 = (v2_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_0_r1_1, v13, memory_order_release);
  int v14 = (v9_r4 == 0);
  atomic_store_explicit(__LINE__, &atom_0_r4_0, v14, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(__LINE__, &vars[1], 1, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&vars[2], 0);
  atomic_init(&atom_0_r1_1, 0); 
  atomic_init(&atom_0_r4_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v10 = atomic_load_explicit(__LINE__, &atom_0_r1_1, memory_order_acquire);
  int v11 = atomic_load_explicit(__LINE__, &atom_0_r4_0, memory_order_acquire);
  int v12_conj = v10 & v11;
  if (v12_conj == 1) MODEL_ASSERT(0);
  return 0;
}
