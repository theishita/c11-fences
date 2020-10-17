/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[3]; 
atomic<int> atom_0_r3_1; 
atomic<int> atom_1_r3_2; 
atomic<int> atom_1_r5_0; 
atomic<int> atom_2_r3_1; 
atomic<int> atom_2_r5_0; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);
  int v2_r3 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v3_r4 = v2_r3 ^ v2_r3;
  int v4_r4 = v3_r4 + 1;
  atomic_store_explicit(__LINE__, &vars[1], v4_r4, memory_order_release);
  int v29 = (v2_r3 == 1);
  atomic_store_explicit(__LINE__, &atom_0_r3_1, v29, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(__LINE__, &vars[1], 2, memory_order_release);
  int v6_r3 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v7_r4 = v6_r3 ^ v6_r3;
  int v10_r5 = atomic_load_explicit(__LINE__, &vars[2+v7_r4], memory_order_acquire);
  int v30 = (v6_r3 == 2);
  atomic_store_explicit(__LINE__, &atom_1_r3_2, v30, memory_order_release);
  int v31 = (v10_r5 == 0);
  atomic_store_explicit(__LINE__, &atom_1_r5_0, v31, memory_order_release);

}

void t2(void *arg){
label_3:;
  atomic_store_explicit(__LINE__, &vars[2], 1, memory_order_release);
  int v12_r3 = atomic_load_explicit(__LINE__, &vars[2], memory_order_acquire);
  int v13_r4 = v12_r3 ^ v12_r3;
  int v16_r5 = atomic_load_explicit(__LINE__, &vars[0+v13_r4], memory_order_acquire);
  int v32 = (v12_r3 == 1);
  atomic_store_explicit(__LINE__, &atom_2_r3_1, v32, memory_order_release);
  int v33 = (v16_r5 == 0);
  atomic_store_explicit(__LINE__, &atom_2_r5_0, v33, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[2], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_0_r3_1, 0); 
  atomic_init(&atom_1_r3_2, 0); 
  atomic_init(&atom_1_r5_0, 0); 
  atomic_init(&atom_2_r3_1, 0); 
  atomic_init(&atom_2_r5_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v17 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v18 = (v17 == 2);
  int v19 = atomic_load_explicit(__LINE__, &atom_0_r3_1, memory_order_acquire);
  int v20 = atomic_load_explicit(__LINE__, &atom_1_r3_2, memory_order_acquire);
  int v21 = atomic_load_explicit(__LINE__, &atom_1_r5_0, memory_order_acquire);
  int v22 = atomic_load_explicit(__LINE__, &atom_2_r3_1, memory_order_acquire);
  int v23 = atomic_load_explicit(__LINE__, &atom_2_r5_0, memory_order_acquire);
  int v24_conj = v22 & v23;
  int v25_conj = v21 & v24_conj;
  int v26_conj = v20 & v25_conj;
  int v27_conj = v19 & v26_conj;
  int v28_conj = v18 & v27_conj;
  if (v28_conj == 1) MODEL_ASSERT(0);
  return 0;
}
