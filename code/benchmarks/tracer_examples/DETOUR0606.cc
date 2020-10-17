/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[2]; 
atomic<int> atom_0_r3_0; 
atomic<int> atom_1_r3_2; 
atomic<int> atom_1_r7_2; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, __LINE__, &vars[0], 3, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v2_r3 = atomic_load_explicit(__LINE__, __LINE__, &vars[1], memory_order_acquire);
  int v16 = (v2_r3 == 0);
  atomic_store_explicit(__LINE__, __LINE__, &atom_0_r3_0, v16, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(__LINE__, __LINE__, &vars[1], 1, memory_order_release);
  int v4_r3 = atomic_load_explicit(__LINE__, __LINE__, &vars[1], memory_order_acquire);
  int v5_r4 = v4_r3 ^ v4_r3;
  atomic_store_explicit(__LINE__, __LINE__, &vars[0+v5_r4], 1, memory_order_release);
  int v7_r7 = atomic_load_explicit(__LINE__, __LINE__, &vars[0], memory_order_acquire);
  int v17 = (v4_r3 == 2);
  atomic_store_explicit(__LINE__, __LINE__, &atom_1_r3_2, v17, memory_order_release);
  int v18 = (v7_r7 == 2);
  atomic_store_explicit(__LINE__, __LINE__, &atom_1_r7_2, v18, memory_order_release);

}

void t2(void *arg){
label_3:;
  atomic_store_explicit(__LINE__, __LINE__, &vars[1], 2, memory_order_release);

}

void t3(void *arg){
label_4:;
  atomic_store_explicit(__LINE__, __LINE__, &vars[0], 2, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 
  thrd_t thr3; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_0_r3_0, 0); 
  atomic_init(&atom_1_r3_2, 0); 
  atomic_init(&atom_1_r7_2, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);
  thrd_create(&thr3, t3, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);
  thrd_join(thr3);

  int v8 = atomic_load_explicit(__LINE__, __LINE__, &atom_0_r3_0, memory_order_acquire);
  int v9 = atomic_load_explicit(__LINE__, __LINE__, &vars[0], memory_order_acquire);
  int v10 = (v9 == 3);
  int v11 = atomic_load_explicit(__LINE__, __LINE__, &atom_1_r3_2, memory_order_acquire);
  int v12 = atomic_load_explicit(__LINE__, __LINE__, &atom_1_r7_2, memory_order_acquire);
  int v13_conj = v11 & v12;
  int v14_conj = v10 & v13_conj;
  int v15_conj = v8 & v14_conj;
  if (v15_conj == 1) MODEL_ASSERT(0);
  return 0;
}
