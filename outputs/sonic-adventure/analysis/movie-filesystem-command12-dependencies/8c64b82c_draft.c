/* Ghidra draft only; not buildable/verified source. */

int FUN_8c64b82c(int param_1,undefined4 param_2,int param_3,uint param_4,undefined4 param_5,
                undefined1 param_6)

{
  int iVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int local_2c;
  undefined4 local_28;
  int local_24;
  
  local_28 = param_2;
  local_24 = param_3;
  iVar1 = (*(code *)PTR_FUN_8c64b94c)(param_1,param_2,param_5,&local_2c);
  if (-1 < iVar1) {
    iVar1 = (int)DAT_8c64b946;
    uVar4 = 0;
    iVar7 = (int)DAT_8c64b948;
    *(undefined1 *)(local_2c + 1) = param_6;
    iVar1 = param_1 * iVar1 + DAT_8c64b950;
    *(undefined4 *)(iVar1 + 0x30) = 1;
    *(uint *)(iVar1 + 0x34) = param_4 * 2 + *(int *)(PTR_DAT_8c64b954 + *(int *)(iVar1 + 4) * 0x50);
    iVar6 = local_24;
    if (param_4 != 0) {
      do {
        iVar1 = (*(code *)PTR_FUN_8c64b958)(local_2c,iVar6);
        if (iVar1 < 0) goto LAB_8c64b8fe;
        uVar4 = uVar4 + 1;
        iVar6 = iVar6 + iVar7;
      } while (uVar4 < param_4);
    }
    (*(code *)PTR_FUN_8c64b95c)(local_2c,0);
    iVar1 = (*(code *)PTR_FUN_8c64b960)(param_1,local_28,&local_2c);
    if (-1 < iVar1) {
      uVar4 = 0;
      iVar6 = param_1 * 0x800 + DAT_8c64b964;
      if (param_4 != 0) {
        iVar3 = (int)DAT_8c64b94a;
        iVar5 = local_24;
        do {
          iVar1 = (*(code *)PTR_FUN_8c64b968)(local_2c,iVar6);
          if ((iVar1 < 0) ||
             (iVar2 = (*(code *)PTR_FUN_8c64b96c)(iVar6,iVar5), iVar1 = iVar3, iVar2 != 0))
          goto LAB_8c64b8fe;
          uVar4 = uVar4 + 1;
          iVar5 = iVar5 + iVar7;
        } while (uVar4 < param_4);
      }
      iVar1 = 0;
LAB_8c64b8fe:
      (*(code *)PTR_FUN_8c64b95c)(local_2c,0);
    }
  }
  return iVar1;
}

