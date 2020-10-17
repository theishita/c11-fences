/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[3]; 
atomic<int> atom_1_r3_0; 
atomic<int> atom_1_r5_1; 
atomic<int> atom_1_r7_0; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__LINE__, &vars[1], 1, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(__LINE__, &vars[1], 2, memory_order_release);
  int v2_r3 = atomic_load_explicit(__LINE__, &vars[2], memory_order_acquire);
  int v4_r5 = atomic_load_explicit(__LINE__, &vars[2], memory_order_acquire);
  int v5_r6 = v4_r5 ^ v4_r5;
  int v8_r7 = atomic_load_explicit(__LINE__, &vars[0+v5_r6], memory_order_acquire);
  int v17 = (v2_r3 == 0);
  atomic_store_explicit(__LINE__, &atom_1_r3_0, v17, memory_order_release);
  int v18 = (v4_r5 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r5_1, v18, memory_order_release);
  int v19 = (v8_r7 == 0);
  atomic_store_explicit(__LINE__, &atom_1_r7_0, v19, memory_order_release);

}

void t2(void *arg){
label_3:;
  atomic_store_explicit(__LINE__, &vars[2], 1, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[2], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_1_r3_0, 0); 
  atomic_init(&atom_1_r5_1, 0); 
  atomic_init(&atom_1_r7_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v9 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v10 = (v9 == 2);
  int v11 = atomic_load_explicit(__LINE__, &atom_1_r3_0, memory_order_acquire);
  int v12 = atomic_load_explicit(__LINE__, &atom_1_r5_1, memory_order_acquire);
  int v13 = atomic_load_explicit(__LINE__, &atom_1_r7_0, memory_order_acquire);
  int v14_conj = v12 & v13;
  int v15_conj = v11 & v14_conj;
  int v16_conj = v10 & v15_conj;
  if (v16_conj == 1) MODEL_ASSERT(0);
  return 0;
}
