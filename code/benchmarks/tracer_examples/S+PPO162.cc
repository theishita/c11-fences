/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[4]; 
atomic<int> atom_1_r1_1; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, &vars[0], 2, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  atomic_store_explicit(__LINE__, &vars[1], 1, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v2_r1 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v4_r3 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v5_r4 = v4_r3 ^ v4_r3;
  int v8_r5 = atomic_load_explicit(__LINE__, &vars[2+v5_r4], memory_order_acquire);
  int v9_r7 = v8_r5 ^ v8_r5;
  atomic_store_explicit(__LINE__, &vars[3+v9_r7], 1, memory_order_release);
  atomic_store_explicit(__LINE__, &vars[3], 2, memory_order_release);
  int v11_r11 = atomic_load_explicit(__LINE__, &vars[3], memory_order_acquire);
  int v12_r12 = v11_r11 ^ v11_r11;
  atomic_store_explicit(__LINE__, &vars[0+v12_r12], 1, memory_order_release);
  int v20 = (v2_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r1_1, v20, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[0], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[2], 0);
  atomic_init(&vars[3], 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v13 = atomic_load_explicit(__LINE__, &vars[3], memory_order_acquire);
  int v14 = (v13 == 2);
  int v15 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v16 = (v15 == 2);
  int v17 = atomic_load_explicit(__LINE__, &atom_1_r1_1, memory_order_acquire);
  int v18_conj = v16 & v17;
  int v19_conj = v14 & v18_conj;
  if (v19_conj == 1) MODEL_ASSERT(0);
  return 0;
}
