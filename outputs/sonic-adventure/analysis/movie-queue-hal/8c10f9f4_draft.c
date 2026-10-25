/* Ghidra draft only; not buildable/verified source. */

undefined4
FUN_8c10f9f4(int param_1,uint param_2,uint param_3,uint param_4,uint param_5,int param_6,int param_7
            ,undefined4 param_8,undefined4 param_9)

{
  uint uVar1;
  uint uVar2;
  
  uVar2 = DAT_8c10fa5c;
  uVar1 = DAT_8c10fa58;
  param_3 = param_3 & DAT_8c10fa54;
  *(uint *)(param_1 + 4) = param_2 & DAT_8c10fa54;
  *(uint *)(param_1 + 8) = param_3;
  *(uint *)(param_1 + 0xc) = param_4 & uVar1 | uVar2;
  *(uint *)(param_1 + 0x10) = param_5 & 1;
  if (param_6 == 1) {
    *(uint *)(param_1 + 0x14) = *(uint *)(param_1 + 0x14) | 1;
  }
  else {
    *(uint *)(param_1 + 0x14) = *(uint *)(param_1 + 0x14) & 0xfffffffd;
  }
  if (param_7 == 1) {
    *(uint *)(param_1 + 0x14) = *(uint *)(param_1 + 0x14) | 4;
  }
  else {
    *(uint *)(param_1 + 0x14) = *(uint *)(param_1 + 0x14) & 0xfffffffe;
  }
  *(uint *)(param_1 + 0x14) = *(uint *)(param_1 + 0x14) & 7;
  *(undefined4 *)(param_1 + 0x1c) = param_8;
  *(undefined4 *)(param_1 + 0x20) = param_9;
  *(undefined4 *)(param_1 + 0x30) = 0;
  return 0;
}

