/* Ghidra draft only; not buildable/verified source. */

void callback_8c60d180(uint *param_1)

{
  if ((param_1[1] != 0) && ((*param_1 & 8) == 0)) {
                    /* WARNING: Could not recover jumptable at 0x8c60d196. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)*DAT_8c60d260)(param_1[1]);
    return;
  }
  return;
}

