/* Ghidra draft only; not buildable/verified source. */

void FUN_8c6209a8(int param_1)

{
  int iVar1;
  undefined1 *puVar2;
  undefined4 in_PR;
  char in_FPSCR_SZ;
  undefined1 local_20 [4];
  undefined1 local_1c [4];
  
  if (in_FPSCR_SZ == '\0') {
    puVar2 = local_1c;
  }
  else {
    puVar2 = local_20;
  }
  *(undefined4 *)(puVar2 + -4) = in_PR;
  if (*DAT_8c620a20 < *(int *)(param_1 + 8)) {
                    /* WARNING: Could not recover jumptable at 0x8c6209ca. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*DAT_8c620a24)();
    return;
  }
  iVar1 = (*(code *)PTR_FUN_8c620a28)(param_1);
  if (iVar1 != 0) {
                    /* WARNING: Could not recover jumptable at 0x8c6209dc. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*DAT_8c620a24)();
    return;
  }
                    /* WARNING: Could not recover jumptable at 0x8c6209e4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  *(undefined4 *)(puVar2 + -0x10) = 0;
  (*DAT_8c620a2c)();
  return;
}

