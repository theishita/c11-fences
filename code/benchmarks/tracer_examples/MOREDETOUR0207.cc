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
atomic<int> atom_1_r4_0; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  int v2_r1 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v3_r3 = v2_r1 ^ v2_r1;
  int v4_r3 = v3_r3 + 1;
  atomic_store_explicit(__LINE__, &vars[1], v4_r3, memory_order_release);
  int v17 = (v2_r1 == 2);
  atomic_store_explicit(__LINE__, &atom_0_r1_2, v17, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v6_r1 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  atomic_store_explicit(__LINE__, &vars[1], 2, memory_order_release);
  int v8_r4 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  atomic_store_explicit(__LINE__, &vars[0], 2, memory_order_release);
  int v18 = (v6_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r1_1, v18, memory_order_release);
  int v19 = (v8_r4 == 0);
  atomic_store_explicit(__LINE__, &atom_1_r4_0, v19, memory_order_release);

}

void t2(void *arg){
label_3:;
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_0_r1_2, 0); 
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_1_r4_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v9 = atomic_load_explicit(__LINE__, &atom_0_r1_2, memory_order_acquire);
  int v10 = atomic_load_explicit(__LINE__, &atom_1_r1_1, memory_order_acquire);
  int v11 = atomic_load_explicit(__LINE__, &atom_1_r4_0, memory_order_acquire);
  int v12 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v13 = (v12 == 2);
  int v14_conj = v11 & v13;
  int v15_conj = v10 & v14_conj;
  int v16_conj = v9 & v15_conj;
  if (v16_conj == 1) MODEL_ASSERT(0);
  return 0;
}
