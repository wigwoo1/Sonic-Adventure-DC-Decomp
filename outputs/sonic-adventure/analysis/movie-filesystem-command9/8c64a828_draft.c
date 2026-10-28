/* Ghidra draft only; not buildable/verified source. */

int FUN_8c64a828(int param_1,undefined4 param_2)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  undefined1 *puVar4;
  int iVar5;
  int iVar6;
  undefined1 auStack_2c [20];
  
  iVar5 = (int)DAT_8c64a862;
  iVar6 = param_1 * iVar5 + DAT_8c64a864;
  if (*(char *)(iVar6 + 2) != '\0') {
    iVar1 = (*(code *)PTR_FUN_8c64a874)(param_1);
    if (iVar1 == 0) {
      (*(code *)PTR_FUN_8c64a878)(param_1);
    }
    else {
      iVar1 = (*(code *)PTR_FUN_8c64a950)(param_1);
      if ((iVar1 != 0) && (*(char *)(param_1 * iVar5 + DAT_8c64a954 + 8) != '\0')) {
        iVar5 = (*(code *)PTR_thunk_FUN_8c64a4b2_8c64a958)(param_1);
        if (iVar5 < 0) {
          return (int)DAT_8c64a94c;
        }
        if (((*(int *)(*(int *)(iVar6 + 0x1c) + 0x38) != 0) &&
            (iVar5 = FUN_8c64a3fa(param_1,auStack_2c), iVar5 != 0)) &&
           (iVar5 = FUN_8c64a7ba(param_1), -1 < iVar5)) {
          while( true ) {
            iVar5 = FUN_8c64a6ae(param_1);
            if ((iVar5 < 0) || (iVar1 = FUN_8c64a6fc(param_1,iVar5 + 1), iVar1 < 0)) {
              iVar5 = (int)DAT_8c64aa7e;
              for (puVar4 = (undefined1 *)(iVar6 + 0x48);
                  puVar4 < (undefined1 *)(iVar6 + 0x48 + iVar5); puVar4 = puVar4 + 1) {
                *puVar4 = 1;
              }
              iVar5 = FUN_8c64ab36(param_1);
              if (iVar5 < 0) {
                return iVar5;
              }
              iVar5 = FUN_8c64afe0(param_1);
              return iVar5;
            }
            iVar2 = (*(code *)PTR_FUN_8c64a95c)(param_1,iVar5,param_2);
            if (iVar2 < 0) {
              return iVar2;
            }
            iVar2 = (*(code *)PTR_FUN_8c64a960)(param_1,iVar1,param_2);
            if (iVar2 < 0) break;
            iVar2 = FUN_8c64b080(param_1,iVar5);
            if (iVar2 < 1) {
              uVar3 = FUN_8c64a74e(param_1,iVar5);
              (**(code **)(*DAT_8c64a964 + 0x10))(uVar3,2,iVar1);
            }
            else {
              FUN_8c64b18e(param_1,iVar1);
            }
            uVar3 = FUN_8c64b1c0(param_1,iVar5);
            FUN_8c64b18e(param_1,iVar1,uVar3);
            FUN_8c64b18e(param_1,iVar5,DAT_8c64a968);
          }
          return iVar2;
        }
        return (int)DAT_8c64a94e;
      }
    }
  }
  return (int)DAT_8c64a94a;
}

