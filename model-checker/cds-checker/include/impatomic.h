/**
 * @file impatomic.h
 * @brief Common header for C11/C++11 atomics
 *
 * Note that some features are unavailable, as they require support from a true
 * C11/C++11 compiler.
 */

#ifndef __IMPATOMIC_H__
#define __IMPATOMIC_H__

#include "memoryorder.h"
#include "cmodelint.h"

#ifdef __cplusplus
namespace std {
#else
#include <stdbool.h>
#endif

#define CPP0X( feature )

typedef struct atomic_flag
{
#ifdef __cplusplus
    bool test_and_set( memory_order = memory_order_seq_cst ) volatile;
    void clear( memory_order = memory_order_seq_cst ) volatile;

    CPP0X( atomic_flag() = default; )
    CPP0X( atomic_flag( const atomic_flag& ) = delete; )
    atomic_flag& operator =( const atomic_flag& ) CPP0X(=delete);

CPP0X(private:)
#endif
    bool __f__;
} atomic_flag;

#define ATOMIC_FLAG_INIT { false }

#ifdef __cplusplus
extern "C" {
#endif

extern bool atomic_flag_test_and_set( volatile atomic_flag* );
extern bool atomic_flag_test_and_set_explicit
( volatile atomic_flag*, memory_order );
extern void atomic_flag_clear( volatile atomic_flag* );
extern void atomic_flag_clear_explicit
( volatile atomic_flag*, memory_order );
extern void __atomic_flag_wait__
( volatile atomic_flag* );
extern void __atomic_flag_wait_explicit__
( volatile atomic_flag*, memory_order );

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

inline bool atomic_flag::test_and_set( memory_order __x__ ) volatile
{ return atomic_flag_test_and_set_explicit( this, __x__ ); }

inline void atomic_flag::clear( memory_order __x__ ) volatile
{ atomic_flag_clear_explicit( this, __x__ ); }

#endif


/*
        The remainder of the example implementation uses the following
        macros. These macros exploit GNU extensions for value-returning
        blocks (AKA statement expressions) and __typeof__.

        The macros rely on data fields of atomic structs being named __f__.
        Other symbols used are __a__=atomic, __e__=expected, __f__=field,
        __g__=flag, __m__=modified, __o__=operation, __r__=result,
        __p__=pointer to field, __v__=value (for single evaluation),
        __x__=memory-ordering, and __y__=memory-ordering.
*/

#define _ATOMIC_LOAD_( line_no, __a__, __x__ )                                         \
        ({ volatile __typeof__((__a__)->__f__)* __p__ = & ((__a__)->__f__);   \
                __typeof__((__a__)->__f__) __r__ = (__typeof__((__a__)->__f__))model_read_action((void *)__p__, __x__, line_no);  \
                __r__; })

#define _ATOMIC_STORE_( line_no, __a__, __m__, __x__ )                                 \
        ({ volatile __typeof__((__a__)->__f__)* __p__ = & ((__a__)->__f__);   \
                __typeof__(__m__) __v__ = (__m__);                            \
                model_write_action((void *) __p__,  __x__, (uint64_t) __v__, line_no); \
                __v__ = __v__; /* Silence clang (-Wunused-value) */           \
         })


#define _ATOMIC_INIT_( __a__, __m__ )                                         \
        ({ volatile __typeof__((__a__)->__f__)* __p__ = & ((__a__)->__f__);   \
                __typeof__(__m__) __v__ = (__m__);                            \
                model_init_action((void *) __p__,  (uint64_t) __v__);         \
                __v__ = __v__; /* Silence clang (-Wunused-value) */           \
         })

#define _ATOMIC_MODIFY_( line_no, __a__, __o__, __m__, __x__ )                         \
        ({ volatile __typeof__((__a__)->__f__)* __p__ = & ((__a__)->__f__);   \
        __typeof__((__a__)->__f__) __old__=(__typeof__((__a__)->__f__)) model_rmwr_action((void *)__p__, __x__, line_no); \
        __typeof__(__m__) __v__ = (__m__);                                    \
        __typeof__((__a__)->__f__) __copy__= __old__;                         \
        __copy__ __o__ __v__;                                                 \
        model_rmw_action((void *)__p__, __x__, (uint64_t) __copy__);          \
        __old__ = __old__; /* Silence clang (-Wunused-value) */               \
         })

/* No spurious failure for now */
#define _ATOMIC_CMPSWP_WEAK_ _ATOMIC_CMPSWP_

#define _ATOMIC_CMPSWP_( line_no, __a__, __e__, __m__, __x__ )                         \
        ({ volatile __typeof__((__a__)->__f__)* __p__ = & ((__a__)->__f__);   \
                __typeof__(__e__) __q__ = (__e__);                            \
                __typeof__(__m__) __v__ = (__m__);                            \
                bool __r__;                                                   \
                __typeof__((__a__)->__f__) __t__=(__typeof__((__a__)->__f__)) model_rmwr_action((void *)__p__, __x__, line_no); \
                if (__t__ == * __q__ ) {                                      \
                        model_rmw_action((void *)__p__, __x__, (uint64_t) __v__); __r__ = true; } \
                else {  model_rmwc_action((void *)__p__, __x__); *__q__ = __t__;  __r__ = false;} \
                __r__; })

#define _ATOMIC_FENCE_( __x__ ) \
	({ model_fence_action(__x__);})
 

#define ATOMIC_CHAR_LOCK_FREE 1
#define ATOMIC_CHAR16_T_LOCK_FREE 1
#define ATOMIC_CHAR32_T_LOCK_FREE 1
#define ATOMIC_WCHAR_T_LOCK_FREE 1
#define ATOMIC_SHORT_LOCK_FREE 1
#define ATOMIC_INT_LOCK_FREE 1
#define ATOMIC_LONG_LOCK_FREE 1
#define ATOMIC_LLONG_LOCK_FREE 1
#define ATOMIC_ADDRESS_LOCK_FREE 1

typedef struct atomic_bool
{
#ifdef __cplusplus
    bool is_lock_free() const volatile;
    void store( int, bool, memory_order = memory_order_seq_cst ) volatile;
    bool load( int, memory_order = memory_order_seq_cst ) volatile;
    bool exchange( int, bool, memory_order = memory_order_seq_cst ) volatile;
    bool compare_exchange_weak ( int,  bool&, bool, memory_order, memory_order ) volatile;
    bool compare_exchange_strong ( int, bool&, bool, memory_order, memory_order ) volatile;
    bool compare_exchange_weak ( int,  bool&, bool,
                        memory_order = memory_order_seq_cst) volatile;
    bool compare_exchange_strong ( int, bool&, bool,
                        memory_order = memory_order_seq_cst) volatile;

    CPP0X( atomic_bool() = delete; )
    CPP0X( constexpr explicit atomic_bool( bool __v__ ) : __f__( __v__ ) { } )
    CPP0X( atomic_bool( const atomic_bool& ) = delete; )
    atomic_bool& operator =( const atomic_bool& ) CPP0X(=delete);

    bool operator =( bool __v__ ) volatile
    { store(0,  __v__ ); return __v__; }

    friend void atomic_store_explicit( int, volatile atomic_bool*, bool,
                                       memory_order );
    friend bool atomic_load_explicit( int, volatile atomic_bool*, memory_order );
    friend bool atomic_exchange_explicit( int, volatile atomic_bool*, bool,
                                      memory_order );
    friend bool atomic_compare_exchange_weak_explicit( int, volatile atomic_bool*, bool*, bool,
                                              memory_order, memory_order );
    friend bool atomic_compare_exchange_strong_explicit( int, volatile atomic_bool*, bool*, bool,
                                              memory_order, memory_order );

CPP0X(private:)
#endif
    bool __f__;
} atomic_bool;


typedef struct atomic_address
{
#ifdef __cplusplus
    bool is_lock_free() const volatile;
    void store( int, void*, memory_order = memory_order_seq_cst ) volatile;
    void* load( int, memory_order = memory_order_seq_cst ) volatile;
    void* exchange( int, void*, memory_order = memory_order_seq_cst ) volatile;
    bool compare_exchange_weak( int,  void*&, void*, memory_order, memory_order ) volatile;
    bool compare_exchange_strong( int, void*&, void*, memory_order, memory_order ) volatile;
    bool compare_exchange_weak( int,  void*&, void*,
                       memory_order = memory_order_seq_cst ) volatile;
    bool compare_exchange_strong( int, void*&, void*,
                       memory_order = memory_order_seq_cst ) volatile;
    void* fetch_add( int, ptrdiff_t, memory_order = memory_order_seq_cst ) volatile;
    void* fetch_sub( int, ptrdiff_t, memory_order = memory_order_seq_cst ) volatile;

    CPP0X( atomic_address() = default; )
    CPP0X( constexpr explicit atomic_address( void* __v__ ) : __f__( __v__) { } )
    CPP0X( atomic_address( const atomic_address& ) = delete; )
    atomic_address& operator =( const atomic_address & ) CPP0X(=delete);

    void* operator =( void* __v__ ) volatile
    { store(0,  __v__ ); return __v__; }

    void* operator +=( ptrdiff_t __v__ ) volatile
    { return fetch_add( 0, __v__ ); }

    void* operator -=( ptrdiff_t __v__ ) volatile
    { return fetch_sub( 0, __v__ ); }

    friend void atomic_store_explicit( int, volatile atomic_address*, void*,
                                       memory_order );
    friend void* atomic_load_explicit( int, volatile atomic_address*, memory_order );
    friend void* atomic_exchange_explicit( int, volatile atomic_address*, void*,
                                       memory_order );
    friend bool atomic_compare_exchange_weak_explicit( int, volatile atomic_address*,
                              void**, void*, memory_order, memory_order );
    friend bool atomic_compare_exchange_strong_explicit( int, volatile atomic_address*,
                              void**, void*, memory_order, memory_order );
    friend void* atomic_fetch_add_explicit( int, volatile atomic_address*, ptrdiff_t,
                                            memory_order );
    friend void* atomic_fetch_sub_explicit( int, volatile atomic_address*, ptrdiff_t,
                                            memory_order );

CPP0X(private:)
#endif
    void* __f__;
} atomic_address;


typedef struct atomic_char
{
#ifdef __cplusplus
    bool is_lock_free() const volatile;
    void store( int, char,
                memory_order = memory_order_seq_cst ) volatile;
    char load( int, memory_order = memory_order_seq_cst ) volatile;
    char exchange( int, char,
                      memory_order = memory_order_seq_cst ) volatile;
    bool compare_exchange_weak( int,  char&, char,
                       memory_order, memory_order ) volatile;
    bool compare_exchange_strong( int, char&, char,
                       memory_order, memory_order ) volatile;
    bool compare_exchange_weak( int,  char&, char,
                       memory_order = memory_order_seq_cst ) volatile;
    bool compare_exchange_strong( int, char&, char,
                       memory_order = memory_order_seq_cst ) volatile;
    char fetch_add( int, char,
                           memory_order = memory_order_seq_cst ) volatile;
    char fetch_sub( int, char,
                           memory_order = memory_order_seq_cst ) volatile;
    char fetch_and( int, char,
                           memory_order = memory_order_seq_cst ) volatile;
    char fetch_or( int,char,
                           memory_order = memory_order_seq_cst ) volatile;
    char fetch_xor( int, char,
                           memory_order = memory_order_seq_cst ) volatile;

    CPP0X( atomic_char() = default; )
    CPP0X( constexpr atomic_char( char __v__ ) : __f__( __v__) { } )
    CPP0X( atomic_char( const atomic_char& ) = delete; )
    atomic_char& operator =( const atomic_char& ) CPP0X(=delete);

    char operator =( char __v__ ) volatile
    { store(0,  __v__ ); return __v__; }

    char operator ++( int ) volatile
    { return fetch_add( 0, 1 ); }

    char operator --( int ) volatile
    { return fetch_sub( 0, 1 ); }

    char operator ++() volatile
    { return fetch_add( 0, 1 ) + 1; }

    char operator --() volatile
    { return fetch_sub( 0, 1 ) - 1; }

    char operator +=( char __v__ ) volatile
    { return fetch_add( 0, __v__ ) + __v__; }

    char operator -=( char __v__ ) volatile
    { return fetch_sub( 0, __v__ ) - __v__; }

    char operator &=( char __v__ ) volatile
    { return fetch_and( 0, __v__ ) & __v__; }

    char operator |=( char __v__ ) volatile
    { return fetch_or( 0, __v__ ) | __v__; }

    char operator ^=( char __v__ ) volatile
    { return fetch_xor( 0, __v__ ) ^ __v__; }

    friend void atomic_store_explicit( int, volatile atomic_char*, char,
                                       memory_order );
    friend char atomic_load_explicit( int, volatile atomic_char*,
                                             memory_order );
    friend char atomic_exchange_explicit( int, volatile atomic_char*,
                                             char, memory_order );
    friend bool atomic_compare_exchange_weak_explicit( int, volatile atomic_char*,
                      char*, char, memory_order, memory_order );
    friend bool atomic_compare_exchange_strong_explicit( int, volatile atomic_char*,
                      char*, char, memory_order, memory_order );
    friend char atomic_fetch_add_explicit( int, volatile atomic_char*,
                                                  char, memory_order );
    friend char atomic_fetch_sub_explicit( int, volatile atomic_char*,
                                                  char, memory_order );
    friend char atomic_fetch_and_explicit( int, volatile atomic_char*,
                                                  char, memory_order );
    friend char atomic_fetch_or_explicit( int,  volatile atomic_char*,
                                                  char, memory_order );
    friend char atomic_fetch_xor_explicit( int, volatile atomic_char*,
                                                  char, memory_order );

CPP0X(private:)
#endif
    char __f__;
} atomic_char;


typedef struct atomic_schar
{
#ifdef __cplusplus
    bool is_lock_free() const volatile;
    void store( int, signed char,
                memory_order = memory_order_seq_cst ) volatile;
    signed char load( int, memory_order = memory_order_seq_cst ) volatile;
    signed char exchange( int, signed char,
                      memory_order = memory_order_seq_cst ) volatile;
    bool compare_exchange_weak( int,  signed char&, signed char,
                       memory_order, memory_order ) volatile;
    bool compare_exchange_strong( int, signed char&, signed char,
                       memory_order, memory_order ) volatile;
    bool compare_exchange_weak( int,  signed char&, signed char,
                       memory_order = memory_order_seq_cst ) volatile;
    bool compare_exchange_strong( int, signed char&, signed char,
                       memory_order = memory_order_seq_cst ) volatile;
    signed char fetch_add( int, signed char,
                           memory_order = memory_order_seq_cst ) volatile;
    signed char fetch_sub( int, signed char,
                           memory_order = memory_order_seq_cst ) volatile;
    signed char fetch_and( int, signed char,
                           memory_order = memory_order_seq_cst ) volatile;
    signed char fetch_or( int,signed char,
                           memory_order = memory_order_seq_cst ) volatile;
    signed char fetch_xor( int, signed char,
                           memory_order = memory_order_seq_cst ) volatile;

    CPP0X( atomic_schar() = default; )
    CPP0X( constexpr atomic_schar( signed char __v__ ) : __f__( __v__) { } )
    CPP0X( atomic_schar( const atomic_schar& ) = delete; )
    atomic_schar& operator =( const atomic_schar& ) CPP0X(=delete);

    signed char operator =( signed char __v__ ) volatile
    { store(0,  __v__ ); return __v__; }

    signed char operator ++( int ) volatile
    { return fetch_add( 0, 1 ); }

    signed char operator --( int ) volatile
    { return fetch_sub( 0, 1 ); }

    signed char operator ++() volatile
    { return fetch_add( 0, 1 ) + 1; }

    signed char operator --() volatile
    { return fetch_sub( 0, 1 ) - 1; }

    signed char operator +=( signed char __v__ ) volatile
    { return fetch_add( 0, __v__ ) + __v__; }

    signed char operator -=( signed char __v__ ) volatile
    { return fetch_sub( 0, __v__ ) - __v__; }

    signed char operator &=( signed char __v__ ) volatile
    { return fetch_and( 0, __v__ ) & __v__; }

    signed char operator |=( signed char __v__ ) volatile
    { return fetch_or( 0, __v__ ) | __v__; }

    signed char operator ^=( signed char __v__ ) volatile
    { return fetch_xor( 0, __v__ ) ^ __v__; }

    friend void atomic_store_explicit( int, volatile atomic_schar*, signed char,
                                       memory_order );
    friend signed char atomic_load_explicit( int, volatile atomic_schar*,
                                             memory_order );
    friend signed char atomic_exchange_explicit( int, volatile atomic_schar*,
                                             signed char, memory_order );
    friend bool atomic_compare_exchange_weak_explicit( int, volatile atomic_schar*,
                      signed char*, signed char, memory_order, memory_order );
    friend bool atomic_compare_exchange_strong_explicit( int, volatile atomic_schar*,
                      signed char*, signed char, memory_order, memory_order );
    friend signed char atomic_fetch_add_explicit( int, volatile atomic_schar*,
                                                  signed char, memory_order );
    friend signed char atomic_fetch_sub_explicit( int, volatile atomic_schar*,
                                                  signed char, memory_order );
    friend signed char atomic_fetch_and_explicit( int, volatile atomic_schar*,
                                                  signed char, memory_order );
    friend signed char atomic_fetch_or_explicit( int,  volatile atomic_schar*,
                                                  signed char, memory_order );
    friend signed char atomic_fetch_xor_explicit( int, volatile atomic_schar*,
                                                  signed char, memory_order );

CPP0X(private:)
#endif
    signed char __f__;
} atomic_schar;


typedef struct atomic_uchar
{
#ifdef __cplusplus
    bool is_lock_free() const volatile;
    void store( int, unsigned char,
                memory_order = memory_order_seq_cst ) volatile;
    unsigned char load( int, memory_order = memory_order_seq_cst ) volatile;
    unsigned char exchange( int, unsigned char,
                      memory_order = memory_order_seq_cst ) volatile;
    bool compare_exchange_weak( int,  unsigned char&, unsigned char,
                       memory_order, memory_order ) volatile;
    bool compare_exchange_strong( int, unsigned char&, unsigned char,
                       memory_order, memory_order ) volatile;
    bool compare_exchange_weak( int,  unsigned char&, unsigned char,
                       memory_order = memory_order_seq_cst ) volatile;
    bool compare_exchange_strong( int, unsigned char&, unsigned char,
                       memory_order = memory_order_seq_cst ) volatile;
    unsigned char fetch_add( int, unsigned char,
                           memory_order = memory_order_seq_cst ) volatile;
    unsigned char fetch_sub( int, unsigned char,
                           memory_order = memory_order_seq_cst ) volatile;
    unsigned char fetch_and( int, unsigned char,
                           memory_order = memory_order_seq_cst ) volatile;
    unsigned char fetch_or( int,unsigned char,
                           memory_order = memory_order_seq_cst ) volatile;
    unsigned char fetch_xor( int, unsigned char,
                           memory_order = memory_order_seq_cst ) volatile;

    CPP0X( atomic_uchar() = default; )
    CPP0X( constexpr atomic_uchar( unsigned char __v__ ) : __f__( __v__) { } )
    CPP0X( atomic_uchar( const atomic_uchar& ) = delete; )
    atomic_uchar& operator =( const atomic_uchar& ) CPP0X(=delete);

    unsigned char operator =( unsigned char __v__ ) volatile
    { store(0,  __v__ ); return __v__; }

    unsigned char operator ++( int ) volatile
    { return fetch_add( 0, 1 ); }

    unsigned char operator --( int ) volatile
    { return fetch_sub( 0, 1 ); }

    unsigned char operator ++() volatile
    { return fetch_add( 0, 1 ) + 1; }

    unsigned char operator --() volatile
    { return fetch_sub( 0, 1 ) - 1; }

    unsigned char operator +=( unsigned char __v__ ) volatile
    { return fetch_add( 0, __v__ ) + __v__; }

    unsigned char operator -=( unsigned char __v__ ) volatile
    { return fetch_sub( 0, __v__ ) - __v__; }

    unsigned char operator &=( unsigned char __v__ ) volatile
    { return fetch_and( 0, __v__ ) & __v__; }

    unsigned char operator |=( unsigned char __v__ ) volatile
    { return fetch_or( 0, __v__ ) | __v__; }

    unsigned char operator ^=( unsigned char __v__ ) volatile
    { return fetch_xor( 0, __v__ ) ^ __v__; }

    friend void atomic_store_explicit( int, volatile atomic_uchar*, unsigned char,
                                       memory_order );
    friend unsigned char atomic_load_explicit( int, volatile atomic_uchar*,
                                             memory_order );
    friend unsigned char atomic_exchange_explicit( int, volatile atomic_uchar*,
                                             unsigned char, memory_order );
    friend bool atomic_compare_exchange_weak_explicit( int, volatile atomic_uchar*,
                      unsigned char*, unsigned char, memory_order, memory_order );
    friend bool atomic_compare_exchange_strong_explicit( int, volatile atomic_uchar*,
                      unsigned char*, unsigned char, memory_order, memory_order );
    friend unsigned char atomic_fetch_add_explicit( int, volatile atomic_uchar*,
                                                  unsigned char, memory_order );
    friend unsigned char atomic_fetch_sub_explicit( int, volatile atomic_uchar*,
                                                  unsigned char, memory_order );
    friend unsigned char atomic_fetch_and_explicit( int, volatile atomic_uchar*,
                                                  unsigned char, memory_order );
    friend unsigned char atomic_fetch_or_explicit( int,  volatile atomic_uchar*,
                                                  unsigned char, memory_order );
    friend unsigned char atomic_fetch_xor_explicit( int, volatile atomic_uchar*,
                                                  unsigned char, memory_order );

CPP0X(private:)
#endif
    unsigned char __f__;
} atomic_uchar;


typedef struct atomic_short
{
#ifdef __cplusplus
    bool is_lock_free() const volatile;
    void store( int, short,
                memory_order = memory_order_seq_cst ) volatile;
    short load( int,memory_order = memory_order_seq_cst ) volatile;
    short exchange( int, short,
                      memory_order = memory_order_seq_cst ) volatile;
    bool compare_exchange_weak( int,  short&, short,
                       memory_order, memory_order ) volatile;
    bool compare_exchange_strong( int, short&, short,
                       memory_order, memory_order ) volatile;
    bool compare_exchange_weak( int,  short&, short,
                       memory_order = memory_order_seq_cst ) volatile;
    bool compare_exchange_strong( int, short&, short,
                       memory_order = memory_order_seq_cst ) volatile;
    short fetch_add( int, short,
                           memory_order = memory_order_seq_cst ) volatile;
    short fetch_sub( int, short,
                           memory_order = memory_order_seq_cst ) volatile;
    short fetch_and( int, short,
                           memory_order = memory_order_seq_cst ) volatile;
    short fetch_or( int,short,
                           memory_order = memory_order_seq_cst ) volatile;
    short fetch_xor( int, short,
                           memory_order = memory_order_seq_cst ) volatile;

    CPP0X( atomic_short() = default; )
    CPP0X( constexpr atomic_short( short __v__ ) : __f__( __v__) { } )
    CPP0X( atomic_short( const atomic_short& ) = delete; )
    atomic_short& operator =( const atomic_short& ) CPP0X(=delete);

    short operator =( short __v__ ) volatile
    { store(0,  __v__ ); return __v__; }

    short operator ++( int ) volatile
    { return fetch_add( 0, 1 ); }

    short operator --( int ) volatile
    { return fetch_sub( 0, 1 ); }

    short operator ++() volatile
    { return fetch_add( 0, 1 ) + 1; }

    short operator --() volatile
    { return fetch_sub( 0, 1 ) - 1; }

    short operator +=( short __v__ ) volatile
    { return fetch_add( 0, __v__ ) + __v__; }

    short operator -=( short __v__ ) volatile
    { return fetch_sub( 0, __v__ ) - __v__; }

    short operator &=( short __v__ ) volatile
    { return fetch_and( 0, __v__ ) & __v__; }

    short operator |=( short __v__ ) volatile
    { return fetch_or( 0, __v__ ) | __v__; }

    short operator ^=( short __v__ ) volatile
    { return fetch_xor( 0, __v__ ) ^ __v__; }

    friend void atomic_store_explicit( int, volatile atomic_short*, short,
                                       memory_order );
    friend short atomic_load_explicit( int, volatile atomic_short*,
                                             memory_order );
    friend short atomic_exchange_explicit( int, volatile atomic_short*,
                                             short, memory_order );
    friend bool atomic_compare_exchange_weak_explicit( int, volatile atomic_short*,
                      short*, short, memory_order, memory_order );
    friend bool atomic_compare_exchange_strong_explicit( int, volatile atomic_short*,
                      short*, short, memory_order, memory_order );
    friend short atomic_fetch_add_explicit( int, volatile atomic_short*,
                                                  short, memory_order );
    friend short atomic_fetch_sub_explicit( int, volatile atomic_short*,
                                                  short, memory_order );
    friend short atomic_fetch_and_explicit( int, volatile atomic_short*,
                                                  short, memory_order );
    friend short atomic_fetch_or_explicit( int,  volatile atomic_short*,
                                                  short, memory_order );
    friend short atomic_fetch_xor_explicit( int, volatile atomic_short*,
                                                  short, memory_order );

CPP0X(private:)
#endif
    short __f__;
} atomic_short;


typedef struct atomic_ushort
{
#ifdef __cplusplus
    bool is_lock_free() const volatile;
    void store( int, unsigned short,
                memory_order = memory_order_seq_cst ) volatile;
    unsigned short load( int,memory_order = memory_order_seq_cst ) volatile;
    unsigned short exchange( int, unsigned short,
                      memory_order = memory_order_seq_cst ) volatile;
    bool compare_exchange_weak( int,  unsigned short&, unsigned short,
                       memory_order, memory_order ) volatile;
    bool compare_exchange_strong( int, unsigned short&, unsigned short,
                       memory_order, memory_order ) volatile;
    bool compare_exchange_weak( int,  unsigned short&, unsigned short,
                       memory_order = memory_order_seq_cst ) volatile;
    bool compare_exchange_strong( int, unsigned short&, unsigned short,
                       memory_order = memory_order_seq_cst ) volatile;
    unsigned short fetch_add( int, unsigned short,
                           memory_order = memory_order_seq_cst ) volatile;
    unsigned short fetch_sub( int, unsigned short,
                           memory_order = memory_order_seq_cst ) volatile;
    unsigned short fetch_and( int, unsigned short,
                           memory_order = memory_order_seq_cst ) volatile;
    unsigned short fetch_or( int,unsigned short,
                           memory_order = memory_order_seq_cst ) volatile;
    unsigned short fetch_xor( int, unsigned short,
                           memory_order = memory_order_seq_cst ) volatile;

    CPP0X( atomic_ushort() = default; )
    CPP0X( constexpr atomic_ushort( unsigned short __v__ ) : __f__( __v__) { } )
    CPP0X( atomic_ushort( const atomic_ushort& ) = delete; )
    atomic_ushort& operator =( const atomic_ushort& ) CPP0X(=delete);

    unsigned short operator =( unsigned short __v__ ) volatile
    { store(0,  __v__ ); return __v__; }

    unsigned short operator ++( int ) volatile
    { return fetch_add( 0, 1 ); }

    unsigned short operator --( int ) volatile
    { return fetch_sub( 0, 1 ); }

    unsigned short operator ++() volatile
    { return fetch_add( 0, 1 ) + 1; }

    unsigned short operator --() volatile
    { return fetch_sub( 0, 1 ) - 1; }

    unsigned short operator +=( unsigned short __v__ ) volatile
    { return fetch_add( 0, __v__ ) + __v__; }

    unsigned short operator -=( unsigned short __v__ ) volatile
    { return fetch_sub( 0, __v__ ) - __v__; }

    unsigned short operator &=( unsigned short __v__ ) volatile
    { return fetch_and( 0, __v__ ) & __v__; }

    unsigned short operator |=( unsigned short __v__ ) volatile
    { return fetch_or( 0, __v__ ) | __v__; }

    unsigned short operator ^=( unsigned short __v__ ) volatile
    { return fetch_xor( 0, __v__ ) ^ __v__; }

    friend void atomic_store_explicit( int, volatile atomic_ushort*, unsigned short,
                                       memory_order );
    friend unsigned short atomic_load_explicit( int, volatile atomic_ushort*,
                                             memory_order );
    friend unsigned short atomic_exchange_explicit( int, volatile atomic_ushort*,
                                             unsigned short, memory_order );
    friend bool atomic_compare_exchange_weak_explicit( int, volatile atomic_ushort*,
                      unsigned short*, unsigned short, memory_order, memory_order );
    friend bool atomic_compare_exchange_strong_explicit( int, volatile atomic_ushort*,
                      unsigned short*, unsigned short, memory_order, memory_order );
    friend unsigned short atomic_fetch_add_explicit( int, volatile atomic_ushort*,
                                                  unsigned short, memory_order );
    friend unsigned short atomic_fetch_sub_explicit( int, volatile atomic_ushort*,
                                                  unsigned short, memory_order );
    friend unsigned short atomic_fetch_and_explicit( int, volatile atomic_ushort*,
                                                  unsigned short, memory_order );
    friend unsigned short atomic_fetch_or_explicit( int,  volatile atomic_ushort*,
                                                  unsigned short, memory_order );
    friend unsigned short atomic_fetch_xor_explicit( int, volatile atomic_ushort*,
                                                  unsigned short, memory_order );

CPP0X(private:)
#endif
    unsigned short __f__;
} atomic_ushort;


typedef struct atomic_int
{
#ifdef __cplusplus
    bool is_lock_free() const volatile;
    void store( int, int,
                memory_order = memory_order_seq_cst ) volatile;
    int load( int,memory_order = memory_order_seq_cst ) volatile;
    int exchange( int, int,
                      memory_order = memory_order_seq_cst ) volatile;
    bool compare_exchange_weak( int,  int&, int,
                       memory_order, memory_order ) volatile;
    bool compare_exchange_strong( int, int&, int,
                       memory_order, memory_order ) volatile;
    bool compare_exchange_weak( int,  int&, int,
                       memory_order = memory_order_seq_cst ) volatile;
    bool compare_exchange_strong( int, int&, int,
                       memory_order = memory_order_seq_cst ) volatile;
    int fetch_add( int, int,
                           memory_order = memory_order_seq_cst ) volatile;
    int fetch_sub( int, int,
                           memory_order = memory_order_seq_cst ) volatile;
    int fetch_and( int, int,
                           memory_order = memory_order_seq_cst ) volatile;
    int fetch_or( int,int,
                           memory_order = memory_order_seq_cst ) volatile;
    int fetch_xor( int, int,
                           memory_order = memory_order_seq_cst ) volatile;

    CPP0X( atomic_int() = default; )
    CPP0X( constexpr atomic_int( int __v__ ) : __f__( __v__) { } )
    CPP0X( atomic_int( const atomic_int& ) = delete; )
    atomic_int& operator =( const atomic_int& ) CPP0X(=delete);

    int operator =( int __v__ ) volatile
    { store(0,  __v__ ); return __v__; }

    int operator ++( int ) volatile
    { return fetch_add( 0, 1 ); }

    int operator --( int ) volatile
    { return fetch_sub( 0, 1 ); }

    int operator ++() volatile
    { return fetch_add( 0, 1 ) + 1; }

    int operator --() volatile
    { return fetch_sub( 0, 1 ) - 1; }

    int operator +=( int __v__ ) volatile
    { return fetch_add( 0, __v__ ) + __v__; }

    int operator -=( int __v__ ) volatile
    { return fetch_sub( 0, __v__ ) - __v__; }

    int operator &=( int __v__ ) volatile
    { return fetch_and( 0, __v__ ) & __v__; }

    int operator |=( int __v__ ) volatile
    { return fetch_or( 0, __v__ ) | __v__; }

    int operator ^=( int __v__ ) volatile
    { return fetch_xor( 0, __v__ ) ^ __v__; }

    friend void atomic_store_explicit( int, volatile atomic_int*, int,
                                       memory_order );
    friend int atomic_load_explicit( int, volatile atomic_int*,
                                             memory_order );
    friend int atomic_exchange_explicit( int, volatile atomic_int*,
                                             int, memory_order );
    friend bool atomic_compare_exchange_weak_explicit( int, volatile atomic_int*,
                      int*, int, memory_order, memory_order );
    friend bool atomic_compare_exchange_strong_explicit( int, volatile atomic_int*,
                      int*, int, memory_order, memory_order );
    friend int atomic_fetch_add_explicit( int, volatile atomic_int*,
                                                  int, memory_order );
    friend int atomic_fetch_sub_explicit( int, volatile atomic_int*,
                                                  int, memory_order );
    friend int atomic_fetch_and_explicit( int, volatile atomic_int*,
                                                  int, memory_order );
    friend int atomic_fetch_or_explicit( int,  volatile atomic_int*,
                                                  int, memory_order );
    friend int atomic_fetch_xor_explicit( int, volatile atomic_int*,
                                                  int, memory_order );

CPP0X(private:)
#endif
    int __f__;
} atomic_int;


typedef struct atomic_uint
{
#ifdef __cplusplus
    bool is_lock_free() const volatile;
    void store( int, unsigned int,
                memory_order = memory_order_seq_cst ) volatile;
    unsigned int load( int, memory_order = memory_order_seq_cst ) volatile;
    unsigned int exchange( int, unsigned int,
                      memory_order = memory_order_seq_cst ) volatile;
    bool compare_exchange_weak( int,  unsigned int&, unsigned int,
                       memory_order, memory_order ) volatile;
    bool compare_exchange_strong( int, unsigned int&, unsigned int,
                       memory_order, memory_order ) volatile;
    bool compare_exchange_weak( int,  unsigned int&, unsigned int,
                       memory_order = memory_order_seq_cst ) volatile;
    bool compare_exchange_strong( int, unsigned int&, unsigned int,
                       memory_order = memory_order_seq_cst ) volatile;
    unsigned int fetch_add( int, unsigned int,
                           memory_order = memory_order_seq_cst ) volatile;
    unsigned int fetch_sub( int, unsigned int,
                           memory_order = memory_order_seq_cst ) volatile;
    unsigned int fetch_and( int, unsigned int,
                           memory_order = memory_order_seq_cst ) volatile;
    unsigned int fetch_or( int,unsigned int,
                           memory_order = memory_order_seq_cst ) volatile;
    unsigned int fetch_xor( int, unsigned int,
                           memory_order = memory_order_seq_cst ) volatile;

    CPP0X( atomic_uint() = default; )
    CPP0X( constexpr atomic_uint( unsigned int __v__ ) : __f__( __v__) { } )
    CPP0X( atomic_uint( const atomic_uint& ) = delete; )
    atomic_uint& operator =( const atomic_uint& ) CPP0X(=delete);

    unsigned int operator =( unsigned int __v__ ) volatile
    { store(0,  __v__ ); return __v__; }

    unsigned int operator ++( int ) volatile
    { return fetch_add( 0, 1 ); }

    unsigned int operator --( int ) volatile
    { return fetch_sub( 0, 1 ); }

    unsigned int operator ++() volatile
    { return fetch_add( 0, 1 ) + 1; }

    unsigned int operator --() volatile
    { return fetch_sub( 0, 1 ) - 1; }

    unsigned int operator +=( unsigned int __v__ ) volatile
    { return fetch_add( 0, __v__ ) + __v__; }

    unsigned int operator -=( unsigned int __v__ ) volatile
    { return fetch_sub( 0, __v__ ) - __v__; }

    unsigned int operator &=( unsigned int __v__ ) volatile
    { return fetch_and( 0, __v__ ) & __v__; }

    unsigned int operator |=( unsigned int __v__ ) volatile
    { return fetch_or( 0, __v__ ) | __v__; }

    unsigned int operator ^=( unsigned int __v__ ) volatile
    { return fetch_xor( 0, __v__ ) ^ __v__; }

    friend void atomic_store_explicit( int, volatile atomic_uint*, unsigned int,
                                       memory_order );
    friend unsigned int atomic_load_explicit( int, volatile atomic_uint*,
                                             memory_order );
    friend unsigned int atomic_exchange_explicit( int, volatile atomic_uint*,
                                             unsigned int, memory_order );
    friend bool atomic_compare_exchange_weak_explicit( int, volatile atomic_uint*,
                      unsigned int*, unsigned int, memory_order, memory_order );
    friend bool atomic_compare_exchange_strong_explicit( int, volatile atomic_uint*,
                      unsigned int*, unsigned int, memory_order, memory_order );
    friend unsigned int atomic_fetch_add_explicit( int, volatile atomic_uint*,
                                                  unsigned int, memory_order );
    friend unsigned int atomic_fetch_sub_explicit( int, volatile atomic_uint*,
                                                  unsigned int, memory_order );
    friend unsigned int atomic_fetch_and_explicit( int, volatile atomic_uint*,
                                                  unsigned int, memory_order );
    friend unsigned int atomic_fetch_or_explicit( int,  volatile atomic_uint*,
                                                  unsigned int, memory_order );
    friend unsigned int atomic_fetch_xor_explicit( int, volatile atomic_uint*,
                                                  unsigned int, memory_order );

CPP0X(private:)
#endif
    unsigned int __f__;
} atomic_uint;


typedef struct atomic_long
{
#ifdef __cplusplus
    bool is_lock_free() const volatile;
    void store( int, long,
                memory_order = memory_order_seq_cst ) volatile;
    long load( int, memory_order = memory_order_seq_cst ) volatile;
    long exchange( int, long,
                      memory_order = memory_order_seq_cst ) volatile;
    bool compare_exchange_weak( int,  long&, long,
                       memory_order, memory_order ) volatile;
    bool compare_exchange_strong( int, long&, long,
                       memory_order, memory_order ) volatile;
    bool compare_exchange_weak( int,  long&, long,
                       memory_order = memory_order_seq_cst ) volatile;
    bool compare_exchange_strong( int, long&, long,
                       memory_order = memory_order_seq_cst ) volatile;
    long fetch_add( int, long,
                           memory_order = memory_order_seq_cst ) volatile;
    long fetch_sub( int, long,
                           memory_order = memory_order_seq_cst ) volatile;
    long fetch_and( int, long,
                           memory_order = memory_order_seq_cst ) volatile;
    long fetch_or( int,long,
                           memory_order = memory_order_seq_cst ) volatile;
    long fetch_xor( int, long,
                           memory_order = memory_order_seq_cst ) volatile;

    CPP0X( atomic_long() = default; )
    CPP0X( constexpr atomic_long( long __v__ ) : __f__( __v__) { } )
    CPP0X( atomic_long( const atomic_long& ) = delete; )
    atomic_long& operator =( const atomic_long& ) CPP0X(=delete);

    long operator =( long __v__ ) volatile
    { store(0,  __v__ ); return __v__; }

    long operator ++( int ) volatile
    { return fetch_add( 0, 1 ); }

    long operator --( int ) volatile
    { return fetch_sub( 0, 1 ); }

    long operator ++() volatile
    { return fetch_add( 0, 1 ) + 1; }

    long operator --() volatile
    { return fetch_sub( 0, 1 ) - 1; }

    long operator +=( long __v__ ) volatile
    { return fetch_add( 0, __v__ ) + __v__; }

    long operator -=( long __v__ ) volatile
    { return fetch_sub( 0, __v__ ) - __v__; }

    long operator &=( long __v__ ) volatile
    { return fetch_and( 0, __v__ ) & __v__; }

    long operator |=( long __v__ ) volatile
    { return fetch_or( 0, __v__ ) | __v__; }

    long operator ^=( long __v__ ) volatile
    { return fetch_xor( 0, __v__ ) ^ __v__; }

    friend void atomic_store_explicit( int, volatile atomic_long*, long,
                                       memory_order );
    friend long atomic_load_explicit( int, volatile atomic_long*,
                                             memory_order );
    friend long atomic_exchange_explicit( int, volatile atomic_long*,
                                             long, memory_order );
    friend bool atomic_compare_exchange_weak_explicit( int, volatile atomic_long*,
                      long*, long, memory_order, memory_order );
    friend bool atomic_compare_exchange_strong_explicit( int, volatile atomic_long*,
                      long*, long, memory_order, memory_order );
    friend long atomic_fetch_add_explicit( int, volatile atomic_long*,
                                                  long, memory_order );
    friend long atomic_fetch_sub_explicit( int, volatile atomic_long*,
                                                  long, memory_order );
    friend long atomic_fetch_and_explicit( int, volatile atomic_long*,
                                                  long, memory_order );
    friend long atomic_fetch_or_explicit( int,  volatile atomic_long*,
                                                  long, memory_order );
    friend long atomic_fetch_xor_explicit( int, volatile atomic_long*,
                                                  long, memory_order );

CPP0X(private:)
#endif
    long __f__;
} atomic_long;


typedef struct atomic_ulong
{
#ifdef __cplusplus
    bool is_lock_free() const volatile;
    void store( int, unsigned long,
                memory_order = memory_order_seq_cst ) volatile;
    unsigned long load( int, memory_order = memory_order_seq_cst ) volatile;
    unsigned long exchange( int, unsigned long,
                      memory_order = memory_order_seq_cst ) volatile;
    bool compare_exchange_weak( int,  unsigned long&, unsigned long,
                       memory_order, memory_order ) volatile;
    bool compare_exchange_strong( int, unsigned long&, unsigned long,
                       memory_order, memory_order ) volatile;
    bool compare_exchange_weak( int,  unsigned long&, unsigned long,
                       memory_order = memory_order_seq_cst ) volatile;
    bool compare_exchange_strong( int, unsigned long&, unsigned long,
                       memory_order = memory_order_seq_cst ) volatile;
    unsigned long fetch_add( int, unsigned long,
                           memory_order = memory_order_seq_cst ) volatile;
    unsigned long fetch_sub( int, unsigned long,
                           memory_order = memory_order_seq_cst ) volatile;
    unsigned long fetch_and( int, unsigned long,
                           memory_order = memory_order_seq_cst ) volatile;
    unsigned long fetch_or( int,unsigned long,
                           memory_order = memory_order_seq_cst ) volatile;
    unsigned long fetch_xor( int, unsigned long,
                           memory_order = memory_order_seq_cst ) volatile;

    CPP0X( atomic_ulong() = default; )
    CPP0X( constexpr atomic_ulong( unsigned long __v__ ) : __f__( __v__) { } )
    CPP0X( atomic_ulong( const atomic_ulong& ) = delete; )
    atomic_ulong& operator =( const atomic_ulong& ) CPP0X(=delete);

    unsigned long operator =( unsigned long __v__ ) volatile
    { store(0,  __v__ ); return __v__; }

    unsigned long operator ++( int ) volatile
    { return fetch_add( 0, 1 ); }

    unsigned long operator --( int ) volatile
    { return fetch_sub( 0, 1 ); }

    unsigned long operator ++() volatile
    { return fetch_add( 0, 1 ) + 1; }

    unsigned long operator --() volatile
    { return fetch_sub( 0, 1 ) - 1; }

    unsigned long operator +=( unsigned long __v__ ) volatile
    { return fetch_add( 0, __v__ ) + __v__; }

    unsigned long operator -=( unsigned long __v__ ) volatile
    { return fetch_sub( 0, __v__ ) - __v__; }

    unsigned long operator &=( unsigned long __v__ ) volatile
    { return fetch_and( 0, __v__ ) & __v__; }

    unsigned long operator |=( unsigned long __v__ ) volatile
    { return fetch_or( 0, __v__ ) | __v__; }

    unsigned long operator ^=( unsigned long __v__ ) volatile
    { return fetch_xor( 0, __v__ ) ^ __v__; }

    friend void atomic_store_explicit( int, volatile atomic_ulong*, unsigned long,
                                       memory_order );
    friend unsigned long atomic_load_explicit( int, volatile atomic_ulong*,
                                             memory_order );
    friend unsigned long atomic_exchange_explicit( int, volatile atomic_ulong*,
                                             unsigned long, memory_order );
    friend bool atomic_compare_exchange_weak_explicit( int, volatile atomic_ulong*,
                      unsigned long*, unsigned long, memory_order, memory_order );
    friend bool atomic_compare_exchange_strong_explicit( int, volatile atomic_ulong*,
                      unsigned long*, unsigned long, memory_order, memory_order );
    friend unsigned long atomic_fetch_add_explicit( int, volatile atomic_ulong*,
                                                  unsigned long, memory_order );
    friend unsigned long atomic_fetch_sub_explicit( int, volatile atomic_ulong*,
                                                  unsigned long, memory_order );
    friend unsigned long atomic_fetch_and_explicit( int, volatile atomic_ulong*,
                                                  unsigned long, memory_order );
    friend unsigned long atomic_fetch_or_explicit( int,  volatile atomic_ulong*,
                                                  unsigned long, memory_order );
    friend unsigned long atomic_fetch_xor_explicit( int, volatile atomic_ulong*,
                                                  unsigned long, memory_order );

CPP0X(private:)
#endif
    unsigned long __f__;
} atomic_ulong;


typedef struct atomic_llong
{
#ifdef __cplusplus
    bool is_lock_free() const volatile;
    void store( int, long long,
                memory_order = memory_order_seq_cst ) volatile;
    long long load( int, memory_order = memory_order_seq_cst ) volatile;
    long long exchange( int, long long,
                      memory_order = memory_order_seq_cst ) volatile;
    bool compare_exchange_weak( int,  long long&, long long,
                       memory_order, memory_order ) volatile;
    bool compare_exchange_strong( int, long long&, long long,
                       memory_order, memory_order ) volatile;
    bool compare_exchange_weak( int,  long long&, long long,
                       memory_order = memory_order_seq_cst ) volatile;
    bool compare_exchange_strong( int, long long&, long long,
                       memory_order = memory_order_seq_cst ) volatile;
    long long fetch_add( int, long long,
                           memory_order = memory_order_seq_cst ) volatile;
    long long fetch_sub( int, long long,
                           memory_order = memory_order_seq_cst ) volatile;
    long long fetch_and( int, long long,
                           memory_order = memory_order_seq_cst ) volatile;
    long long fetch_or( int,long long,
                           memory_order = memory_order_seq_cst ) volatile;
    long long fetch_xor( int, long long,
                           memory_order = memory_order_seq_cst ) volatile;

    CPP0X( atomic_llong() = default; )
    CPP0X( constexpr atomic_llong( long long __v__ ) : __f__( __v__) { } )
    CPP0X( atomic_llong( const atomic_llong& ) = delete; )
    atomic_llong& operator =( const atomic_llong& ) CPP0X(=delete);

    long long operator =( long long __v__ ) volatile
    { store(0,  __v__ ); return __v__; }

    long long operator ++( int ) volatile
    { return fetch_add( 0, 1 ); }

    long long operator --( int ) volatile
    { return fetch_sub( 0, 1 ); }

    long long operator ++() volatile
    { return fetch_add( 0, 1 ) + 1; }

    long long operator --() volatile
    { return fetch_sub( 0, 1 ) - 1; }

    long long operator +=( long long __v__ ) volatile
    { return fetch_add( 0, __v__ ) + __v__; }

    long long operator -=( long long __v__ ) volatile
    { return fetch_sub( 0, __v__ ) - __v__; }

    long long operator &=( long long __v__ ) volatile
    { return fetch_and( 0, __v__ ) & __v__; }

    long long operator |=( long long __v__ ) volatile
    { return fetch_or( 0, __v__ ) | __v__; }

    long long operator ^=( long long __v__ ) volatile
    { return fetch_xor( 0, __v__ ) ^ __v__; }

    friend void atomic_store_explicit( int, volatile atomic_llong*, long long,
                                       memory_order );
    friend long long atomic_load_explicit( int, volatile atomic_llong*,
                                             memory_order );
    friend long long atomic_exchange_explicit( int, volatile atomic_llong*,
                                             long long, memory_order );
    friend bool atomic_compare_exchange_weak_explicit( int, volatile atomic_llong*,
                      long long*, long long, memory_order, memory_order );
    friend bool atomic_compare_exchange_strong_explicit( int, volatile atomic_llong*,
                      long long*, long long, memory_order, memory_order );
    friend long long atomic_fetch_add_explicit( int, volatile atomic_llong*,
                                                  long long, memory_order );
    friend long long atomic_fetch_sub_explicit( int, volatile atomic_llong*,
                                                  long long, memory_order );
    friend long long atomic_fetch_and_explicit( int, volatile atomic_llong*,
                                                  long long, memory_order );
    friend long long atomic_fetch_or_explicit( int,  volatile atomic_llong*,
                                                  long long, memory_order );
    friend long long atomic_fetch_xor_explicit( int, volatile atomic_llong*,
                                                  long long, memory_order );

CPP0X(private:)
#endif
    long long __f__;
} atomic_llong;


typedef struct atomic_ullong
{
#ifdef __cplusplus
    bool is_lock_free() const volatile;
    void store( int, unsigned long long,
                memory_order = memory_order_seq_cst ) volatile;
    unsigned long long load( int, memory_order = memory_order_seq_cst ) volatile;
    unsigned long long exchange( int, unsigned long long,
                      memory_order = memory_order_seq_cst ) volatile;
    bool compare_exchange_weak( int,  unsigned long long&, unsigned long long,
                       memory_order, memory_order ) volatile;
    bool compare_exchange_strong( int, unsigned long long&, unsigned long long,
                       memory_order, memory_order ) volatile;
    bool compare_exchange_weak( int,  unsigned long long&, unsigned long long,
                       memory_order = memory_order_seq_cst ) volatile;
    bool compare_exchange_strong( int, unsigned long long&, unsigned long long,
                       memory_order = memory_order_seq_cst ) volatile;
    unsigned long long fetch_add( int, unsigned long long,
                           memory_order = memory_order_seq_cst ) volatile;
    unsigned long long fetch_sub( int, unsigned long long,
                           memory_order = memory_order_seq_cst ) volatile;
    unsigned long long fetch_and( int, unsigned long long,
                           memory_order = memory_order_seq_cst ) volatile;
    unsigned long long fetch_or( int,unsigned long long,
                           memory_order = memory_order_seq_cst ) volatile;
    unsigned long long fetch_xor( int, unsigned long long,
                           memory_order = memory_order_seq_cst ) volatile;

    CPP0X( atomic_ullong() = default; )
    CPP0X( constexpr atomic_ullong( unsigned long long __v__ ) : __f__( __v__) { } )
    CPP0X( atomic_ullong( const atomic_ullong& ) = delete; )
    atomic_ullong& operator =( const atomic_ullong& ) CPP0X(=delete);

    unsigned long long operator =( unsigned long long __v__ ) volatile
    { store(0,  __v__ ); return __v__; }

    unsigned long long operator ++( int ) volatile
    { return fetch_add( 0, 1 ); }

    unsigned long long operator --( int ) volatile
    { return fetch_sub( 0, 1 ); }

    unsigned long long operator ++() volatile
    { return fetch_add( 0, 1 ) + 1; }

    unsigned long long operator --() volatile
    { return fetch_sub( 0, 1 ) - 1; }

    unsigned long long operator +=( unsigned long long __v__ ) volatile
    { return fetch_add( 0, __v__ ) + __v__; }

    unsigned long long operator -=( unsigned long long __v__ ) volatile
    { return fetch_sub( 0, __v__ ) - __v__; }

    unsigned long long operator &=( unsigned long long __v__ ) volatile
    { return fetch_and( 0, __v__ ) & __v__; }

    unsigned long long operator |=( unsigned long long __v__ ) volatile
    { return fetch_or( 0, __v__ ) | __v__; }

    unsigned long long operator ^=( unsigned long long __v__ ) volatile
    { return fetch_xor( 0, __v__ ) ^ __v__; }

    friend void atomic_store_explicit( int, volatile atomic_ullong*, unsigned long long,
                                       memory_order );
    friend unsigned long long atomic_load_explicit( int, volatile atomic_ullong*,
                                             memory_order );
    friend unsigned long long atomic_exchange_explicit( int, volatile atomic_ullong*,
                                             unsigned long long, memory_order );
    friend bool atomic_compare_exchange_weak_explicit( int, volatile atomic_ullong*,
                      unsigned long long*, unsigned long long, memory_order, memory_order );
    friend bool atomic_compare_exchange_strong_explicit( int, volatile atomic_ullong*,
                      unsigned long long*, unsigned long long, memory_order, memory_order );
    friend unsigned long long atomic_fetch_add_explicit( int, volatile atomic_ullong*,
                                                  unsigned long long, memory_order );
    friend unsigned long long atomic_fetch_sub_explicit( int, volatile atomic_ullong*,
                                                  unsigned long long, memory_order );
    friend unsigned long long atomic_fetch_and_explicit( int, volatile atomic_ullong*,
                                                  unsigned long long, memory_order );
    friend unsigned long long atomic_fetch_or_explicit( int,  volatile atomic_ullong*,
                                                  unsigned long long, memory_order );
    friend unsigned long long atomic_fetch_xor_explicit( int, volatile atomic_ullong*,
                                                  unsigned long long, memory_order );

CPP0X(private:)
#endif
    unsigned long long __f__;
} atomic_ullong;


typedef atomic_schar atomic_int_least8_t;
typedef atomic_uchar atomic_uint_least8_t;
typedef atomic_short atomic_int_least16_t;
typedef atomic_ushort atomic_uint_least16_t;
typedef atomic_int atomic_int_least32_t;
typedef atomic_uint atomic_uint_least32_t;
typedef atomic_llong atomic_int_least64_t;
typedef atomic_ullong atomic_uint_least64_t;

typedef atomic_schar atomic_int_fast8_t;
typedef atomic_uchar atomic_uint_fast8_t;
typedef atomic_short atomic_int_fast16_t;
typedef atomic_ushort atomic_uint_fast16_t;
typedef atomic_int atomic_int_fast32_t;
typedef atomic_uint atomic_uint_fast32_t;
typedef atomic_llong atomic_int_fast64_t;
typedef atomic_ullong atomic_uint_fast64_t;

typedef atomic_long atomic_intptr_t;
typedef atomic_ulong atomic_uintptr_t;

typedef atomic_long atomic_ssize_t;
typedef atomic_ulong atomic_size_t;

typedef atomic_long atomic_ptrdiff_t;

typedef atomic_llong atomic_intmax_t;
typedef atomic_ullong atomic_uintmax_t;


#ifdef __cplusplus


typedef struct atomic_wchar_t
{
#ifdef __cplusplus
    bool is_lock_free() const volatile;
    void store( int, wchar_t, memory_order = memory_order_seq_cst ) volatile;
    wchar_t load( int, memory_order = memory_order_seq_cst ) volatile;
    wchar_t exchange( int, wchar_t,
                      memory_order = memory_order_seq_cst ) volatile;
    bool compare_exchange_weak( int,  wchar_t&, wchar_t,
                       memory_order, memory_order ) volatile;
    bool compare_exchange_strong( int, wchar_t&, wchar_t,
                       memory_order, memory_order ) volatile;
    bool compare_exchange_weak( int,  wchar_t&, wchar_t,
                       memory_order = memory_order_seq_cst ) volatile;
    bool compare_exchange_strong( int, wchar_t&, wchar_t,
                       memory_order = memory_order_seq_cst ) volatile;
    wchar_t fetch_add( int, wchar_t,
                           memory_order = memory_order_seq_cst ) volatile;
    wchar_t fetch_sub( int, wchar_t,
                           memory_order = memory_order_seq_cst ) volatile;
    wchar_t fetch_and( int, wchar_t,
                           memory_order = memory_order_seq_cst ) volatile;
    wchar_t fetch_or( int,wchar_t,
                           memory_order = memory_order_seq_cst ) volatile;
    wchar_t fetch_xor( int, wchar_t,
                           memory_order = memory_order_seq_cst ) volatile;

    CPP0X( atomic_wchar_t() = default; )
    CPP0X( constexpr atomic_wchar_t( wchar_t __v__ ) : __f__( __v__) { } )
    CPP0X( atomic_wchar_t( const atomic_wchar_t& ) = delete; )
    atomic_wchar_t& operator =( const atomic_wchar_t& ) CPP0X(=delete);

    wchar_t operator =( wchar_t __v__ ) volatile
    { store(0,  __v__ ); return __v__; }

    wchar_t operator ++( int ) volatile
    { return fetch_add( 0, 1 ); }

    wchar_t operator --( int ) volatile
    { return fetch_sub( 0, 1 ); }

    wchar_t operator ++() volatile
    { return fetch_add( 0, 1 ) + 1; }

    wchar_t operator --() volatile
    { return fetch_sub( 0, 1 ) - 1; }

    wchar_t operator +=( wchar_t __v__ ) volatile
    { return fetch_add( 0, __v__ ) + __v__; }

    wchar_t operator -=( wchar_t __v__ ) volatile
    { return fetch_sub( 0, __v__ ) - __v__; }

    wchar_t operator &=( wchar_t __v__ ) volatile
    { return fetch_and( 0, __v__ ) & __v__; }

    wchar_t operator |=( wchar_t __v__ ) volatile
    { return fetch_or( 0, __v__ ) | __v__; }

    wchar_t operator ^=( wchar_t __v__ ) volatile
    { return fetch_xor( 0, __v__ ) ^ __v__; }

    friend void atomic_store_explicit( int, volatile atomic_wchar_t*, wchar_t,
                                       memory_order );
    friend wchar_t atomic_load_explicit( int, volatile atomic_wchar_t*,
                                             memory_order );
    friend wchar_t atomic_exchange_explicit( int, volatile atomic_wchar_t*,
                                             wchar_t, memory_order );
    friend bool atomic_compare_exchange_weak_explicit( int, volatile atomic_wchar_t*,
                    wchar_t*, wchar_t, memory_order, memory_order );
    friend bool atomic_compare_exchange_strong_explicit( int, volatile atomic_wchar_t*,
                    wchar_t*, wchar_t, memory_order, memory_order );
    friend wchar_t atomic_fetch_add_explicit( int, volatile atomic_wchar_t*,
                                                  wchar_t, memory_order );
    friend wchar_t atomic_fetch_sub_explicit( int, volatile atomic_wchar_t*,
                                                  wchar_t, memory_order );
    friend wchar_t atomic_fetch_and_explicit( int, volatile atomic_wchar_t*,
                                                  wchar_t, memory_order );
    friend wchar_t atomic_fetch_or_explicit( int, volatile atomic_wchar_t*,
                                                  wchar_t, memory_order );
    friend wchar_t atomic_fetch_xor_explicit( int, volatile atomic_wchar_t*,
                                                  wchar_t, memory_order );

CPP0X(private:)
#endif
    wchar_t __f__;
} atomic_wchar_t;


#else

typedef atomic_int_least16_t atomic_char16_t;
typedef atomic_int_least32_t atomic_char32_t;
typedef atomic_int_least32_t atomic_wchar_t;

#endif


#ifdef __cplusplus

template< typename T >
struct atomic
{
#ifdef __cplusplus

    bool is_lock_free() const volatile;
    void store( int, T, memory_order = memory_order_seq_cst ) volatile;
    T load( int, memory_order = memory_order_seq_cst ) volatile;
    T exchange( int, T __v__, memory_order = memory_order_seq_cst ) volatile;
    bool compare_exchange_weak( int,  T&, T, memory_order, memory_order ) volatile;
    bool compare_exchange_strong( int, T&, T, memory_order, memory_order ) volatile;
    bool compare_exchange_weak( int,  T&, T, memory_order = memory_order_seq_cst ) volatile;
    bool compare_exchange_strong( int, T&, T, memory_order = memory_order_seq_cst ) volatile;

    CPP0X( atomic() = default; )
    CPP0X( constexpr explicit atomic( T __v__ ) : __f__( __v__ ) { } )
    CPP0X( atomic( const atomic& ) = delete; )
    atomic& operator =( const atomic& ) CPP0X(=delete);

    T operator =( T __v__ ) volatile
    { store(0,  __v__ ); return __v__; }

CPP0X(private:)
#endif
    T __f__;
};

#endif

#ifdef __cplusplus

template<typename T> struct atomic< T* > : atomic_address
{
    T* load( int, memory_order = memory_order_seq_cst ) volatile;
    T* exchange( int, T*, memory_order = memory_order_seq_cst ) volatile;
    bool compare_exchange_weak( int,  T*&, T*, memory_order, memory_order ) volatile;
    bool compare_exchange_strong( int, T*&, T*, memory_order, memory_order ) volatile;
    bool compare_exchange_weak( int,  T*&, T*,
                       memory_order = memory_order_seq_cst ) volatile;
    bool compare_exchange_strong( int, T*&, T*,
                       memory_order = memory_order_seq_cst ) volatile;
    T* fetch_add( int, ptrdiff_t, memory_order = memory_order_seq_cst ) volatile;
    T* fetch_sub( int, ptrdiff_t, memory_order = memory_order_seq_cst ) volatile;

    CPP0X( atomic() = default; )
    CPP0X( constexpr explicit atomic( T __v__ ) : atomic_address( __v__ ) { } )
    CPP0X( atomic( const atomic& ) = delete; )
    atomic& operator =( const atomic& ) CPP0X(=delete);

    T* operator =( T* __v__ ) volatile
    { store(0,  __v__ ); return __v__; }

    T* operator ++( int ) volatile
    { return fetch_add( 0, 1 ); }

    T* operator --( int ) volatile
    { return fetch_sub( 0, 1 ); }

    T* operator ++() volatile
    { return fetch_add( 0, 1 ) + 1; }

    T* operator --() volatile
    { return fetch_sub( 0, 1 ) - 1; }

    T* operator +=( T* __v__ ) volatile
    { return fetch_add( 0, __v__ ) + __v__; }

    T* operator -=( T* __v__ ) volatile
    { return fetch_sub( 0, __v__ ) - __v__; }
};

#endif

#ifdef __cplusplus


template<> struct atomic< bool > : atomic_bool
{
    CPP0X( atomic() = default; )
    CPP0X( constexpr explicit atomic( bool __v__ )
    : atomic_bool( __v__ ) { } )
    CPP0X( atomic( const atomic& ) = delete; )
    atomic& operator =( const atomic& ) CPP0X(=delete);

    bool operator =( bool __v__ ) volatile
    { store(0,  __v__ ); return __v__; }
};


template<> struct atomic< void* > : atomic_address
{
    CPP0X( atomic() = default; )
    CPP0X( constexpr explicit atomic( void* __v__ )
    : atomic_address( __v__ ) { } )
    CPP0X( atomic( const atomic& ) = delete; )
    atomic& operator =( const atomic& ) CPP0X(=delete);

    void* operator =( void* __v__ ) volatile
    { store(0,  __v__ ); return __v__; }
};


template<> struct atomic< char > : atomic_char
{
    CPP0X( atomic() = default; )
    CPP0X( constexpr explicit atomic( char __v__ )
    : atomic_char( __v__ ) { } )
    CPP0X( atomic( const atomic& ) = delete; )
    atomic& operator =( const atomic& ) CPP0X(=delete);

    char operator =( char __v__ ) volatile
    { store(0,  __v__ ); return __v__; }
};


template<> struct atomic< signed char > : atomic_schar
{
    CPP0X( atomic() = default; )
    CPP0X( constexpr explicit atomic( signed char __v__ )
    : atomic_schar( __v__ ) { } )
    CPP0X( atomic( const atomic& ) = delete; )
    atomic& operator =( const atomic& ) CPP0X(=delete);

    signed char operator =( signed char __v__ ) volatile
    { store(0,  __v__ ); return __v__; }
};


template<> struct atomic< unsigned char > : atomic_uchar
{
    CPP0X( atomic() = default; )
    CPP0X( constexpr explicit atomic( unsigned char __v__ )
    : atomic_uchar( __v__ ) { } )
    CPP0X( atomic( const atomic& ) = delete; )
    atomic& operator =( const atomic& ) CPP0X(=delete);

    unsigned char operator =( unsigned char __v__ ) volatile
    { store(0,  __v__ ); return __v__; }
};


template<> struct atomic< short > : atomic_short
{
    CPP0X( atomic() = default; )
    CPP0X( constexpr explicit atomic( short __v__ )
    : atomic_short( __v__ ) { } )
    CPP0X( atomic( const atomic& ) = delete; )
    atomic& operator =( const atomic& ) CPP0X(=delete);

    short operator =( short __v__ ) volatile
    { store(0,  __v__ ); return __v__; }
};


template<> struct atomic< unsigned short > : atomic_ushort
{
    CPP0X( atomic() = default; )
    CPP0X( constexpr explicit atomic( unsigned short __v__ )
    : atomic_ushort( __v__ ) { } )
    CPP0X( atomic( const atomic& ) = delete; )
    atomic& operator =( const atomic& ) CPP0X(=delete);

    unsigned short operator =( unsigned short __v__ ) volatile
    { store(0,  __v__ ); return __v__; }
};


template<> struct atomic< int > : atomic_int
{
    CPP0X( atomic() = default; )
    CPP0X( constexpr explicit atomic( int __v__ )
    : atomic_int( __v__ ) { } )
    CPP0X( atomic( const atomic& ) = delete; )
    atomic& operator =( const atomic& ) CPP0X(=delete);

    int operator =( int __v__ ) volatile
    { store(0,  __v__ ); return __v__; }
};


template<> struct atomic< unsigned int > : atomic_uint
{
    CPP0X( atomic() = default; )
    CPP0X( constexpr explicit atomic( unsigned int __v__ )
    : atomic_uint( __v__ ) { } )
    CPP0X( atomic( const atomic& ) = delete; )
    atomic& operator =( const atomic& ) CPP0X(=delete);

    unsigned int operator =( unsigned int __v__ ) volatile
    { store(0,  __v__ ); return __v__; }
};


template<> struct atomic< long > : atomic_long
{
    CPP0X( atomic() = default; )
    CPP0X( constexpr explicit atomic( long __v__ )
    : atomic_long( __v__ ) { } )
    CPP0X( atomic( const atomic& ) = delete; )
    atomic& operator =( const atomic& ) CPP0X(=delete);

    long operator =( long __v__ ) volatile
    { store(0,  __v__ ); return __v__; }
};


template<> struct atomic< unsigned long > : atomic_ulong
{
    CPP0X( atomic() = default; )
    CPP0X( constexpr explicit atomic( unsigned long __v__ )
    : atomic_ulong( __v__ ) { } )
    CPP0X( atomic( const atomic& ) = delete; )
    atomic& operator =( const atomic& ) CPP0X(=delete);

    unsigned long operator =( unsigned long __v__ ) volatile
    { store(0,  __v__ ); return __v__; }
};


template<> struct atomic< long long > : atomic_llong
{
    CPP0X( atomic() = default; )
    CPP0X( constexpr explicit atomic( long long __v__ )
    : atomic_llong( __v__ ) { } )
    CPP0X( atomic( const atomic& ) = delete; )
    atomic& operator =( const atomic& ) CPP0X(=delete);

    long long operator =( long long __v__ ) volatile
    { store(0,  __v__ ); return __v__; }
};


template<> struct atomic< unsigned long long > : atomic_ullong
{
    CPP0X( atomic() = default; )
    CPP0X( constexpr explicit atomic( unsigned long long __v__ )
    : atomic_ullong( __v__ ) { } )
    CPP0X( atomic( const atomic& ) = delete; )
    atomic& operator =( const atomic& ) CPP0X(=delete);

    unsigned long long operator =( unsigned long long __v__ ) volatile
    { store(0,  __v__ ); return __v__; }
};


template<> struct atomic< wchar_t > : atomic_wchar_t
{
    CPP0X( atomic() = default; )
    CPP0X( constexpr explicit atomic( wchar_t __v__ )
    : atomic_wchar_t( __v__ ) { } )
    CPP0X( atomic( const atomic& ) = delete; )
    atomic& operator =( const atomic& ) CPP0X(=delete);

    wchar_t operator =( wchar_t __v__ ) volatile
    { store(0,  __v__ ); return __v__; }
};


#endif


#ifdef __cplusplus


inline bool atomic_is_lock_free
( const volatile atomic_bool* __a__ )
{ return false; }

inline bool atomic_load_explicit
( int line_no, volatile atomic_bool* __a__, memory_order __x__ )
{ return _ATOMIC_LOAD_( line_no, __a__, __x__ ); }

inline bool atomic_load
(  int line_no, volatile atomic_bool* __a__ ) { return atomic_load_explicit( line_no, __a__, memory_order_seq_cst ); }

inline void atomic_init
( volatile atomic_bool* __a__, bool __m__ )
{ _ATOMIC_INIT_( __a__, __m__ ); }

inline void atomic_store_explicit
( int line_no, volatile atomic_bool* __a__, bool __m__, memory_order __x__ )
{ _ATOMIC_STORE_( line_no, __a__, __m__, __x__ ); }

inline void atomic_store
( int line_no, volatile atomic_bool* __a__, bool __m__ )
{ atomic_store_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }

inline bool atomic_exchange_explicit
( int line_no, volatile atomic_bool* __a__, bool __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, =, __m__, __x__ ); }

inline bool atomic_exchange
( int line_no, volatile atomic_bool* __a__, bool __m__ )
{ return atomic_exchange_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }

inline bool atomic_compare_exchange_weak_explicit
( int line_no, volatile atomic_bool* __a__, bool* __e__, bool __m__,
  memory_order __x__, memory_order __y__ )
{ return _ATOMIC_CMPSWP_WEAK_( line_no, __a__, __e__, __m__, __x__ ); }

inline bool atomic_compare_exchange_strong_explicit
( int line_no, volatile atomic_bool* __a__, bool* __e__, bool __m__,
  memory_order __x__, memory_order __y__ )
{ return _ATOMIC_CMPSWP_( line_no, __a__, __e__, __m__, __x__ ); }

inline bool atomic_compare_exchange_weak
( int line_no, volatile atomic_bool* __a__, bool* __e__, bool __m__ )
{ return atomic_compare_exchange_weak_explicit( line_no, __a__, __e__, __m__,
                 memory_order_seq_cst, memory_order_seq_cst ); }

inline bool atomic_compare_exchange_strong
( int line_no, volatile atomic_bool* __a__, bool* __e__, bool __m__ )
{ return atomic_compare_exchange_strong_explicit( line_no, __a__, __e__, __m__,
                 memory_order_seq_cst, memory_order_seq_cst ); }


inline bool atomic_is_lock_free( const volatile atomic_address* __a__ )
{ return false; }

inline void* atomic_load_explicit
( int line_no, volatile atomic_address* __a__, memory_order __x__ )
{ return _ATOMIC_LOAD_( line_no, __a__, __x__ ); }

inline void* atomic_load( int line_no, volatile atomic_address* __a__ )
{ return atomic_load_explicit( line_no, __a__, memory_order_seq_cst ); }

inline void atomic_init
( volatile atomic_address* __a__, void* __m__ )
{ _ATOMIC_INIT_( __a__, __m__ ); }

inline void atomic_store_explicit
( int line_no, volatile atomic_address* __a__, void* __m__, memory_order __x__ )
{ _ATOMIC_STORE_( line_no, __a__, __m__, __x__ ); }

inline void atomic_store
( int line_no, volatile atomic_address* __a__, void* __m__ )
{ atomic_store_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }

inline void* atomic_exchange_explicit
( int line_no, volatile atomic_address* __a__, void* __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, =, __m__,  __x__ ); }

