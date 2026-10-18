/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c05694a(uint param_1,uint param_2)

{
  uint *puVar1;
  
  puVar1 = DAT_8c0569b4;
  *_DAT_8c0569ac = *_DAT_8c0569ac | param_1;
  *puVar1 = *puVar1 | param_2;
  return;
}

