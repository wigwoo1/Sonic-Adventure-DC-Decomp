/* Ghidra draft only; not buildable/verified source. */

int callback_8c6004d4(int param_1)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  int iVar4;
  
  if (param_1 == 0) {
    param_1 = *(int *)(*(int *)PTR_DAT_8c60059c + 0x10);
  }
  iVar1 = (*(code *)PTR_FUN_8c6005a0)(param_1,0);
  if (iVar1 == 0) {
    FUN_8c5ffa08(0xfffffff8);
  }
  else {
    iVar4 = (*(int **)PTR_DAT_8c60059c)[6];
    for (iVar2 = 0; iVar2 < **(int **)PTR_DAT_8c60059c; iVar2 = iVar2 + 1) {
      if (*(short *)(iVar4 + 0x4c) == 0) {
        piVar3 = (int *)(param_1 + 0x10 + (iVar1 + -1) * 0x2c);
        FUN_8c600464(iVar4,*piVar3 + (int)DAT_8c600596,piVar3[1]);
        FUN_8c5ffa08(0);
        return iVar4;
      }
      iVar4 = iVar4 + 0x54;
    }
    FUN_8c5ffa08(0xfffffff7);
  }
  return 0;
}

