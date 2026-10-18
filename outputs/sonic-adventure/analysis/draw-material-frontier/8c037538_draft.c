/* Ghidra draft only; not buildable/verified source. */

undefined4 * FUN_8c037538(void)

{
  undefined4 *puVar1;
  uint uVar2;
  uint uVar3;
  undefined4 *puVar4;
  
  uVar3 = *DAT_8c037584;
  *DAT_8c037588 = DAT_8c037584;
  *DAT_8c037590 = DAT_8c03758c & uVar3;
  uVar2 = uVar3 >> 0x18 & 0x1c;
  *DAT_8c037594 = uVar2;
  *DAT_8c037598 = uVar2;
  puVar1 = DAT_8c0375a4;
  puVar4 = (undefined4 *)(uVar3 & DAT_8c03759c | DAT_8c0375a0);
  puVar4[3] = DAT_8c0375a4[3];
  puVar4[2] = puVar1[2] & DAT_8c0375a8 | DAT_8c0375ac;
  puVar4[1] = puVar1[1];
  *puVar4 = *puVar1;
  return puVar4 + 8;
}

