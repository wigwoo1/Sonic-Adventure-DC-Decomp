/* Ghidra draft only; not buildable/verified source. */

int FUN_8c600f1e(int param_1,int param_2,uint param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  code *pcVar6;
  
  iVar1 = DAT_8c601000;
  pcVar6 = FUN_8c010b58;
  iVar2 = 0;
  iVar4 = (int)DAT_8c600ffe;
  for (iVar5 = 0; iVar5 < iVar4; iVar5 = iVar5 + 1) {
    if ((param_3 & 0x1f) == 0) {
      iVar2 = FUN_8c601084(param_1,param_2,param_3);
    }
    else {
      iVar2 = FUN_8c600fac(param_1,param_2,param_3,pcVar6);
    }
    if (((iVar2 != 0) || (param_2 == 0)) ||
       (iVar3 = iVar1, *(int *)(param_1 + 0x10) <= *(int *)(param_1 + 0x14))) break;
    for (; iVar3 != 0; iVar3 = iVar3 + -1) {
    }
  }
  if (iVar5 == iVar4) {
    iVar2 = -0x15;
  }
  else if (-1 < iVar2) {
    iVar2 = FUN_8c600cf0(param_1);
  }
  return iVar2;
}

