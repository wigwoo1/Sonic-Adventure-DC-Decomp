/* Ghidra draft only; not buildable/verified source. */

void callback_8c600240(void)

{
  undefined *puVar1;
  undefined *puVar2;
  undefined *puVar3;
  int iVar4;
  int iVar5;
  
  if (*(int *)PTR_DAT_8c600344 != 0) {
    (*(code *)PTR_FUN_8c600348)();
  }
  if (*(int *)PTR_DAT_8c60034c != 0) {
    (*(code *)PTR_FUN_8c600348)();
  }
  puVar3 = PTR_DAT_8c600354;
  puVar2 = PTR_FUN_8c600350;
  puVar1 = PTR_DAT_8c60034c;
  iVar4 = (int)DAT_8c60033e;
  *(undefined4 *)PTR_DAT_8c600344 = 0;
  *(undefined4 *)puVar1 = 0;
  iVar4 = *(int *)puVar3 + iVar4;
  for (iVar5 = 0; iVar5 < **(int **)puVar3 + 1; iVar5 = iVar5 + 1) {
    (*(code *)puVar2)(iVar4);
    iVar4 = iVar4 + 0x54;
  }
  *(undefined4 *)puVar3 = 0;
  return;
}

