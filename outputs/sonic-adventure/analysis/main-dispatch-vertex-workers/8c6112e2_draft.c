/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Restarted to delay deadcode elimination for space: register */

void FUN_8c6112e2(double param_1,int *param_2)

{
  uint *puVar1;
  double *pdVar2;
  uint uVar3;
  uint uVar4;
  double *pdVar5;
  uint uVar6;
  uint uVar7;
  undefined4 unaff_r8;
  int iVar8;
  undefined4 unaff_r10;
  undefined4 unaff_r11;
  undefined1 *puVar9;
  float fVar10;
  float fVar11;
  double dVar12;
  float fVar13;
  double dVar14;
  float fVar15;
  double dVar16;
  double dVar17;
  double dVar18;
  float fVar19;
  double dVar20;
  double dVar21;
  double dVar22;
  double in_xd0;
  double in_xd2;
  float in_xf4;
  float in_xf5;
  undefined8 in_xd6;
  float fVar23;
  undefined8 in_xd8;
  float fVar24;
  undefined8 in_xd10;
  float fVar25;
  undefined8 in_xd12;
  float fVar26;
  double in_xd14;
  float fVar27;
  bool bVar28;
  bool bVar29;
  bool bVar30;
  bool bVar31;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 local_20 [16];
  undefined1 local_10 [16];
  
  puVar1 = DAT_8c61135c;
  if (in_FPSCR_SZ == '\0') {
    puVar9 = local_10;
  }
  else {
    puVar9 = local_20;
  }
  *(undefined4 *)(puVar9 + -4) = unaff_r11;
  *(undefined4 *)(puVar9 + -8) = unaff_r10;
  *(undefined4 *)(puVar9 + -0xc) = unaff_r8;
  *(int **)(puVar9 + -0x10) = param_2;
  iVar8 = *param_2;
  uVar4 = param_2[1];
  pdVar2 = (double *)param_2[2];
  uVar6 = param_2[4];
  uVar7 = param_2[5];
  if (in_FPSCR_SZ == '\0') {
    dVar17 = (double)CONCAT44(param_2[6],param_2[7]);
    dVar20 = (double)CONCAT44(param_2[8],param_2[9]);
    dVar21 = (double)CONCAT44(param_2[10],param_2[0xb]);
    dVar22 = (double)CONCAT44(*(undefined4 *)DAT_8c61134c,*(undefined4 *)((int)DAT_8c61134c + 4));
    dVar18 = (double)CONCAT44(*(undefined4 *)DAT_8c611350,*(undefined4 *)DAT_8c611354);
  }
  else {
    dVar17 = *(double *)(param_2 + 6);
    in_xd6 = *(undefined8 *)(param_2 + 8);
    dVar20 = *(double *)(param_2 + 10);
    in_xd10 = *(undefined8 *)(param_2 + 0xc);
    dVar21 = *(double *)(param_2 + 0xe);
    in_xd12 = *(undefined8 *)(param_2 + 0x10);
    dVar22 = *DAT_8c61134c;
    in_xd14 = DAT_8c61134c[1];
    dVar18 = *DAT_8c611350;
    in_xd8 = *DAT_8c611354;
  }
  uVar3 = (DAT_8c611358 & uVar4) >> 0x18;
  *DAT_8c61135c = uVar3;
  puVar1[1] = uVar3;
  pdVar5 = (double *)(uVar4 & DAT_8c611360 | DAT_8c611364);
  if (iVar8 != 0) {
    if (in_FPSCR_SZ == '\0') {
      dVar12 = (double)CONCAT44(*(undefined4 *)pdVar2,*(undefined4 *)((int)pdVar2 + 4));
      dVar14 = (double)CONCAT44(*(undefined4 *)(pdVar2 + 1),0x3f800000);
      pdVar2 = (double *)((int)pdVar2 + 0xc);
    }
    else {
      dVar12 = *pdVar2;
      in_xd0 = pdVar2[1];
      dVar14 = pdVar2[2];
      pdVar2 = pdVar2 + 3;
    }
                    /* WARNING: Ignoring partial resolution of indirect */
    fVar23 = (float)((ulonglong)in_xd6 >> 0x20);
    fVar24 = (float)((ulonglong)in_xd8 >> 0x20);
    fVar25 = (float)((ulonglong)in_xd10 >> 0x20);
    fVar26 = (float)((ulonglong)in_xd12 >> 0x20);
    fVar27 = (float)((ulonglong)in_xd14 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      fVar19 = SUB84(dVar12,0);
      fVar11 = (float)((ulonglong)dVar12 >> 0x20);
      fVar13 = SUB84(dVar14,0);
      fVar15 = (float)((ulonglong)dVar14 >> 0x20);
      fVar10 = SUB84(in_xd0,0) * fVar19 + in_xf4 * fVar11 + (float)in_xd8 * fVar13 +
               (float)in_xd12 * fVar15;
      fVar10 = (float)((ulonglong)in_xd0 >> 0x20) * fVar19 + in_xf5 * fVar11 + fVar24 * fVar13 +
               fVar26 * fVar15;
      fVar10 = SUB84(in_xd2,0) * fVar19 + (float)in_xd6 * fVar11 + (float)in_xd10 * fVar13 +
               SUB84(in_xd14,0) * fVar15;
      fVar10 = (float)((ulonglong)in_xd2 >> 0x20) * fVar19 + fVar23 * fVar11 + fVar25 * fVar13 +
               fVar27 * fVar15;
    }
    do {
      uVar3 = 0;
      fVar10 = (float)((ulonglong)dVar14 >> 0x20);
      dVar16 = dVar14;
      if (in_FPSCR_SZ == '\0') {
        dVar16 = (double)CONCAT44(fVar10,SUB84(param_1,0));
      }
      fVar19 = (float)((ulonglong)dVar18 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        param_1 = (double)CONCAT44((float)((ulonglong)dVar16 >> 0x20) * fVar10,SUB84(dVar16,0));
        bVar31 = fVar10 < fVar19;
      }
      else {
        param_1 = dVar16 * dVar14;
        bVar31 = dVar14 < dVar18;
      }
      dVar16 = dVar14;
      if (in_FPSCR_SZ == '\0') {
        dVar14 = (double)CONCAT44(fVar10,fVar10);
        dVar16 = in_xd2;
      }
      in_xd2 = dVar16;
      if (!bVar31) {
        if (in_FPSCR_PR == '\0') {
          param_1 = (double)CONCAT44(1.0 / SQRT((float)((ulonglong)param_1 >> 0x20)),
                                     SUB84(param_1,0));
          dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) *
                                    (float)((ulonglong)dVar17 >> 0x20),
                                    SUB84(dVar12,0) * SUB84(dVar17,0));
        }
        else {
          dVar12 = dVar12 * dVar17 * dVar17;
        }
        fVar10 = (float)((ulonglong)param_1 >> 0x20);
        in_xd2 = param_1;
        if (in_FPSCR_SZ == '\0') {
          dVar14 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),fVar10);
          in_xd2 = dVar16;
        }
        if (in_FPSCR_PR == '\0') {
          fVar11 = SUB84(dVar12,0) * fVar10 + SUB84(dVar22,0);
          dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) * fVar10 +
                                    (float)((ulonglong)dVar22 >> 0x20),fVar11);
          bVar31 = SUB84(dVar21,0) < fVar11;
        }
        else {
          dVar12 = dVar12 * param_1 * param_1 + dVar22 + dVar22;
          fVar11 = SUB84(dVar12,0);
          bVar31 = dVar21 < dVar12;
        }
        if (in_FPSCR_PR == '\0') {
          bVar28 = fVar11 < (float)((ulonglong)dVar21 >> 0x20);
        }
        else {
          bVar28 = dVar12 < dVar21;
        }
        fVar10 = (float)((ulonglong)dVar12 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          bVar29 = SUB84(dVar20,0) < fVar10;
        }
        else {
          bVar29 = dVar20 < dVar12;
        }
        if (in_FPSCR_PR == '\0') {
          bVar30 = fVar10 < (float)((ulonglong)dVar20 >> 0x20);
        }
        else {
          bVar30 = dVar12 < dVar20;
        }
        uVar3 = (((uint)bVar31 << 1 | (uint)bVar28) << 1 | (uint)bVar29) << 1 | (uint)bVar30;
      }
      CacheBlockInvalidate(uVar4);
      fVar10 = (float)((ulonglong)dVar14 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        bVar31 = SUB84(dVar18,0) < fVar10;
      }
      else {
        bVar31 = dVar18 < dVar14;
      }
      if (in_FPSCR_PR == '\0') {
        bVar28 = fVar10 < fVar19;
      }
      else {
        bVar28 = dVar14 < dVar18;
      }
      uVar3 = (uVar3 << 1 | (uint)bVar31) << 1 | (uint)bVar28;
      if (in_FPSCR_SZ == '\0') {
        *(int *)(pdVar5 + 3) = SUB84(dVar14,0);
        *(int *)((int)pdVar5 + 0x14) = SUB84(dVar12,0);
        pdVar5 = pdVar5 + 2;
        *(int *)pdVar5 = (int)((ulonglong)dVar12 >> 0x20);
      }
      else {
        *(double *)((int)pdVar5 + 0x14) = in_xd2;
        *(double *)((int)pdVar5 + 0xc) = in_xd0;
        pdVar5 = (double *)((int)pdVar5 + 4);
        *pdVar5 = dVar12;
      }
      *(uint *)((int)pdVar5 + -4) = uVar3;
      if (in_FPSCR_SZ == '\0') {
        dVar12 = (double)CONCAT44(*(undefined4 *)pdVar2,*(undefined4 *)((int)pdVar2 + 4));
        dVar14 = (double)CONCAT44(*(undefined4 *)(pdVar2 + 1),0x3f800000);
        pdVar2 = (double *)((int)pdVar2 + 0xc);
      }
      else {
        dVar12 = *pdVar2;
        in_xd0 = pdVar2[1];
        dVar14 = pdVar2[2];
        pdVar2 = pdVar2 + 3;
      }
                    /* WARNING: Ignoring partial resolution of indirect */
      if (in_FPSCR_PR == '\0') {
        fVar19 = SUB84(dVar12,0);
        fVar11 = (float)((ulonglong)dVar12 >> 0x20);
        fVar13 = SUB84(dVar14,0);
        fVar15 = (float)((ulonglong)dVar14 >> 0x20);
        fVar10 = SUB84(in_xd0,0) * fVar19 + in_xf4 * fVar11 + (float)in_xd8 * fVar13 +
                 (float)in_xd12 * fVar15;
        fVar10 = (float)((ulonglong)in_xd0 >> 0x20) * fVar19 + in_xf5 * fVar11 + fVar24 * fVar13 +
                 fVar26 * fVar15;
        fVar10 = SUB84(in_xd2,0) * fVar19 + (float)in_xd6 * fVar11 + (float)in_xd10 * fVar13 +
                 SUB84(in_xd14,0) * fVar15;
        fVar10 = (float)((ulonglong)in_xd2 >> 0x20) * fVar19 + fVar23 * fVar11 + fVar25 * fVar13 +
                 fVar27 * fVar15;
      }
      uVar6 = uVar6 & uVar3;
      uVar7 = uVar7 | uVar3;
      iVar8 = iVar8 + -1;
      pdVar5 = pdVar5 + 2;
      uVar4 = uVar4 + 0x20;
    } while (iVar8 != 0);
  }
  iVar8 = *(int *)(puVar9 + -0x10);
  *(uint *)(iVar8 + 0x10) = uVar6;
  *(uint *)(iVar8 + 0x14) = uVar7;
  return;
}

