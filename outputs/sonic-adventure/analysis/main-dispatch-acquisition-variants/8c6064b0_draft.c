/* Ghidra draft only; not buildable/verified source. */

undefined4 * FUN_8c6064b0(undefined4 *param_1)

{
  undefined4 *puVar1;
  uint *puVar2;
  uint uVar3;
  undefined4 *puVar4;
  uint uVar5;
  
  puVar1 = DAT_8c606724;
  uVar3 = DAT_8c606724[4];
  *DAT_8c60672c = DAT_8c606728 & uVar3;
  puVar2 = DAT_8c606734;
  uVar5 = uVar3 >> 0x18 & 0x1c;
  puVar4 = (undefined4 *)(uVar3 & DAT_8c606738 | DAT_8c60673c);
  *DAT_8c606730 = uVar5;
  *puVar2 = uVar5;
  puVar4[7] = param_1[3];
  puVar4[6] = param_1[2];
  puVar4[5] = param_1[1];
  puVar4[4] = *param_1;
  uVar3 = DAT_8c606744;
  puVar4[3] = puVar1[3];
  puVar4[2] = puVar1[2] & DAT_8c606740 | uVar3;
  puVar4[1] = puVar1[1];
  *puVar4 = *puVar1;
  *DAT_8c606748 = (int)(puVar1 + 4);
  return puVar4 + 8;
}

