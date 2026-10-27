/* Ghidra draft only; not buildable/verified source. */

void FUN_8c64b18e(int param_1,int param_2)

{
  int iVar1;
  
  iVar1 = param_1 * DAT_8c64b1e0 + DAT_8c64b1e8;
  if (param_2 <= **(int **)(iVar1 + 0x1c)) {
                    /* WARNING: Could not recover jumptable at 0x8c64b1b6. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(*DAT_8c64b1ec + 0x10))(*(undefined4 *)(iVar1 + 0x24),param_2 << 1);
    return;
  }
  return;
}

