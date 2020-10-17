/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[5]; 
atomic<int> atom_0_r1_1; 
atomic<int> atom_1_r1_1; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  int v2_r1 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v3_r3 = v2_r1 ^ v2_r1;
  int v4_r3 = v3_r3 + 1;
  atomic_store_explicit(__LINE__, &vars[1], v4_r3, memory_order_release);
  int v20 = (v2_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_0_r1_1, v20, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v6_r1 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v8_r3 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v9_cmpeq = (v8_r3 == v8_r3);
  if (v9_cmpeq)  goto lbl_LC00; else goto lbl_LC00;
lbl_LC00:;
  atomic_store_explicit(__LINE__, &vars[2], 1, memory_order_release);
  int v11_r6 = atomic_load_explicit(__LINE__, &vars[2], memory_order_acquire);
  int v12_r7 = v11_r6 ^ v11_r6;
  int v15_r8 = atomic_load_explicit(__LINE__, &vars[3+v12_r7], memory_order_acquire);
  int v16_r10 = v15_r8 ^ v15_r8;
  atomic_store_explicit(__LINE__, &vars[4+v16_r10], 1, memory_order_release);
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);
  int v21 = (v6_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r1_1, v21, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 

  atomic_init(&vars[4], 0);
  atomic_init(&vars[2], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&vars[3], 0);
  atomic_init(&atom_0_r1_1, 0); 
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v17 = atomic_load_explicit(__LINE__, &atom_0_r1_1, memory_order_acquire);
  int v18 = atomic_load_explicit(__LINE__, &atom_1_r1_1, memory_order_acquire);
  int v19_conj = v17 & v18;
  if (v19_conj == 1) MODEL_ASSERT(0);
  return 0;
}
