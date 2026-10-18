#include "input_io.h"
struct sa_cache_operation_io {struct sa_input_io registers;int (*operation)(void *,uint32_t);};
