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
  int v3_cmpeq = (v2_r1 == v2_r1);
  if (v3_cmpeq)  goto lbl_LC00; else goto lbl_LC00;
lbl_LC00:;
  atomic_store_explicit(__LINE__, &vars[2], 1, memory_order_release);
  int v5_r5 = atomic_load_explicit(__LINE__, &vars[2], memory_order_acquire);
  int v6_r6 = v5_r5 ^ v5_r5;
  int v9_r7 = atomic_load_explicit(__LINE__, &vars[3+v6_r6], memory_order_acquire);
  int v11_r9 = atomic_load_explicit(__LINE__, &vars[3], memory_order_acquire);
  int v12_r10 = v11_r9 ^ v11_r9;
  int v15_r11 = atomic_load_explicit(__LINE__, &vars[4+v12_r10], memory_order_acquire);
  int v16_cmpeq = (v15_r11 == v15_r11);
  if (v16_cmpeq)  goto lbl_LC01; else goto lbl_LC01;
lbl_LC01:;
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);
  int v21 = (v2_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r1_1, v21, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&vars[3], 0);
  atomic_init(&vars[2], 0);
  atomic_init(&vars[4], 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v17 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v18 = (v17 == 2);
  int v19 = atomic_load_explicit(__LINE__, &atom_1_r1_1, memory_order_acquire);
  int v20_conj = v18 & v19;
  if (v20_conj == 1) MODEL_ASSERT(0);
  return 0;
}
