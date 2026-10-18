/* Ghidra draft only; not buildable/verified source. */

undefined4 * FUN_8c0374d4(void)

{
  undefined4 *puVar1;
  uint uVar2;
  uint uVar3;
  undefined4 *puVar4;
  
  uVar3 = *DAT_8c037514;
  *DAT_8c037518 = DAT_8c037514;
  *DAT_8c037520 = DAT_8c03751c & uVar3;
  uVar2 = uVar3 >> 0x18 & 0x1c;
  *DAT_8c037524 = uVar2;
  *DAT_8c037528 = uVar2;
  puVar1 = DAT_8c037534;
  puVar4 = (undefined4 *)(uVar3 & DAT_8c03752c | DAT_8c037530);
  puVar4[2] = DAT_8c037534[2];
  puVar4[1] = puVar1[1];
  *puVar4 = *puVar1;
  return puVar4 + 8;
}

