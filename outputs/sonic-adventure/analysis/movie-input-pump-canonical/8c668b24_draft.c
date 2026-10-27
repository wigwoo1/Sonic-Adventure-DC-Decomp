/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c668b24(uint *param_1,int param_2)

{
  uint uVar1;
  
  do {
    uVar1 = *param_1;
    param_2 = param_2 + -1;
    *param_1 = ((uVar1 & 0xff) << 8 | (uVar1 & 0xff00) >> 8) << 0x10 |
               ((uVar1 & 0xff0000) >> 0x10) << 8 | uVar1 >> 0x18;
    param_1 = param_1 + 1;
  } while (param_2 != 0);
  return 0;
}

