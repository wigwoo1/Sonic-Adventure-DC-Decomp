/* Ghidra draft only; not buildable/verified source. */

void FUN_8c601e2a(void)

{
  undefined *puVar1;
  int *piVar2;
  int iVar3;
  uint *puVar4;
  undefined4 uVar5;
  int iVar6;
  uint uVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  undefined1 *puVar11;
  
  if (*DAT_8c601e58 == 0) {
    return;
  }
  iVar9 = 0;
  puVar11 = &DAT_8c88fe1c;
  iVar8 = 0;
  do {
    iVar3 = (*(code *)PTR_FUN_8c601f68)(iVar9,puVar11);
    puVar4 = (uint *)(*(code *)PTR_FUN_8c601f6c)(iVar9);
    iVar6 = DAT_8c601f74;
    iVar10 = iVar9 * 0x34 + DAT_8c601f70;
    *(uint **)(iVar10 + 0x30) = puVar4;
    *(undefined4 *)(iVar10 + 0x2c) = *(undefined4 *)(iVar10 + 8);
    uVar5 = FUN_8c601df6(iVar10,puVar4,iVar3);
    puVar1 = PTR_FUN_8c601f78;
    *(undefined4 *)(iVar6 + iVar9 * 4) = uVar5;
    iVar6 = (*(code *)puVar1)();
    if (iVar6 == 0) {
      if ((((*puVar4 & 0x21) == 0) || (*(char *)(iVar3 + 2) != '\0')) ||
         (*(char *)(iVar3 + 4) != '\b')) {
        if ((((*puVar4 & 0x40) == 0) || (*(char *)(iVar3 + 2) != '\0')) ||
           (*(char *)(iVar3 + 4) != '\b')) {
          FUN_8c601bb2(iVar10);
        }
        else {
          (*(code *)PTR_FUN_8c601f7c)(iVar8,iVar10,iVar3,puVar4);
        }
      }
      else {
        FUN_8c601da6(iVar10,puVar4);
        FUN_8c601bf0(iVar10,iVar3,iVar9);
      }
      piVar2 = DAT_8c601f80;
      *(uint *)(iVar10 + 0xc) = ~*(uint *)(iVar10 + 8);
      if (*piVar2 == 0) {
        uVar7 = *(uint *)(iVar10 + 0x2c) ^ *(uint *)(iVar10 + 8);
        *(uint *)(iVar10 + 0x10) = *(uint *)(iVar10 + 8) & uVar7;
        uVar7 = uVar7 & *(uint *)(iVar10 + 0xc);
      }
      else {
        uVar7 = *(uint *)(iVar10 + 0x2c) ^ *(uint *)(iVar10 + 8) ^ 0xffffffff;
        *(uint *)(iVar10 + 0x10) = *(uint *)(iVar10 + 8) | uVar7;
        uVar7 = uVar7 | *(uint *)(iVar10 + 0xc);
      }
      *(uint *)(iVar10 + 0x14) = uVar7;
      iVar8 = iVar8 + 1;
    }
    else {
      *(uint **)(iVar10 + 0x24) = puVar4 + 5;
    }
    iVar9 = iVar9 + 1;
  } while (iVar9 < 0x18);
  return;
}

