/* Copyright (C) 2018 Tuan Phong Ngo 
 * This benchmark is part of TRACER
 */

/*
 * Lamport's Bakery critical section algorithm, implemented with fences.
 *
 * Adapted from the benchmark in the URL:
 *   https://www.geeksforgeeks.org/operating-system-bakery-algorithm/
 */

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

#define rel memory_order_release
#define acq memory_order_acquire
#define acq_rel memory_order_acq_rel

#define LOOP 3
#define THREAD_COUNT 3

// shared variables
atomic<int> tickets[THREAD_COUNT];
atomic<int> choosing[THREAD_COUNT];
atomic<int> resource;
atomic<int> __fence_var;

bool lock(int thread) {
  int ok = 0;
  atomic_store_explicit(&choosing[thread], 1, rel);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, acq_rel);
  int max_ticket = 0;
  for (int i = 0; i < THREAD_COUNT; ++i) {
    int ticket = atomic_load_explicit(&tickets[i], acq);
    max_ticket = ticket > max_ticket ? ticket : max_ticket;
  }
  atomic_store_explicit(&tickets[thread], max_ticket + 1, rel);
  atomic_store_explicit(&choosing[thread], 0, rel);
  atomic_fetch_add_explicit(__LINE__, &__fence_var, 0, acq_rel);
  for (int other = 0; other < THREAD_COUNT; ++other) {
    ok = 0;
    for (int i=0; i<LOOP; i++) {
      if (!atomic_load_explicit(&choosing[other], acq)) {
        ok = 1;
        break;
      }
    }
    
    if (ok==0) return false;
    else {
      ok = 0;
      for (int i=0; i<LOOP; i++) {
        int other_ticket = atomic_load_explicit(&tickets[other],acq);
        int thread_ticket = atomic_load_explicit(&tickets[thread], acq);
        if (!(other_ticket != 0 &&
              (other_ticket < thread_ticket ||
                (other_ticket == thread_ticket && other < thread)
               )
              )) {
                ok = 1;
                break;
              }
      }
      if (ok==0) return false;
    }
  }
  return true;
}

static void unlock(int thread) {
  atomic_store_explicit(&tickets[thread], 0, rel);
}

static void use_resource(int thread) {
  if (atomic_load_explicit(&resource, acq) != 0) {
    MODEL_ASSERT(0); // raise mutex assertion violation
  }
  atomic_store_explicit(&resource, thread, rel);
  atomic_store_explicit(&resource, 0, rel);
}

static void thread_body(void *arg) {
  int thread = *((int *)arg);
  if (lock(thread)) {
    use_resource(thread);
    unlock(thread);
  }
}

int user_main(int argc, char **argv) {
  thrd_t threads[THREAD_COUNT];
  int arg[THREAD_COUNT];
  
  for (int i=0; i<THREAD_COUNT; i++) {
    atomic_init(&tickets[i], 0);
    atomic_init(&choosing[i], 0);
  }
  atomic_init(&resource, 0);
  atomic_init(&__fence_var, 0);
  
  for (int i = 0; i < THREAD_COUNT; ++i) {
    arg[i]=i;
    thrd_create(&threads[i], thread_body, &arg[i]);
  }
  
  for (int i = 0; i < THREAD_COUNT; ++i) {
    thrd_join(threads[i]);
  }
  
  return 0;
}