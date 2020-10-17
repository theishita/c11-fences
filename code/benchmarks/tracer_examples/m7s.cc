/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[2]; 
atomic<int> atom_1_r1_1; 
atomic<int> atom_1_r2_0; 
atomic<int> atom_2_r3_0; 
atomic<int> atom_1_r2_2; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v2_r1 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v4_r2 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v24 = (v2_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r1_1, v24, memory_order_release);
  int v25 = (v4_r2 == 0);
  atomic_store_explicit(__LINE__, &atom_1_r2_0, v25, memory_order_release);
  int v27 = (v4_r2 == 2);
  atomic_store_explicit(__LINE__, &atom_1_r2_2, v27, memory_order_release);

}

void t2(void *arg){
label_3:;
  atomic_store_explicit(__LINE__, &vars[1], 1, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v6_r3 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v26 = (v6_r3 == 0);
  atomic_store_explicit(__LINE__, &atom_2_r3_0, v26, memory_order_release);

}

void t3(void *arg){
label_4:;
  atomic_store_explicit(__LINE__, &vars[1], 2, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 
  thrd_t thr3; 

  atomic_init(&vars[0], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_1_r2_0, 0); 
  atomic_init(&atom_2_r3_0, 0); 
  atomic_init(&atom_1_r2_2, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);
  thrd_create(&thr3, t3, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);
  thrd_join(thr3);

  int v7 = atomic_load_explicit(__LINE__, &atom_1_r1_1, memory_order_acquire);
  int v8 = atomic_load_explicit(__LINE__, &atom_1_r2_0, memory_order_acquire);
  int v9 = atomic_load_explicit(__LINE__, &atom_2_r3_0, memory_order_acquire);
  int v10_conj = v8 & v9;
  int v11_conj = v7 & v10_conj;
  int v12 = atomic_load_explicit(__LINE__, &atom_1_r1_1, memory_order_acquire);
  int v13 = atomic_load_explicit(__LINE__, &atom_1_r2_2, memory_order_acquire);
  int v14 = atomic_load_explicit(__LINE__, &atom_2_r3_0, memory_order_acquire);
  int v15 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v16 = (v15 == 1);
  int v17 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v18 = (v17 == 1);
  int v19_conj = v16 & v18;
  int v20_conj = v14 & v19_conj;
  int v21_conj = v13 & v20_conj;
  int v22_conj = v12 & v21_conj;
  int v23_disj = v11_conj | v22_conj;
  if (v23_disj == 1) MODEL_ASSERT(0);
  return 0;
}
