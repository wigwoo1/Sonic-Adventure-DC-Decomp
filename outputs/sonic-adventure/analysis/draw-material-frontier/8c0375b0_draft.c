/* Ghidra draft only; not buildable/verified source. */

undefined4 * FUN_8c0375b0(void)

{
  undefined4 *puVar1;
  uint uVar2;
  uint uVar3;
  undefined4 *puVar4;
  
  uVar3 = *DAT_8c0375f4;
  *DAT_8c0375f8 = DAT_8c0375f4;
  *DAT_8c037600 = DAT_8c0375fc & uVar3;
  uVar2 = uVar3 >> 0x18 & 0x1c;
  *DAT_8c037604 = uVar2;
  *DAT_8c037608 = uVar2;
  puVar1 = DAT_8c037614;
  puVar4 = (undefined4 *)(uVar3 & DAT_8c03760c | DAT_8c037610);
  puVar4[3] = DAT_8c037614[3];
  puVar4[2] = puVar1[2];
  puVar4[1] = puVar1[1];
  *puVar4 = *puVar1;
  return puVar4 + 8;
}

