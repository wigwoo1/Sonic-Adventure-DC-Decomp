/* Ghidra draft only; not buildable/verified source. */

int FUN_8c037108(int param_1)

{
  undefined8 *puVar1;
  uint uVar2;
  int iVar3;
  undefined4 unaff_r8;
  undefined4 unaff_r9;
  undefined4 unaff_r10;
  undefined4 unaff_r11;
  undefined4 unaff_r12;
  undefined4 unaff_r13;
  undefined4 unaff_r14;
  undefined8 in_dr12;
  undefined8 in_dr14;
  undefined8 in_xd12;
  undefined8 in_xd14;
  int in_GBR;
  undefined4 in_PR;
  char in_FPSCR_SZ;
  
  iVar3 = DAT_8c037180;
  *(undefined4 *)(DAT_8c037180 + -4) = in_PR;
  if (in_FPSCR_SZ == '\0') {
    *(int *)(iVar3 + -8) = (int)in_dr14;
    *(int *)(iVar3 + -0xc) = (int)((ulonglong)in_dr14 >> 0x20);
    *(int *)(iVar3 + -0x10) = (int)in_dr12;
    puVar1 = (undefined8 *)(iVar3 + -0x14);
    *(int *)puVar1 = (int)((ulonglong)in_dr12 >> 0x20);
  }
  else {
    *(undefined8 *)(iVar3 + -0xc) = in_xd14;
    *(undefined8 *)(iVar3 + -0x14) = in_dr14;
    *(undefined8 *)(iVar3 + -0x1c) = in_xd12;
    puVar1 = (undefined8 *)(iVar3 + -0x24);
    *puVar1 = in_dr12;
  }
  *(undefined4 *)((int)puVar1 + -4) = unaff_r14;
  *(undefined4 *)(puVar1 + -1) = unaff_r13;
  *(undefined4 *)((int)puVar1 + -0xc) = unaff_r12;
  *(undefined4 *)(puVar1 + -2) = unaff_r11;
  *(undefined4 *)((int)puVar1 + -0x14) = unaff_r10;
  *(undefined4 *)(puVar1 + -3) = unaff_r9;
  *(undefined4 *)((int)puVar1 + -0x1c) = unaff_r8;
  FUN_8c037294();
  if (0 < *(int *)(param_1 + 8)) {
    iVar3 = (uint)(**(ushort **)(param_1 + 0xc) & DAT_8c03717c) * 0x14 + *(int *)(param_1 + 0x10);
    *(int *)(in_GBR + 0x34) = iVar3;
    uVar2 = *(uint *)(iVar3 + 0x10);
    if (*DAT_8c037184 != 0) {
      uVar2 = uVar2 & DAT_8c037184[1] | DAT_8c037184[2];
    }
    *(uint *)(in_GBR + 0x2c) = uVar2;
    FUN_8c037350();
    FUN_8c0376d0();
  }
  if (in_FPSCR_SZ == '\0') {
    iVar3 = DAT_8c037188 + 0x2c;
  }
  else {
    iVar3 = DAT_8c037188 + 0x3c;
  }
  return iVar3 + 4;
}

