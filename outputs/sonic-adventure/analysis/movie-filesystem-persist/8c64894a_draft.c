/* Ghidra draft only; not buildable/verified source. */

int FUN_8c64894a(int param_1,short param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar3 = param_1 * DAT_8c6489b0 + DAT_8c6489d0;
  iVar1 = *(int *)(iVar3 + 0x1c);
  iVar2 = *(int *)(iVar1 + 8) - *(int *)(iVar1 + 0x10);
  iVar1 = FUN_8c6489d4(param_1,(int)param_2,iVar2 - *(int *)(iVar1 + 0x18),iVar2 + -1);
  if ((iVar1 < 1) &&
     (iVar2 = *(int *)(iVar3 + 0x1c), iVar1 = *(int *)(iVar2 + 0x1c),
     iVar1 = FUN_8c6489d4(param_1,(int)param_2,iVar1,iVar1 + *(int *)(iVar2 + 0x20) + -1), iVar1 < 1
     )) {
    iVar1 = -1;
  }
  return iVar1;
}

