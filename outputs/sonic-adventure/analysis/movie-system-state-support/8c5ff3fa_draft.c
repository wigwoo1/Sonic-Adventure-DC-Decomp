/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c5ff3fa(int *param_1)

{
  int iVar1;
  
  if (param_1 != (int *)0x0) {
    *param_1 = 0;
    iVar1 = FUN_8c5ff494();
    if (iVar1 != 0) {
      *param_1 = (int)*(char *)(iVar1 + 0xb);
      return 0;
    }
  }
  return 0xffffffff;
}

