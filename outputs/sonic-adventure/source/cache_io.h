#ifndef SA_CACHE_IO_H
#define SA_CACHE_IO_H
#include "input_io.h"
struct sa_cache_io {struct sa_input_io registers;int (*writeback)(void *,uint32_t);};
#endif
