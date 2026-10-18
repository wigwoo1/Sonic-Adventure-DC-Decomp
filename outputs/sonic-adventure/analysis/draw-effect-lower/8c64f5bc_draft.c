/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c64f5bc(int *param_1,int param_2)

{
  int iVar1;
  int iVar2;
  undefined8 uVar3;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  iVar2 = 0;
  iVar1 = *param_1;
  if (0 < param_2) {
    do {
      if (in_FPSCR_SZ == '\0') {
        uVar3 = CONCAT44(*(undefined4 *)(iVar1 + 8),*DAT_8c64f5e4);
      }
      else {
        uVar3 = *(undefined8 *)(iVar1 + 8);
      }
      if (in_FPSCR_PR == '\0' && (float)uVar3 < (float)((ulonglong)uVar3 >> 0x20)) {
        return 0;
      }
      iVar2 = iVar2 + 1;
      iVar1 = iVar1 + 0xc;
    } while (iVar2 < param_2);
  }
  return 1;
}

