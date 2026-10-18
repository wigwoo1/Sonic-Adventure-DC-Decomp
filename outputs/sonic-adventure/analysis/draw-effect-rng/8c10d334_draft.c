/* Ghidra draft only; not buildable/verified source. */

uint FUN_8c10d334(void)

{
  uint uVar1;
  uint uVar2;
  
  uVar1 = (uint)DAT_8c10d354;
  uVar2 = *DAT_8c10d358 * DAT_8c10d35c + (int)DAT_8c10d352;
  *DAT_8c10d358 = uVar2;
  return uVar2 >> 0x10 & uVar1;
}

