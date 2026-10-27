/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c650e40(void)

{
  ushort uVar1;
  int iVar2;
  uint *puVar3;
  int iVar4;
  undefined4 *puVar5;
  int iVar6;
  uint uVar7;
  uint *puVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  
  iVar4 = DAT_8c650f34;
  iVar2 = DAT_8c650f28;
  uVar1 = DAT_8c650f20;
  iVar6 = *DAT_8c650f2c * 0x100 + DAT_8c650f28 + *DAT_8c650f30 * 0x20;
  *(uint *)(iVar6 + 0x18) = *(uint *)(iVar6 + 0x18) & 0xfffffffc;
  puVar5 = DAT_8c650f38;
  uVar7 = 0;
  do {
    iVar6 = *(int *)(uVar7 * 4 + iVar4);
    if ((*(uint *)(uVar7 * 0x100 + iVar2 + iVar6 * 0x20 + 0x18) & 3) == 1) {
      iVar11 = uVar7 * 0x100;
      iVar10 = iVar6 * 0x20;
      iVar9 = iVar11 + iVar2 + iVar10;
      *(uint *)(iVar9 + 0x18) = *(uint *)(iVar9 + 0x18) & 0xfffffffc | 2;
      *puVar5 = *(undefined4 *)(iVar9 + 4);
      *DAT_8c650f40 = (*(int *)(iVar9 + 0xc) + DAT_8c650f3c) * 2;
      *DAT_8c650f44 = *(uint *)(iVar9 + 0x14) >> 5;
      if ((*(uint *)(iVar9 + 0x18) & 4) == 0) {
        uVar1 = DAT_8c650f22;
      }
      if ((*(uint *)(iVar11 + iVar2 + iVar10 + 0x18) & 0x10) != 0) {
        uVar1 = uVar1 | DAT_8c650f24;
      }
      *DAT_8c650f48 = (int)(short)uVar1;
      iVar10 = iVar11 + iVar2 + iVar10;
      *DAT_8c650f4c = *(undefined4 *)(iVar10 + 0xc);
      *DAT_8c650f50 = *(undefined4 *)(iVar10 + 0x14);
      *DAT_8c650f54 = 1;
      puVar3 = DAT_8c650f2c;
      puVar8 = (uint *)(uVar7 * 4 + iVar4);
      *DAT_8c650f30 = iVar6;
      *puVar3 = uVar7;
      *puVar8 = *puVar8 + 1 & 7;
      return 1;
    }
    uVar7 = uVar7 + 1;
  } while (uVar7 < 2);
  return 0;
}

