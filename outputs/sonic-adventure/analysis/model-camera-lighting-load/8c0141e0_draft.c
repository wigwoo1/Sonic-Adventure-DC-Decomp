/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c0141e0(undefined4 param_1)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  int local_10;
  undefined4 local_c;
  
  local_c = param_1;
  iVar1 = (*(code *)PTR_FUN_8c014380)(0);
  if (iVar1 == 0) {
    return 0;
  }
  (*(code *)PTR_FUN_8c014384)(iVar1,&local_10);
  iVar2 = (*(code *)PTR_FUN_8c014388)(0x800);
  uVar3 = (uint)(iVar2 != 0);
  local_10 = (*(code *)PTR_FUN_8c01438c)();
  local_10 = local_10 + uVar3;
  (*(code *)PTR_FUN_8c014390)(iVar1,local_10,local_c);
  (*(code *)PTR_FUN_8c014394)(iVar1);
  return 1;
}

