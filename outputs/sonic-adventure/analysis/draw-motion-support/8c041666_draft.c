/* Ghidra draft only; not buildable/verified source. */

void callback_8c041666(undefined4 param_1)

{
  ushort *puVar1;
  int iVar2;
  int *piVar3;
  
  puVar1 = DAT_8c0418e4;
  piVar3 = (int *)((char)((char)*DAT_8c0418e4 * '\x1c') + DAT_8c0418e8);
  iVar2 = *piVar3;
  piVar3[1] = iVar2;
  piVar3[2] = iVar2 + 0xc;
  piVar3[5] = 0;
  (**(code **)((uint)*puVar1 * 4 + DAT_8c0418ec))(param_1);
  (**(code **)(DAT_8c0418f0 + (uint)*puVar1 * 4))(param_1);
  (**(code **)(DAT_8c0418f4 + (uint)*puVar1 * 4))(param_1);
  *piVar3 = iVar2 + 0x18;
  return;
}

