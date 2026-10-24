/* Ghidra draft only; not buildable/verified source. */

uint FUN_8c601d84(uint param_1)

{
  uint uVar1;
  
  uVar1 = param_1 >> 10 & DAT_8c601e3c;
  if ((DAT_8c601e40 & param_1) != 0) {
    uVar1 = uVar1 | (int)DAT_8c601e36;
  }
  if ((param_1 & DAT_8c601e44) != 0) {
    uVar1 = uVar1 | 0xf;
  }
  return uVar1;
}

