#ifndef SIMPLE_ARRAY_H
#define SIMPLE_ARRAY_H

#ifdef __cplusplus
extern "C"{
#endif

#include <stdlib.h>

struct simple_array{
	void          **data;
	unsigned long   size;
	unsigned long   capacity;
};
typedef struct simple_array simple_array_t;

inline void simple_array_add(simple_array_t* arr, 
		void *value, 
		unsigned long append_len)
{
	if (!arr) return;
	if (arr->size + 1 > arr->capacity)
	{
		arr->capacity += append_len;
		arr->data = (void **)realloc(arr->data, sizeof(value) * arr->capacity);
	}
	arr->data[arr->size] = value;
	arr->size++;
}

#ifdef __cplusplus
}
#endif

#endif
