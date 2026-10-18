/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Restarted to delay deadcode elimination for space: register */

void FUN_8c6111d6(double param_1,int *param_2)

{
  uint *puVar1;
  double *pdVar2;
  uint uVar3;
  uint uVar4;
  double *pdVar5;
  double *pdVar6;
  double *pdVar7;
  uint uVar8;
  uint uVar9;
  undefined4 unaff_r8;
  int iVar10;
  undefined4 unaff_r10;
  undefined4 unaff_r11;
  undefined1 *puVar11;
  float fVar12;
  float fVar13;
  double dVar14;
  float fVar15;
  double dVar16;
  float fVar17;
  double dVar18;
  double dVar19;
  double dVar20;
  float fVar21;
  double dVar22;
  double dVar23;
  double dVar24;
  double in_xd0;
  double in_xd2;
  float in_xf4;
  float in_xf5;
  undefined8 in_xd6;
  float fVar25;
  undefined8 in_xd8;
  float fVar26;
  undefined8 in_xd10;
  float fVar27;
  undefined8 in_xd12;
  float fVar28;
  double in_xd14;
  float fVar29;
  bool bVar30;
  bool bVar31;
  bool bVar32;
  bool bVar33;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 local_20 [16];
  undefined1 local_10 [16];
  
  puVar1 = DAT_8c611250;
  if (in_FPSCR_SZ == '\0') {
    puVar11 = local_10;
  }
  else {
    puVar11 = local_20;
  }
  *(undefined4 *)(puVar11 + -4) = unaff_r11;
  *(undefined4 *)(puVar11 + -8) = unaff_r10;
  *(undefined4 *)(puVar11 + -0xc) = unaff_r8;
  *(int **)(puVar11 + -0x10) = param_2;
  iVar10 = *param_2;
  uVar4 = param_2[1];
  pdVar2 = (double *)param_2[2];
  pdVar6 = (double *)param_2[3];
  uVar8 = param_2[4];
  uVar9 = param_2[5];
  if (in_FPSCR_SZ == '\0') {
    dVar19 = (double)CONCAT44(param_2[6],param_2[7]);
    dVar22 = (double)CONCAT44(param_2[8],param_2[9]);
    dVar23 = (double)CONCAT44(param_2[10],param_2[0xb]);
    dVar24 = (double)CONCAT44(*(undefined4 *)DAT_8c611240,*(undefined4 *)((int)DAT_8c611240 + 4));
    dVar20 = (double)CONCAT44(*(undefined4 *)DAT_8c611244,*(undefined4 *)DAT_8c611248);
  }
  else {
    dVar19 = *(double *)(param_2 + 6);
    in_xd6 = *(undefined8 *)(param_2 + 8);
    dVar22 = *(double *)(param_2 + 10);
    in_xd10 = *(undefined8 *)(param_2 + 0xc);
    dVar23 = *(double *)(param_2 + 0xe);
    in_xd12 = *(undefined8 *)(param_2 + 0x10);
    dVar24 = *DAT_8c611240;
    in_xd14 = DAT_8c611240[1];
    dVar20 = *DAT_8c611244;
    in_xd8 = *DAT_8c611248;
  }
  uVar3 = (DAT_8c61124c & uVar4) >> 0x18;
  *DAT_8c611250 = uVar3;
  puVar1[1] = uVar3;
  pdVar5 = (double *)(uVar4 & DAT_8c611254 | DAT_8c611258);
  if (iVar10 != 0) {
    if (in_FPSCR_SZ == '\0') {
      dVar14 = (double)CONCAT44(*(undefined4 *)pdVar2,*(undefined4 *)((int)pdVar2 + 4));
      dVar16 = (double)CONCAT44(*(undefined4 *)(pdVar2 + 1),0x3f800000);
      pdVar2 = (double *)((int)pdVar2 + 0xc);
    }
    else {
      dVar14 = *pdVar2;
      in_xd0 = pdVar2[1];
      dVar16 = pdVar2[2];
      pdVar2 = pdVar2 + 3;
    }
                    /* WARNING: Ignoring partial resolution of indirect */
    fVar25 = (float)((ulonglong)in_xd6 >> 0x20);
    fVar26 = (float)((ulonglong)in_xd8 >> 0x20);
    fVar27 = (float)((ulonglong)in_xd10 >> 0x20);
    fVar28 = (float)((ulonglong)in_xd12 >> 0x20);
    fVar29 = (float)((ulonglong)in_xd14 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      fVar21 = SUB84(dVar14,0);
      fVar13 = (float)((ulonglong)dVar14 >> 0x20);
      fVar15 = SUB84(dVar16,0);
      fVar17 = (float)((ulonglong)dVar16 >> 0x20);
      fVar12 = SUB84(in_xd0,0) * fVar21 + in_xf4 * fVar13 + (float)in_xd8 * fVar15 +
               (float)in_xd12 * fVar17;
      fVar12 = (float)((ulonglong)in_xd0 >> 0x20) * fVar21 + in_xf5 * fVar13 + fVar26 * fVar15 +
               fVar28 * fVar17;
      fVar12 = SUB84(in_xd2,0) * fVar21 + (float)in_xd6 * fVar13 + (float)in_xd10 * fVar15 +
               SUB84(in_xd14,0) * fVar17;
      fVar12 = (float)((ulonglong)in_xd2 >> 0x20) * fVar21 + fVar25 * fVar13 + fVar27 * fVar15 +
               fVar29 * fVar17;
    }
    do {
      uVar3 = 0;
      fVar12 = (float)((ulonglong)dVar16 >> 0x20);
      dVar18 = dVar16;
      if (in_FPSCR_SZ == '\0') {
        dVar18 = (double)CONCAT44(fVar12,SUB84(param_1,0));
      }
      fVar21 = (float)((ulonglong)dVar20 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        param_1 = (double)CONCAT44((float)((ulonglong)dVar18 >> 0x20) * fVar12,SUB84(dVar18,0));
        bVar33 = fVar12 < fVar21;
      }
      else {
        param_1 = dVar18 * dVar16;
        bVar33 = dVar16 < dVar20;
      }
      dVar18 = dVar16;
      if (in_FPSCR_SZ == '\0') {
        dVar16 = (double)CONCAT44(fVar12,fVar12);
        dVar18 = in_xd2;
      }
      in_xd2 = dVar18;
      if (!bVar33) {
        if (in_FPSCR_PR == '\0') {
          param_1 = (double)CONCAT44(1.0 / SQRT((float)((ulonglong)param_1 >> 0x20)),
                                     SUB84(param_1,0));
          dVar14 = (double)CONCAT44((float)((ulonglong)dVar14 >> 0x20) *
                                    (float)((ulonglong)dVar19 >> 0x20),
                                    SUB84(dVar14,0) * SUB84(dVar19,0));
        }
        else {
          dVar14 = dVar14 * dVar19 * dVar19;
        }
        fVar12 = (float)((ulonglong)param_1 >> 0x20);
        in_xd2 = param_1;
        if (in_FPSCR_SZ == '\0') {
          dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),fVar12);
          in_xd2 = dVar18;
        }
        if (in_FPSCR_PR == '\0') {
          fVar13 = SUB84(dVar14,0) * fVar12 + SUB84(dVar24,0);
          dVar14 = (double)CONCAT44((float)((ulonglong)dVar14 >> 0x20) * fVar12 +
                                    (float)((ulonglong)dVar24 >> 0x20),fVar13);
          bVar33 = SUB84(dVar23,0) < fVar13;
        }
        else {
          dVar14 = dVar14 * param_1 * param_1 + dVar24 + dVar24;
          fVar13 = SUB84(dVar14,0);
          bVar33 = dVar23 < dVar14;
        }
        if (in_FPSCR_PR == '\0') {
          bVar30 = fVar13 < (float)((ulonglong)dVar23 >> 0x20);
        }
        else {
          bVar30 = dVar14 < dVar23;
        }
        fVar12 = (float)((ulonglong)dVar14 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          bVar31 = SUB84(dVar22,0) < fVar12;
        }
        else {
          bVar31 = dVar22 < dVar14;
        }
        if (in_FPSCR_PR == '\0') {
          bVar32 = fVar12 < (float)((ulonglong)dVar22 >> 0x20);
        }
        else {
          bVar32 = dVar14 < dVar22;
        }
        uVar3 = (((uint)bVar33 << 1 | (uint)bVar30) << 1 | (uint)bVar31) << 1 | (uint)bVar32;
      }
      CacheBlockInvalidate(uVar4);
      fVar12 = (float)((ulonglong)dVar16 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        bVar33 = SUB84(dVar20,0) < fVar12;
      }
      else {
        bVar33 = dVar20 < dVar16;
      }
      if (in_FPSCR_PR == '\0') {
        bVar30 = fVar12 < fVar21;
      }
      else {
        bVar30 = dVar16 < dVar20;
      }
      uVar3 = (uVar3 << 1 | (uint)bVar33) << 1 | (uint)bVar30;
      if (in_FPSCR_SZ == '\0') {
        *(int *)(pdVar5 + 3) = SUB84(dVar16,0);
        *(int *)((int)pdVar5 + 0x14) = SUB84(dVar14,0);
        pdVar5 = pdVar5 + 2;
        *(int *)pdVar5 = (int)((ulonglong)dVar14 >> 0x20);
        dVar14 = (double)CONCAT44(*(undefined4 *)pdVar6,SUB84(dVar16,0));
        pdVar7 = (double *)((int)pdVar6 + 4);
        pdVar6 = (double *)((int)pdVar6 + 0xc);
      }
      else {
        *(double *)((int)pdVar5 + 0x14) = in_xd2;
        *(double *)((int)pdVar5 + 0xc) = in_xd0;
        pdVar5 = (double *)((int)pdVar5 + 4);
        *pdVar5 = dVar14;
        dVar14 = *pdVar6;
        in_xd0 = pdVar6[1];
        pdVar7 = pdVar6 + 2;
        pdVar6 = pdVar6 + 3;
      }
      dVar16 = *pdVar7;
      *(uint *)((int)pdVar5 + -4) = uVar3;
      if (in_FPSCR_SZ == '\0') {
        *(int *)(pdVar5 + -1) = (int)((ulonglong)dVar16 >> 0x20);
        *(int *)((int)pdVar5 + -0xc) = SUB84(dVar16,0);
        pdVar5 = pdVar5 + -2;
        *(int *)pdVar5 = (int)((ulonglong)dVar14 >> 0x20);
        dVar14 = (double)CONCAT44(*(undefined4 *)pdVar2,*(undefined4 *)((int)pdVar2 + 4));
        dVar16 = (double)CONCAT44(*(undefined4 *)(pdVar2 + 1),0x3f800000);
        pdVar2 = (double *)((int)pdVar2 + 0xc);
      }
      else {
        *(double *)((int)pdVar5 + -0xc) = dVar16;
        *(double *)((int)pdVar5 + -0x14) = in_xd0;
        pdVar5 = (double *)((int)pdVar5 + -0x1c);
        *pdVar5 = dVar14;
        dVar14 = *pdVar2;
        in_xd0 = pdVar2[1];
        dVar16 = pdVar2[2];
        pdVar2 = pdVar2 + 3;
      }
                    /* WARNING: Ignoring partial resolution of indirect */
      if (in_FPSCR_PR == '\0') {
        fVar21 = SUB84(dVar14,0);
        fVar13 = (float)((ulonglong)dVar14 >> 0x20);
        fVar15 = SUB84(dVar16,0);
        fVar17 = (float)((ulonglong)dVar16 >> 0x20);
        fVar12 = SUB84(in_xd0,0) * fVar21 + in_xf4 * fVar13 + (float)in_xd8 * fVar15 +
                 (float)in_xd12 * fVar17;
        fVar12 = (float)((ulonglong)in_xd0 >> 0x20) * fVar21 + in_xf5 * fVar13 + fVar26 * fVar15 +
                 fVar28 * fVar17;
        fVar12 = SUB84(in_xd2,0) * fVar21 + (float)in_xd6 * fVar13 + (float)in_xd10 * fVar15 +
                 SUB84(in_xd14,0) * fVar17;
        fVar12 = (float)((ulonglong)in_xd2 >> 0x20) * fVar21 + fVar25 * fVar13 + fVar27 * fVar15 +
                 fVar29 * fVar17;
      }
      uVar8 = uVar8 & uVar3;
      uVar9 = uVar9 | uVar3;
      iVar10 = iVar10 + -1;
      pdVar5 = pdVar5 + 4;
      uVar4 = uVar4 + 0x20;
    } while (iVar10 != 0);
  }
  iVar10 = *(int *)(puVar11 + -0x10);
  *(uint *)(iVar10 + 0x10) = uVar8;
  *(uint *)(iVar10 + 0x14) = uVar9;
  return;
}

