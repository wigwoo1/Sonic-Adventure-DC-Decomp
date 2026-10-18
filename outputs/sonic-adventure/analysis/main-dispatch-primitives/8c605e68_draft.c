/* Ghidra draft only; not buildable/verified source. */

undefined4 * FUN_8c605e68(void)

{
  undefined4 *puVar1;
  uint *puVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  undefined4 *puVar6;
  uint uVar7;
  
  uVar4 = DAT_8c605edc;
  puVar1 = DAT_8c605ebc;
  uVar5 = DAT_8c605ebc[4];
  uVar7 = uVar5 >> 0x18 & 0x1c;
  *DAT_8c605ecc = DAT_8c605ec8 & uVar5;
  uVar3 = DAT_8c605ed8;
  puVar2 = DAT_8c605ed4;
  *DAT_8c605ed0 = uVar7;
  *puVar2 = uVar7;
  puVar6 = (undefined4 *)(uVar5 & uVar3 | uVar4);
  puVar6[2] = puVar1[2] & DAT_8c605ee0 | DAT_8c605ee4;
  puVar6[1] = puVar1[1];
  *puVar6 = *puVar1;
  *DAT_8c605ee8 = (int)(puVar1 + 4);
  return puVar6 + 8;
}

