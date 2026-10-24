/* Ghidra draft only; not buildable/verified source. */

void FUN_8c09ce40(void)

{
  int iVar1;
  
  if ((*DAT_8c09cec0 != 0) && (*(int *)PTR_DAT_8c09cec4 == 0)) {
    (*(code *)PTR_FUN_8c09cec8)();
    (*(code *)PTR_FUN_8c09cecc)();
    return;
  }
  iVar1 = (*(code *)PTR_FUN_8c09ced0)();
  if ((iVar1 != 6) && (iVar1 != 0)) {
    return;
  }
  (*(code *)PTR_FUN_8c09ced4)();
  return;
}

