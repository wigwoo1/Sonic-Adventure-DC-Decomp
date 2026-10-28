/* Ghidra draft only; not buildable/verified source. */

uint FUN_8c64b91c(int param_1,undefined4 param_2,int param_3,uint param_4,undefined4 param_5,
                 uint param_6)

{
  undefined *puVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  int local_24;
  int local_20;
  undefined4 local_1c;
  
  local_20 = param_3;
  local_1c = param_2;
  uVar2 = (*(code *)PTR_FUN_8c64b970)(param_1,1);
  if (-1 < (int)uVar2) {
    if (uVar2 < param_4) {
      uVar2 = (uint)DAT_8c64ba12;
    }
    else if ((DAT_8c64ba18 & param_6) == 0) {
      uVar2 = (*(code *)PTR_FUN_8c64ba1c)(param_1,local_1c,param_5,&local_24);
      iVar4 = DAT_8c64ba20;
      if (-1 < (int)uVar2) {
        iVar3 = (int)DAT_8c64ba14;
        uVar5 = 0;
        *(char *)(local_24 + 1) = (char)param_6;
        puVar1 = PTR_DAT_8c64ba24;
        iVar4 = param_1 * iVar3 + iVar4;
        *(undefined4 *)(iVar4 + 0x30) = 1;
        *(uint *)(iVar4 + 0x34) = *(int *)(puVar1 + *(int *)(iVar4 + 4) * 0x50) + param_4;
        if (param_4 != 0) {
          iVar3 = (int)DAT_8c64ba16;
          iVar4 = local_20;
          do {
            uVar2 = (*(code *)PTR_FUN_8c64ba28)(local_24,iVar4);
            if ((int)uVar2 < 0) goto LAB_8c64b9f8;
            uVar5 = uVar5 + 1;
            iVar4 = iVar4 + iVar3;
          } while (uVar5 < param_4);
        }
        uVar2 = 0;
LAB_8c64b9f8:
        (*(code *)PTR_FUN_8c64ba2c)(local_24,0);
      }
    }
    else {
      uVar2 = FUN_8c64ba30(param_1,local_1c,local_20,param_4,param_5,param_6);
    }
  }
  return uVar2;
}

