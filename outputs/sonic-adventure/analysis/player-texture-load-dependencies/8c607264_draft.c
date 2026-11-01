/* Ghidra draft only; not buildable/verified source. */

void FUN_8c607264(int *param_1,int *param_2,int param_3,int param_4)

{
  int *piVar1;
  int iVar2;
  
  iVar2 = 0;
  piVar1 = param_2;
  if (0 < param_4) {
    do {
      iVar2 = iVar2 + 1;
      *piVar1 = param_3;
      param_3 = param_3 + 0x1c;
      piVar1 = piVar1 + 3;
    } while (iVar2 < param_4);
  }
  *param_1 = (int)param_2;
  return;
}

