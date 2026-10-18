/* Ghidra draft only; not buildable/verified source. */

void FUN_8c605006(uint param_1,undefined4 *param_2)

{
  code *pcVar1;
  char in_FPSCR_SZ;
  
  *param_2 = DAT_8c6051d0;
  param_2[1] = 0;
  param_2[2] = 0;
  param_2[3] = 1;
  param_2[4] = 0;
  param_2[5] = 6;
  param_2[6] = 1;
  param_2[7] = 0;
  param_2[8] = 1;
  param_2[9] = 0;
  param_2[10] = 0;
  param_2[0xb] = 8;
  param_2[0xc] = 6;
  param_2[0xd] = 0;
  param_2[0xe] = 0;
  param_2[0xf] = 2;
  param_2[0x10] = 0;
  param_2[0x11] = 0;
  param_2[0x12] = 0;
  param_2[0x14] = 0;
  param_2[0x13] = 0;
  param_2[0x15] = 0;
  param_2[0x16] = 0;
  param_2[0x17] = 4;
  param_2[0x18] = 3;
  param_2[0x1b] = 0;
  param_2[0x19] = 0;
  param_2[0x1a] = 0;
  if (in_FPSCR_SZ == '\0') {
    param_2[0x1c] = 0x3f800000;
    param_2[0x1d] = 0x3f800000;
    param_2[0x1e] = 0x3f800000;
    param_2[0x1f] = 0x3f800000;
    param_2[0x20] = 0x3f800000;
    param_2[0x21] = 0;
    param_2[0x22] = 0;
    pcVar1 = (code *)PTR_FUN_8c6051d4;
    param_2[0x23] = 0;
  }
  else {
    *(ulonglong *)(param_2 + 0x1c) = CONCAT44(0x3f800000,param_1);
    *(ulonglong *)(param_2 + 0x1d) = CONCAT44(0x3f800000,param_1);
    *(ulonglong *)(param_2 + 0x1e) = CONCAT44(0x3f800000,param_1);
    *(ulonglong *)(param_2 + 0x1f) = CONCAT44(0x3f800000,param_1);
    *(ulonglong *)(param_2 + 0x20) = CONCAT44(0x3f800000,param_1);
    *(ulonglong *)(param_2 + 0x21) = (ulonglong)param_1;
    *(ulonglong *)(param_2 + 0x22) = (ulonglong)param_1;
    pcVar1 = (code *)PTR_FUN_8c6051d4;
    *(ulonglong *)(param_2 + 0x23) = (ulonglong)param_1;
  }
  (*pcVar1)();
  return;
}

