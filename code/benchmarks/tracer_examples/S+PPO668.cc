/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[5]; 
atomic<int> atom_1_r1_1; 

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
  int v6_r7 = v5_r6 ^ v5_r6;
  int v9_r8 = atomic_load_explicit(__LINE__, &vars[3+v6_r7], memory_order_acquire);
  int v10_r10 = v9_r8 ^ v9_r8;
  int v11_r10 = v10_r10 + 1;
  atomic_store_explicit(__LINE__, &vars[4], v11_r10, memory_order_release);
  int v13_r12 = atomic_load_explicit(__LINE__, &vars[4], memory_order_acquire);
  int v14_cmpeq = (v13_r12 == v13_r12);
  if (v14_cmpeq)  goto lbl_LC00; else goto lbl_LC00;
lbl_LC00:;
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);
  int v19 = (v2_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r1_1, v19, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[0], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[3], 0);
  atomic_init(&vars[4], 0);
  atomic_init(&vars[2], 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v15 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v16 = (v15 == 2);
  int v17 = atomic_load_explicit(__LINE__, &atom_1_r1_1, memory_order_acquire);
  int v18_conj = v16 & v17;
  if (v18_conj == 1) MODEL_ASSERT(0);
  return 0;
}
