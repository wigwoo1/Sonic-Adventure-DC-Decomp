/* Ghidra draft only; not buildable/verified source. */

undefined4 * FUN_8c605fca(void)

{
  undefined4 *puVar1;
  uint *puVar2;
  uint uVar3;
  uint uVar4;
  undefined4 *puVar5;
  uint uVar6;
  
  puVar1 = DAT_8c606170;
  uVar4 = DAT_8c606170[4];
  *DAT_8c606178 = DAT_8c606174 & uVar4;
  uVar3 = DAT_8c606190;
  puVar2 = DAT_8c606180;
  uVar6 = uVar4 >> 0x18 & 0x1c;
  puVar5 = (undefined4 *)(uVar4 & DAT_8c606184 | DAT_8c606188);
  *DAT_8c60617c = uVar6;
  *puVar2 = uVar6;
  puVar5[3] = puVar1[3];
  puVar5[2] = puVar1[2] & DAT_8c60618c | uVar3;
  puVar5[1] = puVar1[1];
  *puVar5 = *puVar1;
  *DAT_8c606194 = (int)(puVar1 + 4);
  return puVar5 + 8;
}

