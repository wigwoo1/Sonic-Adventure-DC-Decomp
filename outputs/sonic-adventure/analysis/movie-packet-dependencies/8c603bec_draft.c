/* Ghidra draft only; not buildable/verified source. */

undefined4 thunk_FUN_8c603bfc(char *param_1,char *param_2,int param_3)

{
  char cVar1;
  char cVar2;
  bool bVar3;
  
  do {
    bVar3 = param_3 < 1;
    param_3 = param_3 + -1;
    if (bVar3) {
      return 1;
    }
    cVar1 = *param_2;
    param_2 = param_2 + 1;
    cVar2 = *param_1;
    param_1 = param_1 + 1;
  } while (cVar2 == cVar1);
  return 0;
}

