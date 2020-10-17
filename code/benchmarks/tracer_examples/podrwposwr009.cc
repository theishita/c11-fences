/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[6]; 
atomic<int> atom_1_r1_1; 
atomic<int> atom_3_r1_1; 
atomic<int> atom_3_r8_1; 

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
  atomic_store_explicit(__LINE__, &vars[2], 1, memory_order_release);
  int v4_r5 = atomic_load_explicit(__LINE__, &vars[2], memory_order_acquire);
  int v5_r6 = v4_r5 ^ v4_r5;
  atomic_store_explicit(__LINE__, &vars[3+v5_r6], 1, memory_order_release);
  int v25 = (v2_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r1_1, v25, memory_order_release);

}

void t2(void *arg){
label_3:;
  atomic_store_explicit(__LINE__, &vars[3], 2, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__LINE__, &vars[4], 1, memory_order_release);

}

void t3(void *arg){
label_4:;
  int v7_r1 = atomic_load_explicit(__LINE__, &vars[4], memory_order_acquire);
  int v8_r3 = v7_r1 ^ v7_r1;
  int v11_r4 = atomic_load_explicit(__LINE__, &vars[5+v8_r3], memory_order_acquire);
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);
  int v13_r8 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v26 = (v7_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_3_r1_1, v26, memory_order_release);
  int v27 = (v13_r8 == 1);
  atomic_store_explicit(__LINE__, &atom_3_r8_1, v27, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 
  thrd_t thr3; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[5], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&vars[4], 0);
  atomic_init(&vars[3], 0);
  atomic_init(&vars[2], 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_3_r1_1, 0); 
  atomic_init(&atom_3_r8_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);
  thrd_create(&thr3, t3, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);
  thrd_join(thr3);

  int v14 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v15 = (v14 == 2);
  int v16 = atomic_load_explicit(__LINE__, &vars[3], memory_order_acquire);
  int v17 = (v16 == 2);
  int v18 = atomic_load_explicit(__LINE__, &atom_1_r1_1, memory_order_acquire);
  int v19 = atomic_load_explicit(__LINE__, &atom_3_r1_1, memory_order_acquire);
  int v20 = atomic_load_explicit(__LINE__, &atom_3_r8_1, memory_order_acquire);
  int v21_conj = v19 & v20;
  int v22_conj = v18 & v21_conj;
  int v23_conj = v17 & v22_conj;
  int v24_conj = v15 & v23_conj;
  if (v24_conj == 1) MODEL_ASSERT(0);
  return 0;
}
