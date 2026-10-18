/* Ghidra draft only; not buildable/verified source. */

void callback_8c60d140(undefined4 param_1)

{
  int *piVar1;
  undefined4 *puVar2;
  int iVar3;
  
  puVar2 = DAT_8c60d254;
  piVar1 = DAT_8c60d250;
  iVar3 = *DAT_8c60d250;
  DAT_8c60d250[1] = iVar3;
  piVar1[2] = iVar3 + 0x10;
  piVar1[5] = 0;
  (*(code *)*puVar2)(param_1);
  (*(code *)*DAT_8c60d258)(param_1);
  (*(code *)*DAT_8c60d25c)(param_1);
  *piVar1 = iVar3 + 0x20;
  return;
}

