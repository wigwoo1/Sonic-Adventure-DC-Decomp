/* Ghidra draft only; not buildable/verified source. */

int FUN_8c643624(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                undefined4 param_5)

{
  int iVar1;
  int local_1c;
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  
  iVar1 = DAT_8c6436a8;
  local_18 = param_3;
  local_14 = param_4;
  local_10 = param_2;
  if (((*(int *)PTR_DAT_8c6436a4 != 0) && (iVar1 = DAT_8c6436ac, param_1 != 0)) &&
     ((*(code *)PTR_FUN_8c6436b0)(param_1,&local_1c), iVar1 = DAT_8c6436b4, local_1c != 2)) {
    (*(code *)PTR_FUN_8c6436b8)(param_1,0);
    (*(code *)PTR_FUN_8c6436bc)(param_1,local_10);
    (*(code *)PTR_FUN_8c6436c0)(param_1,local_18);
    (*(code *)PTR_FUN_8c6436c4)(param_1,local_14,param_5);
    (*(code *)PTR_FUN_8c6436b8)(param_1,0);
    iVar1 = 0;
  }
  if (iVar1 != 0) {
    (*(code *)PTR_FUN_8c6436cc)(PTR_FUN_8c6436c8,iVar1);
  }
  return iVar1;
}

