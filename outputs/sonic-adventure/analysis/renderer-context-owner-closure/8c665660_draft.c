/* Ghidra draft only; not buildable/verified source. */

int FUN_8c665660(uint param_1)

{
  int iVar1;
  
  iVar1 = 0;
  if (param_1 < DAT_8c6656b0) {
    if (DAT_8c6656ac <= param_1) {
      param_1 = param_1 - DAT_8c6656ac;
      iVar1 = 4;
    }
    iVar1 = (param_1 & 0xfffffffc) * 2 + (param_1 & 3) + iVar1;
  }
  else {
    if (DAT_8c6656b4 <= param_1) {
      param_1 = param_1 - DAT_8c6656ac;
      iVar1 = 4;
    }
    iVar1 = (param_1 & 0xfffffffc) * 2 + (param_1 & 3) + iVar1 + DAT_8c6656b8;
  }
  return iVar1;
}

