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
atomic<int> atom_1_r4_0; 
atomic<int> atom_1_r7_1; 
atomic<int> atom_1_r9_0; 
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
  int v36 = (v2_r3 == 1);
  atomic_store_explicit(__LINE__, &atom_0_r3_1, v36, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v6_r1 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v7_r3 = v6_r1 ^ v6_r1;
  int v10_r4 = atomic_load_explicit(__LINE__, &vars[2+v7_r3], memory_order_acquire);
  atomic_store_explicit(__LINE__, &vars[2], 1, memory_order_release);
  int v12_r7 = atomic_load_explicit(__LINE__, &vars[2], memory_order_acquire);
  int v13_r8 = v12_r7 ^ v12_r7;
  int v16_r9 = atomic_load_explicit(__LINE__, &vars[3+v13_r8], memory_order_acquire);
  int v37 = (v6_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r1_1, v37, memory_order_release);
  int v38 = (v10_r4 == 0);
  atomic_store_explicit(__LINE__, &atom_1_r4_0, v38, memory_order_release);
  int v39 = (v12_r7 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r7_1, v39, memory_order_release);
  int v40 = (v16_r9 == 0);
  atomic_store_explicit(__LINE__, &atom_1_r9_0, v40, memory_order_release);

}

void t2(void *arg){
label_3:;
  atomic_store_explicit(__LINE__, &vars[3], 1, memory_order_release);
  int v18_r3 = atomic_load_explicit(__LINE__, &vars[3], memory_order_acquire);
  int v19_r4 = v18_r3 ^ v18_r3;
  int v22_r5 = atomic_load_explicit(__LINE__, &vars[0+v19_r4], memory_order_acquire);
  int v41 = (v18_r3 == 1);
  atomic_store_explicit(__LINE__, &atom_2_r3_1, v41, memory_order_release);
  int v42 = (v22_r5 == 0);
  atomic_store_explicit(__LINE__, &atom_2_r5_0, v42, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars[0], 0);
  atomic_init(&vars[3], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[2], 0);
  atomic_init(&atom_0_r3_1, 0); 
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_1_r4_0, 0); 
  atomic_init(&atom_1_r7_1, 0); 
  atomic_init(&atom_1_r9_0, 0); 
  atomic_init(&atom_2_r3_1, 0); 
  atomic_init(&atom_2_r5_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v23 = atomic_load_explicit(__LINE__, &atom_0_r3_1, memory_order_acquire);
  int v24 = atomic_load_explicit(__LINE__, &atom_1_r1_1, memory_order_acquire);
  int v25 = atomic_load_explicit(__LINE__, &atom_1_r4_0, memory_order_acquire);
  int v26 = atomic_load_explicit(__LINE__, &atom_1_r7_1, memory_order_acquire);
  int v27 = atomic_load_explicit(__LINE__, &atom_1_r9_0, memory_order_acquire);
  int v28 = atomic_load_explicit(__LINE__, &atom_2_r3_1, memory_order_acquire);
  int v29 = atomic_load_explicit(__LINE__, &atom_2_r5_0, memory_order_acquire);
  int v30_conj = v28 & v29;
  int v31_conj = v27 & v30_conj;
  int v32_conj = v26 & v31_conj;
  int v33_conj = v25 & v32_conj;
  int v34_conj = v24 & v33_conj;
  int v35_conj = v23 & v34_conj;
  if (v35_conj == 1) MODEL_ASSERT(0);
  return 0;
}
