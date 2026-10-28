/* Ghidra draft only; not buildable/verified source. */

char * FUN_8c64b2a6(void)

{
  char *pcVar1;
  int iVar2;
  
  iVar2 = 0;
  pcVar1 = DAT_8c64b2f0;
  while( true ) {
    if (*DAT_8c64b2f4 <= iVar2) {
      *DAT_8c64b2f8 = (int)DAT_8c64b2e2;
      return (char *)0x0;
    }
    if (*pcVar1 == '\0') break;
    pcVar1 = pcVar1 + 0x48;
    iVar2 = iVar2 + 1;
  }
  *pcVar1 = '\x01';
  *(int *)(pcVar1 + 0xc) = iVar2;
  return pcVar1;
}

