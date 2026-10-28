/* Ghidra draft only; not buildable/verified source. */

void FUN_8c07256a(uint param_1,int param_2,int param_3)

{
  char in_FPSCR_SZ;
  
  if (param_3 != 0) {
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(param_3 + 0x40) = 0;
      *(undefined4 *)(param_3 + 0x3c) = 0;
      *(undefined4 *)(param_3 + 0x38) = 0;
    }
    else {
      *(ulonglong *)(param_3 + 0x40) = (ulonglong)param_1;
      *(ulonglong *)(param_3 + 0x3c) = (ulonglong)param_1;
      *(ulonglong *)(param_3 + 0x38) = (ulonglong)param_1;
    }
  }
  if (param_2 != 0) {
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(param_2 + 0xc) = 0;
      *(undefined4 *)(param_2 + 8) = 0;
      *(undefined4 *)(param_2 + 4) = 0;
    }
    else {
      *(ulonglong *)(param_2 + 0xc) = (ulonglong)param_1;
      *(ulonglong *)(param_2 + 8) = (ulonglong)param_1;
      *(ulonglong *)(param_2 + 4) = (ulonglong)param_1;
    }
  }
  return;
}

