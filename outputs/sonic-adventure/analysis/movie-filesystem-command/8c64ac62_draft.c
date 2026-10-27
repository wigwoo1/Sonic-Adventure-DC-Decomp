/* Ghidra draft only; not buildable/verified source. */

int FUN_8c64ac62(char *param_1)

{
  int iVar1;
  int iVar2;
  
  iVar2 = 0;
  iVar1 = 0;
  do {
    if (*param_1 == '\0') {
      return iVar1;
    }
    iVar2 = iVar2 + 1;
    iVar1 = iVar1 + 1;
    param_1 = param_1 + 1;
  } while (iVar2 < 0xc);
  return iVar1;
}

