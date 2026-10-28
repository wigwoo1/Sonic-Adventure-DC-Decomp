/* Ghidra draft only; not buildable/verified source. */

int FUN_8c648b40(int param_1,int param_2)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  
  iVar5 = (int)DAT_8c648bce;
  iVar6 = param_1 * iVar5 + DAT_8c648bd8;
  if (*(char *)(iVar6 + 2) != '\0') {
    iVar2 = (*(code *)PTR_FUN_8c648bdc)(param_1);
    if (iVar2 == 0) {
      FUN_8c64850a(param_1);
    }
    else {
      iVar2 = (*(code *)PTR_FUN_8c648be0)(param_1);
      if ((iVar2 != 0) && (*(char *)(param_1 * iVar5 + DAT_8c648bd8 + 8) != '\0')) {
        iVar5 = (*(code *)PTR_thunk_FUN_8c64a4b2_8c648be4)(param_1);
        uVar1 = DAT_8c648be8;
        if (iVar5 < 0) {
          return (int)DAT_8c648bd2;
        }
        iVar5 = 0;
        if (param_2 == 1) {
          iVar5 = 0;
          while( true ) {
            if (*(int *)(*(int *)(iVar6 + 0x1c) + 0x38) <= iVar5) {
              return iVar5;
            }
            uVar3 = (*(code *)PTR_FUN_8c648bec)(param_1,iVar5);
            if ((uVar3 & 0xffff) != uVar1) break;
            iVar5 = iVar5 + 1;
          }
          return iVar5;
        }
        if (*(int *)PTR_DAT_8c648cc8 != 0) {
          for (iVar2 = 0; iVar2 < *(int *)(*(int *)(iVar6 + 0x1c) + 0x30); iVar2 = iVar2 + 1) {
            uVar3 = (*(code *)PTR_FUN_8c648ccc)(param_1,iVar2);
            if (((uVar3 & 0xffff) == uVar1) &&
               (iVar4 = (*(code *)PTR_FUN_8c648cd0)(param_1,iVar2), -1 < iVar4)) {
              iVar5 = iVar5 + 1;
            }
          }
          return iVar5;
        }
        iVar2 = 0;
        while (iVar2 < *(int *)(*(int *)(iVar6 + 0x1c) + 0x30)) {
          uVar3 = (*(code *)PTR_FUN_8c648ccc)(param_1,iVar2);
          iVar2 = iVar2 + 1;
          if ((uVar3 & 0xffff) == uVar1) {
            iVar5 = iVar5 + 1;
          }
        }
        return iVar5;
      }
    }
  }
  return (int)DAT_8c648bd0;
}

