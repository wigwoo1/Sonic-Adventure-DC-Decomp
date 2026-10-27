/* Ghidra draft only; not buildable/verified source. */

int FUN_8c64a9a8(int param_1)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  uint uVar7;
  undefined4 uVar8;
  
  uVar1 = DAT_8c64aa88;
  uVar8 = 1;
  iVar6 = 0;
  iVar2 = param_1 * DAT_8c64aa80 + DAT_8c64aa84;
  iVar4 = (int)DAT_8c64aa82;
  iVar3 = *(int *)(iVar2 + 0x1c);
  iVar5 = *(int *)(iVar3 + 0x18);
  uVar7 = *(uint *)(iVar3 + 0x14);
  iVar2 = *(int *)(iVar2 + 0x28);
  if (0 < iVar5) {
    do {
      iVar3 = (*(code *)PTR_FUN_8c64aa8c)(param_1,uVar7 & 0xffff,iVar2,uVar8);
      if (iVar3 < 0) {
        return iVar3;
      }
      uVar7 = FUN_8c64b1c0(param_1,uVar7 & 0xffff);
      if ((uVar7 & 0xffff) == uVar1) {
        return 0;
      }
      iVar6 = iVar6 + 1;
      iVar2 = iVar2 + iVar4;
    } while (iVar6 < iVar5);
  }
  return 0;
}

