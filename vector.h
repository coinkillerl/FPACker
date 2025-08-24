#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include <string.h>

#define VECTOR_MIN_ITEMS 32

#define __vector(T) vector_##T
#define vector(T) __vector(T) // a layer of indirection is required to be able to pass another macro as a type parameter

#define __VECTOR_DEFINE(T) \
	typedef struct vector(T) {\
		T *items;\
		size_t len;\
		size_t cap;\
	} vector(T);\
	static void vector_##T##_clear (vector(T) *v);\
	static int vector_##T##_erase (vector(T) *v, const size_t pos);\
	static int vector_##T##_insert (vector(T) *v, const size_t pos, const T value);\
	static int vector_##T##_pop_back (vector(T) *v);\
	static int vector_##T##_push_back (vector(T) *v, const T value);\
	static void vector_##T##_clear (vector(T) *v) {\
		v->len = 0;\
		v->cap = 0;\
		free(v->items);\
		v->items = NULL;\
	}\
	static int vector_##T##_erase (vector(T) *v, const size_t pos) {\
		if (!v->len || pos > v->len - 1) return -1;\
		(void)memmove(&v->items[pos], &v->items[pos + 1], v->len - (pos + 1));\
		if (v->len <= v->cap / 4 && v->cap > VECTOR_MIN_ITEMS) {\
			v->cap /= 4;\
			v->items = realloc(v->items, v->cap * sizeof(T));\
		}\
		if (!v->items) return -1;\
		v->len--;\
		return 0;\
	}\
	static int vector_##T##_insert (vector(T) *v, const size_t pos, const T value) {\
		if (pos > v->len) return -1;\
		if (v->len >= v->cap || !v->items) {\
			if (v->items)\
				v->cap = v->cap * 2 > VECTOR_MIN_ITEMS ? v->cap * 2 : VECTOR_MIN_ITEMS;\
			else\
				v->cap = v->cap > VECTOR_MIN_ITEMS ? v->cap : VECTOR_MIN_ITEMS;\
			v->items = realloc(v->items, v->cap * sizeof(T));\
		}\
		if (!v->items) return -1;\
		(void)memmove(&v->items[pos + 1], &v->items[pos], v->len - pos);\
		v->items[pos] = value;\
		v->len++;\
		return 0;\
	}\
	static int vector_##T##_pop_back (vector(T) *v) {\
		if (!v->len) return 0;\
		if (v->len <= v->cap / 4 && v->cap > VECTOR_MIN_ITEMS) {\
			v->cap /= 4;\
			v->items = realloc(v->items, v->cap * sizeof(T));\
		}\
		if (!v->items) return -1;\
		v->len--;\
		return 0;\
	}\
	static int vector_##T##_push_back (vector(T) *v, const T value) {\
		if (v->len >= v->cap || !v->items) {\
			if (v->items)\
				v->cap = v->cap * 2 > VECTOR_MIN_ITEMS ? v->cap * 2 : VECTOR_MIN_ITEMS;\
			else\
				v->cap = v->cap > VECTOR_MIN_ITEMS ? v->cap : VECTOR_MIN_ITEMS;\
			v->items = realloc(v->items, v->cap * sizeof(T));\
		}\
		if (!v->items) return -1;\
		v->items[v->len] = value;\
		v->len++;\
		return 0;\
	}
#define VECTOR_DEFINE(T) __VECTOR_DEFINE(T) // a layer of indirection is required to be able to pass another macro as a type parameter

#define vector_clear(T, v) vector_##T##_clear(v)
#define vector_erase(T, v, pos) vector_##T##_erase(v, pos)
#define vector_insert(T, v, pos, value) vector_##T##_insert(v, pos, value)
#define vector_pop_back(T, v) vector_##T##_pop_back(v)
#define vector_push_back(T, v, value) vector_##T##_push_back(v, value)

#endif // VECTOR_H
