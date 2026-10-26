/* Ghidra draft only; not buildable/verified source. */

void FUN_8c665466(int *param_1,int param_2)

{
  int iVar1;
  
  if (*param_1 == 0) {
    iVar1 = 0;
  }
  else {
    iVar1 = *param_1;
  }
  *(int *)(param_2 + 4) = iVar1;
  *param_1 = param_2;
  return;
}

