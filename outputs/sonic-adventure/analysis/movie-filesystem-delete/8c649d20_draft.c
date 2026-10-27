/* Ghidra draft only; not buildable/verified source. */

int FUN_8c649d20(int param_1,undefined4 param_2)

{
  undefined *puVar1;
  int iVar2;
  int iVar3;
  
  puVar1 = PTR_DAT_8c649de4;
  iVar2 = (int)DAT_8c649dd8;
  iVar3 = param_1 * iVar2 + DAT_8c649de0;
  *(undefined4 *)(iVar3 + 0x30) = 1;
  *(undefined4 *)(iVar3 + 0x34) = *(undefined4 *)(puVar1 + *(int *)(iVar3 + 4) * 0x50);
  if (*(char *)(iVar3 + 2) != '\0') {
    iVar3 = (*(code *)PTR_FUN_8c649de8)(param_1);
    if (iVar3 == 0) {
      (*(code *)PTR_FUN_8c649dec)(param_1);
    }
    else {
      iVar3 = (*(code *)PTR_FUN_8c649df0)(param_1);
      if ((iVar3 != 0) && (*(char *)(param_1 * iVar2 + DAT_8c649de0 + 8) != '\0')) {
        iVar2 = (*(code *)PTR_thunk_FUN_8c64a4b2_8c649df4)(param_1);
        if (iVar2 < 0) {
          return (int)DAT_8c649ddc;
        }
        iVar2 = (*(code *)PTR_FUN_8c649df8)(param_1,param_2);
        if (iVar2 < 0) {
          return (int)DAT_8c649dde;
        }
        iVar2 = FUN_8c649c68(param_1,param_2);
        if (iVar2 < 0) {
          return iVar2;
        }
        iVar2 = FUN_8c64ab36(param_1);
        if (iVar2 < 0) {
          return iVar2;
        }
        iVar2 = (*(code *)PTR_FUN_8c649dfc)(param_1);
        return iVar2;
      }
    }
  }
  return (int)DAT_8c649dda;
}