inline void* atomic_exchange
( int line_no, volatile atomic_address* __a__, void* __m__ )
{ return atomic_exchange_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }

inline bool atomic_compare_exchange_weak_explicit
( int line_no, volatile atomic_address* __a__, void** __e__, void* __m__,
  memory_order __x__, memory_order __y__ )
{ return _ATOMIC_CMPSWP_WEAK_( line_no, __a__, __e__, __m__, __x__ ); }

inline bool atomic_compare_exchange_strong_explicit
( int line_no, volatile atomic_address* __a__, void** __e__, void* __m__,
  memory_order __x__, memory_order __y__ )
{ return _ATOMIC_CMPSWP_( line_no, __a__, __e__, __m__, __x__ ); }

inline bool atomic_compare_exchange_weak
( int line_no, volatile atomic_address* __a__, void** __e__, void* __m__ )
{ return atomic_compare_exchange_weak_explicit( line_no, __a__, __e__, __m__,
                 memory_order_seq_cst, memory_order_seq_cst ); }

inline bool atomic_compare_exchange_strong
( int line_no, volatile atomic_address* __a__, void** __e__, void* __m__ )
{ return atomic_compare_exchange_strong_explicit( line_no, __a__, __e__, __m__,
                 memory_order_seq_cst, memory_order_seq_cst ); }


