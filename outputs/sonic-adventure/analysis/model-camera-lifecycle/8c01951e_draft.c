/* Ghidra draft only; not buildable/verified source. */

void FUN_8c01951e(void)

{
  undefined *puVar1;
  undefined4 *puVar2;
  int *piVar3;
  
  piVar3 = DAT_8c01966c;
  puVar2 = DAT_8c019668;
  puVar1 = PTR_DAT_8c019664;
  *(undefined1 *)(*(int *)PTR_DAT_8c019664 + 8) = 0;
  *(undefined2 *)(*(int *)puVar1 + 4) = 0;
  *(undefined1 *)(*(int *)puVar1 + 6) = *(undefined1 *)*puVar2;
  *(undefined4 *)(*(int *)puVar1 + 0xc) =
       *(undefined4 *)(*(char *)(*(int *)puVar1 + 6) * 0x10 + *piVar3);
  *(undefined1 *)(*(int *)puVar1 + 9) = 0;
  *(undefined4 *)(*(int *)puVar1 + 0x10) = *(undefined4 *)(*DAT_8c019670 + 4);
  *(undefined4 *)(*(int *)PTR_DAT_8c019674 + 0x14) = 0;
  *(undefined1 *)(*(int *)puVar1 + 7) =
       *(undefined1 *)(*(char *)(*(int *)puVar1 + 6) * 0x10 + *piVar3 + 5);
  **(undefined4 **)puVar1 = 1;
  *(undefined1 *)(*(int *)puVar1 + 0xb) = 2;
  return;
}

