/* Ghidra draft only; not buildable/verified source. */

void FUN_8c610d30(double param_1)

{
  int iVar1;
  undefined8 in_xd4;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  iVar1 = DAT_8c610d64;
  if (in_FPSCR_PR == '\0') {
    param_1 = (double)CONCAT44(-(float)((ulonglong)param_1 >> 0x20),SUB84(param_1,0));
  }
  else {
    param_1 = -param_1;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(DAT_8c610d64 + 0xc) = (int)((ulonglong)param_1 >> 0x20);
    *(int *)(iVar1 + 0x20) = SUB84(param_1,0);
  }
  else {
    *(double *)(DAT_8c610d64 + 0xc) = param_1;
    *(undefined8 *)(iVar1 + 0x20) = in_xd4;
  }
  return;
}

