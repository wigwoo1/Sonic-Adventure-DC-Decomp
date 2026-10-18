/* Ghidra draft only; not buildable/verified source. */

undefined4 * FUN_8c037460(void)

{
  undefined4 *puVar1;
  uint uVar2;
  uint uVar3;
  undefined4 *puVar4;
  
  uVar3 = *DAT_8c0374a8;
  *DAT_8c0374ac = DAT_8c0374a8;
  *DAT_8c0374b4 = DAT_8c0374b0 & uVar3;
  uVar2 = uVar3 >> 0x18 & 0x1c;
  *DAT_8c0374b8 = uVar2;
  *DAT_8c0374bc = uVar2;
  puVar1 = DAT_8c0374c8;
  puVar4 = (undefined4 *)(uVar3 & DAT_8c0374c0 | DAT_8c0374c4);
  puVar4[2] = DAT_8c0374c8[2] & DAT_8c0374cc | DAT_8c0374d0;
  puVar4[1] = puVar1[1];
  *puVar4 = *puVar1;
  return puVar4 + 8;
}

