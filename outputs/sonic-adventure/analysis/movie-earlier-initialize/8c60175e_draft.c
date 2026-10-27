/* Ghidra draft only; not buildable/verified source. */

void FUN_8c60175e(void)

{
  if (*(int *)PTR_DAT_8c601818 != 0) {
                    /* WARNING: Could not recover jumptable at 0x8c60176c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(*(int *)PTR_DAT_8c601818 + 0x74))();
    return;
  }
  return;
}

