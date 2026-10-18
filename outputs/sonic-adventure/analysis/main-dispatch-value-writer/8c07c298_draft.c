/* Ghidra draft only; not buildable/verified source. */

void FUN_8c07c298(byte param_1,undefined4 param_2)

{
  int *piVar1;
  int iVar2;
  
  piVar1 = (int *)((uint)param_1 * 4 + DAT_8c07c3a0);
  iVar2 = *piVar1;
  FUN_8c07c9d2((int)(char)param_1,*(undefined4 *)(iVar2 + 0x14),param_2,
               *(undefined4 *)(iVar2 + 0x1c));
  *(undefined1 *)(*(int *)(*piVar1 + 0x3c) + 0x24) = 0;
  return;
}

