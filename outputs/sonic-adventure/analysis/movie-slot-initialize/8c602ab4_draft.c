/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c602ab4(undefined4 param_1)

{
  int *piVar1;
  int iVar2;
  short local_10;
  ushort local_e;
  short local_c;
  char local_a;
  char local_9;
  
  piVar1 = (int *)(*(code *)PTR_FUN_8c602b20)(param_1);
  iVar2 = (*(code *)PTR_FUN_8c602b24)(&local_10,piVar1);
  if ((((-1 < iVar2) && (*piVar1 == 0xe)) && (local_10 == 1)) &&
     ((((uint)local_e == (int)DAT_8c602b12 && (local_c == 1)) &&
      ((local_a == '\0' && (local_9 == '\0')))))) {
    return 1;
  }
  return 0;
}

