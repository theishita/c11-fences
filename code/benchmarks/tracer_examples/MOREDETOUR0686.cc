/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[2]; 
atomic<int> atom_0_r3_1; 
atomic<int> atom_0_r4_0; 
atomic<int> atom_1_r3_2; 
atomic<int> atom_2_r3_0; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);
  int v2_r3 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v3_cmpeq = (v2_r3 == v2_r3);
  if (v3_cmpeq)  goto lbl_LC00; else goto lbl_LC00;
lbl_LC00:;
  int v5_r4 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  atomic_store_explicit(__LINE__, &vars[1], 2, memory_order_release);
  int v20 = (v2_r3 == 1);
  atomic_store_explicit(__LINE__, &atom_0_r3_1, v20, memory_order_release);
  int v21 = (v5_r4 == 0);
  atomic_store_explicit(__LINE__, &atom_0_r4_0, v21, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(__LINE__, &vars[1], 1, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v7_r3 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v22 = (v7_r3 == 2);
  atomic_store_explicit(__LINE__, &atom_1_r3_2, v22, memory_order_release);

}

void t2(void *arg){
label_3:;
  atomic_store_explicit(__LINE__, &vars[1], 3, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v9_r3 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v23 = (v9_r3 == 0);
  atomic_store_explicit(__LINE__, &atom_2_r3_0, v23, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_0_r3_1, 0); 
  atomic_init(&atom_0_r4_0, 0); 
  atomic_init(&atom_1_r3_2, 0); 
  atomic_init(&atom_2_r3_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v10 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v11 = (v10 == 3);
  int v12 = atomic_load_explicit(__LINE__, &atom_0_r3_1, memory_order_acquire);
  int v13 = atomic_load_explicit(__LINE__, &atom_0_r4_0, memory_order_acquire);
  int v14 = atomic_load_explicit(__LINE__, &atom_1_r3_2, memory_order_acquire);
  int v15 = atomic_load_explicit(__LINE__, &atom_2_r3_0, memory_order_acquire);
  int v16_conj = v14 & v15;
  int v17_conj = v13 & v16_conj;
  int v18_conj = v12 & v17_conj;
  int v19_conj = v11 & v18_conj;
  if (v19_conj == 1) MODEL_ASSERT(0);
  return 0;
}
