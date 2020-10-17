/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[4]; 
atomic<int> atom_0_r1_1; 
atomic<int> atom_1_r1_1; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  int v2_r1 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v3_r3 = v2_r1 ^ v2_r1;
  int v4_r3 = v3_r3 + 1;
  atomic_store_explicit(__LINE__, &vars[1], v4_r3, memory_order_release);
  int v23 = (v2_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_0_r1_1, v23, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v6_r1 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v8_r3 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v9_r4 = v8_r3 ^ v8_r3;
  int v12_r5 = atomic_load_explicit(__LINE__, &vars[2+v9_r4], memory_order_acquire);
  int v13_r7 = v12_r5 ^ v12_r5;
  int v14_r7 = v13_r7 + 1;
  atomic_store_explicit(__LINE__, &vars[3], v14_r7, memory_order_release);
  atomic_store_explicit(__LINE__, &vars[3], 2, memory_order_release);
  int v16_r10 = atomic_load_explicit(__LINE__, &vars[3], memory_order_acquire);
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);
  int v24 = (v6_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r1_1, v24, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[3], 0);
  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_0_r1_1, 0); 
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v17 = atomic_load_explicit(__LINE__, &vars[3], memory_order_acquire);
  int v18 = (v17 == 2);
  int v19 = atomic_load_explicit(__LINE__, &atom_0_r1_1, memory_order_acquire);
  int v20 = atomic_load_explicit(__LINE__, &atom_1_r1_1, memory_order_acquire);
  int v21_conj = v19 & v20;
  int v22_conj = v18 & v21_conj;
  if (v22_conj == 1) MODEL_ASSERT(0);
  return 0;
}
