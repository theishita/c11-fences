/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[2]; 
atomic<int> atom_0_r5_5; 
atomic<int> atom_1_r4_4; 
atomic<int> atom_1_r1_2; 
atomic<int> atom_2_r5_2; 
atomic<int> atom_2_r1_1; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, &vars[0], 2, memory_order_release);
  atomic_store_explicit(__LINE__, &vars[0], 4, memory_order_release);
  atomic_store_explicit(__LINE__, &vars[0], 5, memory_order_release);
  int v2_r5 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v3_r6 = v2_r5 ^ v2_r5;
  atomic_store_explicit(__LINE__, &vars[1+v3_r6], 1, memory_order_release);
  int v23 = (v2_r5 == 5);
  atomic_store_explicit(__LINE__, &atom_0_r5_5, v23, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v5_r1 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  atomic_store_explicit(__LINE__, &vars[0], 3, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v7_r4 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v24 = (v7_r4 == 4);
  atomic_store_explicit(__LINE__, &atom_1_r4_4, v24, memory_order_release);
  int v25 = (v5_r1 == 2);
  atomic_store_explicit(__LINE__, &atom_1_r1_2, v25, memory_order_release);

}

void t2(void *arg){
label_3:;
  int v9_r1 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v10_r3 = v9_r1 ^ v9_r1;
  int v11_r3 = v10_r3 + 1;
  atomic_store_explicit(__LINE__, &vars[0], v11_r3, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v13_r5 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v26 = (v13_r5 == 2);
  atomic_store_explicit(__LINE__, &atom_2_r5_2, v26, memory_order_release);
  int v27 = (v9_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_2_r1_1, v27, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_0_r5_5, 0); 
  atomic_init(&atom_1_r4_4, 0); 
  atomic_init(&atom_1_r1_2, 0); 
  atomic_init(&atom_2_r5_2, 0); 
  atomic_init(&atom_2_r1_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v14 = atomic_load_explicit(__LINE__, &atom_0_r5_5, memory_order_acquire);
  int v15 = atomic_load_explicit(__LINE__, &atom_1_r4_4, memory_order_acquire);
  int v16 = atomic_load_explicit(__LINE__, &atom_1_r1_2, memory_order_acquire);
  int v17 = atomic_load_explicit(__LINE__, &atom_2_r5_2, memory_order_acquire);
  int v18 = atomic_load_explicit(__LINE__, &atom_2_r1_1, memory_order_acquire);
  int v19_conj = v17 & v18;
  int v20_conj = v16 & v19_conj;
  int v21_conj = v15 & v20_conj;
  int v22_conj = v14 & v21_conj;
  if (v22_conj == 1) MODEL_ASSERT(0);
  return 0;
}
