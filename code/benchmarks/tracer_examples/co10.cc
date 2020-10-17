/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[1]; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, __LINE__, &vars[0], 1, memory_order_release);
  atomic_store_explicit(__LINE__, __LINE__, &vars[0], 2, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 

  atomic_init(&vars[0], 0);
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);

  thrd_join(thr0);

  int v1 = atomic_load_explicit(__LINE__, __LINE__, &vars[0], memory_order_acquire);
  int v2 = (v1 == 1);
  if (v2 == 1) MODEL_ASSERT(0);
  return 0;
}
