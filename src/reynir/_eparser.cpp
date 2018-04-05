#define _CFFI_

/* We try to define Py_LIMITED_API before including Python.h.

   Mess: we can only define it if Py_DEBUG, Py_TRACE_REFS and
   Py_REF_DEBUG are not defined.  This is a best-effort approximation:
   we can learn about Py_DEBUG from pyconfig.h, but it is unclear if
   the same works for the other two macros.  Py_DEBUG implies them,
   but not the other way around.
*/
#ifndef _CFFI_USE_EMBEDDING
#  include <pyconfig.h>
#  if !defined(Py_DEBUG) && !defined(Py_TRACE_REFS) && !defined(Py_REF_DEBUG)
#    define Py_LIMITED_API
#  endif
#endif

#include <Python.h>
#ifdef __cplusplus
extern "C" {
#endif
#include <stddef.h>

/* This part is from file 'cffi/parse_c_type.h'.  It is copied at the
   beginning of C sources generated by CFFI's ffi.set_source(). */

typedef void *_cffi_opcode_t;

#define _CFFI_OP(opcode, arg)   (_cffi_opcode_t)(opcode | (((uintptr_t)(arg)) << 8))
#define _CFFI_GETOP(cffi_opcode)    ((unsigned char)(uintptr_t)cffi_opcode)
#define _CFFI_GETARG(cffi_opcode)   (((intptr_t)cffi_opcode) >> 8)

#define _CFFI_OP_PRIMITIVE       1
#define _CFFI_OP_POINTER         3
#define _CFFI_OP_ARRAY           5
#define _CFFI_OP_OPEN_ARRAY      7
#define _CFFI_OP_STRUCT_UNION    9
#define _CFFI_OP_ENUM           11
#define _CFFI_OP_FUNCTION       13
#define _CFFI_OP_FUNCTION_END   15
#define _CFFI_OP_NOOP           17
#define _CFFI_OP_BITFIELD       19
#define _CFFI_OP_TYPENAME       21
#define _CFFI_OP_CPYTHON_BLTN_V 23   // varargs
#define _CFFI_OP_CPYTHON_BLTN_N 25   // noargs
#define _CFFI_OP_CPYTHON_BLTN_O 27   // O  (i.e. a single arg)
#define _CFFI_OP_CONSTANT       29
#define _CFFI_OP_CONSTANT_INT   31
#define _CFFI_OP_GLOBAL_VAR     33
#define _CFFI_OP_DLOPEN_FUNC    35
#define _CFFI_OP_DLOPEN_CONST   37
#define _CFFI_OP_GLOBAL_VAR_F   39
#define _CFFI_OP_EXTERN_PYTHON  41

#define _CFFI_PRIM_VOID          0
#define _CFFI_PRIM_BOOL          1
#define _CFFI_PRIM_CHAR          2
#define _CFFI_PRIM_SCHAR         3
#define _CFFI_PRIM_UCHAR         4
#define _CFFI_PRIM_SHORT         5
#define _CFFI_PRIM_USHORT        6
#define _CFFI_PRIM_INT           7
#define _CFFI_PRIM_UINT          8
#define _CFFI_PRIM_LONG          9
#define _CFFI_PRIM_ULONG        10
#define _CFFI_PRIM_LONGLONG     11
#define _CFFI_PRIM_ULONGLONG    12
#define _CFFI_PRIM_FLOAT        13
#define _CFFI_PRIM_DOUBLE       14
#define _CFFI_PRIM_LONGDOUBLE   15

#define _CFFI_PRIM_WCHAR        16
#define _CFFI_PRIM_INT8         17
#define _CFFI_PRIM_UINT8        18
#define _CFFI_PRIM_INT16        19
#define _CFFI_PRIM_UINT16       20
#define _CFFI_PRIM_INT32        21
#define _CFFI_PRIM_UINT32       22
#define _CFFI_PRIM_INT64        23
#define _CFFI_PRIM_UINT64       24
#define _CFFI_PRIM_INTPTR       25
#define _CFFI_PRIM_UINTPTR      26
#define _CFFI_PRIM_PTRDIFF      27
#define _CFFI_PRIM_SIZE         28
#define _CFFI_PRIM_SSIZE        29
#define _CFFI_PRIM_INT_LEAST8   30
#define _CFFI_PRIM_UINT_LEAST8  31
#define _CFFI_PRIM_INT_LEAST16  32
#define _CFFI_PRIM_UINT_LEAST16 33
#define _CFFI_PRIM_INT_LEAST32  34
#define _CFFI_PRIM_UINT_LEAST32 35
#define _CFFI_PRIM_INT_LEAST64  36
#define _CFFI_PRIM_UINT_LEAST64 37
#define _CFFI_PRIM_INT_FAST8    38
#define _CFFI_PRIM_UINT_FAST8   39
#define _CFFI_PRIM_INT_FAST16   40
#define _CFFI_PRIM_UINT_FAST16  41
#define _CFFI_PRIM_INT_FAST32   42
#define _CFFI_PRIM_UINT_FAST32  43
#define _CFFI_PRIM_INT_FAST64   44
#define _CFFI_PRIM_UINT_FAST64  45
#define _CFFI_PRIM_INTMAX       46
#define _CFFI_PRIM_UINTMAX      47

#define _CFFI__NUM_PRIM         48
#define _CFFI__UNKNOWN_PRIM           (-1)
#define _CFFI__UNKNOWN_FLOAT_PRIM     (-2)
#define _CFFI__UNKNOWN_LONG_DOUBLE    (-3)

#define _CFFI__IO_FILE_STRUCT         (-1)


struct _cffi_global_s {
    const char *name;
    void *address;
    _cffi_opcode_t type_op;
    void *size_or_direct_fn;  // OP_GLOBAL_VAR: size, or 0 if unknown
                              // OP_CPYTHON_BLTN_*: addr of direct function
};

struct _cffi_getconst_s {
    unsigned long long value;
    const struct _cffi_type_context_s *ctx;
    int gindex;
};

struct _cffi_struct_union_s {
    const char *name;
    int type_index;          // -> _cffi_types, on a OP_STRUCT_UNION
    int flags;               // _CFFI_F_* flags below
    size_t size;
    int alignment;
    int first_field_index;   // -> _cffi_fields array
    int num_fields;
};
#define _CFFI_F_UNION         0x01   // is a union, not a struct
#define _CFFI_F_CHECK_FIELDS  0x02   // complain if fields are not in the
                                     // "standard layout" or if some are missing
#define _CFFI_F_PACKED        0x04   // for CHECK_FIELDS, assume a packed struct
#define _CFFI_F_EXTERNAL      0x08   // in some other ffi.include()
#define _CFFI_F_OPAQUE        0x10   // opaque

struct _cffi_field_s {
    const char *name;
    size_t field_offset;
    size_t field_size;
    _cffi_opcode_t field_type_op;
};

struct _cffi_enum_s {
    const char *name;
    int type_index;          // -> _cffi_types, on a OP_ENUM
    int type_prim;           // _CFFI_PRIM_xxx
    const char *enumerators; // comma-delimited string
};

struct _cffi_typename_s {
    const char *name;
    int type_index;   /* if opaque, points to a possibly artificial
                         OP_STRUCT which is itself opaque */
};

struct _cffi_type_context_s {
    _cffi_opcode_t *types;
    const struct _cffi_global_s *globals;
    const struct _cffi_field_s *fields;
    const struct _cffi_struct_union_s *struct_unions;
    const struct _cffi_enum_s *enums;
    const struct _cffi_typename_s *typenames;
    int num_globals;
    int num_struct_unions;
    int num_enums;
    int num_typenames;
    const char *const *includes;
    int num_types;
    int flags;      /* future extension */
};

struct _cffi_parse_info_s {
    const struct _cffi_type_context_s *ctx;
    _cffi_opcode_t *output;
    unsigned int output_size;
    size_t error_location;
    const char *error_message;
};

struct _cffi_externpy_s {
    const char *name;
    size_t size_of_result;
    void *reserved1, *reserved2;
};

#ifdef _CFFI_INTERNAL
static int parse_c_type(struct _cffi_parse_info_s *info, const char *input);
static int search_in_globals(const struct _cffi_type_context_s *ctx,
                             const char *search, size_t search_len);
static int search_in_struct_unions(const struct _cffi_type_context_s *ctx,
                                   const char *search, size_t search_len);
#endif

/* this block of #ifs should be kept exactly identical between
   c/_cffi_backend.c, cffi/vengine_cpy.py, cffi/vengine_gen.py
   and cffi/_cffi_include.h */
#if defined(_MSC_VER)
# include <malloc.h>   /* for alloca() */
# if _MSC_VER < 1600   /* MSVC < 2010 */
   typedef __int8 int8_t;
   typedef __int16 int16_t;
   typedef __int32 int32_t;
   typedef __int64 int64_t;
   typedef unsigned __int8 uint8_t;
   typedef unsigned __int16 uint16_t;
   typedef unsigned __int32 uint32_t;
   typedef unsigned __int64 uint64_t;
   typedef __int8 int_least8_t;
   typedef __int16 int_least16_t;
   typedef __int32 int_least32_t;
   typedef __int64 int_least64_t;
   typedef unsigned __int8 uint_least8_t;
   typedef unsigned __int16 uint_least16_t;
   typedef unsigned __int32 uint_least32_t;
   typedef unsigned __int64 uint_least64_t;
   typedef __int8 int_fast8_t;
   typedef __int16 int_fast16_t;
   typedef __int32 int_fast32_t;
   typedef __int64 int_fast64_t;
   typedef unsigned __int8 uint_fast8_t;
   typedef unsigned __int16 uint_fast16_t;
   typedef unsigned __int32 uint_fast32_t;
   typedef unsigned __int64 uint_fast64_t;
   typedef __int64 intmax_t;
   typedef unsigned __int64 uintmax_t;
# else
#  include <stdint.h>
# endif
# if _MSC_VER < 1800   /* MSVC < 2013 */
#  ifndef __cplusplus
    typedef unsigned char _Bool;
#  endif
# endif
#else
# include <stdint.h>
# if (defined (__SVR4) && defined (__sun)) || defined(_AIX) || defined(__hpux)
#  include <alloca.h>
# endif
#endif

#ifdef __GNUC__
# define _CFFI_UNUSED_FN  __attribute__((unused))
#else
# define _CFFI_UNUSED_FN  /* nothing */
#endif

#ifdef __cplusplus
# ifndef _Bool
   typedef bool _Bool;   /* semi-hackish: C++ has no _Bool; bool is builtin */
# endif
#endif

/**********  CPython-specific section  **********/
#ifndef PYPY_VERSION


#if PY_MAJOR_VERSION >= 3
# define PyInt_FromLong PyLong_FromLong
#endif

#define _cffi_from_c_double PyFloat_FromDouble
#define _cffi_from_c_float PyFloat_FromDouble
#define _cffi_from_c_long PyInt_FromLong
#define _cffi_from_c_ulong PyLong_FromUnsignedLong
#define _cffi_from_c_longlong PyLong_FromLongLong
#define _cffi_from_c_ulonglong PyLong_FromUnsignedLongLong

#define _cffi_to_c_double PyFloat_AsDouble
#define _cffi_to_c_float PyFloat_AsDouble

#define _cffi_from_c_int(x, type)                                        \
    (((type)-1) > 0 ? /* unsigned */                                     \
        (sizeof(type) < sizeof(long) ?                                   \
            PyInt_FromLong((long)x) :                                    \
         sizeof(type) == sizeof(long) ?                                  \
            PyLong_FromUnsignedLong((unsigned long)x) :                  \
            PyLong_FromUnsignedLongLong((unsigned long long)x)) :        \
        (sizeof(type) <= sizeof(long) ?                                  \
            PyInt_FromLong((long)x) :                                    \
            PyLong_FromLongLong((long long)x)))

#define _cffi_to_c_int(o, type)                                          \
    ((type)(                                                             \
     sizeof(type) == 1 ? (((type)-1) > 0 ? (type)_cffi_to_c_u8(o)        \
                                         : (type)_cffi_to_c_i8(o)) :     \
     sizeof(type) == 2 ? (((type)-1) > 0 ? (type)_cffi_to_c_u16(o)       \
                                         : (type)_cffi_to_c_i16(o)) :    \
     sizeof(type) == 4 ? (((type)-1) > 0 ? (type)_cffi_to_c_u32(o)       \
                                         : (type)_cffi_to_c_i32(o)) :    \
     sizeof(type) == 8 ? (((type)-1) > 0 ? (type)_cffi_to_c_u64(o)       \
                                         : (type)_cffi_to_c_i64(o)) :    \
     (Py_FatalError("unsupported size for type " #type), (type)0)))

#define _cffi_to_c_i8                                                    \
                 ((int(*)(PyObject *))_cffi_exports[1])
#define _cffi_to_c_u8                                                    \
                 ((int(*)(PyObject *))_cffi_exports[2])
#define _cffi_to_c_i16                                                   \
                 ((int(*)(PyObject *))_cffi_exports[3])
#define _cffi_to_c_u16                                                   \
                 ((int(*)(PyObject *))_cffi_exports[4])
#define _cffi_to_c_i32                                                   \
                 ((int(*)(PyObject *))_cffi_exports[5])
#define _cffi_to_c_u32                                                   \
                 ((unsigned int(*)(PyObject *))_cffi_exports[6])
#define _cffi_to_c_i64                                                   \
                 ((long long(*)(PyObject *))_cffi_exports[7])
#define _cffi_to_c_u64                                                   \
                 ((unsigned long long(*)(PyObject *))_cffi_exports[8])
#define _cffi_to_c_char                                                  \
                 ((int(*)(PyObject *))_cffi_exports[9])
#define _cffi_from_c_pointer                                             \
    ((PyObject *(*)(char *, struct _cffi_ctypedescr *))_cffi_exports[10])
#define _cffi_to_c_pointer                                               \
    ((char *(*)(PyObject *, struct _cffi_ctypedescr *))_cffi_exports[11])
#define _cffi_get_struct_layout                                          \
    not used any more
#define _cffi_restore_errno                                              \
    ((void(*)(void))_cffi_exports[13])
#define _cffi_save_errno                                                 \
    ((void(*)(void))_cffi_exports[14])
#define _cffi_from_c_char                                                \
    ((PyObject *(*)(char))_cffi_exports[15])
#define _cffi_from_c_deref                                               \
    ((PyObject *(*)(char *, struct _cffi_ctypedescr *))_cffi_exports[16])
#define _cffi_to_c                                                       \
    ((int(*)(char *, struct _cffi_ctypedescr *, PyObject *))_cffi_exports[17])
#define _cffi_from_c_struct                                              \
    ((PyObject *(*)(char *, struct _cffi_ctypedescr *))_cffi_exports[18])
#define _cffi_to_c_wchar_t                                               \
    ((wchar_t(*)(PyObject *))_cffi_exports[19])
#define _cffi_from_c_wchar_t                                             \
    ((PyObject *(*)(wchar_t))_cffi_exports[20])
#define _cffi_to_c_long_double                                           \
    ((long double(*)(PyObject *))_cffi_exports[21])
#define _cffi_to_c__Bool                                                 \
    ((_Bool(*)(PyObject *))_cffi_exports[22])
#define _cffi_prepare_pointer_call_argument                              \
    ((Py_ssize_t(*)(struct _cffi_ctypedescr *,                           \
                    PyObject *, char **))_cffi_exports[23])
#define _cffi_convert_array_from_object                                  \
    ((int(*)(char *, struct _cffi_ctypedescr *, PyObject *))_cffi_exports[24])
#define _CFFI_CPIDX  25
#define _cffi_call_python                                                \
    ((void(*)(struct _cffi_externpy_s *, char *))_cffi_exports[_CFFI_CPIDX])
#define _CFFI_NUM_EXPORTS 26

struct _cffi_ctypedescr;

static void *_cffi_exports[_CFFI_NUM_EXPORTS];

#define _cffi_type(index)   (                           \
    assert((((uintptr_t)_cffi_types[index]) & 1) == 0), \
    (struct _cffi_ctypedescr *)_cffi_types[index])

static PyObject *_cffi_init(const char *module_name, Py_ssize_t version,
                            const struct _cffi_type_context_s *ctx)
{
    PyObject *module, *o_arg, *new_module;
    void *raw[] = {
        (void *)module_name,
        (void *)version,
        (void *)_cffi_exports,
        (void *)ctx,
    };

    module = PyImport_ImportModule("_cffi_backend");
    if (module == NULL)
        goto failure;

    o_arg = PyLong_FromVoidPtr((void *)raw);
    if (o_arg == NULL)
        goto failure;

    new_module = PyObject_CallMethod(
        module, (char *)"_init_cffi_1_0_external_module", (char *)"O", o_arg);

    Py_DECREF(o_arg);
    Py_DECREF(module);
    return new_module;

  failure:
    Py_XDECREF(module);
    return NULL;
}

/**********  end CPython-specific section  **********/
#else
_CFFI_UNUSED_FN
static void (*_cffi_call_python_org)(struct _cffi_externpy_s *, char *);
# define _cffi_call_python  _cffi_call_python_org
#endif


#define _cffi_array_len(array)   (sizeof(array) / sizeof((array)[0]))

#define _cffi_prim_int(size, sign)                                      \
    ((size) == 1 ? ((sign) ? _CFFI_PRIM_INT8  : _CFFI_PRIM_UINT8)  :    \
     (size) == 2 ? ((sign) ? _CFFI_PRIM_INT16 : _CFFI_PRIM_UINT16) :    \
     (size) == 4 ? ((sign) ? _CFFI_PRIM_INT32 : _CFFI_PRIM_UINT32) :    \
     (size) == 8 ? ((sign) ? _CFFI_PRIM_INT64 : _CFFI_PRIM_UINT64) :    \
     _CFFI__UNKNOWN_PRIM)

#define _cffi_prim_float(size)                                          \
    ((size) == sizeof(float) ? _CFFI_PRIM_FLOAT :                       \
     (size) == sizeof(double) ? _CFFI_PRIM_DOUBLE :                     \
     (size) == sizeof(long double) ? _CFFI__UNKNOWN_LONG_DOUBLE :       \
     _CFFI__UNKNOWN_FLOAT_PRIM)

#define _cffi_check_int(got, got_nonpos, expected)      \
    ((got_nonpos) == (expected <= 0) &&                 \
     (got) == (unsigned long long)expected)

#ifdef MS_WIN32
# define _cffi_stdcall  __stdcall
#else
# define _cffi_stdcall  /* nothing */
#endif

#ifdef __cplusplus
}
#endif

/************************************************************/

extern "C" {


    typedef unsigned int UINT;
    typedef int INT;
    typedef int BOOL; // Different from C++
    typedef char CHAR;
    typedef unsigned char BYTE;

    struct Grammar {
        UINT nNonterminals;   // Number of nonterminals
        UINT nTerminals;      // Number of terminals (indexed from 1)
        INT iRoot;            // Index of root nonterminal (negative)
    };

    struct Parser {
        struct Grammar* pGrammar;
    };

    struct Production {
        UINT nId;
        UINT nPriority;
        UINT n;
        INT* pList;
    };

    struct Label {
        INT iNt;
        UINT nDot;
        struct Production* pProd;
        UINT nI;
        UINT nJ;
    };

    struct FamilyEntry {
        struct Production* pProd;
        struct Node* p1;
        struct Node* p2;
        struct FamilyEntry* pNext;
    };

    struct Node {
        struct Label label;
        struct FamilyEntry* pHead;
        UINT nRefCount;
    } Node;

    typedef BOOL (*MatchingFunc)(UINT nHandle, UINT nToken, UINT nTerminal);
    typedef BYTE* (*AllocFunc)(UINT nHandle, UINT nToken, UINT nSize);

    struct Node* earleyParse(struct Parser*, UINT nTokens, INT iRoot, UINT nHandle, UINT* pnErrorToken);
    struct Grammar* newGrammar(const CHAR* pszGrammarFile);
    void deleteGrammar(struct Grammar*);
    struct Parser* newParser(struct Grammar*, MatchingFunc fpMatcher, AllocFunc fpAlloc);
    void deleteParser(struct Parser*);
    void deleteForest(struct Node*);
    void dumpForest(struct Node*, struct Grammar*);
    UINT numCombinations(struct Node*);

    void printAllocationReport(void);


}


/************************************************************/

static void *_cffi_types[] = {
/*  0 */ _CFFI_OP(_CFFI_OP_FUNCTION, 11), // int()(unsigned int, unsigned int, unsigned int)
/*  1 */ _CFFI_OP(_CFFI_OP_PRIMITIVE, 8), // unsigned int
/*  2 */ _CFFI_OP(_CFFI_OP_PRIMITIVE, 8),
/*  3 */ _CFFI_OP(_CFFI_OP_PRIMITIVE, 8),
/*  4 */ _CFFI_OP(_CFFI_OP_FUNCTION_END, 0),
/*  5 */ _CFFI_OP(_CFFI_OP_FUNCTION, 16), // struct Grammar *()(char const *)
/*  6 */ _CFFI_OP(_CFFI_OP_POINTER, 43), // char const *
/*  7 */ _CFFI_OP(_CFFI_OP_FUNCTION_END, 0),
/*  8 */ _CFFI_OP(_CFFI_OP_FUNCTION, 26), // struct Node *()(struct Parser *, unsigned int, int, unsigned int, unsigned int *)
/*  9 */ _CFFI_OP(_CFFI_OP_POINTER, 50), // struct Parser *
/* 10 */ _CFFI_OP(_CFFI_OP_PRIMITIVE, 8),
/* 11 */ _CFFI_OP(_CFFI_OP_PRIMITIVE, 7), // int
/* 12 */ _CFFI_OP(_CFFI_OP_PRIMITIVE, 8),
/* 13 */ _CFFI_OP(_CFFI_OP_POINTER, 1), // unsigned int *
/* 14 */ _CFFI_OP(_CFFI_OP_FUNCTION_END, 0),
/* 15 */ _CFFI_OP(_CFFI_OP_FUNCTION, 9), // struct Parser *()(struct Grammar *, int(*)(unsigned int, unsigned int, unsigned int), unsigned char *(*)(unsigned int, unsigned int, unsigned int))
/* 16 */ _CFFI_OP(_CFFI_OP_POINTER, 47), // struct Grammar *
/* 17 */ _CFFI_OP(_CFFI_OP_POINTER, 0), // int(*)(unsigned int, unsigned int, unsigned int)
/* 18 */ _CFFI_OP(_CFFI_OP_POINTER, 20), // unsigned char *(*)(unsigned int, unsigned int, unsigned int)
/* 19 */ _CFFI_OP(_CFFI_OP_FUNCTION_END, 0),
/* 20 */ _CFFI_OP(_CFFI_OP_FUNCTION, 53), // unsigned char *()(unsigned int, unsigned int, unsigned int)
/* 21 */ _CFFI_OP(_CFFI_OP_PRIMITIVE, 8),
/* 22 */ _CFFI_OP(_CFFI_OP_PRIMITIVE, 8),
/* 23 */ _CFFI_OP(_CFFI_OP_PRIMITIVE, 8),
/* 24 */ _CFFI_OP(_CFFI_OP_FUNCTION_END, 0),
/* 25 */ _CFFI_OP(_CFFI_OP_FUNCTION, 1), // unsigned int()(struct Node *)
/* 26 */ _CFFI_OP(_CFFI_OP_POINTER, 49), // struct Node *
/* 27 */ _CFFI_OP(_CFFI_OP_FUNCTION_END, 0),
/* 28 */ _CFFI_OP(_CFFI_OP_FUNCTION, 55), // void()(struct Grammar *)
/* 29 */ _CFFI_OP(_CFFI_OP_NOOP, 16),
/* 30 */ _CFFI_OP(_CFFI_OP_FUNCTION_END, 0),
/* 31 */ _CFFI_OP(_CFFI_OP_FUNCTION, 55), // void()(struct Node *)
/* 32 */ _CFFI_OP(_CFFI_OP_NOOP, 26),
/* 33 */ _CFFI_OP(_CFFI_OP_FUNCTION_END, 0),
/* 34 */ _CFFI_OP(_CFFI_OP_FUNCTION, 55), // void()(struct Node *, struct Grammar *)
/* 35 */ _CFFI_OP(_CFFI_OP_NOOP, 26),
/* 36 */ _CFFI_OP(_CFFI_OP_NOOP, 16),
/* 37 */ _CFFI_OP(_CFFI_OP_FUNCTION_END, 0),
/* 38 */ _CFFI_OP(_CFFI_OP_FUNCTION, 55), // void()(struct Parser *)
/* 39 */ _CFFI_OP(_CFFI_OP_NOOP, 9),
/* 40 */ _CFFI_OP(_CFFI_OP_FUNCTION_END, 0),
/* 41 */ _CFFI_OP(_CFFI_OP_FUNCTION, 55), // void()(void)
/* 42 */ _CFFI_OP(_CFFI_OP_FUNCTION_END, 0),
/* 43 */ _CFFI_OP(_CFFI_OP_PRIMITIVE, 2), // char
/* 44 */ _CFFI_OP(_CFFI_OP_POINTER, 11), // int *
/* 45 */ _CFFI_OP(_CFFI_OP_POINTER, 46), // struct FamilyEntry *
/* 46 */ _CFFI_OP(_CFFI_OP_STRUCT_UNION, 0), // struct FamilyEntry
/* 47 */ _CFFI_OP(_CFFI_OP_STRUCT_UNION, 1), // struct Grammar
/* 48 */ _CFFI_OP(_CFFI_OP_STRUCT_UNION, 2), // struct Label
/* 49 */ _CFFI_OP(_CFFI_OP_STRUCT_UNION, 3), // struct Node
/* 50 */ _CFFI_OP(_CFFI_OP_STRUCT_UNION, 4), // struct Parser
/* 51 */ _CFFI_OP(_CFFI_OP_POINTER, 52), // struct Production *
/* 52 */ _CFFI_OP(_CFFI_OP_STRUCT_UNION, 5), // struct Production
/* 53 */ _CFFI_OP(_CFFI_OP_POINTER, 54), // unsigned char *
/* 54 */ _CFFI_OP(_CFFI_OP_PRIMITIVE, 4), // unsigned char
/* 55 */ _CFFI_OP(_CFFI_OP_PRIMITIVE, 0), // void
};

static void _cffi_d_deleteForest(struct Node * x0)
{
  deleteForest(x0);
}
#ifndef PYPY_VERSION
static PyObject *
_cffi_f_deleteForest(PyObject *self, PyObject *arg0)
{
  struct Node * x0;
  Py_ssize_t datasize;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(26), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = (struct Node *)alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(26), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { deleteForest(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  Py_INCREF(Py_None);
  return Py_None;
}
#else
#  define _cffi_f_deleteForest _cffi_d_deleteForest
#endif

static void _cffi_d_deleteGrammar(struct Grammar * x0)
{
  deleteGrammar(x0);
}
#ifndef PYPY_VERSION
static PyObject *
_cffi_f_deleteGrammar(PyObject *self, PyObject *arg0)
{
  struct Grammar * x0;
  Py_ssize_t datasize;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(16), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = (struct Grammar *)alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(16), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { deleteGrammar(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  Py_INCREF(Py_None);
  return Py_None;
}
#else
#  define _cffi_f_deleteGrammar _cffi_d_deleteGrammar
#endif

static void _cffi_d_deleteParser(struct Parser * x0)
{
  deleteParser(x0);
}
#ifndef PYPY_VERSION
static PyObject *
_cffi_f_deleteParser(PyObject *self, PyObject *arg0)
{
  struct Parser * x0;
  Py_ssize_t datasize;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(9), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = (struct Parser *)alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(9), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { deleteParser(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  Py_INCREF(Py_None);
  return Py_None;
}
#else
#  define _cffi_f_deleteParser _cffi_d_deleteParser
#endif

static void _cffi_d_dumpForest(struct Node * x0, struct Grammar * x1)
{
  dumpForest(x0, x1);
}
#ifndef PYPY_VERSION
static PyObject *
_cffi_f_dumpForest(PyObject *self, PyObject *args)
{
  struct Node * x0;
  struct Grammar * x1;
  Py_ssize_t datasize;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_UnpackTuple(args, "dumpForest", 2, 2, &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(26), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = (struct Node *)alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(26), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(16), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = (struct Grammar *)alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(16), arg1) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { dumpForest(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  Py_INCREF(Py_None);
  return Py_None;
}
#else
#  define _cffi_f_dumpForest _cffi_d_dumpForest
#endif

static struct Node * _cffi_d_earleyParse(struct Parser * x0, unsigned int x1, int x2, unsigned int x3, unsigned int * x4)
{
  return earleyParse(x0, x1, x2, x3, x4);
}
#ifndef PYPY_VERSION
static PyObject *
_cffi_f_earleyParse(PyObject *self, PyObject *args)
{
  struct Parser * x0;
  unsigned int x1;
  int x2;
  unsigned int x3;
  unsigned int * x4;
  Py_ssize_t datasize;
  struct Node * result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;
  PyObject *arg4;

  if (!PyArg_UnpackTuple(args, "earleyParse", 5, 5, &arg0, &arg1, &arg2, &arg3, &arg4))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(9), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = (struct Parser *)alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(9), arg0) < 0)
      return NULL;
  }

  x1 = _cffi_to_c_int(arg1, unsigned int);
  if (x1 == (unsigned int)-1 && PyErr_Occurred())
    return NULL;

  x2 = _cffi_to_c_int(arg2, int);
  if (x2 == (int)-1 && PyErr_Occurred())
    return NULL;

  x3 = _cffi_to_c_int(arg3, unsigned int);
  if (x3 == (unsigned int)-1 && PyErr_Occurred())
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(13), arg4, (char **)&x4);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x4 = (unsigned int *)alloca((size_t)datasize);
    memset((void *)x4, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x4, _cffi_type(13), arg4) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = earleyParse(x0, x1, x2, x3, x4); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_pointer((char *)result, _cffi_type(26));
}
#else
#  define _cffi_f_earleyParse _cffi_d_earleyParse
#endif

static struct Grammar * _cffi_d_newGrammar(char const * x0)
{
  return newGrammar(x0);
}
#ifndef PYPY_VERSION
static PyObject *
_cffi_f_newGrammar(PyObject *self, PyObject *arg0)
{
  char const * x0;
  Py_ssize_t datasize;
  struct Grammar * result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(6), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = (char const *)alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(6), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = newGrammar(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_pointer((char *)result, _cffi_type(16));
}
#else
#  define _cffi_f_newGrammar _cffi_d_newGrammar
#endif

static struct Parser * _cffi_d_newParser(struct Grammar * x0, int(* x1)(unsigned int, unsigned int, unsigned int), unsigned char *(* x2)(unsigned int, unsigned int, unsigned int))
{
  return newParser(x0, x1, x2);
}
#ifndef PYPY_VERSION
static PyObject *
_cffi_f_newParser(PyObject *self, PyObject *args)
{
  struct Grammar * x0;
  int(* x1)(unsigned int, unsigned int, unsigned int);
  unsigned char *(* x2)(unsigned int, unsigned int, unsigned int);
  Py_ssize_t datasize;
  struct Parser * result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_UnpackTuple(args, "newParser", 3, 3, &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(16), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = (struct Grammar *)alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(16), arg0) < 0)
      return NULL;
  }

  x1 = (int(*)(unsigned int, unsigned int, unsigned int))_cffi_to_c_pointer(arg1, _cffi_type(17));
  if (x1 == (int(*)(unsigned int, unsigned int, unsigned int))NULL && PyErr_Occurred())
    return NULL;

  x2 = (unsigned char *(*)(unsigned int, unsigned int, unsigned int))_cffi_to_c_pointer(arg2, _cffi_type(18));
  if (x2 == (unsigned char *(*)(unsigned int, unsigned int, unsigned int))NULL && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = newParser(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_pointer((char *)result, _cffi_type(9));
}
#else
#  define _cffi_f_newParser _cffi_d_newParser
#endif

static unsigned int _cffi_d_numCombinations(struct Node * x0)
{
  return numCombinations(x0);
}
#ifndef PYPY_VERSION
static PyObject *
_cffi_f_numCombinations(PyObject *self, PyObject *arg0)
{
  struct Node * x0;
  Py_ssize_t datasize;
  unsigned int result;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(26), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = (struct Node *)alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(26), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = numCombinations(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, unsigned int);
}
#else
#  define _cffi_f_numCombinations _cffi_d_numCombinations
#endif

static void _cffi_d_printAllocationReport(void)
{
  printAllocationReport();
}
#ifndef PYPY_VERSION
static PyObject *
_cffi_f_printAllocationReport(PyObject *self, PyObject *noarg)
{

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { printAllocationReport(); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  (void)noarg; /* unused */
  Py_INCREF(Py_None);
  return Py_None;
}
#else
#  define _cffi_f_printAllocationReport _cffi_d_printAllocationReport
#endif

_CFFI_UNUSED_FN
static void _cffi_checkfld_struct_FamilyEntry(struct FamilyEntry *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { struct Production * *tmp = &p->pProd; (void)tmp; }
  { struct Node * *tmp = &p->p1; (void)tmp; }
  { struct Node * *tmp = &p->p2; (void)tmp; }
  { struct FamilyEntry * *tmp = &p->pNext; (void)tmp; }
}
struct _cffi_align_struct_FamilyEntry { char x; struct FamilyEntry y; };

_CFFI_UNUSED_FN
static void _cffi_checkfld_struct_Grammar(struct Grammar *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->nNonterminals) | 0);  /* check that 'struct Grammar.nNonterminals' is an integer */
  (void)((p->nTerminals) | 0);  /* check that 'struct Grammar.nTerminals' is an integer */
  (void)((p->iRoot) | 0);  /* check that 'struct Grammar.iRoot' is an integer */
}
struct _cffi_align_struct_Grammar { char x; struct Grammar y; };

_CFFI_UNUSED_FN
static void _cffi_checkfld_struct_Label(struct Label *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->iNt) | 0);  /* check that 'struct Label.iNt' is an integer */
  (void)((p->nDot) | 0);  /* check that 'struct Label.nDot' is an integer */
  { struct Production * *tmp = &p->pProd; (void)tmp; }
  (void)((p->nI) | 0);  /* check that 'struct Label.nI' is an integer */
  (void)((p->nJ) | 0);  /* check that 'struct Label.nJ' is an integer */
}
struct _cffi_align_struct_Label { char x; struct Label y; };

_CFFI_UNUSED_FN
static void _cffi_checkfld_struct_Node(struct Node *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { struct Label *tmp = &p->label; (void)tmp; }
  { struct FamilyEntry * *tmp = &p->pHead; (void)tmp; }
  (void)((p->nRefCount) | 0);  /* check that 'struct Node.nRefCount' is an integer */
}
struct _cffi_align_struct_Node { char x; struct Node y; };

_CFFI_UNUSED_FN
static void _cffi_checkfld_struct_Parser(struct Parser *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { struct Grammar * *tmp = &p->pGrammar; (void)tmp; }
}
struct _cffi_align_struct_Parser { char x; struct Parser y; };

_CFFI_UNUSED_FN
static void _cffi_checkfld_struct_Production(struct Production *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->nId) | 0);  /* check that 'struct Production.nId' is an integer */
  (void)((p->nPriority) | 0);  /* check that 'struct Production.nPriority' is an integer */
  (void)((p->n) | 0);  /* check that 'struct Production.n' is an integer */
  { int * *tmp = &p->pList; (void)tmp; }
}
struct _cffi_align_struct_Production { char x; struct Production y; };

static struct Node *_cffi_var_Node(void)
{
  return &(Node);
}

static const struct _cffi_global_s _cffi_globals[] = {
  { "Node", (void *)_cffi_var_Node, _CFFI_OP(_CFFI_OP_GLOBAL_VAR_F, 49), (void *)0 },
  { "deleteForest", (void *)_cffi_f_deleteForest, _CFFI_OP(_CFFI_OP_CPYTHON_BLTN_O, 31), (void *)_cffi_d_deleteForest },
  { "deleteGrammar", (void *)_cffi_f_deleteGrammar, _CFFI_OP(_CFFI_OP_CPYTHON_BLTN_O, 28), (void *)_cffi_d_deleteGrammar },
  { "deleteParser", (void *)_cffi_f_deleteParser, _CFFI_OP(_CFFI_OP_CPYTHON_BLTN_O, 38), (void *)_cffi_d_deleteParser },
  { "dumpForest", (void *)_cffi_f_dumpForest, _CFFI_OP(_CFFI_OP_CPYTHON_BLTN_V, 34), (void *)_cffi_d_dumpForest },
  { "earleyParse", (void *)_cffi_f_earleyParse, _CFFI_OP(_CFFI_OP_CPYTHON_BLTN_V, 8), (void *)_cffi_d_earleyParse },
  { "newGrammar", (void *)_cffi_f_newGrammar, _CFFI_OP(_CFFI_OP_CPYTHON_BLTN_O, 5), (void *)_cffi_d_newGrammar },
  { "newParser", (void *)_cffi_f_newParser, _CFFI_OP(_CFFI_OP_CPYTHON_BLTN_V, 15), (void *)_cffi_d_newParser },
  { "numCombinations", (void *)_cffi_f_numCombinations, _CFFI_OP(_CFFI_OP_CPYTHON_BLTN_O, 25), (void *)_cffi_d_numCombinations },
  { "printAllocationReport", (void *)_cffi_f_printAllocationReport, _CFFI_OP(_CFFI_OP_CPYTHON_BLTN_N, 41), (void *)_cffi_d_printAllocationReport },
};

static const struct _cffi_field_s _cffi_fields[] = {
  { "pProd", offsetof(struct FamilyEntry, pProd),
             sizeof(((struct FamilyEntry *)0)->pProd),
             _CFFI_OP(_CFFI_OP_NOOP, 51) },
  { "p1", offsetof(struct FamilyEntry, p1),
          sizeof(((struct FamilyEntry *)0)->p1),
          _CFFI_OP(_CFFI_OP_NOOP, 26) },
  { "p2", offsetof(struct FamilyEntry, p2),
          sizeof(((struct FamilyEntry *)0)->p2),
          _CFFI_OP(_CFFI_OP_NOOP, 26) },
  { "pNext", offsetof(struct FamilyEntry, pNext),
             sizeof(((struct FamilyEntry *)0)->pNext),
             _CFFI_OP(_CFFI_OP_NOOP, 45) },
  { "nNonterminals", offsetof(struct Grammar, nNonterminals),
                     sizeof(((struct Grammar *)0)->nNonterminals),
                     _CFFI_OP(_CFFI_OP_NOOP, 1) },
  { "nTerminals", offsetof(struct Grammar, nTerminals),
                  sizeof(((struct Grammar *)0)->nTerminals),
                  _CFFI_OP(_CFFI_OP_NOOP, 1) },
  { "iRoot", offsetof(struct Grammar, iRoot),
             sizeof(((struct Grammar *)0)->iRoot),
             _CFFI_OP(_CFFI_OP_NOOP, 11) },
  { "iNt", offsetof(struct Label, iNt),
           sizeof(((struct Label *)0)->iNt),
           _CFFI_OP(_CFFI_OP_NOOP, 11) },
  { "nDot", offsetof(struct Label, nDot),
            sizeof(((struct Label *)0)->nDot),
            _CFFI_OP(_CFFI_OP_NOOP, 1) },
  { "pProd", offsetof(struct Label, pProd),
             sizeof(((struct Label *)0)->pProd),
             _CFFI_OP(_CFFI_OP_NOOP, 51) },
  { "nI", offsetof(struct Label, nI),
          sizeof(((struct Label *)0)->nI),
          _CFFI_OP(_CFFI_OP_NOOP, 1) },
  { "nJ", offsetof(struct Label, nJ),
          sizeof(((struct Label *)0)->nJ),
          _CFFI_OP(_CFFI_OP_NOOP, 1) },
  { "label", offsetof(struct Node, label),
             sizeof(((struct Node *)0)->label),
             _CFFI_OP(_CFFI_OP_NOOP, 48) },
  { "pHead", offsetof(struct Node, pHead),
             sizeof(((struct Node *)0)->pHead),
             _CFFI_OP(_CFFI_OP_NOOP, 45) },
  { "nRefCount", offsetof(struct Node, nRefCount),
                 sizeof(((struct Node *)0)->nRefCount),
                 _CFFI_OP(_CFFI_OP_NOOP, 1) },
  { "pGrammar", offsetof(struct Parser, pGrammar),
                sizeof(((struct Parser *)0)->pGrammar),
                _CFFI_OP(_CFFI_OP_NOOP, 16) },
  { "nId", offsetof(struct Production, nId),
           sizeof(((struct Production *)0)->nId),
           _CFFI_OP(_CFFI_OP_NOOP, 1) },
  { "nPriority", offsetof(struct Production, nPriority),
                 sizeof(((struct Production *)0)->nPriority),
                 _CFFI_OP(_CFFI_OP_NOOP, 1) },
  { "n", offsetof(struct Production, n),
         sizeof(((struct Production *)0)->n),
         _CFFI_OP(_CFFI_OP_NOOP, 1) },
  { "pList", offsetof(struct Production, pList),
             sizeof(((struct Production *)0)->pList),
             _CFFI_OP(_CFFI_OP_NOOP, 44) },
};

static const struct _cffi_struct_union_s _cffi_struct_unions[] = {
  { "FamilyEntry", 46, _CFFI_F_CHECK_FIELDS,
    sizeof(struct FamilyEntry), offsetof(struct _cffi_align_struct_FamilyEntry, y), 0, 4 },
  { "Grammar", 47, _CFFI_F_CHECK_FIELDS,
    sizeof(struct Grammar), offsetof(struct _cffi_align_struct_Grammar, y), 4, 3 },
  { "Label", 48, _CFFI_F_CHECK_FIELDS,
    sizeof(struct Label), offsetof(struct _cffi_align_struct_Label, y), 7, 5 },
  { "Node", 49, _CFFI_F_CHECK_FIELDS,
    sizeof(struct Node), offsetof(struct _cffi_align_struct_Node, y), 12, 3 },
  { "Parser", 50, _CFFI_F_CHECK_FIELDS,
    sizeof(struct Parser), offsetof(struct _cffi_align_struct_Parser, y), 15, 1 },
  { "Production", 52, _CFFI_F_CHECK_FIELDS,
    sizeof(struct Production), offsetof(struct _cffi_align_struct_Production, y), 16, 4 },
};

static const struct _cffi_typename_s _cffi_typenames[] = {
  { "AllocFunc", 18 },
  { "BOOL", 11 },
  { "BYTE", 54 },
  { "CHAR", 43 },
  { "INT", 11 },
  { "MatchingFunc", 17 },
  { "UINT", 1 },
};

static const struct _cffi_type_context_s _cffi_type_context = {
  _cffi_types,
  _cffi_globals,
  _cffi_fields,
  _cffi_struct_unions,
  NULL,  /* no enums */
  _cffi_typenames,
  10,  /* num_globals */
  6,  /* num_struct_unions */
  0,  /* num_enums */
  7,  /* num_typenames */
  NULL,  /* no includes */
  56,  /* num_types */
  0,  /* flags */
};

#ifdef PYPY_VERSION
PyMODINIT_FUNC
_cffi_pypyinit__eparser(const void *p[])
{
    p[0] = (const void *)0x2601;
    p[1] = &_cffi_type_context;
}
#  ifdef _MSC_VER
     PyMODINIT_FUNC
#  if PY_MAJOR_VERSION >= 3
     PyInit__eparser(void) { return NULL; }
#  else
     init_eparser(void) { }
#  endif
#  endif
#elif PY_MAJOR_VERSION >= 3
PyMODINIT_FUNC
PyInit__eparser(void)
{
  return _cffi_init("reynir._eparser", 0x2601, &_cffi_type_context);
}
#else
PyMODINIT_FUNC
init_eparser(void)
{
  _cffi_init("reynir._eparser", 0x2601, &_cffi_type_context);
}
#endif