inline bool atomic_is_lock_free( const volatile atomic_char* __a__ )
{ return false; }

inline char atomic_load_explicit
( int line_no, volatile atomic_char* __a__, memory_order __x__ )
{ return _ATOMIC_LOAD_( line_no, __a__, __x__ ); }

inline char atomic_load( int line_no, volatile atomic_char* __a__ )
{ return atomic_load_explicit( line_no, __a__, memory_order_seq_cst ); }

inline void atomic_init
( volatile atomic_char* __a__, char __m__ )
{ _ATOMIC_INIT_( __a__, __m__ ); }

inline void atomic_store_explicit
( int line_no, volatile atomic_char* __a__, char __m__, memory_order __x__ )
{ _ATOMIC_STORE_( line_no, __a__, __m__, __x__ ); }

inline void atomic_store
( int line_no, volatile atomic_char* __a__, char __m__ )
{ atomic_store_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }

inline char atomic_exchange_explicit
( int line_no, volatile atomic_char* __a__, char __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, =, __m__, __x__ ); }

inline char atomic_exchange
( int line_no, volatile atomic_char* __a__, char __m__ )
{ return atomic_exchange_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }

inline bool atomic_compare_exchange_weak_explicit
( int line_no, volatile atomic_char* __a__, char* __e__, char __m__,
  memory_order __x__, memory_order __y__ )
{ return _ATOMIC_CMPSWP_WEAK_( line_no, __a__, __e__, __m__, __x__ ); }

