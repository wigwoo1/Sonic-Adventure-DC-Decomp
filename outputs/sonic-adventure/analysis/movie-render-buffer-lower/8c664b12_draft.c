/* Ghidra draft only; not buildable/verified source. */

void FUN_8c664b12(int *param_1,int *param_2,int param_3)

{
  int iVar1;
  
  if (*param_1 == 0) {
    *(undefined4 *)(param_3 + 4) = 0;
    *(undefined4 *)(param_3 + 8) = 0;
    *param_1 = param_3;
    *param_2 = param_3;
  }
  else {
    iVar1 = *param_1;
    *(undefined4 *)(param_3 + 4) = *(undefined4 *)(iVar1 + 4);
    *(int *)(param_3 + 8) = *param_1;
    *(int *)(iVar1 + 4) = param_3;
    *param_1 = param_3;
  }
  return;
}

