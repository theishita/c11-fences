/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[2]; 
atomic<int> atom_0_r4_2; 
atomic<int> atom_1_r3_0; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);
  atomic_store_explicit(__LINE__, &vars[0], 2, memory_order_release);
  int v2_r4 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v3_cmpeq = (v2_r4 == v2_r4);
  if (v3_cmpeq)  goto lbl_LC00; else goto lbl_LC00;
lbl_LC00:;
  atomic_store_explicit(__LINE__, &vars[1], 1, memory_order_release);
  int v12 = (v2_r4 == 2);
  atomic_store_explicit(__LINE__, &atom_0_r4_2, v12, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(__LINE__, &vars[1], 2, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v5_r3 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v13 = (v5_r3 == 0);
  atomic_store_explicit(__LINE__, &atom_1_r3_0, v13, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[0], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&atom_0_r4_2, 0); 
  atomic_init(&atom_1_r3_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v6 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v7 = (v6 == 2);
  int v8 = atomic_load_explicit(__LINE__, &atom_0_r4_2, memory_order_acquire);
  int v9 = atomic_load_explicit(__LINE__, &atom_1_r3_0, memory_order_acquire);
  int v10_conj = v8 & v9;
  int v11_conj = v7 & v10_conj;
  if (v11_conj == 1) MODEL_ASSERT(0);
  return 0;
}
