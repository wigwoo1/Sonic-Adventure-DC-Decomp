/* Ghidra draft only; not buildable/verified source. */

undefined4 thunk_FUN_8c09cc80(undefined4 param_1)

{
  int *piVar1;
  int iVar2;
  uint uVar3;
  int iStack_10;
  undefined4 uStack_c;
  
  piVar1 = DAT_8c09ce00;
  uStack_c = param_1;
  iVar2 = (*(code *)PTR_FUN_8c09ce04)(0);
  *piVar1 = iVar2;
  if (iVar2 == 0) {
    return 0xffffffff;
  }
  (*(code *)PTR_FUN_8c09ce08)(&iStack_10);
  iVar2 = (*(code *)PTR_FUN_8c09ce0c)(0x800);
  uVar3 = (uint)(iVar2 != 0);
  iStack_10 = (*(code *)PTR_FUN_8c09ce10)();
  iStack_10 = iStack_10 + uVar3;
  (*(code *)PTR_FUN_8c09ce14)(*piVar1,iStack_10,uStack_c);
  (*(code *)PTR_FUN_8c09ce18)(*piVar1);
  return 0;
}

