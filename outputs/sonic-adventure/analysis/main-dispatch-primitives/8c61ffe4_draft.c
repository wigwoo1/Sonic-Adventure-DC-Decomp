/* Ghidra draft only; not buildable/verified source. */

void FUN_8c61ffe4(double param_1,ushort *param_2)

{
  ushort uVar1;
  int iVar2;
  int *piVar3;
  int *piVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  uint *unaff_r8;
  uint *puVar9;
  int iVar10;
  int iVar11;
  ushort *puVar12;
  ushort *puVar13;
  ushort *puVar14;
  ushort *puVar15;
  double in_dr0;
  double dVar16;
  undefined4 uVar17;
  float fVar18;
  double in_dr2;
  uint uVar20;
  double dVar19;
  double dVar21;
  undefined4 uVar22;
  float fVar23;
  undefined8 in_xd2;
  double in_xd4;
  bool bVar24;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  piVar3 = DAT_8c620144;
  iVar2 = DAT_8c620140;
  puVar12 = *(ushort **)(param_2 + 2);
  uVar5 = *param_2 & DAT_8c620148;
  if (uVar5 == 0) {
    *DAT_8c620144 = *DAT_8c620144 + (uint)param_2[1] * 3;
    iVar10 = 0;
    while (uVar5 = DAT_8c620154, iVar10 < (int)(uint)param_2[1]) {
      uVar1 = *puVar12;
      puVar15 = puVar12 + 1;
      puVar13 = puVar12 + 2;
      iVar8 = *DAT_8c620150;
      puVar12 = puVar12 + 3;
      iVar7 = (uint)*puVar15 * 0x20 + iVar8;
      if (in_FPSCR_SZ == '\0') {
        dVar21 = (double)CONCAT44(*(undefined4 *)(iVar7 + 0x10),SUB84(param_1,0));
      }
      else {
        dVar21 = *(double *)(iVar7 + 0x10);
      }
      iVar11 = (uint)uVar1 * 0x20 + iVar8;
      uVar17 = (undefined4)((ulonglong)dVar21 >> 0x20);
      dVar19 = dVar21;
      if (in_FPSCR_SZ == '\0') {
        dVar19 = (double)CONCAT44(uVar17,*(undefined4 *)(iVar11 + 0x10));
      }
      if (in_FPSCR_PR == '\0') {
        dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) - SUB84(dVar19,0),
                                  SUB84(dVar19,0));
      }
      else {
        dVar19 = dVar19 - dVar19;
      }
      iVar8 = (uint)*puVar13 * 0x20 + iVar8;
      if (in_FPSCR_SZ == '\0') {
        fVar23 = *(float *)(iVar7 + 0x14);
        dVar21 = (double)CONCAT44(uVar17,fVar23);
        dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),*(undefined4 *)(iVar8 + 0x14));
        in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar23);
      }
      else {
        fVar23 = SUB84(dVar21,0);
      }
      if (in_FPSCR_PR == '\0') {
        fVar18 = SUB84(dVar19,0) - fVar23;
        dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) * fVar18,fVar18);
      }
      else {
        dVar19 = (dVar19 - dVar21) * (dVar19 - dVar21);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),*(undefined4 *)(iVar11 + 0x14));
      }
      if (in_FPSCR_PR == '\0') {
        dVar16 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),SUB84(in_dr0,0) - SUB84(dVar19,0)
                                 );
      }
      else {
        dVar16 = in_dr0 - dVar19;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),*(undefined4 *)(iVar8 + 0x10));
      }
      if (in_FPSCR_PR == '\0') {
        dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),
                                  SUB84(dVar19,0) - (float)((ulonglong)dVar21 >> 0x20));
      }
      else {
        dVar19 = dVar19 - dVar21;
      }
      uVar20 = (uint)((ulonglong)dVar19 >> 0x20);
      param_1 = dVar19;
      if (in_FPSCR_SZ == '\0') {
        param_1 = (double)CONCAT44(uVar20,fVar23);
      }
      if (in_FPSCR_PR == '\0') {
        in_dr0 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),SUB84(dVar16,0) * SUB84(dVar19,0)
                                 );
      }
      else {
        in_dr0 = dVar16 * dVar19;
      }
      if (in_FPSCR_PR == '\0') {
        fVar23 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
        param_1 = (double)CONCAT44(fVar23,SUB84(param_1,0));
        bVar24 = 0.0 < fVar23;
      }
      else {
        param_1 = param_1 - in_dr0;
        bVar24 = (double)((ulonglong)uVar20 << 0x20) < param_1;
      }
      iVar10 = iVar10 + 1;
      if (!bVar24) {
        if (in_FPSCR_SZ == '\0') {
          unaff_r8[7] = *(uint *)(iVar2 + 0x1c);
          unaff_r8[6] = *(uint *)(iVar2 + 0x18);
          unaff_r8[5] = *(uint *)(iVar2 + 0x14);
          unaff_r8[4] = *(uint *)(iVar2 + 0x10);
          unaff_r8[3] = *(uint *)(iVar11 + 0x18);
          unaff_r8[2] = *(uint *)(iVar11 + 0x14);
          puVar9 = unaff_r8 + 1;
          *puVar9 = *(uint *)(iVar11 + 0x10);
        }
        else {
          *(undefined8 *)(unaff_r8 + 6) = *(undefined8 *)(iVar2 + 0x1c);
          *(undefined8 *)(unaff_r8 + 4) = *(undefined8 *)(iVar2 + 0x18);
          *(undefined8 *)(unaff_r8 + 2) = *(undefined8 *)(iVar2 + 0x14);
          *(undefined8 *)unaff_r8 = *(undefined8 *)(iVar2 + 0x10);
          *(undefined8 *)(unaff_r8 + -2) = *(undefined8 *)(iVar11 + 0x18);
          *(undefined8 *)(unaff_r8 + -4) = *(undefined8 *)(iVar11 + 0x14);
          puVar9 = unaff_r8 + -6;
          *(undefined8 *)puVar9 = *(undefined8 *)(iVar11 + 0x10);
        }
        puVar9[-1] = uVar5;
        if (in_FPSCR_SZ == '\0') {
          puVar9[0xe] = *(uint *)(iVar2 + 0x1c);
          puVar9[0xd] = *(uint *)(iVar2 + 0x18);
          puVar9[0xc] = *(uint *)(iVar2 + 0x14);
          puVar9[0xb] = *(uint *)(iVar2 + 0x10);
          puVar9[10] = *(uint *)(iVar7 + 0x18);
          puVar9[9] = *(uint *)(iVar7 + 0x14);
          puVar9 = puVar9 + 8;
          *puVar9 = *(uint *)(iVar7 + 0x10);
        }
        else {
          *(undefined8 *)(puVar9 + 0xd) = *(undefined8 *)(iVar2 + 0x1c);
          *(undefined8 *)(puVar9 + 0xb) = *(undefined8 *)(iVar2 + 0x18);
          *(undefined8 *)(puVar9 + 9) = *(undefined8 *)(iVar2 + 0x14);
          *(undefined8 *)(puVar9 + 7) = *(undefined8 *)(iVar2 + 0x10);
          *(undefined8 *)(puVar9 + 5) = *(undefined8 *)(iVar7 + 0x18);
          *(undefined8 *)(puVar9 + 3) = *(undefined8 *)(iVar7 + 0x14);
          puVar9 = puVar9 + 1;
          *(undefined8 *)puVar9 = *(undefined8 *)(iVar7 + 0x10);
        }
        puVar9[-1] = uVar5;
        uVar5 = DAT_8c620158;
        if (in_FPSCR_SZ == '\0') {
          puVar9[0xe] = *(uint *)(iVar2 + 0x1c);
          puVar9[0xd] = *(uint *)(iVar2 + 0x18);
          puVar9[0xc] = *(uint *)(iVar2 + 0x14);
          puVar9[0xb] = *(uint *)(iVar2 + 0x10);
          puVar9[10] = *(uint *)(iVar8 + 0x18);
          puVar9[9] = *(uint *)(iVar8 + 0x14);
          puVar9 = puVar9 + 8;
          *puVar9 = *(uint *)(iVar8 + 0x10);
        }
        else {
          *(undefined8 *)(puVar9 + 0xd) = *(undefined8 *)(iVar2 + 0x1c);
          *(undefined8 *)(puVar9 + 0xb) = *(undefined8 *)(iVar2 + 0x18);
          *(undefined8 *)(puVar9 + 9) = *(undefined8 *)(iVar2 + 0x14);
          *(undefined8 *)(puVar9 + 7) = *(undefined8 *)(iVar2 + 0x10);
          *(undefined8 *)(puVar9 + 5) = *(undefined8 *)(iVar8 + 0x18);
          *(undefined8 *)(puVar9 + 3) = *(undefined8 *)(iVar8 + 0x14);
          puVar9 = puVar9 + 1;
          *(undefined8 *)puVar9 = *(undefined8 *)(iVar8 + 0x10);
        }
        puVar9[-1] = uVar5;
        unaff_r8 = puVar9 + 7;
        *DAT_8c62015c = *DAT_8c62015c + 3;
      }
    }
  }
  else if (uVar5 == (int)DAT_8c62013c) {
    *DAT_8c620144 = *DAT_8c620144 + (uint)param_2[1] * 4;
    for (iVar10 = 0; uVar5 = DAT_8c620414, uVar17 = SUB84(in_dr2,0), iVar10 < (int)(uint)param_2[1];
        iVar10 = iVar10 + 1) {
      uVar1 = *puVar12;
      puVar15 = puVar12 + 1;
      puVar13 = puVar12 + 2;
      iVar11 = *DAT_8c620410;
      puVar14 = puVar12 + 3;
      puVar12 = puVar12 + 4;
      iVar7 = (uint)*puVar15 * 0x20 + iVar11;
      iVar8 = (uint)uVar1 * 0x20 + iVar11;
      if (in_FPSCR_SZ == '\0') {
        dVar21 = (double)CONCAT44(*(undefined4 *)(iVar7 + 0x10),SUB84(param_1,0));
        uVar17 = *(undefined4 *)(iVar8 + 0x10);
      }
      else {
        dVar21 = *(double *)(iVar7 + 0x10);
      }
      uVar22 = (undefined4)((ulonglong)dVar21 >> 0x20);
      dVar19 = dVar21;
      if (in_FPSCR_SZ == '\0') {
        dVar19 = (double)CONCAT44(uVar22,uVar17);
      }
      if (in_FPSCR_PR == '\0') {
        dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) - SUB84(dVar19,0),
                                  SUB84(dVar19,0));
      }
      else {
        dVar19 = dVar19 - dVar19;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar21 = (double)CONCAT44(uVar22,*(undefined4 *)(iVar7 + 0x14));
      }
      iVar6 = (uint)*puVar13 * 0x20 + iVar11;
      fVar23 = SUB84(dVar21,0);
      if (in_FPSCR_SZ == '\0') {
        dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),*(undefined4 *)(iVar6 + 0x14));
        in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar23);
      }
      if (in_FPSCR_PR == '\0') {
        fVar18 = SUB84(dVar19,0) - fVar23;
        dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) * fVar18,fVar18);
      }
      else {
        dVar19 = (dVar19 - dVar21) * (dVar19 - dVar21);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),*(undefined4 *)(iVar8 + 0x14));
      }
      if (in_FPSCR_PR == '\0') {
        dVar16 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),SUB84(in_dr0,0) - SUB84(dVar19,0)
                                 );
      }
      else {
        dVar16 = in_dr0 - dVar19;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),*(undefined4 *)(iVar6 + 0x10));
      }
      if (in_FPSCR_PR == '\0') {
        dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),
                                  SUB84(dVar19,0) - (float)((ulonglong)dVar21 >> 0x20));
      }
      else {
        dVar19 = dVar19 - dVar21;
      }
      uVar20 = (uint)((ulonglong)dVar19 >> 0x20);
      param_1 = dVar19;
      if (in_FPSCR_SZ == '\0') {
        param_1 = (double)CONCAT44(uVar20,fVar23);
      }
      if (in_FPSCR_PR == '\0') {
        in_dr0 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),SUB84(dVar16,0) * SUB84(dVar19,0)
                                 );
      }
      else {
        in_dr0 = dVar16 * dVar19;
      }
      in_dr2 = (double)((ulonglong)uVar20 << 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar23 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
        param_1 = (double)CONCAT44(fVar23,SUB84(param_1,0));
        bVar24 = 0.0 < fVar23;
      }
      else {
        param_1 = param_1 - in_dr0;
        bVar24 = in_dr2 < param_1;
      }
      iVar11 = (uint)*puVar14 * 0x20 + iVar11;
      if (!bVar24) {
        if (in_FPSCR_SZ == '\0') {
          unaff_r8[7] = *(uint *)(iVar2 + 0x1c);
          unaff_r8[6] = *(uint *)(iVar2 + 0x18);
          unaff_r8[5] = *(uint *)(iVar2 + 0x14);
          unaff_r8[4] = *(uint *)(iVar2 + 0x10);
          unaff_r8[3] = *(uint *)(iVar8 + 0x18);
          unaff_r8[2] = *(uint *)(iVar8 + 0x14);
          in_dr2 = (double)(ulonglong)*(uint *)(iVar8 + 0x10);
          puVar9 = unaff_r8 + 1;
          *puVar9 = *(uint *)(iVar8 + 0x10);
        }
        else {
          *(undefined8 *)(unaff_r8 + 6) = *(undefined8 *)(iVar2 + 0x1c);
          *(undefined8 *)(unaff_r8 + 4) = *(undefined8 *)(iVar2 + 0x18);
          *(undefined8 *)(unaff_r8 + 2) = *(undefined8 *)(iVar2 + 0x14);
          *(undefined8 *)unaff_r8 = *(undefined8 *)(iVar2 + 0x10);
          *(undefined8 *)(unaff_r8 + -2) = *(undefined8 *)(iVar8 + 0x18);
          *(undefined8 *)(unaff_r8 + -4) = *(undefined8 *)(iVar8 + 0x14);
          puVar9 = unaff_r8 + -6;
          *(undefined8 *)puVar9 = *(undefined8 *)(iVar8 + 0x10);
        }
        puVar9[-1] = uVar5;
        if (in_FPSCR_SZ == '\0') {
          puVar9[0xe] = *(uint *)(iVar2 + 0x1c);
          puVar9[0xd] = *(uint *)(iVar2 + 0x18);
          puVar9[0xc] = *(uint *)(iVar2 + 0x14);
          puVar9[0xb] = *(uint *)(iVar2 + 0x10);
          puVar9[10] = *(uint *)(iVar7 + 0x18);
          puVar9[9] = *(uint *)(iVar7 + 0x14);
          in_dr2 = (double)(ulonglong)*(uint *)(iVar7 + 0x10);
          puVar9 = puVar9 + 8;
          *puVar9 = *(uint *)(iVar7 + 0x10);
        }
        else {
          *(undefined8 *)(puVar9 + 0xd) = *(undefined8 *)(iVar2 + 0x1c);
          *(undefined8 *)(puVar9 + 0xb) = *(undefined8 *)(iVar2 + 0x18);
          *(undefined8 *)(puVar9 + 9) = *(undefined8 *)(iVar2 + 0x14);
          *(undefined8 *)(puVar9 + 7) = *(undefined8 *)(iVar2 + 0x10);
          *(undefined8 *)(puVar9 + 5) = *(undefined8 *)(iVar7 + 0x18);
          *(undefined8 *)(puVar9 + 3) = *(undefined8 *)(iVar7 + 0x14);
          puVar9 = puVar9 + 1;
          *(undefined8 *)puVar9 = *(undefined8 *)(iVar7 + 0x10);
        }
        puVar9[-1] = uVar5;
        if (in_FPSCR_SZ == '\0') {
          puVar9[0xe] = *(uint *)(iVar2 + 0x1c);
          puVar9[0xd] = *(uint *)(iVar2 + 0x18);
          puVar9[0xc] = *(uint *)(iVar2 + 0x14);
          puVar9[0xb] = *(uint *)(iVar2 + 0x10);
          puVar9[10] = *(uint *)(iVar6 + 0x18);
          puVar9[9] = *(uint *)(iVar6 + 0x14);
          in_dr2 = (double)(ulonglong)*(uint *)(iVar6 + 0x10);
          puVar9 = puVar9 + 8;
          *puVar9 = *(uint *)(iVar6 + 0x10);
        }
        else {
          *(undefined8 *)(puVar9 + 0xd) = *(undefined8 *)(iVar2 + 0x1c);
          *(undefined8 *)(puVar9 + 0xb) = *(undefined8 *)(iVar2 + 0x18);
          *(undefined8 *)(puVar9 + 9) = *(undefined8 *)(iVar2 + 0x14);
          *(undefined8 *)(puVar9 + 7) = *(undefined8 *)(iVar2 + 0x10);
          *(undefined8 *)(puVar9 + 5) = *(undefined8 *)(iVar6 + 0x18);
          *(undefined8 *)(puVar9 + 3) = *(undefined8 *)(iVar6 + 0x14);
          puVar9 = puVar9 + 1;
          *(undefined8 *)puVar9 = *(undefined8 *)(iVar6 + 0x10);
        }
        puVar9[-1] = uVar5;
        uVar5 = DAT_8c620418;
        if (in_FPSCR_SZ == '\0') {
          puVar9[0xe] = *(uint *)(iVar2 + 0x1c);
          puVar9[0xd] = *(uint *)(iVar2 + 0x18);
          puVar9[0xc] = *(uint *)(iVar2 + 0x14);
          puVar9[0xb] = *(uint *)(iVar2 + 0x10);
          puVar9[10] = *(uint *)(iVar11 + 0x18);
          puVar9[9] = *(uint *)(iVar11 + 0x14);
          in_dr2 = (double)(ulonglong)*(uint *)(iVar11 + 0x10);
          puVar9 = puVar9 + 8;
          *puVar9 = *(uint *)(iVar11 + 0x10);
        }
        else {
          *(undefined8 *)(puVar9 + 0xd) = *(undefined8 *)(iVar2 + 0x1c);
          *(undefined8 *)(puVar9 + 0xb) = *(undefined8 *)(iVar2 + 0x18);
          *(undefined8 *)(puVar9 + 9) = *(undefined8 *)(iVar2 + 0x14);
          *(undefined8 *)(puVar9 + 7) = *(undefined8 *)(iVar2 + 0x10);
          *(undefined8 *)(puVar9 + 5) = *(undefined8 *)(iVar11 + 0x18);
          *(undefined8 *)(puVar9 + 3) = *(undefined8 *)(iVar11 + 0x14);
          puVar9 = puVar9 + 1;
          *(undefined8 *)puVar9 = *(undefined8 *)(iVar11 + 0x10);
        }
        puVar9[-1] = uVar5;
        unaff_r8 = puVar9 + 7;
        *DAT_8c62041c = *DAT_8c62041c + 4;
      }
    }
  }
  else if (uVar5 == DAT_8c62014c) {
    for (iVar10 = 0; piVar4 = DAT_8c620410, uVar17 = SUB84(param_1,0),
        iVar10 < (int)(uint)param_2[1]; iVar10 = iVar10 + 1) {
      puVar15 = puVar12 + 1;
      uVar5 = (uint)*puVar12;
      *piVar3 = *piVar3 + uVar5;
      iVar7 = *piVar4;
      iVar8 = (uint)puVar12[2] * 0x20 + iVar7;
      if (in_FPSCR_SZ == '\0') {
        uVar17 = *(undefined4 *)(iVar8 + 0x10);
      }
      else {
        in_xd4 = *(double *)(iVar8 + 0x10);
      }
      iVar11 = (uint)*puVar15 * 0x20 + iVar7;
      if (in_FPSCR_SZ == '\0') {
        dVar21 = (double)CONCAT44(uVar17,*(undefined4 *)(iVar11 + 0x10));
      }
      else {
        in_xd2 = *(undefined8 *)(iVar11 + 0x10);
        dVar21 = in_xd4;
      }
      if (in_FPSCR_PR == '\0') {
        dVar21 = (double)CONCAT44((float)((ulonglong)dVar21 >> 0x20) - SUB84(dVar21,0),
                                  SUB84(dVar21,0));
      }
      else {
        dVar21 = dVar21 - dVar21;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar19 = (double)CONCAT44(*(undefined4 *)(iVar8 + 0x14),uVar17);
      }
      else {
        dVar19 = *(double *)(iVar8 + 0x14);
      }
      iVar7 = (uint)puVar12[3] * 0x20 + iVar7;
      fVar23 = (float)((ulonglong)dVar19 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),*(undefined4 *)(iVar7 + 0x14));
        in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar23);
      }
      else {
        in_xd2 = *(undefined8 *)(iVar7 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        fVar23 = SUB84(dVar21,0) - fVar23;
        dVar21 = (double)CONCAT44((float)((ulonglong)dVar21 >> 0x20) * fVar23,fVar23);
      }
      else {
        dVar21 = (dVar21 - dVar19) * (dVar21 - dVar19);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),*(undefined4 *)(iVar11 + 0x14));
      }
      else {
        in_xd2 = *(undefined8 *)(iVar11 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        dVar16 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),SUB84(in_dr0,0) - SUB84(dVar21,0)
                                 );
      }
      else {
        dVar16 = in_dr0 - dVar21;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),*(undefined4 *)(iVar7 + 0x10));
      }
      else {
        in_xd2 = *(undefined8 *)(iVar7 + 0x10);
      }
      if (in_FPSCR_PR == '\0') {
        dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),SUB84(dVar21,0) - SUB84(dVar19,0)
                                 );
      }
      else {
        dVar21 = dVar21 - dVar19;
      }
      uVar20 = (uint)((ulonglong)dVar21 >> 0x20);
      param_1 = dVar21;
      if (in_FPSCR_SZ == '\0') {
        param_1 = (double)CONCAT44(uVar20,SUB84(dVar19,0));
      }
      if (in_FPSCR_PR == '\0') {
        in_dr0 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),SUB84(dVar16,0) * SUB84(dVar21,0)
                                 );
      }
      else {
        in_dr0 = dVar16 * dVar21;
      }
      dVar21 = (double)((ulonglong)uVar20 << 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar23 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
        param_1 = (double)CONCAT44(fVar23,SUB84(param_1,0));
        bVar24 = 0.0 < fVar23;
      }
      else {
        param_1 = param_1 - in_dr0;
        bVar24 = dVar21 < param_1;
      }
      if (bVar24) {
        puVar12 = puVar15 + uVar5;
      }
      else {
        *DAT_8c62041c = *DAT_8c62041c + uVar5;
        iVar7 = 0;
        if (0 < (int)(uVar5 - 1)) {
          do {
            uVar1 = *puVar15;
            puVar15 = puVar15 + 1;
            if (in_FPSCR_SZ == '\0') {
              dVar21 = (double)(ulonglong)*(uint *)(iVar2 + 0x1c);
            }
            else {
              in_xd2 = *(undefined8 *)(iVar2 + 0x1c);
            }
            iVar8 = *DAT_8c620410;
            if (in_FPSCR_SZ == '\0') {
              puVar9 = unaff_r8 + 7;
              *puVar9 = SUB84(dVar21,0);
              dVar21 = (double)(ulonglong)*(uint *)(iVar2 + 0x18);
            }
            else {
              puVar9 = unaff_r8 + 6;
              *(undefined8 *)puVar9 = in_xd2;
              in_xd2 = *(undefined8 *)(iVar2 + 0x18);
            }
            if (in_FPSCR_SZ == '\0') {
              puVar9 = puVar9 + -1;
              *puVar9 = SUB84(dVar21,0);
            }
            else {
              puVar9 = puVar9 + -2;
              *(undefined8 *)puVar9 = in_xd2;
            }
            uVar20 = DAT_8c620414;
            iVar8 = (uint)uVar1 * 0x20 + iVar8;
            if (in_FPSCR_SZ == '\0') {
              puVar9[-1] = *(uint *)(iVar2 + 0x14);
              puVar9[-2] = *(uint *)(iVar2 + 0x10);
              puVar9[-3] = *(uint *)(iVar8 + 0x18);
              puVar9[-4] = *(uint *)(iVar8 + 0x14);
              dVar21 = (double)(ulonglong)*(uint *)(iVar8 + 0x10);
              puVar9 = puVar9 + -5;
              *puVar9 = *(uint *)(iVar8 + 0x10);
            }
            else {
              *(undefined8 *)(puVar9 + -2) = *(undefined8 *)(iVar2 + 0x14);
              *(undefined8 *)(puVar9 + -4) = *(undefined8 *)(iVar2 + 0x10);
              *(undefined8 *)(puVar9 + -6) = *(undefined8 *)(iVar8 + 0x18);
              *(undefined8 *)(puVar9 + -8) = *(undefined8 *)(iVar8 + 0x14);
              in_xd2 = *(undefined8 *)(iVar8 + 0x10);
              puVar9 = puVar9 + -10;
              *(undefined8 *)puVar9 = in_xd2;
            }
            puVar9[-1] = uVar20;
            iVar7 = iVar7 + 1;
            unaff_r8 = puVar9 + 7;
          } while (iVar7 < (int)(uVar5 - 1));
        }
        uVar5 = SUB84(dVar21,0);
        uVar1 = *puVar15;
        puVar12 = puVar15 + 1;
        if (in_FPSCR_SZ == '\0') {
          uVar5 = *(uint *)(iVar2 + 0x1c);
        }
        else {
          in_xd2 = *(undefined8 *)(iVar2 + 0x1c);
        }
        iVar7 = *DAT_8c620410;
        if (in_FPSCR_SZ == '\0') {
          puVar9 = unaff_r8 + 7;
          *puVar9 = uVar5;
          uVar5 = *(uint *)(iVar2 + 0x18);
        }
        else {
          puVar9 = unaff_r8 + 6;
          *(undefined8 *)puVar9 = in_xd2;
          in_xd2 = *(undefined8 *)(iVar2 + 0x18);
        }
        if (in_FPSCR_SZ == '\0') {
          puVar9 = puVar9 + -1;
          *puVar9 = uVar5;
        }
        else {
          puVar9 = puVar9 + -2;
          *(undefined8 *)puVar9 = in_xd2;
        }
        uVar5 = DAT_8c620418;
        iVar7 = (uint)uVar1 * 0x20 + iVar7;
        if (in_FPSCR_SZ == '\0') {
          puVar9[-1] = *(uint *)(iVar2 + 0x14);
          puVar9[-2] = *(uint *)(iVar2 + 0x10);
          puVar9[-3] = *(uint *)(iVar7 + 0x18);
          puVar9[-4] = *(uint *)(iVar7 + 0x14);
          puVar9 = puVar9 + -5;
          *puVar9 = *(uint *)(iVar7 + 0x10);
        }
        else {
          *(undefined8 *)(puVar9 + -2) = *(undefined8 *)(iVar2 + 0x14);
          *(undefined8 *)(puVar9 + -4) = *(undefined8 *)(iVar2 + 0x10);
          *(undefined8 *)(puVar9 + -6) = *(undefined8 *)(iVar7 + 0x18);
          *(undefined8 *)(puVar9 + -8) = *(undefined8 *)(iVar7 + 0x14);
          in_xd2 = *(undefined8 *)(iVar7 + 0x10);
          puVar9 = puVar9 + -10;
          *(undefined8 *)puVar9 = in_xd2;
        }
        puVar9[-1] = uVar5;
        unaff_r8 = puVar9 + 7;
      }
    }
  }
  else if (uVar5 == DAT_8c620148) {
    *DAT_8c620420 = param_2;
    (*(code *)PTR_FUN_8c620424)(*DAT_8c620420);
  }
  return;
}

