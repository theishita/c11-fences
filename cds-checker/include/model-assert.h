#ifndef __MODEL_ASSERT_H__
#define __MODEL_ASSERT_H__
#include <stdatomic.h>
#include <stdlib.h>
// #include <atomic>

#if __cplusplus
extern "C" {
#else
#include <stdbool.h>
#endif

void model_assert(bool expr, const char *file, int line);
#define MODEL_ASSERT(expr) model_assert((expr), __FILE__, __LINE__)

int load(atomic_int x, memory_order order, const char *file, int line);
#define LOAD(atomic_var, mem_ord) load(atomic_var, mem_ord, __FILE__, __LINE__)

#if __cplusplus
}
#endif

#endif /* __MODEL_ASSERT_H__ */
