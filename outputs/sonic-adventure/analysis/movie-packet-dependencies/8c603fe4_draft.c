/* Ghidra draft only; not buildable/verified source. */

int FUN_8c603fe4(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = *(int *)(param_1 + 0x24);
  if (iVar1 == 0) {
    iVar1 = FUN_8c603ec4(param_1);
    if (iVar1 != -10) {
      return iVar1;
    }
    uVar2 = 1;
  }
  else {
    if (iVar1 != 1) {
      if (iVar1 != 2) {
        return -2;
      }
      iVar1 = FUN_8c603e08(param_1);
      return iVar1;
    }
    iVar1 = (*(code *)PTR_FUN_8c60410c)(*(undefined4 *)(param_1 + 4));
    if (iVar1 == -1) {
      return -3;
    }
    uVar2 = 2;
  }
  *(undefined4 *)(param_1 + 0x24) = uVar2;
  return 1;
}

