/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c10cd08) */
/* WARNING: Removing unreachable block (ram,0x8c10cd00) */
/* WARNING: Removing unreachable block (ram,0x8c10ccf8) */
/* WARNING: Removing unreachable block (ram,0x8c10ccf0) */
/* WARNING: Removing unreachable block (ram,0x8c10cce8) */
/* WARNING: Removing unreachable block (ram,0x8c10cce0) */
/* WARNING: Removing unreachable block (ram,0x8c10ccd8) */
/* WARNING: Removing unreachable block (ram,0x8c10ccd0) */
/* WARNING: Removing unreachable block (ram,0x8c10ccc8) */
/* WARNING: Removing unreachable block (ram,0x8c10ccc0) */
/* WARNING: Removing unreachable block (ram,0x8c10ccb8) */
/* WARNING: Removing unreachable block (ram,0x8c10ccb0) */
/* WARNING: Removing unreachable block (ram,0x8c10cca8) */
/* WARNING: Removing unreachable block (ram,0x8c10cca0) */
/* WARNING: Removing unreachable block (ram,0x8c10cc98) */
/* WARNING: Removing unreachable block (ram,0x8c10cc90) */
/* WARNING: Removing unreachable block (ram,0x8c10cc94) */
/* WARNING: Removing unreachable block (ram,0x8c10cc9c) */
/* WARNING: Removing unreachable block (ram,0x8c10cca4) */
/* WARNING: Removing unreachable block (ram,0x8c10ccac) */
/* WARNING: Removing unreachable block (ram,0x8c10ccb4) */
/* WARNING: Removing unreachable block (ram,0x8c10ccbc) */
/* WARNING: Removing unreachable block (ram,0x8c10ccc4) */
/* WARNING: Removing unreachable block (ram,0x8c10cccc) */
/* WARNING: Removing unreachable block (ram,0x8c10ccd4) */
/* WARNING: Removing unreachable block (ram,0x8c10ccdc) */
/* WARNING: Removing unreachable block (ram,0x8c10cce4) */
/* WARNING: Removing unreachable block (ram,0x8c10ccec) */
/* WARNING: Removing unreachable block (ram,0x8c10ccf4) */
/* WARNING: Removing unreachable block (ram,0x8c10ccfc) */
/* WARNING: Removing unreachable block (ram,0x8c10cd04) */
/* WARNING: Removing unreachable block (ram,0x8c10cd0c) */

