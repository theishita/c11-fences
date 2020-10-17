/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[5]; 
atomic<int> atom_1_r3_0; 
atomic<int> atom_3_r1_1; 
atomic<int> atom_3_r8_1; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, __LINE__, &vars[0], 2, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__LINE__, __LINE__, &vars[1], 1, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(__LINE__, __LINE__, &vars[1], 2, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v2_r3 = atomic_load_explicit(__LINE__, __LINE__, &vars[2], memory_order_acquire);
  int v22 = (v2_r3 == 0);
  atomic_store_explicit(__LINE__, __LINE__, &atom_1_r3_0, v22, memory_order_release);

}

void t2(void *arg){
label_3:;
  atomic_store_explicit(__LINE__, __LINE__, &vars[2], 1, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__LINE__, __LINE__, &vars[3], 1, memory_order_release);

}

void t3(void *arg){
label_4:;
  int v4_r1 = atomic_load_explicit(__LINE__, __LINE__, &vars[3], memory_order_acquire);
  int v5_r3 = v4_r1 ^ v4_r1;
  int v8_r4 = atomic_load_explicit(__LINE__, __LINE__, &vars[4+v5_r3], memory_order_acquire);
  atomic_store_explicit(__LINE__, __LINE__, &vars[0], 1, memory_order_release);
  int v10_r8 = atomic_load_explicit(__LINE__, __LINE__, &vars[0], memory_order_acquire);
  int v23 = (v4_r1 == 1);
  atomic_store_explicit(__LINE__, __LINE__, &atom_3_r1_1, v23, memory_order_release);
  int v24 = (v10_r8 == 1);
  atomic_store_explicit(__LINE__, __LINE__, &atom_3_r8_1, v24, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 
  thrd_t thr3; 

  atomic_init(&vars[3], 0);
  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&vars[4], 0);
  atomic_init(&atom_1_r3_0, 0); 
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

  int v11 = atomic_load_explicit(__LINE__, __LINE__, &vars[0], memory_order_acquire);
  int v12 = (v11 == 2);
  int v13 = atomic_load_explicit(__LINE__, __LINE__, &vars[1], memory_order_acquire);
  int v14 = (v13 == 2);
  int v15 = atomic_load_explicit(__LINE__, __LINE__, &atom_1_r3_0, memory_order_acquire);
  int v16 = atomic_load_explicit(__LINE__, __LINE__, &atom_3_r1_1, memory_order_acquire);
  int v17 = atomic_load_explicit(__LINE__, __LINE__, &atom_3_r8_1, memory_order_acquire);
  int v18_conj = v16 & v17;
  int v19_conj = v15 & v18_conj;
  int v20_conj = v14 & v19_conj;
  int v21_conj = v12 & v20_conj;
  if (v21_conj == 1) MODEL_ASSERT(0);
  return 0;
}
