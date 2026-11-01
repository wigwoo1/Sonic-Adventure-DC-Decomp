/* Ghidra draft only; not buildable/verified source. */

int FUN_8c64ee32(int param_1)

{
  int iVar1;
  
  iVar1 = FUN_8c64ee8c(param_1);
  if (-1 < iVar1) {
    iVar1 = (*(code *)PTR_FUN_8c64ee88)(0,param_1);
    if (iVar1 < 0) {
      (*(code *)PTR_FUN_8c64ee74)(param_1 + 4);
    }
    else {
      iVar1 = 1;
    }
  }
  return iVar1;
}

