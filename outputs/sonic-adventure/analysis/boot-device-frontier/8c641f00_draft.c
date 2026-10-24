/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c641f00(void)

{
  bool bVar1;
  undefined4 *puVar2;
  undefined *puVar3;
  undefined4 uVar4;
  uint *puVar5;
  uint *puVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int *piVar10;
  int iVar11;
  undefined1 auStack_28 [4];
  uint local_24;
  
  iVar8 = DAT_8c641fb0;
  piVar10 = DAT_8c641fa8;
  if (*(int *)PTR_DAT_8c641fa0 == 0) {
    if (*DAT_8c641fa4 == 0) {
      DAT_8c641fa8[*DAT_8c641fa8 + -4] = 1;
      *DAT_8c641fac = 1;
    }
    else {
      *(undefined4 *)(DAT_8c641fb0 + 0x48) = *DAT_8c641fb4;
      iVar9 = *piVar10;
      *DAT_8c641fb8 = iVar9;
      *DAT_8c641fbc = 1;
      puVar2 = DAT_8c641fc0;
      *DAT_8c641fc0 = 0;
      *DAT_8c641fc8 = *DAT_8c641fc4;
      puVar2[2] = 1;
      iVar11 = DAT_8c641fcc;
      *(undefined4 *)(iVar8 + 4) = 0;
      *(undefined4 *)(iVar11 + iVar9 * 4) = 2;
      uVar4 = DAT_8c6420d4;
      puVar3 = PTR_FUN_8c6420d0;
      if ((*DAT_8c641fd0 & 0x20) == 0) {
        iVar11 = *DAT_8c6420c4 * 4 + DAT_8c6420c8;
        piVar10 = DAT_8c6420c4 + *DAT_8c6420c4 + *DAT_8c6420cc * 4 + 0x1d;
        iVar8 = *DAT_8c6420cc;
        iVar9 = (int)DAT_8c6420c0;
        do {
          iVar7 = (*(code *)puVar3)(uVar4,*(undefined4 *)(iVar11 + iVar8 * 0x10),*piVar10 + 0x20,
                                    0x10,auStack_28);
        } while (iVar7 == iVar9);
        FUN_8c641e00(*DAT_8c6420cc);
      }
      else {
        *DAT_8c641fd4 = 0x10;
        *DAT_8c641fd8 = 0x10;
        (*(code *)PTR_FUN_8c641fe4)(DAT_8c641fe0,PTR_DAT_8c641fdc,4);
        FUN_8c641e00(*DAT_8c641fe8);
      }
    }
  }
  *DAT_8c6420d8 = 0;
  bVar1 = false;
  iVar8 = *DAT_8c6420dc;
  while (iVar8 != 0) {
    (*(code *)*DAT_8c6420e0)(DAT_8c6420e0[1]);
    if (*DAT_8c6420e8 + *DAT_8c6420e4 + 1U < *DAT_8c6420d8) {
      bVar1 = true;
      break;
    }
    iVar8 = *DAT_8c6420dc;
  }
  if (bVar1) {
    *DAT_8c6420ec = 0;
    *DAT_8c6420d8 = 0;
    bVar1 = false;
    iVar8 = *DAT_8c6420dc;
    while (iVar8 != 0) {
      (*(code *)*DAT_8c6420e0)(DAT_8c6420e0[1]);
      if (0x14 < *DAT_8c6420d8) {
        bVar1 = true;
        break;
      }
      iVar8 = *DAT_8c6421ec;
    }
    if (bVar1) {
      (*(code *)PTR_FUN_8c6421f0)(4);
    }
  }
  puVar5 = DAT_8c6421f4;
  *DAT_8c6421f4 = *DAT_8c6421f4 + 1;
  *puVar5 = *puVar5 & 3;
  *(undefined4 *)(DAT_8c6421f8 + *puVar5 * 4) = 0;
  puVar3 = PTR_FUN_8c642204;
  if ((*DAT_8c6421fc == 1) || (*(int *)PTR_DAT_8c642200 == 1)) {
    local_24 = *DAT_8c6421f4 & 1;
    (*(code *)PTR_FUN_8c642204)(8,1);
    (*(code *)puVar3)(8,0);
    iVar8 = local_24 * 0x20;
    (*(code *)puVar3)((int)DAT_8c6421e0,*(undefined4 *)(DAT_8c642208 + iVar8));
    (*(code *)puVar3)((int)DAT_8c6421e2,*(undefined4 *)(DAT_8c64220c + iVar8));
    (*(code *)puVar3)((int)DAT_8c6421e4,*(undefined4 *)(DAT_8c642210 + iVar8));
    (*(code *)puVar3)((int)DAT_8c6421e6,*(undefined4 *)(DAT_8c642214 + iVar8));
    (*(code *)puVar3)((int)DAT_8c6421e8,*(undefined4 *)(DAT_8c642218 + iVar8));
    (*(code *)puVar3)((int)DAT_8c6421ea,DAT_8c64221c);
    (*(code *)PTR_FUN_8c642220)((int)DAT_8c6421ea);
    *DAT_8c642224 = 0;
    *DAT_8c64222c = *DAT_8c642228;
    puVar6 = DAT_8c642230;
    puVar5 = DAT_8c6421f4;
    if (*(int *)PTR_DAT_8c642200 != 0) {
      *DAT_8c642230 = *DAT_8c6421f4;
      puVar6[*puVar5 - 6] = 2;
      *DAT_8c642234 = 1;
      puVar5[8] = 1;
    }
  }
  return 0;
}

