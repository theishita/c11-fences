/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[4]; 
atomic<int> atom_0_r3_1; 
atomic<int> atom_1_r1_1; 
atomic<int> atom_2_r1_1; 
atomic<int> atom_2_r4_0; 
atomic<int> atom_2_r7_1; 
atomic<int> atom_2_r9_0; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);
  int v2_r3 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v3_r4 = v2_r3 ^ v2_r3;
  int v4_r4 = v3_r4 + 1;
  atomic_store_explicit(__LINE__, &vars[1], v4_r4, memory_order_release);
  int v32 = (v2_r3 == 1);
  atomic_store_explicit(__LINE__, &atom_0_r3_1, v32, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v6_r1 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v7_r3 = v6_r1 ^ v6_r1;
  int v8_r3 = v7_r3 + 1;
  atomic_store_explicit(__LINE__, &vars[2], v8_r3, memory_order_release);
  int v33 = (v6_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r1_1, v33, memory_order_release);

}

void t2(void *arg){
label_3:;
  int v10_r1 = atomic_load_explicit(__LINE__, &vars[2], memory_order_acquire);
  int v11_r3 = v10_r1 ^ v10_r1;
  int v14_r4 = atomic_load_explicit(__LINE__, &vars[3+v11_r3], memory_order_acquire);
  atomic_store_explicit(__LINE__, &vars[3], 1, memory_order_release);
  int v16_r7 = atomic_load_explicit(__LINE__, &vars[3], memory_order_acquire);
  int v17_r8 = v16_r7 ^ v16_r7;
  int v20_r9 = atomic_load_explicit(__LINE__, &vars[0+v17_r8], memory_order_acquire);
  int v34 = (v10_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_2_r1_1, v34, memory_order_release);
  int v35 = (v14_r4 == 0);
  atomic_store_explicit(__LINE__, &atom_2_r4_0, v35, memory_order_release);
  int v36 = (v16_r7 == 1);
  atomic_store_explicit(__LINE__, &atom_2_r7_1, v36, memory_order_release);
  int v37 = (v20_r9 == 0);
  atomic_store_explicit(__LINE__, &atom_2_r9_0, v37, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars[3], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[2], 0);
  atomic_init(&atom_0_r3_1, 0); 
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_2_r1_1, 0); 
  atomic_init(&atom_2_r4_0, 0); 
  atomic_init(&atom_2_r7_1, 0); 
  atomic_init(&atom_2_r9_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v21 = atomic_load_explicit(__LINE__, &atom_0_r3_1, memory_order_acquire);
  int v22 = atomic_load_explicit(__LINE__, &atom_1_r1_1, memory_order_acquire);
  int v23 = atomic_load_explicit(__LINE__, &atom_2_r1_1, memory_order_acquire);
  int v24 = atomic_load_explicit(__LINE__, &atom_2_r4_0, memory_order_acquire);
  int v25 = atomic_load_explicit(__LINE__, &atom_2_r7_1, memory_order_acquire);
  int v26 = atomic_load_explicit(__LINE__, &atom_2_r9_0, memory_order_acquire);
  int v27_conj = v25 & v26;
  int v28_conj = v24 & v27_conj;
  int v29_conj = v23 & v28_conj;
  int v30_conj = v22 & v29_conj;
  int v31_conj = v21 & v30_conj;
  if (v31_conj == 1) MODEL_ASSERT(0);
  return 0;
}
