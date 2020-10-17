/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[2]; 
atomic<int> atom_1_r1_1; 
atomic<int> atom_2_r3_0; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, __LINE__, &vars[0], 1, memory_order_release);
  atomic_store_explicit(__LINE__, __LINE__, &vars[0], 3, memory_order_release);
  int v2_r4 = atomic_load_explicit(__LINE__, __LINE__, &vars[0], memory_order_acquire);
  int v3_r5 = v2_r4 ^ v2_r4;
  int v4_r5 = v3_r5 + 1;
  atomic_store_explicit(__LINE__, __LINE__, &vars[1], v4_r5, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v6_r1 = atomic_load_explicit(__LINE__, __LINE__, &vars[0], memory_order_acquire);
  atomic_store_explicit(__LINE__, __LINE__, &vars[0], 2, memory_order_release);
  int v18 = (v6_r1 == 1);
  atomic_store_explicit(__LINE__, __LINE__, &atom_1_r1_1, v18, memory_order_release);

}

void t2(void *arg){
label_3:;
  atomic_store_explicit(__LINE__, __LINE__, &vars[1], 2, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v8_r3 = atomic_load_explicit(__LINE__, __LINE__, &vars[0], memory_order_acquire);
  int v19 = (v8_r3 == 0);
  atomic_store_explicit(__LINE__, __LINE__, &atom_2_r3_0, v19, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_2_r3_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v9 = atomic_load_explicit(__LINE__, __LINE__, &vars[1], memory_order_acquire);
  int v10 = (v9 == 2);
  int v11 = atomic_load_explicit(__LINE__, __LINE__, &vars[0], memory_order_acquire);
  int v12 = (v11 == 3);
  int v13 = atomic_load_explicit(__LINE__, __LINE__, &atom_1_r1_1, memory_order_acquire);
  int v14 = atomic_load_explicit(__LINE__, __LINE__, &atom_2_r3_0, memory_order_acquire);
  int v15_conj = v13 & v14;
  int v16_conj = v12 & v15_conj;
  int v17_conj = v10 & v16_conj;
  if (v17_conj == 1) MODEL_ASSERT(0);
  return 0;
}
