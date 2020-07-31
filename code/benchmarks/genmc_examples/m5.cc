#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> var_y;
atomic<int> var_x;
atomic<int> atom_1_r1_1;
atomic<int> atom_1_r2_0;
atomic<int> atom_1_r2_2;
atomic<int> _rcmc_fence_var;
atomic<int> done_t0;
atomic<int> done_t1;
atomic<int> done_t2;
atomic<int> done_t3;

static void t0(void *arg) {
	var_x.store(1, memory_order_release);
	var_y.store(1, memory_order_release);
	done_t0.store(1, memory_order_release);
}

static void t1(void *arg) {
	int v2_r1 = var_y.load(memory_order_acquire);
	int v4_r2 = var_x.load(memory_order_acquire);
	int v64 = (v2_r1 == 1);
	atom_1_r1_1.store(v64, memory_order_release);
	int v65 = (v4_r2 == 0);
	atom_1_r2_0.store(v65, memory_order_release);
	int v66 = (v4_r2 == 2);
	atom_1_r2_2.store(v66, memory_order_release);
	done_t1.store(1, memory_order_release);
}

static void t2(void *arg) {
	var_x.store(2, memory_order_release);
	done_t2.store(1, memory_order_release);
}

static void t3(void *arg) {
	var_y.store(2, memory_order_release);
	done_t3.store(1, memory_order_release);
}

static void check(void *arg) {
	MODEL_ASSERT((done_t0.load(memory_order_acquire) == 1) && (done_t1.load(memory_order_acquire) == 1) && (done_t2.load(memory_order_acquire) == 1) && (done_t3.load(memory_order_acquire) == 1));

	int v5 = atom_1_r1_1.load(memory_order_acquire);
	int v6 = atom_1_r2_0.load(memory_order_acquire);
	int v7 = var_x.load(memory_order_acquire);
	int v8 = (v7 == 1);
	int v9 = var_y.load(memory_order_acquire);
	int v10 = (v9 == 1);
	int v11_conj = v8 & v10;
	int v12_conj = v6 & v11_conj;
	int v13_conj = v5 & v12_conj;
	int v14 = atom_1_r1_1.load(memory_order_acquire);
	int v15 = atom_1_r2_0.load(memory_order_acquire);
	int v16 = var_x.load(memory_order_acquire);
	int v17 = (v16 == 1);
	int v18 = var_y.load(memory_order_acquire);
	int v19 = (v18 == 2);
	int v20_conj = v17 & v19;
	int v21_conj = v15 & v20_conj;
	int v22_conj = v14 & v21_conj;
	int v23 = atom_1_r1_1.load(memory_order_acquire);
	int v24 = atom_1_r2_0.load(memory_order_acquire);
	int v25 = var_x.load(memory_order_acquire);
	int v26 = (v25 == 2);
	int v27 = var_y.load(memory_order_acquire);
	int v28 = (v27 == 1);
	int v29_conj = v26 & v28;
	int v30_conj = v24 & v29_conj;
	int v31_conj = v23 & v30_conj;
	int v32 = atom_1_r1_1.load(memory_order_acquire);
	int v33 = atom_1_r2_0.load(memory_order_acquire);
	int v34 = var_x.load(memory_order_acquire);
	int v35 = (v34 == 2);
	int v36 = var_y.load(memory_order_acquire);
	int v37 = (v36 == 2);
	int v38_conj = v35 & v37;
	int v39_conj = v33 & v38_conj;
	int v40_conj = v32 & v39_conj;
	int v41 = atom_1_r1_1.load(memory_order_acquire);
	int v42 = atom_1_r2_2.load(memory_order_acquire);
	int v43 = var_x.load(memory_order_acquire);
	int v44 = (v43 == 1);
	int v45 = var_y.load(memory_order_acquire);
	int v46 = (v45 == 1);
	int v47_conj = v44 & v46;
	int v48_conj = v42 & v47_conj;
	int v49_conj = v41 & v48_conj;
	int v50 = atom_1_r1_1.load(memory_order_acquire);
	int v51 = atom_1_r2_2.load(memory_order_acquire);
	int v52 = var_x.load(memory_order_acquire);
	int v53 = (v52 == 1);
	int v54 = var_y.load(memory_order_acquire);
	int v55 = (v54 == 2);
	int v56_conj = v53 & v55;
	int v57_conj = v51 & v56_conj;
	int v58_conj = v50 & v57_conj;
	int v59_disj = v49_conj | v58_conj;
	int v60_disj = v40_conj | v59_disj;
	int v61_disj = v31_conj | v60_disj;
	int v62_disj = v22_conj | v61_disj;
	int v63_disj = v13_conj | v62_disj;

	if (v63_disj == 1)
		MODEL_ASSERT(0);
}

int user_main(int argc, char **argv) {
	thrd_t thr0, thr1, thr2, thr3, checker;

	atomic_init(&var_y, 0);
	atomic_init(&var_x, 0);
	atomic_init(&atom_1_r1_1, 0);
	atomic_init(&atom_1_r2_0, 0);
	atomic_init(&atom_1_r2_2, 0);
	atomic_init(&_rcmc_fence_var, 0);
	atomic_init(&done_t0, 0);
	atomic_init(&done_t1, 0);
	atomic_init(&done_t2, 0);
	atomic_init(&done_t3, 0);

	thrd_create(&thr0, (thrd_start_t)&t0, NULL);
	thrd_create(&thr1, (thrd_start_t)&t1, NULL);
	thrd_create(&thr2, (thrd_start_t)&t2, NULL);
	thrd_create(&thr3, (thrd_start_t)&t3, NULL);
	thrd_create(&checker, (thrd_start_t)&check, NULL);

	return 0;
}
