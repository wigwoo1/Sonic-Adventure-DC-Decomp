/* Ghidra draft only; not buildable/verified source. */

void FUN_8c07b320(int param_1)

{
  int iVar1;
  
  iVar1 = (*(code *)PTR_FUN_8c07b4c4)(2,6,PTR_LAB_8c07b4c0);
  if (iVar1 != 0) {
    *(undefined1 *)(*(int *)(iVar1 + 0x20) + 8) = *(undefined1 *)(param_1 + 8);
  }
  return;
}

