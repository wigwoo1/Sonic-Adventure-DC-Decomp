/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c64f2ba) */
/* WARNING: Restarted to delay deadcode elimination for space: register */

void FUN_8c64f244(uint *param_1,int param_2)

{
  float fVar1;
  char in_FPSCR_PR;
  
  if (0 < param_2) goto LAB_8c64f27e;
  do {
    if ((param_1[2] != 0) && (in_FPSCR_PR == '\0')) {
      fVar1 = ((float)(param_1[2] & 0xffff) * 6.2831855) / 65536.0;
      sin(fVar1);
      cos(fVar1);
    }
    if (0 < param_2) {
      return;
    }
LAB_8c64f27e:
    if ((param_1[1] != 0) && (in_FPSCR_PR == '\0')) {
      fVar1 = ((float)(param_1[1] & 0xffff) * 6.2831855) / 65536.0;
      sin(fVar1);
      cos(fVar1);
    }
    if ((*param_1 != 0) && (in_FPSCR_PR == '\0')) {
      fVar1 = ((float)(*param_1 & 0xffff) * 6.2831855) / 65536.0;
      sin(fVar1);
      cos(fVar1);
    }
  } while (0 < param_2);
  return;
}

