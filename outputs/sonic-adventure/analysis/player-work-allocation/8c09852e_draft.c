/* Ghidra draft only; not buildable/verified source. */

int FUN_8c09852e(ushort param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  int iVar2;
  
  iVar1 = FUN_8c0985ec(param_2,param_3,param_3,0);
  if (iVar1 != 0) {
    if ((param_1 & 2) != 0) {
      iVar2 = FUN_8c098b0c(1,0x40);
      if (iVar2 == 0) {
        FUN_8c09865c(iVar1);
        return 0;
      }
      *(int *)(iVar1 + 0x20) = iVar2;
    }
    if ((param_1 & 4) != 0) {
      iVar2 = FUN_8c098b0c(2,0x1c);
      if (iVar2 == 0) {
        FUN_8c09865c(iVar1);
        return 0;
      }
      *(int *)(iVar1 + 0x28) = iVar2;
    }
    if ((param_1 & 1) != 0) {
      iVar2 = FUN_8c098b0c(1,0x40);
      if (iVar2 == 0) {
        FUN_8c09865c(iVar1);
        return 0;
      }
      *(int *)(iVar1 + 0x24) = iVar2;
    }
    if ((param_1 & 8) == 0) {
      return iVar1;
    }
    iVar2 = FUN_8c098b0c(1,0x10);
    if (iVar2 != 0) {
      *(int *)(iVar1 + 0x2c) = iVar2;
      return iVar1;
    }
    FUN_8c09865c(iVar1);
  }
  return 0;
}

