/* Ghidra draft only; not buildable/verified source. */

void FUN_8c647cc4(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                 undefined4 param_5,undefined4 param_6,undefined4 param_7)

{
  int iVar1;
  
  iVar1 = param_1 * 0x80 + DAT_8c647d10;
  *(undefined4 *)(iVar1 + 0x14) = param_2;
  *(undefined4 *)(iVar1 + 0x18) = param_3;
  *(undefined4 *)(iVar1 + 0x1c) = param_4;
  *(undefined4 *)(iVar1 + 0x20) = param_5;
  *(undefined4 *)(iVar1 + 0x24) = param_6;
  *(undefined4 *)(iVar1 + 0x28) = param_7;
  *(undefined4 *)(iVar1 + 4) = 1;
  return;
}

