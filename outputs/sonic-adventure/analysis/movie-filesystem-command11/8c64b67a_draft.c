/* Ghidra draft only; not buildable/verified source. */

uint FUN_8c64b67a(int param_1,undefined4 param_2,int param_3,uint param_4)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  undefined4 local_24;
  undefined4 local_20;
  int local_1c;
  
  local_20 = param_2;
  local_1c = param_3;
  if (((param_4 != 0) ||
      (uVar1 = (*(code *)PTR_FUN_8c64b6b8)(param_1,param_2), param_4 = uVar1, -1 < (int)uVar1)) &&
     (uVar1 = (*(code *)PTR_FUN_8c64b780)(param_1,local_20,&local_24), -1 < (int)uVar1)) {
    uVar4 = 0;
    iVar2 = param_1 * DAT_8c64b778 + DAT_8c64b784;
    *(undefined4 *)(iVar2 + 0x30) = 1;
    *(uint *)(iVar2 + 0x34) = param_4;
    if (param_4 != 0) {
      iVar3 = (int)DAT_8c64b77a;
      iVar2 = local_1c;
      do {
        uVar1 = (*(code *)PTR_FUN_8c64b788)(local_24,iVar2);
        if ((int)uVar1 < 0) goto LAB_8c64b702;
        uVar4 = uVar4 + 1;
        iVar2 = iVar2 + iVar3;
      } while (uVar4 < param_4);
    }
    uVar1 = 0;
LAB_8c64b702:
    (*(code *)PTR_FUN_8c64b78c)(local_24,0);
  }
  return uVar1;
}

