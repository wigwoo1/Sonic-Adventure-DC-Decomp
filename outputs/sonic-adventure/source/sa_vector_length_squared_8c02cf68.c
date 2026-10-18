#include "fpu_cleanup_transfer.h"

#include <stdint.h>

#ifdef _WIN32
#define SA_EXPORT __declspec(dllexport)
#else
#define SA_EXPORT
#endif

struct sa_cpu_state {
    uint32_t r[16];
    uint32_t sr;
    uint32_t pr;
    uint32_t pc;
};

typedef int (*sa_fipr_instruction_service)(void *, uint32_t,
                                           struct sa_fpu_banks *);

/* 8c02cf68..8c02cf70: zero FR7, execute FIPR FV4,FV4, and return its
 * result in FR0 in the RTS delay slot. FIPR's numeric operation is supplied
 * by the SH-4 instruction service, shared with the native runtime. */
SA_EXPORT int sa_vector_length_squared_8c02cf68(
    struct sa_cpu_state *cpu,
    struct sa_fpu_banks *fpu,
    void *context,
    sa_fipr_instruction_service execute_fipr) {
    if (cpu == 0 || fpu == 0 || execute_fipr == 0) {
        return -1;
    }

    uint32_t active_bank = (fpu->fpscr >> 21) & 1u;
    fpu->bank[active_bank][7] = 0;

    int status = execute_fipr(context, 0xf5edu, fpu);
    if (status != 0) {
        return status;
    }

    /* RTS delay slot: FMOV FR7,FR0. With SZ=1, the encoded register pair
     * uses the alternate bank and copies the DR6 pair into DR0. */
    if ((fpu->fpscr & 0x00100000u) != 0) {
        uint32_t source_bank = active_bank ^ 1u;
        fpu->bank[active_bank][0] = fpu->bank[source_bank][6];
        fpu->bank[active_bank][1] = fpu->bank[source_bank][7];
    } else {
        fpu->bank[active_bank][0] = fpu->bank[active_bank][7];
    }
    cpu->pc = cpu->pr;
    return 0;
}
