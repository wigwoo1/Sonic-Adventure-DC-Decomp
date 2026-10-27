/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c64ddc0(int *param_1)

{
  undefined *puVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  
  if ((param_1[4] & 2U) == 0) {
    if (((param_1[4] & 1U) != 0) || (*param_1 == -1)) {
      *DAT_8c64df04 = 1;
      return 0xffffffff;
    }
  }
  else {
    iVar2 = (*(code *)PTR_FUN_8c64deec)((uint)param_1[6] >> 0x10 & DAT_8c64dee8,1);
    if (iVar2 == 1) {
      iVar2 = 0;
      uVar3 = 0;
      iVar4 = 0;
      while (uVar3 < *(uint *)PTR_DAT_8c64defc) {
        uVar3 = uVar3 + 1;
        if (*(int *)(*(int *)PTR_DAT_8c64def8 + iVar2) == *param_1) {
          iVar4 = iVar4 + 1;
        }
        iVar2 = iVar2 + 0x44;
      }
      if (1 < iVar4) {
        if ((param_1[4] & 1U) != 0) {
          return 1;
        }
        *(short *)(param_1 + 0x10) = (short)param_1[0x10] + -1;
        if ((short)param_1[0x10] != 0) {
          return 1;
        }
        param_1[4] = param_1[4] & 0xfffffffd;
        goto LAB_8c64de9a;
      }
      *(short *)(param_1 + 0x10) = (short)param_1[0x10] + -1;
      if ((short)param_1[0x10] != 0) {
        return 1;
      }
      (*(code *)PTR_FUN_8c64def0)(param_1 + 6);
      uVar3 = param_1[4] & 1;
      param_1[4] = param_1[4] & 0xfffffffd;
    }
    else {
      if ((short)param_1[0x10] != 0) {
        *(short *)(param_1 + 0x10) = (short)param_1[0x10] + -1;
      }
      if ((short)param_1[0x10] != 0) {
        return 1;
      }
      (*(code *)PTR_FUN_8c64def0)(param_1 + 6);
      puVar1 = PTR_DAT_8c64def4;
      param_1[4] = param_1[4] & 0xfffffffd;
      if (*(int *)puVar1 == *param_1) {
        *(undefined4 *)puVar1 = 0xffffffff;
      }
      uVar3 = param_1[4];
    }
    if (uVar3 != 0) {
      return 1;
    }
  }
LAB_8c64de9a:
  (*(code *)PTR_FUN_8c64df00)(param_1);
  return 1;
}

