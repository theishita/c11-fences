/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[2]; 
atomic<int> atom_0_r3_2; 
atomic<int> atom_1_r1_1; 
atomic<int> atom_2_r1_3; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, &vars[0], 2, memory_order_release);
  int v2_r3 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  atomic_store_explicit(__LINE__, &vars[1], 1, memory_order_release);
  atomic_store_explicit(__LINE__, &vars[1], 3, memory_order_release);
  int v20 = (v2_r3 == 2);
  atomic_store_explicit(__LINE__, &atom_0_r3_2, v20, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v4_r1 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  atomic_store_explicit(__LINE__, &vars[1], 2, memory_order_release);
  int v21 = (v4_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r1_1, v21, memory_order_release);

}

void t2(void *arg){
label_3:;
  int v6_r1 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v7_r3 = v6_r1 ^ v6_r1;
  int v8_r3 = v7_r3 + 1;
  atomic_store_explicit(__LINE__, &vars[0], v8_r3, memory_order_release);
  int v22 = (v6_r1 == 3);
  atomic_store_explicit(__LINE__, &atom_2_r1_3, v22, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_0_r3_2, 0); 
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_2_r1_3, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v9 = atomic_load_explicit(__LINE__, &atom_0_r3_2, memory_order_acquire);
  int v10 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v11 = (v10 == 3);
  int v12 = atomic_load_explicit(__LINE__, &atom_1_r1_1, memory_order_acquire);
  int v13 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v14 = (v13 == 2);
  int v15 = atomic_load_explicit(__LINE__, &atom_2_r1_3, memory_order_acquire);
  int v16_conj = v14 & v15;
  int v17_conj = v12 & v16_conj;
  int v18_conj = v11 & v17_conj;
  int v19_conj = v9 & v18_conj;
  if (v19_conj == 1) MODEL_ASSERT(0);
  return 0;
}