inline bool atomic_compare_exchange_strong_explicit
( int line_no, volatile atomic_char* __a__, char* __e__, char __m__,
  memory_order __x__, memory_order __y__ )
{ return _ATOMIC_CMPSWP_( line_no, __a__, __e__, __m__, __x__ ); }

inline bool atomic_compare_exchange_weak
( int line_no, volatile atomic_char* __a__, char* __e__, char __m__ )
{ return atomic_compare_exchange_weak_explicit( line_no, __a__, __e__, __m__,
                 memory_order_seq_cst, memory_order_seq_cst ); }

inline bool atomic_compare_exchange_strong
( int line_no, volatile atomic_char* __a__, char* __e__, char __m__ )
{ return atomic_compare_exchange_strong_explicit( line_no, __a__, __e__, __m__,
                 memory_order_seq_cst, memory_order_seq_cst ); }


inline bool atomic_is_lock_free( const volatile atomic_schar* __a__ )
{ return false; }

inline signed char atomic_load_explicit
( int line_no, volatile atomic_schar* __a__, memory_order __x__ )
{ return _ATOMIC_LOAD_( line_no, __a__, __x__ ); }

inline signed char atomic_load( int line_no, volatile atomic_schar* __a__ )
{ return atomic_load_explicit( line_no, __a__, memory_order_seq_cst ); }

inline void atomic_init
( volatile atomic_schar* __a__, signed char __m__ )
{ _ATOMIC_INIT_( __a__, __m__ ); }

inline void atomic_store_explicit
( int line_no, volatile atomic_schar* __a__, signed char __m__, memory_order __x__ )
{ _ATOMIC_STORE_( line_no, __a__, __m__, __x__ ); }

inline void atomic_store
( int line_no, volatile atomic_schar* __a__, signed char __m__ )
{ atomic_store_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }

inline signed char atomic_exchange_explicit
( int line_no, volatile atomic_schar* __a__, signed char __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, =, __m__, __x__ ); }

inline signed char atomic_exchange
( int line_no, volatile atomic_schar* __a__, signed char __m__ )
{ return atomic_exchange_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }

inline bool atomic_compare_exchange_weak_explicit
( int line_no, volatile atomic_schar* __a__, signed char* __e__, signed char __m__,
  memory_order __x__, memory_order __y__ )
{ return _ATOMIC_CMPSWP_WEAK_( line_no, __a__, __e__, __m__, __x__ ); }

inline bool atomic_compare_exchange_strong_explicit
( int line_no, volatile atomic_schar* __a__, signed char* __e__, signed char __m__,
  memory_order __x__, memory_order __y__ )
{ return _ATOMIC_CMPSWP_( line_no, __a__, __e__, __m__, __x__ ); }

inline bool atomic_compare_exchange_weak
( int line_no, volatile atomic_schar* __a__, signed char* __e__, signed char __m__ )
{ return atomic_compare_exchange_weak_explicit( line_no, __a__, __e__, __m__,
                 memory_order_seq_cst, memory_order_seq_cst ); }

inline bool atomic_compare_exchange_strong
( int line_no, volatile atomic_schar* __a__, signed char* __e__, signed char __m__ )
{ return atomic_compare_exchange_strong_explicit( line_no, __a__, __e__, __m__,
                 memory_order_seq_cst, memory_order_seq_cst ); }


inline bool atomic_is_lock_free( const volatile atomic_uchar* __a__ )
{ return false; }

inline unsigned char atomic_load_explicit
( int line_no, volatile atomic_uchar* __a__, memory_order __x__ )
{ return _ATOMIC_LOAD_( line_no, __a__, __x__ ); }

inline unsigned char atomic_load( int line_no, volatile atomic_uchar* __a__ )
{ return atomic_load_explicit( line_no, __a__, memory_order_seq_cst ); }

inline void atomic_init
( volatile atomic_uchar* __a__, unsigned char __m__ )
{ _ATOMIC_INIT_( __a__, __m__ ); }

inline void atomic_store_explicit
( int line_no, volatile atomic_uchar* __a__, unsigned char __m__, memory_order __x__ )
{ _ATOMIC_STORE_( line_no, __a__, __m__, __x__ ); }

inline void atomic_store
( int line_no, volatile atomic_uchar* __a__, unsigned char __m__ )
{ atomic_store_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }

inline unsigned char atomic_exchange_explicit
( int line_no, volatile atomic_uchar* __a__, unsigned char __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, =, __m__, __x__ ); }

inline unsigned char atomic_exchange
( int line_no, volatile atomic_uchar* __a__, unsigned char __m__ )
{ return atomic_exchange_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }

inline bool atomic_compare_exchange_weak_explicit
( int line_no, volatile atomic_uchar* __a__, unsigned char* __e__, unsigned char __m__,
  memory_order __x__, memory_order __y__ )
{ return _ATOMIC_CMPSWP_WEAK_( line_no, __a__, __e__, __m__, __x__ ); }

inline bool atomic_compare_exchange_strong_explicit
( int line_no, volatile atomic_uchar* __a__, unsigned char* __e__, unsigned char __m__,
  memory_order __x__, memory_order __y__ )
{ return _ATOMIC_CMPSWP_( line_no, __a__, __e__, __m__, __x__ ); }

inline bool atomic_compare_exchange_weak
( int line_no, volatile atomic_uchar* __a__, unsigned char* __e__, unsigned char __m__ )
{ return atomic_compare_exchange_weak_explicit( line_no, __a__, __e__, __m__,
                 memory_order_seq_cst, memory_order_seq_cst ); }

inline bool atomic_compare_exchange_strong
( int line_no, volatile atomic_uchar* __a__, unsigned char* __e__, unsigned char __m__ )
{ return atomic_compare_exchange_strong_explicit( line_no, __a__, __e__, __m__,
                 memory_order_seq_cst, memory_order_seq_cst ); }


inline bool atomic_is_lock_free( const volatile atomic_short* __a__ )
{ return false; }

inline short atomic_load_explicit
( int line_no, volatile atomic_short* __a__, memory_order __x__ )
{ return _ATOMIC_LOAD_( line_no, __a__, __x__ ); }

inline short atomic_load( int line_no, volatile atomic_short* __a__ )
{ return atomic_load_explicit( line_no, __a__, memory_order_seq_cst ); }

inline void atomic_init
( volatile atomic_short* __a__, short __m__ )
{ _ATOMIC_INIT_( __a__, __m__ ); }

inline void atomic_store_explicit
( int line_no, volatile atomic_short* __a__, short __m__, memory_order __x__ )
{ _ATOMIC_STORE_( line_no, __a__, __m__, __x__ ); }

inline void atomic_store
( int line_no, volatile atomic_short* __a__, short __m__ )
{ atomic_store_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }

inline short atomic_exchange_explicit
( int line_no, volatile atomic_short* __a__, short __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, =, __m__, __x__ ); }

inline short atomic_exchange
( int line_no, volatile atomic_short* __a__, short __m__ )
{ return atomic_exchange_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }

inline bool atomic_compare_exchange_weak_explicit
( int line_no, volatile atomic_short* __a__, short* __e__, short __m__,
  memory_order __x__, memory_order __y__ )
{ return _ATOMIC_CMPSWP_WEAK_( line_no, __a__, __e__, __m__, __x__ ); }

inline bool atomic_compare_exchange_strong_explicit
( int line_no, volatile atomic_short* __a__, short* __e__, short __m__,
  memory_order __x__, memory_order __y__ )
{ return _ATOMIC_CMPSWP_( line_no, __a__, __e__, __m__, __x__ ); }

inline bool atomic_compare_exchange_weak
( int line_no, volatile atomic_short* __a__, short* __e__, short __m__ )
{ return atomic_compare_exchange_weak_explicit( line_no, __a__, __e__, __m__,
                 memory_order_seq_cst, memory_order_seq_cst ); }

inline bool atomic_compare_exchange_strong
( int line_no, volatile atomic_short* __a__, short* __e__, short __m__ )
{ return atomic_compare_exchange_strong_explicit( line_no, __a__, __e__, __m__,
                 memory_order_seq_cst, memory_order_seq_cst ); }


inline bool atomic_is_lock_free( const volatile atomic_ushort* __a__ )
{ return false; }

inline unsigned short atomic_load_explicit
( int line_no, volatile atomic_ushort* __a__, memory_order __x__ )
{ return _ATOMIC_LOAD_( line_no, __a__, __x__ ); }

inline unsigned short atomic_load( int line_no, volatile atomic_ushort* __a__ )
{ return atomic_load_explicit( line_no, __a__, memory_order_seq_cst ); }

inline void atomic_init
( volatile atomic_ushort* __a__, unsigned short __m__ )
{ _ATOMIC_INIT_( __a__, __m__ ); }

inline void atomic_store_explicit
( int line_no, volatile atomic_ushort* __a__, unsigned short __m__, memory_order __x__ )
{ _ATOMIC_STORE_( line_no, __a__, __m__, __x__ ); }

inline void atomic_store
( int line_no, volatile atomic_ushort* __a__, unsigned short __m__ )
{ atomic_store_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }

inline unsigned short atomic_exchange_explicit
( int line_no, volatile atomic_ushort* __a__, unsigned short __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, =, __m__, __x__ ); }

inline unsigned short atomic_exchange
( int line_no, volatile atomic_ushort* __a__, unsigned short __m__ )
{ return atomic_exchange_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }

inline bool atomic_compare_exchange_weak_explicit
( int line_no, volatile atomic_ushort* __a__, unsigned short* __e__, unsigned short __m__,
  memory_order __x__, memory_order __y__ )
{ return _ATOMIC_CMPSWP_WEAK_( line_no, __a__, __e__, __m__, __x__ ); }

inline bool atomic_compare_exchange_strong_explicit
( int line_no, volatile atomic_ushort* __a__, unsigned short* __e__, unsigned short __m__,
  memory_order __x__, memory_order __y__ )
{ return _ATOMIC_CMPSWP_( line_no, __a__, __e__, __m__, __x__ ); }

inline bool atomic_compare_exchange_weak
( int line_no,  volatile atomic_ushort* __a__, unsigned short* __e__, unsigned short __m__ )
{ return atomic_compare_exchange_weak_explicit( line_no, __a__, __e__, __m__,
                 memory_order_seq_cst, memory_order_seq_cst ); }

