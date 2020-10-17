/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[2]; 
atomic<int> atom_0_r1_4; 
atomic<int> atom_1_r1_1; 
atomic<int> atom_1_r4_0; 
atomic<int> atom_2_r3_2; 
atomic<int> atom_3_r1_2; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  int v2_r1 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v3_r3 = v2_r1 ^ v2_r1;
  int v4_r3 = v3_r3 + 1;
  atomic_store_explicit(__LINE__, &vars[1], v4_r3, memory_order_release);
  int v27 = (v2_r1 == 4);
  atomic_store_explicit(__LINE__, &atom_0_r1_4, v27, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v6_r1 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v7_r3 = v6_r1 ^ v6_r1;
  int v10_r4 = atomic_load_explicit(__LINE__, &vars[0+v7_r3], memory_order_acquire);
  atomic_store_explicit(__LINE__, &vars[0], 2, memory_order_release);
  atomic_store_explicit(__LINE__, &vars[0], 4, memory_order_release);
  int v28 = (v6_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r1_1, v28, memory_order_release);
  int v29 = (v10_r4 == 0);
  atomic_store_explicit(__LINE__, &atom_1_r4_0, v29, memory_order_release);

}

void t2(void *arg){
label_3:;
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v12_r3 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v30 = (v12_r3 == 2);
  atomic_store_explicit(__LINE__, &atom_2_r3_2, v30, memory_order_release);

}

void t3(void *arg){
label_4:;
  int v14_r1 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  atomic_store_explicit(__LINE__, &vars[0], 3, memory_order_release);
  int v31 = (v14_r1 == 2);
  atomic_store_explicit(__LINE__, &atom_3_r1_2, v31, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 
  thrd_t thr3; 

  atomic_init(&vars[0], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&atom_0_r1_4, 0); 
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_1_r4_0, 0); 
  atomic_init(&atom_2_r3_2, 0); 
  atomic_init(&atom_3_r1_2, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);
  thrd_create(&thr3, t3, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);
  thrd_join(thr3);

  int v15 = atomic_load_explicit(__LINE__, &atom_0_r1_4, memory_order_acquire);
  int v16 = atomic_load_explicit(__LINE__, &atom_1_r1_1, memory_order_acquire);
  int v17 = atomic_load_explicit(__LINE__, &atom_1_r4_0, memory_order_acquire);
  int v18 = atomic_load_explicit(__LINE__, &atom_2_r3_2, memory_order_acquire);
  int v19 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v20 = (v19 == 4);
  int v21 = atomic_load_explicit(__LINE__, &atom_3_r1_2, memory_order_acquire);
  int v22_conj = v20 & v21;
  int v23_conj = v18 & v22_conj;
  int v24_conj = v17 & v23_conj;
  int v25_conj = v16 & v24_conj;
  int v26_conj = v15 & v25_conj;
  if (v26_conj == 1) MODEL_ASSERT(0);
  return 0;
}
