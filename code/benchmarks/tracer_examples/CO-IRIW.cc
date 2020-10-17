/* Copyright (C) 2018 Tuan Phong Ngo
 * This file is part of TRACER */

#include <threads.h>
#include <atomic>
#include <iostream>
#include <model-assert.h>

using namespace std;

atomic<int> vars[1]; 
atomic<int> atom_1_r0_2; 
atomic<int> atom_1_r1_2; 
atomic<int> atom_3_r0_2; 
atomic<int> atom_3_r1_2; 
atomic<int> atom_3_r1_1; 
atomic<int> atom_3_r0_1; 
atomic<int> atom_3_r0_0; 
atomic<int> atom_3_r1_0; 
atomic<int> atom_1_r1_1; 
atomic<int> atom_1_r0_1; 
atomic<int> atom_1_r0_0; 
atomic<int> atom_1_r1_0; 

atomic<int> __fence_var;

void t0(void *arg){
label_1:;
  atomic_store_explicit(__LINE__, &vars[0], 1, memory_order_release);

}

void t1(void *arg){
label_2:;
  int v2_r0 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v4_r1 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v164 = (v2_r0 == 2);
  atomic_store_explicit(__LINE__, &atom_1_r0_2, v164, memory_order_release);
  int v165 = (v4_r1 == 2);
  atomic_store_explicit(__LINE__, &atom_1_r1_2, v165, memory_order_release);
  int v172 = (v4_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r1_1, v172, memory_order_release);
  int v173 = (v2_r0 == 1);
  atomic_store_explicit(__LINE__, &atom_1_r0_1, v173, memory_order_release);
  int v174 = (v2_r0 == 0);
  atomic_store_explicit(__LINE__, &atom_1_r0_0, v174, memory_order_release);
  int v175 = (v4_r1 == 0);
  atomic_store_explicit(__LINE__, &atom_1_r1_0, v175, memory_order_release);

}

void t2(void *arg){
label_3:;
  atomic_store_explicit(__LINE__, &vars[0], 2, memory_order_release);

}

void t3(void *arg){
label_4:;
  int v6_r0 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v8_r1 = atomic_load_explicit(__LINE__, &vars[0], memory_order_acquire);
  int v166 = (v6_r0 == 2);
  atomic_store_explicit(__LINE__, &atom_3_r0_2, v166, memory_order_release);
  int v167 = (v8_r1 == 2);
  atomic_store_explicit(__LINE__, &atom_3_r1_2, v167, memory_order_release);
  int v168 = (v8_r1 == 1);
  atomic_store_explicit(__LINE__, &atom_3_r1_1, v168, memory_order_release);
  int v169 = (v6_r0 == 1);
  atomic_store_explicit(__LINE__, &atom_3_r0_1, v169, memory_order_release);
  int v170 = (v6_r0 == 0);
  atomic_store_explicit(__LINE__, &atom_3_r0_0, v170, memory_order_release);
  int v171 = (v8_r1 == 0);
  atomic_store_explicit(__LINE__, &atom_3_r1_0, v171, memory_order_release);

}

