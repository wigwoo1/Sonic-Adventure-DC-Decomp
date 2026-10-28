/* Ghidra draft only; not buildable/verified source. */

int FUN_8c64aecc(int param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  
  iVar1 = DAT_8c64afd0;
  iVar6 = 0;
  iVar5 = param_1 * DAT_8c64afc8 + DAT_8c64afcc;
  iVar4 = *(int *)(*(int *)(iVar5 + 0x1c) + 0x10);
  iVar3 = DAT_8c64afd0 + 5;
  if (0 < iVar4) {
    do {
      iVar2 = (*(code *)PTR_FUN_8c64afd4)
                        (param_1,*(int *)(*(int *)(iVar5 + 0x1c) + 0xc) + iVar6,
                         iVar6 * 0x200 + *(int *)(iVar5 + 0x24));
      if (iVar2 < 0) {
        FUN_8c64b18e(param_1,*(int *)(*(int *)(iVar5 + 0x1c) + 0xc) + iVar6,iVar3);
        return iVar2;
      }
      FUN_8c64b18e(param_1,*(int *)(*(int *)(iVar5 + 0x1c) + 0xc) + iVar6,iVar1);
      iVar6 = iVar6 + 1;
    } while (iVar6 < iVar4);
  }
  return 0;
}

