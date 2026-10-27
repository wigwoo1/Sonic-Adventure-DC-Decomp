/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c6469f4(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 uVar1;
  
  uVar1 = (*(code *)PTR_FUN_8c646ad0)(param_1,param_2,param_3);
  if (*(int *)((int)DAT_8c646ac6 + *DAT_8c646acc) != 0) {
    FUN_8c646654();
  }
  return uVar1;
}

