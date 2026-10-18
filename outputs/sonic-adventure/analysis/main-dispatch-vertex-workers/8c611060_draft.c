/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Restarted to delay deadcode elimination for space: register */

void FUN_8c611060(int *param_1)

{
  uint *puVar1;
  uint uVar2;
  uint uVar3;
  double *pdVar4;
  double *pdVar5;
  double *pdVar6;
  undefined4 unaff_r8;
  int iVar7;
  undefined4 unaff_r10;
  undefined4 unaff_r11;
  undefined1 *puVar8;
  float fVar9;
  double dVar11;
  float fVar12;
  float fVar13;
  double dVar14;
  float fVar15;
  double dVar16;
  double dVar17;
  double in_dr12;
  double dVar18;
  double in_xd0;
  float in_xf2;
  float in_xf3;
  float in_xf4;
  float in_xf5;
  undefined8 in_xd6;
  float fVar19;
  float in_xf8;
  float in_xf9;
  double in_xd10;
  undefined8 in_xd12;
  float fVar20;
  double in_xd14;
  float fVar21;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_20 [16];
  undefined1 local_10 [16];
  float fVar10;
  
  puVar1 = DAT_8c6110c0;
  if (in_FPSCR_SZ == '\0') {
    puVar8 = local_10;
  }
  else {
    puVar8 = auStack_20;
  }
  *(undefined4 *)(puVar8 + -4) = unaff_r11;
  *(undefined4 *)(puVar8 + -8) = unaff_r10;
  *(undefined4 *)(puVar8 + -0xc) = unaff_r8;
  iVar7 = *param_1;
  uVar3 = param_1[1];
  pdVar5 = (double *)param_1[2];
  pdVar6 = (double *)param_1[3];
  if (in_FPSCR_SZ == '\0') {
    dVar16 = (double)CONCAT44(param_1[6],param_1[7]);
    dVar18 = (double)CONCAT44(*(undefined4 *)DAT_8c6110b8,*(undefined4 *)((int)DAT_8c6110b8 + 4));
  }
  else {
    dVar16 = *(double *)(param_1 + 6);
    in_xd6 = *(undefined8 *)(param_1 + 8);
    dVar18 = *DAT_8c6110b8;
    in_xd14 = DAT_8c6110b8[1];
  }
  uVar2 = (DAT_8c6110bc & uVar3) >> 0x18;
  *DAT_8c6110c0 = uVar2;
  puVar1[1] = uVar2;
  pdVar4 = (double *)(uVar3 & DAT_8c6110c4 | DAT_8c6110c8);
  if (iVar7 != 0) {
    if (in_FPSCR_SZ == '\0') {
      dVar11 = (double)CONCAT44(*(undefined4 *)pdVar5,*(undefined4 *)((int)pdVar5 + 4));
      dVar14 = (double)CONCAT44(*(undefined4 *)(pdVar5 + 1),0x3f800000);
      pdVar5 = (double *)((int)pdVar5 + 0xc);
    }
    else {
      dVar11 = *pdVar5;
      in_xd0 = pdVar5[1];
      dVar14 = pdVar5[2];
      pdVar5 = pdVar5 + 3;
    }
                    /* WARNING: Ignoring partial resolution of indirect */
    fVar19 = (float)((ulonglong)in_xd6 >> 0x20);
    fVar20 = (float)((ulonglong)in_xd12 >> 0x20);
    fVar21 = (float)((ulonglong)in_xd14 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      fVar10 = SUB84(dVar11,0);
      fVar12 = (float)((ulonglong)dVar11 >> 0x20);
      fVar13 = SUB84(dVar14,0);
      fVar15 = (float)((ulonglong)dVar14 >> 0x20);
      fVar9 = SUB84(in_xd0,0) * fVar10 + in_xf4 * fVar12 + in_xf8 * fVar13 + (float)in_xd12 * fVar15
      ;
      fVar9 = (float)((ulonglong)in_xd0 >> 0x20) * fVar10 + in_xf5 * fVar12 + in_xf9 * fVar13 +
              fVar20 * fVar15;
      fVar9 = in_xf2 * fVar10 + (float)in_xd6 * fVar12 + SUB84(in_xd10,0) * fVar13 +
              SUB84(in_xd14,0) * fVar15;
      fVar9 = in_xf3 * fVar10 + fVar19 * fVar12 + (float)((ulonglong)in_xd10 >> 0x20) * fVar13 +
              fVar21 * fVar15;
    }
    do {
      if (in_FPSCR_PR == '\0') {
        fVar9 = (float)((ulonglong)dVar14 >> 0x20);
        dVar14 = (double)CONCAT44(1.0 / SQRT(fVar9 * fVar9),SUB84(dVar14,0));
        dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) *
                                  (float)((ulonglong)dVar16 >> 0x20),
                                  SUB84(dVar11,0) * SUB84(dVar16,0));
      }
      else {
        dVar14 = dVar14 * dVar14;
        dVar11 = dVar11 * dVar16 * dVar16;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar17 = (double)CONCAT44(*(undefined4 *)pdVar6,*(undefined4 *)((int)pdVar6 + 4));
        in_dr12 = (double)CONCAT44(*(undefined4 *)(pdVar6 + 1),SUB84(in_dr12,0));
        pdVar6 = (double *)((int)pdVar6 + 0xc);
      }
      else {
        dVar17 = *pdVar6;
        in_xd10 = pdVar6[1];
        in_dr12 = pdVar6[2];
        pdVar6 = pdVar6 + 3;
      }
      fVar9 = (float)((ulonglong)dVar14 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) * fVar9,SUB84(dVar11,0) * fVar9
                                 );
      }
      else {
        dVar11 = dVar11 * dVar14 * dVar14;
      }
      CacheBlockInvalidate(uVar3);
      if (in_FPSCR_PR == '\0') {
        dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) +
                                  (float)((ulonglong)dVar18 >> 0x20),
                                  SUB84(dVar11,0) + SUB84(dVar18,0));
      }
      else {
        dVar11 = dVar11 + dVar18 + dVar18;
      }
      if (in_FPSCR_SZ == '\0') {
        *(float *)(pdVar4 + 3) = fVar9;
        *(int *)((int)pdVar4 + 0x14) = SUB84(dVar11,0);
        pdVar4 = pdVar4 + 2;
        *(int *)pdVar4 = (int)((ulonglong)dVar11 >> 0x20);
      }
      else {
        *(double *)((int)pdVar4 + 0x14) = dVar14;
        *(double *)((int)pdVar4 + 0xc) = in_xd0;
        pdVar4 = (double *)((int)pdVar4 + 4);
        *pdVar4 = dVar11;
      }
      *(undefined4 *)((int)pdVar4 + -4) = 0;
      if (in_FPSCR_SZ == '\0') {
        *(int *)(pdVar4 + -1) = (int)((ulonglong)in_dr12 >> 0x20);
        *(int *)((int)pdVar4 + -0xc) = SUB84(dVar17,0);
        pdVar4 = pdVar4 + -2;
        *(int *)pdVar4 = (int)((ulonglong)dVar17 >> 0x20);
        dVar11 = (double)CONCAT44(*(undefined4 *)pdVar5,*(undefined4 *)((int)pdVar5 + 4));
        dVar14 = (double)CONCAT44(*(undefined4 *)(pdVar5 + 1),0x3f800000);
        pdVar5 = (double *)((int)pdVar5 + 0xc);
      }
      else {
        *(double *)((int)pdVar4 + -0xc) = in_dr12;
        *(double *)((int)pdVar4 + -0x14) = in_xd10;
        pdVar4 = (double *)((int)pdVar4 + -0x1c);
        *pdVar4 = dVar17;
        dVar11 = *pdVar5;
        in_xd0 = pdVar5[1];
        dVar14 = pdVar5[2];
        pdVar5 = pdVar5 + 3;
      }
                    /* WARNING: Ignoring partial resolution of indirect */
      if (in_FPSCR_PR == '\0') {
        fVar10 = SUB84(dVar11,0);
        fVar12 = (float)((ulonglong)dVar11 >> 0x20);
        fVar13 = SUB84(dVar14,0);
        fVar15 = (float)((ulonglong)dVar14 >> 0x20);
        fVar9 = SUB84(in_xd0,0) * fVar10 + in_xf4 * fVar12 + in_xf8 * fVar13 +
                (float)in_xd12 * fVar15;
        fVar9 = (float)((ulonglong)in_xd0 >> 0x20) * fVar10 + in_xf5 * fVar12 + in_xf9 * fVar13 +
                fVar20 * fVar15;
        fVar9 = in_xf2 * fVar10 + (float)in_xd6 * fVar12 + SUB84(in_xd10,0) * fVar13 +
                SUB84(in_xd14,0) * fVar15;
        fVar9 = in_xf3 * fVar10 + fVar19 * fVar12 + (float)((ulonglong)in_xd10 >> 0x20) * fVar13 +
                fVar21 * fVar15;
      }
      iVar7 = iVar7 + -1;
      pdVar4 = pdVar4 + 4;
      uVar3 = uVar3 + 0x20;
    } while (iVar7 != 0);
  }
  return;
}

