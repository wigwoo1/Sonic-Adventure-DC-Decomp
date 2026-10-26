/* Ghidra draft only; not buildable/verified source. */

int FUN_8c6634a0(uint param_1,int *param_2,undefined4 *param_3)

{
  undefined *puVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  int iVar6;
  
  if ((int)param_1 < 0) {
    uVar2 = ~(~param_1 + 1 & 0x1f) + 1;
  }
  else {
    uVar2 = param_1 & 0x1f;
  }
  if (uVar2 != 0) {
    param_1 = param_1 + (0x20 - uVar2);
  }
  iVar3 = (*(code *)PTR_FUN_8c663524)(0,param_1 << 1,param_2,(int)DAT_8c663520,0);
  if (iVar3 == 0) {
    iVar6 = *param_2;
    iVar4 = (*(code *)PTR_FUN_8c663528)(iVar6);
    puVar1 = PTR_FUN_8c663528;
    *param_2 = iVar4;
    uVar5 = (*(code *)puVar1)(iVar6 + 4);
    *param_3 = uVar5;
  }
  return iVar3;
}

