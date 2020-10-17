/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[2]; 
atomic<int> atom_0_r1_1; 
atomic<int> atom_0_r3_1; 
atomic<int> atom_2_r1_1; 
atomic<int> atom_2_r4_1; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  int v2_r1 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v4_r3 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v24 = (v2_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_0_r1_1, v24, memory_order_release);
  int v25 = (v4_r3 == 1);
  atomic_store_explicit(__LINE__, &atom_0_r3_1, v25, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(__LINE__, &vars[0], 2, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__LINE__, &vars[1], 1, memory_order_release);

}

void t2(void *arg){
label_3:;
  int v6_r1 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v7_r3 = v6_r1 ^ v6_r1;
  int v10_r4 = atomic_load_explicit(__LINE__, &vars[1+v7_r3], memory_order_acquire);
  int v26 = (v6_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_2_r1_1, v26, memory_order_release);
  int v27 = (v10_r4 == 1);
  atomic_store_explicit(__LINE__, &atom_2_r4_1, v27, memory_order_release);

}

void t3(void *arg){
label_4:;
  atomic_store_explicit(__LINE__, &vars[1], 2, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 
  thrd_t thr3; 

  atomic_init(&vars[0], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&atom_0_r1_1, 0); 
  atomic_init(&atom_0_r3_1, 0); 
  atomic_init(&atom_2_r1_1, 0); 
  atomic_init(&atom_2_r4_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);
  thrd_create(&thr3, t3, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);
  thrd_join(thr3);

  int v11 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v12 = (v11 == 2);
  int v13 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v14 = (v13 == 2);
  int v15 = atomic_load_explicit(__LINE__, &atom_0_r1_1, memory_order_acquire);
  int v16 = atomic_load_explicit(__LINE__, &atom_0_r3_1, memory_order_acquire);
  int v17 = atomic_load_explicit(__LINE__, &atom_2_r1_1, memory_order_acquire);
  int v18 = atomic_load_explicit(__LINE__, &atom_2_r4_1, memory_order_acquire);
  int v19_conj = v17 & v18;
  int v20_conj = v16 & v19_conj;
  int v21_conj = v15 & v20_conj;
  int v22_conj = v14 & v21_conj;
  int v23_conj = v12 & v22_conj;
  if (v23_conj == 1) MODEL_ASSERT(0);
  return 0;
}
