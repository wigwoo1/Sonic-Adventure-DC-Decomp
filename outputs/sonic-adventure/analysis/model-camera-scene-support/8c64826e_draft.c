/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c64826e(int param_1)

{
  int iVar1;
  int iVar2;
  
  iVar2 = param_1 * DAT_8c648360 + DAT_8c648368;
  iVar1 = (*(code *)PTR_FUN_8c64836c)();
  if ((iVar1 != 0) && (*(char *)(iVar2 + 8) != '\0')) {
    return 1;
  }
  return 0;
}

