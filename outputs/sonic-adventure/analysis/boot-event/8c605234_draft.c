/* Ghidra draft only; not buildable/verified source. */

void FUN_8c605234(int param_1)

{
  int *piVar1;
  int iVar2;
  
  piVar1 = DAT_8c605374;
  if (*DAT_8c605374 != 0) {
    (*(code *)PTR_FUN_8c605378)();
    *piVar1 = 0;
  }
  if (param_1 != 0) {
    iVar2 = (*(code *)PTR_FUN_8c60537c)((int)DAT_8c60536e,param_1,0x58,0);
    *piVar1 = iVar2;
  }
  return;
}

