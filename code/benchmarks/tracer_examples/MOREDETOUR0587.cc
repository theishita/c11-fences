/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[2]; 
atomic<int> atom_0_r5_2; 
atomic<int> atom_1_r4_5; 
atomic<int> atom_1_r5_0; 
atomic<int> atom_2_r4_4; 
atomic<int> atom_2_r1_2; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__LINE__, &vars[1], 1, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v2_r5 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v20 = (v2_r5 == 2);
  atomic_store_explicit(__LINE__, &atom_0_r5_2, v20, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(__LINE__, &vars[1], 2, memory_order_release);
  atomic_store_explicit(__LINE__, &vars[1], 4, memory_order_release);
  int v4_r4 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v6_r5 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v21 = (v4_r4 == 5);
  atomic_store_explicit(__LINE__, &atom_1_r4_5, v21, memory_order_release);
  int v22 = (v6_r5 == 0);
  atomic_store_explicit(__LINE__, &atom_1_r5_0, v22, memory_order_release);

}

void t2(void *arg){
label_3:;
  int v8_r1 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  atomic_store_explicit(__LINE__, &vars[1], 3, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v10_r4 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v23 = (v10_r4 == 4);
  atomic_store_explicit(__LINE__, &atom_2_r4_4, v23, memory_order_release);
  int v24 = (v8_r1 == 2);
  atomic_store_explicit(__LINE__, &atom_2_r1_2, v24, memory_order_release);

}

void t3(void *arg){
label_4:;
  atomic_store_explicit(__LINE__, &vars[1], 5, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 
  thrd_t thr3; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_0_r5_2, 0); 
  atomic_init(&atom_1_r4_5, 0); 
  atomic_init(&atom_1_r5_0, 0); 
  atomic_init(&atom_2_r4_4, 0); 
  atomic_init(&atom_2_r1_2, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);
  thrd_create(&thr3, t3, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);
  thrd_join(thr3);

  int v11 = atomic_load_explicit(__LINE__, &atom_0_r5_2, memory_order_acquire);
  int v12 = atomic_load_explicit(__LINE__, &atom_1_r4_5, memory_order_acquire);
  int v13 = atomic_load_explicit(__LINE__, &atom_1_r5_0, memory_order_acquire);
  int v14 = atomic_load_explicit(__LINE__, &atom_2_r4_4, memory_order_acquire);
  int v15 = atomic_load_explicit(__LINE__, &atom_2_r1_2, memory_order_acquire);
  int v16_conj = v14 & v15;
  int v17_conj = v13 & v16_conj;
  int v18_conj = v12 & v17_conj;
  int v19_conj = v11 & v18_conj;
  if (v19_conj == 1) MODEL_ASSERT(0);
  return 0;
}