inline bool atomic_compare_exchange_strong
( int line_no, volatile atomic_ushort* __a__, unsigned short* __e__, unsigned short __m__ )
{ return atomic_compare_exchange_strong_explicit( line_no, __a__, __e__, __m__,
                 memory_order_seq_cst, memory_order_seq_cst ); }


inline bool atomic_is_lock_free( const volatile atomic_int* __a__ )
{ return false; }

inline int atomic_load_explicit
( int line_no, volatile atomic_int* __a__, memory_order __x__ )
{ return _ATOMIC_LOAD_( line_no, __a__, __x__ ); }

inline int atomic_load( int line_no, volatile atomic_int* __a__ )
{ return atomic_load_explicit( line_no, __a__, memory_order_seq_cst ); }

inline void atomic_init
( volatile atomic_int* __a__, int __m__ )
{ _ATOMIC_INIT_( __a__, __m__ ); }

inline void atomic_store_explicit
( int line_no, volatile atomic_int* __a__, int __m__, memory_order __x__ )
{ _ATOMIC_STORE_( line_no, __a__, __m__, __x__ ); }

inline void atomic_store
( int line_no, volatile atomic_int* __a__, int __m__ )
{ atomic_store_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }

inline int atomic_exchange_explicit
( int line_no, volatile atomic_int* __a__, int __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, =, __m__, __x__ ); }

inline int atomic_exchange
( int line_no, volatile atomic_int* __a__, int __m__ )
{ return atomic_exchange_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }

inline bool atomic_compare_exchange_weak_explicit
( int line_no, volatile atomic_int* __a__, int* __e__, int __m__,
  memory_order __x__, memory_order __y__ )
{ return _ATOMIC_CMPSWP_WEAK_( line_no, __a__, __e__, __m__, __x__ ); }

inline bool atomic_compare_exchange_strong_explicit
( int line_no, volatile atomic_int* __a__, int* __e__, int __m__,
  memory_order __x__, memory_order __y__ )
{ return _ATOMIC_CMPSWP_( line_no, __a__, __e__, __m__, __x__ ); }

inline bool atomic_compare_exchange_weak
( int line_no,  volatile atomic_int* __a__, int* __e__, int __m__ )
{ return atomic_compare_exchange_weak_explicit( line_no, __a__, __e__, __m__,
                 memory_order_seq_cst, memory_order_seq_cst ); }

inline bool atomic_compare_exchange_strong
( int line_no, volatile atomic_int* __a__, int* __e__, int __m__ )
{ return atomic_compare_exchange_strong_explicit( line_no, __a__, __e__, __m__,
                 memory_order_seq_cst, memory_order_seq_cst ); }


inline bool atomic_is_lock_free( const volatile atomic_uint* __a__ )
{ return false; }

inline unsigned int atomic_load_explicit
( int line_no, volatile atomic_uint* __a__, memory_order __x__ )
{ return _ATOMIC_LOAD_( line_no, __a__, __x__ ); }

inline unsigned int atomic_load( int line_no, volatile atomic_uint* __a__ )
{ return atomic_load_explicit( line_no, __a__, memory_order_seq_cst ); }

inline void atomic_init
( volatile atomic_uint* __a__, unsigned int __m__ )
{ _ATOMIC_INIT_( __a__, __m__ ); }

inline void atomic_store_explicit
( int line_no, volatile atomic_uint* __a__, unsigned int __m__, memory_order __x__ )
{ _ATOMIC_STORE_( line_no, __a__, __m__, __x__ ); }

inline void atomic_store
( int line_no, volatile atomic_uint* __a__, unsigned int __m__ )
{ atomic_store_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }

inline unsigned int atomic_exchange_explicit
( int line_no, volatile atomic_uint* __a__, unsigned int __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, =, __m__, __x__ ); }

inline unsigned int atomic_exchange
( int line_no, volatile atomic_uint* __a__, unsigned int __m__ )
{ return atomic_exchange_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }

inline bool atomic_compare_exchange_weak_explicit
( int line_no, volatile atomic_uint* __a__, unsigned int* __e__, unsigned int __m__,
  memory_order __x__, memory_order __y__ )
{ return _ATOMIC_CMPSWP_WEAK_( line_no, __a__, __e__, __m__, __x__ ); }

inline bool atomic_compare_exchange_strong_explicit
( int line_no, volatile atomic_uint* __a__, unsigned int* __e__, unsigned int __m__,
  memory_order __x__, memory_order __y__ )
{ return _ATOMIC_CMPSWP_( line_no, __a__, __e__, __m__, __x__ ); }

inline bool atomic_compare_exchange_weak
( int line_no,  volatile atomic_uint* __a__, unsigned int* __e__, unsigned int __m__ )
{ return atomic_compare_exchange_weak_explicit( line_no, __a__, __e__, __m__,
                 memory_order_seq_cst, memory_order_seq_cst ); }

inline bool atomic_compare_exchange_strong
( int line_no, volatile atomic_uint* __a__, unsigned int* __e__, unsigned int __m__ )
{ return atomic_compare_exchange_strong_explicit( line_no, __a__, __e__, __m__,
                 memory_order_seq_cst, memory_order_seq_cst ); }


inline bool atomic_is_lock_free( const volatile atomic_long* __a__ )
{ return false; }

inline long atomic_load_explicit
( int line_no, volatile atomic_long* __a__, memory_order __x__ )
{ return _ATOMIC_LOAD_( line_no, __a__, __x__ ); }

inline long atomic_load( int line_no, volatile atomic_long* __a__ )
{ return atomic_load_explicit( line_no, __a__, memory_order_seq_cst ); }

inline void atomic_init
( volatile atomic_long* __a__, long __m__ )
{ _ATOMIC_INIT_( __a__, __m__ ); }

inline void atomic_store_explicit
( int line_no, volatile atomic_long* __a__, long __m__, memory_order __x__ )
{ _ATOMIC_STORE_( line_no, __a__, __m__, __x__ ); }

inline void atomic_store
( int line_no, volatile atomic_long* __a__, long __m__ )
{ atomic_store_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }

inline long atomic_exchange_explicit
( int line_no, volatile atomic_long* __a__, long __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, =, __m__, __x__ ); }

inline long atomic_exchange
( int line_no, volatile atomic_long* __a__, long __m__ )
{ return atomic_exchange_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }

inline bool atomic_compare_exchange_weak_explicit
( int line_no, volatile atomic_long* __a__, long* __e__, long __m__,
  memory_order __x__, memory_order __y__ )
{ return _ATOMIC_CMPSWP_WEAK_( line_no, __a__, __e__, __m__, __x__ ); }

inline bool atomic_compare_exchange_strong_explicit
( int line_no, volatile atomic_long* __a__, long* __e__, long __m__,
  memory_order __x__, memory_order __y__ )
{ return _ATOMIC_CMPSWP_( line_no, __a__, __e__, __m__, __x__ ); }

inline bool atomic_compare_exchange_weak
( int line_no,  volatile atomic_long* __a__, long* __e__, long __m__ )
{ return atomic_compare_exchange_weak_explicit( line_no, __a__, __e__, __m__,
                 memory_order_seq_cst, memory_order_seq_cst ); }

inline bool atomic_compare_exchange_strong
( int line_no, volatile atomic_long* __a__, long* __e__, long __m__ )
{ return atomic_compare_exchange_strong_explicit( line_no, __a__, __e__, __m__,
                 memory_order_seq_cst, memory_order_seq_cst ); }


inline bool atomic_is_lock_free( const volatile atomic_ulong* __a__ )
{ return false; }

inline unsigned long atomic_load_explicit
( int line_no, volatile atomic_ulong* __a__, memory_order __x__ )
{ return _ATOMIC_LOAD_( line_no, __a__, __x__ ); }

inline unsigned long atomic_load( int line_no, volatile atomic_ulong* __a__ )
{ return atomic_load_explicit( line_no, __a__, memory_order_seq_cst ); }

inline void atomic_init
( volatile atomic_ulong* __a__, unsigned long __m__ )
{ _ATOMIC_INIT_( __a__, __m__ ); }

inline void atomic_store_explicit
( int line_no, volatile atomic_ulong* __a__, unsigned long __m__, memory_order __x__ )
{ _ATOMIC_STORE_( line_no, __a__, __m__, __x__ ); }

inline void atomic_store
( int line_no, volatile atomic_ulong* __a__, unsigned long __m__ )
{ atomic_store_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }

inline unsigned long atomic_exchange_explicit
( int line_no, volatile atomic_ulong* __a__, unsigned long __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, =, __m__, __x__ ); }

inline unsigned long atomic_exchange
( int line_no, volatile atomic_ulong* __a__, unsigned long __m__ )
{ return atomic_exchange_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }

inline bool atomic_compare_exchange_weak_explicit
( int line_no, volatile atomic_ulong* __a__, unsigned long* __e__, unsigned long __m__,
  memory_order __x__, memory_order __y__ )
{ return _ATOMIC_CMPSWP_WEAK_( line_no, __a__, __e__, __m__, __x__ ); }

inline bool atomic_compare_exchange_strong_explicit
( int line_no, volatile atomic_ulong* __a__, unsigned long* __e__, unsigned long __m__,
  memory_order __x__, memory_order __y__ )
{ return _ATOMIC_CMPSWP_( line_no, __a__, __e__, __m__, __x__ ); }

inline bool atomic_compare_exchange_weak
( int line_no,  volatile atomic_ulong* __a__, unsigned long* __e__, unsigned long __m__ )
{ return atomic_compare_exchange_weak_explicit( line_no, __a__, __e__, __m__,
                 memory_order_seq_cst, memory_order_seq_cst ); }

inline bool atomic_compare_exchange_strong
( int line_no, volatile atomic_ulong* __a__, unsigned long* __e__, unsigned long __m__ )
{ return atomic_compare_exchange_strong_explicit( line_no, __a__, __e__, __m__,
                 memory_order_seq_cst, memory_order_seq_cst ); }


inline bool atomic_is_lock_free( const volatile atomic_llong* __a__ )
{ return false; }

inline long long atomic_load_explicit
( int line_no, volatile atomic_llong* __a__, memory_order __x__ )
{ return _ATOMIC_LOAD_( line_no, __a__, __x__ ); }

inline long long atomic_load( int line_no, volatile atomic_llong* __a__ )
{ return atomic_load_explicit( line_no, __a__, memory_order_seq_cst ); }

inline void atomic_init
( volatile atomic_llong* __a__, long long __m__ )
{ _ATOMIC_INIT_( __a__, __m__ ); }

inline void atomic_store_explicit
( int line_no, volatile atomic_llong* __a__, long long __m__, memory_order __x__ )
{ _ATOMIC_STORE_( line_no, __a__, __m__, __x__ ); }

inline void atomic_store
( int line_no, volatile atomic_llong* __a__, long long __m__ )
{ atomic_store_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }

inline long long atomic_exchange_explicit
( int line_no, volatile atomic_llong* __a__, long long __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, =, __m__, __x__ ); }

inline long long atomic_exchange
( int line_no, volatile atomic_llong* __a__, long long __m__ )
{ return atomic_exchange_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }

inline bool atomic_compare_exchange_weak_explicit
( int line_no, volatile atomic_llong* __a__, long long* __e__, long long __m__,
  memory_order __x__, memory_order __y__ )
{ return _ATOMIC_CMPSWP_WEAK_( line_no, __a__, __e__, __m__, __x__ ); }

inline bool atomic_compare_exchange_strong_explicit
( int line_no, volatile atomic_llong* __a__, long long* __e__, long long __m__,
  memory_order __x__, memory_order __y__ )
{ return _ATOMIC_CMPSWP_( line_no, __a__, __e__, __m__, __x__ ); }

inline bool atomic_compare_exchange_weak
( int line_no,  volatile atomic_llong* __a__, long long* __e__, long long __m__ )
{ return atomic_compare_exchange_weak_explicit( line_no, __a__, __e__, __m__,
                 memory_order_seq_cst, memory_order_seq_cst ); }

inline bool atomic_compare_exchange_strong
( int line_no, volatile atomic_llong* __a__, long long* __e__, long long __m__ )
{ return atomic_compare_exchange_strong_explicit( line_no, __a__, __e__, __m__,
                 memory_order_seq_cst, memory_order_seq_cst ); }


inline bool atomic_is_lock_free( const volatile atomic_ullong* __a__ )
{ return false; }

inline unsigned long long atomic_load_explicit
( int line_no, volatile atomic_ullong* __a__, memory_order __x__ )
{ return _ATOMIC_LOAD_( line_no, __a__, __x__ ); }

inline unsigned long long atomic_load( int line_no, volatile atomic_ullong* __a__ )
{ return atomic_load_explicit( line_no, __a__, memory_order_seq_cst ); }

inline void atomic_init
( volatile atomic_ullong* __a__, unsigned long long __m__ )
{ _ATOMIC_INIT_( __a__, __m__ ); }

inline void atomic_store_explicit
( int line_no, volatile atomic_ullong* __a__, unsigned long long __m__, memory_order __x__ )
{ _ATOMIC_STORE_( line_no, __a__, __m__, __x__ ); }

inline void atomic_store
( int line_no, volatile atomic_ullong* __a__, unsigned long long __m__ )
{ atomic_store_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }

inline unsigned long long atomic_exchange_explicit
( int line_no, volatile atomic_ullong* __a__, unsigned long long __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, =, __m__, __x__ ); }

inline unsigned long long atomic_exchange
( int line_no, volatile atomic_ullong* __a__, unsigned long long __m__ )
{ return atomic_exchange_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }

inline bool atomic_compare_exchange_weak_explicit
( int line_no, volatile atomic_ullong* __a__, unsigned long long* __e__, unsigned long long __m__,
  memory_order __x__, memory_order __y__ )
{ return _ATOMIC_CMPSWP_WEAK_( line_no, __a__, __e__, __m__, __x__ ); }

inline bool atomic_compare_exchange_strong_explicit
( int line_no, volatile atomic_ullong* __a__, unsigned long long* __e__, unsigned long long __m__,
  memory_order __x__, memory_order __y__ )
{ return _ATOMIC_CMPSWP_( line_no, __a__, __e__, __m__, __x__ ); }

inline bool atomic_compare_exchange_weak
( int line_no,  volatile atomic_ullong* __a__, unsigned long long* __e__, unsigned long long __m__ )
{ return atomic_compare_exchange_weak_explicit( line_no, __a__, __e__, __m__,
                 memory_order_seq_cst, memory_order_seq_cst ); }

inline bool atomic_compare_exchange_strong
( int line_no, volatile atomic_ullong* __a__, unsigned long long* __e__, unsigned long long __m__ )
{ return atomic_compare_exchange_strong_explicit( line_no, __a__, __e__, __m__,
                 memory_order_seq_cst, memory_order_seq_cst ); }


inline bool atomic_is_lock_free( const volatile atomic_wchar_t* __a__ )
{ return false; }

inline wchar_t atomic_load_explicit
(  int line_no, volatile atomic_wchar_t* __a__, memory_order __x__ )
{ return _ATOMIC_LOAD_( line_no, __a__, __x__ ); }

inline wchar_t atomic_load( int line_no, volatile atomic_wchar_t* __a__ )
{ return atomic_load_explicit( line_no, __a__, memory_order_seq_cst ); }

inline void atomic_init
( volatile atomic_wchar_t* __a__, wchar_t __m__ )
{ _ATOMIC_INIT_( __a__, __m__ ); }

inline void atomic_store_explicit
( int line_no, volatile atomic_wchar_t* __a__, wchar_t __m__, memory_order __x__ )
{ _ATOMIC_STORE_( line_no, __a__, __m__, __x__ ); }

inline void atomic_store
( int line_no, volatile atomic_wchar_t* __a__, wchar_t __m__ )
{ atomic_store_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }

inline wchar_t atomic_exchange_explicit
( int line_no, volatile atomic_wchar_t* __a__, wchar_t __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, =, __m__, __x__ ); }

inline wchar_t atomic_exchange
( int line_no, volatile atomic_wchar_t* __a__, wchar_t __m__ )
{ return atomic_exchange_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }

inline bool atomic_compare_exchange_weak_explicit
( int line_no, volatile atomic_wchar_t* __a__, wchar_t* __e__, wchar_t __m__,
  memory_order __x__, memory_order __y__ )
{ return _ATOMIC_CMPSWP_WEAK_( line_no, __a__, __e__, __m__, __x__ ); }

inline bool atomic_compare_exchange_strong_explicit
( int line_no, volatile atomic_wchar_t* __a__, wchar_t* __e__, wchar_t __m__,
  memory_order __x__, memory_order __y__ )
{ return _ATOMIC_CMPSWP_( line_no, __a__, __e__, __m__, __x__ ); }

inline bool atomic_compare_exchange_weak
( int line_no,  volatile atomic_wchar_t* __a__, wchar_t* __e__, wchar_t __m__ )
{ return atomic_compare_exchange_weak_explicit( line_no, __a__, __e__, __m__,
                 memory_order_seq_cst, memory_order_seq_cst ); }

inline bool atomic_compare_exchange_strong
( int line_no, volatile atomic_wchar_t* __a__, wchar_t* __e__, wchar_t __m__ )
{ return atomic_compare_exchange_strong_explicit( line_no, __a__, __e__, __m__,
                 memory_order_seq_cst, memory_order_seq_cst ); }


inline void* atomic_fetch_add_explicit
( int line_no, volatile atomic_address* __a__, ptrdiff_t __m__, memory_order __x__ )
{
	volatile __typeof__((__a__)->__f__)* __p__ = & ((__a__)->__f__);
	__typeof__((__a__)->__f__) __old__=(__typeof__((__a__)->__f__)) model_rmwr_action((void *)__p__, __x__, line_no);
	__typeof__((__a__)->__f__) __copy__= __old__;
	__copy__ = (void *) (((char *)__copy__) + __m__);
	model_rmw_action((void *)__p__, __x__, (uint64_t) __copy__);
	return __old__;
}

 inline void* atomic_fetch_add
( int line_no, volatile atomic_address* __a__, ptrdiff_t __m__ )
{ return atomic_fetch_add_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline void* atomic_fetch_sub_explicit
( int line_no, volatile atomic_address* __a__, ptrdiff_t __m__, memory_order __x__ )
{	volatile __typeof__((__a__)->__f__)* __p__ = & ((__a__)->__f__);
	__typeof__((__a__)->__f__) __old__=(__typeof__((__a__)->__f__)) model_rmwr_action((void *)__p__, __x__, line_no);
	__typeof__((__a__)->__f__) __copy__= __old__;
	__copy__ = (void *) (((char *)__copy__) - __m__);
	model_rmw_action((void *)__p__, __x__, (uint64_t) __copy__);
	return __old__;
}

inline void* atomic_fetch_sub
( int line_no, volatile atomic_address* __a__, ptrdiff_t __m__ )
{ return atomic_fetch_sub_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }

inline char atomic_fetch_add_explicit
( int line_no, volatile atomic_char* __a__, char __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, +=, __m__, __x__ ); }

inline char atomic_fetch_add
( int line_no, volatile atomic_char* __a__, char __m__ )
{ return atomic_fetch_add_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline char atomic_fetch_sub_explicit
( int line_no, volatile atomic_char* __a__, char __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, -=, __m__, __x__ ); }

inline char atomic_fetch_sub
( int line_no, volatile atomic_char* __a__, char __m__ )
{ return atomic_fetch_sub_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline char atomic_fetch_and_explicit
( int line_no, volatile atomic_char* __a__, char __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, &=, __m__, __x__ ); }

inline char atomic_fetch_and
( int line_no, volatile atomic_char* __a__, char __m__ )
{ return atomic_fetch_and_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline char atomic_fetch_or_explicit
( int line_no, volatile atomic_char* __a__, char __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, |=, __m__, __x__ ); }

inline char atomic_fetch_or
( int line_no, volatile atomic_char* __a__, char __m__ )
{ return atomic_fetch_or_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline char atomic_fetch_xor_explicit
( int line_no, volatile atomic_char* __a__, char __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, ^=, __m__, __x__ ); }

inline char atomic_fetch_xor
( int line_no, volatile atomic_char* __a__, char __m__ )
{ return atomic_fetch_xor_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline signed char atomic_fetch_add_explicit
( int line_no, volatile atomic_schar* __a__, signed char __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, +=, __m__, __x__ ); }

inline signed char atomic_fetch_add
( int line_no, volatile atomic_schar* __a__, signed char __m__ )
{ return atomic_fetch_add_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline signed char atomic_fetch_sub_explicit
( int line_no, volatile atomic_schar* __a__, signed char __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, -=, __m__, __x__ ); }

inline signed char atomic_fetch_sub
( int line_no, volatile atomic_schar* __a__, signed char __m__ )
{ return atomic_fetch_sub_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline signed char atomic_fetch_and_explicit
( int line_no, volatile atomic_schar* __a__, signed char __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, &=, __m__, __x__ ); }

