/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c645374(char *param_1,char *param_2,int param_3)

{
  char cVar1;
  char cVar2;
  
  while( true ) {
    if (param_3 < 1) {
      return 1;
    }
    if (*param_1 == '\0') break;
    cVar2 = *param_1;
    param_1 = param_1 + 1;
    cVar1 = FUN_8c645348((int)cVar2);
    cVar2 = *param_2;
    param_2 = param_2 + 1;
    cVar2 = FUN_8c645348((int)cVar2);
    if (cVar1 != cVar2) {
      return 0;
    }
    param_3 = param_3 + -1;
  }
  if (*param_2 == '\0') {
    return 1;
  }
  return 0;
}

