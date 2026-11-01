/* Ghidra draft only; not buildable/verified source. */

void FUN_8c0b3f2c(int param_1)

{
  double *pdVar1;
  undefined *puVar2;
  int iVar3;
  double dVar4;
  int iVar5;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 uStack_20;
  int iStack_1c;
  int local_18;
  undefined *local_14;
  undefined *local_10;
  
  iVar3 = *(int *)(param_1 + 0x20);
  if (*DAT_8c0b4000._4_4_ < 1) {
    local_14 = PTR_PTR_8c0b4008;
    local_10 = PTR_DAT_8c0b400c;
    (*(code *)PTR_FUN_8c0b4010)(0,8);
    (*(code *)PTR_FUN_8c0b4010)(1,10);
    puVar2 = PTR_FUN_8c0b4014;
    for (iVar3 = *(int *)(iVar3 + 0x10); iVar3 != 0; iVar3 = *(int *)(iVar3 + 0x30)) {
      local_18 = (int)*(short *)(iVar3 + 0x1c);
      local_2c = *(undefined4 *)(iVar3 + 4);
      local_28 = *(undefined4 *)(iVar3 + 8);
      local_24 = *(undefined4 *)(iVar3 + 0xc);
      if (in_FPSCR_SZ == '\0') {
        uStack_20 = *(undefined4 *)(iVar3 + 0x24);
        pdVar1 = (double *)(iVar3 + 0x20);
      }
      else {
        uStack_20 = (undefined4)*(undefined8 *)(iVar3 + 0x24);
        pdVar1 = (double *)(iVar3 + 0x24);
      }
      dVar4 = *pdVar1;
      if (in_FPSCR_PR == '\0') {
        dVar4 = (double)CONCAT44((float)((ulonglong)dVar4 >> 0x20) * SUB84(dVar4,0),SUB84(dVar4,0));
      }
      else {
        dVar4 = dVar4 * dVar4;
      }
      iVar5 = (int)((ulonglong)dVar4 >> 0x20);
      iStack_1c = iVar5;
      if (in_FPSCR_SZ != '\0') {
        iStack_1c = SUB84(dVar4,0);
        local_18 = iVar5;
      }
      (*(code *)puVar2)(&local_2c,0,0x31);
    }
  }
  return;
}

