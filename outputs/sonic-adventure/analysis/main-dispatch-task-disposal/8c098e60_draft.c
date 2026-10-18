/* Ghidra draft only; not buildable/verified source. */

void FUN_8c098e60(int param_1)

{
  int iVar1;
  
  if (*(undefined **)(param_1 + 0x10) == PTR_FUN_8c098ec0) {
    FUN_8c098e98(param_1);
    if (*(int *)(param_1 + 0xc) == 0) {
      *(undefined **)(param_1 + 0x10) = PTR_FUN_8c098ec4;
      return;
    }
  }
  else {
    iVar1 = *(int *)(param_1 + 0xc);
    while (iVar1 != 0) {
      FUN_8c09865c(*(undefined4 *)(param_1 + 0xc));
      iVar1 = *(int *)(param_1 + 0xc);
    }
  }
  return;
}

