/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[2]; 
atomic<int> atom_0_r3_1; 
atomic<int> atom_0_r5_0; 
atomic<int> atom_1_r3_1; 
atomic<int> atom_2_r3_2; 
atomic<int> atom_2_r5_0; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v2_r3 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v3_r4 = v2_r3 ^ v2_r3;
  int v6_r5 = atomic_load_explicit(__LINE__, &vars[1+v3_r4], memory_order_acquire);
  int v27 = (v2_r3 == 1);
  atomic_store_explicit(__LINE__, &atom_0_r3_1, v27, memory_order_release);
  int v28 = (v6_r5 == 0);
  atomic_store_explicit(__LINE__, &atom_0_r5_0, v28, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(__LINE__, &vars[1], 1, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v8_r3 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v29 = (v8_r3 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r3_1, v29, memory_order_release);

}

void t2(void *arg){
label_3:;
  atomic_store_explicit(__LINE__, &vars[1], 2, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v10_r3 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v11_r4 = v10_r3 ^ v10_r3;
  int v14_r5 = atomic_load_explicit(__LINE__, &vars[0+v11_r4], memory_order_acquire);
  int v30 = (v10_r3 == 2);
  atomic_store_explicit(__LINE__, &atom_2_r3_2, v30, memory_order_release);
  int v31 = (v14_r5 == 0);
  atomic_store_explicit(__LINE__, &atom_2_r5_0, v31, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars[0], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&atom_0_r3_1, 0); 
  atomic_init(&atom_0_r5_0, 0); 
  atomic_init(&atom_1_r3_1, 0); 
  atomic_init(&atom_2_r3_2, 0); 
  atomic_init(&atom_2_r5_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v15 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v16 = (v15 == 2);
  int v17 = atomic_load_explicit(__LINE__, &atom_0_r3_1, memory_order_acquire);
  int v18 = atomic_load_explicit(__LINE__, &atom_0_r5_0, memory_order_acquire);
  int v19 = atomic_load_explicit(__LINE__, &atom_1_r3_1, memory_order_acquire);
  int v20 = atomic_load_explicit(__LINE__, &atom_2_r3_2, memory_order_acquire);
  int v21 = atomic_load_explicit(__LINE__, &atom_2_r5_0, memory_order_acquire);
  int v22_conj = v20 & v21;
  int v23_conj = v19 & v22_conj;
  int v24_conj = v18 & v23_conj;
  int v25_conj = v17 & v24_conj;
  int v26_conj = v16 & v25_conj;
  if (v26_conj == 1) MODEL_ASSERT(0);
  return 0;
}
