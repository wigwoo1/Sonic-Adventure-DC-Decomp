/* Ghidra draft only; not buildable/verified source. */

void FUN_8c64b3ec(undefined1 *param_1,int param_2)

{
  undefined1 uVar1;
  undefined4 uVar2;
  
  uVar2 = (*(code *)PTR_FUN_8c64b4d4)();
  uVar1 = FUN_8c64b390(uVar2);
  *param_1 = uVar1;
  uVar2 = (*(code *)PTR_FUN_8c64b4d8)();
  uVar1 = FUN_8c64b390(uVar2);
  param_1[1] = uVar1;
  uVar1 = FUN_8c64b390(*(undefined1 *)(param_2 + 2));
  param_1[2] = uVar1;
  uVar1 = FUN_8c64b390(*(undefined1 *)(param_2 + 3));
  param_1[3] = uVar1;
  uVar1 = FUN_8c64b390(*(undefined1 *)(param_2 + 4));
  param_1[4] = uVar1;
  uVar1 = FUN_8c64b390(*(undefined1 *)(param_2 + 5));
  param_1[5] = uVar1;
  uVar1 = FUN_8c64b390(*(undefined1 *)(param_2 + 6));
  param_1[6] = uVar1;
  param_1[7] = PTR_DAT_8c64b4dc[*(byte *)(param_2 + 7) & 7];
  return;
}

