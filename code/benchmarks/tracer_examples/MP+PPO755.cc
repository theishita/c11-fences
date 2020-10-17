/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[3]; 
atomic<int> atom_1_r1_1; 
atomic<int> atom_1_r11_1; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, __LINE__, &vars[0], 2, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__LINE__, __LINE__, &vars[1], 1, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v2_r1 = atomic_load_explicit(__LINE__, __LINE__, &vars[1], memory_order_acquire);
  atomic_store_explicit(__LINE__, __LINE__, &vars[1], 2, memory_order_release);
  int v4_r4 = atomic_load_explicit(__LINE__, __LINE__, &vars[1], memory_order_acquire);
  int v5_r5 = v4_r4 ^ v4_r4;
  int v8_r6 = atomic_load_explicit(__LINE__, __LINE__, &vars[2+v5_r5], memory_order_acquire);
  int v10_r8 = atomic_load_explicit(__LINE__, __LINE__, &vars[2], memory_order_acquire);
  int v11_cmpeq = (v10_r8 == v10_r8);
  if (v11_cmpeq)  goto lbl_LC00; else goto lbl_LC00;
lbl_LC00:;
  atomic_store_explicit(__LINE__, __LINE__, &vars[0], 1, memory_order_release);
  int v13_r11 = atomic_load_explicit(__LINE__, __LINE__, &vars[0], memory_order_acquire);
  int v23 = (v2_r1 == 1);
  atomic_store_explicit(__LINE__, __LINE__, &atom_1_r1_1, v23, memory_order_release);
  int v24 = (v13_r11 == 1);
  atomic_store_explicit(__LINE__, __LINE__, &atom_1_r11_1, v24, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[0], 0);
  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_1_r11_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v14 = atomic_load_explicit(__LINE__, __LINE__, &vars[0], memory_order_acquire);
  int v15 = (v14 == 2);
  int v16 = atomic_load_explicit(__LINE__, __LINE__, &vars[1], memory_order_acquire);
  int v17 = (v16 == 2);
  int v18 = atomic_load_explicit(__LINE__, __LINE__, &atom_1_r1_1, memory_order_acquire);
  int v19 = atomic_load_explicit(__LINE__, __LINE__, &atom_1_r11_1, memory_order_acquire);
  int v20_conj = v18 & v19;
  int v21_conj = v17 & v20_conj;
  int v22_conj = v15 & v21_conj;
  if (v22_conj == 1) MODEL_ASSERT(0);
  return 0;
}
