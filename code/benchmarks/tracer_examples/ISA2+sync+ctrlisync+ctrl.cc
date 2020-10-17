/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[3]; 
atomic<int> atom_1_r1_1; 
atomic<int> atom_2_r1_1; 
atomic<int> atom_2_r3_0; 

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
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__LINE__, &vars[2], 1, memory_order_release);
  int v14 = (v2_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r1_1, v14, memory_order_release);

}

void t2(void *arg){
label_3:;
  int v5_r1 = atomic_load_explicit(__LINE__, &vars[2], memory_order_acquire);
  int v6_cmpeq = (v5_r1 == v5_r1);
  if (v6_cmpeq)  goto lbl_LC01; else goto lbl_LC01;
lbl_LC01:;
  int v8_r3 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v15 = (v5_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_2_r1_1, v15, memory_order_release);
  int v16 = (v8_r3 == 0);
  atomic_store_explicit(__LINE__, &atom_2_r3_0, v16, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[2], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_2_r1_1, 0); 
  atomic_init(&atom_2_r3_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v9 = atomic_load_explicit(__LINE__, &atom_1_r1_1, memory_order_acquire);
  int v10 = atomic_load_explicit(__LINE__, &atom_2_r1_1, memory_order_acquire);
  int v11 = atomic_load_explicit(__LINE__, &atom_2_r3_0, memory_order_acquire);
  int v12_conj = v10 & v11;
  int v13_conj = v9 & v12_conj;
  if (v13_conj == 1) MODEL_ASSERT(0);
  return 0;
}
