/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c5ff9e6(char *param_1)

{
  char cVar1;
  int iVar2;
  char *pcVar3;
  
  iVar2 = 0;
  pcVar3 = PTR_DAT_8c5ffc08;
  do {
    param_1 = param_1 + 1;
    cVar1 = *pcVar3;
    pcVar3 = pcVar3 + 1;
    if (cVar1 != *param_1) {
      return 0;
    }
    iVar2 = iVar2 + 1;
  } while (iVar2 < 5);
  return 1;
}

