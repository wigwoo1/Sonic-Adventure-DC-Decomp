/* Ghidra draft only; not buildable/verified source. */

void FUN_8c668cca(int param_1,int param_2)

{
  int iVar1;
  
  *(int *)(DAT_8c668d70 + param_1) = *(int *)(DAT_8c668d70 + param_1) + -1;
  iVar1 = (int)DAT_8c668d6a;
  *(undefined4 *)(param_2 + 0xc) = 0;
  if (*(int *)(iVar1 + param_2) != 0) {
                    /* WARNING: Could not recover jumptable at 0x8c668ce4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(iVar1 + param_2))();
    return;
  }
  return;
}

