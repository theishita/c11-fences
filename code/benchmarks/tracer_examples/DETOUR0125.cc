/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[3]; 
atomic<int> atom_0_r3_2; 
atomic<int> atom_2_r1_1; 
atomic<int> atom_2_r4_0; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);
  int v2_r3 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v3_r4 = v2_r3 ^ v2_r3;
  int v4_r4 = v3_r4 + 1;
  atomic_store_explicit(__LINE__, &vars[1], v4_r4, memory_order_release);
  atomic_store_explicit(__LINE__, &vars[2], 1, memory_order_release);
  int v16 = (v2_r3 == 2);
  atomic_store_explicit(__LINE__, &atom_0_r3_2, v16, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(__LINE__, &vars[0], 2, memory_order_release);

}

void t2(void *arg){
label_3:;
  int v6_r1 = atomic_load_explicit(__LINE__, &vars[2], memory_order_acquire);
  int v7_r3 = v6_r1 ^ v6_r1;
  int v10_r4 = atomic_load_explicit(__LINE__, &vars[0+v7_r3], memory_order_acquire);
  int v17 = (v6_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_2_r1_1, v17, memory_order_release);
  int v18 = (v10_r4 == 0);
  atomic_store_explicit(__LINE__, &atom_2_r4_0, v18, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars[0], 0);
  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&atom_0_r3_2, 0); 
  atomic_init(&atom_2_r1_1, 0); 
  atomic_init(&atom_2_r4_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v11 = atomic_load_explicit(__LINE__, &atom_0_r3_2, memory_order_acquire);
  int v12 = atomic_load_explicit(__LINE__, &atom_2_r1_1, memory_order_acquire);
  int v13 = atomic_load_explicit(__LINE__, &atom_2_r4_0, memory_order_acquire);
  int v14_conj = v12 & v13;
  int v15_conj = v11 & v14_conj;
  if (v15_conj == 1) MODEL_ASSERT(0);
  return 0;
}
