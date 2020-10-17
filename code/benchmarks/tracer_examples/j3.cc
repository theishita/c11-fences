/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[2]; 
atomic<int> atom_0_r2_2; 
atomic<int> atom_0_r4_2; 
atomic<int> atom_1_r1_1; 
atomic<int> atom_2_r1_3; 
atomic<int> atom_2_r3_0; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);
  int v2_r2 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v3_r3 = v2_r2 ^ v2_r2;
  int v6_r4 = atomic_load_explicit(__LINE__, &vars[1+v3_r3], memory_order_acquire);
  atomic_store_explicit(__LINE__, &vars[1], 3, memory_order_release);
  int v30 = (v2_r2 == 2);
  atomic_store_explicit(__LINE__, &atom_0_r2_2, v30, memory_order_release);
  int v31 = (v6_r4 == 2);
  atomic_store_explicit(__LINE__, &atom_0_r4_2, v31, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v8_r1 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  atomic_store_explicit(__LINE__, &vars[1], 2, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__LINE__, &vars[0], 2, memory_order_release);
  int v32 = (v8_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r1_1, v32, memory_order_release);

}

void t2(void *arg){
label_3:;
  int v10_r1 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v11_r2 = v10_r1 ^ v10_r1;
  int v14_r3 = atomic_load_explicit(__LINE__, &vars[0+v11_r2], memory_order_acquire);
  int v33 = (v10_r1 == 3);
  atomic_store_explicit(__LINE__, &atom_2_r1_3, v33, memory_order_release);
  int v34 = (v14_r3 == 0);
  atomic_store_explicit(__LINE__, &atom_2_r3_0, v34, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars[0], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&atom_0_r2_2, 0); 
  atomic_init(&atom_0_r4_2, 0); 
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_2_r1_3, 0); 
  atomic_init(&atom_2_r3_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v15 = atomic_load_explicit(__LINE__, &atom_0_r2_2, memory_order_acquire);
  int v16 = atomic_load_explicit(__LINE__, &atom_0_r4_2, memory_order_acquire);
  int v17 = atomic_load_explicit(__LINE__, &atom_1_r1_1, memory_order_acquire);
  int v18 = atomic_load_explicit(__LINE__, &atom_2_r1_3, memory_order_acquire);
  int v19 = atomic_load_explicit(__LINE__, &atom_2_r3_0, memory_order_acquire);
  int v20 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v21 = (v20 == 2);
  int v22 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v23 = (v22 == 3);
  int v24_conj = v21 & v23;
  int v25_conj = v19 & v24_conj;
  int v26_conj = v18 & v25_conj;
  int v27_conj = v17 & v26_conj;
  int v28_conj = v16 & v27_conj;
  int v29_conj = v15 & v28_conj;
  if (v29_conj == 1) MODEL_ASSERT(0);
  return 0;
}
