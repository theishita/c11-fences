/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[2]; 
atomic<int> atom_1_r1_1; 
atomic<int> atom_2_r1_1; 
atomic<int> atom_2_r4_0; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);
  atomic_store_explicit(__LINE__, &vars[0], 3, memory_order_release);
  int v2_r4 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v3_r5 = v2_r4 ^ v2_r4;
  int v4_r5 = v3_r5 + 1;
  atomic_store_explicit(__LINE__, &vars[1], v4_r5, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v6_r1 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  atomic_store_explicit(__LINE__, &vars[0], 2, memory_order_release);
  int v21 = (v6_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r1_1, v21, memory_order_release);

}

void t2(void *arg){
label_3:;
  int v8_r1 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v9_r3 = v8_r1 ^ v8_r1;
  int v12_r4 = atomic_load_explicit(__LINE__, &vars[0+v9_r3], memory_order_acquire);
  int v22 = (v8_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_2_r1_1, v22, memory_order_release);
  int v23 = (v12_r4 == 0);
  atomic_store_explicit(__LINE__, &atom_2_r4_0, v23, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_2_r1_1, 0); 
  atomic_init(&atom_2_r4_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v13 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v14 = (v13 == 3);
  int v15 = atomic_load_explicit(__LINE__, &atom_1_r1_1, memory_order_acquire);
  int v16 = atomic_load_explicit(__LINE__, &atom_2_r1_1, memory_order_acquire);
  int v17 = atomic_load_explicit(__LINE__, &atom_2_r4_0, memory_order_acquire);
  int v18_conj = v16 & v17;
  int v19_conj = v15 & v18_conj;
  int v20_conj = v14 & v19_conj;
  if (v20_conj == 1) MODEL_ASSERT(0);
  return 0;
}