longlong FUN_8c10cc80(void)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  undefined4 *puVar4;
  int in_r0;
  uint in_r1;
  uint uVar5;
  bool bVar6;
  bool bVar8;
  bool bVar9;
  bool bVar10;
  bool bVar11;
  bool bVar12;
  bool bVar13;
  bool bVar14;
  bool bVar15;
  bool bVar16;
  bool bVar17;
  bool bVar18;
  bool bVar19;
  bool bVar20;
  bool bVar21;
  bool bVar22;
  bool bVar23;
  bool bVar24;
  bool bVar25;
  bool bVar26;
  bool bVar27;
  bool bVar28;
  bool bVar29;
  bool bVar30;
  bool bVar31;
  bool bVar32;
  bool bVar33;
  bool bVar34;
  bool bVar35;
  bool bVar36;
  bool bVar37;
  bool bVar7;
  
  puVar4 = DAT_8c10cd30;
  if (in_r0 == 0) {
    *DAT_8c10cd30 = DAT_8c10cd34;
    return ZEXT48(puVar4) << 0x20;
  }
  uVar3 = (uint)((in_r1 & 0x80000000) != 0);
  uVar1 = uVar3 - in_r0;
  bVar37 = uVar3 < uVar1;
  uVar3 = in_r1 << 2;
  bVar6 = (uVar1 & 0x80000000) == 0;
  uVar1 = uVar1 * 2 | (uint)((in_r1 & 0x40000000) != 0);
  if (bVar37) {
    if (bVar37) {
      uVar2 = uVar1 + in_r0;
      bVar7 = uVar2 < uVar1;
      if (!bVar6) {
        bVar7 = !bVar7;
      }
    }
    else {
      uVar2 = uVar1 - in_r0;
      bVar7 = uVar1 < uVar2;
      if (bVar6) {
        bVar7 = !bVar7;
      }
    }
  }
  else {
    uVar2 = uVar1 - in_r0;
    bVar7 = uVar1 < uVar2;
    if (!bVar6) {
      bVar7 = !bVar7;
    }
  }
  bVar6 = (uVar2 & 0x80000000) == 0;
  uVar1 = uVar2 << 1 | (uint)((uVar3 & 0x80000000) != 0);
  if (bVar7) {
    if (bVar7) {
      uVar2 = uVar1 + in_r0;
      bVar8 = uVar2 < uVar1;
      if (!bVar6) {
        bVar8 = !bVar8;
      }
    }
    else {
      uVar2 = uVar1 - in_r0;
      bVar8 = uVar1 < uVar2;
      if (bVar6) {
        bVar8 = !bVar8;
      }
    }
  }
  else {
    uVar2 = uVar1 - in_r0;
    bVar8 = uVar1 < uVar2;
    if (!bVar6) {
      bVar8 = !bVar8;
    }
  }
  bVar6 = (uVar2 & 0x80000000) == 0;
  uVar1 = uVar2 << 1 | (uint)((uVar3 & 0x40000000) != 0);
  if (bVar8) {
    if (bVar8) {
      uVar2 = uVar1 + in_r0;
      bVar9 = uVar2 < uVar1;
      if (!bVar6) {
        bVar9 = !bVar9;
      }
    }
    else {
      uVar2 = uVar1 - in_r0;
      bVar9 = uVar1 < uVar2;
      if (bVar6) {
        bVar9 = !bVar9;
      }
    }
  }
  else {
    uVar2 = uVar1 - in_r0;
    bVar9 = uVar1 < uVar2;
    if (!bVar6) {
      bVar9 = !bVar9;
    }
  }
  bVar6 = (uVar2 & 0x80000000) == 0;
  uVar1 = uVar2 << 1 | (uint)((uVar3 & 0x20000000) != 0);
  if (bVar9) {
    if (bVar9) {
      uVar2 = uVar1 + in_r0;
      bVar10 = uVar2 < uVar1;
      if (!bVar6) {
        bVar10 = !bVar10;
      }
    }
    else {
      uVar2 = uVar1 - in_r0;
      bVar10 = uVar1 < uVar2;
      if (bVar6) {
        bVar10 = !bVar10;
      }
    }
  }
  else {
    uVar2 = uVar1 - in_r0;
    bVar10 = uVar1 < uVar2;
    if (!bVar6) {
      bVar10 = !bVar10;
    }
  }
  bVar6 = (uVar2 & 0x80000000) == 0;
  uVar1 = uVar2 << 1 | (uint)((uVar3 & 0x10000000) != 0);
  if (bVar10) {
    if (bVar10) {
      uVar2 = uVar1 + in_r0;
      bVar11 = uVar2 < uVar1;
      if (!bVar6) {
        bVar11 = !bVar11;
      }
    }
    else {
      uVar2 = uVar1 - in_r0;
      bVar11 = uVar1 < uVar2;
      if (bVar6) {
        bVar11 = !bVar11;
      }
    }
  }
  else {
    uVar2 = uVar1 - in_r0;
    bVar11 = uVar1 < uVar2;
    if (!bVar6) {
      bVar11 = !bVar11;
    }
  }
  bVar6 = (uVar2 & 0x80000000) == 0;
  uVar1 = uVar2 << 1 | (uint)((uVar3 & 0x8000000) != 0);
  if (bVar11) {
    if (bVar11) {
      uVar2 = uVar1 + in_r0;
      bVar12 = uVar2 < uVar1;
      if (!bVar6) {
        bVar12 = !bVar12;
      }
    }
    else {
      uVar2 = uVar1 - in_r0;
      bVar12 = uVar1 < uVar2;
      if (bVar6) {
        bVar12 = !bVar12;
      }
    }
  }
  else {
    uVar2 = uVar1 - in_r0;
    bVar12 = uVar1 < uVar2;
    if (!bVar6) {
      bVar12 = !bVar12;
    }
  }
  bVar6 = (uVar2 & 0x80000000) == 0;
  uVar1 = uVar2 << 1 | (uint)((uVar3 & 0x4000000) != 0);
  if (bVar12) {
    if (bVar12) {
      uVar2 = uVar1 + in_r0;
      bVar13 = uVar2 < uVar1;
      if (!bVar6) {
        bVar13 = !bVar13;
      }
    }
    else {
      uVar2 = uVar1 - in_r0;
      bVar13 = uVar1 < uVar2;
      if (bVar6) {
        bVar13 = !bVar13;
      }
    }
  }
  else {
    uVar2 = uVar1 - in_r0;
    bVar13 = uVar1 < uVar2;
    if (!bVar6) {
      bVar13 = !bVar13;
    }
  }
  bVar6 = (uVar2 & 0x80000000) == 0;
  uVar1 = uVar2 << 1 | (uint)((uVar3 & 0x2000000) != 0);
  if (bVar13) {
    if (bVar13) {
      uVar2 = uVar1 + in_r0;
      bVar14 = uVar2 < uVar1;
      if (!bVar6) {
        bVar14 = !bVar14;
      }
    }
    else {
      uVar2 = uVar1 - in_r0;
      bVar14 = uVar1 < uVar2;
      if (bVar6) {
        bVar14 = !bVar14;
      }
    }
  }
  else {
    uVar2 = uVar1 - in_r0;
    bVar14 = uVar1 < uVar2;
    if (!bVar6) {
      bVar14 = !bVar14;
    }
  }
  bVar6 = (uVar2 & 0x80000000) == 0;
  uVar1 = uVar2 << 1 | (uint)((uVar3 & 0x1000000) != 0);
  if (bVar14) {
    if (bVar14) {
      uVar2 = uVar1 + in_r0;
      bVar15 = uVar2 < uVar1;
      if (!bVar6) {
        bVar15 = !bVar15;
      }
    }
    else {
      uVar2 = uVar1 - in_r0;
      bVar15 = uVar1 < uVar2;
      if (bVar6) {
        bVar15 = !bVar15;
      }
    }
  }
  else {
    uVar2 = uVar1 - in_r0;
    bVar15 = uVar1 < uVar2;
    if (!bVar6) {
      bVar15 = !bVar15;
    }
  }
  bVar6 = (uVar2 & 0x80000000) == 0;
  uVar1 = uVar2 << 1 | (uint)((uVar3 & 0x800000) != 0);
  if (bVar15) {
    if (bVar15) {
      uVar2 = uVar1 + in_r0;
      bVar16 = uVar2 < uVar1;
      if (!bVar6) {
        bVar16 = !bVar16;
      }
    }
    else {
      uVar2 = uVar1 - in_r0;
      bVar16 = uVar1 < uVar2;
      if (bVar6) {
        bVar16 = !bVar16;
      }
    }
  }
  else {
    uVar2 = uVar1 - in_r0;
    bVar16 = uVar1 < uVar2;
    if (!bVar6) {
      bVar16 = !bVar16;
    }
  }
  bVar6 = (uVar2 & 0x80000000) == 0;
  uVar1 = uVar2 << 1 | (uint)((uVar3 & 0x400000) != 0);
  if (bVar16) {
    if (bVar16) {
      uVar2 = uVar1 + in_r0;
      bVar17 = uVar2 < uVar1;
      if (!bVar6) {
        bVar17 = !bVar17;
      }
    }
    else {
      uVar2 = uVar1 - in_r0;
      bVar17 = uVar1 < uVar2;
      if (bVar6) {
        bVar17 = !bVar17;
      }
    }
  }
  else {
    uVar2 = uVar1 - in_r0;
    bVar17 = uVar1 < uVar2;
    if (!bVar6) {
      bVar17 = !bVar17;
    }
  }
  bVar6 = (uVar2 & 0x80000000) == 0;
  uVar1 = uVar2 << 1 | (uint)((uVar3 & 0x200000) != 0);
  if (bVar17) {
    if (bVar17) {
      uVar2 = uVar1 + in_r0;
      bVar18 = uVar2 < uVar1;
      if (!bVar6) {
        bVar18 = !bVar18;
      }
    }
    else {
      uVar2 = uVar1 - in_r0;
      bVar18 = uVar1 < uVar2;
      if (bVar6) {
        bVar18 = !bVar18;
      }
    }
  }
  else {
    uVar2 = uVar1 - in_r0;
    bVar18 = uVar1 < uVar2;
    if (!bVar6) {
      bVar18 = !bVar18;
    }
  }
  bVar6 = (uVar2 & 0x80000000) == 0;
  uVar1 = uVar2 << 1 | (uint)((uVar3 & 0x100000) != 0);
  if (bVar18) {
    if (bVar18) {
      uVar2 = uVar1 + in_r0;
      bVar19 = uVar2 < uVar1;
      if (!bVar6) {
        bVar19 = !bVar19;
      }
    }
    else {
      uVar2 = uVar1 - in_r0;
      bVar19 = uVar1 < uVar2;
      if (bVar6) {
        bVar19 = !bVar19;
      }
    }
  }
  else {
    uVar2 = uVar1 - in_r0;
    bVar19 = uVar1 < uVar2;
    if (!bVar6) {
      bVar19 = !bVar19;
    }
  }
  bVar6 = (uVar2 & 0x80000000) == 0;
  uVar1 = uVar2 << 1 | (uint)((uVar3 & 0x80000) != 0);
  if (bVar19) {
    if (bVar19) {
      uVar2 = uVar1 + in_r0;
      bVar20 = uVar2 < uVar1;
      if (!bVar6) {
        bVar20 = !bVar20;
      }
    }
    else {
      uVar2 = uVar1 - in_r0;
      bVar20 = uVar1 < uVar2;
      if (bVar6) {
        bVar20 = !bVar20;
      }
    }
  }
  else {
    uVar2 = uVar1 - in_r0;
    bVar20 = uVar1 < uVar2;
    if (!bVar6) {
      bVar20 = !bVar20;
    }
  }
  bVar6 = (uVar2 & 0x80000000) == 0;
  uVar1 = uVar2 << 1 | (uint)((uVar3 & 0x40000) != 0);
  if (bVar20) {
    if (bVar20) {
      uVar2 = uVar1 + in_r0;
      bVar21 = uVar2 < uVar1;
      if (!bVar6) {
        bVar21 = !bVar21;
      }
    }
    else {
      uVar2 = uVar1 - in_r0;
      bVar21 = uVar1 < uVar2;
      if (bVar6) {
        bVar21 = !bVar21;
      }
    }
  }
  else {
    uVar2 = uVar1 - in_r0;
    bVar21 = uVar1 < uVar2;
    if (!bVar6) {
      bVar21 = !bVar21;
    }
  }
  bVar6 = (uVar2 & 0x80000000) == 0;
  uVar1 = uVar2 << 1 | (uint)((uVar3 & 0x20000) != 0);
  if (bVar21) {
    if (bVar21) {
      uVar2 = uVar1 + in_r0;
      bVar22 = uVar2 < uVar1;
      if (!bVar6) {
        bVar22 = !bVar22;
      }
    }
    else {
      uVar2 = uVar1 - in_r0;
      bVar22 = uVar1 < uVar2;
      if (bVar6) {
        bVar22 = !bVar22;
      }
    }
  }
  else {
    uVar2 = uVar1 - in_r0;
    bVar22 = uVar1 < uVar2;
    if (!bVar6) {
      bVar22 = !bVar22;
    }
  }
  bVar6 = (uVar2 & 0x80000000) == 0;
  uVar1 = uVar2 << 1 | (uint)((uVar3 & 0x10000) != 0);
  if (bVar22) {
    if (bVar22) {
      uVar2 = uVar1 + in_r0;
      bVar23 = uVar2 < uVar1;
      if (!bVar6) {
        bVar23 = !bVar23;
      }
    }
    else {
      uVar2 = uVar1 - in_r0;
      bVar23 = uVar1 < uVar2;
      if (bVar6) {
        bVar23 = !bVar23;
      }
    }
  }
  else {
    uVar2 = uVar1 - in_r0;
    bVar23 = uVar1 < uVar2;
    if (!bVar6) {
      bVar23 = !bVar23;
    }
  }
  bVar6 = (uVar2 & 0x80000000) == 0;
  uVar1 = uVar2 << 1 | (uint)((uVar3 & 0x8000) != 0);
  if (bVar23) {
    if (bVar23) {
      uVar2 = uVar1 + in_r0;
      bVar24 = uVar2 < uVar1;
      if (!bVar6) {
        bVar24 = !bVar24;
      }
    }
    else {
      uVar2 = uVar1 - in_r0;
      bVar24 = uVar1 < uVar2;
      if (bVar6) {
        bVar24 = !bVar24;
      }
    }
  }
  else {
    uVar2 = uVar1 - in_r0;
    bVar24 = uVar1 < uVar2;
    if (!bVar6) {
      bVar24 = !bVar24;
    }
  }
  bVar6 = (uVar2 & 0x80000000) == 0;
  uVar1 = uVar2 << 1 | (uint)((uVar3 & 0x4000) != 0);
  if (bVar24) {
    if (bVar24) {
      uVar2 = uVar1 + in_r0;
      bVar25 = uVar2 < uVar1;
      if (!bVar6) {
        bVar25 = !bVar25;
      }
    }
    else {
      uVar2 = uVar1 - in_r0;
      bVar25 = uVar1 < uVar2;
      if (bVar6) {
        bVar25 = !bVar25;
      }
    }
  }
  else {
    uVar2 = uVar1 - in_r0;
    bVar25 = uVar1 < uVar2;
    if (!bVar6) {
      bVar25 = !bVar25;
    }
  }
  bVar6 = (uVar2 & 0x80000000) == 0;
  uVar1 = uVar2 << 1 | (uint)((uVar3 & 0x2000) != 0);
  if (bVar25) {
    if (bVar25) {
      uVar2 = uVar1 + in_r0;
      bVar26 = uVar2 < uVar1;
      if (!bVar6) {
        bVar26 = !bVar26;
      }
    }
    else {
      uVar2 = uVar1 - in_r0;
      bVar26 = uVar1 < uVar2;
      if (bVar6) {
        bVar26 = !bVar26;
      }
    }
  }
  else {
    uVar2 = uVar1 - in_r0;
    bVar26 = uVar1 < uVar2;
    if (!bVar6) {
      bVar26 = !bVar26;
    }
  }
  bVar6 = (uVar2 & 0x80000000) == 0;
  uVar1 = uVar2 << 1 | (uint)((uVar3 & 0x1000) != 0);
  if (bVar26) {
    if (bVar26) {
      uVar2 = uVar1 + in_r0;
      bVar27 = uVar2 < uVar1;
      if (!bVar6) {
        bVar27 = !bVar27;
      }
    }
    else {
      uVar2 = uVar1 - in_r0;
      bVar27 = uVar1 < uVar2;
      if (bVar6) {
        bVar27 = !bVar27;
      }
    }
  }
  else {
    uVar2 = uVar1 - in_r0;
    bVar27 = uVar1 < uVar2;
    if (!bVar6) {
      bVar27 = !bVar27;
    }
  }
  bVar6 = (uVar2 & 0x80000000) == 0;
  uVar1 = uVar2 << 1 | (uint)((uVar3 & 0x800) != 0);
  if (bVar27) {
    if (bVar27) {
      uVar2 = uVar1 + in_r0;
      bVar28 = uVar2 < uVar1;
      if (!bVar6) {
        bVar28 = !bVar28;
      }
    }
    else {
      uVar2 = uVar1 - in_r0;
      bVar28 = uVar1 < uVar2;
      if (bVar6) {
        bVar28 = !bVar28;
      }
    }
  }
  else {
    uVar2 = uVar1 - in_r0;
    bVar28 = uVar1 < uVar2;
    if (!bVar6) {
      bVar28 = !bVar28;
    }
  }
  bVar6 = (uVar2 & 0x80000000) == 0;
  uVar1 = uVar2 << 1 | (uint)((uVar3 & 0x400) != 0);
  if (bVar28) {
    if (bVar28) {
      uVar2 = uVar1 + in_r0;
      bVar29 = uVar2 < uVar1;
      if (!bVar6) {
        bVar29 = !bVar29;
      }
    }
    else {
      uVar2 = uVar1 - in_r0;
      bVar29 = uVar1 < uVar2;
      if (bVar6) {
        bVar29 = !bVar29;
      }
    }
  }
  else {
    uVar2 = uVar1 - in_r0;
    bVar29 = uVar1 < uVar2;
    if (!bVar6) {
      bVar29 = !bVar29;
    }
  }
  bVar6 = (uVar2 & 0x80000000) == 0;
  uVar1 = uVar2 << 1 | (uint)((uVar3 & 0x200) != 0);
  if (bVar29) {
    if (bVar29) {
      uVar2 = uVar1 + in_r0;
      bVar30 = uVar2 < uVar1;
      if (!bVar6) {
        bVar30 = !bVar30;
      }
    }
    else {
      uVar2 = uVar1 - in_r0;
      bVar30 = uVar1 < uVar2;
      if (bVar6) {
        bVar30 = !bVar30;
      }
    }
  }
  else {
    uVar2 = uVar1 - in_r0;
    bVar30 = uVar1 < uVar2;
    if (!bVar6) {
      bVar30 = !bVar30;
    }
  }
  bVar6 = (uVar2 & 0x80000000) == 0;
  uVar1 = uVar2 << 1 | (uint)((uVar3 & 0x100) != 0);
  if (bVar30) {
    if (bVar30) {
      uVar2 = uVar1 + in_r0;
      bVar31 = uVar2 < uVar1;
      if (!bVar6) {
        bVar31 = !bVar31;
      }
    }
    else {
      uVar2 = uVar1 - in_r0;
      bVar31 = uVar1 < uVar2;
      if (bVar6) {
        bVar31 = !bVar31;
      }
    }
  }
  else {
    uVar2 = uVar1 - in_r0;
    bVar31 = uVar1 < uVar2;
    if (!bVar6) {
      bVar31 = !bVar31;
    }
  }
  bVar6 = (uVar2 & 0x80000000) == 0;
  uVar1 = uVar2 << 1 | (uint)((uVar3 & 0x80) != 0);
  if (bVar31) {
    if (bVar31) {
      uVar2 = uVar1 + in_r0;
      bVar32 = uVar2 < uVar1;
      if (!bVar6) {
        bVar32 = !bVar32;
      }
    }
    else {
      uVar2 = uVar1 - in_r0;
      bVar32 = uVar1 < uVar2;
      if (bVar6) {
        bVar32 = !bVar32;
      }
    }
  }
  else {
    uVar2 = uVar1 - in_r0;
    bVar32 = uVar1 < uVar2;
    if (!bVar6) {
      bVar32 = !bVar32;
    }
  }
  bVar6 = (uVar2 & 0x80000000) == 0;
  uVar1 = uVar2 << 1 | (uint)((uVar3 & 0x40) != 0);
  if (bVar32) {
    if (bVar32) {
      uVar2 = uVar1 + in_r0;
      bVar33 = uVar2 < uVar1;
      if (!bVar6) {
        bVar33 = !bVar33;
      }
    }
    else {
      uVar2 = uVar1 - in_r0;
      bVar33 = uVar1 < uVar2;
      if (bVar6) {
        bVar33 = !bVar33;
      }
    }
  }
  else {
    uVar2 = uVar1 - in_r0;
    bVar33 = uVar1 < uVar2;
    if (!bVar6) {
      bVar33 = !bVar33;
    }
  }
  bVar6 = (uVar2 & 0x80000000) == 0;
  uVar1 = uVar2 << 1 | (uint)((uVar3 & 0x20) != 0);
  if (bVar33) {
    if (bVar33) {
      uVar2 = uVar1 + in_r0;
      bVar34 = uVar2 < uVar1;
      if (!bVar6) {
        bVar34 = !bVar34;
      }
    }
    else {
      uVar2 = uVar1 - in_r0;
      bVar34 = uVar1 < uVar2;
      if (bVar6) {
        bVar34 = !bVar34;
      }
    }
  }
  else {
    uVar2 = uVar1 - in_r0;
    bVar34 = uVar1 < uVar2;
    if (!bVar6) {
      bVar34 = !bVar34;
    }
  }
  bVar6 = (uVar2 & 0x80000000) == 0;
  uVar1 = uVar2 << 1 | (uint)((uVar3 & 0x10) != 0);
  if (bVar34) {
    if (bVar34) {
      uVar2 = uVar1 + in_r0;
      bVar35 = uVar2 < uVar1;
      if (!bVar6) {
        bVar35 = !bVar35;
      }
    }
    else {
      uVar2 = uVar1 - in_r0;
      bVar35 = uVar1 < uVar2;
      if (bVar6) {
        bVar35 = !bVar35;
      }
    }
  }
  else {
    uVar2 = uVar1 - in_r0;
    bVar35 = uVar1 < uVar2;
    if (!bVar6) {
      bVar35 = !bVar35;
    }
  }
  bVar6 = (uVar2 & 0x80000000) == 0;
  uVar1 = uVar2 << 1 | (uint)((uVar3 & 8) != 0);
  if (bVar35) {
    if (bVar35) {
      uVar2 = uVar1 + in_r0;
      bVar36 = uVar2 < uVar1;
      if (!bVar6) {
        bVar36 = !bVar36;
      }
    }
    else {
      uVar2 = uVar1 - in_r0;
      bVar36 = uVar1 < uVar2;
      if (bVar6) {
        bVar36 = !bVar36;
      }
    }
  }
  else {
    uVar2 = uVar1 - in_r0;
    bVar36 = uVar1 < uVar2;
    if (!bVar6) {
      bVar36 = !bVar36;
    }
  }
  uVar5 = ((((((((((((((((((((((((((((((uint)!bVar37 << 1 | (uint)!bVar7) << 1 | (uint)!bVar8) << 1
                                    | (uint)!bVar9) << 1 | (uint)!bVar10) << 1 | (uint)!bVar11) << 1
                                 | (uint)!bVar12) << 1 | (uint)!bVar13) << 1 | (uint)!bVar14) << 1 |
                              (uint)!bVar15) << 1 | (uint)!bVar16) << 1 | (uint)!bVar17) << 1 |
                           (uint)!bVar18) << 1 | (uint)!bVar19) << 1 | (uint)!bVar20) << 1 |
                        (uint)!bVar21) << 1 | (uint)!bVar22) << 1 | (uint)!bVar23) << 1 |
                     (uint)!bVar24) << 1 | (uint)!bVar25) << 1 | (uint)!bVar26) << 1 | (uint)!bVar27
                  ) << 1 | (uint)!bVar28) << 1 | (uint)!bVar29) << 1 | (uint)!bVar30) << 1 |
              (uint)!bVar31) << 1 | (uint)!bVar32) << 1 | (uint)!bVar33) << 1 | (uint)!bVar34) << 1
          | (uint)!bVar35) << 1 | (uint)!bVar36;
  bVar37 = (uVar2 & 0x80000000) == 0;
  uVar1 = uVar2 << 1 | (uint)((uVar3 & 4) != 0);
  if (bVar36) {
    if (bVar36) {
      uVar3 = uVar1 + in_r0;
      bVar6 = uVar3 < uVar1;
      if (!bVar37) {
        bVar6 = !bVar6;
      }
    }
    else {
      uVar3 = uVar1 - in_r0;
      bVar6 = uVar1 < uVar3;
      if (bVar37) {
        bVar6 = !bVar6;
      }
    }
  }
  else {
    uVar3 = uVar1 - in_r0;
    bVar6 = uVar1 < uVar3;
    if (!bVar37) {
      bVar6 = !bVar6;
    }
  }
  if (!bVar6) {
    return CONCAT44(uVar5,uVar3);
  }
  return CONCAT44(uVar5,in_r0 + uVar3);
}

