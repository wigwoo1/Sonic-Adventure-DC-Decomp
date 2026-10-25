/* Ghidra draft only; not buildable/verified source. */

int FUN_8c64541c(char *param_1,char *param_2)

{
  char *pcVar1;
  int iVar2;
  
  iVar2 = 0;
  do {
    pcVar1 = param_2;
    if (*param_1 == '\0') {
      return iVar2;
    }
    for (; *pcVar1 != '\0'; pcVar1 = pcVar1 + 1) {
      if (*param_1 == *pcVar1) {
        return iVar2;
      }
    }
    iVar2 = iVar2 + 1;
    param_1 = param_1 + 1;
  } while( true );
}

