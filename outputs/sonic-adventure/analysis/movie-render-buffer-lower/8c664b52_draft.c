/* Ghidra draft only; not buildable/verified source. */

void FUN_8c664b52(undefined4 *param_1,undefined4 *param_2,int param_3)

{
  int iVar1;
  
  iVar1 = *(int *)(param_3 + 8);
  if (*(int *)(param_3 + 4) == 0) {
    *param_1 = *(undefined4 *)(param_3 + 8);
  }
  else {
    *(undefined4 *)(*(int *)(param_3 + 4) + 8) = *(undefined4 *)(param_3 + 8);
  }
  if (*(int *)(param_3 + 8) == 0) {
    *param_2 = *(undefined4 *)(param_3 + 4);
  }
  else {
    *(undefined4 *)(iVar1 + 4) = *(undefined4 *)(param_3 + 4);
  }
  return;
}

