/* Ghidra draft only; not buildable/verified source. */

int FUN_8c64ed8a(int param_1,int *param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int local_2c;
  int local_28;
  
  local_28 = param_1;
  iVar1 = (*(code *)PTR_FUN_8c64ee78)(param_2,*(undefined4 *)(param_1 + 4),&local_2c);
  if (-1 < iVar1) {
    iVar1 = param_2[6];
    iVar2 = (*(code *)PTR_FUN_8c64ee7c)(iVar1);
    if (iVar2 == 0) {
      *DAT_8c64ee80 = 7;
      iVar1 = -7;
    }
    else {
      iVar3 = (local_2c - iVar1) + -8;
      iVar1 = (*(code *)PTR_FUN_8c64ee68)(iVar2,iVar1);
      if (iVar1 < 0) {
        iVar1 = -1;
      }
      else {
        if (0 < iVar3) {
          (*(code *)PTR_FUN_8c64ee84)(iVar3,1);
        }
        iVar1 = 1;
        *param_2 = iVar2;
      }
    }
  }
  return iVar1;
}

