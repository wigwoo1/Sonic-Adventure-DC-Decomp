/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c10ee64(uint param_1,uint param_2)

{
  uint uVar1;
  
  uVar1 = ((param_1 & DAT_8c10ee8c) >> 0x14) << 8 | (param_2 & DAT_8c10ee8c) >> 0x14 | DAT_8c10ee90;
  *DAT_8c10ee94 = uVar1;
  *(uint *)PTR_DAT_8c10ee98 = uVar1;
  return 0;
}

