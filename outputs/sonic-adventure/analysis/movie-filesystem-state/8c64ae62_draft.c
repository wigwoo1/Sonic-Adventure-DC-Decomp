/* Ghidra draft only; not buildable/verified source. */

int FUN_8c64ae62(int param_1)

{
  undefined *puVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  undefined4 uVar6;
  
  puVar1 = PTR_FUN_8c64aec4;
  uVar6 = 1;
  iVar4 = param_1 * DAT_8c64aeba + DAT_8c64aec0;
  iVar3 = *(int *)(*(int *)(iVar4 + 0x1c) + 0x10);
  iVar5 = 0;
  if (0 < iVar3) {
    do {
      iVar2 = (*(code *)puVar1)(param_1,*(int *)(*(int *)(iVar4 + 0x1c) + 0xc) + iVar5,
                                iVar5 * 0x200 + *(int *)(iVar4 + 0x24),uVar6);
      if (iVar2 < 0) {
        return iVar2;
      }
      iVar5 = iVar5 + 1;
    } while (iVar5 < iVar3);
  }
  return 0;
}

