/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[2]; 
atomic<int> atom_0_r5_2; 
atomic<int> atom_1_r5_5; 
atomic<int> atom_1_r7_0; 
atomic<int> atom_2_r4_4; 
atomic<int> atom_2_r1_2; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, __LINE__, &vars[0], 1, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__LINE__, __LINE__, &vars[1], 1, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v2_r5 = atomic_load_explicit(__LINE__, __LINE__, &vars[1], memory_order_acquire);
  int v22 = (v2_r5 == 2);
  atomic_store_explicit(__LINE__, __LINE__, &atom_0_r5_2, v22, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(__LINE__, __LINE__, &vars[1], 2, memory_order_release);
  atomic_store_explicit(__LINE__, __LINE__, &vars[1], 4, memory_order_release);
  atomic_store_explicit(__LINE__, __LINE__, &vars[1], 5, memory_order_release);
  int v4_r5 = atomic_load_explicit(__LINE__, __LINE__, &vars[1], memory_order_acquire);
  int v5_r6 = v4_r5 ^ v4_r5;
  int v8_r7 = atomic_load_explicit(__LINE__, __LINE__, &vars[0+v5_r6], memory_order_acquire);
  int v23 = (v4_r5 == 5);
  atomic_store_explicit(__LINE__, __LINE__, &atom_1_r5_5, v23, memory_order_release);
  int v24 = (v8_r7 == 0);
  atomic_store_explicit(__LINE__, __LINE__, &atom_1_r7_0, v24, memory_order_release);

}

void t2(void *arg){
label_3:;
  int v10_r1 = atomic_load_explicit(__LINE__, __LINE__, &vars[1], memory_order_acquire);
  atomic_store_explicit(__LINE__, __LINE__, &vars[1], 3, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v12_r4 = atomic_load_explicit(__LINE__, __LINE__, &vars[1], memory_order_acquire);
  int v25 = (v12_r4 == 4);
  atomic_store_explicit(__LINE__, __LINE__, &atom_2_r4_4, v25, memory_order_release);
  int v26 = (v10_r1 == 2);
  atomic_store_explicit(__LINE__, __LINE__, &atom_2_r1_2, v26, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars[0], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&atom_0_r5_2, 0); 
  atomic_init(&atom_1_r5_5, 0); 
  atomic_init(&atom_1_r7_0, 0); 
  atomic_init(&atom_2_r4_4, 0); 
  atomic_init(&atom_2_r1_2, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v13 = atomic_load_explicit(__LINE__, __LINE__, &atom_0_r5_2, memory_order_acquire);
  int v14 = atomic_load_explicit(__LINE__, __LINE__, &atom_1_r5_5, memory_order_acquire);
  int v15 = atomic_load_explicit(__LINE__, __LINE__, &atom_1_r7_0, memory_order_acquire);
  int v16 = atomic_load_explicit(__LINE__, __LINE__, &atom_2_r4_4, memory_order_acquire);
  int v17 = atomic_load_explicit(__LINE__, __LINE__, &atom_2_r1_2, memory_order_acquire);
  int v18_conj = v16 & v17;
  int v19_conj = v15 & v18_conj;
  int v20_conj = v14 & v19_conj;
  int v21_conj = v13 & v20_conj;
  if (v21_conj == 1) MODEL_ASSERT(0);
  return 0;
}
