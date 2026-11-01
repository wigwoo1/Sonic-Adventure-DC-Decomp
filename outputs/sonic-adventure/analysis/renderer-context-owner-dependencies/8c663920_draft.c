/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c663920(undefined4 param_1,undefined4 param_2)

{
  undefined4 uVar1;
  int iVar2;
  
  uVar1 = (*(code *)PTR_FUN_8c663a40)(param_1);
  iVar2 = FUN_8c66385a(uVar1,uVar1);
  uVar1 = 0;
  if (iVar2 != 0) {
    uVar1 = (*(code *)PTR_FUN_8c663a40)(param_2);
    uVar1 = FUN_8c66385a(uVar1,uVar1);
  }
  return uVar1;
}

