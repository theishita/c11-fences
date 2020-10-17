/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[2]; 
atomic<int> atom_0_r3_1; 
atomic<int> atom_1_r3_2; 
atomic<int> atom_1_r5_0; 
atomic<int> atom_2_r3_1; 
atomic<int> atom_3_r3_2; 
atomic<int> atom_3_r5_0; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v2_r3 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v34 = (v2_r3 == 1);
  atomic_store_explicit(__LINE__, &atom_0_r3_1, v34, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(__LINE__, &vars[0], 2, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v4_r3 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v5_r4 = v4_r3 ^ v4_r3;
  int v8_r5 = atomic_load_explicit(__LINE__, &vars[1+v5_r4], memory_order_acquire);
  int v35 = (v4_r3 == 2);
  atomic_store_explicit(__LINE__, &atom_1_r3_2, v35, memory_order_release);
  int v36 = (v8_r5 == 0);
  atomic_store_explicit(__LINE__, &atom_1_r5_0, v36, memory_order_release);

}

void t2(void *arg){
label_3:;
  atomic_store_explicit(__LINE__, &vars[1], 1, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v10_r3 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v37 = (v10_r3 == 1);
  atomic_store_explicit(__LINE__, &atom_2_r3_1, v37, memory_order_release);

}

void t3(void *arg){
label_4:;
  atomic_store_explicit(__LINE__, &vars[1], 2, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v12_r3 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v13_r4 = v12_r3 ^ v12_r3;
  int v16_r5 = atomic_load_explicit(__LINE__, &vars[0+v13_r4], memory_order_acquire);
  int v38 = (v12_r3 == 2);
  atomic_store_explicit(__LINE__, &atom_3_r3_2, v38, memory_order_release);
  int v39 = (v16_r5 == 0);
  atomic_store_explicit(__LINE__, &atom_3_r5_0, v39, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 
  thrd_t thr3; 

  atomic_init(&vars[0], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&atom_0_r3_1, 0); 
  atomic_init(&atom_1_r3_2, 0); 
  atomic_init(&atom_1_r5_0, 0); 
  atomic_init(&atom_2_r3_1, 0); 
  atomic_init(&atom_3_r3_2, 0); 
  atomic_init(&atom_3_r5_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);
  thrd_create(&thr3, t3, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);
  thrd_join(thr3);

  int v17 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v18 = (v17 == 2);
  int v19 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v20 = (v19 == 2);
  int v21 = atomic_load_explicit(__LINE__, &atom_0_r3_1, memory_order_acquire);
  int v22 = atomic_load_explicit(__LINE__, &atom_1_r3_2, memory_order_acquire);
  int v23 = atomic_load_explicit(__LINE__, &atom_1_r5_0, memory_order_acquire);
  int v24 = atomic_load_explicit(__LINE__, &atom_2_r3_1, memory_order_acquire);
  int v25 = atomic_load_explicit(__LINE__, &atom_3_r3_2, memory_order_acquire);
  int v26 = atomic_load_explicit(__LINE__, &atom_3_r5_0, memory_order_acquire);
  int v27_conj = v25 & v26;
  int v28_conj = v24 & v27_conj;
  int v29_conj = v23 & v28_conj;
  int v30_conj = v22 & v29_conj;
  int v31_conj = v21 & v30_conj;
  int v32_conj = v20 & v31_conj;
  int v33_conj = v18 & v32_conj;
  if (v33_conj == 1) MODEL_ASSERT(0);
  return 0;
}