inline signed char atomic_fetch_and
( int line_no, volatile atomic_schar* __a__, signed char __m__ )
{ return atomic_fetch_and_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline signed char atomic_fetch_or_explicit
( int line_no, volatile atomic_schar* __a__, signed char __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, |=, __m__, __x__ ); }

inline signed char atomic_fetch_or
( int line_no, volatile atomic_schar* __a__, signed char __m__ )
{ return atomic_fetch_or_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline signed char atomic_fetch_xor_explicit
( int line_no, volatile atomic_schar* __a__, signed char __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, ^=, __m__, __x__ ); }

inline signed char atomic_fetch_xor
( int line_no, volatile atomic_schar* __a__, signed char __m__ )
{ return atomic_fetch_xor_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline unsigned char atomic_fetch_add_explicit
( int line_no, volatile atomic_uchar* __a__, unsigned char __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, +=, __m__, __x__ ); }

inline unsigned char atomic_fetch_add
( int line_no, volatile atomic_uchar* __a__, unsigned char __m__ )
{ return atomic_fetch_add_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline unsigned char atomic_fetch_sub_explicit
( int line_no, volatile atomic_uchar* __a__, unsigned char __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, -=, __m__, __x__ ); }

inline unsigned char atomic_fetch_sub
( int line_no, volatile atomic_uchar* __a__, unsigned char __m__ )
{ return atomic_fetch_sub_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline unsigned char atomic_fetch_and_explicit
( int line_no, volatile atomic_uchar* __a__, unsigned char __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, &=, __m__, __x__ ); }

inline unsigned char atomic_fetch_and
( int line_no, volatile atomic_uchar* __a__, unsigned char __m__ )
{ return atomic_fetch_and_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline unsigned char atomic_fetch_or_explicit
( int line_no, volatile atomic_uchar* __a__, unsigned char __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, |=, __m__, __x__ ); }

inline unsigned char atomic_fetch_or
( int line_no, volatile atomic_uchar* __a__, unsigned char __m__ )
{ return atomic_fetch_or_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline unsigned char atomic_fetch_xor_explicit
( int line_no, volatile atomic_uchar* __a__, unsigned char __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, ^=, __m__, __x__ ); }

inline unsigned char atomic_fetch_xor
( int line_no, volatile atomic_uchar* __a__, unsigned char __m__ )
{ return atomic_fetch_xor_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline short atomic_fetch_add_explicit
( int line_no, volatile atomic_short* __a__, short __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, +=, __m__, __x__ ); }

inline short atomic_fetch_add
( int line_no, volatile atomic_short* __a__, short __m__ )
{ return atomic_fetch_add_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline short atomic_fetch_sub_explicit
( int line_no, volatile atomic_short* __a__, short __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, -=, __m__, __x__ ); }

inline short atomic_fetch_sub
( int line_no, volatile atomic_short* __a__, short __m__ )
{ return atomic_fetch_sub_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline short atomic_fetch_and_explicit
( int line_no, volatile atomic_short* __a__, short __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, &=, __m__, __x__ ); }

inline short atomic_fetch_and
( int line_no, volatile atomic_short* __a__, short __m__ )
{ return atomic_fetch_and_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline short atomic_fetch_or_explicit
( int line_no, volatile atomic_short* __a__, short __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, |=, __m__, __x__ ); }

inline short atomic_fetch_or
( int line_no, volatile atomic_short* __a__, short __m__ )
{ return atomic_fetch_or_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline short atomic_fetch_xor_explicit
( int line_no, volatile atomic_short* __a__, short __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, ^=, __m__, __x__ ); }

inline short atomic_fetch_xor
( int line_no, volatile atomic_short* __a__, short __m__ )
{ return atomic_fetch_xor_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline unsigned short atomic_fetch_add_explicit
( int line_no, volatile atomic_ushort* __a__, unsigned short __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, +=, __m__, __x__ ); }

inline unsigned short atomic_fetch_add
( int line_no, volatile atomic_ushort* __a__, unsigned short __m__ )
{ return atomic_fetch_add_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline unsigned short atomic_fetch_sub_explicit
( int line_no, volatile atomic_ushort* __a__, unsigned short __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, -=, __m__, __x__ ); }

inline unsigned short atomic_fetch_sub
( int line_no, volatile atomic_ushort* __a__, unsigned short __m__ )
{ return atomic_fetch_sub_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline unsigned short atomic_fetch_and_explicit
( int line_no, volatile atomic_ushort* __a__, unsigned short __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, &=, __m__, __x__ ); }

inline unsigned short atomic_fetch_and
( int line_no, volatile atomic_ushort* __a__, unsigned short __m__ )
{ return atomic_fetch_and_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline unsigned short atomic_fetch_or_explicit
( int line_no, volatile atomic_ushort* __a__, unsigned short __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, |=, __m__, __x__ ); }

inline unsigned short atomic_fetch_or
( int line_no, volatile atomic_ushort* __a__, unsigned short __m__ )
{ return atomic_fetch_or_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline unsigned short atomic_fetch_xor_explicit
( int line_no, volatile atomic_ushort* __a__, unsigned short __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, ^=, __m__, __x__ ); }

inline unsigned short atomic_fetch_xor
( int line_no, volatile atomic_ushort* __a__, unsigned short __m__ )
{ return atomic_fetch_xor_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline int atomic_fetch_add_explicit
( int line_no, volatile atomic_int* __a__, int __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, +=, __m__, __x__ ); }

inline int atomic_fetch_add
( int line_no, volatile atomic_int* __a__, int __m__ )
{ return atomic_fetch_add_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline int atomic_fetch_sub_explicit
( int line_no, volatile atomic_int* __a__, int __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, -=, __m__, __x__ ); }

inline int atomic_fetch_sub
( int line_no, volatile atomic_int* __a__, int __m__ )
{ return atomic_fetch_sub_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline int atomic_fetch_and_explicit
( int line_no, volatile atomic_int* __a__, int __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, &=, __m__, __x__ ); }

inline int atomic_fetch_and
( int line_no, volatile atomic_int* __a__, int __m__ )
{ return atomic_fetch_and_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline int atomic_fetch_or_explicit
( int line_no, volatile atomic_int* __a__, int __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, |=, __m__, __x__ ); }

inline int atomic_fetch_or
( int line_no, volatile atomic_int* __a__, int __m__ )
{ return atomic_fetch_or_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline int atomic_fetch_xor_explicit
( int line_no, volatile atomic_int* __a__, int __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, ^=, __m__, __x__ ); }

inline int atomic_fetch_xor
( int line_no, volatile atomic_int* __a__, int __m__ )
{ return atomic_fetch_xor_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline unsigned int atomic_fetch_add_explicit
( int line_no, volatile atomic_uint* __a__, unsigned int __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, +=, __m__, __x__ ); }

inline unsigned int atomic_fetch_add
( int line_no, volatile atomic_uint* __a__, unsigned int __m__ )
{ return atomic_fetch_add_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline unsigned int atomic_fetch_sub_explicit
( int line_no, volatile atomic_uint* __a__, unsigned int __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, -=, __m__, __x__ ); }

inline unsigned int atomic_fetch_sub
( int line_no, volatile atomic_uint* __a__, unsigned int __m__ )
{ return atomic_fetch_sub_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline unsigned int atomic_fetch_and_explicit
( int line_no, volatile atomic_uint* __a__, unsigned int __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, &=, __m__, __x__ ); }

inline unsigned int atomic_fetch_and
( int line_no, volatile atomic_uint* __a__, unsigned int __m__ )
{ return atomic_fetch_and_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline unsigned int atomic_fetch_or_explicit
( int line_no, volatile atomic_uint* __a__, unsigned int __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, |=, __m__, __x__ ); }

inline unsigned int atomic_fetch_or
( int line_no, volatile atomic_uint* __a__, unsigned int __m__ )
{ return atomic_fetch_or_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline unsigned int atomic_fetch_xor_explicit
( int line_no, volatile atomic_uint* __a__, unsigned int __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, ^=, __m__, __x__ ); }

inline unsigned int atomic_fetch_xor
( int line_no, volatile atomic_uint* __a__, unsigned int __m__ )
{ return atomic_fetch_xor_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline long atomic_fetch_add_explicit
( int line_no, volatile atomic_long* __a__, long __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, +=, __m__, __x__ ); }

inline long atomic_fetch_add
( int line_no, volatile atomic_long* __a__, long __m__ )
{ return atomic_fetch_add_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline long atomic_fetch_sub_explicit
( int line_no, volatile atomic_long* __a__, long __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, -=, __m__, __x__ ); }

inline long atomic_fetch_sub
( int line_no, volatile atomic_long* __a__, long __m__ )
{ return atomic_fetch_sub_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline long atomic_fetch_and_explicit
( int line_no, volatile atomic_long* __a__, long __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, &=, __m__, __x__ ); }

inline long atomic_fetch_and
( int line_no, volatile atomic_long* __a__, long __m__ )
{ return atomic_fetch_and_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline long atomic_fetch_or_explicit
( int line_no, volatile atomic_long* __a__, long __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, |=, __m__, __x__ ); }

inline long atomic_fetch_or
( int line_no, volatile atomic_long* __a__, long __m__ )
{ return atomic_fetch_or_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline long atomic_fetch_xor_explicit
( int line_no, volatile atomic_long* __a__, long __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, ^=, __m__, __x__ ); }

inline long atomic_fetch_xor
( int line_no, volatile atomic_long* __a__, long __m__ )
{ return atomic_fetch_xor_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline unsigned long atomic_fetch_add_explicit
( int line_no, volatile atomic_ulong* __a__, unsigned long __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, +=, __m__, __x__ ); }

inline unsigned long atomic_fetch_add
( int line_no, volatile atomic_ulong* __a__, unsigned long __m__ )
{ return atomic_fetch_add_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline unsigned long atomic_fetch_sub_explicit
( int line_no, volatile atomic_ulong* __a__, unsigned long __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, -=, __m__, __x__ ); }

inline unsigned long atomic_fetch_sub
( int line_no, volatile atomic_ulong* __a__, unsigned long __m__ )
{ return atomic_fetch_sub_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline unsigned long atomic_fetch_and_explicit
( int line_no, volatile atomic_ulong* __a__, unsigned long __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, &=, __m__, __x__ ); }

inline unsigned long atomic_fetch_and
( int line_no, volatile atomic_ulong* __a__, unsigned long __m__ )
{ return atomic_fetch_and_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline unsigned long atomic_fetch_or_explicit
( int line_no, volatile atomic_ulong* __a__, unsigned long __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, |=, __m__, __x__ ); }

inline unsigned long atomic_fetch_or
( int line_no, volatile atomic_ulong* __a__, unsigned long __m__ )
{ return atomic_fetch_or_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline unsigned long atomic_fetch_xor_explicit
( int line_no, volatile atomic_ulong* __a__, unsigned long __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, ^=, __m__, __x__ ); }

inline unsigned long atomic_fetch_xor
( int line_no, volatile atomic_ulong* __a__, unsigned long __m__ )
{ return atomic_fetch_xor_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline long long atomic_fetch_add_explicit
( int line_no, volatile atomic_llong* __a__, long long __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, +=, __m__, __x__ ); }

inline long long atomic_fetch_add
( int line_no, volatile atomic_llong* __a__, long long __m__ )
{ return atomic_fetch_add_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline long long atomic_fetch_sub_explicit
( int line_no, volatile atomic_llong* __a__, long long __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, -=, __m__, __x__ ); }

inline long long atomic_fetch_sub
( int line_no, volatile atomic_llong* __a__, long long __m__ )
{ return atomic_fetch_sub_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline long long atomic_fetch_and_explicit
( int line_no, volatile atomic_llong* __a__, long long __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, &=, __m__, __x__ ); }

inline long long atomic_fetch_and
( int line_no, volatile atomic_llong* __a__, long long __m__ )
{ return atomic_fetch_and_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline long long atomic_fetch_or_explicit
( int line_no, volatile atomic_llong* __a__, long long __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, |=, __m__, __x__ ); }

inline long long atomic_fetch_or
( int line_no, volatile atomic_llong* __a__, long long __m__ )
{ return atomic_fetch_or_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline long long atomic_fetch_xor_explicit
( int line_no, volatile atomic_llong* __a__, long long __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, ^=, __m__, __x__ ); }

inline long long atomic_fetch_xor
( int line_no, volatile atomic_llong* __a__, long long __m__ )
{ return atomic_fetch_xor_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline unsigned long long atomic_fetch_add_explicit
( int line_no, volatile atomic_ullong* __a__, unsigned long long __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, +=, __m__, __x__ ); }

inline unsigned long long atomic_fetch_add
( int line_no, volatile atomic_ullong* __a__, unsigned long long __m__ )
{ return atomic_fetch_add_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline unsigned long long atomic_fetch_sub_explicit
( int line_no, volatile atomic_ullong* __a__, unsigned long long __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, -=, __m__, __x__ ); }

inline unsigned long long atomic_fetch_sub
( int line_no, volatile atomic_ullong* __a__, unsigned long long __m__ )
{ return atomic_fetch_sub_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline unsigned long long atomic_fetch_and_explicit
( int line_no, volatile atomic_ullong* __a__, unsigned long long __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, &=, __m__, __x__ ); }

inline unsigned long long atomic_fetch_and
( int line_no, volatile atomic_ullong* __a__, unsigned long long __m__ )
{ return atomic_fetch_and_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline unsigned long long atomic_fetch_or_explicit
( int line_no, volatile atomic_ullong* __a__, unsigned long long __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, |=, __m__, __x__ ); }

inline unsigned long long atomic_fetch_or
( int line_no, volatile atomic_ullong* __a__, unsigned long long __m__ )
{ return atomic_fetch_or_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline unsigned long long atomic_fetch_xor_explicit
( int line_no, volatile atomic_ullong* __a__, unsigned long long __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, ^=, __m__, __x__ ); }

inline unsigned long long atomic_fetch_xor
( int line_no, volatile atomic_ullong* __a__, unsigned long long __m__ )
{ return atomic_fetch_xor_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline wchar_t atomic_fetch_add_explicit
( int line_no, volatile atomic_wchar_t* __a__, wchar_t __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, +=, __m__, __x__ ); }

inline wchar_t atomic_fetch_add
( int line_no, volatile atomic_wchar_t* __a__, wchar_t __m__ )
{ return atomic_fetch_add_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline wchar_t atomic_fetch_sub_explicit
( int line_no, volatile atomic_wchar_t* __a__, wchar_t __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, -=, __m__, __x__ ); }

inline wchar_t atomic_fetch_sub
( int line_no, volatile atomic_wchar_t* __a__, wchar_t __m__ )
{ return atomic_fetch_sub_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline wchar_t atomic_fetch_and_explicit
( int line_no, volatile atomic_wchar_t* __a__, wchar_t __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, &=, __m__, __x__ ); }

inline wchar_t atomic_fetch_and
( int line_no, volatile atomic_wchar_t* __a__, wchar_t __m__ )
{ return atomic_fetch_and_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline wchar_t atomic_fetch_or_explicit
( int line_no, volatile atomic_wchar_t* __a__, wchar_t __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, |=, __m__, __x__ ); }

