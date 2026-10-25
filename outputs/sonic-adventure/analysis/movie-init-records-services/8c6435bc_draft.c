/* Ghidra draft only; not buildable/verified source. */

int FUN_8c6435bc(int *param_1)

{
  undefined *puVar1;
  int iVar2;
  
  puVar1 = PTR_DAT_8c643610;
  iVar2 = DAT_8c643614;
  if (((*(int *)PTR_DAT_8c643610 != 0) && (iVar2 = DAT_8c643618, param_1 != (int *)0x0)) &&
     (*param_1 == 1)) {
    iVar2 = (int)DAT_8c64360c;
    *param_1 = 0;
    *(int *)(iVar2 + *(int *)puVar1) = *(int *)(iVar2 + *(int *)puVar1) + -1;
    iVar2 = 0;
  }
  if (iVar2 != 0) {
    (*(code *)PTR_FUN_8c643620)(PTR_FUN_8c64361c,iVar2);
  }
  return iVar2;
}

