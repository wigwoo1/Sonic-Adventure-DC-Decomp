/* Ghidra draft only; not buildable/verified source. */

/* WARNING (jumptable): Heritage AFTER dead removal. Revisit: 0x00000208 */
/* WARNING: Restarted to delay deadcode elimination for space: register */

undefined4
FUN_8c611444(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
            undefined4 *param_5)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  double *pdVar3;
  undefined8 *puVar4;
  undefined8 *puVar5;
  double *pdVar6;
  undefined8 *puVar7;
  uint uVar8;
  int iVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  int iVar12;
  undefined8 *puVar13;
  float fVar14;
  undefined8 uVar15;
  double dVar16;
  float fVar17;
  double dVar18;
  float fVar22;
  double dVar19;
  double dVar20;
  float fVar23;
  double dVar21;
  undefined4 uVar24;
  undefined4 uVar25;
  float fVar26;
  double dVar27;
  undefined8 in_dr14;
  double dVar28;
  double in_xd0;
  undefined8 uVar29;
  double in_xd2;
  double dVar30;
  double in_xd4;
  double in_xd6;
  double in_xd8;
  double in_xd10;
  double dVar31;
  float in_xf12;
  float in_xf13;
  float in_xf14;
  float in_xf15;
  bool bVar32;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined8 local_20;
  
  puVar7 = DAT_8c6114d4;
  puVar4 = DAT_8c6114c4;
  pdVar3 = DAT_8c6114c0;
  puVar2 = DAT_8c6114bc;
  puVar1 = DAT_8c6114b8;
  if (in_FPSCR_SZ == '\0') {
    puVar13 = (undefined8 *)((int)&local_20 + 4);
    local_20 = CONCAT44((int)((ulonglong)in_dr14 >> 0x20),(undefined4)local_20);
  }
  else {
    puVar13 = &local_20;
    local_20 = in_dr14;
  }
  *(undefined4 *)((int)puVar13 + -4) = in_PR;
  *(undefined4 **)((int)puVar13 + -0x44) = param_5;
  uVar11 = *DAT_8c6114b0;
  iVar12 = param_5[1];
  uVar10 = *param_5;
  *DAT_8c6114b4 = 0;
  puVar5 = DAT_8c6114cc;
  *puVar1 = *puVar2;
  pdVar6 = DAT_8c6114d0;
  if (in_FPSCR_SZ == '\0') {
    dVar18 = (double)CONCAT44(*(undefined4 *)pdVar3,*(undefined4 *)puVar4);
  }
  else {
    in_xd2 = (double)*puVar4;
    dVar18 = *pdVar3;
  }
  if (in_FPSCR_PR == '\0') {
    dVar18 = (double)CONCAT44((float)((ulonglong)dVar18 >> 0x20) * SUB84(dVar18,0),SUB84(dVar18,0));
  }
  else {
    dVar18 = dVar18 * dVar18;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)DAT_8c6114c8 = (int)((ulonglong)dVar18 >> 0x20);
    dVar18 = (double)CONCAT44(*(undefined4 *)pdVar3,*(undefined4 *)puVar5);
  }
  else {
    *DAT_8c6114c8 = dVar18;
    dVar18 = *pdVar3;
    in_xd2 = (double)*puVar5;
  }
  uVar8 = (uint)DAT_8c6114ae;
  if (in_FPSCR_PR == '\0') {
    dVar18 = (double)CONCAT44((float)((ulonglong)dVar18 >> 0x20) * SUB84(dVar18,0),SUB84(dVar18,0));
  }
  else {
    dVar18 = dVar18 * dVar18;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)pdVar6 = (int)((ulonglong)dVar18 >> 0x20);
  }
  else {
    *pdVar6 = dVar18;
  }
  puVar4 = DAT_8c6115d8;
  pdVar6 = DAT_8c6115d4;
  if ((*DAT_8c6114d8 & uVar8) == 0) {
                    /* WARNING: Could not recover jumptable at 0x8c61149a. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar10 = (*DAT_8c6114dc)();
    return uVar10;
  }
  iVar9 = *(int *)((int)puVar13 + -0x44);
  if (in_FPSCR_SZ == '\0') {
    uVar15 = CONCAT44(*(undefined4 *)(iVar9 + 0x18),*(undefined4 *)(iVar9 + 0x1c));
    fVar22 = *(float *)(iVar9 + 0x20);
    iVar9 = iVar9 + 0x24;
  }
  else {
    uVar15 = *(undefined8 *)(iVar9 + 0x18);
    in_xd0 = *(double *)(iVar9 + 0x20);
    fVar22 = (float)((ulonglong)*(undefined8 *)(iVar9 + 0x28) >> 0x20);
    iVar9 = iVar9 + 0x30;
  }
  uVar29 = CONCAT44(fVar22,0x3f800000);
                    /* WARNING (jumptable): Ignoring partial resolution of indirect */
                    /* WARNING: Ignoring partial resolution of indirect */
  if (in_FPSCR_PR == '\0') {
    fVar17 = (float)uVar15;
    fVar23 = (float)((ulonglong)uVar15 >> 0x20);
    fVar14 = SUB84(in_xd0,0) * fVar17 + SUB84(in_xd4,0) * fVar23 + SUB84(in_xd8,0) * 1.0 +
             in_xf12 * fVar22;
    fVar14 = (float)((ulonglong)in_xd0 >> 0x20) * fVar17 +
             (float)((ulonglong)in_xd4 >> 0x20) * fVar23 + (float)((ulonglong)in_xd8 >> 0x20) * 1.0
             + in_xf13 * fVar22;
    fVar14 = SUB84(in_xd2,0) * fVar17 + SUB84(in_xd6,0) * fVar23 + SUB84(in_xd10,0) * 1.0 +
             in_xf14 * fVar22;
    fVar22 = (float)((ulonglong)in_xd2 >> 0x20) * fVar17 +
             (float)((ulonglong)in_xd6 >> 0x20) * fVar23 + (float)((ulonglong)in_xd10 >> 0x20) * 1.0
             + in_xf15 * fVar22;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)puVar13 + -0x38) = (int)((ulonglong)uVar29 >> 0x20);
    *(int *)((int)puVar13 + -0x3c) = (int)uVar15;
    *(int *)((int)puVar13 + -0x40) = (int)((ulonglong)uVar15 >> 0x20);
  }
  else {
    *(undefined8 *)((int)puVar13 + -0x3c) = uVar29;
    *(double *)((int)puVar13 + -0x44) = in_xd0;
    *(undefined8 *)((int)puVar13 + -0x4c) = uVar15;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar27 = (double)CONCAT44(*(undefined4 *)(*(int *)((int)puVar13 + -0x44) + 0x24),param_4);
    fVar22 = *(float *)((int)puVar13 + -0x38);
    dVar19 = (double)CONCAT44(*(undefined4 *)DAT_8c6115bc,fVar22);
    dVar18 = (double)CONCAT44(fVar22,param_1);
  }
  else {
    dVar27 = *(double *)(*(int *)((int)puVar13 + -0x44) + 0x24);
    in_xd2 = *(double *)((int)puVar13 + -0x38);
    dVar19 = *DAT_8c6115bc;
    fVar22 = (float)((ulonglong)in_xd2 >> 0x20);
    dVar18 = in_xd2;
  }
  fVar14 = (float)((ulonglong)dVar27 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),SUB84(dVar19,0) + fVar14);
  }
  else {
    dVar19 = dVar19 + dVar27;
  }
  fVar17 = (float)((ulonglong)dVar19 >> 0x20);
  if (in_FPSCR_PR != '\0' || SUB84(dVar19,0) <= fVar17) {
                    /* WARNING: Could not recover jumptable at 0x8c61150a. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar10 = (*DAT_8c6115c0)();
    return uVar10;
  }
  dVar28 = dVar18;
  if (in_FPSCR_SZ == '\0') {
    dVar19 = (double)CONCAT44(fVar17,fVar22);
    dVar28 = in_xd2;
  }
  if (in_FPSCR_PR == '\0') {
    fVar17 = SUB84(dVar19,0) - fVar14;
  }
  else {
    fVar17 = SUB84(dVar19 - dVar27,0);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar19 = (double)CONCAT44(*(undefined4 *)DAT_8c6115c4,fVar17);
  }
  else {
    dVar19 = *DAT_8c6115c4;
  }
  fVar17 = (float)((ulonglong)dVar19 >> 0x20);
  if (in_FPSCR_PR == '\0' && fVar17 < SUB84(dVar19,0)) {
                    /* WARNING: Could not recover jumptable at 0x8c61151c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar10 = (*DAT_8c6115c0)(iVar9);
    return uVar10;
  }
  if (in_FPSCR_PR == '\0') {
    dVar18 = (double)CONCAT44(fVar22 + fVar14,SUB84(dVar18,0));
  }
  else {
    dVar18 = dVar18 + dVar27;
  }
  if (in_FPSCR_SZ == '\0') {
    param_3 = *(undefined4 *)((int)puVar13 + -0x40);
    param_2 = *(undefined4 *)((int)pdVar3 + 0xc);
    dVar19 = (double)CONCAT44(fVar17,(int)((ulonglong)dVar18 >> 0x20));
  }
  else {
    in_xd8 = *(double *)((int)puVar13 + -0x40);
    in_xd6 = *(double *)((int)pdVar3 + 0xc);
    dVar28 = dVar18;
  }
  if (in_FPSCR_PR == '\0') {
    dVar18 = (double)CONCAT44(1.0 / SUB84(dVar19,0),SUB84(dVar18,0));
  }
  else {
    dVar18 = (double)CONCAT44(0x3f800000,SUB84(dVar18,0)) / dVar19;
  }
  dVar21 = in_xd8;
  if (in_FPSCR_SZ == '\0') {
    dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),param_3);
    dVar21 = dVar28;
  }
  if (in_FPSCR_PR == '\0') {
    fVar22 = SUB84(dVar19,0) - fVar14;
  }
  else {
    fVar22 = SUB84(dVar19 - dVar27,0);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar19 = (double)CONCAT44(*(undefined4 *)DAT_8c6115c8,param_3);
  }
  else {
    dVar19 = *DAT_8c6115c8;
  }
  if (in_FPSCR_PR == '\0') {
    dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),SUB84(dVar19,0) + fVar14);
  }
  else {
    dVar19 = dVar19 + dVar27;
  }
  fVar17 = (float)((ulonglong)dVar19 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    dVar28 = (double)CONCAT44(*(undefined4 *)((int)puVar13 + -0x3c),(int)in_dr14);
    dVar20 = (double)CONCAT44(fVar17,fVar22);
  }
  else {
    dVar28 = *(double *)((int)puVar13 + -0x3c);
    dVar20 = dVar19;
  }
  if (in_FPSCR_PR == '\0') {
    dVar20 = (double)CONCAT44((float)((ulonglong)dVar20 >> 0x20) * SUB84(dVar20,0),SUB84(dVar20,0));
  }
  else {
    dVar20 = dVar20 * dVar20;
  }
  fVar22 = (float)((ulonglong)dVar28 >> 0x20);
  dVar30 = dVar28;
  if (in_FPSCR_SZ == '\0') {
    dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),fVar22);
    dVar30 = dVar21;
  }
  if (in_FPSCR_PR == '\0') {
    dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0) - fVar14);
  }
  else {
    dVar20 = dVar20 - dVar27;
  }
  dVar21 = in_xd6;
  if (in_FPSCR_SZ == '\0') {
    dVar18 = (double)CONCAT44((int)((ulonglong)dVar18 >> 0x20),param_2);
    dVar21 = in_xd4;
  }
  if (in_FPSCR_PR == '\0') {
    dVar19 = (double)CONCAT44(fVar17 * SUB84(dVar19,0),SUB84(dVar19,0));
  }
  else {
    dVar19 = dVar19 * dVar19;
  }
  if (in_FPSCR_SZ == '\0') {
    fVar17 = *(float *)(pdVar3 + 2);
    dVar27 = (double)CONCAT44(fVar14,fVar17);
    dVar31 = (double)CONCAT44(fVar17,param_2);
    dVar16 = (double)CONCAT44((int)((ulonglong)dVar18 >> 0x20),*(undefined4 *)DAT_8c6115cc);
  }
  else {
    in_xd0 = *DAT_8c6115cc;
    dVar31 = pdVar3[2];
    fVar17 = SUB84(dVar27,0);
    dVar16 = dVar18;
    in_xd10 = dVar31;
  }
  fVar14 = (float)((ulonglong)dVar16 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar18 = (double)(ulonglong)
                     (uint)(fVar14 * (float)((ulonglong)dVar20 >> 0x20) + SUB84(dVar18,0));
  }
  if (in_FPSCR_SZ == '\0') {
    in_xd0 = (double)CONCAT44(SUB84(dVar16,0),SUB84(dVar20,0));
  }
  if (in_FPSCR_PR == '\0') {
    fVar23 = (float)((ulonglong)in_xd0 >> 0x20) * SUB84(in_xd0,0);
    dVar20 = (double)CONCAT44(fVar23,SUB84(in_xd0,0));
    dVar31 = (double)CONCAT44(fVar14 * fVar23 + (float)((ulonglong)dVar31 >> 0x20),
                              fVar14 * (float)((ulonglong)dVar19 >> 0x20) + SUB84(dVar31,0));
    fVar22 = fVar22 + (float)((ulonglong)dVar27 >> 0x20);
  }
  else {
    dVar20 = in_xd0 * in_xd0;
    fVar23 = (float)((ulonglong)dVar20 >> 0x20);
    fVar22 = (float)((ulonglong)(dVar28 + dVar27) >> 0x20);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar20 = (double)CONCAT44(fVar23,*(undefined4 *)puVar7);
  }
  else {
    dVar30 = (double)*puVar7;
  }
  if (in_FPSCR_PR == '\0') {
    bVar32 = SUB84(dVar31,0) < SUB84(dVar20,0);
    dVar27 = (double)(ulonglong)(uint)(fVar14 * SUB84(dVar16,0) * fVar22 + fVar17);
  }
  else {
    bVar32 = dVar31 < dVar20;
  }
  uVar24 = SUB84(dVar27,0);
  dVar27 = in_xd10;
  if (in_FPSCR_SZ == '\0') {
    dVar27 = (double)CONCAT44(uVar24,SUB84(dVar18,0));
  }
  if (bVar32) {
                    /* WARNING: Could not recover jumptable at 0x8c611574. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar10 = (*DAT_8c6115c0)();
    return uVar10;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),*(undefined4 *)(puVar7 + 1));
  }
  else {
    dVar30 = (double)puVar7[1];
  }
  if (in_FPSCR_PR == '\0') {
    bVar32 = SUB84(dVar20,0) < SUB84(dVar27,0);
  }
  else {
    bVar32 = dVar20 < dVar27;
  }
  if (!bVar32) {
    if (in_FPSCR_SZ == '\0') {
      dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),*(undefined4 *)((int)puVar7 + 4));
    }
    else {
      dVar30 = *(double *)((int)puVar7 + 4);
    }
    if (in_FPSCR_PR == '\0') {
      bVar32 = (float)((ulonglong)dVar27 >> 0x20) < SUB84(dVar20,0);
    }
    else {
      bVar32 = dVar27 < dVar20;
    }
    if (bVar32) {
                    /* WARNING: Could not recover jumptable at 0x8c611590. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      uVar10 = (*DAT_8c6115c0)();
      return uVar10;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),*(undefined4 *)((int)puVar7 + 0xc))
      ;
    }
    else {
      dVar30 = *(double *)((int)puVar7 + 0xc);
    }
    if (in_FPSCR_PR == '\0') {
      bVar32 = SUB84(dVar20,0) < (float)((ulonglong)dVar31 >> 0x20);
    }
    else {
      bVar32 = dVar20 < dVar31;
    }
    if (!bVar32) {
      *DAT_8c6115d0 = *DAT_8c6115d0 + *(int *)(*(int *)((int)puVar13 + -0x44) + 8);
      puVar2 = DAT_8c6115dc;
      if (in_FPSCR_SZ == '\0') {
        uVar24 = *(undefined4 *)pdVar6;
        fVar22 = *(float *)((int)puVar4 + 4);
        uVar15 = CONCAT44(*(undefined4 *)puVar4,fVar22);
        fVar14 = *(float *)(puVar4 + 1);
        uVar29 = CONCAT44(fVar22,SUB84(dVar27,0));
        dVar30 = (double)((ulonglong)dVar31 & 0xffffffff);
      }
      else {
        in_xd10 = *pdVar6;
        uVar15 = *puVar4;
        uVar29 = puVar4[1];
        fVar14 = (float)((ulonglong)puVar4[2] >> 0x20);
        fVar22 = (float)uVar15;
      }
                    /* WARNING: Ignoring partial resolution of indirect */
      fVar17 = (float)((ulonglong)uVar15 >> 0x20);
      fVar23 = (float)uVar29;
      if (in_FPSCR_PR == '\0') {
        fVar26 = fVar23 * fVar22 + (float)((ulonglong)uVar29 >> 0x20) * fVar17 +
                 SUB84(dVar30,0) * 0.0 + (float)((ulonglong)dVar30 >> 0x20) * fVar14;
      }
      uVar25 = SUB84(dVar30,0);
      dVar18 = in_xd6;
      dVar27 = in_xd6;
      if (in_FPSCR_SZ == '\0') {
        dVar21 = (double)CONCAT44(fVar23,fVar23);
        dVar18 = (double)CONCAT44(uVar25,uVar25);
        dVar27 = (double)CONCAT44(uVar25,SUB84(dVar19,0));
      }
                    /* WARNING: Ignoring partial resolution of indirect */
      if (in_FPSCR_PR == '\0') {
        fVar23 = SUB84(dVar21,0) * fVar22 + (float)((ulonglong)dVar21 >> 0x20) * fVar17 +
                 SUB84(dVar18,0) * 0.0 + (float)((ulonglong)dVar18 >> 0x20) * fVar14;
      }
      fVar26 = SUB84(dVar18,0);
      fVar23 = SUB84(dVar27,0);
      dVar19 = in_xd6;
      dVar18 = in_xd6;
      if (in_FPSCR_SZ == '\0') {
        dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),fVar26);
        in_xd10 = (double)CONCAT44(uVar24,fVar26);
        dVar19 = (double)CONCAT44(fVar26,SUB84(dVar21,0));
        fVar23 = fVar26;
        dVar18 = in_xd8;
      }
                    /* WARNING: Ignoring partial resolution of indirect */
      if (in_FPSCR_PR == '\0') {
        fVar22 = SUB84(dVar19,0) * fVar22 + (float)((ulonglong)dVar19 >> 0x20) * fVar17 +
                 SUB84(in_xd10,0) * 0.0 + (float)((ulonglong)in_xd10 >> 0x20) * fVar14;
      }
      fVar22 = (float)((ulonglong)dVar27 >> 0x20);
      dVar28 = in_xd6;
      if (in_FPSCR_SZ == '\0') {
        in_xd6 = (double)((longlong)in_xd10 << 0x20);
        dVar28 = (double)CONCAT44(SUB84(in_xd10,0),uVar24);
      }
                    /* WARNING (jumptable): Ignoring partial resolution of indirect */
                    /* WARNING: Ignoring partial resolution of indirect */
      fVar14 = SUB84(dVar28,0);
      fVar17 = (float)((ulonglong)dVar28 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar26 = fVar23 * fVar23 + fVar22 * fVar22 + SUB84(in_xd6,0) * fVar14 +
                 (float)((ulonglong)in_xd6 >> 0x20) * fVar17;
        fVar26 = (1.0 / SQRT(SUB84(in_xd6,0))) * fVar14;
        dVar28 = (double)CONCAT44(fVar17 * fVar26,fVar14);
        dVar21 = (double)CONCAT44(fVar22 * fVar26,fVar23 * fVar26);
      }
      else {
        dVar21 = in_xd6 * dVar28;
        dVar28 = dVar28 * dVar21;
        dVar21 = dVar27 * dVar21 * dVar21;
      }
      if (in_FPSCR_SZ == '\0') {
        DAT_8c6115dc[2] = (int)((ulonglong)dVar28 >> 0x20);
        puVar2[1] = SUB84(dVar21,0);
        *puVar2 = (int)((ulonglong)dVar21 >> 0x20);
      }
      else {
        *(double *)(DAT_8c6115dc + 1) = dVar28;
        *(double *)(puVar2 + -1) = dVar18;
        *(double *)(puVar2 + -3) = dVar21;
      }
      *(undefined4 *)((int)puVar13 + -0x34) = *(undefined4 *)(*(int *)((int)puVar13 + -0x44) + 8);
      *(undefined4 *)((int)puVar13 + -0x30) = uVar11;
      *(undefined4 *)((int)puVar13 + -0x2c) = uVar10;
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)((int)puVar13 + -0x1c) = *(undefined4 *)DAT_8c6116e8;
        *(undefined4 *)((int)puVar13 + -0x18) = *(undefined4 *)DAT_8c6116ec;
      }
      else {
        *(undefined8 *)((int)puVar13 + -0x1c) = *DAT_8c6116e8;
        *(undefined8 *)((int)puVar13 + -0x18) = *DAT_8c6116ec;
      }
      if ((*DAT_8c6116f0 & 2) == 0) {
        *(undefined4 *)((int)puVar13 + -0x24) = 0xffffffff;
        *(undefined4 *)((int)puVar13 + -0x20) = 0;
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)((int)puVar13 + -0x14) = *(undefined4 *)puVar7;
          *(undefined4 *)((int)puVar13 + -0x10) = *(undefined4 *)(puVar7 + 1);
          *(undefined4 *)((int)puVar13 + -0xc) = *(undefined4 *)((int)puVar7 + 4);
          *(undefined4 *)((int)puVar13 + -8) = *(undefined4 *)((int)puVar7 + 0xc);
        }
        else {
          *(undefined8 *)((int)puVar13 + -0x14) = *puVar7;
          *(undefined8 *)((int)puVar13 + -0x10) = puVar7[1];
          *(undefined8 *)((int)puVar13 + -0xc) = *(undefined8 *)((int)puVar7 + 4);
          *(undefined8 *)((int)puVar13 + -8) = *(undefined8 *)((int)puVar7 + 0xc);
        }
        if (iVar12 == 0) {
          (*(code *)PTR_FUN_8c611700)(dVar19,(undefined1 *)((int)puVar13 + -0x34),*puVar1);
        }
        else {
          *(int *)((int)puVar13 + -0x28) = iVar12;
          (*(code *)PTR_FUN_8c6116fc)((undefined1 *)((int)puVar13 + -0x34),*puVar1);
        }
        uVar8 = *(uint *)((int)puVar13 + -0x24);
        if ((((uVar8 & 0x3c) != 0) || ((uVar8 & 2) != 0)) || ((uVar8 & 1) != 0)) {
          return 0xffffffff;
        }
        *DAT_8c611704 = *(undefined4 *)((int)puVar13 + -0x20);
      }
      else if (iVar12 == 0) {
        (*(code *)PTR_FUN_8c6116f8)((undefined1 *)((int)puVar13 + -0x34),*puVar1);
      }
      else {
        *(int *)((int)puVar13 + -0x28) = iVar12;
        (*(code *)PTR_FUN_8c6116f4)((undefined1 *)((int)puVar13 + -0x34),*puVar1);
      }
      return 0;
    }
                    /* WARNING: Could not recover jumptable at 0x8c61159e. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar10 = (*DAT_8c6115c0)();
    return uVar10;
  }
                    /* WARNING: Could not recover jumptable at 0x8c611582. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  uVar10 = (*DAT_8c6115c0)();
  return uVar10;
}

