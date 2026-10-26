/* Ghidra draft only; not buildable/verified source. */

int FUN_8c6536ec(uint *param_1)

{
  int iVar1;
  
  iVar1 = FUN_8c653f08();
  if (iVar1 == 0) {
    if ((*param_1 & 0xffff) == 3) {
      iVar1 = (*(code *)PTR_FUN_8c65374c)(param_1);
    }
    else {
      iVar1 = (*(code *)PTR_FUN_8c653750)(param_1[5],param_1);
    }
    if (iVar1 == 3) {
      (*(code *)PTR_FUN_8c653754)(3);
    }
  }
  return iVar1;
}

