/* Ghidra draft only; not buildable/verified source. */

void FUN_8c07c9d2(uint param_1,undefined4 param_2,int param_3,undefined4 param_4)

{
  int iVar1;
  int iVar2;
  
  iVar1 = (param_1 & 0xff) * 4;
  iVar2 = *(int *)(DAT_8c07cb00 + iVar1);
  iVar1 = *(int *)(DAT_8c07cb04 + iVar1);
  *(undefined4 *)(iVar2 + 0x14) = param_2;
  *(int *)(iVar2 + 0x18) = 0x4000 - param_3;
  *(int *)(iVar1 + 0x20) = 0x4000 - param_3;
  *(undefined4 *)(iVar2 + 0x1c) = param_4;
  return;
}

