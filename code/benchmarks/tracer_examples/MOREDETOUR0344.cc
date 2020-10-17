/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[2]; 
atomic<int> atom_0_r3_2; 
atomic<int> atom_0_r4_0; 
atomic<int> atom_2_r1_2; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, &vars[0], 2, memory_order_release);
  int v2_r3 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v3_cmpeq = (v2_r3 == v2_r3);
  if (v3_cmpeq)  goto lbl_LC00; else goto lbl_LC00;
lbl_LC00:;
  int v5_r4 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  atomic_store_explicit(__LINE__, &vars[1], 2, memory_order_release);
  int v21 = (v2_r3 == 2);
  atomic_store_explicit(__LINE__, &atom_0_r3_2, v21, memory_order_release);
  int v22 = (v5_r4 == 0);
  atomic_store_explicit(__LINE__, &atom_0_r4_0, v22, memory_order_release);

}

void t1(void *arg){
label_2:;
  atomic_store_explicit(__LINE__, &vars[1], 1, memory_order_release);

}

void t2(void *arg){
label_3:;
  int v7_r1 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v8_r3 = v7_r1 ^ v7_r1;
  int v9_r3 = v8_r3 + 1;
  atomic_store_explicit(__LINE__, &vars[0], v9_r3, memory_order_release);
  int v23 = (v7_r1 == 2);
  atomic_store_explicit(__LINE__, &atom_2_r1_2, v23, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars[0], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&atom_0_r3_2, 0); 
  atomic_init(&atom_0_r4_0, 0); 
  atomic_init(&atom_2_r1_2, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v10 = atomic_load_explicit(__LINE__, &atom_0_r3_2, memory_order_acquire);
  int v11 = atomic_load_explicit(__LINE__, &atom_0_r4_0, memory_order_acquire);
  int v12 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v13 = (v12 == 2);
  int v14 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v15 = (v14 == 2);
  int v16 = atomic_load_explicit(__LINE__, &atom_2_r1_2, memory_order_acquire);
  int v17_conj = v15 & v16;
  int v18_conj = v13 & v17_conj;
  int v19_conj = v11 & v18_conj;
  int v20_conj = v10 & v19_conj;
  if (v20_conj == 1) MODEL_ASSERT(0);
  return 0;
}
