/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[2]; 
atomic<int> atom_1_r1_1; 
atomic<int> atom_1_r2_0; 
atomic<int> atom_1_r2_2; 

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
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  int v4_r2 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v64 = (v2_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r1_1, v64, memory_order_release);
  int v65 = (v4_r2 == 0);
  atomic_store_explicit(__LINE__, &atom_1_r2_0, v65, memory_order_release);
  int v66 = (v4_r2 == 2);
  atomic_store_explicit(__LINE__, &atom_1_r2_2, v66, memory_order_release);

}

void t2(void *arg){
label_3:;
  atomic_store_explicit(__LINE__, &vars[0], 2, memory_order_release);

}

void t3(void *arg){
label_4:;
  atomic_store_explicit(__LINE__, &vars[1], 2, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 
  thrd_t thr3; 

  atomic_init(&vars[0], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_1_r2_0, 0); 
  atomic_init(&atom_1_r2_2, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);
  thrd_create(&thr3, t3, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);
  thrd_join(thr3);

  int v5 = atomic_load_explicit(__LINE__, &atom_1_r1_1, memory_order_acquire);
  int v6 = atomic_load_explicit(__LINE__, &atom_1_r2_0, memory_order_acquire);
  int v7 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v8 = (v7 == 1);
  int v9 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v10 = (v9 == 1);
  int v11_conj = v8 & v10;
  int v12_conj = v6 & v11_conj;
  int v13_conj = v5 & v12_conj;
  int v14 = atomic_load_explicit(__LINE__, &atom_1_r1_1, memory_order_acquire);
  int v15 = atomic_load_explicit(__LINE__, &atom_1_r2_0, memory_order_acquire);
  int v16 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v17 = (v16 == 1);
  int v18 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v19 = (v18 == 2);
  int v20_conj = v17 & v19;
  int v21_conj = v15 & v20_conj;
  int v22_conj = v14 & v21_conj;
  int v23 = atomic_load_explicit(__LINE__, &atom_1_r1_1, memory_order_acquire);
  int v24 = atomic_load_explicit(__LINE__, &atom_1_r2_0, memory_order_acquire);
  int v25 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v26 = (v25 == 2);
  int v27 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v28 = (v27 == 1);
  int v29_conj = v26 & v28;
  int v30_conj = v24 & v29_conj;
  int v31_conj = v23 & v30_conj;
  int v32 = atomic_load_explicit(__LINE__, &atom_1_r1_1, memory_order_acquire);
  int v33 = atomic_load_explicit(__LINE__, &atom_1_r2_0, memory_order_acquire);
  int v34 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v35 = (v34 == 2);
  int v36 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v37 = (v36 == 2);
  int v38_conj = v35 & v37;
  int v39_conj = v33 & v38_conj;
  int v40_conj = v32 & v39_conj;
  int v41 = atomic_load_explicit(__LINE__, &atom_1_r1_1, memory_order_acquire);
  int v42 = atomic_load_explicit(__LINE__, &atom_1_r2_2, memory_order_acquire);
  int v43 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v44 = (v43 == 1);
  int v45 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v46 = (v45 == 1);
  int v47_conj = v44 & v46;
  int v48_conj = v42 & v47_conj;
  int v49_conj = v41 & v48_conj;
  int v50 = atomic_load_explicit(__LINE__, &atom_1_r1_1, memory_order_acquire);
  int v51 = atomic_load_explicit(__LINE__, &atom_1_r2_2, memory_order_acquire);
  int v52 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v53 = (v52 == 1);
  int v54 = atomic_load_explicit(__LINE__, &vars[1], memory_order_acquire);
  int v55 = (v54 == 2);
  int v56_conj = v53 & v55;
  int v57_conj = v51 & v56_conj;
  int v58_conj = v50 & v57_conj;
  int v59_disj = v49_conj | v58_conj;
  int v60_disj = v40_conj | v59_disj;
  int v61_disj = v31_conj | v60_disj;
  int v62_disj = v22_conj | v61_disj;
  int v63_disj = v13_conj | v62_disj;
  if (v63_disj == 1) MODEL_ASSERT(0);
  return 0;
}
