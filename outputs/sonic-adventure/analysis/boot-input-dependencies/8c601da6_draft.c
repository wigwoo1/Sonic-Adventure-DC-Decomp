/* Ghidra draft only; not buildable/verified source. */

void FUN_8c601da6(undefined4 *param_1,uint *param_2)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 local_10 [2];
  
  if (((*param_2 & 1) == 0) || (iVar1 = (*(code *)PTR_FUN_8c601e48)(local_10,param_2), -1 < iVar1))
  {
    iVar1 = (*(code *)PTR_FUN_8c601e4c)(5);
    if (iVar1 == 1) {
      uVar2 = FUN_8c601d84(local_10[0]);
      param_1[1] = uVar2;
    }
    else {
      param_1[1] = local_10[0];
    }
    *param_1 = param_1[1];
    param_1[9] = param_2 + 5;
  }
  else {
    param_1[1] = 0;
    *param_1 = 0;
  }
  return;
}

