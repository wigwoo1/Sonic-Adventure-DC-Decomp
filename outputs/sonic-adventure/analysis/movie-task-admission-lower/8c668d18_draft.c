/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void callback_8c668d18(int param_1)

{
  int iVar1;
  int iVar2;
  
  iVar2 = _DAT_8c668d74 + param_1;
  for (iVar1 = 0; iVar1 < *(int *)(_DAT_8c668d76 + param_1); iVar1 = iVar1 + 1) {
    if (*(int *)(iVar2 + 0xc) != 0) {
      FUN_8c668cca(param_1,iVar2);
    }
    iVar2 = iVar2 + DAT_8c668d66;
  }
                    /* WARNING: Could not recover jumptable at 0x8c668d52. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*_DAT_8c668d88)(param_1,0,(int)_DAT_8c668d72);
  return;
}

