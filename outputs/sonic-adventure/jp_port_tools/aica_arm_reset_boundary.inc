/* Authored native reset adaptation within the external ARM unit's scope.
 * ARM reset enters Supervisor mode with IRQ/FIQ disabled. The pinned core's
 * arm_clear_state selects the mode but leaves both interrupt masks clear. */
void jp_aica_arm_reset(void *state)
{
    arm_clear_state(state);
    ARMSTATE->cpsr|=PSR_IRQMASK|PSR_FIQMASK;
}
