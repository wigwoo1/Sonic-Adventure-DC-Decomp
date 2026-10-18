/* Ghidra draft only; not buildable/verified source. */

longlong FUN_8c639b98(int param_1)

{
  uint in_r1;
  int iVar1;
  char in_FPSCR_SZ;
  
  if (*DAT_8c639ba8 - param_1 != 0) {
    *DAT_8c639ba8 = *DAT_8c639ba8 - param_1;
    iVar1 = *DAT_8c639bd4 + param_1 * -0x40;
    *DAT_8c639bd4 = iVar1;
    if (in_FPSCR_SZ == '\0') {
      iVar1 = iVar1 + 0x20;
    }
    else {
      iVar1 = iVar1 + 0x40;
    }
    return CONCAT44(iVar1,1);
  }
  return (ulonglong)in_r1 << 0x20;
}

