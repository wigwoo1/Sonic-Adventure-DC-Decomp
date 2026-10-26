/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c653dc0(int *param_1,undefined4 *param_2)

{
  undefined4 uVar1;
  int local_1c;
  int local_18;
  int local_14;
  undefined4 local_10;
  undefined4 *local_c;
  int *local_8;
  
  local_c = param_2;
  local_8 = param_1;
  uVar1 = (*(code *)PTR_FUN_8c653ea0)(&local_1c,&local_18,&local_14,&local_10);
  *local_8 = local_1c + local_18 + local_14;
  *local_c = local_10;
  return uVar1;
}

