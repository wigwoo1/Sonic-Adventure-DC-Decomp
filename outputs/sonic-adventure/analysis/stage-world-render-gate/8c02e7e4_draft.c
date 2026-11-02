/* Ghidra draft only; not buildable/verified source. */

undefined8 FUN_8c02e7e4(void)

{
  int iVar1;
  char in_FPSCR_SZ;
  
  iVar1 = *DAT_8c02e80c;
  if (in_FPSCR_SZ == '\0') {
    iVar1 = iVar1 + -0x20;
  }
  return CONCAT44(iVar1,1);
}

