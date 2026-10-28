/* Ghidra draft only; not buildable/verified source. */

void FUN_8c63a970(ushort param_1)

{
  float fVar1;
  char in_FPSCR_PR;
  
  if (in_FPSCR_PR == '\0') {
    fVar1 = ((float)param_1 * 6.2831855) / 65536.0;
    sin(fVar1);
    cos(fVar1);
  }
  return;
}

