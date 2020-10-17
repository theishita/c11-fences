/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[4]; 
atomic<int> atom_0_r2_1; 
atomic<int> atom_1_r2_1; 
atomic<int> atom_2_r2_1; 
atomic<int> atom_3_r2_1; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  int v1_r8 = 100 * 10;
  int v2_r8 = v1_r8 / 10;
  int v3_r8 = v2_r8 * 10;
  int v4_r8 = v3_r8 / 10;
  int v5_r8 = v4_r8 ^ v4_r8;
  int v8_r2 = atomic_load_explicit(__LINE__, &vars[1+v5_r8], memory_order_acquire);
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);
  int v40 = (v8_r2 == 1);
  atomic_store_explicit(__LINE__, &atom_0_r2_1, v40, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v9_r8 = 100 / 10;
  int v10_r8 = v9_r8 * 10;
  int v11_r8 = v10_r8 / 10;
  int v12_r8 = v11_r8 * 10;
  int v13_r8 = v12_r8 ^ v12_r8;
  int v16_r2 = atomic_load_explicit(__LINE__, &vars[0+v13_r8], memory_order_acquire);
  atomic_store_explicit(__LINE__, &vars[1], 1, memory_order_release);
  int v41 = (v16_r2 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r2_1, v41, memory_order_release);

}

void t2(void *arg){
label_3:;
  int v17_r8 = 100 * 10;
  int v18_r8 = v17_r8 / 10;
  int v19_r8 = v18_r8 * 10;
  int v20_r8 = v19_r8 / 10;
  int v21_r8 = v20_r8 ^ v20_r8;
  int v24_r2 = atomic_load_explicit(__LINE__, &vars[3+v21_r8], memory_order_acquire);
  atomic_store_explicit(__LINE__, &vars[2], 1, memory_order_release);
  int v42 = (v24_r2 == 1);
  atomic_store_explicit(__LINE__, &atom_2_r2_1, v42, memory_order_release);

}

void t3(void *arg){
label_4:;
  int v25_r8 = 100 / 10;
  int v26_r8 = v25_r8 * 10;
  int v27_r8 = v26_r8 / 10;
  int v28_r8 = v27_r8 * 10;
  int v29_r8 = v28_r8 ^ v28_r8;
  int v32_r2 = atomic_load_explicit(__LINE__, &vars[2+v29_r8], memory_order_acquire);
  atomic_store_explicit(__LINE__, &vars[3], 1, memory_order_release);
  int v43 = (v32_r2 == 1);
  atomic_store_explicit(__LINE__, &atom_3_r2_1, v43, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 
  thrd_t thr3; 

  atomic_init(&vars[1], 0);
  atomic_init(&vars[0], 0);
  atomic_init(&vars[3], 0);
  atomic_init(&vars[2], 0);
  atomic_init(&atom_0_r2_1, 0); 
  atomic_init(&atom_1_r2_1, 0); 
  atomic_init(&atom_2_r2_1, 0); 
  atomic_init(&atom_3_r2_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);
  thrd_create(&thr3, t3, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);
  thrd_join(thr3);

  int v33 = atomic_load_explicit(__LINE__, &atom_0_r2_1, memory_order_acquire);
  int v34 = atomic_load_explicit(__LINE__, &atom_1_r2_1, memory_order_acquire);
  int v35_conj = v33 & v34;
  int v36 = atomic_load_explicit(__LINE__, &atom_2_r2_1, memory_order_acquire);
  int v37 = atomic_load_explicit(__LINE__, &atom_3_r2_1, memory_order_acquire);
  int v38_conj = v36 & v37;
  int v39_disj = v35_conj | v38_conj;
  if (v39_disj == 1) MODEL_ASSERT(0);
  return 0;
}
