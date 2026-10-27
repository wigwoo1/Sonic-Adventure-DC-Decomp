/* Ghidra draft only; not buildable/verified source. */

void FUN_8c64b472(short *param_1,undefined1 *param_2)

{
  undefined1 uVar1;
  short sVar2;
  short sVar3;
  
  sVar2 = FUN_8c64b3b4(*param_2);
  sVar3 = FUN_8c64b3b4(param_2[1]);
  *param_1 = sVar2 * 100 + sVar3;
  uVar1 = FUN_8c64b3b4(param_2[2]);
  *(undefined1 *)(param_1 + 1) = uVar1;
  uVar1 = FUN_8c64b3b4(param_2[3]);
  *(undefined1 *)((int)param_1 + 3) = uVar1;
  uVar1 = FUN_8c64b3b4(param_2[4]);
  *(undefined1 *)(param_1 + 2) = uVar1;
  uVar1 = FUN_8c64b3b4(param_2[5]);
  *(undefined1 *)((int)param_1 + 5) = uVar1;
  uVar1 = FUN_8c64b3b4(param_2[6]);
  *(undefined1 *)(param_1 + 3) = uVar1;
  *(undefined *)((int)param_1 + 7) = PTR_DAT_8c64b4e0[(byte)param_2[7] & 7];
  return;
}

