/* Ghidra draft only; not buildable/verified source. */

undefined8 FUN_8c1108e0(int param_1,int param_2)

{
  int iVar1;
  char in_FPSCR_SZ;
  
  *DAT_8c110950 = param_2;
  if (in_FPSCR_SZ == '\0') {
    param_1 = param_1 + 0x8c;
  }
  else {
    param_1 = param_1 + 200;
  }
  iVar1 = *DAT_8c110950;
  if (iVar1 == 0) {
    iVar1 = 1;
  }
  return CONCAT44(*(undefined4 *)(param_1 + -0x50),iVar1);
}

