/* Explicit context for the original exception handlers returning to SVC/ARM.
 * Caller supplies already-entered exception registers, saved CPSR and inactive
 * SVC bank. No automatic interrupt injection, nesting or device timing model. */
struct sa_arm_exception_banks {
 uint32_t spsr;
 uint32_t svc_r8_r14[7];
 uint32_t exception_r8_r14[7];
 uint32_t returned;
};
