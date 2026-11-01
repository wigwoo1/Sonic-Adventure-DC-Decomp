/* Ghidra draft only; not buildable/verified source. */

uint FUN_8c64e7a2(int param_1,uint param_2,int param_3)

{
  uint uVar1;
  
  if ((param_1 != 0) && (uVar1 = 0, param_2 != 0)) {
    do {
      if (*(int *)(uVar1 * 0x44 + param_1) == param_3) {
        return uVar1;
      }
      uVar1 = uVar1 + 1;
    } while (uVar1 < param_2);
  }
  return 0xffffffff;
}

