/* Ghidra draft only; not buildable/verified source. */

void FUN_8c040942(undefined4 *param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  uVar2 = *param_1;
  iVar1 = FUN_8c03fe1a(param_1[1],param_1[1]);
  if (iVar1 == 0) {
    FUN_8c03fe60(uVar2);
    (*(code *)PTR_FUN_8c0409b0)();
    FUN_8c040784(uVar2);
    (*(code *)PTR_FUN_8c0409b4)();
    return;
  }
  return;
}

