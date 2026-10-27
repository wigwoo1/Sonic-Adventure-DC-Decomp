/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c5ff39c(int *param_1)

{
  undefined *puVar1;
  int iVar2;
  int local_c [2];
  
  puVar1 = PTR_DAT_8c5ff4b4;
  if (param_1 != (int *)0x0) {
    *param_1 = 0;
    if ((*(int *)puVar1 != 0) && (iVar2 = (*(code *)PTR_FUN_8c5ff4bc)(0), iVar2 != -1)) {
      if (*DAT_8c5ff4c0 == -1) {
        FUN_8c5ff3fa(local_c);
        if (local_c[0] == -1) {
          *param_1 = 1;
        }
        else {
          iVar2 = FUN_8c5ff396(local_c[0]);
          *param_1 = iVar2;
        }
      }
      else {
        *param_1 = (int)*DAT_8c5ff4c0;
      }
      return 0;
    }
  }
  return 0xffffffff;
}

