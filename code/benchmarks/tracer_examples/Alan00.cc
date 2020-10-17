/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[5]; 
atomic<int> atom_2_r6_2; 
atomic<int> atom_2_r7_0; 
atomic<int> atom_2_r8_1; 
atomic<int> atom_3_r6_1; 
atomic<int> atom_3_r7_0; 
atomic<int> atom_3_r8_1; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__LINE__, &vars[1], 1, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__LINE__, &vars[2], 1, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(__LINE__, &vars[3], 2, memory_order_release);

}

void t2(void *arg){
label_3:;
  int v2_r6 = atomic_load_explicit(__LINE__, &vars[3], memory_order_acquire);
  int v3_r7 = v2_r6 ^ v2_r6;
  int v6_r7 = atomic_load_explicit(__LINE__, &vars[0+v3_r7], memory_order_acquire);
  int v8_r8 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v9_r9 = v8_r8 ^ v8_r8;
  atomic_store_explicit(__LINE__, &vars[4+v9_r9], 1, memory_order_release);
  int v33 = (v2_r6 == 2);
  atomic_store_explicit(__LINE__, &atom_2_r6_2, v33, memory_order_release);
  int v34 = (v6_r7 == 0);
  atomic_store_explicit(__LINE__, &atom_2_r7_0, v34, memory_order_release);
  int v35 = (v8_r8 == 1);
  atomic_store_explicit(__LINE__, &atom_2_r8_1, v35, memory_order_release);

}

void t3(void *arg){
label_4:;
  int v11_r6 = atomic_load_explicit(__LINE__, &vars[4], memory_order_acquire);
  int v12_r7 = v11_r6 ^ v11_r6;
  int v15_r7 = atomic_load_explicit(__LINE__, &vars[1+v12_r7], memory_order_acquire);
  int v17_r8 = atomic_load_explicit(__LINE__, &vars[2], memory_order_acquire);
  int v18_r9 = v17_r8 ^ v17_r8;
  atomic_store_explicit(__LINE__, &vars[3+v18_r9], 1, memory_order_release);
  int v36 = (v11_r6 == 1);
  atomic_store_explicit(__LINE__, &atom_3_r6_1, v36, memory_order_release);
  int v37 = (v15_r7 == 0);
  atomic_store_explicit(__LINE__, &atom_3_r7_0, v37, memory_order_release);
  int v38 = (v17_r8 == 1);
  atomic_store_explicit(__LINE__, &atom_3_r8_1, v38, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 
  thrd_t thr3; 

  atomic_init(&vars[3], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[4], 0);
  atomic_init(&vars[2], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_2_r6_2, 0); 
  atomic_init(&atom_2_r7_0, 0); 
  atomic_init(&atom_2_r8_1, 0); 
  atomic_init(&atom_3_r6_1, 0); 
  atomic_init(&atom_3_r7_0, 0); 
  atomic_init(&atom_3_r8_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);
  thrd_create(&thr3, t3, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);
  thrd_join(thr3);

  int v19 = atomic_load_explicit(__LINE__, &vars[3], memory_order_acquire);
  int v20 = (v19 == 2);
  int v21 = atomic_load_explicit(__LINE__, &atom_2_r6_2, memory_order_acquire);
  int v22 = atomic_load_explicit(__LINE__, &atom_2_r7_0, memory_order_acquire);
  int v23 = atomic_load_explicit(__LINE__, &atom_2_r8_1, memory_order_acquire);
  int v24 = atomic_load_explicit(__LINE__, &atom_3_r6_1, memory_order_acquire);
  int v25 = atomic_load_explicit(__LINE__, &atom_3_r7_0, memory_order_acquire);
  int v26 = atomic_load_explicit(__LINE__, &atom_3_r8_1, memory_order_acquire);
  int v27_conj = v25 & v26;
  int v28_conj = v24 & v27_conj;
  int v29_conj = v23 & v28_conj;
  int v30_conj = v22 & v29_conj;
  int v31_conj = v21 & v30_conj;
  int v32_conj = v20 & v31_conj;
  if (v32_conj == 1) MODEL_ASSERT(0);
  return 0;
}
