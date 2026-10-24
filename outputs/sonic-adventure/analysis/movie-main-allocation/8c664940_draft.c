/* Ghidra draft only; not buildable/verified source. */

int FUN_8c664940(int param_1,short *param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  if (param_1 < *DAT_8c664a80) {
    *param_2 = 0;
  }
  else {
    *param_2 = 1;
  }
  iVar3 = *(int *)(PTR_DAT_8c664a84 + *param_2 * 0x14);
  iVar2 = 0;
  while (iVar1 = iVar3, iVar1 != 0) {
    if (*(int *)(iVar1 + 0xc) == param_1) {
      iVar3 = 0;
      iVar2 = iVar1;
    }
    else {
      iVar3 = *(int *)(iVar1 + 8);
    }
  }
  return iVar2;
}

