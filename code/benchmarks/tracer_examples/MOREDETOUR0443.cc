/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[2]; 
atomic<int> atom_1_r1_1; 
atomic<int> atom_1_r4_3; 
atomic<int> atom_1_r6_0; 
atomic<int> atom_2_r1_1; 

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
  atomic_store_explicit(__LINE__, &vars[1], 3, memory_order_release);
  int v4_r4 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v5_r5 = v4_r4 ^ v4_r4;
  int v8_r6 = atomic_load_explicit(__LINE__, &vars[0+v5_r5], memory_order_acquire);
  int v21 = (v2_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r1_1, v21, memory_order_release);
  int v22 = (v4_r4 == 3);
  atomic_store_explicit(__LINE__, &atom_1_r4_3, v22, memory_order_release);
  int v23 = (v8_r6 == 0);
  atomic_store_explicit(__LINE__, &atom_1_r6_0, v23, memory_order_release);

}

void t2(void *arg){
label_3:;
  int v10_r1 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  atomic_store_explicit(__LINE__, &vars[1], 2, memory_order_release);
  int v24 = (v10_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_2_r1_1, v24, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars[0], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_1_r4_3, 0); 
  atomic_init(&atom_1_r6_0, 0); 
  atomic_init(&atom_2_r1_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v11 = atomic_load_explicit(__LINE__, &atom_1_r1_1, memory_order_acquire);
  int v12 = atomic_load_explicit(__LINE__, &atom_1_r4_3, memory_order_acquire);
  int v13 = atomic_load_explicit(__LINE__, &atom_1_r6_0, memory_order_acquire);
  int v14 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v15 = (v14 == 3);
  int v16 = atomic_load_explicit(__LINE__, &atom_2_r1_1, memory_order_acquire);
  int v17_conj = v15 & v16;
  int v18_conj = v13 & v17_conj;
  int v19_conj = v12 & v18_conj;
  int v20_conj = v11 & v19_conj;
  if (v20_conj == 1) MODEL_ASSERT(0);
  return 0;
}
