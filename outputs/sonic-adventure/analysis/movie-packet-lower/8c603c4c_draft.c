/* Ghidra draft only; not buildable/verified source. */

int FUN_8c603c4c(char *param_1,int param_2)

{
  int iVar1;
  char cVar2;
  int iVar3;
  
  iVar3 = 0;
  param_2 = param_2 << 6;
  do {
    if (param_2 < 1) {
      return iVar3;
    }
    cVar2 = *param_1;
    param_1 = param_1 + 1;
    iVar1 = 8;
    do {
      if (((int)cVar2 & 0x80U) != 0) {
        return iVar3;
      }
      iVar1 = iVar1 + -1;
      cVar2 = cVar2 << 1;
      iVar3 = iVar3 + 1;
    } while (0 < iVar1);
    param_2 = param_2 + -1;
  } while( true );
}

