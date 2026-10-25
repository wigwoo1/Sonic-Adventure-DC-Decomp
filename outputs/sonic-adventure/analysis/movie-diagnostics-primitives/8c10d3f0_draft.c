/* Ghidra draft only; not buildable/verified source. */

int FUN_8c10d3f0(char *param_1)

{
  char cVar1;
  int iVar2;
  
  iVar2 = 0;
  while (cVar1 = *param_1, param_1 = param_1 + 1, cVar1 != '\0') {
    iVar2 = iVar2 + 1;
  }
  return iVar2;
}

