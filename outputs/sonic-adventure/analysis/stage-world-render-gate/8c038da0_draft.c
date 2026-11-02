/* Ghidra draft only; not buildable/verified source. */

void FUN_8c038da0(void)

{
  undefined4 uVar1;
  uint uVar2;
  int in_GBR;
  
  uVar2 = *DAT_8c038de0;
  *(uint *)(in_GBR + 0x1c) = (uint)((uVar2 & DAT_8c038de4) != 0);
  *(undefined4 *)(in_GBR + 0x20) = *DAT_8c038de8;
  *(undefined4 *)(in_GBR + 0x24) = *DAT_8c038dec;
  uVar1 = 0;
  if ((uVar2 & DAT_8c038df0) != 0) {
    uVar1 = 0xffffffff;
  }
  if ((uVar2 & DAT_8c038df4) != 0) {
    uVar1 = 1;
  }
  *(undefined4 *)(in_GBR + 0x28) = uVar1;
  (*(code *)PTR_FUN_8c038df8)();
  return;
}

