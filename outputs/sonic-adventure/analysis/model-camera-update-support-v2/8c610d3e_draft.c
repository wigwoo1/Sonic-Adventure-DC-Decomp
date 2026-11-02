/* Ghidra draft only; not buildable/verified source. */

void FUN_8c610d3e(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  undefined8 in_xd4;
  char in_FPSCR_SZ;
  
  iVar1 = DAT_8c610d74;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(DAT_8c610d74 + 4) = param_1;
    *(undefined4 *)(iVar1 + 8) = param_2;
    *(undefined4 *)(iVar1 + 0xc) = param_3;
  }
  else {
    *(ulonglong *)(DAT_8c610d74 + 4) = CONCAT44(param_1,param_2);
    *(undefined8 *)(iVar1 + 8) = in_xd4;
    *(ulonglong *)(iVar1 + 0xc) = CONCAT44(param_3,param_4);
  }
  return;
}

