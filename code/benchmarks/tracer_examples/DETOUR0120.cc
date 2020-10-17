/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[3]; 
atomic<int> atom_0_r3_0; 
atomic<int> atom_1_r1_1; 
atomic<int> atom_1_r4_0; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, __LINE__, &vars[0], 1, memory_order_release);
  int v2_r3 = atomic_load_explicit(__LINE__, __LINE__, &vars[1], memory_order_acquire);
  atomic_store_explicit(__LINE__, __LINE__, &vars[1], 1, memory_order_release);
  atomic_store_explicit(__LINE__, __LINE__, &vars[2], 1, memory_order_release);
  int v14 = (v2_r3 == 0);
  atomic_store_explicit(__LINE__, __LINE__, &atom_0_r3_0, v14, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v4_r1 = atomic_load_explicit(__LINE__, __LINE__, &vars[2], memory_order_acquire);
  int v5_r3 = v4_r1 ^ v4_r1;
  int v8_r4 = atomic_load_explicit(__LINE__, __LINE__, &vars[0+v5_r3], memory_order_acquire);
  int v15 = (v4_r1 == 1);
  atomic_store_explicit(__LINE__, __LINE__, &atom_1_r1_1, v15, memory_order_release);
  int v16 = (v8_r4 == 0);
  atomic_store_explicit(__LINE__, __LINE__, &atom_1_r4_0, v16, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&vars[2], 0);
  atomic_init(&atom_0_r3_0, 0); 
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_1_r4_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v9 = atomic_load_explicit(__LINE__, __LINE__, &atom_0_r3_0, memory_order_acquire);
  int v10 = atomic_load_explicit(__LINE__, __LINE__, &atom_1_r1_1, memory_order_acquire);
  int v11 = atomic_load_explicit(__LINE__, __LINE__, &atom_1_r4_0, memory_order_acquire);
  int v12_conj = v10 & v11;
  int v13_conj = v9 & v12_conj;
  if (v13_conj == 1) MODEL_ASSERT(0);
  return 0;
}
