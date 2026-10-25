/* Ghidra draft only; not buildable/verified source. */

void FUN_8c65f558(void)

{
  int *piVar1;
  undefined4 uVar2;
  int iVar3;
  int *piVar4;
  undefined1 *puVar5;
  uint uVar6;
  byte *pbVar7;
  int iVar8;
  byte *pbVar9;
  int iVar10;
  int iVar11;
  byte bVar12;
  uint uVar13;
  byte local_24 [8];
  
  piVar1 = DAT_8c65f65c;
  iVar3 = (int)DAT_8c65f63c;
  uVar6 = 0;
  iVar10 = (int)DAT_8c65f63e;
  iVar11 = 0;
  *(undefined4 *)(iVar3 + *DAT_8c65f65c) = 0;
  iVar8 = (int)DAT_8c65f640;
  *(undefined4 *)(iVar3 + 4 + *piVar1) = 0;
  do {
    *(int *)((int)DAT_8c65f642 + *piVar1 + uVar6) = *(int *)((int)DAT_8c65f644 + *piVar1) + iVar11;
    piVar4 = (int *)((int)DAT_8c65f646 + *piVar1 + uVar6);
    uVar6 = uVar6 + 4;
    *piVar4 = *(int *)((int)DAT_8c65f644 + *piVar1) + iVar10;
    iVar10 = iVar10 + iVar8;
    iVar11 = iVar11 + iVar8;
  } while (uVar6 < 0x60);
  *(undefined4 *)((int)DAT_8c65f64a + *piVar1) = *(undefined4 *)((int)DAT_8c65f648 + *piVar1);
  iVar3 = (int)DAT_8c65f64e;
  *(int *)(iVar3 + *piVar1) = *(int *)((int)DAT_8c65f648 + *piVar1) + (int)DAT_8c65f64c;
  *(undefined4 *)((int)DAT_8c65f650 + *piVar1) = *(undefined4 *)(iVar3 + -4 + *piVar1);
  (*(code *)PTR_FUN_8c65f660)(*piVar1 + 4,0,8);
  (*(code *)PTR_FUN_8c65f660)(*piVar1 + 0x28,0,(int)DAT_8c65f652);
  uVar6 = (uint)DAT_8c65f654;
  uVar13 = 0;
  do {
    FUN_8c6602d2(*piVar1 + (int)DAT_8c65f656 + uVar13);
    uVar13 = uVar13 + 0x78;
  } while (uVar13 < uVar6);
  iVar3 = 0;
  do {
    puVar5 = (undefined1 *)((int)DAT_8c65f658 + *piVar1 + iVar3);
    iVar3 = iVar3 + 1;
    *puVar5 = 0;
  } while (iVar3 < 0x18);
  pbVar9 = local_24;
  (*(code *)PTR_FUN_8c65f668)();
  iVar3 = 0;
  if ((*DAT_8c65f66c & 0xff) < 2) {
    bVar12 = 7;
  }
  else {
    bVar12 = 0xf;
  }
  pbVar7 = pbVar9 + 4;
  iVar8 = 0;
  do {
    if ((*pbVar9 & bVar12) != 0) {
      iVar11 = 0;
      *(undefined1 *)((int)DAT_8c65f788 + *piVar1 + iVar8) = 0x52;
      iVar10 = iVar3;
      do {
        iVar10 = iVar10 + 1;
        iVar11 = iVar11 + 1;
        *(undefined1 *)((int)DAT_8c65f788 + *piVar1 + iVar10) = 0x45;
      } while (iVar11 < 5);
    }
    pbVar9 = pbVar9 + 1;
    iVar3 = iVar3 + 6;
    iVar8 = iVar8 + 6;
  } while (pbVar9 < pbVar7);
  *(undefined4 *)((int)DAT_8c65f78a + *piVar1) = DAT_8c65f798;
  iVar3 = (int)DAT_8c65f78c;
  *(undefined4 *)(iVar3 + *piVar1) = 0;
  *(undefined4 *)(iVar3 + -4 + *piVar1) = 0;
  *(undefined4 *)(iVar3 + 4 + *piVar1) = 1;
  iVar3 = (int)DAT_8c65f78e;
  *(undefined4 *)(iVar3 + *piVar1) = 0;
  *(undefined4 *)(iVar3 + 0x24 + *piVar1) = 0;
  *(undefined4 *)(iVar3 + 0x20 + *piVar1) = 0;
  *(undefined4 *)(iVar3 + 4 + *piVar1) = 5;
  *(undefined4 *)(iVar3 + 8 + *piVar1) = 0;
  *(undefined4 *)(iVar3 + 0x10 + *piVar1) = 10;
  uVar2 = DAT_8c65f79c;
  *(undefined4 *)(iVar3 + 0x14 + *piVar1) = 5;
  *(undefined4 *)(iVar3 + 0x18 + *piVar1) = 0;
  *(undefined4 *)(iVar3 + 0x1c + *piVar1) = 0;
  iVar3 = (int)DAT_8c65f790;
  *(undefined4 *)(iVar3 + *piVar1) = 0;
  *(undefined4 *)(iVar3 + 4 + *piVar1) = 0;
  *(undefined4 *)(iVar3 + 0x10 + *piVar1) = 0;
  iVar3 = (int)DAT_8c65f792;
  *(undefined4 *)*piVar1 = uVar2;
  *(undefined4 *)(*piVar1 + 0x24) = DAT_8c65f7a0;
  *(undefined4 *)(iVar3 + *piVar1) = DAT_8c65f7a4;
  iVar3 = (int)DAT_8c65f794;
  *(undefined4 *)(iVar3 + *piVar1) = DAT_8c65f7a8;
  *(undefined4 *)(iVar3 + 0x1c + *piVar1) = DAT_8c65f7ac;
  *(undefined4 *)(iVar3 + 0x58 + *piVar1) = DAT_8c65f7b0;
  iVar3 = (int)DAT_8c65f796;
  *(undefined4 *)(iVar3 + *piVar1) = DAT_8c65f7b4;
  *(undefined4 *)(iVar3 + 0x18 + *piVar1) = DAT_8c65f7b8;
  *(undefined4 *)(iVar3 + 0x34 + *piVar1) = DAT_8c65f7bc;
  *(undefined4 *)(iVar3 + 0x3c + *piVar1) = DAT_8c65f7c0;
  (*(code *)PTR_FUN_8c65f7c4)(0);
  (*(code *)PTR_FUN_8c65f7c8)();
  return;
}

