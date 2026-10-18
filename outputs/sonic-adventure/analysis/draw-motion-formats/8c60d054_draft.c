/* Ghidra draft only; not buildable/verified source. */

void callback_8c60d054(undefined4 param_1)

{
  undefined4 *puVar1;
  int *piVar2;
  int iVar3;
  
  piVar2 = DAT_8c60d13c;
  puVar1 = DAT_8c60d130;
  iVar3 = *DAT_8c60d13c;
  DAT_8c60d13c[1] = iVar3;
  piVar2[2] = iVar3 + 4;
  piVar2[5] = 0;
  (*(code *)*puVar1)(param_1);
  (*(code *)*DAT_8c60d134)(param_1);
  (*(code *)*DAT_8c60d138)(param_1);
  *piVar2 = iVar3 + 8;
  return;
}

