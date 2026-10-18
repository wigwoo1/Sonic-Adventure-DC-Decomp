/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Restarted to delay deadcode elimination for space: register */

void FUN_8c037294(double param_1,int *param_2)

{
  uint uVar1;
  double *pdVar2;
  uint uVar3;
  double *pdVar4;
  int iVar5;
  float fVar6;
  float fVar7;
  double dVar8;
  float fVar9;
  float fVar11;
  float fVar13;
  double dVar12;
  float fVar14;
  double dVar15;
  double dVar16;
  float fVar17;
  double dVar18;
  double dVar19;
  double in_xd0;
  float in_xf2;
  float in_xf3;
  float fVar20;
  double in_xd4;
  float fVar21;
  double in_xd6;
  float fVar22;
  double in_xd8;
  float in_xf10;
  float in_xf11;
  float in_xf12;
  float in_xf13;
  float in_xf14;
  float in_xf15;
  int in_GBR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  float fVar10;
  
  pdVar2 = (double *)*param_2;
  iVar5 = param_2[2];
  if (in_FPSCR_SZ == '\0') {
    dVar15 = (double)CONCAT44(*(undefined4 *)DAT_8c037338,*(undefined4 *)((int)DAT_8c037338 + 4));
    dVar16 = (double)CONCAT44(*(undefined4 *)(DAT_8c037338 + 1),
                              *(undefined4 *)((int)DAT_8c037338 + 0xc));
  }
  else {
    dVar15 = *DAT_8c037338;
    in_xd4 = DAT_8c037338[1];
    dVar16 = DAT_8c037338[2];
    in_xd6 = DAT_8c037338[3];
  }
  if (in_FPSCR_PR == '\0') {
    fVar13 = (float)((ulonglong)dVar16 >> 0x20);
    dVar16 = (double)CONCAT44(fVar13,SUB84(dVar16,0) * fVar13);
  }
  else {
    dVar16 = dVar16 * dVar16;
  }
  uVar3 = *DAT_8c03733c;
  *(uint *)(in_GBR + 0x3c) = uVar3;
  uVar1 = uVar3 >> 0x18 & 0x1c;
  *DAT_8c037340 = uVar1;
  *DAT_8c037344 = uVar1;
  if (in_FPSCR_SZ == '\0') {
    dVar8 = (double)CONCAT44(*(undefined4 *)pdVar2,*(undefined4 *)((int)pdVar2 + 4));
    fVar13 = *(float *)(pdVar2 + 1);
    pdVar2 = (double *)((int)pdVar2 + 0xc);
  }
  else {
    dVar8 = *pdVar2;
    in_xd0 = pdVar2[1];
    fVar13 = (float)((ulonglong)pdVar2[2] >> 0x20);
    pdVar2 = pdVar2 + 3;
  }
  pdVar4 = (double *)((uVar3 & DAT_8c037348 | DAT_8c03734c) - 0x10);
                    /* WARNING: Ignoring partial resolution of indirect */
  fVar20 = SUB84(in_xd4,0);
  fVar21 = (float)((ulonglong)in_xd4 >> 0x20);
  fVar22 = (float)((ulonglong)in_xd6 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    fVar17 = SUB84(dVar8,0);
    fVar7 = (float)((ulonglong)dVar8 >> 0x20);
    fVar6 = in_xf12 * fVar13;
    fVar9 = in_xf13 * fVar13;
    fVar13 = in_xf3 * fVar17 + fVar22 * fVar7 + in_xf11 * 1.0 + in_xf15 * fVar13;
    fVar6 = SUB84(in_xd0,0) * fVar17 + fVar20 * fVar7 + SUB84(in_xd8,0) * 1.0 + fVar6;
    fVar6 = (float)((ulonglong)in_xd0 >> 0x20) * fVar17 + fVar21 * fVar7 +
            (float)((ulonglong)in_xd8 >> 0x20) * 1.0 + fVar9;
  }
  if (in_FPSCR_PR == '\0') {
    dVar12 = (double)CONCAT44(fVar13,1.0 / fVar13);
  }
  else {
    dVar12 = (double)CONCAT44(fVar13,0x3f800000) / (double)CONCAT44(fVar13,0x3f800000);
  }
  do {
    if (in_FPSCR_SZ == '\0') {
      dVar18 = (double)CONCAT44(*(undefined4 *)pdVar2,SUB84(param_1,0));
      pdVar2 = (double *)((int)pdVar2 + 4);
    }
    else {
      dVar18 = *pdVar2;
      pdVar2 = pdVar2 + 1;
    }
    fVar13 = (float)((ulonglong)dVar16 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      dVar8 = (double)CONCAT44((float)((ulonglong)dVar8 >> 0x20) * fVar13,SUB84(dVar8,0));
    }
    else {
      dVar8 = dVar8 * dVar16;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar18 = (double)CONCAT44((int)((ulonglong)dVar18 >> 0x20),*(undefined4 *)pdVar2);
      pdVar2 = (double *)((int)pdVar2 + 4);
    }
    else {
      in_xd8 = *pdVar2;
      pdVar2 = pdVar2 + 1;
    }
    if (in_FPSCR_PR == '\0') {
      dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),SUB84(dVar8,0) * SUB84(dVar16,0));
    }
    else {
      dVar8 = dVar8 * dVar16;
    }
    if (in_FPSCR_SZ == '\0') {
      fVar6 = *(float *)pdVar2;
      pdVar2 = (double *)((int)pdVar2 + 4);
    }
    else {
      fVar6 = (float)((ulonglong)*pdVar2 >> 0x20);
      pdVar2 = pdVar2 + 1;
    }
                    /* WARNING: Ignoring partial resolution of indirect */
    fVar9 = (float)((ulonglong)in_xd8 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      fVar10 = SUB84(dVar18,0);
      fVar11 = (float)((ulonglong)dVar18 >> 0x20);
      fVar17 = in_xf12 * fVar6;
      fVar7 = in_xf13 * fVar6;
      fVar6 = in_xf3 * fVar10 + fVar22 * fVar11 + in_xf11 * 1.0 + in_xf15 * fVar6;
      fVar17 = SUB84(in_xd0,0) * fVar10 + fVar20 * fVar11 + SUB84(in_xd8,0) * 1.0 + fVar17;
      fVar17 = (float)((ulonglong)in_xd0 >> 0x20) * fVar10 + fVar21 * fVar11 + fVar9 * 1.0 + fVar7;
    }
    fVar17 = SUB84(dVar12,0);
    if (in_FPSCR_PR == '\0') {
      dVar19 = (double)CONCAT44(fVar6,1.0 / fVar6);
      dVar8 = (double)CONCAT44((float)((ulonglong)dVar8 >> 0x20) * fVar17,SUB84(dVar8,0));
    }
    else {
      dVar19 = (double)CONCAT44(fVar6,0x3f800000) / (double)CONCAT44(fVar6,0x3f800000);
      dVar8 = dVar8 * dVar12;
    }
    if (in_FPSCR_SZ == '\0') {
      pdVar4 = pdVar4 + 3;
      *(float *)pdVar4 = fVar17;
    }
    else {
      pdVar4 = (double *)((int)pdVar4 + 0x14);
      *pdVar4 = (double)CONCAT44(in_xf3,in_xf2);
    }
    fVar6 = (float)((ulonglong)dVar15 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      dVar8 = (double)CONCAT44((float)((ulonglong)dVar8 >> 0x20) + fVar6,
                               SUB84(dVar8,0) * fVar17 + SUB84(dVar15,0));
    }
    else {
      dVar8 = dVar8 * dVar12 + dVar15 + dVar15;
    }
    if (in_FPSCR_SZ == '\0') {
      pdVar4 = (double *)((int)pdVar4 + -4);
      *(int *)pdVar4 = SUB84(dVar8,0);
    }
    else {
      pdVar4 = pdVar4 + -1;
      *pdVar4 = in_xd0;
    }
    if (in_FPSCR_PR == '\0') {
      dVar18 = (double)CONCAT44((float)((ulonglong)dVar18 >> 0x20) * fVar13,SUB84(dVar18,0));
    }
    else {
      dVar18 = dVar18 * dVar16;
    }
    if (in_FPSCR_SZ == '\0') {
      pdVar4 = (double *)((int)pdVar4 + -4);
      *(int *)pdVar4 = (int)((ulonglong)dVar8 >> 0x20);
    }
    else {
      pdVar4 = pdVar4 + -1;
      *pdVar4 = dVar8;
    }
    if (in_FPSCR_PR == '\0') {
      dVar18 = (double)CONCAT44((int)((ulonglong)dVar18 >> 0x20),SUB84(dVar18,0) * SUB84(dVar16,0));
    }
    else {
      dVar18 = dVar18 * dVar16;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar8 = (double)CONCAT44(*(undefined4 *)pdVar2,*(undefined4 *)((int)pdVar2 + 4));
      dVar12 = (double)CONCAT44(*(undefined4 *)(pdVar2 + 1),0x3f800000);
      pdVar2 = (double *)((int)pdVar2 + 0xc);
    }
    else {
      dVar8 = *pdVar2;
      in_xd0 = pdVar2[1];
      dVar12 = pdVar2[2];
      pdVar2 = pdVar2 + 3;
    }
    fVar13 = SUB84(dVar19,0);
    if (in_FPSCR_PR == '\0') {
      fVar7 = SUB84(dVar8,0);
                    /* WARNING: Ignoring partial resolution of indirect */
      fVar10 = (float)((ulonglong)dVar8 >> 0x20);
      fVar11 = SUB84(dVar12,0);
      fVar14 = (float)((ulonglong)dVar12 >> 0x20);
      fVar17 = SUB84(in_xd0,0) * fVar7 + fVar20 * fVar10 + SUB84(in_xd8,0) * fVar11 +
               in_xf12 * fVar14;
      fVar9 = (float)((ulonglong)in_xd0 >> 0x20) * fVar7 + fVar21 * fVar10 + fVar9 * fVar11 +
              in_xf13 * fVar14;
      fVar9 = in_xf2 * fVar7 + SUB84(in_xd6,0) * fVar10 + in_xf10 * fVar11 + in_xf14 * fVar14;
      fVar9 = in_xf3 * fVar7 + fVar22 * fVar10 + in_xf11 * fVar11 + in_xf15 * fVar14;
      fVar9 = (float)((ulonglong)dVar12 >> 0x20);
      dVar12 = (double)CONCAT44(fVar9,SUB84(dVar12,0) / fVar9);
      param_1 = (double)CONCAT44((float)((ulonglong)dVar18 >> 0x20) * fVar13,
                                 SUB84(dVar18,0) * fVar13 + SUB84(dVar15,0));
    }
    else {
      dVar12 = dVar12 / dVar12;
      param_1 = dVar18 * dVar19 * dVar19 + dVar15;
    }
    if (in_FPSCR_SZ == '\0') {
      pdVar4 = pdVar4 + 3;
      *(float *)pdVar4 = fVar13;
    }
    else {
      pdVar4 = (double *)((int)pdVar4 + 0x14);
      *pdVar4 = (double)CONCAT44(in_xf11,in_xf10);
    }
    if (in_FPSCR_PR == '\0') {
      param_1 = (double)CONCAT44((float)((ulonglong)param_1 >> 0x20) + fVar6,SUB84(param_1,0));
    }
    else {
      param_1 = param_1 + dVar15;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)pdVar4 + -4) = SUB84(param_1,0);
      pdVar4 = pdVar4 + -1;
      *(int *)pdVar4 = (int)((ulonglong)param_1 >> 0x20);
    }
    else {
      pdVar4[-1] = in_xd8;
      pdVar4 = pdVar4 + -2;
      *pdVar4 = param_1;
    }
    iVar5 = iVar5 + -2;
    CacheBlockInvalidate(uVar3);
    uVar3 = uVar3 + 0x20;
  } while (0 < iVar5);
  return;
}

