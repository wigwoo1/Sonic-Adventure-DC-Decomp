#ifndef SA_INPUT_IO_H
#define SA_INPUT_IO_H
#include <stdint.h>
struct sa_input_io {void *user;uint32_t (*read)(void *,uint32_t,unsigned);void (*write)(void *,uint32_t,uint32_t,unsigned);};
#endif
