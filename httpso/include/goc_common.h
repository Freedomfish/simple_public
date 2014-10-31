#ifndef GOC_COMMON_H
#define GOC_COMMON_H

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>

struct goc_string {
	size_t length;
	char*  data;
};

typedef struct goc_string goc_string_t;

#ifdef __cplusplus
}
#endif

#endif
