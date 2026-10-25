/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c60041a(int *param_1)

{
  int iVar1;
  int *piVar2;
  
  iVar1 = (*(code *)PTR_FUN_8c600480)(*(undefined4 *)(*(int *)PTR_DAT_8c600478 + 0x10),0);
  if (iVar1 == 0) {
    return 0xfffffff8;
  }
  piVar2 = (int *)(*(int *)(*(int *)PTR_DAT_8c600478 + 0x10) + 0x10 + (iVar1 + -1) * 0x2c);
  *param_1 = *piVar2 + (int)DAT_8c600470;
  param_1[1] = piVar2[1];
  *(char *)(param_1 + 2) = (char)piVar2[2];
  return 0;
}