int user_main(int argc, char **argv){
  thrd_t thr0; 
  thrd_t thr1; 
  thrd_t thr2; 
  thrd_t thr3; 

  atomic_init(&vars[0], 0);
  atomic_init(&atom_1_r0_2, 0); 
  atomic_init(&atom_1_r1_2, 0); 
  atomic_init(&atom_3_r0_2, 0); 
  atomic_init(&atom_3_r1_2, 0); 
  atomic_init(&atom_3_r1_1, 0); 
  atomic_init(&atom_3_r0_1, 0); 
  atomic_init(&atom_3_r0_0, 0); 
  atomic_init(&atom_3_r1_0, 0); 
  atomic_init(&atom_1_r1_1, 0); 
  atomic_init(&atom_1_r0_1, 0); 
  atomic_init(&atom_1_r0_0, 0); 
  atomic_init(&atom_1_r1_0, 0); 
  atomic_init(&__fence_var, 0); 

  thrd_create(&thr0, t0, NULL);
  thrd_create(&thr1, t1, NULL);
  thrd_create(&thr2, t2, NULL);
  thrd_create(&thr3, t3, NULL);

  thrd_join(thr0);
  thrd_join(thr1);
  thrd_join(thr2);
  thrd_join(thr3);

  int v9 = atomic_load_explicit(__LINE__, &atom_1_r0_2, memory_order_acquire);
  int v10 = atomic_load_explicit(__LINE__, &atom_1_r1_2, memory_order_acquire);
  int v11 = atomic_load_explicit(__LINE__, &atom_3_r0_2, memory_order_acquire);
  int v12 = atomic_load_explicit(__LINE__, &atom_3_r1_2, memory_order_acquire);
  int v13 = atomic_load_explicit(__LINE__, &atom_3_r1_1, memory_order_acquire);
  int v14_disj = v12 | v13;
  int v15_conj = v11 & v14_disj;
  int v16 = atomic_load_explicit(__LINE__, &atom_3_r0_1, memory_order_acquire);
  int v17 = atomic_load_explicit(__LINE__, &atom_3_r1_2, memory_order_acquire);
  int v18 = atomic_load_explicit(__LINE__, &atom_3_r1_1, memory_order_acquire);
  int v19_disj = v17 | v18;
  int v20_conj = v16 & v19_disj;
  int v21 = atomic_load_explicit(__LINE__, &atom_3_r0_0, memory_order_acquire);
  int v22 = atomic_load_explicit(__LINE__, &atom_3_r1_2, memory_order_acquire);
  int v23 = atomic_load_explicit(__LINE__, &atom_3_r1_1, memory_order_acquire);
  int v24 = atomic_load_explicit(__LINE__, &atom_3_r1_0, memory_order_acquire);
  int v25_disj = v23 | v24;
  int v26_disj = v22 | v25_disj;
  int v27_conj = v21 & v26_disj;
  int v28_disj = v20_conj | v27_conj;
  int v29_disj = v15_conj | v28_disj;
  int v30_conj = v10 & v29_disj;
  int v31 = atomic_load_explicit(__LINE__, &atom_1_r1_1, memory_order_acquire);
  int v32 = atomic_load_explicit(__LINE__, &atom_3_r0_2, memory_order_acquire);
  int v33 = atomic_load_explicit(__LINE__, &atom_3_r1_2, memory_order_acquire);
  int v34 = atomic_load_explicit(__LINE__, &atom_3_r1_1, memory_order_acquire);
  int v35_disj = v33 | v34;
  int v36_conj = v32 & v35_disj;
  int v37 = atomic_load_explicit(__LINE__, &atom_3_r0_1, memory_order_acquire);
  int v38 = atomic_load_explicit(__LINE__, &atom_3_r1_1, memory_order_acquire);
  int v39_conj = v37 & v38;
  int v40 = atomic_load_explicit(__LINE__, &atom_3_r0_0, memory_order_acquire);
  int v41 = atomic_load_explicit(__LINE__, &atom_3_r1_2, memory_order_acquire);
  int v42 = atomic_load_explicit(__LINE__, &atom_3_r1_1, memory_order_acquire);
  int v43 = atomic_load_explicit(__LINE__, &atom_3_r1_0, memory_order_acquire);
  int v44_disj = v42 | v43;
  int v45_disj = v41 | v44_disj;
  int v46_conj = v40 & v45_disj;
  int v47_disj = v39_conj | v46_conj;
  int v48_disj = v36_conj | v47_disj;
  int v49_conj = v31 & v48_disj;
  int v50_disj = v30_conj | v49_conj;
  int v51_conj = v9 & v50_disj;
  int v52 = atomic_load_explicit(__LINE__, &atom_1_r0_1, memory_order_acquire);
  int v53 = atomic_load_explicit(__LINE__, &atom_1_r1_2, memory_order_acquire);
  int v54 = atomic_load_explicit(__LINE__, &atom_3_r0_2, memory_order_acquire);
  int v55 = atomic_load_explicit(__LINE__, &atom_3_r1_2, memory_order_acquire);
  int v56_conj = v54 & v55;
  int v57 = atomic_load_explicit(__LINE__, &atom_3_r0_1, memory_order_acquire);
  int v58 = atomic_load_explicit(__LINE__, &atom_3_r1_2, memory_order_acquire);
  int v59 = atomic_load_explicit(__LINE__, &atom_3_r1_1, memory_order_acquire);
  int v60_disj = v58 | v59;
  int v61_conj = v57 & v60_disj;
  int v62 = atomic_load_explicit(__LINE__, &atom_3_r0_0, memory_order_acquire);
  int v63 = atomic_load_explicit(__LINE__, &atom_3_r1_2, memory_order_acquire);
  int v64 = atomic_load_explicit(__LINE__, &atom_3_r1_1, memory_order_acquire);
  int v65 = atomic_load_explicit(__LINE__, &atom_3_r1_0, memory_order_acquire);
  int v66_disj = v64 | v65;
  int v67_disj = v63 | v66_disj;
  int v68_conj = v62 & v67_disj;
  int v69_disj = v61_conj | v68_conj;
  int v70_disj = v56_conj | v69_disj;
  int v71_conj = v53 & v70_disj;
  int v72 = atomic_load_explicit(__LINE__, &atom_1_r1_1, memory_order_acquire);
  int v73 = atomic_load_explicit(__LINE__, &atom_3_r0_2, memory_order_acquire);
  int v74 = atomic_load_explicit(__LINE__, &atom_3_r1_2, memory_order_acquire);
  int v75 = atomic_load_explicit(__LINE__, &atom_3_r1_1, memory_order_acquire);
  int v76_disj = v74 | v75;
  int v77_conj = v73 & v76_disj;
  int v78 = atomic_load_explicit(__LINE__, &atom_3_r0_1, memory_order_acquire);
  int v79 = atomic_load_explicit(__LINE__, &atom_3_r1_2, memory_order_acquire);
  int v80 = atomic_load_explicit(__LINE__, &atom_3_r1_1, memory_order_acquire);
  int v81_disj = v79 | v80;
  int v82_conj = v78 & v81_disj;
  int v83 = atomic_load_explicit(__LINE__, &atom_3_r0_0, memory_order_acquire);
  int v84 = atomic_load_explicit(__LINE__, &atom_3_r1_2, memory_order_acquire);
  int v85 = atomic_load_explicit(__LINE__, &atom_3_r1_1, memory_order_acquire);
  int v86 = atomic_load_explicit(__LINE__, &atom_3_r1_0, memory_order_acquire);
  int v87_disj = v85 | v86;
  int v88_disj = v84 | v87_disj;
  int v89_conj = v83 & v88_disj;
  int v90_disj = v82_conj | v89_conj;
  int v91_disj = v77_conj | v90_disj;
  int v92_conj = v72 & v91_disj;
  int v93_disj = v71_conj | v92_conj;
  int v94_conj = v52 & v93_disj;
  int v95 = atomic_load_explicit(__LINE__, &atom_1_r0_0, memory_order_acquire);
  int v96 = atomic_load_explicit(__LINE__, &atom_1_r1_2, memory_order_acquire);
  int v97 = atomic_load_explicit(__LINE__, &atom_3_r0_2, memory_order_acquire);
  int v98 = atomic_load_explicit(__LINE__, &atom_3_r1_2, memory_order_acquire);
  int v99 = atomic_load_explicit(__LINE__, &atom_3_r1_1, memory_order_acquire);
  int v100_disj = v98 | v99;
  int v101_conj = v97 & v100_disj;
  int v102 = atomic_load_explicit(__LINE__, &atom_3_r0_1, memory_order_acquire);
  int v103 = atomic_load_explicit(__LINE__, &atom_3_r1_2, memory_order_acquire);
  int v104 = atomic_load_explicit(__LINE__, &atom_3_r1_1, memory_order_acquire);
  int v105_disj = v103 | v104;
  int v106_conj = v102 & v105_disj;
  int v107 = atomic_load_explicit(__LINE__, &atom_3_r0_0, memory_order_acquire);
  int v108 = atomic_load_explicit(__LINE__, &atom_3_r1_2, memory_order_acquire);
  int v109 = atomic_load_explicit(__LINE__, &atom_3_r1_1, memory_order_acquire);
  int v110 = atomic_load_explicit(__LINE__, &atom_3_r1_0, memory_order_acquire);
  int v111_disj = v109 | v110;
  int v112_disj = v108 | v111_disj;
  int v113_conj = v107 & v112_disj;
  int v114_disj = v106_conj | v113_conj;
  int v115_disj = v101_conj | v114_disj;
  int v116_conj = v96 & v115_disj;
  int v117 = atomic_load_explicit(__LINE__, &atom_1_r1_1, memory_order_acquire);
  int v118 = atomic_load_explicit(__LINE__, &atom_3_r0_2, memory_order_acquire);
  int v119 = atomic_load_explicit(__LINE__, &atom_3_r1_2, memory_order_acquire);
  int v120 = atomic_load_explicit(__LINE__, &atom_3_r1_1, memory_order_acquire);
  int v121_disj = v119 | v120;
  int v122_conj = v118 & v121_disj;
  int v123 = atomic_load_explicit(__LINE__, &atom_3_r0_1, memory_order_acquire);
  int v124 = atomic_load_explicit(__LINE__, &atom_3_r1_2, memory_order_acquire);
  int v125 = atomic_load_explicit(__LINE__, &atom_3_r1_1, memory_order_acquire);
  int v126_disj = v124 | v125;
  int v127_conj = v123 & v126_disj;
  int v128 = atomic_load_explicit(__LINE__, &atom_3_r0_0, memory_order_acquire);
  int v129 = atomic_load_explicit(__LINE__, &atom_3_r1_2, memory_order_acquire);
  int v130 = atomic_load_explicit(__LINE__, &atom_3_r1_1, memory_order_acquire);
  int v131 = atomic_load_explicit(__LINE__, &atom_3_r1_0, memory_order_acquire);
  int v132_disj = v130 | v131;
  int v133_disj = v129 | v132_disj;
  int v134_conj = v128 & v133_disj;
  int v135_disj = v127_conj | v134_conj;
  int v136_disj = v122_conj | v135_disj;
  int v137_conj = v117 & v136_disj;
  int v138 = atomic_load_explicit(__LINE__, &atom_1_r1_0, memory_order_acquire);
  int v139 = atomic_load_explicit(__LINE__, &atom_3_r0_2, memory_order_acquire);
  int v140 = atomic_load_explicit(__LINE__, &atom_3_r1_2, memory_order_acquire);
  int v141 = atomic_load_explicit(__LINE__, &atom_3_r1_1, memory_order_acquire);
  int v142_disj = v140 | v141;
  int v143_conj = v139 & v142_disj;
  int v144 = atomic_load_explicit(__LINE__, &atom_3_r0_1, memory_order_acquire);
  int v145 = atomic_load_explicit(__LINE__, &atom_3_r1_2, memory_order_acquire);
  int v146 = atomic_load_explicit(__LINE__, &atom_3_r1_1, memory_order_acquire);
  int v147_disj = v145 | v146;
  int v148_conj = v144 & v147_disj;
  int v149 = atomic_load_explicit(__LINE__, &atom_3_r0_0, memory_order_acquire);
  int v150 = atomic_load_explicit(__LINE__, &atom_3_r1_2, memory_order_acquire);
  int v151 = atomic_load_explicit(__LINE__, &atom_3_r1_1, memory_order_acquire);
  int v152 = atomic_load_explicit(__LINE__, &atom_3_r1_0, memory_order_acquire);
  int v153_disj = v151 | v152;
  int v154_disj = v150 | v153_disj;
  int v155_conj = v149 & v154_disj;
  int v156_disj = v148_conj | v155_conj;
  int v157_disj = v143_conj | v156_disj;
  int v158_conj = v138 & v157_disj;
  int v159_disj = v137_conj | v158_conj;
  int v160_disj = v116_conj | v159_disj;
  int v161_conj = v95 & v160_disj;
  int v162_disj = v94_conj | v161_conj;
  int v163_disj = v51_conj | v162_disj;
  if (v163_disj == 0) MODEL_ASSERT(0);
  return 0;
}
