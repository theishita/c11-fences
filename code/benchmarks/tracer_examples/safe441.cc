/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[2]; 
atomic<int> atom_0_r1_2; 
atomic<int> atom_1_r1_1; 
atomic<int> atom_1_r3_0; 
atomic<int> atom_2_r3_1; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  int v2_r1 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__LINE__, &vars[1], 1, memory_order_release);
  int v19 = (v2_r1 == 2);
  atomic_store_explicit(__LINE__, &atom_0_r1_2, v19, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v4_r1 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v6_r3 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v20 = (v4_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r1_1, v20, memory_order_release);
  int v21 = (v6_r3 == 0);
  atomic_store_explicit(__LINE__, &atom_1_r3_0, v21, memory_order_release);

}

void t2(void *arg){
label_3:;
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v8_r3 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v22 = (v8_r3 == 1);
  atomic_store_explicit(__LINE__, &atom_2_r3_1, v22, memory_order_release);

}

void t3(void *arg){
label_4:;
  atomic_store_explicit(__LINE__, &vars[0], 2, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 
  thrd_t thr3; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_0_r1_2, 0); 
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_1_r3_0, 0); 
  atomic_init(&atom_2_r3_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);
  thrd_create(&thr3, t3, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);
  thrd_join(thr3);

  int v9 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v10 = (v9 == 2);
  int v11 = atomic_load_explicit(__LINE__, &atom_0_r1_2, memory_order_acquire);
  int v12 = atomic_load_explicit(__LINE__, &atom_1_r1_1, memory_order_acquire);
  int v13 = atomic_load_explicit(__LINE__, &atom_1_r3_0, memory_order_acquire);
  int v14 = atomic_load_explicit(__LINE__, &atom_2_r3_1, memory_order_acquire);
  int v15_conj = v13 & v14;
  int v16_conj = v12 & v15_conj;
  int v17_conj = v11 & v16_conj;
  int v18_conj = v10 & v17_conj;
  if (v18_conj == 1) MODEL_ASSERT(0);
  return 0;
}
