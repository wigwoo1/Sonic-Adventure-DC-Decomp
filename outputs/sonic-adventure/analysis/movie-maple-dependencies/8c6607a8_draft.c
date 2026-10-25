/* Ghidra draft only; not buildable/verified source. */

int FUN_8c6607a8(undefined4 *param_1,undefined4 param_2,int param_3)

{
  int iVar1;
  
  iVar1 = 0;
  if (0 < param_3) {
    do {
      iVar1 = iVar1 + 1;
      *param_1 = param_2;
      param_1 = param_1 + 1;
    } while (iVar1 < param_3);
  }
  return param_3;
}

