/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c10c07e(void)

{
  int iVar1;
  int iVar2;
  
  iVar1 = (*(code *)PTR_FUN_8c10c0cc)();
  iVar2 = FUN_8c10c044();
  if (iVar1 == 0) {
    return 1;
  }
  if ((iVar2 != 1) && (iVar2 != 3)) {
    return 0;
  }
  return 2;
}

