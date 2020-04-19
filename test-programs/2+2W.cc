#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> var_y;
atomic<int> var_x;

static void t0(void *arg){
  var_x.store(1, memory_order_release);
  var_y.store(2, memory_order_release);
}

static void t1(void *arg){
  var_y.store(1, memory_order_release);
  var_x.store(2, memory_order_release);
}

int user_main(int argc, char **argv){
  thrd_t thr0;
  thrd_t thr1;

  atomic_init(&var_y, 0);
  atomic_init(&var_x, 0);

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);

  thrd_join(thr0);
  thrd_join(thr1);

  int v1 = var_x.load(memory_order_acquire);
  int v2 = (v1 == 1);
  int v3 = var_y.load(memory_order_acquire);
  int v4 = (v3 == 1);
  int v5_conj = v2 & v4;
  if (v5_conj == 1) {
      MODEL_ASSERT(0);
  }
  return 0;
}
