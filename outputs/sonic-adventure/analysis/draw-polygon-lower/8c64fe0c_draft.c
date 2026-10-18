/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c64ffc2) */
/* WARNING: Removing unreachable block (ram,0x8c64fe56) */
/* WARNING: Removing unreachable block (ram,0x8c64fe6e) */
/* WARNING: Removing unreachable block (ram,0x8c64ffdc) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c64fe0c(undefined4 param_1,undefined4 param_2,double param_3,int param_4,int *param_5,
                 int *param_6)

{
  double *pdVar1;
  undefined4 uVar2;
  double *pdVar3;
  undefined4 uVar4;
  int iVar5;
  int iVar6;
  double *pdVar7;
  int iVar8;
  double *pdVar9;
  undefined4 uVar10;
  double *pdVar11;
  undefined4 uVar12;
  undefined4 *puVar13;
  float *pfVar14;
  float fVar15;
  double in_dr0;
  undefined4 uVar16;
  float fVar17;
  float fVar18;
  float fVar19;
  double in_dr2;
  double dVar20;
  double dVar21;
  undefined4 uVar22;
  float fVar23;
  double dVar24;
  float fVar25;
  double dVar26;
  float fVar27;
  double dVar28;
  undefined4 uVar30;
  double dVar29;
  float fVar31;
  double in_dr12;
  float fVar32;
  float fVar33;
  double in_dr14;
  double in_xd0;
  double in_xd2;
  double dVar34;
  double dVar35;
  double in_xd8;
  double in_xd10;
  double in_xd12;
  double dVar36;
  double dVar37;
  double in_xd14;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_34 [12];
  float local_28 [3];
  
  uVar4 = _DAT_8c650140;
  pdVar3 = DAT_8c650138;
  uVar2 = DAT_8c650134;
  pdVar1 = DAT_8c650128;
  fVar17 = SUB84(in_dr14,0);
  fVar33 = SUB84(in_dr12,0);
  if (in_FPSCR_SZ == '\0') {
    pfVar14 = local_28;
    stack0xffffffd4 = CONCAT44(fVar33,auStack_34._8_4_);
  }
  else {
    pfVar14 = (float *)auStack_34;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar24 = (double)CONCAT44(*(undefined4 *)DAT_8c650128,param_1);
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)DAT_8c65012c);
  }
  else {
    dVar24 = *DAT_8c650128;
    in_xd2 = *DAT_8c65012c;
  }
  param_4 = param_4 + -1;
  iVar5 = *param_5;
  uVar16 = _DAT_8c650140;
  if (in_FPSCR_SZ == '\0') {
    uVar16 = (undefined4)((ulonglong)dVar24 >> 0x20);
    dVar24 = (double)CONCAT44(uVar16,uVar16);
    uVar16 = DAT_8c65013c;
  }
  if (in_FPSCR_PR == '\0') {
    dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),SUB84(dVar24,0) * SUB84(in_dr2,0));
  }
  else {
    dVar24 = dVar24 * in_dr2;
  }
  if (in_FPSCR_SZ == '\0') {
    param_2 = *(undefined4 *)DAT_8c650138;
    dVar20 = (double)CONCAT44(*(undefined4 *)DAT_8c650130,SUB84(in_dr2,0));
  }
  else {
    in_xd8 = *DAT_8c650138;
    dVar20 = *DAT_8c650130;
  }
  if (in_FPSCR_PR == '\0') {
    dVar26 = (double)CONCAT44(uVar16,0x40000000);
  }
  else {
    dVar26 = (double)CONCAT44(uVar16,0x3f800000) + (double)CONCAT44(uVar16,0x3f800000);
  }
  dVar28 = dVar24;
  if (in_FPSCR_SZ == '\0') {
    dVar28 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),param_2);
  }
  fVar23 = (float)((ulonglong)dVar20 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) * fVar23,SUB84(dVar28,0));
  }
  else {
    dVar28 = dVar28 * dVar20;
  }
  iVar8 = *param_6;
  *(int *)((int)pfVar14 + -0x30) = iVar5;
  if (in_FPSCR_SZ == '\0') {
    dVar20 = (double)CONCAT44(fVar23,*(undefined4 *)(iVar5 + 8));
  }
  else {
    in_xd2 = *(double *)(iVar5 + 8);
  }
  fVar23 = SUB84(dVar24,0);
  dVar24 = (double)CONCAT44(0x3f800000,fVar23);
  fVar25 = SUB84(dVar26,0);
  fVar27 = (float)((ulonglong)dVar26 >> 0x20);
  if (SUB84(dVar20,0) == SUB84(dVar28,0)) {
    if (in_FPSCR_SZ == '\0') {
      dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),
                                *(undefined4 *)(param_4 * 0xc + *(int *)((int)pfVar14 + -0x30) + 8))
      ;
    }
    else {
      in_xd2 = *(double *)(param_4 * 0xc + *(int *)((int)pfVar14 + -0x30) + 8);
    }
    if (SUB84(dVar20,0) == SUB84(dVar28,0)) goto LAB_8c64ff86;
  }
  pdVar9 = (double *)*param_5;
  if (in_FPSCR_SZ == '\0') {
    dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),*(undefined4 *)(pdVar9 + 1));
    param_3 = (double)CONCAT44(0x3f800000,SUB84(param_3,0));
  }
  else {
    in_xd2 = pdVar9[1];
    param_3 = dVar24;
  }
  if (in_FPSCR_PR == '\0') {
    param_3 = (double)CONCAT44((float)((ulonglong)param_3 >> 0x20) / SUB84(dVar20,0),
                               SUB84(param_3,0));
  }
  else {
    param_3 = param_3 / dVar20;
  }
  pdVar11 = (double *)(param_4 * 0xc + (int)pdVar9);
  if (in_FPSCR_SZ == '\0') {
    dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),0x3f800000);
    dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),*(undefined4 *)(pdVar11 + 1));
    fVar17 = *(float *)((int)DAT_8c650128 + 0xc);
  }
  else {
    in_xd2 = pdVar11[1];
    in_xd14 = *(double *)((int)DAT_8c650128 + 0xc);
    in_xd8 = dVar24;
  }
  if (in_FPSCR_PR == '\0') {
    dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),SUB84(dVar28,0) / SUB84(dVar20,0));
  }
  else {
    dVar28 = dVar28 / dVar20;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),*(undefined4 *)pdVar9);
    param_3 = (double)CONCAT44((int)((ulonglong)param_3 >> 0x20),fVar17);
  }
  else {
    in_xd2 = *pdVar9;
    in_xd10 = in_xd14;
  }
  if (in_FPSCR_PR == '\0') {
    dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0) * fVar23);
  }
  else {
    dVar20 = dVar20 * dVar24;
  }
  uVar16 = (undefined4)((ulonglong)param_3 >> 0x20);
  dVar29 = param_3;
  if (in_FPSCR_SZ == '\0') {
    dVar29 = (double)CONCAT44(uVar16,SUB84(in_dr0,0));
  }
  if (in_FPSCR_PR == '\0') {
    param_3 = (double)CONCAT44(uVar16,(float)((ulonglong)dVar29 >> 0x20) * SUB84(dVar20,0) +
                                      SUB84(param_3,0));
  }
  if (in_FPSCR_SZ == '\0') {
    dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),*(undefined4 *)pdVar11);
  }
  else {
    in_xd2 = *pdVar11;
  }
  if (in_FPSCR_PR == '\0') {
    dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0) * fVar23);
  }
  else {
    dVar20 = dVar20 * dVar24;
  }
  uVar16 = SUB84(dVar28,0);
  dVar36 = in_xd8;
  if (in_FPSCR_SZ == '\0') {
    dVar36 = (double)CONCAT44(uVar16,SUB84(dVar29,0));
  }
  if (in_FPSCR_PR == '\0') {
    fVar17 = (float)((ulonglong)dVar36 >> 0x20) * SUB84(dVar20,0) + fVar17;
  }
  uVar30 = (undefined4)((ulonglong)param_3 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),*(undefined4 *)((int)pdVar9 + 4));
    dVar29 = (double)CONCAT44(uVar30,SUB84(dVar36,0));
  }
  else {
    in_xd2 = *(double *)((int)pdVar9 + 4);
    dVar29 = param_3;
  }
  fVar31 = (float)((ulonglong)dVar28 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0) * fVar31);
  }
  else {
    dVar20 = dVar20 * dVar28;
  }
  if (in_FPSCR_SZ == '\0') {
    fVar33 = *(float *)(DAT_8c650128 + 2);
    in_dr12 = (double)CONCAT44((int)((ulonglong)in_dr12 >> 0x20),fVar33);
    dVar36 = (double)CONCAT44(fVar33,fVar17);
  }
  else {
    dVar36 = DAT_8c650128[2];
    in_xd12 = dVar36;
  }
  if (in_FPSCR_PR == '\0') {
    dVar36 = (double)CONCAT44((float)((ulonglong)dVar29 >> 0x20) * SUB84(dVar20,0) +
                              (float)((ulonglong)dVar36 >> 0x20),SUB84(dVar36,0));
  }
  if (in_FPSCR_SZ == '\0') {
    dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),*(undefined4 *)((int)pdVar11 + 4));
    dVar29 = (double)CONCAT44(uVar16,SUB84(dVar29,0));
  }
  else {
    in_xd2 = *(double *)((int)pdVar11 + 4);
    dVar29 = in_xd8;
  }
  if (in_FPSCR_PR == '\0') {
    dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0) * fVar31);
  }
  else {
    dVar20 = dVar20 * dVar28;
  }
  fVar17 = (float)((ulonglong)dVar36 >> 0x20);
  dVar37 = dVar36;
  if (in_FPSCR_SZ == '\0') {
    dVar29 = (double)CONCAT44((int)((ulonglong)dVar29 >> 0x20),fVar17);
    dVar37 = in_xd0;
  }
  if (in_FPSCR_PR == '\0') {
    in_dr12 = (double)CONCAT44((int)((ulonglong)in_dr12 >> 0x20),
                               (float)((ulonglong)dVar29 >> 0x20) * SUB84(dVar20,0) + fVar33);
  }
  fVar33 = SUB84(param_3,0);
  dVar21 = in_xd10;
  if (in_FPSCR_SZ == '\0') {
    dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),fVar33);
    dVar21 = in_xd2;
  }
  fVar32 = SUB84(dVar36,0);
  fVar31 = SUB84(in_dr12,0);
  if (in_FPSCR_PR == '\0') {
    fVar18 = SUB84(dVar20,0) - fVar32;
    fVar15 = SUB84(dVar29,0) - fVar31;
  }
  else {
    fVar18 = SUB84(dVar20 - dVar36,0);
    fVar15 = SUB84(dVar29 - in_dr12,0);
  }
  if (in_FPSCR_SZ == '\0') {
    *(float *)((int)pfVar14 + -0x14) = fVar18;
    dVar29 = (double)CONCAT44(fVar15,fVar18);
    dVar20 = (double)CONCAT44(fVar18,fVar15);
  }
  else {
    *(double *)((int)pfVar14 + -0x14) = dVar21;
    fVar15 = SUB84(dVar21,0);
    dVar29 = dVar37;
    dVar20 = dVar21;
  }
  fVar18 = (float)((ulonglong)dVar20 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar29 = (double)CONCAT44(fVar18 * SUB84(dVar29,0) + (float)((ulonglong)dVar29 >> 0x20) * fVar15
                              ,SUB84(dVar29,0));
  }
  else {
    dVar29 = dVar29 * dVar20;
  }
  dVar35 = dVar24;
  if (in_FPSCR_SZ == '\0') {
    dVar29 = (double)CONCAT44((int)((ulonglong)dVar29 >> 0x20),0x3f800000);
    dVar35 = dVar21;
  }
  if (in_FPSCR_PR == '\0') {
    fVar19 = SQRT((float)((ulonglong)dVar29 >> 0x20));
    dVar29 = (double)CONCAT44(fVar19,SUB84(dVar29,0) / fVar19);
  }
  else {
    dVar29 = SQRT(dVar29) / SQRT(dVar29);
  }
  fVar19 = SUB84(dVar29,0);
  if (in_FPSCR_SZ == '\0') {
    *(float *)((int)pfVar14 + -0xc) = fVar19;
  }
  else {
    *(double *)((int)pfVar14 + -0xc) = dVar35;
  }
  if (in_FPSCR_PR == '\0') {
    dVar20 = (double)CONCAT44(fVar18,fVar15 * fVar19);
  }
  else {
    dVar20 = dVar20 * dVar29;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar29 = (double)CONCAT44(*(undefined4 *)((int)pfVar14 + -0x14),fVar19);
  }
  else {
    dVar29 = *(double *)((int)pfVar14 + -0x14);
  }
  if (in_FPSCR_PR == '\0') {
    in_dr0 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0) * fVar27);
    dVar20 = (double)CONCAT44((float)((ulonglong)dVar29 >> 0x20) * SUB84(dVar29,0) * fVar27,
                              SUB84(dVar29,0));
  }
  else {
    in_dr0 = dVar20 * dVar26;
    dVar20 = dVar29 * dVar29 * dVar26;
  }
  fVar18 = SUB84(in_dr0,0);
  dVar29 = dVar37;
  if (in_FPSCR_SZ == '\0') {
    dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),fVar18);
    dVar29 = dVar35;
  }
  if (in_FPSCR_PR == '\0') {
    dVar20 = (double)CONCAT44(-(float)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0) * fVar25);
  }
  else {
    dVar20 = -(dVar20 * dVar26);
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)pfVar14 + -0x10) = (int)((ulonglong)dVar20 >> 0x20);
    *(int *)((int)pfVar14 + -4) = SUB84(dVar20,0);
    dVar20 = (double)CONCAT44(*(undefined4 *)((int)pfVar14 + -0x10),SUB84(dVar20,0));
  }
  else {
    *(double *)((int)pfVar14 + -0x10) = dVar20;
    *(double *)((int)pfVar14 + -4) = dVar29;
    dVar20 = *(double *)((int)pfVar14 + -0x10);
  }
  if (in_FPSCR_PR == '\0') {
    dVar20 = (double)CONCAT44((float)((ulonglong)dVar20 >> 0x20) * fVar25,SUB84(dVar20,0));
  }
  else {
    dVar20 = dVar20 * dVar26;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)pfVar14 + -8) = (int)((ulonglong)dVar20 >> 0x20);
  }
  else {
    *(double *)((int)pfVar14 + -8) = dVar20;
  }
  puVar13 = (undefined4 *)param_5[1];
  dVar21 = dVar36;
  if (in_FPSCR_SZ == '\0') {
    dVar21 = (double)CONCAT44(fVar17,SUB84(dVar20,0));
  }
  *(undefined4 **)((int)pfVar14 + -0x30) = puVar13;
  uVar12 = *puVar13;
  uVar10 = *(undefined4 *)(param_4 * 4 + *(int *)((int)pfVar14 + -0x30));
  *(undefined4 *)(iVar8 + 0x18) = uVar12;
  if (in_FPSCR_SZ == '\0') {
    pdVar9 = (double *)(iVar8 + 0xc);
    *(undefined4 *)pdVar9 = uVar30;
    dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),*(undefined4 *)((int)pfVar14 + -0x10)
                             );
  }
  else {
    pdVar9 = (double *)(iVar8 + 8);
    *pdVar9 = param_3;
    dVar29 = *(double *)((int)pfVar14 + -0x10);
  }
  if (in_FPSCR_PR == '\0') {
    dVar21 = (double)CONCAT44((float)((ulonglong)dVar21 >> 0x20) - SUB84(dVar21,0),SUB84(dVar21,0));
  }
  else {
    dVar21 = dVar21 - dVar21;
  }
  dVar35 = in_xd10;
  if (in_FPSCR_SZ == '\0') {
    dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),fVar33);
    dVar35 = dVar29;
  }
  if (in_FPSCR_PR == '\0') {
    dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),SUB84(dVar21,0) - fVar18);
  }
  else {
    dVar21 = dVar21 - in_dr0;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)pdVar9 + -4) = (int)((ulonglong)dVar21 >> 0x20);
    pdVar9 = pdVar9 + -1;
    *(int *)pdVar9 = SUB84(dVar21,0);
  }
  else {
    pdVar9[-1] = dVar21;
    pdVar9 = pdVar9 + -2;
    *pdVar9 = dVar35;
  }
  *(undefined4 *)((int)pdVar9 + -4) = uVar4;
  dVar20 = in_xd12;
  if (in_FPSCR_SZ == '\0') {
    dVar20 = (double)CONCAT44(fVar31,SUB84(dVar21,0));
  }
  *(undefined4 *)((int)pdVar9 + 0x34) = uVar10;
  if (in_FPSCR_SZ == '\0') {
    pdVar9 = pdVar9 + 5;
    *(undefined4 *)pdVar9 = uVar16;
    dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),*(undefined4 *)((int)pfVar14 + -0x10)
                             );
  }
  else {
    pdVar9 = (double *)((int)pdVar9 + 0x24);
    *pdVar9 = in_xd8;
    dVar35 = *(double *)((int)pfVar14 + -0x10);
  }
  if (in_FPSCR_PR == '\0') {
    dVar20 = (double)CONCAT44((float)((ulonglong)dVar20 >> 0x20) - SUB84(dVar20,0),SUB84(dVar20,0));
  }
  else {
    dVar20 = dVar20 - dVar20;
  }
  in_xd2 = in_xd14;
  if (in_FPSCR_SZ == '\0') {
    dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),fVar32);
    in_xd2 = dVar35;
  }
  if (in_FPSCR_PR == '\0') {
    dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0) - fVar18);
  }
  else {
    dVar20 = dVar20 - in_dr0;
  }
  uVar22 = (undefined4)((ulonglong)dVar20 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)((int)pdVar9 + -4) = uVar22;
    pdVar9 = pdVar9 + -1;
    *(int *)pdVar9 = SUB84(dVar20,0);
  }
  else {
    pdVar9[-1] = dVar20;
    pdVar9 = pdVar9 + -2;
    *pdVar9 = in_xd2;
  }
  *(undefined4 *)((int)pdVar9 + -4) = uVar4;
  *(undefined4 *)((int)pdVar9 + 0x34) = uVar12;
  if (in_FPSCR_SZ == '\0') {
    pdVar9 = pdVar9 + 5;
    *(undefined4 *)pdVar9 = uVar30;
    dVar20 = (double)CONCAT44(uVar22,*(undefined4 *)((int)pfVar14 + -8));
  }
  else {
    pdVar9 = (double *)((int)pdVar9 + 0x24);
    *pdVar9 = param_3;
    in_xd2 = *(double *)((int)pfVar14 + -8);
  }
  if (in_FPSCR_PR == '\0') {
    dVar36 = (double)CONCAT44(fVar17 + SUB84(dVar20,0),fVar32);
  }
  else {
    dVar36 = dVar36 + dVar20;
  }
  if (in_FPSCR_SZ == '\0') {
    pdVar9 = (double *)((int)pdVar9 + -4);
    *(int *)pdVar9 = (int)((ulonglong)dVar36 >> 0x20);
    dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),*(undefined4 *)((int)pfVar14 + -4));
  }
  else {
    pdVar9 = pdVar9 + -1;
    *pdVar9 = dVar36;
    in_xd2 = *(double *)((int)pfVar14 + -4);
  }
  if (in_FPSCR_PR == '\0') {
    param_3 = (double)(ulonglong)(uint)(fVar33 + SUB84(dVar20,0));
  }
  else {
    param_3 = param_3 + dVar20;
  }
  if (in_FPSCR_SZ == '\0') {
    pdVar9 = (double *)((int)pdVar9 + -4);
    *(int *)pdVar9 = SUB84(param_3,0);
  }
  else {
    pdVar9 = pdVar9 + -1;
    *pdVar9 = in_xd10;
  }
  *(undefined4 *)((int)pdVar9 + -4) = uVar4;
  *(undefined4 *)((int)pdVar9 + 0x34) = uVar10;
  if (in_FPSCR_SZ == '\0') {
    pdVar9 = pdVar9 + 5;
    *(undefined4 *)pdVar9 = uVar16;
    dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),*(undefined4 *)((int)pfVar14 + -8));
  }
  else {
    pdVar9 = (double *)((int)pdVar9 + 0x24);
    *pdVar9 = in_xd8;
    in_xd2 = *(double *)((int)pfVar14 + -8);
  }
  if (in_FPSCR_PR == '\0') {
    in_dr12 = (double)CONCAT44((int)((ulonglong)in_dr12 >> 0x20),fVar31 + SUB84(dVar20,0));
  }
  else {
    in_dr12 = in_dr12 + dVar20;
  }
  if (in_FPSCR_SZ == '\0') {
    pdVar9 = (double *)((int)pdVar9 + -4);
    *(int *)pdVar9 = SUB84(in_dr12,0);
    dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),*(undefined4 *)((int)pfVar14 + -4));
  }
  else {
    pdVar9 = pdVar9 + -1;
    *pdVar9 = in_xd12;
    in_xd2 = *(double *)((int)pfVar14 + -4);
  }
  if (in_FPSCR_PR == '\0') {
    in_dr14 = (double)(ulonglong)(uint)(SUB84(dVar36,0) + SUB84(dVar20,0));
  }
  else {
    in_dr14 = dVar36 + dVar20;
  }
  if (in_FPSCR_SZ == '\0') {
    pdVar9 = (double *)((int)pdVar9 + -4);
    *(int *)pdVar9 = SUB84(in_dr14,0);
  }
  else {
    pdVar9 = pdVar9 + -1;
    *pdVar9 = in_xd14;
  }
  *(undefined4 *)((int)pdVar9 + -4) = uVar2;
  iVar8 = (int)pdVar9 + 0x1c;
  in_xd0 = dVar37;
LAB_8c64ff86:
  puVar13 = (undefined4 *)param_5[1];
  iVar5 = 1;
  *(undefined4 **)((int)pfVar14 + -0x30) = puVar13;
  *(undefined4 *)((int)pfVar14 + -0x30) = *(undefined4 *)((int)pfVar14 + -0x30);
  pdVar9 = (double *)*param_5;
  do {
    pdVar11 = (double *)((int)pdVar9 + 0xc);
    fVar33 = SUB84(in_dr14,0);
    puVar13 = puVar13 + 1;
    uVar16 = SUB84(dVar20,0);
    iVar6 = *param_5;
    *(int *)((int)pfVar14 + -0x34) = iVar6;
    if (in_FPSCR_SZ == '\0') {
      uVar16 = *(undefined4 *)pdVar3;
    }
    else {
      in_xd2 = *pdVar3;
    }
    iVar6 = iVar6 + iVar5 * 0xc;
    if (in_FPSCR_SZ == '\0') {
      dVar20 = (double)CONCAT44(*(undefined4 *)(iVar6 + 8),uVar16);
    }
    else {
      dVar20 = *(double *)(iVar6 + 8);
    }
    if ((float)((ulonglong)dVar20 >> 0x20) == SUB84(dVar20,0)) {
      if (in_FPSCR_SZ == '\0') {
        dVar20 = (double)(ulonglong)*(uint *)pdVar3;
      }
      else {
        in_xd2 = *pdVar3;
      }
      iVar6 = (iVar5 + -1) * 0xc + *(int *)((int)pfVar14 + -0x34);
      if (in_FPSCR_SZ == '\0') {
        dVar20 = (double)CONCAT44(*(undefined4 *)(iVar6 + 8),SUB84(dVar20,0));
      }
      else {
        dVar20 = *(double *)(iVar6 + 8);
      }
      if ((float)((ulonglong)dVar20 >> 0x20) != SUB84(dVar20,0)) goto LAB_8c64ffe4;
    }
    else {
LAB_8c64ffe4:
      dVar36 = dVar24;
      dVar29 = dVar24;
      if (in_FPSCR_SZ == '\0') {
        dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),0x3f800000);
        dVar36 = in_xd8;
        dVar29 = (double)CONCAT44(0x3f800000,SUB84(param_3,0));
      }
      pdVar7 = (double *)((iVar5 + -1) * 0xc + *param_5);
      *(double **)((int)pfVar14 + -0x34) = pdVar7;
      if (in_FPSCR_SZ == '\0') {
        dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),*(undefined4 *)(pdVar7 + 1));
      }
      else {
        in_xd2 = pdVar7[1];
      }
      if (in_FPSCR_PR == '\0') {
        dVar29 = (double)CONCAT44((float)((ulonglong)dVar29 >> 0x20) / SUB84(dVar20,0),
                                  SUB84(dVar29,0));
      }
      else {
        dVar29 = dVar29 / dVar20;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),
                                  *(undefined4 *)((int)pdVar9 + 0x14));
      }
      else {
        in_xd2 = *(double *)((int)pdVar9 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),SUB84(dVar28,0) / SUB84(dVar20,0)
                                 );
      }
      else {
        dVar28 = dVar28 / dVar20;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),*(undefined4 *)pdVar7);
        fVar33 = *(float *)((int)pdVar1 + 0xc);
      }
      else {
        in_xd2 = *pdVar7;
        in_xd14 = *(double *)((int)pdVar1 + 0xc);
      }
      if (in_FPSCR_PR == '\0') {
        dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0) * fVar23);
      }
      else {
        dVar20 = dVar20 * dVar24;
      }
      uVar16 = (undefined4)((ulonglong)dVar29 >> 0x20);
      param_3 = dVar29;
      dVar37 = in_xd14;
      if (in_FPSCR_SZ == '\0') {
        dVar29 = (double)CONCAT44(uVar16,SUB84(in_dr0,0));
        param_3 = (double)CONCAT44(uVar16,fVar33);
        dVar37 = in_xd10;
      }
      in_xd10 = dVar37;
      if (in_FPSCR_PR == '\0') {
        param_3 = (double)CONCAT44(uVar16,(float)((ulonglong)dVar29 >> 0x20) * SUB84(dVar20,0) +
                                          SUB84(param_3,0));
      }
      uVar16 = SUB84(dVar28,0);
      if (in_FPSCR_SZ == '\0') {
        dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),*(undefined4 *)pdVar11);
        dVar29 = (double)CONCAT44(uVar16,SUB84(dVar29,0));
      }
      else {
        in_xd2 = *pdVar11;
        dVar29 = dVar36;
      }
      if (in_FPSCR_PR == '\0') {
        fVar17 = SUB84(dVar20,0) * fVar23;
        dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),fVar17);
        fVar33 = (float)((ulonglong)dVar29 >> 0x20) * fVar17 + fVar33;
      }
      else {
        dVar20 = dVar20 * dVar24;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),*(undefined4 *)((int)pdVar7 + 4))
        ;
      }
      else {
        in_xd2 = *(double *)((int)pdVar7 + 4);
      }
      fVar17 = (float)((ulonglong)dVar28 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0) * fVar17);
      }
      else {
        dVar20 = dVar20 * dVar28;
      }
      uVar30 = (undefined4)((ulonglong)param_3 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        fVar31 = *(float *)(pdVar1 + 2);
        in_dr12 = (double)CONCAT44((int)((ulonglong)in_dr12 >> 0x20),fVar31);
        dVar37 = (double)CONCAT44(fVar31,fVar33);
        dVar29 = (double)CONCAT44(uVar30,SUB84(dVar29,0));
      }
      else {
        dVar37 = pdVar1[2];
        fVar31 = SUB84(in_dr12,0);
        dVar29 = param_3;
        in_xd12 = dVar37;
      }
      if (in_FPSCR_PR == '\0') {
        dVar37 = (double)CONCAT44((float)((ulonglong)dVar29 >> 0x20) * SUB84(dVar20,0) +
                                  (float)((ulonglong)dVar37 >> 0x20),SUB84(dVar37,0));
      }
      if (in_FPSCR_SZ == '\0') {
        dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),*(undefined4 *)(pdVar9 + 2));
        dVar29 = (double)CONCAT44(uVar16,SUB84(dVar29,0));
      }
      else {
        in_xd2 = pdVar9[2];
        dVar29 = dVar36;
      }
      if (in_FPSCR_PR == '\0') {
        dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0) * fVar17);
      }
      else {
        dVar20 = dVar20 * dVar28;
      }
      fVar33 = (float)((ulonglong)dVar37 >> 0x20);
      dVar21 = dVar37;
      if (in_FPSCR_SZ == '\0') {
        dVar29 = (double)CONCAT44((int)((ulonglong)dVar29 >> 0x20),fVar33);
        dVar21 = in_xd0;
      }
      if (in_FPSCR_PR == '\0') {
        in_dr12 = (double)CONCAT44((int)((ulonglong)in_dr12 >> 0x20),
                                   (float)((ulonglong)dVar29 >> 0x20) * SUB84(dVar20,0) + fVar31);
      }
      fVar17 = SUB84(param_3,0);
      dVar35 = in_xd10;
      if (in_FPSCR_SZ == '\0') {
        dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),fVar17);
        dVar35 = in_xd2;
      }
      fVar32 = SUB84(dVar37,0);
      fVar31 = SUB84(in_dr12,0);
      if (in_FPSCR_PR == '\0') {
        fVar18 = SUB84(dVar20,0) - fVar32;
        fVar15 = SUB84(dVar29,0) - fVar31;
      }
      else {
        fVar18 = SUB84(dVar20 - dVar37,0);
        fVar15 = SUB84(dVar29 - in_dr12,0);
      }
      if (in_FPSCR_SZ == '\0') {
        *(float *)((int)pfVar14 + -0x2c) = fVar18;
        dVar29 = (double)CONCAT44(fVar15,fVar18);
        dVar20 = (double)CONCAT44(fVar18,fVar15);
      }
      else {
        *(double *)((int)pfVar14 + -0x2c) = dVar35;
        fVar15 = SUB84(dVar35,0);
        dVar29 = dVar21;
        dVar20 = dVar35;
      }
      fVar18 = (float)((ulonglong)dVar20 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar29 = (double)CONCAT44(fVar18 * SUB84(dVar29,0) +
                                  (float)((ulonglong)dVar29 >> 0x20) * fVar15,SUB84(dVar29,0));
      }
      else {
        dVar29 = dVar29 * dVar20;
      }
      dVar34 = dVar24;
      if (in_FPSCR_SZ == '\0') {
        dVar29 = (double)CONCAT44((int)((ulonglong)dVar29 >> 0x20),0x3f800000);
        dVar34 = dVar35;
      }
      if (in_FPSCR_PR == '\0') {
        fVar19 = SUB84(dVar29,0) / SQRT((float)((ulonglong)dVar29 >> 0x20));
      }
      else {
        fVar19 = SUB84(SQRT(dVar29) / SQRT(dVar29),0);
      }
      if (in_FPSCR_SZ == '\0') {
        *(float *)((int)pfVar14 + -0x24) = fVar19;
        dVar29 = (double)CONCAT44(*(undefined4 *)((int)pfVar14 + -0x2c),fVar19);
      }
      else {
        *(double *)((int)pfVar14 + -0x24) = dVar34;
        dVar29 = *(double *)((int)pfVar14 + -0x2c);
      }
      fVar19 = SUB84(dVar29,0);
      if (in_FPSCR_PR == '\0') {
        in_dr0 = (double)CONCAT44(fVar18,fVar15 * fVar19 * fVar27);
        dVar20 = (double)CONCAT44(-((float)((ulonglong)dVar29 >> 0x20) * fVar19 * fVar27),fVar19);
      }
      else {
        in_dr0 = dVar20 * dVar29 * dVar26;
        dVar20 = -(dVar29 * dVar29 * dVar26);
      }
      uVar10 = (undefined4)((ulonglong)dVar20 >> 0x20);
      fVar18 = SUB84(in_dr0,0);
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)((int)pfVar14 + -0x28) = uVar10;
        dVar20 = (double)CONCAT44(uVar10,fVar18);
      }
      else {
        *(double *)((int)pfVar14 + -0x28) = dVar20;
        dVar34 = dVar21;
      }
      if (in_FPSCR_PR == '\0') {
        fVar15 = SUB84(dVar20,0) * fVar25;
      }
      else {
        fVar15 = SUB84(dVar20 * dVar26,0);
      }
      if (in_FPSCR_SZ == '\0') {
        *(float *)((int)pfVar14 + -0x18) = fVar15;
        dVar20 = (double)CONCAT44(*(undefined4 *)((int)pfVar14 + -0x28),fVar15);
      }
      else {
        *(double *)((int)pfVar14 + -0x18) = dVar34;
        dVar20 = *(double *)((int)pfVar14 + -0x28);
      }
      if (in_FPSCR_PR == '\0') {
        dVar20 = (double)CONCAT44((float)((ulonglong)dVar20 >> 0x20) * fVar25,SUB84(dVar20,0));
      }
      else {
        dVar20 = dVar20 * dVar26;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pfVar14 + -0x1c) = (int)((ulonglong)dVar20 >> 0x20);
      }
      else {
        *(double *)((int)pfVar14 + -0x1c) = dVar20;
      }
      dVar29 = dVar37;
      if (in_FPSCR_SZ == '\0') {
        dVar29 = (double)CONCAT44(fVar33,SUB84(dVar20,0));
      }
      *(undefined4 *)((int)pfVar14 + -0x34) = **(undefined4 **)((int)pfVar14 + -0x30);
      *(undefined4 *)((int)pfVar14 + -0x20) = *puVar13;
      *(undefined4 *)(iVar8 + 0x18) = *(undefined4 *)((int)pfVar14 + -0x34);
      if (in_FPSCR_SZ == '\0') {
        pdVar9 = (double *)(iVar8 + 0xc);
        *(undefined4 *)pdVar9 = uVar30;
        dVar29 = (double)CONCAT44((int)((ulonglong)dVar29 >> 0x20),
                                  *(undefined4 *)((int)pfVar14 + -0x28));
      }
      else {
        pdVar9 = (double *)(iVar8 + 8);
        *pdVar9 = param_3;
        dVar34 = *(double *)((int)pfVar14 + -0x28);
      }
      if (in_FPSCR_PR == '\0') {
        dVar29 = (double)CONCAT44((float)((ulonglong)dVar29 >> 0x20) - SUB84(dVar29,0),
                                  SUB84(dVar29,0));
      }
      else {
        dVar29 = dVar29 - dVar29;
      }
      dVar35 = in_xd10;
      if (in_FPSCR_SZ == '\0') {
        dVar29 = (double)CONCAT44((int)((ulonglong)dVar29 >> 0x20),fVar17);
        dVar35 = dVar34;
      }
      if (in_FPSCR_PR == '\0') {
        dVar29 = (double)CONCAT44((int)((ulonglong)dVar29 >> 0x20),SUB84(dVar29,0) - fVar18);
      }
      else {
        dVar29 = dVar29 - in_dr0;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar9 + -4) = (int)((ulonglong)dVar29 >> 0x20);
        pdVar9 = pdVar9 + -1;
        *(int *)pdVar9 = SUB84(dVar29,0);
      }
      else {
        pdVar9[-1] = dVar29;
        pdVar9 = pdVar9 + -2;
        *pdVar9 = dVar35;
      }
      *(undefined4 *)((int)pdVar9 + -4) = uVar4;
      dVar20 = in_xd12;
      if (in_FPSCR_SZ == '\0') {
        dVar20 = (double)CONCAT44(fVar31,SUB84(dVar29,0));
      }
      *(undefined4 *)((int)pdVar9 + 0x34) = *(undefined4 *)((int)pfVar14 + -0x20);
      if (in_FPSCR_SZ == '\0') {
        pdVar9 = pdVar9 + 5;
        *(undefined4 *)pdVar9 = uVar16;
        dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),
                                  *(undefined4 *)((int)pfVar14 + -0x28));
      }
      else {
        pdVar9 = (double *)((int)pdVar9 + 0x24);
        *pdVar9 = dVar36;
        dVar35 = *(double *)((int)pfVar14 + -0x28);
      }
      if (in_FPSCR_PR == '\0') {
        dVar20 = (double)CONCAT44((float)((ulonglong)dVar20 >> 0x20) - SUB84(dVar20,0),
                                  SUB84(dVar20,0));
      }
      else {
        dVar20 = dVar20 - dVar20;
      }
      in_xd2 = in_xd14;
      if (in_FPSCR_SZ == '\0') {
        dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),fVar32);
        in_xd2 = dVar35;
      }
      if (in_FPSCR_PR == '\0') {
        dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0) - fVar18);
      }
      else {
        dVar20 = dVar20 - in_dr0;
      }
      uVar10 = (undefined4)((ulonglong)dVar20 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)((int)pdVar9 + -4) = uVar10;
        pdVar9 = pdVar9 + -1;
        *(int *)pdVar9 = SUB84(dVar20,0);
      }
      else {
        pdVar9[-1] = dVar20;
        pdVar9 = pdVar9 + -2;
        *pdVar9 = in_xd2;
      }
      *(undefined4 *)((int)pdVar9 + -4) = uVar4;
      *(undefined4 *)((int)pdVar9 + 0x34) = *(undefined4 *)((int)pfVar14 + -0x34);
      if (in_FPSCR_SZ == '\0') {
        pdVar9 = pdVar9 + 5;
        *(undefined4 *)pdVar9 = uVar30;
        dVar20 = (double)CONCAT44(uVar10,*(undefined4 *)((int)pfVar14 + -0x1c));
      }
      else {
        pdVar9 = (double *)((int)pdVar9 + 0x24);
        *pdVar9 = param_3;
        in_xd2 = *(double *)((int)pfVar14 + -0x1c);
      }
      if (in_FPSCR_PR == '\0') {
        dVar37 = (double)CONCAT44(fVar33 + SUB84(dVar20,0),fVar32);
      }
      else {
        dVar37 = dVar37 + dVar20;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar9 = (double *)((int)pdVar9 + -4);
        *(int *)pdVar9 = (int)((ulonglong)dVar37 >> 0x20);
        dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),
                                  *(undefined4 *)((int)pfVar14 + -0x18));
      }
      else {
        pdVar9 = pdVar9 + -1;
        *pdVar9 = dVar37;
        in_xd2 = *(double *)((int)pfVar14 + -0x18);
      }
      if (in_FPSCR_PR == '\0') {
        param_3 = (double)(ulonglong)(uint)(fVar17 + SUB84(dVar20,0));
      }
      else {
        param_3 = param_3 + dVar20;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar9 = (double *)((int)pdVar9 + -4);
        *(int *)pdVar9 = SUB84(param_3,0);
      }
      else {
        pdVar9 = pdVar9 + -1;
        *pdVar9 = in_xd10;
      }
      *(undefined4 *)((int)pdVar9 + -4) = uVar4;
      *(undefined4 *)((int)pdVar9 + 0x34) = *(undefined4 *)((int)pfVar14 + -0x20);
      if (in_FPSCR_SZ == '\0') {
        pdVar9 = pdVar9 + 5;
        *(undefined4 *)pdVar9 = uVar16;
        dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),
                                  *(undefined4 *)((int)pfVar14 + -0x1c));
      }
      else {
        pdVar9 = (double *)((int)pdVar9 + 0x24);
        *pdVar9 = dVar36;
        in_xd2 = *(double *)((int)pfVar14 + -0x1c);
      }
      if (in_FPSCR_PR == '\0') {
        in_dr12 = (double)CONCAT44((int)((ulonglong)in_dr12 >> 0x20),fVar31 + SUB84(dVar20,0));
      }
      else {
        in_dr12 = in_dr12 + dVar20;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar9 = (double *)((int)pdVar9 + -4);
        *(int *)pdVar9 = SUB84(in_dr12,0);
        dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),
                                  *(undefined4 *)((int)pfVar14 + -0x18));
      }
      else {
        pdVar9 = pdVar9 + -1;
        *pdVar9 = in_xd12;
        in_xd2 = *(double *)((int)pfVar14 + -0x18);
      }
      if (in_FPSCR_PR == '\0') {
        in_dr14 = (double)(ulonglong)(uint)(SUB84(dVar37,0) + SUB84(dVar20,0));
      }
      else {
        in_dr14 = dVar37 + dVar20;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar9 = (double *)((int)pdVar9 + -4);
        *(int *)pdVar9 = SUB84(in_dr14,0);
      }
      else {
        pdVar9 = pdVar9 + -1;
        *pdVar9 = in_xd14;
      }
      *(undefined4 *)((int)pdVar9 + -4) = uVar2;
      iVar8 = (int)pdVar9 + 0x1c;
      in_xd0 = dVar21;
      in_xd8 = dVar36;
    }
    iVar5 = iVar5 + 1;
    *(int *)((int)pfVar14 + -0x30) = *(int *)((int)pfVar14 + -0x30) + 4;
    pdVar9 = pdVar11;
    if (param_4 < iVar5) {
      *param_6 = iVar8;
      return;
    }
  } while( true );
}

