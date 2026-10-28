/* Ghidra draft only; not buildable/verified source. */

int FUN_8c64a244(int param_1,undefined4 param_2,short param_3,undefined1 param_4)

{
  undefined *puVar1;
  int iVar2;
  int iVar3;
  undefined1 auStack_3c [48];
  
  puVar1 = PTR_DAT_8c64a338;
  iVar2 = (int)DAT_8c64a32a;
  iVar3 = param_1 * iVar2 + DAT_8c64a334;
  *(undefined4 *)(iVar3 + 0x30) = 1;
  *(undefined4 *)(iVar3 + 0x34) = *(undefined4 *)(puVar1 + *(int *)(iVar3 + 4) * 0x50);
  if (*(char *)(iVar3 + 2) != '\0') {
    iVar3 = (*(code *)PTR_FUN_8c64a33c)(param_1);
    if (iVar3 == 0) {
      (*(code *)PTR_FUN_8c64a340)(param_1);
    }
    else {
      iVar3 = (*(code *)PTR_FUN_8c64a344)(param_1);
      if ((iVar3 != 0) && (*(char *)(param_1 * iVar2 + DAT_8c64a334 + 8) != '\0')) {
        iVar2 = (*(code *)PTR_thunk_FUN_8c64a4b2_8c64a348)(param_1);
        if (iVar2 < 0) {
          return (int)DAT_8c64a32e;
        }
        iVar2 = (*(code *)PTR_FUN_8c64a34c)(param_1,param_2);
        if (iVar2 < 0) {
          return (int)DAT_8c64a330;
        }
        iVar2 = FUN_8c64ad56(param_1,param_2,auStack_3c);
        if (iVar2 < 0) {
          return (int)DAT_8c64a332;
        }
        iVar3 = (*(code *)PTR_FUN_8c64a350)(param_1,iVar2);
        if (*(int *)PTR_DAT_8c64a354 != 0) {
          (**(code **)(*DAT_8c64a358 + 0x10))(iVar3,0x1a,(int)param_3);
        }
        puVar1 = PTR_FUN_8c64a35c;
        *(undefined1 *)(iVar3 + 1) = param_4;
        (*(code *)puVar1)(param_1,iVar2);
        iVar2 = FUN_8c64ab36(param_1);
        return iVar2;
      }
    }
  }
  return (int)DAT_8c64a32c;
}

