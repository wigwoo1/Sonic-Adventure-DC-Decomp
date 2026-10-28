/* Ghidra draft only; not buildable/verified source. */

int FUN_8c649e00(int param_1,undefined4 *param_2,int param_3)

{
  bool bVar1;
  undefined *puVar2;
  int iVar3;
  int iVar4;
  undefined4 *puVar5;
  
  puVar2 = PTR_DAT_8c649eec;
  iVar3 = (int)DAT_8c649ee0;
  iVar4 = param_1 * iVar3 + DAT_8c649ee8;
  *(undefined4 *)(iVar4 + 0x30) = 1;
  *(undefined4 *)(iVar4 + 0x34) = *(undefined4 *)(puVar2 + *(int *)(iVar4 + 4) * 0x50);
  if (*(char *)(iVar4 + 2) != '\0') {
    iVar4 = (*(code *)PTR_FUN_8c649ef0)(param_1);
    if (iVar4 == 0) {
      (*(code *)PTR_FUN_8c649ef4)(param_1);
    }
    else {
      iVar4 = (*(code *)PTR_FUN_8c649ef8)(param_1);
      if ((iVar4 != 0) && (*(char *)(param_1 * iVar3 + DAT_8c649ee8 + 8) != '\0')) {
        iVar3 = (*(code *)PTR_thunk_FUN_8c64a4b2_8c649efc)(param_1);
        if (iVar3 < 0) {
          return (int)DAT_8c649ee4;
        }
        iVar3 = 0;
        puVar5 = param_2;
        if (0 < param_3) {
          do {
            iVar4 = (*(code *)PTR_FUN_8c649f00)(param_1,*puVar5);
            if (iVar4 < 0) goto LAB_8c649eda;
            iVar3 = iVar3 + 1;
            puVar5 = puVar5 + 1;
          } while (iVar3 < param_3);
        }
        iVar3 = 0;
        bVar1 = false;
        if (0 < param_3) {
          do {
            iVar4 = FUN_8c649c68(param_1,*param_2);
            iVar3 = iVar3 + 1;
            if (iVar4 < 0) {
              bVar1 = true;
            }
            param_2 = param_2 + 1;
          } while (iVar3 < param_3);
        }
        iVar3 = FUN_8c64ab36(param_1);
        if (iVar3 < 0) {
          return iVar3;
        }
        iVar3 = (*(code *)PTR_FUN_8c649f04)(param_1);
        if (iVar3 < 0) {
          return iVar3;
        }
        if (!bVar1) {
          return 0;
        }
LAB_8c649eda:
        return (int)DAT_8c649ee6;
      }
    }
  }
  return (int)DAT_8c649ee2;
}

