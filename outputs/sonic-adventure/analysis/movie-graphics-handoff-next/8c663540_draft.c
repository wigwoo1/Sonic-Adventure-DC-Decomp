/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c663540(undefined4 param_1,undefined4 param_2)

{
  undefined4 uVar1;
  int local_14;
  int local_10 [2];
  
  if ((*DAT_8c663640 & 4) == 0) {
    uVar1 = (*(code *)PTR_FUN_8c663644)(1,param_1,param_2);
  }
  else {
    FUN_8c663c86(&local_14,local_10);
    uVar1 = (*(code *)PTR_FUN_8c663644)(local_14 < local_10[0],param_1,param_2);
  }
  return uVar1;
}

