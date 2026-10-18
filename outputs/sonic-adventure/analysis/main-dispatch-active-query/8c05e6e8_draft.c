/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c05e734) */

undefined4 FUN_8c05e6e8(int param_1)

{
  int iVar1;
  float fVar2;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  iVar1 = FUN_8c05e6c4();
  if (iVar1 == 0) {
    if (*(char *)(param_1 + 8) == '\0') {
      FUN_8c05e69e();
    }
  }
  else {
    iVar1 = *(int *)(*(int *)(*(int *)(*(int *)(param_1 + 0x38) + 0x18) + 0x10) + 0x20);
    if (*(short *)(iVar1 + 6) == 0) {
      FUN_8c05e69e();
      return 0xffffffff;
    }
    if (*(short *)(*(int *)(param_1 + 0x38) + 0x16) == 1) {
      fVar2 = DAT_8c05e764._4_4_;
      if (in_FPSCR_SZ == '\0') {
        fVar2 = (float)DAT_8c05e764;
      }
      if (in_FPSCR_PR == '\0' && (float)*(int *)(iVar1 + 0xc) < fVar2) {
        return 1;
      }
      FUN_8c05e69e();
    }
    else {
      FUN_8c05e69e();
    }
  }
  return 0;
}

