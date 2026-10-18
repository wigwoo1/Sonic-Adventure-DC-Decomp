/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c64bed8(undefined4 param_1)

{
  uint *puVar1;
  
  puVar1 = (uint *)(*(code *)PTR_FUN_8c64bfd8)(param_1);
  if ((*puVar1 & (int)DAT_8c64bfd4) != 0) {
    return 1;
  }
  return 0;
}

