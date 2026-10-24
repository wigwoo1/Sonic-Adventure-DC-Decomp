/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c6453e2(char *param_1,char *param_2)

{
  char cVar1;
  char cVar2;
  
  do {
    if (*param_1 == '\0') {
      if (*param_2 != '\0') {
        return 0;
      }
      return 1;
    }
    cVar2 = *param_1;
    param_1 = param_1 + 1;
    cVar1 = FUN_8c645348((int)cVar2);
    cVar2 = *param_2;
    param_2 = param_2 + 1;
    cVar2 = FUN_8c645348((int)cVar2);
  } while (cVar1 == cVar2);
  return 0;
}

