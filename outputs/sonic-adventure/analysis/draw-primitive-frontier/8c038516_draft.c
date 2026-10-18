/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c03851e) */
/* WARNING: Removing unreachable block (ram,0x8c03863e) */
/* WARNING: Restarted to delay deadcode elimination for space: register */

ulonglong FUN_8c038516(undefined4 param_1,undefined8 param_2,undefined4 param_3)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  byte bVar5;
  double *pdVar6;
  int in_r1;
  double *pdVar7;
  double *unaff_r8;
  double *unaff_r9;
  double *unaff_r10;
  short *unaff_r13;
  float fVar8;
  float fVar9;
  float fVar10;
  double dVar11;
  undefined4 uVar12;
  float fVar13;
  float fVar14;
  float fVar15;
  double dVar16;
  float fVar17;
  double dVar18;
  float fVar19;
  double dVar20;
  double dVar22;
  float fVar23;
  float fVar24;
  float fVar25;
  double dVar26;
  undefined4 in_fr15;
  double dVar27;
  double in_xd0;
  double in_xd2;
  double in_xd4;
  float in_xf6;
  float in_xf7;
  float in_xf8;
  float in_xf9;
  float in_xf10;
  float in_xf11;
  double in_xd12;
  double in_xd14;
  int in_GBR;
  byte in_MD;
  byte in_RB;
  byte in_BL;
  byte in_FD;
  byte in_M;
  byte in_Q;
  byte in_IMASK;
  byte in_S;
  byte in_T;
  bool bVar28;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  ulonglong uVar29;
  float fVar21;
  
  pdVar7 = DAT_8c03865c;
  uVar1 = (uint)in_T | (uint)in_S << 1;
  uVar2 = uVar1 | (uint)in_IMASK << 4;
  uVar3 = uVar2 | (uint)in_Q << 8;
  uVar4 = uVar3 | (uint)in_M << 9;
  bVar5 = (byte)(((uint)in_BL << 0x1c) >> 0x18);
  uVar12 = (undefined4)((ulonglong)param_2 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    dVar27 = (double)CONCAT44(*(undefined4 *)DAT_8c038654,in_fr15);
    pdVar6 = (double *)((int)DAT_8c038654 + 4);
  }
  else {
    dVar27 = *DAT_8c038654;
    pdVar6 = DAT_8c038654 + 1;
  }
  fVar10 = (float)((ulonglong)dVar27 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar22 = (double)CONCAT44(uVar12,1.0 / fVar10);
  }
  else {
    dVar22 = (double)CONCAT44(uVar12,0x3f800000) / dVar27;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar27 = (double)CONCAT44(fVar10,*(undefined4 *)pdVar6);
  }
  else {
    in_xd14 = *pdVar6;
  }
  if (in_FPSCR_PR == '\0') {
    dVar27 = (double)CONCAT44(1.0 / SUB84(dVar27,0),param_3);
  }
  else {
    dVar27 = (double)CONCAT44(0x3f800000,param_3) / dVar27;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar26 = (double)CONCAT44(*(undefined4 *)DAT_8c038658,*(undefined4 *)((int)DAT_8c038658 + 4));
  }
  else {
    dVar26 = *DAT_8c038658;
    in_xd12 = DAT_8c038658[1];
  }
  pdVar6 = (double *)(*unaff_r13 + in_r1);
  if (in_FPSCR_SZ == '\0') {
    dVar11 = (double)CONCAT44(*(undefined4 *)pdVar6,*(undefined4 *)((int)pdVar6 + 4));
    dVar16 = (double)((ulonglong)*(uint *)(pdVar6 + 1) << 0x20);
  }
  else {
    dVar11 = *pdVar6;
    in_xd0 = pdVar6[1];
    dVar16 = pdVar6[2];
  }
  dVar16 = (double)((ulonglong)dVar16 & 0xffffffff00000000);
                    /* WARNING: Ignoring partial resolution of indirect */
  fVar10 = SUB84(in_xd12,0);
  fVar13 = (float)((ulonglong)in_xd12 >> 0x20);
  fVar19 = SUB84(in_xd14,0);
  fVar23 = (float)((ulonglong)in_xd14 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    fVar9 = SUB84(dVar11,0);
    fVar15 = (float)((ulonglong)dVar11 >> 0x20);
    fVar17 = (float)((ulonglong)dVar16 >> 0x20);
    fVar8 = SUB84(in_xd0,0) * fVar9 + SUB84(in_xd4,0) * fVar15 + in_xf8 * 0.0 + fVar10 * fVar17;
    fVar8 = (float)((ulonglong)in_xd0 >> 0x20) * fVar9 + (float)((ulonglong)in_xd4 >> 0x20) * fVar15
            + in_xf9 * 0.0 + fVar13 * fVar17;
    fVar8 = SUB84(in_xd2,0) * fVar9 + in_xf6 * fVar15 + in_xf10 * 0.0 + fVar19 * fVar17;
    fVar8 = (float)((ulonglong)in_xd2 >> 0x20) * fVar9 + in_xf7 * fVar15 + in_xf11 * 0.0 +
            fVar23 * fVar17;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),*(undefined4 *)PTR_DAT_8c038660);
  }
  else {
    in_xd2 = *(double *)PTR_DAT_8c038660;
  }
  fVar8 = SUB84(dVar16,0);
  if (in_FPSCR_PR == '\0') {
    dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) * fVar8 + fVar8,
                              SUB84(dVar11,0) * fVar8 + fVar8);
  }
  else {
    dVar11 = (dVar11 * dVar16 + dVar16) * dVar16 + dVar16;
  }
  if (in_FPSCR_SZ == '\0') {
    uVar12 = (undefined4)((ulonglong)dVar11 >> 0x20);
    dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),uVar12);
    *(int *)DAT_8c03865c = SUB84(dVar11,0);
    pdVar7 = (double *)((int)pdVar7 + -4);
    *(undefined4 *)pdVar7 = uVar12;
    dVar11 = in_xd2;
  }
  else {
    *DAT_8c03865c = in_xd0;
    pdVar7 = pdVar7 + -1;
    *pdVar7 = dVar11;
  }
  *(undefined4 *)((int)pdVar7 + -4) = *(undefined4 *)(in_GBR + 0x48);
  if (in_FPSCR_SZ == '\0') {
    dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),*(undefined4 *)unaff_r8);
    dVar18 = (double)CONCAT44(*(undefined4 *)((int)unaff_r8 + 4),*(undefined4 *)(unaff_r8 + 1));
    pdVar6 = (double *)((int)unaff_r8 + 0xc);
  }
  else {
    dVar11 = *unaff_r8;
    dVar18 = unaff_r8[1];
    in_xd4 = unaff_r8[2];
    pdVar6 = unaff_r8 + 3;
  }
  *(undefined4 *)(pdVar7 + -1) = *(undefined4 *)pdVar6;
  fVar9 = (float)((ulonglong)dVar18 >> 0x20);
  fVar8 = SUB84(dVar18,0);
  if (in_FPSCR_SZ == '\0') {
    *(float *)((int)pdVar7 + -0xc) = SUB84(dVar16,0);
    *(float *)(pdVar7 + -2) = fVar9;
    pdVar7 = (double *)((int)pdVar7 + -0x14);
    *(float *)pdVar7 = fVar8;
  }
  else {
    pdVar7[-2] = dVar11;
    pdVar7[-3] = dVar18;
    pdVar7 = pdVar7 + -4;
    *pdVar7 = in_xd4;
  }
  fVar17 = (float)((ulonglong)dVar26 >> 0x20);
  fVar15 = SUB84(dVar22,0);
  if (in_FPSCR_PR == '\0') {
    fVar21 = 1.0 / fVar8;
    dVar20 = (double)CONCAT44(fVar21,param_1);
    fVar14 = (SUB84(dVar16,0) - fVar17) * fVar21 * fVar15;
  }
  else {
    dVar20 = (double)CONCAT44(0x3f800000,param_1) / dVar18;
    fVar21 = (float)((ulonglong)dVar20 >> 0x20);
    fVar14 = SUB84((dVar16 - dVar26) * dVar20 * dVar22,0);
  }
  if (in_FPSCR_SZ == '\0') {
    pdVar7 = (double *)((int)pdVar7 + -4);
    *(float *)pdVar7 = fVar14;
  }
  else {
    pdVar7 = pdVar7 + -1;
    *pdVar7 = dVar11;
  }
  fVar25 = SUB84(dVar26,0);
  fVar24 = (float)((ulonglong)dVar27 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar16 = (double)CONCAT44((fVar9 - fVar25) * fVar21 * fVar24,fVar8);
  }
  else {
    dVar16 = (dVar18 - dVar26) * dVar20 * dVar27;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)pdVar7 + -4) = (int)((ulonglong)dVar16 >> 0x20);
    pdVar7 = pdVar7 + -1;
    *(float *)pdVar7 = fVar21;
  }
  else {
    pdVar7[-1] = dVar16;
    pdVar7 = pdVar7 + -2;
    *pdVar7 = dVar20;
  }
  pdVar6 = (double *)(unaff_r13[1] + in_r1);
  if (in_FPSCR_SZ == '\0') {
    dVar18 = (double)CONCAT44(*(undefined4 *)pdVar6,*(undefined4 *)((int)pdVar6 + 4));
    dVar16 = (double)CONCAT44(*(undefined4 *)(pdVar6 + 1),fVar14);
  }
  else {
    dVar18 = *pdVar6;
    in_xd0 = pdVar6[1];
    dVar16 = pdVar6[2];
  }
  dVar16 = (double)((ulonglong)dVar16 & 0xffffffff00000000);
                    /* WARNING: Ignoring partial resolution of indirect */
  if (in_FPSCR_PR == '\0') {
    fVar8 = SUB84(dVar18,0);
    fVar21 = (float)((ulonglong)dVar18 >> 0x20);
    fVar14 = (float)((ulonglong)dVar16 >> 0x20);
    fVar9 = SUB84(in_xd0,0) * fVar8 + SUB84(in_xd4,0) * fVar21 + in_xf8 * 0.0 + fVar10 * fVar14;
    fVar9 = (float)((ulonglong)in_xd0 >> 0x20) * fVar8 + (float)((ulonglong)in_xd4 >> 0x20) * fVar21
            + in_xf9 * 0.0 + fVar13 * fVar14;
    fVar9 = SUB84(dVar11,0) * fVar8 + in_xf6 * fVar21 + in_xf10 * 0.0 + fVar19 * fVar14;
    fVar8 = (float)((ulonglong)dVar11 >> 0x20) * fVar8 + in_xf7 * fVar21 + in_xf11 * 0.0 +
            fVar23 * fVar14;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),*(undefined4 *)PTR_DAT_8c038660);
  }
  else {
    dVar11 = *(double *)PTR_DAT_8c038660;
  }
  fVar8 = SUB84(dVar16,0);
  if (in_FPSCR_PR == '\0') {
    dVar18 = (double)CONCAT44((float)((ulonglong)dVar18 >> 0x20) * fVar8 + fVar8,
                              SUB84(dVar18,0) * fVar8 + fVar8);
  }
  else {
    dVar18 = (dVar18 * dVar16 + dVar16) * dVar16 + dVar16;
  }
  if (in_FPSCR_SZ == '\0') {
    fVar8 = (float)((ulonglong)dVar18 >> 0x20);
    dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),fVar8);
    *(float *)((int)pdVar7 + 0x4c) = SUB84(dVar18,0);
    pdVar7 = pdVar7 + 9;
    *(float *)pdVar7 = fVar8;
    dVar18 = dVar11;
  }
  else {
    *(double *)((int)pdVar7 + 0x4c) = in_xd0;
    pdVar7 = (double *)((int)pdVar7 + 0x44);
    *pdVar7 = dVar18;
  }
  *(float *)((int)pdVar7 + -4) = *(float *)(in_GBR + 0x4c);
  if (in_FPSCR_SZ == '\0') {
    dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),*(undefined4 *)unaff_r9);
    dVar11 = (double)CONCAT44(*(undefined4 *)((int)unaff_r9 + 4),*(undefined4 *)(unaff_r9 + 1));
    pdVar6 = (double *)((int)unaff_r9 + 0xc);
  }
  else {
    dVar18 = *unaff_r9;
    dVar11 = unaff_r9[1];
    in_xd4 = unaff_r9[2];
    pdVar6 = unaff_r9 + 3;
  }
  *(float *)(pdVar7 + -1) = *(float *)pdVar6;
  fVar9 = (float)((ulonglong)dVar11 >> 0x20);
  fVar8 = SUB84(dVar11,0);
  if (in_FPSCR_SZ == '\0') {
    *(float *)((int)pdVar7 + -0xc) = SUB84(dVar16,0);
    *(float *)(pdVar7 + -2) = fVar9;
    pdVar7 = (double *)((int)pdVar7 + -0x14);
    *(float *)pdVar7 = fVar8;
  }
  else {
    pdVar7[-2] = dVar18;
    pdVar7[-3] = dVar11;
    pdVar7 = pdVar7 + -4;
    *pdVar7 = in_xd4;
  }
  uVar12 = (undefined4)((ulonglong)dVar20 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    fVar21 = 1.0 / fVar8;
    dVar20 = (double)CONCAT44(uVar12,fVar21);
    fVar14 = (SUB84(dVar16,0) - fVar17) * fVar21 * fVar15;
  }
  else {
    dVar20 = (double)CONCAT44(uVar12,0x3f800000) / dVar11;
    fVar21 = SUB84(dVar20,0);
    fVar14 = SUB84((dVar16 - dVar26) * dVar20 * dVar22,0);
  }
  if (in_FPSCR_SZ == '\0') {
    pdVar7 = (double *)((int)pdVar7 + -4);
    *(float *)pdVar7 = fVar14;
  }
  else {
    pdVar7 = pdVar7 + -1;
    *pdVar7 = dVar18;
  }
  if (in_FPSCR_PR == '\0') {
    dVar22 = (double)CONCAT44((fVar9 - fVar25) * fVar21 * fVar24,fVar8);
  }
  else {
    dVar22 = (dVar11 - dVar26) * dVar20 * dVar27;
  }
  if (in_FPSCR_SZ == '\0') {
    *(float *)((int)pdVar7 + -4) = (float)((ulonglong)dVar22 >> 0x20);
    pdVar7 = pdVar7 + -1;
    *(float *)pdVar7 = fVar21;
  }
  else {
    pdVar7[-1] = dVar22;
    pdVar7 = pdVar7 + -2;
    *pdVar7 = (double)CONCAT44(in_xf7,in_xf6);
  }
  pdVar6 = (double *)(unaff_r13[2] + in_r1);
  if (in_FPSCR_SZ == '\0') {
    dVar16 = (double)CONCAT44(*(undefined4 *)pdVar6,*(undefined4 *)((int)pdVar6 + 4));
    dVar22 = (double)CONCAT44(*(undefined4 *)(pdVar6 + 1),fVar14);
  }
  else {
    dVar16 = *pdVar6;
    in_xd0 = pdVar6[1];
    dVar22 = pdVar6[2];
  }
  dVar22 = (double)((ulonglong)dVar22 & 0xffffffff00000000);
                    /* WARNING: Ignoring partial resolution of indirect */
  if (in_FPSCR_PR == '\0') {
    fVar8 = SUB84(dVar16,0);
    fVar9 = (float)((ulonglong)dVar16 >> 0x20);
    fVar21 = (float)((ulonglong)dVar22 >> 0x20);
    fVar10 = SUB84(in_xd0,0) * fVar8 + SUB84(in_xd4,0) * fVar9 + in_xf8 * 0.0 + fVar10 * fVar21;
    fVar10 = (float)((ulonglong)in_xd0 >> 0x20) * fVar8 + (float)((ulonglong)in_xd4 >> 0x20) * fVar9
             + in_xf9 * 0.0 + fVar13 * fVar21;
    fVar10 = SUB84(dVar18,0) * fVar8 + in_xf6 * fVar9 + in_xf10 * 0.0 + fVar19 * fVar21;
    fVar10 = (float)((ulonglong)dVar18 >> 0x20) * fVar8 + in_xf7 * fVar9 + in_xf11 * 0.0 +
             fVar23 * fVar21;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)PTR_DAT_8c038660);
  }
  else {
    dVar18 = *(double *)PTR_DAT_8c038660;
  }
  fVar10 = SUB84(dVar22,0);
  if (in_FPSCR_PR == '\0') {
    dVar16 = (double)CONCAT44((float)((ulonglong)dVar16 >> 0x20) * fVar10 + fVar10,
                              SUB84(dVar16,0) * fVar10 + fVar10);
  }
  else {
    dVar16 = (dVar16 * dVar22 + dVar22) * dVar22 + dVar22;
  }
  fVar10 = SUB84(dVar16,0);
  if (in_FPSCR_SZ == '\0') {
    fVar13 = (float)((ulonglong)dVar16 >> 0x20);
    dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),fVar13);
    *(float *)((int)pdVar7 + 0x4c) = fVar10;
    pdVar7 = pdVar7 + 9;
    *(float *)pdVar7 = fVar13;
    dVar16 = dVar18;
  }
  else {
    *(double *)((int)pdVar7 + 0x4c) = in_xd0;
    pdVar7 = (double *)((int)pdVar7 + 0x44);
    *pdVar7 = dVar16;
  }
  *(float *)((int)pdVar7 + -4) = *(float *)(in_GBR + 0x50);
  if (in_FPSCR_SZ == '\0') {
    dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)unaff_r10);
    dVar11 = (double)CONCAT44(*(undefined4 *)((int)unaff_r10 + 4),*(undefined4 *)(unaff_r10 + 1));
    pdVar6 = (double *)((int)unaff_r10 + 0xc);
  }
  else {
    dVar16 = *unaff_r10;
    dVar11 = unaff_r10[1];
    in_xd4 = unaff_r10[2];
    pdVar6 = unaff_r10 + 3;
  }
  *(float *)(pdVar7 + -1) = *(float *)pdVar6;
  fVar19 = (float)((ulonglong)dVar11 >> 0x20);
  fVar13 = SUB84(dVar11,0);
  if (in_FPSCR_SZ == '\0') {
    *(float *)((int)pdVar7 + -0xc) = SUB84(dVar22,0);
    *(float *)(pdVar7 + -2) = fVar19;
    pdVar7 = (double *)((int)pdVar7 + -0x14);
    *(float *)pdVar7 = fVar13;
  }
  else {
    pdVar7[-2] = dVar16;
    pdVar7[-3] = dVar11;
    pdVar7 = pdVar7 + -4;
    *pdVar7 = in_xd4;
  }
  if (in_FPSCR_PR == '\0') {
    fVar23 = 1.0 / fVar13;
    dVar18 = (double)CONCAT44(fVar23,fVar15);
    fVar15 = (SUB84(dVar22,0) - fVar17) * fVar23 * fVar15;
  }
  else {
    dVar18 = (double)CONCAT44(0x3f800000,fVar15) / dVar11;
    fVar23 = (float)((ulonglong)dVar18 >> 0x20);
    fVar15 = SUB84((dVar22 - dVar26) * dVar18 * dVar18,0);
  }
  if (in_FPSCR_SZ == '\0') {
    pdVar7 = (double *)((int)pdVar7 + -4);
    *(float *)pdVar7 = fVar15;
  }
  else {
    pdVar7 = pdVar7 + -1;
    *pdVar7 = dVar16;
  }
  if (in_FPSCR_PR == '\0') {
    dVar27 = (double)CONCAT44((fVar19 - fVar25) * fVar23 * fVar24,fVar13);
  }
  else {
    dVar27 = (dVar11 - dVar26) * dVar18 * dVar27;
  }
  if (in_FPSCR_SZ == '\0') {
    *(float *)((int)pdVar7 + -4) = (float)((ulonglong)dVar27 >> 0x20);
    *(float *)(pdVar7 + -1) = fVar23;
  }
  else {
    pdVar7[-1] = dVar27;
    pdVar7[-2] = dVar18;
  }
  dVar27 = (double)CONCAT44(0x3f800000,fVar10);
  if (in_FPSCR_PR == '\0') {
    bVar28 = 1.0 < (float)((ulonglong)dVar20 >> 0x20);
  }
  else {
    bVar28 = dVar27 < dVar20;
  }
  if (!bVar28) {
    if (in_FPSCR_PR == '\0') {
      bVar28 = 1.0 < SUB84(dVar20,0);
    }
    else {
      bVar28 = dVar27 < dVar20;
    }
    if (!bVar28) {
      if (in_FPSCR_PR == '\0') {
        bVar28 = 1.0 < fVar23;
      }
      else {
        bVar28 = dVar27 < dVar18;
      }
      if (bVar28 != true) {
        return CONCAT44(in_r1,(uint)bVar28 | uVar1 & 2 | uVar2 & 0xf0 |
                              (uint)((byte)(uVar3 >> 8) & 1) << 8 | uVar4 & 0x200 |
                              (uVar4 | (uint)in_FD << 0xf) & 0x8000 |
                              (uint)((bVar5 & 0x10) >> 4) << 0x1c |
                              (uint)(((bVar5 | (byte)(((uint)in_RB << 0x1d) >> 0x18) |
                                      (byte)(((uint)in_MD << 0x1e) >> 0x18)) & 0x40) >> 6) << 0x1e)
               | 0x20000000;
      }
    }
  }
  uVar29 = FUN_8c037ff4();
  return uVar29;
}

