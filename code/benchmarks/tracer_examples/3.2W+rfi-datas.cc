/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[3]; 
atomic<int> atom_0_r3_2; 
atomic<int> atom_1_r3_2; 
atomic<int> atom_2_r3_2; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, &vars[0], 2, memory_order_release);
  int v2_r3 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v3_r4 = v2_r3 ^ v2_r3;
  int v4_r4 = v3_r4 + 1;
  atomic_store_explicit(__LINE__, &vars[1], v4_r4, memory_order_release);
  int v27 = (v2_r3 == 2);
  atomic_store_explicit(__LINE__, &atom_0_r3_2, v27, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(__LINE__, &vars[1], 2, memory_order_release);
  int v6_r3 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v7_r4 = v6_r3 ^ v6_r3;
  int v8_r4 = v7_r4 + 1;
  atomic_store_explicit(__LINE__, &vars[2], v8_r4, memory_order_release);
  int v28 = (v6_r3 == 2);
  atomic_store_explicit(__LINE__, &atom_1_r3_2, v28, memory_order_release);

}

void t2(void *arg){
label_3:;
  atomic_store_explicit(__LINE__, &vars[2], 2, memory_order_release);
  int v10_r3 = atomic_load_explicit(__LINE__, &vars[2], memory_order_acquire);
  int v11_r4 = v10_r3 ^ v10_r3;
  int v12_r4 = v11_r4 + 1;
  atomic_store_explicit(__LINE__, &vars[0], v12_r4, memory_order_release);
  int v29 = (v10_r3 == 2);
  atomic_store_explicit(__LINE__, &atom_2_r3_2, v29, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars[0], 0);
  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&atom_0_r3_2, 0); 
  atomic_init(&atom_1_r3_2, 0); 
  atomic_init(&atom_2_r3_2, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v13 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v14 = (v13 == 2);
  int v15 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v16 = (v15 == 2);
  int v17 = atomic_load_explicit(__LINE__, &vars[2], memory_order_acquire);
  int v18 = (v17 == 2);
  int v19 = atomic_load_explicit(__LINE__, &atom_0_r3_2, memory_order_acquire);
  int v20 = atomic_load_explicit(__LINE__, &atom_1_r3_2, memory_order_acquire);
  int v21 = atomic_load_explicit(__LINE__, &atom_2_r3_2, memory_order_acquire);
  int v22_conj = v20 & v21;
  int v23_conj = v19 & v22_conj;
  int v24_conj = v18 & v23_conj;
  int v25_conj = v16 & v24_conj;
  int v26_conj = v14 & v25_conj;
  if (v26_conj == 1) MODEL_ASSERT(0);
  return 0;
}
