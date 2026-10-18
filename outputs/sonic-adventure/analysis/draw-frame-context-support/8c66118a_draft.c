/* Ghidra draft only; not buildable/verified source. */

int FUN_8c66118a(uint param_1)

{
  int iVar1;
  
  iVar1 = 1;
  if ((DAT_8c66128c & param_1) != 0) {
    iVar1 = 2;
    if ((DAT_8c661290 & param_1) != 0) {
      iVar1 = 3;
    }
    if ((param_1 & DAT_8c661294) == 0) {
      iVar1 = iVar1 + 1;
    }
  }
  return iVar1;
}

