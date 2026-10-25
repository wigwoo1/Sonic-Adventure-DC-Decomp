/* Ghidra draft only; not buildable/verified source. */

void FUN_8c10e50e(char *param_1,int param_2,int *param_3)

{
  char cVar1;
  int iVar2;
  int iVar3;
  
  if (*param_3 == 1) {
    (*(code *)PTR_FUN_8c10e58c)(param_3[1],param_1,param_2);
    param_3[1] = param_3[1] + param_2;
    param_3[2] = param_3[2] + param_2;
  }
  else {
    iVar3 = 0;
    if (0 < param_2) {
      do {
        cVar1 = *param_1;
        param_1 = param_1 + 1;
        iVar2 = (*(code *)param_3[3])((int)cVar1,param_3[1]);
        if (iVar2 == -1) {
          return;
        }
        iVar3 = iVar3 + 1;
        param_3[2] = param_3[2] + 1;
      } while (iVar3 < param_2);
    }
  }
  return;
}

