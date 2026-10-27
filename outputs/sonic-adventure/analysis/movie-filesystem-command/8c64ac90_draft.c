/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c64ac90(char *param_1,char *param_2)

{
  int iVar1;
  int iVar2;
  
  iVar1 = FUN_8c64ac62(param_1);
  iVar2 = FUN_8c64ac62(param_2);
  if (iVar1 != iVar2) {
    return 0;
  }
  iVar2 = 0;
  if (0 < iVar1) {
    do {
      if (*param_1 != *param_2) {
        return 0;
      }
      iVar2 = iVar2 + 1;
      param_2 = param_2 + 1;
      param_1 = param_1 + 1;
    } while (iVar2 < iVar1);
  }
  return 1;
}

