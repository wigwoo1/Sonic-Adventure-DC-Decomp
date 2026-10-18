/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c058434(uint param_1,undefined4 param_2)

{
  if (*DAT_8c058480 <= param_1) {
    return 0;
  }
  (*(code *)PTR_FUN_8c05848c)(param_2,param_1 * 0x40 + DAT_8c058490);
  return 1;
}

