/* Ghidra draft only; not buildable/verified source. */

undefined4 thunk_FUN_8c6453cc(char *param_1,char *param_2)

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
    cVar1 = *param_2;
    param_2 = param_2 + 1;
    cVar2 = *param_1;
    param_1 = param_1 + 1;
  } while (cVar2 == cVar1);
  return 0;
}