inline wchar_t atomic_fetch_or
( int line_no, volatile atomic_wchar_t* __a__, wchar_t __m__ )
{ return atomic_fetch_or_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


inline wchar_t atomic_fetch_xor_explicit
( int line_no, volatile atomic_wchar_t* __a__, wchar_t __m__, memory_order __x__ )
{ return _ATOMIC_MODIFY_( line_no, __a__, ^=, __m__, __x__ ); }

inline wchar_t atomic_fetch_xor
( int line_no, volatile atomic_wchar_t* __a__, wchar_t __m__ )
{ return atomic_fetch_xor_explicit( line_no, __a__, __m__, memory_order_seq_cst ); }


#else


#define atomic_is_lock_free( __a__ ) \
false

#define atomic_load( line_no, __a__ ) \
_ATOMIC_LOAD_( line_no, __a__, memory_order_seq_cst )

#define atomic_load_explicit( line_no, __a__, __x__ ) \
_ATOMIC_LOAD_( line_no, __a__, __x__ )

#define atomic_init( __a__, __m__ ) \
_ATOMIC_INIT_( __a__, __m__ )

#define atomic_store( line_no, __a__, __m__ ) \
_ATOMIC_STORE_( line_no, __a__, __m__, memory_order_seq_cst )

#define atomic_store_explicit( line_no, __a__, __m__, __x__ ) \
_ATOMIC_STORE_( line_no, __a__, __m__, __x__ )

#define atomic_exchange( line_no, __a__, __m__ ) \
_ATOMIC_MODIFY_( line_no, __a__, =, __m__, memory_order_seq_cst )

#define atomic_exchange_explicit( line_no, __a__, __m__, __x__ ) \
_ATOMIC_MODIFY_( line_no, __a__, =, __m__, __x__ )

#define atomic_compare_exchange_weak( line_no,  __a__, __e__, __m__ ) \
_ATOMIC_CMPSWP_WEAK_( line_no, __a__, __e__, __m__, memory_order_seq_cst )

#define atomic_compare_exchange_strong( line_no, __a__, __e__, __m__ ) \
_ATOMIC_CMPSWP_( line_no, __a__, __e__, __m__, memory_order_seq_cst )

#define atomic_compare_exchange_weak_explicit( line_no, __a__, __e__, __m__, __x__, __y__ ) \
_ATOMIC_CMPSWP_WEAK_( line_no, __a__, __e__, __m__, __x__ )

#define atomic_compare_exchange_strong_explicit( line_no, __a__, __e__, __m__, __x__, __y__ ) \
_ATOMIC_CMPSWP_( line_no, __a__, __e__, __m__, __x__ )


#define atomic_fetch_add_explicit( line_no, __a__, __m__, __x__ ) \
_ATOMIC_MODIFY_( line_no, __a__, +=, __m__, __x__ )

#define atomic_fetch_add( line_no, __a__, __m__ ) \
_ATOMIC_MODIFY_( line_no, __a__, +=, __m__, memory_order_seq_cst )


#define atomic_fetch_sub_explicit( line_no, __a__, __m__, __x__ ) \
_ATOMIC_MODIFY_( line_no, __a__, -=, __m__, __x__ )

#define atomic_fetch_sub( line_no, __a__, __m__ ) \
_ATOMIC_MODIFY_( line_no, __a__, -=, __m__, memory_order_seq_cst )


#define atomic_fetch_and_explicit( line_no, __a__, __m__, __x__ ) \
_ATOMIC_MODIFY_( line_no, __a__, &=, __m__, __x__ )

#define atomic_fetch_and( line_no, __a__, __m__ ) \
_ATOMIC_MODIFY_( line_no, __a__, &=, __m__, memory_order_seq_cst )


#define atomic_fetch_or_explicit( line_no, __a__, __m__, __x__ ) \
_ATOMIC_MODIFY_( line_no, __a__, |=, __m__, __x__ )

#define atomic_fetch_or( line_no, __a__, __m__ ) \
_ATOMIC_MODIFY_( line_no, __a__, |=, __m__, memory_order_seq_cst )


#define atomic_fetch_xor_explicit( line_no, __a__, __m__, __x__ ) \
_ATOMIC_MODIFY_( line_no, __a__, ^=, __m__, __x__ )

#define atomic_fetch_xor( line_no, __a__, __m__ ) \
_ATOMIC_MODIFY_( line_no, __a__, ^=, __m__, memory_order_seq_cst )


#endif


#ifdef __cplusplus


inline bool atomic_bool::is_lock_free() const volatile
{ return false; }

inline void atomic_bool::store
( int line_no, bool __m__, memory_order __x__ ) volatile
{ atomic_store_explicit( line_no, this, __m__, __x__ ); }

inline bool atomic_bool::load
( int line_no, memory_order __x__ ) volatile
{ return atomic_load_explicit( line_no, this, __x__ ); }

inline bool atomic_bool::exchange
( int line_no, bool __m__, memory_order __x__ ) volatile
{ return atomic_exchange_explicit( line_no, this, __m__, __x__ ); }

inline bool atomic_bool::compare_exchange_weak
( int line_no,  bool& __e__, bool __m__,
  memory_order __x__, memory_order __y__ ) volatile
{ return atomic_compare_exchange_weak_explicit( line_no, this, &__e__, __m__, __x__, __y__ ); }

inline bool atomic_bool::compare_exchange_strong
( int line_no, bool& __e__, bool __m__,
  memory_order __x__, memory_order __y__ ) volatile
{ return atomic_compare_exchange_strong_explicit( line_no, this, &__e__, __m__, __x__, __y__ ); }

inline bool atomic_bool::compare_exchange_weak
( int line_no,  bool& __e__, bool __m__, memory_order __x__ ) volatile
{ return atomic_compare_exchange_weak_explicit( line_no, this, &__e__, __m__, __x__,
      __x__ == memory_order_acq_rel ? memory_order_acquire :
      __x__ == memory_order_release ? memory_order_relaxed : __x__ ); }

inline bool atomic_bool::compare_exchange_strong
( int line_no, bool& __e__, bool __m__, memory_order __x__ ) volatile
{ return atomic_compare_exchange_strong_explicit( line_no, this, &__e__, __m__, __x__,
      __x__ == memory_order_acq_rel ? memory_order_acquire :
      __x__ == memory_order_release ? memory_order_relaxed : __x__ ); }


inline bool atomic_address::is_lock_free() const volatile
{ return false; }

inline void atomic_address::store
( int line_no, void* __m__, memory_order __x__ ) volatile
{ atomic_store_explicit( line_no, this, __m__, __x__ ); }

inline void* atomic_address::load
( int line_no, memory_order __x__ ) volatile
{ return atomic_load_explicit( line_no, this, __x__ ); }

inline void* atomic_address::exchange
( int line_no, void* __m__, memory_order __x__ ) volatile
{ return atomic_exchange_explicit( line_no, this, __m__, __x__ ); }

inline bool atomic_address::compare_exchange_weak
( int line_no,  void*& __e__, void* __m__,
  memory_order __x__, memory_order __y__ ) volatile
{ return atomic_compare_exchange_weak_explicit( line_no, this, &__e__, __m__, __x__, __y__ ); }

inline bool atomic_address::compare_exchange_strong
( int line_no, void*& __e__, void* __m__,
  memory_order __x__, memory_order __y__ ) volatile
{ return atomic_compare_exchange_strong_explicit( line_no, this, &__e__, __m__, __x__, __y__ ); }

inline bool atomic_address::compare_exchange_weak
( int line_no,  void*& __e__, void* __m__, memory_order __x__ ) volatile
{ return atomic_compare_exchange_weak_explicit( line_no, this, &__e__, __m__, __x__,
      __x__ == memory_order_acq_rel ? memory_order_acquire :
      __x__ == memory_order_release ? memory_order_relaxed : __x__ ); }

inline bool atomic_address::compare_exchange_strong
( int line_no, void*& __e__, void* __m__, memory_order __x__ ) volatile
{ return atomic_compare_exchange_strong_explicit( line_no, this, &__e__, __m__, __x__,
      __x__ == memory_order_acq_rel ? memory_order_acquire :
      __x__ == memory_order_release ? memory_order_relaxed : __x__ ); }


inline bool atomic_char::is_lock_free() const volatile
{ return false; }

inline void atomic_char::store
( int line_no, char __m__, memory_order __x__ ) volatile
{ atomic_store_explicit( line_no, this, __m__, __x__ ); }

inline char atomic_char::load
( int line_no, memory_order __x__ ) volatile
{ return atomic_load_explicit( line_no, this, __x__ ); }

inline char atomic_char::exchange
( int line_no, char __m__, memory_order __x__ ) volatile
{ return atomic_exchange_explicit( line_no, this, __m__, __x__ ); }

inline bool atomic_char::compare_exchange_weak
( int line_no,  char& __e__, char __m__,
  memory_order __x__, memory_order __y__ ) volatile
{ return atomic_compare_exchange_weak_explicit( line_no, this, &__e__, __m__, __x__, __y__ ); }

inline bool atomic_char::compare_exchange_strong
( int line_no, char& __e__, char __m__,
  memory_order __x__, memory_order __y__ ) volatile
{ return atomic_compare_exchange_strong_explicit( line_no, this, &__e__, __m__, __x__, __y__ ); }

inline bool atomic_char::compare_exchange_weak
( int line_no,  char& __e__, char __m__, memory_order __x__ ) volatile
{ return atomic_compare_exchange_weak_explicit( line_no, this, &__e__, __m__, __x__,
      __x__ == memory_order_acq_rel ? memory_order_acquire :
      __x__ == memory_order_release ? memory_order_relaxed : __x__ ); }

inline bool atomic_char::compare_exchange_strong
( int line_no, char& __e__, char __m__, memory_order __x__ ) volatile
{ return atomic_compare_exchange_strong_explicit( line_no, this, &__e__, __m__, __x__,
      __x__ == memory_order_acq_rel ? memory_order_acquire :
      __x__ == memory_order_release ? memory_order_relaxed : __x__ ); }


inline bool atomic_schar::is_lock_free() const volatile
{ return false; }

inline void atomic_schar::store
( int line_no, signed char __m__, memory_order __x__ ) volatile
{ atomic_store_explicit( line_no, this, __m__, __x__ ); }

inline signed char atomic_schar::load
( int line_no, memory_order __x__ ) volatile
{ return atomic_load_explicit( line_no, this, __x__ ); }

inline signed char atomic_schar::exchange
( int line_no, signed char __m__, memory_order __x__ ) volatile
{ return atomic_exchange_explicit( line_no, this, __m__, __x__ ); }

inline bool atomic_schar::compare_exchange_weak
( int line_no,  signed char& __e__, signed char __m__,
  memory_order __x__, memory_order __y__ ) volatile
{ return atomic_compare_exchange_weak_explicit( line_no, this, &__e__, __m__, __x__, __y__ ); }

inline bool atomic_schar::compare_exchange_strong
( int line_no, signed char& __e__, signed char __m__,
  memory_order __x__, memory_order __y__ ) volatile
{ return atomic_compare_exchange_strong_explicit( line_no, this, &__e__, __m__, __x__, __y__ ); }

inline bool atomic_schar::compare_exchange_weak
( int line_no,  signed char& __e__, signed char __m__, memory_order __x__ ) volatile
{ return atomic_compare_exchange_weak_explicit( line_no, this, &__e__, __m__, __x__,
      __x__ == memory_order_acq_rel ? memory_order_acquire :
      __x__ == memory_order_release ? memory_order_relaxed : __x__ ); }

inline bool atomic_schar::compare_exchange_strong
( int line_no, signed char& __e__, signed char __m__, memory_order __x__ ) volatile
{ return atomic_compare_exchange_strong_explicit( line_no, this, &__e__, __m__, __x__,
      __x__ == memory_order_acq_rel ? memory_order_acquire :
      __x__ == memory_order_release ? memory_order_relaxed : __x__ ); }


inline bool atomic_uchar::is_lock_free() const volatile
{ return false; }

inline void atomic_uchar::store
( int line_no, unsigned char __m__, memory_order __x__ ) volatile
{ atomic_store_explicit( line_no, this, __m__, __x__ ); }

inline unsigned char atomic_uchar::load
( int line_no, memory_order __x__ ) volatile
{ return atomic_load_explicit( line_no, this, __x__ ); }

inline unsigned char atomic_uchar::exchange
( int line_no, unsigned char __m__, memory_order __x__ ) volatile
{ return atomic_exchange_explicit( line_no, this, __m__, __x__ ); }

inline bool atomic_uchar::compare_exchange_weak
( int line_no,  unsigned char& __e__, unsigned char __m__,
  memory_order __x__, memory_order __y__ ) volatile
{ return atomic_compare_exchange_weak_explicit( line_no, this, &__e__, __m__, __x__, __y__ ); }

inline bool atomic_uchar::compare_exchange_strong
( int line_no, unsigned char& __e__, unsigned char __m__,
  memory_order __x__, memory_order __y__ ) volatile
{ return atomic_compare_exchange_strong_explicit( line_no, this, &__e__, __m__, __x__, __y__ ); }

inline bool atomic_uchar::compare_exchange_weak
( int line_no,  unsigned char& __e__, unsigned char __m__, memory_order __x__ ) volatile
{ return atomic_compare_exchange_weak_explicit( line_no, this, &__e__, __m__, __x__,
      __x__ == memory_order_acq_rel ? memory_order_acquire :
      __x__ == memory_order_release ? memory_order_relaxed : __x__ ); }

inline bool atomic_uchar::compare_exchange_strong
( int line_no, unsigned char& __e__, unsigned char __m__, memory_order __x__ ) volatile
{ return atomic_compare_exchange_strong_explicit( line_no, this, &__e__, __m__, __x__,
      __x__ == memory_order_acq_rel ? memory_order_acquire :
      __x__ == memory_order_release ? memory_order_relaxed : __x__ ); }


inline bool atomic_short::is_lock_free() const volatile
{ return false; }

inline void atomic_short::store
( int line_no, short __m__, memory_order __x__ ) volatile
{ atomic_store_explicit( line_no, this, __m__, __x__ ); }

inline short atomic_short::load
( int line_no, memory_order __x__ ) volatile
{ return atomic_load_explicit( line_no, this, __x__ ); }

inline short atomic_short::exchange
( int line_no, short __m__, memory_order __x__ ) volatile
{ return atomic_exchange_explicit( line_no, this, __m__, __x__ ); }

inline bool atomic_short::compare_exchange_weak
( int line_no,  short& __e__, short __m__,
  memory_order __x__, memory_order __y__ ) volatile
{ return atomic_compare_exchange_weak_explicit( line_no, this, &__e__, __m__, __x__, __y__ ); }

inline bool atomic_short::compare_exchange_strong
( int line_no, short& __e__, short __m__,
  memory_order __x__, memory_order __y__ ) volatile
{ return atomic_compare_exchange_strong_explicit( line_no, this, &__e__, __m__, __x__, __y__ ); }

inline bool atomic_short::compare_exchange_weak
( int line_no,  short& __e__, short __m__, memory_order __x__ ) volatile
{ return atomic_compare_exchange_weak_explicit( line_no, this, &__e__, __m__, __x__,
      __x__ == memory_order_acq_rel ? memory_order_acquire :
      __x__ == memory_order_release ? memory_order_relaxed : __x__ ); }

inline bool atomic_short::compare_exchange_strong
( int line_no, short& __e__, short __m__, memory_order __x__ ) volatile
{ return atomic_compare_exchange_strong_explicit( line_no, this, &__e__, __m__, __x__,
      __x__ == memory_order_acq_rel ? memory_order_acquire :
      __x__ == memory_order_release ? memory_order_relaxed : __x__ ); }


inline bool atomic_ushort::is_lock_free() const volatile
{ return false; }

inline void atomic_ushort::store
( int line_no, unsigned short __m__, memory_order __x__ ) volatile
{ atomic_store_explicit( line_no, this, __m__, __x__ ); }

inline unsigned short atomic_ushort::load
( int line_no, memory_order __x__ ) volatile
{ return atomic_load_explicit( line_no, this, __x__ ); }

inline unsigned short atomic_ushort::exchange
( int line_no, unsigned short __m__, memory_order __x__ ) volatile
{ return atomic_exchange_explicit( line_no, this, __m__, __x__ ); }

inline bool atomic_ushort::compare_exchange_weak
( int line_no,  unsigned short& __e__, unsigned short __m__,
  memory_order __x__, memory_order __y__ ) volatile
{ return atomic_compare_exchange_weak_explicit( line_no, this, &__e__, __m__, __x__, __y__ ); }

inline bool atomic_ushort::compare_exchange_strong
( int line_no, unsigned short& __e__, unsigned short __m__,
  memory_order __x__, memory_order __y__ ) volatile
{ return atomic_compare_exchange_strong_explicit( line_no, this, &__e__, __m__, __x__, __y__ ); }

inline bool atomic_ushort::compare_exchange_weak
( int line_no,  unsigned short& __e__, unsigned short __m__, memory_order __x__ ) volatile
{ return atomic_compare_exchange_weak_explicit( line_no, this, &__e__, __m__, __x__,
      __x__ == memory_order_acq_rel ? memory_order_acquire :
      __x__ == memory_order_release ? memory_order_relaxed : __x__ ); }

inline bool atomic_ushort::compare_exchange_strong
( int line_no, unsigned short& __e__, unsigned short __m__, memory_order __x__ ) volatile
{ return atomic_compare_exchange_strong_explicit( line_no, this, &__e__, __m__, __x__,
      __x__ == memory_order_acq_rel ? memory_order_acquire :
      __x__ == memory_order_release ? memory_order_relaxed : __x__ ); }


inline bool atomic_int::is_lock_free() const volatile
{ return false; }

inline void atomic_int::store
( int line_no, int __m__, memory_order __x__ ) volatile
{ atomic_store_explicit( line_no, this, __m__, __x__ ); }

inline int atomic_int::load
( int line_no, memory_order __x__ ) volatile
{ return atomic_load_explicit( line_no, this, __x__ ); }

inline int atomic_int::exchange
( int line_no, int __m__, memory_order __x__ ) volatile
{ return atomic_exchange_explicit( line_no, this, __m__, __x__ ); }

inline bool atomic_int::compare_exchange_weak
( int line_no,  int& __e__, int __m__,
  memory_order __x__, memory_order __y__ ) volatile
{ return atomic_compare_exchange_weak_explicit( line_no, this, &__e__, __m__, __x__, __y__ ); }

inline bool atomic_int::compare_exchange_strong
( int line_no, int& __e__, int __m__,
  memory_order __x__, memory_order __y__ ) volatile
{ return atomic_compare_exchange_strong_explicit( line_no, this, &__e__, __m__, __x__, __y__ ); }

inline bool atomic_int::compare_exchange_weak
( int line_no,  int& __e__, int __m__, memory_order __x__ ) volatile
{ return atomic_compare_exchange_weak_explicit( line_no, this, &__e__, __m__, __x__,
      __x__ == memory_order_acq_rel ? memory_order_acquire :
      __x__ == memory_order_release ? memory_order_relaxed : __x__ ); }

inline bool atomic_int::compare_exchange_strong
( int line_no, int& __e__, int __m__, memory_order __x__ ) volatile
{ return atomic_compare_exchange_strong_explicit( line_no, this, &__e__, __m__, __x__,
      __x__ == memory_order_acq_rel ? memory_order_acquire :
      __x__ == memory_order_release ? memory_order_relaxed : __x__ ); }


inline bool atomic_uint::is_lock_free() const volatile
{ return false; }

inline void atomic_uint::store
( int line_no, unsigned int __m__, memory_order __x__ ) volatile
{ atomic_store_explicit( line_no, this, __m__, __x__ ); }

inline unsigned int atomic_uint::load
( int line_no, memory_order __x__ ) volatile
{ return atomic_load_explicit( line_no, this, __x__ ); }

inline unsigned int atomic_uint::exchange
( int line_no, unsigned int __m__, memory_order __x__ ) volatile
{ return atomic_exchange_explicit( line_no, this, __m__, __x__ ); }

inline bool atomic_uint::compare_exchange_weak
( int line_no,  unsigned int& __e__, unsigned int __m__,
  memory_order __x__, memory_order __y__ ) volatile
{ return atomic_compare_exchange_weak_explicit( line_no, this, &__e__, __m__, __x__, __y__ ); }

inline bool atomic_uint::compare_exchange_strong
( int line_no, unsigned int& __e__, unsigned int __m__,
  memory_order __x__, memory_order __y__ ) volatile
{ return atomic_compare_exchange_strong_explicit( line_no, this, &__e__, __m__, __x__, __y__ ); }

inline bool atomic_uint::compare_exchange_weak
( int line_no,  unsigned int& __e__, unsigned int __m__, memory_order __x__ ) volatile
{ return atomic_compare_exchange_weak_explicit( line_no, this, &__e__, __m__, __x__,
      __x__ == memory_order_acq_rel ? memory_order_acquire :
      __x__ == memory_order_release ? memory_order_relaxed : __x__ ); }

inline bool atomic_uint::compare_exchange_strong
( int line_no, unsigned int& __e__, unsigned int __m__, memory_order __x__ ) volatile
{ return atomic_compare_exchange_strong_explicit( line_no, this, &__e__, __m__, __x__,
      __x__ == memory_order_acq_rel ? memory_order_acquire :
      __x__ == memory_order_release ? memory_order_relaxed : __x__ ); }


inline bool atomic_long::is_lock_free() const volatile
{ return false; }

inline void atomic_long::store
( int line_no, long __m__, memory_order __x__ ) volatile
{ atomic_store_explicit( line_no, this, __m__, __x__ ); }

inline long atomic_long::load
( int line_no, memory_order __x__ ) volatile
{ return atomic_load_explicit( line_no, this, __x__ ); }

inline long atomic_long::exchange
( int line_no, long __m__, memory_order __x__ ) volatile
{ return atomic_exchange_explicit( line_no, this, __m__, __x__ ); }

inline bool atomic_long::compare_exchange_weak
( int line_no,  long& __e__, long __m__,
  memory_order __x__, memory_order __y__ ) volatile
{ return atomic_compare_exchange_weak_explicit( line_no, this, &__e__, __m__, __x__, __y__ ); }

inline bool atomic_long::compare_exchange_strong
( int line_no, long& __e__, long __m__,
  memory_order __x__, memory_order __y__ ) volatile
{ return atomic_compare_exchange_strong_explicit( line_no, this, &__e__, __m__, __x__, __y__ ); }

inline bool atomic_long::compare_exchange_weak
( int line_no,  long& __e__, long __m__, memory_order __x__ ) volatile
{ return atomic_compare_exchange_weak_explicit( line_no, this, &__e__, __m__, __x__,
      __x__ == memory_order_acq_rel ? memory_order_acquire :
      __x__ == memory_order_release ? memory_order_relaxed : __x__ ); }

inline bool atomic_long::compare_exchange_strong
( int line_no, long& __e__, long __m__, memory_order __x__ ) volatile
{ return atomic_compare_exchange_strong_explicit( line_no, this, &__e__, __m__, __x__,
      __x__ == memory_order_acq_rel ? memory_order_acquire :
      __x__ == memory_order_release ? memory_order_relaxed : __x__ ); }


inline bool atomic_ulong::is_lock_free() const volatile
{ return false; }

inline void atomic_ulong::store
( int line_no, unsigned long __m__, memory_order __x__ ) volatile
{ atomic_store_explicit( line_no, this, __m__, __x__ ); }

inline unsigned long atomic_ulong::load
( int line_no, memory_order __x__ ) volatile
{ return atomic_load_explicit( line_no, this, __x__ ); }

inline unsigned long atomic_ulong::exchange
( int line_no, unsigned long __m__, memory_order __x__ ) volatile
{ return atomic_exchange_explicit( line_no, this, __m__, __x__ ); }

inline bool atomic_ulong::compare_exchange_weak
( int line_no,  unsigned long& __e__, unsigned long __m__,
  memory_order __x__, memory_order __y__ ) volatile
{ return atomic_compare_exchange_weak_explicit( line_no, this, &__e__, __m__, __x__, __y__ ); }

inline bool atomic_ulong::compare_exchange_strong
( int line_no, unsigned long& __e__, unsigned long __m__,
  memory_order __x__, memory_order __y__ ) volatile
{ return atomic_compare_exchange_strong_explicit( line_no, this, &__e__, __m__, __x__, __y__ ); }

inline bool atomic_ulong::compare_exchange_weak
( int line_no,  unsigned long& __e__, unsigned long __m__, memory_order __x__ ) volatile
{ return atomic_compare_exchange_weak_explicit( line_no, this, &__e__, __m__, __x__,
      __x__ == memory_order_acq_rel ? memory_order_acquire :
      __x__ == memory_order_release ? memory_order_relaxed : __x__ ); }

inline bool atomic_ulong::compare_exchange_strong
( int line_no, unsigned long& __e__, unsigned long __m__, memory_order __x__ ) volatile
{ return atomic_compare_exchange_strong_explicit( line_no, this, &__e__, __m__, __x__,
      __x__ == memory_order_acq_rel ? memory_order_acquire :
      __x__ == memory_order_release ? memory_order_relaxed : __x__ ); }


inline bool atomic_llong::is_lock_free() const volatile
{ return false; }

inline void atomic_llong::store
( int line_no, long long __m__, memory_order __x__ ) volatile
{ atomic_store_explicit( line_no, this, __m__, __x__ ); }

inline long long atomic_llong::load
( int line_no, memory_order __x__ ) volatile
{ return atomic_load_explicit( line_no, this, __x__ ); }

inline long long atomic_llong::exchange
( int line_no, long long __m__, memory_order __x__ ) volatile
{ return atomic_exchange_explicit( line_no, this, __m__, __x__ ); }

inline bool atomic_llong::compare_exchange_weak
( int line_no,  long long& __e__, long long __m__,
  memory_order __x__, memory_order __y__ ) volatile
{ return atomic_compare_exchange_weak_explicit( line_no, this, &__e__, __m__, __x__, __y__ ); }

inline bool atomic_llong::compare_exchange_strong
( int line_no, long long& __e__, long long __m__,
  memory_order __x__, memory_order __y__ ) volatile
{ return atomic_compare_exchange_strong_explicit( line_no, this, &__e__, __m__, __x__, __y__ ); }

inline bool atomic_llong::compare_exchange_weak
( int line_no,  long long& __e__, long long __m__, memory_order __x__ ) volatile
{ return atomic_compare_exchange_weak_explicit( line_no, this, &__e__, __m__, __x__,
      __x__ == memory_order_acq_rel ? memory_order_acquire :
      __x__ == memory_order_release ? memory_order_relaxed : __x__ ); }

inline bool atomic_llong::compare_exchange_strong
( int line_no, long long& __e__, long long __m__, memory_order __x__ ) volatile
{ return atomic_compare_exchange_strong_explicit( line_no, this, &__e__, __m__, __x__,
      __x__ == memory_order_acq_rel ? memory_order_acquire :
      __x__ == memory_order_release ? memory_order_relaxed : __x__ ); }


inline bool atomic_ullong::is_lock_free() const volatile
{ return false; }

inline void atomic_ullong::store
( int line_no, unsigned long long __m__, memory_order __x__ ) volatile
{ atomic_store_explicit( line_no, this, __m__, __x__ ); }

inline unsigned long long atomic_ullong::load
( int line_no, memory_order __x__ ) volatile
{ return atomic_load_explicit( line_no, this, __x__ ); }

inline unsigned long long atomic_ullong::exchange
( int line_no, unsigned long long __m__, memory_order __x__ ) volatile
{ return atomic_exchange_explicit( line_no, this, __m__, __x__ ); }

inline bool atomic_ullong::compare_exchange_weak
( int line_no,  unsigned long long& __e__, unsigned long long __m__,
  memory_order __x__, memory_order __y__ ) volatile
{ return atomic_compare_exchange_weak_explicit( line_no, this, &__e__, __m__, __x__, __y__ ); }

inline bool atomic_ullong::compare_exchange_strong
( int line_no, unsigned long long& __e__, unsigned long long __m__,
  memory_order __x__, memory_order __y__ ) volatile
{ return atomic_compare_exchange_strong_explicit( line_no, this, &__e__, __m__, __x__, __y__ ); }

inline bool atomic_ullong::compare_exchange_weak
( int line_no,  unsigned long long& __e__, unsigned long long __m__, memory_order __x__ ) volatile
{ return atomic_compare_exchange_weak_explicit( line_no, this, &__e__, __m__, __x__,
      __x__ == memory_order_acq_rel ? memory_order_acquire :
      __x__ == memory_order_release ? memory_order_relaxed : __x__ ); }

inline bool atomic_ullong::compare_exchange_strong
( int line_no, unsigned long long& __e__, unsigned long long __m__, memory_order __x__ ) volatile
{ return atomic_compare_exchange_strong_explicit( line_no, this, &__e__, __m__, __x__,
      __x__ == memory_order_acq_rel ? memory_order_acquire :
      __x__ == memory_order_release ? memory_order_relaxed : __x__ ); }


inline bool atomic_wchar_t::is_lock_free() const volatile
{ return false; }

inline void atomic_wchar_t::store
( int line_no, wchar_t __m__, memory_order __x__ ) volatile
{ atomic_store_explicit( line_no, this, __m__, __x__ ); }

inline wchar_t atomic_wchar_t::load
( int line_no, memory_order __x__ ) volatile
{ return atomic_load_explicit( line_no, this, __x__ ); }

inline wchar_t atomic_wchar_t::exchange
( int line_no, wchar_t __m__, memory_order __x__ ) volatile
{ return atomic_exchange_explicit( line_no, this, __m__, __x__ ); }

inline bool atomic_wchar_t::compare_exchange_weak
( int line_no,  wchar_t& __e__, wchar_t __m__,
  memory_order __x__, memory_order __y__ ) volatile
{ return atomic_compare_exchange_weak_explicit( line_no, this, &__e__, __m__, __x__, __y__ ); }

inline bool atomic_wchar_t::compare_exchange_strong
( int line_no, wchar_t& __e__, wchar_t __m__,
  memory_order __x__, memory_order __y__ ) volatile
{ return atomic_compare_exchange_strong_explicit( line_no, this, &__e__, __m__, __x__, __y__ ); }

inline bool atomic_wchar_t::compare_exchange_weak
( int line_no,  wchar_t& __e__, wchar_t __m__, memory_order __x__ ) volatile
{ return atomic_compare_exchange_weak_explicit( line_no, this, &__e__, __m__, __x__,
      __x__ == memory_order_acq_rel ? memory_order_acquire :
      __x__ == memory_order_release ? memory_order_relaxed : __x__ ); }

inline bool atomic_wchar_t::compare_exchange_strong
( int line_no, wchar_t& __e__, wchar_t __m__, memory_order __x__ ) volatile
{ return atomic_compare_exchange_strong_explicit( line_no, this, &__e__, __m__, __x__,
      __x__ == memory_order_acq_rel ? memory_order_acquire :
      __x__ == memory_order_release ? memory_order_relaxed : __x__ ); }


template< typename T >
inline bool atomic<T>::is_lock_free() const volatile
{ return false; }

template< typename T >
inline void atomic<T>::store( int line_no, T __v__, memory_order __x__ ) volatile
{ _ATOMIC_STORE_( line_no, this, __v__, __x__ ); }

template< typename T >
inline T atomic<T>::load( int line_no, memory_order __x__ ) volatile
{ return _ATOMIC_LOAD_( line_no, this, __x__ ); }

template< typename T >
inline T atomic<T>::exchange( int line_no, T __v__, memory_order __x__ ) volatile
{ return _ATOMIC_MODIFY_( line_no, this, =, __v__, __x__ ); }

template< typename T >
inline bool atomic<T>::compare_exchange_weak
( int line_no,  T& __r__, T __v__, memory_order __x__, memory_order __y__ ) volatile
{ return _ATOMIC_CMPSWP_WEAK_( line_no, this, &__r__, __v__, __x__ ); }

template< typename T >
inline bool atomic<T>::compare_exchange_strong
( int line_no, T& __r__, T __v__, memory_order __x__, memory_order __y__ ) volatile
{ return _ATOMIC_CMPSWP_( line_no, this, &__r__, __v__, __x__ ); }

template< typename T >
inline bool atomic<T>::compare_exchange_weak
( int line_no,  T& __r__, T __v__, memory_order __x__ ) volatile
{ return compare_exchange_weak( line_no,  __r__, __v__, __x__,
      __x__ == memory_order_acq_rel ? memory_order_acquire :
      __x__ == memory_order_release ? memory_order_relaxed : __x__ ); }

template< typename T >
inline bool atomic<T>::compare_exchange_strong
( int line_no, T& __r__, T __v__, memory_order __x__ ) volatile
{ return compare_exchange_strong( line_no, __r__, __v__, __x__,
      __x__ == memory_order_acq_rel ? memory_order_acquire :
      __x__ == memory_order_release ? memory_order_relaxed : __x__ ); }


inline void* atomic_address::fetch_add
( int line_no, ptrdiff_t __m__, memory_order __x__ ) volatile
{ return atomic_fetch_add_explicit( line_no, this, __m__, __x__ ); }

inline void* atomic_address::fetch_sub
( int line_no, ptrdiff_t __m__, memory_order __x__ ) volatile
{ return atomic_fetch_sub_explicit( line_no, this, __m__, __x__ ); }


inline char atomic_char::fetch_add
( int line_no, char __m__, memory_order __x__ ) volatile
{ return atomic_fetch_add_explicit( line_no, this, __m__, __x__ ); }


inline char atomic_char::fetch_sub
( int line_no, char __m__, memory_order __x__ ) volatile
{ return atomic_fetch_sub_explicit( line_no, this, __m__, __x__ ); }


inline char atomic_char::fetch_and
( int line_no, char __m__, memory_order __x__ ) volatile
{ return atomic_fetch_and_explicit( line_no, this, __m__, __x__ ); }


inline char atomic_char::fetch_or
( int line_no, char __m__, memory_order __x__ ) volatile
{ return atomic_fetch_or_explicit( line_no, this, __m__, __x__ ); }


inline char atomic_char::fetch_xor
( int line_no, char __m__, memory_order __x__ ) volatile
{ return atomic_fetch_xor_explicit( line_no, this, __m__, __x__ ); }


inline signed char atomic_schar::fetch_add
( int line_no, signed char __m__, memory_order __x__ ) volatile
{ return atomic_fetch_add_explicit( line_no, this, __m__, __x__ ); }


inline signed char atomic_schar::fetch_sub
( int line_no, signed char __m__, memory_order __x__ ) volatile
{ return atomic_fetch_sub_explicit( line_no, this, __m__, __x__ ); }


inline signed char atomic_schar::fetch_and
( int line_no, signed char __m__, memory_order __x__ ) volatile
{ return atomic_fetch_and_explicit( line_no, this, __m__, __x__ ); }


inline signed char atomic_schar::fetch_or
( int line_no, signed char __m__, memory_order __x__ ) volatile
{ return atomic_fetch_or_explicit( line_no, this, __m__, __x__ ); }


inline signed char atomic_schar::fetch_xor
( int line_no, signed char __m__, memory_order __x__ ) volatile
{ return atomic_fetch_xor_explicit( line_no, this, __m__, __x__ ); }


inline unsigned char atomic_uchar::fetch_add
( int line_no, unsigned char __m__, memory_order __x__ ) volatile
{ return atomic_fetch_add_explicit( line_no, this, __m__, __x__ ); }


inline unsigned char atomic_uchar::fetch_sub
( int line_no, unsigned char __m__, memory_order __x__ ) volatile
{ return atomic_fetch_sub_explicit( line_no, this, __m__, __x__ ); }


inline unsigned char atomic_uchar::fetch_and
( int line_no, unsigned char __m__, memory_order __x__ ) volatile
{ return atomic_fetch_and_explicit( line_no, this, __m__, __x__ ); }


inline unsigned char atomic_uchar::fetch_or
( int line_no, unsigned char __m__, memory_order __x__ ) volatile
{ return atomic_fetch_or_explicit( line_no, this, __m__, __x__ ); }


inline unsigned char atomic_uchar::fetch_xor
( int line_no, unsigned char __m__, memory_order __x__ ) volatile
{ return atomic_fetch_xor_explicit( line_no, this, __m__, __x__ ); }


inline short atomic_short::fetch_add
( int line_no, short __m__, memory_order __x__ ) volatile
{ return atomic_fetch_add_explicit( line_no, this, __m__, __x__ ); }


inline short atomic_short::fetch_sub
( int line_no, short __m__, memory_order __x__ ) volatile
{ return atomic_fetch_sub_explicit( line_no, this, __m__, __x__ ); }


inline short atomic_short::fetch_and
( int line_no, short __m__, memory_order __x__ ) volatile
{ return atomic_fetch_and_explicit( line_no, this, __m__, __x__ ); }


inline short atomic_short::fetch_or
( int line_no, short __m__, memory_order __x__ ) volatile
{ return atomic_fetch_or_explicit( line_no, this, __m__, __x__ ); }


inline short atomic_short::fetch_xor
( int line_no, short __m__, memory_order __x__ ) volatile
{ return atomic_fetch_xor_explicit( line_no, this, __m__, __x__ ); }


inline unsigned short atomic_ushort::fetch_add
( int line_no, unsigned short __m__, memory_order __x__ ) volatile
{ return atomic_fetch_add_explicit( line_no, this, __m__, __x__ ); }


inline unsigned short atomic_ushort::fetch_sub
( int line_no, unsigned short __m__, memory_order __x__ ) volatile
{ return atomic_fetch_sub_explicit( line_no, this, __m__, __x__ ); }


inline unsigned short atomic_ushort::fetch_and
( int line_no, unsigned short __m__, memory_order __x__ ) volatile
{ return atomic_fetch_and_explicit( line_no, this, __m__, __x__ ); }


inline unsigned short atomic_ushort::fetch_or
( int line_no, unsigned short __m__, memory_order __x__ ) volatile
{ return atomic_fetch_or_explicit( line_no, this, __m__, __x__ ); }


inline unsigned short atomic_ushort::fetch_xor
( int line_no, unsigned short __m__, memory_order __x__ ) volatile
{ return atomic_fetch_xor_explicit( line_no, this, __m__, __x__ ); }


inline int atomic_int::fetch_add
( int line_no, int __m__, memory_order __x__ ) volatile
{ return atomic_fetch_add_explicit( line_no, this, __m__, __x__ ); }


inline int atomic_int::fetch_sub
( int line_no, int __m__, memory_order __x__ ) volatile
{ return atomic_fetch_sub_explicit( line_no, this, __m__, __x__ ); }


inline int atomic_int::fetch_and
( int line_no, int __m__, memory_order __x__ ) volatile
{ return atomic_fetch_and_explicit( line_no, this, __m__, __x__ ); }


inline int atomic_int::fetch_or
( int line_no, int __m__, memory_order __x__ ) volatile
{ return atomic_fetch_or_explicit( line_no, this, __m__, __x__ ); }


inline int atomic_int::fetch_xor
( int line_no, int __m__, memory_order __x__ ) volatile
{ return atomic_fetch_xor_explicit( line_no, this, __m__, __x__ ); }


inline unsigned int atomic_uint::fetch_add
( int line_no, unsigned int __m__, memory_order __x__ ) volatile
{ return atomic_fetch_add_explicit( line_no, this, __m__, __x__ ); }


inline unsigned int atomic_uint::fetch_sub
( int line_no, unsigned int __m__, memory_order __x__ ) volatile
{ return atomic_fetch_sub_explicit( line_no, this, __m__, __x__ ); }


inline unsigned int atomic_uint::fetch_and
( int line_no, unsigned int __m__, memory_order __x__ ) volatile
{ return atomic_fetch_and_explicit( line_no, this, __m__, __x__ ); }


inline unsigned int atomic_uint::fetch_or
( int line_no, unsigned int __m__, memory_order __x__ ) volatile
{ return atomic_fetch_or_explicit( line_no, this, __m__, __x__ ); }


inline unsigned int atomic_uint::fetch_xor
( int line_no, unsigned int __m__, memory_order __x__ ) volatile
{ return atomic_fetch_xor_explicit( line_no, this, __m__, __x__ ); }


inline long atomic_long::fetch_add
( int line_no, long __m__, memory_order __x__ ) volatile
{ return atomic_fetch_add_explicit( line_no, this, __m__, __x__ ); }


inline long atomic_long::fetch_sub
( int line_no, long __m__, memory_order __x__ ) volatile
{ return atomic_fetch_sub_explicit( line_no, this, __m__, __x__ ); }


inline long atomic_long::fetch_and
( int line_no, long __m__, memory_order __x__ ) volatile
{ return atomic_fetch_and_explicit( line_no, this, __m__, __x__ ); }


inline long atomic_long::fetch_or
( int line_no, long __m__, memory_order __x__ ) volatile
{ return atomic_fetch_or_explicit( line_no, this, __m__, __x__ ); }


inline long atomic_long::fetch_xor
( int line_no, long __m__, memory_order __x__ ) volatile
{ return atomic_fetch_xor_explicit( line_no, this, __m__, __x__ ); }


inline unsigned long atomic_ulong::fetch_add
( int line_no, unsigned long __m__, memory_order __x__ ) volatile
{ return atomic_fetch_add_explicit( line_no, this, __m__, __x__ ); }


inline unsigned long atomic_ulong::fetch_sub
( int line_no, unsigned long __m__, memory_order __x__ ) volatile
{ return atomic_fetch_sub_explicit( line_no, this, __m__, __x__ ); }


inline unsigned long atomic_ulong::fetch_and
( int line_no, unsigned long __m__, memory_order __x__ ) volatile
{ return atomic_fetch_and_explicit( line_no, this, __m__, __x__ ); }


inline unsigned long atomic_ulong::fetch_or
( int line_no, unsigned long __m__, memory_order __x__ ) volatile
{ return atomic_fetch_or_explicit( line_no, this, __m__, __x__ ); }


inline unsigned long atomic_ulong::fetch_xor
( int line_no, unsigned long __m__, memory_order __x__ ) volatile
{ return atomic_fetch_xor_explicit( line_no, this, __m__, __x__ ); }


inline long long atomic_llong::fetch_add
( int line_no, long long __m__, memory_order __x__ ) volatile
{ return atomic_fetch_add_explicit( line_no, this, __m__, __x__ ); }


inline long long atomic_llong::fetch_sub
( int line_no, long long __m__, memory_order __x__ ) volatile
{ return atomic_fetch_sub_explicit( line_no, this, __m__, __x__ ); }


inline long long atomic_llong::fetch_and
( int line_no, long long __m__, memory_order __x__ ) volatile
{ return atomic_fetch_and_explicit( line_no, this, __m__, __x__ ); }


inline long long atomic_llong::fetch_or
( int line_no, long long __m__, memory_order __x__ ) volatile
{ return atomic_fetch_or_explicit( line_no, this, __m__, __x__ ); }


inline long long atomic_llong::fetch_xor
( int line_no, long long __m__, memory_order __x__ ) volatile
{ return atomic_fetch_xor_explicit( line_no, this, __m__, __x__ ); }


inline unsigned long long atomic_ullong::fetch_add
( int line_no, unsigned long long __m__, memory_order __x__ ) volatile
{ return atomic_fetch_add_explicit( line_no, this, __m__, __x__ ); }


inline unsigned long long atomic_ullong::fetch_sub
( int line_no, unsigned long long __m__, memory_order __x__ ) volatile
{ return atomic_fetch_sub_explicit( line_no, this, __m__, __x__ ); }


inline unsigned long long atomic_ullong::fetch_and
( int line_no, unsigned long long __m__, memory_order __x__ ) volatile
{ return atomic_fetch_and_explicit( line_no, this, __m__, __x__ ); }


inline unsigned long long atomic_ullong::fetch_or
( int line_no, unsigned long long __m__, memory_order __x__ ) volatile
{ return atomic_fetch_or_explicit( line_no, this, __m__, __x__ ); }


inline unsigned long long atomic_ullong::fetch_xor
( int line_no, unsigned long long __m__, memory_order __x__ ) volatile
{ return atomic_fetch_xor_explicit( line_no, this, __m__, __x__ ); }


inline wchar_t atomic_wchar_t::fetch_add
( int line_no, wchar_t __m__, memory_order __x__ ) volatile
{ return atomic_fetch_add_explicit( line_no, this, __m__, __x__ ); }


inline wchar_t atomic_wchar_t::fetch_sub
( int line_no, wchar_t __m__, memory_order __x__ ) volatile
{ return atomic_fetch_sub_explicit( line_no, this, __m__, __x__ ); }


inline wchar_t atomic_wchar_t::fetch_and
( int line_no, wchar_t __m__, memory_order __x__ ) volatile
{ return atomic_fetch_and_explicit( line_no, this, __m__, __x__ ); }


inline wchar_t atomic_wchar_t::fetch_or
( int line_no, wchar_t __m__, memory_order __x__ ) volatile
{ return atomic_fetch_or_explicit( line_no, this, __m__, __x__ ); }


inline wchar_t atomic_wchar_t::fetch_xor
( int line_no, wchar_t __m__, memory_order __x__ ) volatile
{ return atomic_fetch_xor_explicit( line_no, this, __m__, __x__ ); }


template< typename T >
T* atomic<T*>::load( int line_no, memory_order __x__ ) volatile
{ return static_cast<T*>( atomic_address::load( line_no, __x__ ) ); }

template< typename T >
T* atomic<T*>::exchange( int line_no, T* __v__, memory_order __x__ ) volatile
{ return static_cast<T*>( atomic_address::exchange( line_no, __v__, __x__ ) ); }

template< typename T >
bool atomic<T*>::compare_exchange_weak
( int line_no,  T*& __r__, T* __v__, memory_order __x__, memory_order __y__) volatile
{ return atomic_address::compare_exchange_weak( line_no,  *reinterpret_cast<void**>( &__r__ ),
               static_cast<void*>( __v__ ), __x__, __y__ ); }
//{ return _ATOMIC_CMPSWP_WEAK_( line_no, this, &__r__, __v__, __x__ ); }

template< typename T >
bool atomic<T*>::compare_exchange_strong
( int line_no, T*& __r__, T* __v__, memory_order __x__, memory_order __y__) volatile
{ return atomic_address::compare_exchange_strong( line_no, *reinterpret_cast<void**>( &__r__ ),
               static_cast<void*>( __v__ ), __x__, __y__ ); }
//{ return _ATOMIC_CMPSWP_( line_no, this, &__r__, __v__, __x__ ); }

template< typename T >
bool atomic<T*>::compare_exchange_weak
( int line_no,  T*& __r__, T* __v__, memory_order __x__ ) volatile
{ return compare_exchange_weak( line_no,  __r__, __v__, __x__,
      __x__ == memory_order_acq_rel ? memory_order_acquire :
      __x__ == memory_order_release ? memory_order_relaxed : __x__ ); }

template< typename T >
bool atomic<T*>::compare_exchange_strong
( int line_no, T*& __r__, T* __v__, memory_order __x__ ) volatile
{ return compare_exchange_strong( line_no, __r__, __v__, __x__,
      __x__ == memory_order_acq_rel ? memory_order_acquire :
      __x__ == memory_order_release ? memory_order_relaxed : __x__ ); }

template< typename T >
T* atomic<T*>::fetch_add( int line_no, ptrdiff_t __v__, memory_order __x__ ) volatile
{ return atomic_fetch_add_explicit( line_no, this, sizeof(T) * __v__, __x__ ); }

template< typename T >
T* atomic<T*>::fetch_sub( int line_no, ptrdiff_t __v__, memory_order __x__ ) volatile
{ return atomic_fetch_sub_explicit( line_no, this, sizeof(T) * __v__, __x__ ); }


#endif

#ifdef __cplusplus
extern "C" {
#endif
static inline void atomic_thread_fence(memory_order order)
{ _ATOMIC_FENCE_(order); }

/** @todo Do we want to try to support a user's signal-handler? */
static inline void atomic_signal_fence(memory_order order)
{ /* No-op? */ }
#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
} // namespace std
#endif

#endif /* __IMPATOMIC_H__ */
