#ifndef SA_INTERRUPT_IO_H
#define SA_INTERRUPT_IO_H
#include <stdint.h>
struct sa_interrupt_io {
    uint32_t vbr;
    int status;
    void *context;
    int (*read32)(void *, uint32_t, uint32_t *);
    int (*write32)(void *, uint32_t, uint32_t);
};
#endif
