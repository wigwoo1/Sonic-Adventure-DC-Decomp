/* Ghidra draft only; not buildable/verified source. */

void callback_8c0406e0(undefined4 param_1)

{
  undefined4 *puVar1;
  int *piVar2;
  int iVar3;
  
  piVar2 = DAT_8c040848;
  puVar1 = DAT_8c040844;
  iVar3 = *DAT_8c040848;
  DAT_8c040848[1] = iVar3;
  piVar2[2] = iVar3 + 0x10;
  piVar2[5] = 0;
  (*(code *)*puVar1)(param_1);
  (*(code *)*DAT_8c04084c)(param_1);
  (*(code *)*DAT_8c040850)(param_1);
  *piVar2 = iVar3 + 0x20;
  return;
}

