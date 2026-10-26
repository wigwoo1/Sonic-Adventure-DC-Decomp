/* Ghidra draft only; not buildable/verified source. */

int FUN_8c66563e(uint param_1)

{
  int iVar1;
  
  iVar1 = ((param_1 & 0xfffffff8) >> 1) + (param_1 & 3);
  if ((param_1 & 4) != 0) {
    iVar1 = iVar1 + DAT_8c6656ac;
  }
  if (DAT_8c6656b0 <= param_1) {
    iVar1 = iVar1 + DAT_8c6656ac;
  }
  return iVar1;
}

