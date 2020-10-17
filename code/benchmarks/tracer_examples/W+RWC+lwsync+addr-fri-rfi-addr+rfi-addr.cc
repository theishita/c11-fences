/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[4]; 
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
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__LINE__, &vars[1], 1, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v2_r1 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v3_r3 = v2_r1 ^ v2_r1;
  int v6_r4 = atomic_load_explicit(__LINE__, &vars[2+v3_r3], memory_order_acquire);
  atomic_store_explicit(__LINE__, &vars[2], 1, memory_order_release);
  int v8_r7 = atomic_load_explicit(__LINE__, &vars[2], memory_order_acquire);
  int v9_r8 = v8_r7 ^ v8_r7;
  int v12_r9 = atomic_load_explicit(__LINE__, &vars[3+v9_r8], memory_order_acquire);
  int v30 = (v2_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r1_1, v30, memory_order_release);
  int v31 = (v6_r4 == 0);
  atomic_store_explicit(__LINE__, &atom_1_r4_0, v31, memory_order_release);
  int v32 = (v8_r7 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r7_1, v32, memory_order_release);
  int v33 = (v12_r9 == 0);
  atomic_store_explicit(__LINE__, &atom_1_r9_0, v33, memory_order_release);

}

void t2(void *arg){
label_3:;
  atomic_store_explicit(__LINE__, &vars[3], 1, memory_order_release);
  int v14_r3 = atomic_load_explicit(__LINE__, &vars[3], memory_order_acquire);
  int v15_r4 = v14_r3 ^ v14_r3;
  int v18_r5 = atomic_load_explicit(__LINE__, &vars[0+v15_r4], memory_order_acquire);
  int v34 = (v14_r3 == 1);
  atomic_store_explicit(__LINE__, &atom_2_r3_1, v34, memory_order_release);
  int v35 = (v18_r5 == 0);
  atomic_store_explicit(__LINE__, &atom_2_r5_0, v35, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars[2], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&vars[3], 0);
  atomic_init(&vars[1], 0);
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

  int v19 = atomic_load_explicit(__LINE__, &atom_1_r1_1, memory_order_acquire);
  int v20 = atomic_load_explicit(__LINE__, &atom_1_r4_0, memory_order_acquire);
  int v21 = atomic_load_explicit(__LINE__, &atom_1_r7_1, memory_order_acquire);
  int v22 = atomic_load_explicit(__LINE__, &atom_1_r9_0, memory_order_acquire);
  int v23 = atomic_load_explicit(__LINE__, &atom_2_r3_1, memory_order_acquire);
  int v24 = atomic_load_explicit(__LINE__, &atom_2_r5_0, memory_order_acquire);
  int v25_conj = v23 & v24;
  int v26_conj = v22 & v25_conj;
  int v27_conj = v21 & v26_conj;
  int v28_conj = v20 & v27_conj;
  int v29_conj = v19 & v28_conj;
  if (v29_conj == 1) MODEL_ASSERT(0);
  return 0;
}
