/* Ghidra draft only; not buildable/verified source. */

int FUN_8c65a5dc(int param_1,undefined4 param_2)

{
  undefined *puVar1;
  int iVar2;
  int iVar3;
  undefined4 local_2c;
  undefined4 local_28;
  int local_24;
  undefined4 local_20;
  
  puVar1 = PTR_DAT_8c65a6b8;
  iVar3 = DAT_8c65a6bc;
  local_20 = param_2;
  if (((*(int *)PTR_DAT_8c65a6b8 != 0) && (iVar3 = DAT_8c65a6c0, param_1 != 0)) &&
     ((*(code *)PTR_FUN_8c65a6c4)(param_1,&local_24), iVar3 = DAT_8c65a6c8, local_24 != 2)) {
    iVar3 = (int)DAT_8c65a6b4;
    *DAT_8c65a6cc = 0;
    *(undefined4 *)(param_1 + 0x18) = 0;
    iVar2 = *(int *)(*(int *)puVar1 + iVar3);
    if (iVar2 == 0) {
      iVar3 = (*(code *)PTR_FUN_8c65a6d0)(param_1,local_20);
      if (iVar3 == 0) {
        return 0;
      }
    }
    else {
      iVar3 = 0;
      if (((iVar2 == 1) && (iVar3 = (*(code *)PTR_FUN_8c65a6d4)(param_1,local_20), iVar3 == 0)) &&
         (*(int *)((int)DAT_8c65a6b6 + *(int *)puVar1) == 0)) {
        (*(code *)PTR_FUN_8c65a6d8)(&local_2c,&local_28);
        iVar3 = (*(code *)PTR_FUN_8c65a6dc)(local_2c,local_28);
        puVar1 = PTR_FUN_8c65a6e0;
        if ((iVar3 == 0) && (*(int *)(param_1 + 0x10) == -1)) {
          while (*(int *)(param_1 + 0x18) != 1) {
            (*(code *)puVar1)();
          }
        }
      }
    }
  }
  if (iVar3 != 0) {
    (*(code *)PTR_FUN_8c65a72c)(PTR_FUN_8c65a728,iVar3);
  }
  return iVar3;
}

