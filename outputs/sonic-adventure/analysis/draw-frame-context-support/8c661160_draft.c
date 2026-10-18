/* Ghidra draft only; not buildable/verified source. */

void FUN_8c661160(uint param_1,int param_2,int param_3)

{
  uint *puVar1;
  
  puVar1 = DAT_8c661280;
  *DAT_8c661280 = (param_1 & 7) << 0x18;
  if (param_3 != 0) {
    *puVar1 = *puVar1 | DAT_8c661284;
  }
  if (param_2 != 0) {
    *DAT_8c661280 = *DAT_8c661280 | DAT_8c661288;
  }
  return;
}

