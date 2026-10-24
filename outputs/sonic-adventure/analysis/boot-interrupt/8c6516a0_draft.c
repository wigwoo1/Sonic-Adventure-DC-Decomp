/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c6516a0(void)

{
  undefined4 *puVar1;
  uint uVar2;
  undefined *puVar3;
  undefined4 uVar4;
  undefined4 *puVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  undefined1 auStack_28 [4];
  uint local_24;
  
  uVar4 = DAT_8c6517f0;
  puVar3 = PTR_FUN_8c6517ec;
  uVar2 = DAT_8c6517e8;
  puVar1 = DAT_8c6517e4;
  iVar8 = (int)DAT_8c6517e0;
  local_24 = 0;
  while ((*(int *)(local_24 * 4 + DAT_8c6517f4) != 1 || (*DAT_8c6517f8 != 0))) {
    local_24 = local_24 + 1;
    if (3 < (int)local_24) {
      return 1;
    }
  }
  *DAT_8c6517f8 = 1;
  iVar9 = local_24 * 4;
  *(undefined4 *)(DAT_8c6517f4 + iVar9) = 2;
  *DAT_8c6517fc = local_24;
  puVar5 = DAT_8c651808;
  local_24 = local_24 & 1;
  *DAT_8c651804 = *DAT_8c651800;
  *puVar5 = 1;
  *puVar5 = 0;
  iVar6 = DAT_8c65180c;
  iVar7 = local_24 * 0x20;
  *DAT_8c651810 = *(undefined4 *)(DAT_8c65180c + iVar7);
  *DAT_8c651814 = *(undefined4 *)(iVar6 + 4 + iVar7);
  *DAT_8c651818 = *(undefined4 *)(iVar6 + 0xc + iVar7);
  *DAT_8c65181c = *(undefined4 *)(iVar6 + 0x10 + iVar7);
  *DAT_8c651820 = *(undefined4 *)(iVar6 + 8 + iVar7);
  *puVar1 = DAT_8c651824;
  *DAT_8c651828 = 0;
  *DAT_8c65182c = 0;
  iVar6 = DAT_8c651834;
  *DAT_8c651830 = 0;
  if (*(int *)(iVar6 + iVar9) != 0) {
    *DAT_8c651828 = *DAT_8c651828 | 1;
    iVar6 = (*(code *)puVar3)(uVar4,*(undefined4 *)(DAT_8c651838 + iVar9),
                              *(int *)(iVar6 + iVar9) + 0x20,0x10,auStack_28);
    if (iVar6 == iVar8) {
      return 0;
    }
  }
  if (*(int *)(DAT_8c65183c + iVar9) != 0) {
    *DAT_8c651828 = *DAT_8c651828 | 2;
    puVar1 = DAT_8c651844;
    if (*DAT_8c651840 == 1) {
      *(uint *)*DAT_8c651844 = *(uint *)*DAT_8c651844 & uVar2 | DAT_8c651848;
      iVar6 = (*(code *)puVar3)(uVar4,*puVar1,*DAT_8c65184c,0x10,auStack_28);
      if (iVar6 == iVar8) {
        return 0;
      }
    }
    iVar6 = (*(code *)puVar3)(uVar4,*(undefined4 *)(DAT_8c65183c + -0x50 + iVar9),
                              *(int *)(DAT_8c65183c + iVar9) + 0x20,0x10,auStack_28);
    if (iVar6 == iVar8) {
      return 0;
    }
  }
  iVar6 = DAT_8c651a40;
  if (*(int *)(DAT_8c651a40 + iVar9) != 0) {
    *DAT_8c651a44 = *DAT_8c651a44 | 4;
    iVar6 = (*(code *)puVar3)(uVar4,*(undefined4 *)(DAT_8c651a48 + iVar9),
                              *(int *)(iVar6 + iVar9) + 0x20,0x10,auStack_28);
    if (iVar6 == iVar8) {
      return 0;
    }
  }
  if (*(int *)(DAT_8c651a4c + iVar9) != 0) {
    *DAT_8c651a44 = *DAT_8c651a44 | 8;
    if (*DAT_8c651a50 == 1) {
      *(uint *)*DAT_8c651a54 = *(uint *)*DAT_8c651a54 & uVar2 | DAT_8c651a58;
      iVar6 = (*(code *)puVar3)(uVar4,*DAT_8c651a54,*DAT_8c651a5c,0x10,auStack_28);
      if (iVar6 == iVar8) {
        return 0;
      }
    }
    iVar6 = (*(code *)puVar3)(uVar4,*(undefined4 *)(DAT_8c651a4c + -0x50 + iVar9),
                              *(int *)(DAT_8c651a4c + iVar9) + 0x20,0x10,auStack_28);
    if (iVar6 == iVar8) {
      return 0;
    }
  }
  if (*(int *)(DAT_8c651a60 + iVar9) == 0) {
    return 1;
  }
  *DAT_8c651a44 = *DAT_8c651a44 | 0x10;
  iVar6 = (*(code *)puVar3)(uVar4,*(undefined4 *)(DAT_8c651a60 + -0x50 + iVar9),
                            *(int *)(DAT_8c651a60 + iVar9) + 0x20,0x10,auStack_28);
  if (iVar6 != iVar8) {
    return 1;
  }
  return 0;
}

