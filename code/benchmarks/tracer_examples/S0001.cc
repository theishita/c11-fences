/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[4]; 
atomic<int> atom_1_r1_1; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, __LINE__, &vars[0], 2, memory_order_release);
  int v2_r3 = atomic_load_explicit(__LINE__, __LINE__, &vars[1], memory_order_acquire);
  int v3_r5 = v2_r3 ^ v2_r3;
  int v6_r6 = atomic_load_explicit(__LINE__, __LINE__, &vars[2+v3_r5], memory_order_acquire);
  int v7_r8 = v6_r6 ^ v6_r6;
  atomic_store_explicit(__LINE__, __LINE__, &vars[3+v7_r8], 1, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v9_r1 = atomic_load_explicit(__LINE__, __LINE__, &vars[3], memory_order_acquire);
  int v10_r3 = v9_r1 ^ v9_r1;
  int v11_r3 = v10_r3 + 1;
  atomic_store_explicit(__LINE__, __LINE__, &vars[0], v11_r3, memory_order_release);
  int v16 = (v9_r1 == 1);
  atomic_store_explicit(__LINE__, __LINE__, &atom_1_r1_1, v16, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[2], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&vars[3], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v12 = atomic_load_explicit(__LINE__, __LINE__, &vars[0], memory_order_acquire);
  int v13 = (v12 == 2);
  int v14 = atomic_load_explicit(__LINE__, __LINE__, &atom_1_r1_1, memory_order_acquire);
  int v15_conj = v13 & v14;
  if (v15_conj == 1) MODEL_ASSERT(0);
  return 0;
}
