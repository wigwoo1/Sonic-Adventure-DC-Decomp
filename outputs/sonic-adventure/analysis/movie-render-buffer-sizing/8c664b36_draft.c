/* Ghidra draft only; not buildable/verified source. */

void FUN_8c664b36(int *param_1,int param_2,int param_3)

{
  undefined4 uVar1;
  
  if (*(int *)(param_3 + 8) == 0) {
    *param_1 = param_2;
    *(int *)(param_2 + 4) = param_3;
    uVar1 = 0;
  }
  else {
    *(int *)(*(int *)(param_3 + 8) + 4) = param_2;
    *(int *)(param_2 + 4) = param_3;
    uVar1 = *(undefined4 *)(param_3 + 8);
  }
  *(undefined4 *)(param_2 + 8) = uVar1;
  *(int *)(param_3 + 8) = param_2;
  return;
}

