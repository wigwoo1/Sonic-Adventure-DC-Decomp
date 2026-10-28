/* Ghidra draft only; not buildable/verified source. */

uint FUN_8c64b71c(int param_1,undefined4 param_2,int param_3,uint param_4,undefined4 param_5,
                 uint param_6)

{
  undefined *puVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  int local_20;
  int local_1c;
  
  local_1c = param_3;
  uVar2 = (*(code *)PTR_FUN_8c64b790)(param_1,0);
  iVar3 = (*(code *)PTR_FUN_8c64b794)(param_1,param_2);
  if (0 < iVar3) {
    uVar2 = uVar2 + iVar3;
  }
  if (-1 < (int)uVar2) {
    if (uVar2 < param_4) {
      uVar2 = (uint)DAT_8c64b77c;
    }
    else if ((DAT_8c64b798 & param_6) == 0) {
      uVar2 = (*(code *)PTR_FUN_8c64b818)(param_1,param_2,param_5,&local_20);
      iVar3 = DAT_8c64b81c;
      if (-1 < (int)uVar2) {
        iVar4 = (int)DAT_8c64b814;
        uVar5 = 0;
        *(char *)(local_20 + 1) = (char)param_6;
        puVar1 = PTR_DAT_8c64b820;
        iVar3 = param_1 * iVar4 + iVar3;
        *(undefined4 *)(iVar3 + 0x30) = 1;
        *(uint *)(iVar3 + 0x34) = *(int *)(puVar1 + *(int *)(iVar3 + 4) * 0x50) + param_4;
        if (param_4 != 0) {
          iVar4 = (int)DAT_8c64b816;
          iVar3 = local_1c;
          do {
            uVar2 = (*(code *)PTR_FUN_8c64b824)(local_20,iVar3);
            if ((int)uVar2 < 0) goto LAB_8c64b7fa;
            uVar5 = uVar5 + 1;
            iVar3 = iVar3 + iVar4;
          } while (uVar5 < param_4);
        }
        uVar2 = 0;
LAB_8c64b7fa:
        (*(code *)PTR_FUN_8c64b828)(local_20,0);
      }
    }
    else {
      uVar2 = FUN_8c64b82c(param_1,param_2,local_1c,param_4,param_5,param_6);
    }
  }
  return uVar2;
}

