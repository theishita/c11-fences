/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[4]; 
atomic<int> atom_2_r1_1; 
atomic<int> atom_2_r3_0; 
atomic<int> atom_2_r4_1; 
atomic<int> atom_3_r1_1; 
atomic<int> atom_3_r3_0; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(__LINE__, &vars[1], 1, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__LINE__, &vars[2], 1, memory_order_release);

}

void t2(void *arg){
label_3:;
  int v2_r1 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v3_r2 = v2_r1 ^ v2_r1;
  int v6_r3 = atomic_load_explicit(__LINE__, &vars[1+v3_r2], memory_order_acquire);
  int v8_r4 = atomic_load_explicit(__LINE__, &vars[2], memory_order_acquire);
  int v9_r5 = v8_r4 ^ v8_r4;
  int v10_r5 = v9_r5 + 1;
  atomic_store_explicit(__LINE__, &vars[3], v10_r5, memory_order_release);
  int v26 = (v2_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_2_r1_1, v26, memory_order_release);
  int v27 = (v6_r3 == 0);
  atomic_store_explicit(__LINE__, &atom_2_r3_0, v27, memory_order_release);
  int v28 = (v8_r4 == 1);
  atomic_store_explicit(__LINE__, &atom_2_r4_1, v28, memory_order_release);

}

void t3(void *arg){
label_4:;
  int v12_r1 = atomic_load_explicit(__LINE__, &vars[3], memory_order_acquire);
  int v13_r2 = v12_r1 ^ v12_r1;
  int v16_r3 = atomic_load_explicit(__LINE__, &vars[0+v13_r2], memory_order_acquire);
  int v29 = (v12_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_3_r1_1, v29, memory_order_release);
  int v30 = (v16_r3 == 0);
  atomic_store_explicit(__LINE__, &atom_3_r3_0, v30, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 
  thrd_t thr3; 

  atomic_init(&vars[3], 0);
  atomic_init(&vars[2], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&atom_2_r1_1, 0); 
  atomic_init(&atom_2_r3_0, 0); 
  atomic_init(&atom_2_r4_1, 0); 
  atomic_init(&atom_3_r1_1, 0); 
  atomic_init(&atom_3_r3_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);
  thrd_create(&thr3, t3, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);
  thrd_join(thr3);

  int v17 = atomic_load_explicit(__LINE__, &atom_2_r1_1, memory_order_acquire);
  int v18 = atomic_load_explicit(__LINE__, &atom_2_r3_0, memory_order_acquire);
  int v19 = atomic_load_explicit(__LINE__, &atom_2_r4_1, memory_order_acquire);
  int v20 = atomic_load_explicit(__LINE__, &atom_3_r1_1, memory_order_acquire);
  int v21 = atomic_load_explicit(__LINE__, &atom_3_r3_0, memory_order_acquire);
  int v22_conj = v20 & v21;
  int v23_conj = v19 & v22_conj;
  int v24_conj = v18 & v23_conj;
  int v25_conj = v17 & v24_conj;
  if (v25_conj == 1) MODEL_ASSERT(0);
  return 0;
}
