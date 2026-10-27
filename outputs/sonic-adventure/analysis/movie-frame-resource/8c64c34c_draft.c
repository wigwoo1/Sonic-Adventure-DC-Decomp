/* Ghidra draft only; not buildable/verified source. */

void FUN_8c64c34c(int *param_1,int param_2)

{
                    /* WARNING: Could not recover jumptable at 0x8c64c370. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(PTR_DAT_8c64c488 +
              (uint)((*(uint *)(param_2 * 0xc + *param_1 + 4) & DAT_8c64c484) != 0) * 4))();
  return;
}

