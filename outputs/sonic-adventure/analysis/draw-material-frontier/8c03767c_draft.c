/* Ghidra draft only; not buildable/verified source. */

void FUN_8c03767c(void)

{
  uint *puVar1;
  uint in_r0;
  uint uVar2;
  uint uVar3;
  
  puVar1 = DAT_8c03769c;
  uVar2 = DAT_8c03769c[2] & DAT_8c0376a4;
  uVar3 = DAT_8c0376ac & in_r0;
  *DAT_8c03769c = *DAT_8c03769c & DAT_8c0376a0 | DAT_8c0376a8 & in_r0;
  puVar1[2] = uVar2 | uVar3;
  return;
}

