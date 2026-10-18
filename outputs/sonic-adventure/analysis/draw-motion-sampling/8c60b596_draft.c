/* Ghidra draft only; not buildable/verified source. */

void callback_8c60b596(undefined4 param_1)

{
  int iVar1;
  ushort *puVar2;
  int *piVar3;
  int iVar4;
  
  puVar2 = DAT_8c60b7fc;
  iVar1 = DAT_8c60b7f8;
  piVar3 = (int *)((uint)*DAT_8c60b7fc * 0x1c + DAT_8c60b7f8);
  iVar4 = *piVar3;
  piVar3[1] = iVar4;
  *(int *)((uint)*puVar2 * 0x1c + iVar1 + 8) = iVar4 + 8;
  *(undefined4 *)((uint)*puVar2 * 0x1c + iVar1 + 0x14) = 0;
  (**(code **)(DAT_8c60b800 + (uint)*puVar2 * 4))(param_1);
  (**(code **)(DAT_8c60b804 + (uint)*puVar2 * 4))(param_1);
  (**(code **)(DAT_8c60b808 + (uint)*puVar2 * 4))(param_1);
  *(int *)((uint)*puVar2 * 0x1c + iVar1) = iVar4 + 0x10;
  return;
}

