/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c05e6c4(int param_1)

{
  int iVar1;
  
  iVar1 = *(int *)(*(int *)(param_1 + 0x38) + 0x18);
  if (((iVar1 != 0) && (*(char *)(param_1 + 8) == '\0')) &&
     (*(char *)(*(int *)(*(int *)(iVar1 + 0x10) + 0x20) + 2) == '\b')) {
    return 1;
  }
  return 0;
}

