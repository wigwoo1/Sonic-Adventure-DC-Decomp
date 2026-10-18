/* Ghidra draft only; not buildable/verified source. */

void FUN_8c037098(int param_1)

{
  int iVar1;
  undefined8 *puVar2;
  undefined4 unaff_r8;
  undefined4 unaff_r9;
  undefined4 unaff_r10;
  undefined4 unaff_r11;
  undefined4 unaff_r13;
  undefined4 unaff_r14;
  undefined8 in_dr12;
  undefined8 in_dr14;
  undefined8 in_xd12;
  undefined8 in_xd14;
  char extraout_T;
  undefined4 in_PR;
  char in_FPSCR_SZ;
  
  if (*DAT_8c0370a4 != 0) {
    return;
  }
  FUN_8c03718c();
  iVar1 = DAT_8c037100;
  if (extraout_T == '\0') {
    *(undefined4 *)(DAT_8c037100 + -4) = in_PR;
    if (in_FPSCR_SZ == '\0') {
      *(int *)(iVar1 + -8) = (int)in_dr14;
      *(int *)(iVar1 + -0xc) = (int)((ulonglong)in_dr14 >> 0x20);
      *(int *)(iVar1 + -0x10) = (int)in_dr12;
      puVar2 = (undefined8 *)(iVar1 + -0x14);
      *(int *)puVar2 = (int)((ulonglong)in_dr12 >> 0x20);
    }
    else {
      *(undefined8 *)(iVar1 + -0xc) = in_xd14;
      *(undefined8 *)(iVar1 + -0x14) = in_dr14;
      *(undefined8 *)(iVar1 + -0x1c) = in_xd12;
      puVar2 = (undefined8 *)(iVar1 + -0x24);
      *puVar2 = in_dr12;
    }
    *(undefined4 *)((int)puVar2 + -4) = unaff_r14;
    *(undefined4 *)(puVar2 + -1) = unaff_r13;
    *(code **)((int)puVar2 + -0xc) = FUN_8c037098;
    *(undefined4 *)(puVar2 + -2) = unaff_r11;
    *(undefined4 *)((int)puVar2 + -0x14) = unaff_r10;
    *(undefined4 *)(puVar2 + -3) = unaff_r9;
    *(undefined4 *)((int)puVar2 + -0x1c) = unaff_r8;
    FUN_8c037294();
    if (0 < *(int *)(param_1 + 8)) {
      FUN_8c037350();
      FUN_8c0376d0();
    }
    FUN_8c036ffc();
    return;
  }
  return;
}

