/* Ghidra draft only; not buildable/verified source. */

void FUN_8c602cb8(void)

{
  uint *puVar1;
  undefined *puVar2;
  uint *puVar3;
  uint *puVar4;
  uint uVar5;
  int iVar6;
  uint uVar7;
  code *pcVar8;
  uint uVar9;
  uint uVar10;
  int iVar11;
  
  if (*(int *)PTR_DAT_8c602f00 != 0) {
    pcVar8 = (code *)PTR_LAB_8c602f0c;
    if (*DAT_8c602f04 != 0) {
      pcVar8 = (code *)PTR_FUN_8c602f08;
    }
    (*(code *)PTR_thunk_FUN_8c66085a_8c602f10)();
    puVar2 = PTR_FUN_8c602f1c;
    puVar1 = DAT_8c602f14;
    *DAT_8c602f18 = *DAT_8c602f14;
    uVar5 = (*(code *)puVar2)();
    uVar9 = 0;
    *puVar1 = uVar5;
    iVar11 = 0;
    uVar5 = 1;
    do {
      iVar6 = FUN_8c6033e4(iVar11);
      puVar3 = DAT_8c602f20;
      uVar7 = uVar5;
      if (iVar6 != 0) {
        uVar7 = 0;
      }
      iVar11 = iVar11 + 1;
      uVar5 = uVar5 << 1;
      uVar9 = uVar9 | uVar7;
    } while (iVar11 < 8);
    uVar10 = 1;
    uVar7 = *puVar1;
    iVar11 = 0;
    uVar5 = *DAT_8c602f18;
    do {
      if (((uVar5 ^ uVar7) & uVar7 & uVar10) != 0) {
        if ((uVar10 & uVar9) == 0) {
          *puVar3 = *puVar3 | uVar10;
        }
        else {
          (*pcVar8)(iVar11);
          *puVar3 = *puVar3 & ~uVar10;
        }
      }
      if (((uVar10 & uVar9) != 0) && ((*puVar3 & uVar10) != 0)) {
        (*pcVar8)(iVar11);
        *puVar3 = *puVar3 & ~uVar10;
      }
      puVar4 = DAT_8c602f28;
      puVar2 = PTR_FUN_8c602f24;
      iVar11 = iVar11 + 1;
      uVar10 = uVar10 << 1;
    } while (iVar11 < 8);
    uVar10 = 1;
    uVar5 = *puVar1;
    iVar11 = 0;
    uVar7 = *DAT_8c602f18;
    do {
      if (((uVar5 ^ uVar7) & uVar7 & uVar10) != 0) {
        if ((uVar10 & uVar9) == 0) {
          *puVar4 = *puVar4 | uVar10;
        }
        else {
          (*(code *)puVar2)(iVar11,2,0);
          *puVar4 = *puVar4 & ~uVar10;
        }
      }
      if (((uVar10 & uVar9) != 0) && ((*puVar4 & uVar10) != 0)) {
        (*(code *)puVar2)(iVar11,2,0);
        *puVar4 = *puVar4 & ~uVar10;
      }
      iVar11 = iVar11 + 1;
      uVar10 = uVar10 << 1;
    } while (iVar11 < 8);
  }
  return;
}

