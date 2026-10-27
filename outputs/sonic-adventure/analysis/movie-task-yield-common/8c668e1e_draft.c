/* Ghidra draft only; not buildable/verified source. */

void callback_8c668e1e(int param_1)

{
  int *piVar1;
  undefined *puVar2;
  int iVar3;
  int iVar4;
  
  puVar2 = PTR_FUN_8c668eac;
  piVar1 = DAT_8c668ea4;
  iVar4 = DAT_8c668ea8;
  if (-1 < param_1) {
    iVar4 = *(int *)((int)DAT_8c668e9e + *DAT_8c668ea4) + param_1;
  }
  iVar3 = (int)DAT_8c668ea0;
  *(int *)(*(int *)((int)DAT_8c668ea0 + *DAT_8c668ea4) + 0x14) = iVar4;
  iVar4 = (*(code *)puVar2)(*(int *)(iVar3 + *piVar1) + 0x18);
  if (iVar4 == 0) {
    (*(code *)PTR_FUN_8c668eb0)(*piVar1 + 4,1);
  }
  return;
}

