/* Ghidra draft only; not buildable/verified source. */

int FUN_8c64ae04(int param_1)

{
  int iVar1;
  char *pcVar2;
  int iVar3;
  
  iVar3 = param_1 * DAT_8c64aeba + DAT_8c64aec0;
  pcVar2 = *(char **)(iVar3 + 0x28);
  iVar1 = 0;
  while( true ) {
    if (*(int *)(*(int *)(iVar3 + 0x1c) + 0x30) <= iVar1) {
      return -1;
    }
    if (*pcVar2 == '\0') break;
    pcVar2 = pcVar2 + 0x20;
    iVar1 = iVar1 + 1;
  }
  return iVar1;
}

