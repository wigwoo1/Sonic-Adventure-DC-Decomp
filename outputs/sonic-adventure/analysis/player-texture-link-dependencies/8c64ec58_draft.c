/* Ghidra draft only; not buildable/verified source. */

int FUN_8c64ec58(int param_1,int param_2)

{
  uint *puVar1;
  undefined *puVar2;
  undefined4 *puVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  int local_38;
  int local_34;
  
  local_34 = param_1;
  iVar4 = (*(code *)PTR_FUN_8c64ed0c)(param_2,*(undefined4 *)(param_1 + 4),&local_38);
  puVar3 = DAT_8c64ed1c;
  puVar2 = PTR_FUN_8c64ed18;
  puVar1 = DAT_8c64ed10;
  if (iVar4 < 0) {
    return iVar4;
  }
  uVar8 = *(uint *)(param_2 + 0x18);
  uVar5 = local_38 - 8;
  if ((int)uVar8 < (int)uVar5) {
    local_38 = uVar5 - uVar8;
    uVar5 = uVar8;
  }
  else {
    local_38 = 0;
  }
  uVar8 = *DAT_8c64ed10;
  uVar7 = 0;
  if (uVar5 <= uVar8) {
    uVar8 = uVar5;
  }
  uVar6 = *(uint *)(param_2 + 4) >> 0x10 & DAT_8c64ed14;
  if ((uVar6 == (int)DAT_8c64ed08) || (uVar6 == (int)DAT_8c64ed0a)) {
    iVar4 = (*(code *)PTR_FUN_8c64ee68)(*DAT_8c64ed1c,uVar5);
    if (iVar4 < 0) goto LAB_8c64ed68;
    (*(code *)PTR_FUN_8c64ee6c)(*puVar3,uVar5);
    (*(code *)PTR_FUN_8c64ee70)(param_2 + 4,*puVar3,0);
    do {
      iVar4 = (*(code *)puVar2)();
    } while (iVar4 != 0);
  }
  else if (0 < (int)uVar5) {
    do {
      iVar4 = (*(code *)PTR_FUN_8c64ed20)(*puVar3,uVar8);
      if (iVar4 < 0) goto LAB_8c64ed68;
      (*(code *)PTR_FUN_8c64ed24)(*puVar3,uVar8);
      (*(code *)PTR_FUN_8c64ed28)(param_2 + 4,*puVar3,uVar7,uVar8);
      uVar7 = uVar7 + uVar8;
      if (uVar5 - uVar7 < *puVar1) {
        uVar8 = uVar5 - uVar7;
      }
      do {
        iVar4 = (*(code *)puVar2)();
      } while (iVar4 != 0);
    } while (uVar7 != uVar5);
  }
  if ((local_38 < 1) || (iVar4 = (*(code *)PTR_FUN_8c64ee68)(*puVar3,local_38), -1 < iVar4)) {
    return 1;
  }
LAB_8c64ed68:
  (*(code *)PTR_FUN_8c64ee74)(param_2 + 4);
  return -1;
}

