/* Ghidra draft only; not buildable/verified source. */

undefined4 * FUN_8c605f3c(void)

{
  undefined4 *puVar1;
  uint *puVar2;
  uint uVar3;
  undefined4 *puVar4;
  uint uVar5;
  
  puVar1 = DAT_8c606170;
  uVar3 = DAT_8c606170[5];
  *DAT_8c606178 = DAT_8c606174 & uVar3;
  puVar2 = DAT_8c606180;
  uVar5 = uVar3 >> 0x18 & 0x1c;
  puVar4 = (undefined4 *)(uVar3 & DAT_8c606184 | DAT_8c606188);
  *DAT_8c60617c = uVar5;
  *puVar2 = uVar5;
  puVar4[2] = puVar1[2];
  puVar4[1] = puVar1[1];
  *puVar4 = *puVar1;
  *DAT_8c606194 = (int)(puVar1 + 5);
  return puVar4 + 8;
}

