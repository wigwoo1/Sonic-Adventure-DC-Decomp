/* Ghidra draft only; not buildable/verified source. */

int FUN_8c64bbb2(int param_1,undefined4 param_2,int param_3,int param_4,uint param_5)

{
  int iVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  undefined4 local_28;
  int local_24;
  undefined4 local_20;
  int local_1c;
  
  local_24 = param_4;
  local_20 = param_2;
  local_1c = param_3;
  if (param_5 == 0) {
    iVar1 = (*(code *)PTR_FUN_8c64bc58)(param_1,param_2);
    if (iVar1 < 0) {
      return iVar1;
    }
    param_5 = iVar1 - local_24;
  }
  iVar2 = (*(code *)PTR_FUN_8c64bc5c)(param_1,local_20,&local_28);
  iVar1 = local_1c;
  if (-1 < iVar2) {
    iVar2 = param_1 * DAT_8c64bc4c + DAT_8c64bc60;
    *(undefined4 *)(iVar2 + 0x30) = 1;
    *(uint *)(iVar2 + 0x34) = param_5;
    FUN_8c64bb4c(local_28,local_24,0);
    uVar4 = 0;
    if (param_5 != 0) {
      iVar3 = (int)DAT_8c64bc4e;
      do {
        iVar2 = (*(code *)PTR_FUN_8c64bc64)(local_28,iVar1);
        if (iVar2 < 0) goto LAB_8c64bc32;
        uVar4 = uVar4 + 1;
        iVar1 = iVar1 + iVar3;
      } while (uVar4 < param_5);
    }
    iVar2 = 0;
LAB_8c64bc32:
    (*(code *)PTR_FUN_8c64bc68)(local_28,0);
  }
  return iVar2;
}

