/* Ghidra draft only; not buildable/verified source. */

int FUN_8c606fe4(ushort param_1)

{
  int iVar1;
  
  iVar1 = 2;
  if ((param_1 & 1) != 0) {
    iVar1 = 0x1e;
  }
  if ((param_1 & 2) != 0) {
    iVar1 = iVar1 + 2;
  }
  if ((param_1 & 4) != 0) {
    iVar1 = iVar1 + 2;
  }
  if ((param_1 & 8) != 0) {
    iVar1 = iVar1 + 4;
  }
  return iVar1;
}

