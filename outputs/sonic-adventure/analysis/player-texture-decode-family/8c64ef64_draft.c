/* Ghidra draft only; not buildable/verified source. */

int FUN_8c64ef64(int *param_1)

{
  undefined *puVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  
  uVar3 = param_1[1];
  iVar2 = (*(code *)PTR_FUN_8c64f120)(param_1 + 1,param_1[4],param_1[5],uVar3 >> 0x10,&DAT_8c88f7a8)
  ;
  puVar1 = PTR_FUN_8c64f124;
  if (iVar2 == 1) {
    param_1[1] = param_1[1] | uVar3;
    iVar4 = param_1[6];
    iVar2 = (*(code *)puVar1)(iVar4);
    if (iVar2 == 0) {
      *DAT_8c64f118 = 7;
      iVar2 = -7;
    }
    else {
      (*(code *)PTR_FUN_8c64f128)(iVar2,*param_1,iVar4);
      *param_1 = iVar2;
    }
  }
  else {
    *DAT_8c64f118 = 4;
    iVar2 = -4;
  }
  return iVar2;
}

