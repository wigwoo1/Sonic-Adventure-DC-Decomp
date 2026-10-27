/* Ghidra draft only; not buildable/verified source. */

void FUN_8c647bb8(int param_1)

{
  int iVar1;
  
  iVar1 = param_1 * 0x80 + DAT_8c647c4c;
  if (*(int *)(iVar1 + 0x74) != 0) {
                    /* WARNING: Could not recover jumptable at 0x8c647bd6. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(iVar1 + 0x74))(*(undefined4 *)(iVar1 + 0x7c));
    return;
  }
  return;
}

