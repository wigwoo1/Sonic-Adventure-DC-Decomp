/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Restarted to delay deadcode elimination for space: register */

void FUN_8c611122(int *param_1)

{
  uint *puVar1;
  uint uVar2;
  uint uVar3;
  double *pdVar4;
  double *pdVar5;
  undefined4 unaff_r8;
  int iVar6;
  undefined4 unaff_r10;
  undefined4 unaff_r11;
  undefined1 *puVar7;
  float fVar8;
  double dVar10;
  float fVar11;
  float fVar12;
  double dVar13;
  float fVar14;
  double dVar15;
  double dVar16;
  double in_xd0;
  float in_xf2;
  float in_xf3;
  float in_xf4;
  float in_xf5;
  undefined8 in_xd6;
  float fVar17;
  float in_xf8;
  float in_xf9;
  float in_xf10;
  float in_xf11;
  undefined8 in_xd12;
  float fVar18;
  double in_xd14;
  float fVar19;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_20 [16];
  undefined1 local_10 [16];
  float fVar9;
  
  puVar1 = DAT_8c611180;
  if (in_FPSCR_SZ == '\0') {
    puVar7 = local_10;
  }
  else {
    puVar7 = auStack_20;
  }
  *(undefined4 *)(puVar7 + -4) = unaff_r11;
  *(undefined4 *)(puVar7 + -8) = unaff_r10;
  *(undefined4 *)(puVar7 + -0xc) = unaff_r8;
  iVar6 = *param_1;
  uVar3 = param_1[1];
  pdVar5 = (double *)param_1[2];
  if (in_FPSCR_SZ == '\0') {
    dVar15 = (double)CONCAT44(param_1[6],param_1[7]);
    dVar16 = (double)CONCAT44(*(undefined4 *)DAT_8c611178,*(undefined4 *)((int)DAT_8c611178 + 4));
  }
  else {
    dVar15 = *(double *)(param_1 + 6);
    in_xd6 = *(undefined8 *)(param_1 + 8);
    dVar16 = *DAT_8c611178;
    in_xd14 = DAT_8c611178[1];
  }
  uVar2 = (DAT_8c61117c & uVar3) >> 0x18;
  *DAT_8c611180 = uVar2;
  puVar1[1] = uVar2;
  pdVar4 = (double *)(uVar3 & DAT_8c611184 | DAT_8c611188);
  if (iVar6 != 0) {
    if (in_FPSCR_SZ == '\0') {
      dVar10 = (double)CONCAT44(*(undefined4 *)pdVar5,*(undefined4 *)((int)pdVar5 + 4));
      dVar13 = (double)CONCAT44(*(undefined4 *)(pdVar5 + 1),0x3f800000);
      pdVar5 = (double *)((int)pdVar5 + 0xc);
    }
    else {
      dVar10 = *pdVar5;
      in_xd0 = pdVar5[1];
      dVar13 = pdVar5[2];
      pdVar5 = pdVar5 + 3;
    }
                    /* WARNING: Ignoring partial resolution of indirect */
    fVar17 = (float)((ulonglong)in_xd6 >> 0x20);
    fVar18 = (float)((ulonglong)in_xd12 >> 0x20);
    fVar19 = (float)((ulonglong)in_xd14 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      fVar9 = SUB84(dVar10,0);
      fVar11 = (float)((ulonglong)dVar10 >> 0x20);
      fVar12 = SUB84(dVar13,0);
      fVar14 = (float)((ulonglong)dVar13 >> 0x20);
      fVar8 = SUB84(in_xd0,0) * fVar9 + in_xf4 * fVar11 + in_xf8 * fVar12 + (float)in_xd12 * fVar14;
      fVar8 = (float)((ulonglong)in_xd0 >> 0x20) * fVar9 + in_xf5 * fVar11 + in_xf9 * fVar12 +
              fVar18 * fVar14;
      fVar8 = in_xf2 * fVar9 + (float)in_xd6 * fVar11 + in_xf10 * fVar12 + SUB84(in_xd14,0) * fVar14
      ;
      fVar8 = in_xf3 * fVar9 + fVar17 * fVar11 + in_xf11 * fVar12 + fVar19 * fVar14;
    }
    do {
      if (in_FPSCR_PR == '\0') {
        fVar8 = (float)((ulonglong)dVar13 >> 0x20);
        fVar8 = 1.0 / SQRT(fVar8 * fVar8);
        dVar13 = (double)CONCAT44(fVar8,SUB84(dVar13,0));
        dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) *
                                  (float)((ulonglong)dVar15 >> 0x20) * fVar8,
                                  SUB84(dVar10,0) * SUB84(dVar15,0) * fVar8);
      }
      else {
        dVar13 = dVar13 * dVar13;
        fVar8 = (float)((ulonglong)dVar13 >> 0x20);
        dVar10 = dVar10 * dVar15 * dVar15 * dVar13 * dVar13;
      }
      CacheBlockInvalidate(uVar3);
      if (in_FPSCR_PR == '\0') {
        dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) +
                                  (float)((ulonglong)dVar16 >> 0x20),
                                  SUB84(dVar10,0) + SUB84(dVar16,0));
      }
      else {
        dVar10 = dVar10 + dVar16 + dVar16;
      }
      if (in_FPSCR_SZ == '\0') {
        *(float *)(pdVar4 + 3) = fVar8;
        *(int *)((int)pdVar4 + 0x14) = SUB84(dVar10,0);
        pdVar4 = pdVar4 + 2;
        *(int *)pdVar4 = (int)((ulonglong)dVar10 >> 0x20);
      }
      else {
        *(double *)((int)pdVar4 + 0x14) = dVar13;
        *(double *)((int)pdVar4 + 0xc) = in_xd0;
        pdVar4 = (double *)((int)pdVar4 + 4);
        *pdVar4 = dVar10;
      }
      *(undefined4 *)((int)pdVar4 + -4) = 0;
      if (in_FPSCR_SZ == '\0') {
        dVar10 = (double)CONCAT44(*(undefined4 *)pdVar5,*(undefined4 *)((int)pdVar5 + 4));
        dVar13 = (double)CONCAT44(*(undefined4 *)(pdVar5 + 1),0x3f800000);
        pdVar5 = (double *)((int)pdVar5 + 0xc);
      }
      else {
        dVar10 = *pdVar5;
        in_xd0 = pdVar5[1];
        dVar13 = pdVar5[2];
        pdVar5 = pdVar5 + 3;
      }
                    /* WARNING: Ignoring partial resolution of indirect */
      if (in_FPSCR_PR == '\0') {
        fVar9 = SUB84(dVar10,0);
        fVar11 = (float)((ulonglong)dVar10 >> 0x20);
        fVar12 = SUB84(dVar13,0);
        fVar14 = (float)((ulonglong)dVar13 >> 0x20);
        fVar8 = SUB84(in_xd0,0) * fVar9 + in_xf4 * fVar11 + in_xf8 * fVar12 +
                (float)in_xd12 * fVar14;
        fVar8 = (float)((ulonglong)in_xd0 >> 0x20) * fVar9 + in_xf5 * fVar11 + in_xf9 * fVar12 +
                fVar18 * fVar14;
        fVar8 = in_xf2 * fVar9 + (float)in_xd6 * fVar11 + in_xf10 * fVar12 +
                SUB84(in_xd14,0) * fVar14;
        fVar8 = in_xf3 * fVar9 + fVar17 * fVar11 + in_xf11 * fVar12 + fVar19 * fVar14;
      }
      iVar6 = iVar6 + -1;
      pdVar4 = pdVar4 + 2;
      uVar3 = uVar3 + 0x20;
    } while (iVar6 != 0);
  }
  return;
}

