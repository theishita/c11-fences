// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/3.LB+addr+addr+po.litmus

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> vars0;
atomic<int> vars1;
atomic<int> vars2;
atomic<int> atom_0_r1_1; 
atomic<int> atom_1_r1_1; 
atomic<int> atom_2_r1_1; 
atomic<int> __fence_var;

static void t0(void *arg){
label_1:;
  int v2_r1 = vars0.load(memory_order_acquire);
  int v3_r3 = v2_r1 ^ v2_r1;

  int sum = 1 + v3_r3;
  if(sum == 0) {
    vars0.store(1, memory_order_release);
  }
  else if(sum == 1) {
    vars1.store(1, memory_order_release);
  }
  else if(sum == 2) {
    vars2.store(1, memory_order_release);
  }

  int v14 = (v2_r1 == 1);
  atom_0_r1_1.store(v14, memory_order_release);

}

static void t1(void *arg){
label_2:;
  int v5_r1 = vars1.load(memory_order_acquire);
  int v6_r3 = v5_r1 ^ v5_r1;

  int sum = 2+v6_r3;
  if(sum == 0) {
    vars0.store(1, memory_order_release);
  }
  else if(sum == 1) {
    vars1.store(1, memory_order_release);
  }
  else if(sum == 2) {
    vars2.store(1, memory_order_release);
  }

  int v15 = (v5_r1 == 1);
  atom_1_r1_1.store(v15, memory_order_release);

}

static void t2(void *arg){
label_3:;
  int v8_r1 = vars2.load(memory_order_acquire);
  vars0.store(1, memory_order_release);
  int v16 = (v8_r1 == 1);
  atom_2_r1_1.store(v16, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 

  atomic_init(&vars2, 0);
  atomic_init(&vars1, 0);
  atomic_init(&vars0, 0);
  atomic_init(&atom_0_r1_1, 0); 
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_2_r1_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);

  int v9 = atom_0_r1_1.load(memory_order_acquire);
  int v10 = atom_1_r1_1.load(memory_order_acquire);
  int v11 = atom_2_r1_1.load(memory_order_acquire);
  int v12_conj = v10 & v11;
  int v13_conj = v9 & v12_conj;
  if (v13_conj == 1) MODEL_ASSERT(0);
  return 0;
}