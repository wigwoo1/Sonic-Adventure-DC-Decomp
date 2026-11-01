/* Ghidra draft only; not buildable/verified source. */

uint FUN_8c07ce40(uint param_1)

{
  uint uVar1;
  int iVar2;
  
  iVar2 = *(int *)(DAT_8c07cf80 + (param_1 & 0xff) * 4);
  if (iVar2 == 0) {
    uVar1 = (*(code *)PTR_FUN_8c07cf84)();
    return uVar1;
  }
  return (uint)*(byte *)(iVar2 + 9);
}

