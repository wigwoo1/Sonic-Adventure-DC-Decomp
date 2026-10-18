/* Ghidra draft only; not buildable/verified source. */

void callback_8c60d19e(void)

{
  int *piVar1;
  undefined4 *puVar2;
  int iVar3;
  
  puVar2 = DAT_8c60d268;
  piVar1 = DAT_8c60d264;
  iVar3 = *DAT_8c60d264;
  DAT_8c60d264[1] = iVar3;
  piVar1[2] = iVar3 + 4;
  piVar1[5] = 0;
  (*(code *)*puVar2)();
  *piVar1 = iVar3 + 8;
  return;
}

