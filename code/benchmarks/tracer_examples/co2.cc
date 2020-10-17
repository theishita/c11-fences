/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[1]; 
atomic<int> atom_2_r1_1; 
atomic<int> atom_2_r2_2; 
atomic<int> atom_3_r1_2; 
atomic<int> atom_3_r2_1; 
atomic<int> atom_2_r1_2; 
atomic<int> atom_2_r2_1; 
atomic<int> atom_3_r1_1; 
atomic<int> atom_3_r2_2; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(__LINE__, &vars[0], 2, memory_order_release);

}

void t2(void *arg){
label_3:;
  int v2_r1 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v4_r2 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v24 = (v2_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_2_r1_1, v24, memory_order_release);
  int v25 = (v4_r2 == 2);
  atomic_store_explicit(__LINE__, &atom_2_r2_2, v25, memory_order_release);
  int v28 = (v2_r1 == 2);
  atomic_store_explicit(__LINE__, &atom_2_r1_2, v28, memory_order_release);
  int v29 = (v4_r2 == 1);
  atomic_store_explicit(__LINE__, &atom_2_r2_1, v29, memory_order_release);

}

void t3(void *arg){
label_4:;
  int v6_r1 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v8_r2 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v26 = (v6_r1 == 2);
  atomic_store_explicit(__LINE__, &atom_3_r1_2, v26, memory_order_release);
  int v27 = (v8_r2 == 1);
  atomic_store_explicit(__LINE__, &atom_3_r2_1, v27, memory_order_release);
  int v30 = (v6_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_3_r1_1, v30, memory_order_release);
  int v31 = (v8_r2 == 2);
  atomic_store_explicit(__LINE__, &atom_3_r2_2, v31, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 
  thrd_t thr3; 

  atomic_init(&vars[0], 0);
  atomic_init(&atom_2_r1_1, 0); 
  atomic_init(&atom_2_r2_2, 0); 
  atomic_init(&atom_3_r1_2, 0); 
  atomic_init(&atom_3_r2_1, 0); 
  atomic_init(&atom_2_r1_2, 0); 
  atomic_init(&atom_2_r2_1, 0); 
  atomic_init(&atom_3_r1_1, 0); 
  atomic_init(&atom_3_r2_2, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);
  thrd_create(&thr3, t3, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);
  thrd_join(thr3);

  int v9 = atomic_load_explicit(__LINE__, &atom_2_r1_1, memory_order_acquire);
  int v10 = atomic_load_explicit(__LINE__, &atom_2_r2_2, memory_order_acquire);
  int v11 = atomic_load_explicit(__LINE__, &atom_3_r1_2, memory_order_acquire);
  int v12 = atomic_load_explicit(__LINE__, &atom_3_r2_1, memory_order_acquire);
  int v13_conj = v11 & v12;
  int v14_conj = v10 & v13_conj;
  int v15_conj = v9 & v14_conj;
  int v16 = atomic_load_explicit(__LINE__, &atom_2_r1_2, memory_order_acquire);
  int v17 = atomic_load_explicit(__LINE__, &atom_2_r2_1, memory_order_acquire);
  int v18 = atomic_load_explicit(__LINE__, &atom_3_r1_1, memory_order_acquire);
  int v19 = atomic_load_explicit(__LINE__, &atom_3_r2_2, memory_order_acquire);
  int v20_conj = v18 & v19;
  int v21_conj = v17 & v20_conj;
  int v22_conj = v16 & v21_conj;
  int v23_disj = v15_conj | v22_conj;
  if (v23_disj == 1) MODEL_ASSERT(0);
  return 0;
}
