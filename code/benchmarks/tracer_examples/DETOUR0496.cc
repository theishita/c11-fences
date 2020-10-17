/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[3]; 
atomic<int> atom_0_r3_3; 
atomic<int> atom_2_r5_2; 
atomic<int> atom_2_r1_1; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, &vars[0], 2, memory_order_release);
  int v2_r3 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v3_r4 = v2_r3 ^ v2_r3;
  int v6_r5 = atomic_load_explicit(__LINE__, &vars[1+v3_r4], memory_order_acquire);
  atomic_store_explicit(__LINE__, &vars[2], 1, memory_order_release);
  int v18 = (v2_r3 == 3);
  atomic_store_explicit(__LINE__, &atom_0_r3_3, v18, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(__LINE__, &vars[0], 3, memory_order_release);

}

void t2(void *arg){
label_3:;
  int v8_r1 = atomic_load_explicit(__LINE__, &vars[2], memory_order_acquire);
  int v9_r3 = v8_r1 ^ v8_r1;
  int v10_r3 = v9_r3 + 1;
  atomic_store_explicit(__LINE__, &vars[0], v10_r3, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v12_r5 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v19 = (v12_r5 == 2);
  atomic_store_explicit(__LINE__, &atom_2_r5_2, v19, memory_order_release);
  int v20 = (v8_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_2_r1_1, v20, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars[0], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[2], 0);
  atomic_init(&atom_0_r3_3, 0); 
  atomic_init(&atom_2_r5_2, 0); 
  atomic_init(&atom_2_r1_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v13 = atomic_load_explicit(__LINE__, &atom_0_r3_3, memory_order_acquire);
  int v14 = atomic_load_explicit(__LINE__, &atom_2_r5_2, memory_order_acquire);
  int v15 = atomic_load_explicit(__LINE__, &atom_2_r1_1, memory_order_acquire);
  int v16_conj = v14 & v15;
  int v17_conj = v13 & v16_conj;
  if (v17_conj == 1) MODEL_ASSERT(0);
  return 0;
}
