/* Ghidra draft only; not buildable/verified source. */

void FUN_8c078e40(int param_1,undefined8 *param_2)

{
  int iVar1;
  undefined4 in_PR;
  char in_FPSCR_SZ;
  undefined4 uStack_14;
  undefined4 uStack_10;
  undefined4 uStack_c;
  undefined4 local_8;
  
  if (in_FPSCR_SZ == '\0') {
    uStack_14 = *(undefined4 *)param_2;
    uStack_10 = *(undefined4 *)((int)param_2 + 4);
    uStack_c = *(undefined4 *)(param_2 + 1);
    local_8 = in_PR;
  }
  else {
    uStack_14 = (undefined4)*param_2;
    uStack_10 = (undefined4)param_2[1];
    uStack_c = (undefined4)param_2[2];
    local_8 = (undefined4)((ulonglong)param_2[2] >> 0x20);
  }
  (*(code *)PTR_FUN_8c078fc0)();
  if (*(int *)(param_1 + 0x18) != 0) {
    (*(code *)PTR_FUN_8c078fc4)(0,*(int *)(param_1 + 0x18));
  }
  iVar1 = -*(int *)(param_1 + 0x14);
  if (iVar1 != 0) {
    (*(code *)PTR_FUN_8c078fc8)(0,iVar1,iVar1);
  }
  iVar1 = -*(int *)(param_1 + 0x1c);
  if (iVar1 != 0) {
    (*(code *)PTR_FUN_8c078fcc)(0,iVar1,iVar1);
  }
  (*(code *)PTR_FUN_8c078fd0)(0,&uStack_14,param_2);
  (*(code *)PTR_FUN_8c078fd4)(1);
  return;
}

