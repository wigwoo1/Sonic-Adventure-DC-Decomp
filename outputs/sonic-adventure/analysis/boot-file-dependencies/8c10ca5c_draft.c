/* Ghidra draft only; not buildable/verified source. */

int FUN_8c10ca5c(void)

{
  uint uVar1;
  uint uVar2;
  uint in_r0;
  uint in_r1;
  uint uVar3;
  bool bVar4;
  bool bVar5;
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
  bool bVar38;
  bool bVar7;
  
  if (in_r0 == 0) {
    *DAT_8c10cb08 = DAT_8c10cb0c;
    return 0;
  }
  uVar2 = (uint)((in_r1 & 0x80000000) != 0);
  uVar3 = in_r1 - (uVar2 != 0);
  bVar5 = (-uVar2 & 0x80000000) != 0;
  bVar4 = (in_r0 & 0x80000000) != 0;
  uVar1 = uVar3 * 2;
  bVar6 = (-uVar2 & 0x80000000) == 0;
  uVar2 = uVar2 * -2 | (uint)((uVar3 & 0x80000000) != 0);
  if (bVar5 || bVar4) {
    if (!bVar5 && bVar4) {
      uVar3 = uVar2 + in_r0;
      bVar7 = uVar3 < uVar2;
      if (bVar6) {
        bVar7 = !bVar7;
      }
    }
    else if (bVar5 && !bVar4) {
      uVar3 = uVar2 + in_r0;
      bVar7 = uVar3 < uVar2;
      if (!bVar6) {
        bVar7 = !bVar7;
      }
    }
    else {
      uVar3 = uVar2 - in_r0;
      bVar7 = uVar2 < uVar3;
      if (bVar6) {
        bVar7 = !bVar7;
      }
    }
  }
  else {
    uVar3 = uVar2 - in_r0;
    bVar7 = uVar2 < uVar3;
    if (!bVar6) {
      bVar7 = !bVar7;
    }
  }
  bVar6 = (uVar3 & 0x80000000) == 0;
  uVar2 = uVar3 << 1 | (uint)((uVar1 & 0x80000000) != 0);
  if (bVar7 == false && !bVar4) {
    uVar3 = uVar2 - in_r0;
    bVar8 = uVar2 < uVar3;
    if (!bVar6) {
      bVar8 = !bVar8;
    }
  }
  else if (bVar7 == false && bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar8 = uVar3 < uVar2;
    if (bVar6) {
      bVar8 = !bVar8;
    }
  }
  else if (bVar7 == true && !bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar8 = uVar3 < uVar2;
    if (!bVar6) {
      bVar8 = !bVar8;
    }
  }
  else {
    uVar3 = uVar2 - in_r0;
    bVar8 = uVar2 < uVar3;
    if (bVar6) {
      bVar8 = !bVar8;
    }
  }
  bVar6 = (uVar3 & 0x80000000) == 0;
  uVar2 = uVar3 << 1 | (uint)((uVar1 & 0x40000000) != 0);
  if (bVar8 == false && !bVar4) {
    uVar3 = uVar2 - in_r0;
    bVar9 = uVar2 < uVar3;
    if (!bVar6) {
      bVar9 = !bVar9;
    }
  }
  else if (bVar8 == false && bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar9 = uVar3 < uVar2;
    if (bVar6) {
      bVar9 = !bVar9;
    }
  }
  else if (bVar8 == true && !bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar9 = uVar3 < uVar2;
    if (!bVar6) {
      bVar9 = !bVar9;
    }
  }
  else {
    uVar3 = uVar2 - in_r0;
    bVar9 = uVar2 < uVar3;
    if (bVar6) {
      bVar9 = !bVar9;
    }
  }
  bVar6 = (uVar3 & 0x80000000) == 0;
  uVar2 = uVar3 << 1 | (uint)((uVar1 & 0x20000000) != 0);
  if (bVar9 == false && !bVar4) {
    uVar3 = uVar2 - in_r0;
    bVar10 = uVar2 < uVar3;
    if (!bVar6) {
      bVar10 = !bVar10;
    }
  }
  else if (bVar9 == false && bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar10 = uVar3 < uVar2;
    if (bVar6) {
      bVar10 = !bVar10;
    }
  }
  else if (bVar9 == true && !bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar10 = uVar3 < uVar2;
    if (!bVar6) {
      bVar10 = !bVar10;
    }
  }
  else {
    uVar3 = uVar2 - in_r0;
    bVar10 = uVar2 < uVar3;
    if (bVar6) {
      bVar10 = !bVar10;
    }
  }
  bVar6 = (uVar3 & 0x80000000) == 0;
  uVar2 = uVar3 << 1 | (uint)((uVar1 & 0x10000000) != 0);
  if (bVar10 == false && !bVar4) {
    uVar3 = uVar2 - in_r0;
    bVar11 = uVar2 < uVar3;
    if (!bVar6) {
      bVar11 = !bVar11;
    }
  }
  else if (bVar10 == false && bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar11 = uVar3 < uVar2;
    if (bVar6) {
      bVar11 = !bVar11;
    }
  }
  else if (bVar10 == true && !bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar11 = uVar3 < uVar2;
    if (!bVar6) {
      bVar11 = !bVar11;
    }
  }
  else {
    uVar3 = uVar2 - in_r0;
    bVar11 = uVar2 < uVar3;
    if (bVar6) {
      bVar11 = !bVar11;
    }
  }
  bVar6 = (uVar3 & 0x80000000) == 0;
  uVar2 = uVar3 << 1 | (uint)((uVar1 & 0x8000000) != 0);
  if (bVar11 == false && !bVar4) {
    uVar3 = uVar2 - in_r0;
    bVar12 = uVar2 < uVar3;
    if (!bVar6) {
      bVar12 = !bVar12;
    }
  }
  else if (bVar11 == false && bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar12 = uVar3 < uVar2;
    if (bVar6) {
      bVar12 = !bVar12;
    }
  }
  else if (bVar11 == true && !bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar12 = uVar3 < uVar2;
    if (!bVar6) {
      bVar12 = !bVar12;
    }
  }
  else {
    uVar3 = uVar2 - in_r0;
    bVar12 = uVar2 < uVar3;
    if (bVar6) {
      bVar12 = !bVar12;
    }
  }
  bVar6 = (uVar3 & 0x80000000) == 0;
  uVar2 = uVar3 << 1 | (uint)((uVar1 & 0x4000000) != 0);
  if (bVar12 == false && !bVar4) {
    uVar3 = uVar2 - in_r0;
    bVar13 = uVar2 < uVar3;
    if (!bVar6) {
      bVar13 = !bVar13;
    }
  }
  else if (bVar12 == false && bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar13 = uVar3 < uVar2;
    if (bVar6) {
      bVar13 = !bVar13;
    }
  }
  else if (bVar12 == true && !bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar13 = uVar3 < uVar2;
    if (!bVar6) {
      bVar13 = !bVar13;
    }
  }
  else {
    uVar3 = uVar2 - in_r0;
    bVar13 = uVar2 < uVar3;
    if (bVar6) {
      bVar13 = !bVar13;
    }
  }
  bVar6 = (uVar3 & 0x80000000) == 0;
  uVar2 = uVar3 << 1 | (uint)((uVar1 & 0x2000000) != 0);
  if (bVar13 == false && !bVar4) {
    uVar3 = uVar2 - in_r0;
    bVar14 = uVar2 < uVar3;
    if (!bVar6) {
      bVar14 = !bVar14;
    }
  }
  else if (bVar13 == false && bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar14 = uVar3 < uVar2;
    if (bVar6) {
      bVar14 = !bVar14;
    }
  }
  else if (bVar13 == true && !bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar14 = uVar3 < uVar2;
    if (!bVar6) {
      bVar14 = !bVar14;
    }
  }
  else {
    uVar3 = uVar2 - in_r0;
    bVar14 = uVar2 < uVar3;
    if (bVar6) {
      bVar14 = !bVar14;
    }
  }
  bVar6 = (uVar3 & 0x80000000) == 0;
  uVar2 = uVar3 << 1 | (uint)((uVar1 & 0x1000000) != 0);
  if (bVar14 == false && !bVar4) {
    uVar3 = uVar2 - in_r0;
    bVar15 = uVar2 < uVar3;
    if (!bVar6) {
      bVar15 = !bVar15;
    }
  }
  else if (bVar14 == false && bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar15 = uVar3 < uVar2;
    if (bVar6) {
      bVar15 = !bVar15;
    }
  }
  else if (bVar14 == true && !bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar15 = uVar3 < uVar2;
    if (!bVar6) {
      bVar15 = !bVar15;
    }
  }
  else {
    uVar3 = uVar2 - in_r0;
    bVar15 = uVar2 < uVar3;
    if (bVar6) {
      bVar15 = !bVar15;
    }
  }
  bVar6 = (uVar3 & 0x80000000) == 0;
  uVar2 = uVar3 << 1 | (uint)((uVar1 & 0x800000) != 0);
  if (bVar15 == false && !bVar4) {
    uVar3 = uVar2 - in_r0;
    bVar16 = uVar2 < uVar3;
    if (!bVar6) {
      bVar16 = !bVar16;
    }
  }
  else if (bVar15 == false && bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar16 = uVar3 < uVar2;
    if (bVar6) {
      bVar16 = !bVar16;
    }
  }
  else if (bVar15 == true && !bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar16 = uVar3 < uVar2;
    if (!bVar6) {
      bVar16 = !bVar16;
    }
  }
  else {
    uVar3 = uVar2 - in_r0;
    bVar16 = uVar2 < uVar3;
    if (bVar6) {
      bVar16 = !bVar16;
    }
  }
  bVar6 = (uVar3 & 0x80000000) == 0;
  uVar2 = uVar3 << 1 | (uint)((uVar1 & 0x400000) != 0);
  if (bVar16 == false && !bVar4) {
    uVar3 = uVar2 - in_r0;
    bVar17 = uVar2 < uVar3;
    if (!bVar6) {
      bVar17 = !bVar17;
    }
  }
  else if (bVar16 == false && bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar17 = uVar3 < uVar2;
    if (bVar6) {
      bVar17 = !bVar17;
    }
  }
  else if (bVar16 == true && !bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar17 = uVar3 < uVar2;
    if (!bVar6) {
      bVar17 = !bVar17;
    }
  }
  else {
    uVar3 = uVar2 - in_r0;
    bVar17 = uVar2 < uVar3;
    if (bVar6) {
      bVar17 = !bVar17;
    }
  }
  bVar6 = (uVar3 & 0x80000000) == 0;
  uVar2 = uVar3 << 1 | (uint)((uVar1 & 0x200000) != 0);
  if (bVar17 == false && !bVar4) {
    uVar3 = uVar2 - in_r0;
    bVar18 = uVar2 < uVar3;
    if (!bVar6) {
      bVar18 = !bVar18;
    }
  }
  else if (bVar17 == false && bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar18 = uVar3 < uVar2;
    if (bVar6) {
      bVar18 = !bVar18;
    }
  }
  else if (bVar17 == true && !bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar18 = uVar3 < uVar2;
    if (!bVar6) {
      bVar18 = !bVar18;
    }
  }
  else {
    uVar3 = uVar2 - in_r0;
    bVar18 = uVar2 < uVar3;
    if (bVar6) {
      bVar18 = !bVar18;
    }
  }
  bVar6 = (uVar3 & 0x80000000) == 0;
  uVar2 = uVar3 << 1 | (uint)((uVar1 & 0x100000) != 0);
  if (bVar18 == false && !bVar4) {
    uVar3 = uVar2 - in_r0;
    bVar19 = uVar2 < uVar3;
    if (!bVar6) {
      bVar19 = !bVar19;
    }
  }
  else if (bVar18 == false && bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar19 = uVar3 < uVar2;
    if (bVar6) {
      bVar19 = !bVar19;
    }
  }
  else if (bVar18 == true && !bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar19 = uVar3 < uVar2;
    if (!bVar6) {
      bVar19 = !bVar19;
    }
  }
  else {
    uVar3 = uVar2 - in_r0;
    bVar19 = uVar2 < uVar3;
    if (bVar6) {
      bVar19 = !bVar19;
    }
  }
  bVar6 = (uVar3 & 0x80000000) == 0;
  uVar2 = uVar3 << 1 | (uint)((uVar1 & 0x80000) != 0);
  if (bVar19 == false && !bVar4) {
    uVar3 = uVar2 - in_r0;
    bVar20 = uVar2 < uVar3;
    if (!bVar6) {
      bVar20 = !bVar20;
    }
  }
  else if (bVar19 == false && bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar20 = uVar3 < uVar2;
    if (bVar6) {
      bVar20 = !bVar20;
    }
  }
  else if (bVar19 == true && !bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar20 = uVar3 < uVar2;
    if (!bVar6) {
      bVar20 = !bVar20;
    }
  }
  else {
    uVar3 = uVar2 - in_r0;
    bVar20 = uVar2 < uVar3;
    if (bVar6) {
      bVar20 = !bVar20;
    }
  }
  bVar6 = (uVar3 & 0x80000000) == 0;
  uVar2 = uVar3 << 1 | (uint)((uVar1 & 0x40000) != 0);
  if (bVar20 == false && !bVar4) {
    uVar3 = uVar2 - in_r0;
    bVar21 = uVar2 < uVar3;
    if (!bVar6) {
      bVar21 = !bVar21;
    }
  }
  else if (bVar20 == false && bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar21 = uVar3 < uVar2;
    if (bVar6) {
      bVar21 = !bVar21;
    }
  }
  else if (bVar20 == true && !bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar21 = uVar3 < uVar2;
    if (!bVar6) {
      bVar21 = !bVar21;
    }
  }
  else {
    uVar3 = uVar2 - in_r0;
    bVar21 = uVar2 < uVar3;
    if (bVar6) {
      bVar21 = !bVar21;
    }
  }
  bVar6 = (uVar3 & 0x80000000) == 0;
  uVar2 = uVar3 << 1 | (uint)((uVar1 & 0x20000) != 0);
  if (bVar21 == false && !bVar4) {
    uVar3 = uVar2 - in_r0;
    bVar22 = uVar2 < uVar3;
    if (!bVar6) {
      bVar22 = !bVar22;
    }
  }
  else if (bVar21 == false && bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar22 = uVar3 < uVar2;
    if (bVar6) {
      bVar22 = !bVar22;
    }
  }
  else if (bVar21 == true && !bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar22 = uVar3 < uVar2;
    if (!bVar6) {
      bVar22 = !bVar22;
    }
  }
  else {
    uVar3 = uVar2 - in_r0;
    bVar22 = uVar2 < uVar3;
    if (bVar6) {
      bVar22 = !bVar22;
    }
  }
  bVar6 = (uVar3 & 0x80000000) == 0;
  uVar2 = uVar3 << 1 | (uint)((uVar1 & 0x10000) != 0);
  if (bVar22 == false && !bVar4) {
    uVar3 = uVar2 - in_r0;
    bVar23 = uVar2 < uVar3;
    if (!bVar6) {
      bVar23 = !bVar23;
    }
  }
  else if (bVar22 == false && bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar23 = uVar3 < uVar2;
    if (bVar6) {
      bVar23 = !bVar23;
    }
  }
  else if (bVar22 == true && !bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar23 = uVar3 < uVar2;
    if (!bVar6) {
      bVar23 = !bVar23;
    }
  }
  else {
    uVar3 = uVar2 - in_r0;
    bVar23 = uVar2 < uVar3;
    if (bVar6) {
      bVar23 = !bVar23;
    }
  }
  bVar6 = (uVar3 & 0x80000000) == 0;
  uVar2 = uVar3 << 1 | (uint)((uVar1 & 0x8000) != 0);
  if (bVar23 == false && !bVar4) {
    uVar3 = uVar2 - in_r0;
    bVar24 = uVar2 < uVar3;
    if (!bVar6) {
      bVar24 = !bVar24;
    }
  }
  else if (bVar23 == false && bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar24 = uVar3 < uVar2;
    if (bVar6) {
      bVar24 = !bVar24;
    }
  }
  else if (bVar23 == true && !bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar24 = uVar3 < uVar2;
    if (!bVar6) {
      bVar24 = !bVar24;
    }
  }
  else {
    uVar3 = uVar2 - in_r0;
    bVar24 = uVar2 < uVar3;
    if (bVar6) {
      bVar24 = !bVar24;
    }
  }
  bVar6 = (uVar3 & 0x80000000) == 0;
  uVar2 = uVar3 << 1 | (uint)((uVar1 & 0x4000) != 0);
  if (bVar24 == false && !bVar4) {
    uVar3 = uVar2 - in_r0;
    bVar25 = uVar2 < uVar3;
    if (!bVar6) {
      bVar25 = !bVar25;
    }
  }
  else if (bVar24 == false && bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar25 = uVar3 < uVar2;
    if (bVar6) {
      bVar25 = !bVar25;
    }
  }
  else if (bVar24 == true && !bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar25 = uVar3 < uVar2;
    if (!bVar6) {
      bVar25 = !bVar25;
    }
  }
  else {
    uVar3 = uVar2 - in_r0;
    bVar25 = uVar2 < uVar3;
    if (bVar6) {
      bVar25 = !bVar25;
    }
  }
  bVar6 = (uVar3 & 0x80000000) == 0;
  uVar2 = uVar3 << 1 | (uint)((uVar1 & 0x2000) != 0);
  if (bVar25 == false && !bVar4) {
    uVar3 = uVar2 - in_r0;
    bVar26 = uVar2 < uVar3;
    if (!bVar6) {
      bVar26 = !bVar26;
    }
  }
  else if (bVar25 == false && bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar26 = uVar3 < uVar2;
    if (bVar6) {
      bVar26 = !bVar26;
    }
  }
  else if (bVar25 == true && !bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar26 = uVar3 < uVar2;
    if (!bVar6) {
      bVar26 = !bVar26;
    }
  }
  else {
    uVar3 = uVar2 - in_r0;
    bVar26 = uVar2 < uVar3;
    if (bVar6) {
      bVar26 = !bVar26;
    }
  }
  bVar6 = (uVar3 & 0x80000000) == 0;
  uVar2 = uVar3 << 1 | (uint)((uVar1 & 0x1000) != 0);
  if (bVar26 == false && !bVar4) {
    uVar3 = uVar2 - in_r0;
    bVar27 = uVar2 < uVar3;
    if (!bVar6) {
      bVar27 = !bVar27;
    }
  }
  else if (bVar26 == false && bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar27 = uVar3 < uVar2;
    if (bVar6) {
      bVar27 = !bVar27;
    }
  }
  else if (bVar26 == true && !bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar27 = uVar3 < uVar2;
    if (!bVar6) {
      bVar27 = !bVar27;
    }
  }
  else {
    uVar3 = uVar2 - in_r0;
    bVar27 = uVar2 < uVar3;
    if (bVar6) {
      bVar27 = !bVar27;
    }
  }
  bVar6 = (uVar3 & 0x80000000) == 0;
  uVar2 = uVar3 << 1 | (uint)((uVar1 & 0x800) != 0);
  if (bVar27 == false && !bVar4) {
    uVar3 = uVar2 - in_r0;
    bVar28 = uVar2 < uVar3;
    if (!bVar6) {
      bVar28 = !bVar28;
    }
  }
  else if (bVar27 == false && bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar28 = uVar3 < uVar2;
    if (bVar6) {
      bVar28 = !bVar28;
    }
  }
  else if (bVar27 == true && !bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar28 = uVar3 < uVar2;
    if (!bVar6) {
      bVar28 = !bVar28;
    }
  }
  else {
    uVar3 = uVar2 - in_r0;
    bVar28 = uVar2 < uVar3;
    if (bVar6) {
      bVar28 = !bVar28;
    }
  }
  bVar6 = (uVar3 & 0x80000000) == 0;
  uVar2 = uVar3 << 1 | (uint)((uVar1 & 0x400) != 0);
  if (bVar28 == false && !bVar4) {
    uVar3 = uVar2 - in_r0;
    bVar29 = uVar2 < uVar3;
    if (!bVar6) {
      bVar29 = !bVar29;
    }
  }
  else if (bVar28 == false && bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar29 = uVar3 < uVar2;
    if (bVar6) {
      bVar29 = !bVar29;
    }
  }
  else if (bVar28 == true && !bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar29 = uVar3 < uVar2;
    if (!bVar6) {
      bVar29 = !bVar29;
    }
  }
  else {
    uVar3 = uVar2 - in_r0;
    bVar29 = uVar2 < uVar3;
    if (bVar6) {
      bVar29 = !bVar29;
    }
  }
  bVar6 = (uVar3 & 0x80000000) == 0;
  uVar2 = uVar3 << 1 | (uint)((uVar1 & 0x200) != 0);
  if (bVar29 == false && !bVar4) {
    uVar3 = uVar2 - in_r0;
    bVar30 = uVar2 < uVar3;
    if (!bVar6) {
      bVar30 = !bVar30;
    }
  }
  else if (bVar29 == false && bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar30 = uVar3 < uVar2;
    if (bVar6) {
      bVar30 = !bVar30;
    }
  }
  else if (bVar29 == true && !bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar30 = uVar3 < uVar2;
    if (!bVar6) {
      bVar30 = !bVar30;
    }
  }
  else {
    uVar3 = uVar2 - in_r0;
    bVar30 = uVar2 < uVar3;
    if (bVar6) {
      bVar30 = !bVar30;
    }
  }
  bVar6 = (uVar3 & 0x80000000) == 0;
  uVar2 = uVar3 << 1 | (uint)((uVar1 & 0x100) != 0);
  if (bVar30 == false && !bVar4) {
    uVar3 = uVar2 - in_r0;
    bVar31 = uVar2 < uVar3;
    if (!bVar6) {
      bVar31 = !bVar31;
    }
  }
  else if (bVar30 == false && bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar31 = uVar3 < uVar2;
    if (bVar6) {
      bVar31 = !bVar31;
    }
  }
  else if (bVar30 == true && !bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar31 = uVar3 < uVar2;
    if (!bVar6) {
      bVar31 = !bVar31;
    }
  }
  else {
    uVar3 = uVar2 - in_r0;
    bVar31 = uVar2 < uVar3;
    if (bVar6) {
      bVar31 = !bVar31;
    }
  }
  bVar6 = (uVar3 & 0x80000000) == 0;
  uVar2 = uVar3 << 1 | (uint)((uVar1 & 0x80) != 0);
  if (bVar31 == false && !bVar4) {
    uVar3 = uVar2 - in_r0;
    bVar32 = uVar2 < uVar3;
    if (!bVar6) {
      bVar32 = !bVar32;
    }
  }
  else if (bVar31 == false && bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar32 = uVar3 < uVar2;
    if (bVar6) {
      bVar32 = !bVar32;
    }
  }
  else if (bVar31 == true && !bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar32 = uVar3 < uVar2;
    if (!bVar6) {
      bVar32 = !bVar32;
    }
  }
  else {
    uVar3 = uVar2 - in_r0;
    bVar32 = uVar2 < uVar3;
    if (bVar6) {
      bVar32 = !bVar32;
    }
  }
  bVar6 = (uVar3 & 0x80000000) == 0;
  uVar2 = uVar3 << 1 | (uint)((uVar1 & 0x40) != 0);
  if (bVar32 == false && !bVar4) {
    uVar3 = uVar2 - in_r0;
    bVar33 = uVar2 < uVar3;
    if (!bVar6) {
      bVar33 = !bVar33;
    }
  }
  else if (bVar32 == false && bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar33 = uVar3 < uVar2;
    if (bVar6) {
      bVar33 = !bVar33;
    }
  }
  else if (bVar32 == true && !bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar33 = uVar3 < uVar2;
    if (!bVar6) {
      bVar33 = !bVar33;
    }
  }
  else {
    uVar3 = uVar2 - in_r0;
    bVar33 = uVar2 < uVar3;
    if (bVar6) {
      bVar33 = !bVar33;
    }
  }
  bVar6 = (uVar3 & 0x80000000) == 0;
  uVar2 = uVar3 << 1 | (uint)((uVar1 & 0x20) != 0);
  if (bVar33 == false && !bVar4) {
    uVar3 = uVar2 - in_r0;
    bVar34 = uVar2 < uVar3;
    if (!bVar6) {
      bVar34 = !bVar34;
    }
  }
  else if (bVar33 == false && bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar34 = uVar3 < uVar2;
    if (bVar6) {
      bVar34 = !bVar34;
    }
  }
  else if (bVar33 == true && !bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar34 = uVar3 < uVar2;
    if (!bVar6) {
      bVar34 = !bVar34;
    }
  }
  else {
    uVar3 = uVar2 - in_r0;
    bVar34 = uVar2 < uVar3;
    if (bVar6) {
      bVar34 = !bVar34;
    }
  }
  bVar6 = (uVar3 & 0x80000000) == 0;
  uVar2 = uVar3 << 1 | (uint)((uVar1 & 0x10) != 0);
  if (bVar34 == false && !bVar4) {
    uVar3 = uVar2 - in_r0;
    bVar35 = uVar2 < uVar3;
    if (!bVar6) {
      bVar35 = !bVar35;
    }
  }
  else if (bVar34 == false && bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar35 = uVar3 < uVar2;
    if (bVar6) {
      bVar35 = !bVar35;
    }
  }
  else if (bVar34 == true && !bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar35 = uVar3 < uVar2;
    if (!bVar6) {
      bVar35 = !bVar35;
    }
  }
  else {
    uVar3 = uVar2 - in_r0;
    bVar35 = uVar2 < uVar3;
    if (bVar6) {
      bVar35 = !bVar35;
    }
  }
  bVar6 = (uVar3 & 0x80000000) == 0;
  uVar2 = uVar3 << 1 | (uint)((uVar1 & 8) != 0);
  if (bVar35 == false && !bVar4) {
    uVar3 = uVar2 - in_r0;
    bVar36 = uVar2 < uVar3;
    if (!bVar6) {
      bVar36 = !bVar36;
    }
  }
  else if (bVar35 == false && bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar36 = uVar3 < uVar2;
    if (bVar6) {
      bVar36 = !bVar36;
    }
  }
  else if (bVar35 == true && !bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar36 = uVar3 < uVar2;
    if (!bVar6) {
      bVar36 = !bVar36;
    }
  }
  else {
    uVar3 = uVar2 - in_r0;
    bVar36 = uVar2 < uVar3;
    if (bVar6) {
      bVar36 = !bVar36;
    }
  }
  bVar6 = (uVar3 & 0x80000000) == 0;
  uVar2 = uVar3 << 1 | (uint)((uVar1 & 4) != 0);
  if (bVar36 == false && !bVar4) {
    uVar3 = uVar2 - in_r0;
    bVar37 = uVar2 < uVar3;
    if (!bVar6) {
      bVar37 = !bVar37;
    }
  }
  else if (bVar36 == false && bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar37 = uVar3 < uVar2;
    if (bVar6) {
      bVar37 = !bVar37;
    }
  }
  else if (bVar36 == true && !bVar4) {
    uVar3 = uVar2 + in_r0;
    bVar37 = uVar3 < uVar2;
    if (!bVar6) {
      bVar37 = !bVar37;
    }
  }
  else {
    uVar3 = uVar2 - in_r0;
    bVar37 = uVar2 < uVar3;
    if (bVar6) {
      bVar37 = !bVar37;
    }
  }
  bVar6 = (uVar3 & 0x80000000) == 0;
  uVar2 = uVar3 << 1 | (uint)((uVar1 & 2) != 0);
  if (bVar37 == false && !bVar4) {
    bVar38 = uVar2 < uVar2 - in_r0;
    if (!bVar6) {
      bVar38 = !bVar38;
    }
  }
  else if (bVar37 == false && bVar4) {
    bVar38 = uVar2 + in_r0 < uVar2;
    if (bVar6) {
      bVar38 = !bVar38;
    }
  }
  else if (bVar37 == true && !bVar4) {
    bVar38 = uVar2 + in_r0 < uVar2;
    if (!bVar6) {
      bVar38 = !bVar38;
    }
  }
  else {
    bVar38 = uVar2 < uVar2 - in_r0;
    if (bVar6) {
      bVar38 = !bVar38;
    }
  }
  return ((((((((((((((((((((((((((((((((uint)(bVar7 == bVar4) << 1 | (uint)(bVar8 == bVar4)) << 1 |
                                      (uint)(bVar9 == bVar4)) << 1 | (uint)(bVar10 == bVar4)) << 1 |
                                    (uint)(bVar11 == bVar4)) << 1 | (uint)(bVar12 == bVar4)) << 1 |
                                  (uint)(bVar13 == bVar4)) << 1 | (uint)(bVar14 == bVar4)) << 1 |
                                (uint)(bVar15 == bVar4)) << 1 | (uint)(bVar16 == bVar4)) << 1 |
                              (uint)(bVar17 == bVar4)) << 1 | (uint)(bVar18 == bVar4)) << 1 |
                            (uint)(bVar19 == bVar4)) << 1 | (uint)(bVar20 == bVar4)) << 1 |
                          (uint)(bVar21 == bVar4)) << 1 | (uint)(bVar22 == bVar4)) << 1 |
                        (uint)(bVar23 == bVar4)) << 1 | (uint)(bVar24 == bVar4)) << 1 |
                      (uint)(bVar25 == bVar4)) << 1 | (uint)(bVar26 == bVar4)) << 1 |
                    (uint)(bVar27 == bVar4)) << 1 | (uint)(bVar28 == bVar4)) << 1 |
                  (uint)(bVar29 == bVar4)) << 1 | (uint)(bVar30 == bVar4)) << 1 |
                (uint)(bVar31 == bVar4)) << 1 | (uint)(bVar32 == bVar4)) << 1 |
              (uint)(bVar33 == bVar4)) << 1 | (uint)(bVar34 == bVar4)) << 1 |
            (uint)(bVar35 == bVar4)) << 1 | (uint)(bVar36 == bVar4)) << 1 | (uint)(bVar37 == bVar4))
          << 1 | (uint)(bVar38 == bVar4)) + (uint)(bVar4 != bVar5);
}

