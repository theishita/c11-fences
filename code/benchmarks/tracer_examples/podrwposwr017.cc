// /Users/tuan-phongngo/WETA/litmus_benchmarks/gen-litmuts/power-tests/podrwposwr017.litmus

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> vars0;
atomic<int> vars1;
atomic<int> vars2;
atomic<int> vars3;
atomic<int> vars4; 
atomic<int> atom_2_r1_1; 
atomic<int> atom_3_r1_1; 
atomic<int> atom_3_r8_1; 
atomic<int> __fence_var;

static void t0(void *arg){
label_1:;
  vars0.store(__LINE__, 2, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  vars1.store(__LINE__, 1, memory_order_release);

}

static void t1(void *arg){
label_2:;
  vars1.store(__LINE__, 2, memory_order_release);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  vars2.store(__LINE__, 1, memory_order_release);

}

static void t2(void *arg){
label_3:;
  int v2_r1 = vars2.load(__LINE__, memory_order_acquire);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, memory_order_acq_rel);
  vars3.store(__LINE__, 1, memory_order_release);
  int v22 = (v2_r1 == 1);
  atom_2_r1_1.store(__LINE__, v22, memory_order_release);

}

static void t3(void *arg){
label_4:;
  int v4_r1 = vars3.load(__LINE__, memory_order_acquire);
  int v5_r3 = v4_r1 ^ v4_r1;

  int sum = 4+v5_r3;
  int v8_r4;
  if(sum == 0) {
    v8_r4 = vars0.load(__LINE__, memory_order_acquire);
  }
  else if(sum == 1) {
    v8_r4 = vars1.load(__LINE__, memory_order_acquire);
  }
  else if(sum == 2) {
    v8_r4 = vars2.load(__LINE__, memory_order_acquire);
  }
  else if(sum == 3) {
    v8_r4 = vars3.load(__LINE__, memory_order_acquire);
  }
  else if(sum == 4) {
    v8_r4 = vars4.load(__LINE__, memory_order_acquire);
  }

  vars0.store(__LINE__, 1, memory_order_release);
  int v10_r8 = vars0.load(__LINE__, memory_order_acquire);
  int v23 = (v4_r1 == 1);
  atom_3_r1_1.store(__LINE__, v23, memory_order_release);
  int v24 = (v10_r8 == 1);
  atom_3_r8_1.store(__LINE__, v24, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 
  thrd_t thr3; 

  atomic_init(&vars4, 0);
  atomic_init(&vars3, 0);
  atomic_init(&vars2, 0);
  atomic_init(&vars1, 0);
  atomic_init(&vars0, 0);
  atomic_init(&atom_2_r1_1, 0); 
  atomic_init(&atom_3_r1_1, 0); 
  atomic_init(&atom_3_r8_1, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);
  thrd_create(&thr3, t3, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);
  thrd_join(thr3);

  int v11 = vars0.load(__LINE__, memory_order_acquire);
  int v12 = (v11 == 2);
  int v13 = vars1.load(__LINE__, memory_order_acquire);
  int v14 = (v13 == 2);
  int v15 = atom_2_r1_1.load(__LINE__, memory_order_acquire);
  int v16 = atom_3_r1_1.load(__LINE__, memory_order_acquire);
  int v17 = atom_3_r8_1.load(__LINE__, memory_order_acquire);
  int v18_conj = v16 & v17;
  int v19_conj = v15 & v18_conj;
  int v20_conj = v14 & v19_conj;
  int v21_conj = v12 & v20_conj;
  if (v21_conj == 1) MODEL_ASSERT(0);
  return 0;
}