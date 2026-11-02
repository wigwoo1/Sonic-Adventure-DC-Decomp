/* Ghidra draft only; not buildable/verified source. */

void FUN_8c01b3e0(undefined1 param_1)

{
  undefined *puVar1;
  int *piVar2;
  undefined *puVar3;
  
  piVar2 = DAT_8c01b4e4;
  puVar1 = PTR_DAT_8c01b4e0;
  *(undefined1 *)(*(int *)PTR_DAT_8c01b4e0 + 9) = param_1;
  puVar3 = PTR_DAT_8c01b4e8;
  *(undefined4 *)(*(int *)puVar1 + 0x10) =
       *(undefined4 *)(*(char *)(*(int *)puVar1 + 9) * 8 + *piVar2 + 4);
  *(undefined4 *)(*(int *)puVar3 + 0x14) = 0;
  return;
}

