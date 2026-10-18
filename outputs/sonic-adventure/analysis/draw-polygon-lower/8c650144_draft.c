/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c65038e) */
/* WARNING: Removing unreachable block (ram,0x8c65029c) */
/* WARNING: Removing unreachable block (ram,0x8c650274) */
/* WARNING: Removing unreachable block (ram,0x8c650238) */
/* WARNING: Removing unreachable block (ram,0x8c650194) */
/* WARNING: Removing unreachable block (ram,0x8c6501ac) */
/* WARNING: Removing unreachable block (ram,0x8c650266) */
/* WARNING: Removing unreachable block (ram,0x8c65028c) */
/* WARNING: Removing unreachable block (ram,0x8c650374) */
/* WARNING: Removing unreachable block (ram,0x8c65041e) */
/* WARNING: Removing unreachable block (ram,0x8c65043e) */
/* WARNING: Removing unreachable block (ram,0x8c65044e) */
/* WARNING: Removing unreachable block (ram,0x8c650464) */
/* WARNING: Removing unreachable block (ram,0x8c650474) */

void FUN_8c650144(undefined4 param_1,undefined4 param_2,int param_3,int *param_4,undefined4 *param_5
                 )

{
  double *pdVar1;
  undefined4 uVar2;
  double *pdVar3;
  undefined4 uVar4;
  int iVar5;
  int iVar6;
  double *pdVar7;
  short *psVar8;
  double *pdVar9;
  double *pdVar10;
  undefined4 uVar11;
  int iVar12;
  undefined4 *puVar13;
  undefined4 uVar14;
  double *pdVar15;
  undefined8 *puVar16;
  float in_fr1;
  float fVar17;
  float fVar18;
  undefined4 uVar19;
  float fVar20;
  float fVar21;
  double in_dr2;
  double dVar22;
  undefined4 uVar24;
  double dVar23;
  float fVar25;
  double dVar26;
  float fVar27;
  double dVar28;
  float fVar29;
  float fVar30;
  double dVar31;
  float fVar32;
  double dVar33;
  undefined8 in_dr12;
  double dVar34;
  float fVar36;
  double dVar35;
  double in_dr14;
  double dVar37;
  float fVar39;
  double dVar38;
  undefined4 uVar40;
  undefined8 in_xd0;
  double in_xd2;
  double in_xd10;
  double in_xd12;
  double in_xd14;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_3c [16];
  undefined8 uStack_2c;
  undefined4 uStack_24;
  undefined4 local_20;
  
  uVar4 = DAT_8c6504ec;
  pdVar3 = DAT_8c6504e4;
  uVar2 = DAT_8c6504e0;
  pdVar1 = DAT_8c6504d4;
  if (in_FPSCR_SZ == '\0') {
    _uStack_24 = CONCAT44(SUB84(in_dr14,0),uStack_24);
    puVar16 = &uStack_2c;
    uStack_2c = CONCAT44((int)in_dr12,(int)((ulonglong)in_dr12 >> 0x20));
  }
  else {
    puVar16 = (undefined8 *)auStack_3c;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar26 = (double)CONCAT44(*(undefined4 *)DAT_8c6504d4,DAT_8c6504d0);
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)DAT_8c6504d8);
  }
  else {
    dVar26 = *DAT_8c6504d4;
    in_xd2 = *DAT_8c6504d8;
  }
  param_3 = param_3 + -1;
  iVar5 = *param_4;
  uVar19 = (undefined4)((ulonglong)dVar26 >> 0x20);
  dVar28 = dVar26;
  if (in_FPSCR_SZ == '\0') {
    dVar28 = (double)CONCAT44(uVar19,DAT_8c6504e8);
  }
  if (in_FPSCR_PR == '\0') {
    dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) * SUB84(in_dr2,0),SUB84(dVar28,0));
  }
  else {
    dVar28 = dVar28 * in_dr2;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar33 = (double)CONCAT44(*(undefined4 *)DAT_8c6504e4,param_2);
    dVar22 = (double)CONCAT44(*(undefined4 *)DAT_8c6504dc,SUB84(in_dr2,0));
  }
  else {
    dVar33 = *DAT_8c6504e4;
    dVar22 = *DAT_8c6504dc;
  }
  if (in_FPSCR_PR == '\0') {
    dVar31 = (double)CONCAT44(0x40000000,param_1);
  }
  else {
    dVar31 = (double)CONCAT44(0x3f800000,param_1) + (double)CONCAT44(0x3f800000,param_1);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar31 = (double)CONCAT44((int)((ulonglong)dVar31 >> 0x20),uVar19);
  }
  fVar25 = (float)((ulonglong)dVar22 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar31 = (double)CONCAT44((int)((ulonglong)dVar31 >> 0x20),SUB84(dVar31,0) * fVar25);
  }
  else {
    dVar31 = dVar31 * dVar22;
  }
  pdVar9 = (double *)*param_5;
  *(int *)((int)puVar16 + -0x5c) = iVar5;
  if (in_FPSCR_SZ == '\0') {
    dVar22 = (double)CONCAT44(fVar25,*(undefined4 *)(iVar5 + 8));
  }
  else {
    in_xd2 = *(double *)(iVar5 + 8);
  }
  fVar20 = (float)((ulonglong)dVar33 >> 0x20);
  fVar25 = SUB84(dVar26,0);
  dVar26 = (double)CONCAT44(0x3f800000,fVar25);
  fVar27 = SUB84(dVar28,0);
  fVar29 = (float)((ulonglong)dVar28 >> 0x20);
  fVar30 = SUB84(dVar31,0);
  fVar32 = (float)((ulonglong)dVar31 >> 0x20);
  if (SUB84(dVar22,0) == fVar20) {
    if (in_FPSCR_SZ == '\0') {
      dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                *(undefined4 *)(param_3 * 0xc + *(int *)((int)puVar16 + -0x5c) + 8))
      ;
    }
    else {
      in_xd2 = *(double *)(param_3 * 0xc + *(int *)((int)puVar16 + -0x5c) + 8);
    }
    if (SUB84(dVar22,0) == fVar20) goto LAB_8c650342;
  }
  pdVar10 = (double *)*param_4;
  if (in_FPSCR_SZ == '\0') {
    dVar33 = (double)CONCAT44(fVar20,0x3f800000);
    dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)(pdVar10 + 1));
  }
  else {
    in_xd2 = pdVar10[1];
    in_xd10 = dVar26;
  }
  if (in_FPSCR_PR == '\0') {
    fVar20 = SUB84(dVar33,0) / SUB84(dVar22,0);
  }
  else {
    fVar20 = SUB84(dVar33 / dVar22,0);
  }
  pdVar15 = (double *)(param_3 * 0xc + (int)pdVar10);
  if (in_FPSCR_SZ == '\0') {
    dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)(pdVar15 + 1));
    dVar37 = (double)CONCAT44(*(undefined4 *)((int)DAT_8c6504d4 + 0xc),SUB84(in_dr14,0));
    dVar33 = (double)CONCAT44(0x3f800000,fVar20);
  }
  else {
    in_xd2 = pdVar15[1];
    dVar37 = *(double *)((int)DAT_8c6504d4 + 0xc);
    dVar33 = dVar26;
  }
  if (in_FPSCR_PR == '\0') {
    dVar33 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) / SUB84(dVar22,0),SUB84(dVar33,0));
  }
  else {
    dVar33 = dVar33 / dVar22;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)pdVar10);
    uVar19 = (undefined4)((ulonglong)dVar37 >> 0x20);
    dVar37 = (double)CONCAT44(uVar19,uVar19);
  }
  else {
    in_xd2 = *pdVar10;
    in_xd14 = dVar37;
  }
  if (in_FPSCR_PR == '\0') {
    dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),SUB84(dVar22,0) * fVar29);
  }
  else {
    dVar22 = dVar22 * dVar28;
  }
  uVar19 = SUB84(dVar33,0);
  dVar38 = in_xd10;
  if (in_FPSCR_SZ == '\0') {
    dVar38 = (double)CONCAT44(uVar19,in_fr1);
  }
  if (in_FPSCR_PR == '\0') {
    dVar37 = (double)CONCAT44((int)((ulonglong)dVar37 >> 0x20),
                              (float)((ulonglong)dVar38 >> 0x20) * SUB84(dVar22,0) + SUB84(dVar37,0)
                             );
  }
  if (in_FPSCR_SZ == '\0') {
    dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)pdVar15);
  }
  else {
    in_xd2 = *pdVar15;
  }
  if (in_FPSCR_PR == '\0') {
    dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),SUB84(dVar22,0) * fVar29);
  }
  else {
    dVar22 = dVar22 * dVar28;
  }
  uVar40 = (undefined4)((ulonglong)dVar33 >> 0x20);
  dVar34 = dVar33;
  if (in_FPSCR_SZ == '\0') {
    dVar34 = (double)CONCAT44(uVar40,SUB84(dVar38,0));
  }
  if (in_FPSCR_PR == '\0') {
    dVar37 = (double)CONCAT44((float)((ulonglong)dVar34 >> 0x20) * SUB84(dVar22,0) +
                              (float)((ulonglong)dVar37 >> 0x20),SUB84(dVar37,0));
  }
  if (in_FPSCR_SZ == '\0') {
    dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)((int)pdVar10 + 4));
    dVar38 = (double)CONCAT44(uVar19,SUB84(dVar34,0));
  }
  else {
    in_xd2 = *(double *)((int)pdVar10 + 4);
    dVar38 = in_xd10;
  }
  if (in_FPSCR_PR == '\0') {
    dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),SUB84(dVar22,0) * fVar30);
  }
  else {
    dVar22 = dVar22 * dVar31;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar34 = (double)CONCAT44(*(undefined4 *)(DAT_8c6504d4 + 2),*(undefined4 *)(DAT_8c6504d4 + 2));
  }
  else {
    dVar34 = DAT_8c6504d4[2];
    in_xd12 = dVar34;
  }
  if (in_FPSCR_PR == '\0') {
    dVar34 = (double)CONCAT44((int)((ulonglong)dVar34 >> 0x20),
                              (float)((ulonglong)dVar38 >> 0x20) * SUB84(dVar22,0) + SUB84(dVar34,0)
                             );
  }
  if (in_FPSCR_SZ == '\0') {
    dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)((int)pdVar15 + 4));
    dVar38 = (double)CONCAT44(uVar40,SUB84(dVar38,0));
  }
  else {
    in_xd2 = *(double *)((int)pdVar15 + 4);
    dVar38 = dVar33;
  }
  if (in_FPSCR_PR == '\0') {
    fVar20 = SUB84(dVar22,0) * fVar30;
  }
  else {
    fVar20 = SUB84(dVar22 * dVar31,0);
  }
  dVar22 = in_xd12;
  if (in_FPSCR_SZ == '\0') {
    dVar22 = (double)CONCAT44(SUB84(dVar34,0),fVar20);
  }
  if (in_FPSCR_PR == '\0') {
    dVar34 = (double)CONCAT44((float)((ulonglong)dVar38 >> 0x20) * SUB84(dVar22,0) +
                              (float)((ulonglong)dVar34 >> 0x20),SUB84(dVar34,0));
  }
  fVar20 = SUB84(dVar37,0);
  dVar35 = in_xd14;
  if (in_FPSCR_SZ == '\0') {
    dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),fVar20);
    dVar35 = in_xd2;
  }
  fVar39 = (float)((ulonglong)dVar37 >> 0x20);
  fVar36 = (float)((ulonglong)dVar34 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) - fVar36,SUB84(dVar22,0) - fVar39);
  }
  else {
    dVar22 = (dVar22 - dVar37) - dVar34;
  }
  fVar21 = SUB84(dVar22,0);
  fVar17 = (float)((ulonglong)dVar22 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    *(float *)((int)puVar16 + -0x28) = fVar21;
    *(float *)((int)puVar16 + -0x2c) = fVar17;
    dVar38 = (double)CONCAT44(fVar21,SUB84(dVar38,0));
  }
  else {
    *(double *)((int)puVar16 + -0x28) = dVar35;
    *(double *)((int)puVar16 + -0x2c) = dVar22;
    dVar38 = dVar35;
  }
  fVar18 = (float)((ulonglong)dVar38 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar22 = (double)CONCAT44(fVar18 * fVar21 + fVar17 * fVar17,fVar21);
  }
  else {
    dVar22 = dVar22 * dVar22;
  }
  in_xd2 = dVar26;
  if (in_FPSCR_SZ == '\0') {
    dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),0x3f800000);
    in_xd2 = dVar35;
  }
  if (in_FPSCR_PR == '\0') {
    fVar21 = SUB84(dVar22,0) / SQRT((float)((ulonglong)dVar22 >> 0x20));
  }
  else {
    fVar21 = SUB84(SQRT(dVar22) / SQRT(dVar22),0);
  }
  if (in_FPSCR_SZ == '\0') {
    *(float *)((int)puVar16 + -0x1c) = fVar21;
    dVar22 = (double)CONCAT44(*(undefined4 *)((int)puVar16 + -0x2c),fVar21);
  }
  else {
    *(double *)((int)puVar16 + -0x1c) = in_xd2;
    dVar22 = *(double *)((int)puVar16 + -0x2c);
  }
  if (in_FPSCR_PR == '\0') {
    dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) * SUB84(dVar22,0) * fVar27,
                              SUB84(dVar22,0));
  }
  else {
    dVar22 = dVar22 * dVar22 * dVar28;
  }
  fVar21 = (float)((ulonglong)dVar22 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    *(float *)((int)puVar16 + -0x20) = fVar21;
    dVar38 = (double)CONCAT44(fVar18,*(undefined4 *)((int)puVar16 + -0x28));
  }
  else {
    *(double *)((int)puVar16 + -0x20) = dVar22;
    in_xd0 = *(undefined8 *)((int)puVar16 + -0x28);
  }
  if (in_FPSCR_PR == '\0') {
    fVar17 = SUB84(dVar38,0) * SUB84(dVar22,0) * fVar27;
    dVar22 = (double)CONCAT44(fVar21 * fVar32,SUB84(dVar22,0));
  }
  else {
    fVar17 = SUB84(dVar38 * dVar22 * dVar28,0);
    dVar22 = dVar22 * dVar31;
  }
  in_fr1 = -fVar17;
  uVar11 = (undefined4)((ulonglong)dVar22 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    *(float *)((int)puVar16 + -0x24) = in_fr1;
    *(undefined4 *)((int)puVar16 + -8) = uVar11;
    dVar22 = (double)CONCAT44(uVar11,*(undefined4 *)((int)puVar16 + -0x24));
  }
  else {
    *(undefined8 *)((int)puVar16 + -0x24) = in_xd0;
    *(double *)((int)puVar16 + -8) = dVar22;
    in_xd2 = *(double *)((int)puVar16 + -0x24);
  }
  if (in_FPSCR_PR == '\0') {
    dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),SUB84(dVar22,0) * fVar32);
  }
  else {
    dVar22 = dVar22 * dVar31;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)puVar16 + -4) = SUB84(dVar22,0);
  }
  else {
    *(double *)((int)puVar16 + -4) = in_xd2;
  }
  puVar13 = (undefined4 *)param_4[1];
  *(undefined4 **)((int)puVar16 + -0x5c) = puVar13;
  uVar14 = *puVar13;
  *(int *)((int)puVar16 + -100) = param_3 * 4;
  psVar8 = (short *)param_4[2];
  uVar11 = *(undefined4 *)(param_3 * 4 + *(int *)((int)puVar16 + -0x5c));
  *(short **)((int)puVar16 + -0x60) = psVar8;
  uVar24 = (undefined4)((ulonglong)dVar22 >> 0x20);
  fVar21 = (float)(int)*psVar8;
  if (in_FPSCR_PR == '\0') {
    dVar22 = (double)CONCAT44(uVar24,fVar21 * fVar25);
  }
  else {
    dVar22 = (double)CONCAT44(uVar24,fVar21) * dVar26;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)puVar16 + -0x10) = SUB84(dVar22,0);
  }
  else {
    *(double *)((int)puVar16 + -0x10) = in_xd2;
  }
  uVar24 = (undefined4)((ulonglong)dVar22 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar22 = (double)CONCAT44(uVar24,(float)(int)*(short *)(*(int *)((int)puVar16 + -0x60) + 2) *
                                     fVar25);
  }
  else {
    dVar22 = (double)CONCAT44(uVar24,(float)(int)*(short *)(*(int *)((int)puVar16 + -0x60) + 2)) *
             dVar26;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)puVar16 + -0xc) = SUB84(dVar22,0);
  }
  else {
    *(double *)((int)puVar16 + -0xc) = in_xd2;
  }
  *(short **)((int)puVar16 + -0x5c) =
       (short *)(*(int *)((int)puVar16 + -0x60) + *(int *)((int)puVar16 + -100));
  uVar24 = (undefined4)((ulonglong)dVar22 >> 0x20);
  fVar21 = (float)(int)*(short *)(*(int *)((int)puVar16 + -0x60) + *(int *)((int)puVar16 + -100));
  if (in_FPSCR_PR == '\0') {
    dVar22 = (double)CONCAT44(uVar24,fVar21 * fVar25);
  }
  else {
    dVar22 = (double)CONCAT44(uVar24,fVar21) * dVar26;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)puVar16 + -0x14) = SUB84(dVar22,0);
  }
  else {
    *(double *)((int)puVar16 + -0x14) = in_xd2;
  }
  if (in_FPSCR_PR == '\0') {
    fVar21 = (float)(int)*(short *)(*(int *)((int)puVar16 + -0x5c) + 2) * fVar25;
  }
  else {
    fVar21 = SUB84((double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                    (float)(int)*(short *)(*(int *)((int)puVar16 + -0x5c) + 2)) *
                   dVar26,0);
  }
  if (in_FPSCR_SZ == '\0') {
    *(float *)((int)puVar16 + -0x18) = fVar21;
  }
  else {
    *(double *)((int)puVar16 + -0x18) = in_xd2;
  }
  *(undefined4 *)(pdVar9 + 3) = uVar14;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)((int)pdVar9 + 0x14) = *(undefined4 *)((int)puVar16 + -0xc);
    *(undefined4 *)(pdVar9 + 2) = *(undefined4 *)((int)puVar16 + -0x10);
    pdVar9 = (double *)((int)pdVar9 + 0xc);
    *(undefined4 *)pdVar9 = uVar19;
    dVar22 = (double)CONCAT44(SUB84(dVar34,0),*(undefined4 *)((int)puVar16 + -0x24));
  }
  else {
    pdVar9[2] = *(double *)((int)puVar16 + -0xc);
    pdVar9[1] = *(double *)((int)puVar16 + -0x10);
    *pdVar9 = in_xd10;
    in_xd2 = *(double *)((int)puVar16 + -0x24);
    dVar22 = in_xd12;
  }
  if (in_FPSCR_PR == '\0') {
    dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) - SUB84(dVar22,0),SUB84(dVar22,0));
  }
  else {
    dVar22 = dVar22 - dVar22;
  }
  if (in_FPSCR_SZ == '\0') {
    pdVar9 = (double *)((int)pdVar9 + -4);
    *(int *)pdVar9 = (int)((ulonglong)dVar22 >> 0x20);
    dVar22 = (double)CONCAT44(fVar20,*(undefined4 *)((int)puVar16 + -0x20));
  }
  else {
    pdVar9 = pdVar9 + -1;
    *pdVar9 = dVar22;
    in_xd2 = *(double *)((int)puVar16 + -0x20);
    dVar22 = in_xd14;
  }
  if (in_FPSCR_PR == '\0') {
    dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) - SUB84(dVar22,0),SUB84(dVar22,0));
  }
  else {
    dVar22 = dVar22 - dVar22;
  }
  if (in_FPSCR_SZ == '\0') {
    pdVar9 = (double *)((int)pdVar9 + -4);
    *(int *)pdVar9 = (int)((ulonglong)dVar22 >> 0x20);
  }
  else {
    pdVar9 = pdVar9 + -1;
    *pdVar9 = dVar22;
  }
  *(undefined4 *)((int)pdVar9 + -4) = uVar4;
  dVar38 = dVar34;
  if (in_FPSCR_SZ == '\0') {
    dVar38 = (double)CONCAT44(fVar36,SUB84(dVar22,0));
  }
  *(undefined4 *)((int)pdVar9 + 0x34) = uVar11;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(pdVar9 + 6) = *(undefined4 *)((int)puVar16 + -0x18);
    *(undefined4 *)((int)pdVar9 + 0x2c) = *(undefined4 *)((int)puVar16 + -0x14);
    pdVar9 = pdVar9 + 5;
    *(undefined4 *)pdVar9 = uVar40;
    dVar38 = (double)CONCAT44((int)((ulonglong)dVar38 >> 0x20),*(undefined4 *)((int)puVar16 + -0x24)
                             );
  }
  else {
    *(undefined8 *)((int)pdVar9 + 0x2c) = *(undefined8 *)((int)puVar16 + -0x18);
    *(undefined8 *)((int)pdVar9 + 0x24) = *(undefined8 *)((int)puVar16 + -0x14);
    pdVar9 = (double *)((int)pdVar9 + 0x1c);
    *pdVar9 = dVar33;
    in_xd2 = *(double *)((int)puVar16 + -0x24);
  }
  if (in_FPSCR_PR == '\0') {
    dVar38 = (double)CONCAT44((float)((ulonglong)dVar38 >> 0x20) - SUB84(dVar38,0),SUB84(dVar38,0));
  }
  else {
    dVar38 = dVar38 - dVar38;
  }
  if (in_FPSCR_SZ == '\0') {
    pdVar9 = (double *)((int)pdVar9 + -4);
    *(int *)pdVar9 = (int)((ulonglong)dVar38 >> 0x20);
    dVar22 = (double)CONCAT44(fVar39,*(undefined4 *)((int)puVar16 + -0x20));
  }
  else {
    pdVar9 = pdVar9 + -1;
    *pdVar9 = dVar38;
    in_xd2 = *(double *)((int)puVar16 + -0x20);
    dVar22 = dVar37;
  }
  if (in_FPSCR_PR == '\0') {
    dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) - SUB84(dVar22,0),SUB84(dVar22,0));
  }
  else {
    dVar22 = dVar22 - dVar22;
  }
  uVar24 = (undefined4)((ulonglong)dVar22 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    pdVar9 = (double *)((int)pdVar9 + -4);
    *(undefined4 *)pdVar9 = uVar24;
  }
  else {
    pdVar9 = pdVar9 + -1;
    *pdVar9 = dVar22;
  }
  *(undefined4 *)((int)pdVar9 + -4) = uVar4;
  *(undefined4 *)((int)pdVar9 + 0x34) = uVar14;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(pdVar9 + 6) = *(undefined4 *)((int)puVar16 + -0xc);
    *(undefined4 *)((int)pdVar9 + 0x2c) = *(undefined4 *)((int)puVar16 + -0x10);
    pdVar9 = pdVar9 + 5;
    *(undefined4 *)pdVar9 = uVar19;
    dVar22 = (double)CONCAT44(uVar24,*(undefined4 *)((int)puVar16 + -4));
  }
  else {
    *(undefined8 *)((int)pdVar9 + 0x2c) = *(undefined8 *)((int)puVar16 + -0xc);
    *(undefined8 *)((int)pdVar9 + 0x24) = *(undefined8 *)((int)puVar16 + -0x10);
    pdVar9 = (double *)((int)pdVar9 + 0x1c);
    *pdVar9 = in_xd10;
    in_xd2 = *(double *)((int)puVar16 + -4);
  }
  if (in_FPSCR_PR == '\0') {
    dVar34 = (double)CONCAT44(fVar36,SUB84(dVar34,0) + SUB84(dVar22,0));
  }
  else {
    dVar34 = dVar34 + dVar22;
  }
  if (in_FPSCR_SZ == '\0') {
    pdVar9 = (double *)((int)pdVar9 + -4);
    *(int *)pdVar9 = SUB84(dVar34,0);
    dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)((int)puVar16 + -8));
  }
  else {
    pdVar9 = pdVar9 + -1;
    *pdVar9 = in_xd12;
    in_xd2 = *(double *)((int)puVar16 + -8);
  }
  if (in_FPSCR_PR == '\0') {
    dVar37 = (double)CONCAT44(fVar39,fVar20 + SUB84(dVar22,0));
  }
  else {
    dVar37 = dVar37 + dVar22;
  }
  if (in_FPSCR_SZ == '\0') {
    pdVar9 = (double *)((int)pdVar9 + -4);
    *(int *)pdVar9 = SUB84(dVar37,0);
  }
  else {
    pdVar9 = pdVar9 + -1;
    *pdVar9 = in_xd14;
  }
  *(undefined4 *)((int)pdVar9 + -4) = uVar4;
  *(undefined4 *)((int)pdVar9 + 0x34) = uVar11;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(pdVar9 + 6) = *(undefined4 *)((int)puVar16 + -0x18);
    *(undefined4 *)((int)pdVar9 + 0x2c) = *(undefined4 *)((int)puVar16 + -0x14);
    pdVar9 = pdVar9 + 5;
    *(undefined4 *)pdVar9 = uVar40;
    dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)((int)puVar16 + -4));
  }
  else {
    *(undefined8 *)((int)pdVar9 + 0x2c) = *(undefined8 *)((int)puVar16 + -0x18);
    *(undefined8 *)((int)pdVar9 + 0x24) = *(undefined8 *)((int)puVar16 + -0x14);
    pdVar9 = (double *)((int)pdVar9 + 0x1c);
    *pdVar9 = dVar33;
    in_xd2 = *(double *)((int)puVar16 + -4);
  }
  if (in_FPSCR_PR == '\0') {
    dVar34 = (double)CONCAT44((float)((ulonglong)dVar34 >> 0x20) + SUB84(dVar22,0),SUB84(dVar34,0));
  }
  else {
    dVar34 = dVar34 + dVar22;
  }
  if (in_FPSCR_SZ == '\0') {
    pdVar9 = (double *)((int)pdVar9 + -4);
    *(int *)pdVar9 = (int)((ulonglong)dVar34 >> 0x20);
    dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)((int)puVar16 + -8));
  }
  else {
    pdVar9 = pdVar9 + -1;
    *pdVar9 = dVar34;
    in_xd2 = *(double *)((int)puVar16 + -8);
  }
  if (in_FPSCR_PR == '\0') {
    in_dr14 = (double)CONCAT44((float)((ulonglong)dVar37 >> 0x20) + SUB84(dVar22,0),SUB84(dVar37,0))
    ;
  }
  else {
    in_dr14 = dVar37 + dVar22;
  }
  if (in_FPSCR_SZ == '\0') {
    pdVar9 = (double *)((int)pdVar9 + -4);
    *(int *)pdVar9 = (int)((ulonglong)in_dr14 >> 0x20);
  }
  else {
    pdVar9 = pdVar9 + -1;
    *pdVar9 = in_dr14;
  }
  *(undefined4 *)((int)pdVar9 + -4) = uVar2;
  pdVar9 = (double *)((int)pdVar9 + 0x1c);
LAB_8c650342:
  iVar5 = 1;
  *(undefined4 *)((int)puVar16 + -0x5c) = 4;
  iVar12 = 0;
  pdVar10 = (double *)*param_4;
  do {
    pdVar15 = (double *)((int)pdVar10 + 0xc);
    uVar19 = SUB84(dVar22,0);
    iVar6 = *param_4;
    *(int *)((int)puVar16 + -100) = iVar6;
    if (in_FPSCR_SZ == '\0') {
      uVar19 = *(undefined4 *)pdVar3;
    }
    else {
      in_xd2 = *pdVar3;
    }
    iVar6 = iVar6 + iVar5 * 0xc;
    if (in_FPSCR_SZ == '\0') {
      dVar22 = (double)CONCAT44(*(undefined4 *)(iVar6 + 8),uVar19);
    }
    else {
      dVar22 = *(double *)(iVar6 + 8);
    }
    if ((float)((ulonglong)dVar22 >> 0x20) == SUB84(dVar22,0)) {
      if (in_FPSCR_SZ == '\0') {
        dVar22 = (double)(ulonglong)*(uint *)pdVar3;
      }
      else {
        in_xd2 = *pdVar3;
      }
      iVar6 = (iVar5 + -1) * 0xc + *(int *)((int)puVar16 + -100);
      if (in_FPSCR_SZ == '\0') {
        dVar22 = (double)CONCAT44(*(undefined4 *)(iVar6 + 8),SUB84(dVar22,0));
      }
      else {
        dVar22 = *(double *)(iVar6 + 8);
      }
      if ((float)((ulonglong)dVar22 >> 0x20) != SUB84(dVar22,0)) goto LAB_8c650396;
    }
    else {
LAB_8c650396:
      dVar33 = dVar26;
      dVar37 = dVar26;
      if (in_FPSCR_SZ == '\0') {
        dVar33 = 0.007812501848093234;
        dVar37 = in_xd10;
      }
      pdVar7 = (double *)((iVar5 + -1) * 0xc + *param_4);
      *(double **)((int)puVar16 + -100) = pdVar7;
      if (in_FPSCR_SZ == '\0') {
        dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)(pdVar7 + 1));
      }
      else {
        in_xd2 = pdVar7[1];
      }
      if (in_FPSCR_PR == '\0') {
        dVar33 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) / SUB84(dVar22,0),
                                  SUB84(dVar33,0));
      }
      else {
        dVar33 = dVar33 / dVar22;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                  *(undefined4 *)((int)pdVar10 + 0x14));
      }
      else {
        in_xd2 = *(double *)((int)pdVar10 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),SUB84(dVar33,0) / SUB84(dVar22,0)
                                 );
      }
      else {
        dVar33 = dVar33 / dVar22;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)pdVar7);
        dVar38 = (double)CONCAT44(*(undefined4 *)((int)pdVar1 + 0xc),SUB84(in_dr14,0));
      }
      else {
        in_xd2 = *pdVar7;
        dVar38 = *(double *)((int)pdVar1 + 0xc);
      }
      if (in_FPSCR_PR == '\0') {
        dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),SUB84(dVar22,0) * fVar29);
      }
      else {
        dVar22 = dVar22 * dVar28;
      }
      uVar19 = (undefined4)((ulonglong)dVar33 >> 0x20);
      dVar34 = dVar33;
      dVar35 = dVar38;
      if (in_FPSCR_SZ == '\0') {
        uVar40 = (undefined4)((ulonglong)dVar38 >> 0x20);
        dVar38 = (double)CONCAT44(uVar40,uVar40);
        dVar34 = (double)CONCAT44(uVar19,in_fr1);
        dVar35 = in_xd14;
      }
      in_xd14 = dVar35;
      if (in_FPSCR_PR == '\0') {
        dVar38 = (double)CONCAT44((int)((ulonglong)dVar38 >> 0x20),
                                  (float)((ulonglong)dVar34 >> 0x20) * SUB84(dVar22,0) +
                                  SUB84(dVar38,0));
      }
      uVar40 = SUB84(dVar33,0);
      if (in_FPSCR_SZ == '\0') {
        dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)pdVar15);
        dVar34 = (double)CONCAT44(uVar40,SUB84(dVar34,0));
      }
      else {
        in_xd2 = *pdVar15;
        dVar34 = dVar37;
      }
      if (in_FPSCR_PR == '\0') {
        fVar20 = SUB84(dVar22,0) * fVar29;
        dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),fVar20);
        dVar38 = (double)CONCAT44((float)((ulonglong)dVar34 >> 0x20) * fVar20 +
                                  (float)((ulonglong)dVar38 >> 0x20),SUB84(dVar38,0));
      }
      else {
        dVar22 = dVar22 * dVar28;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)((int)pdVar7 + 4))
        ;
      }
      else {
        in_xd2 = *(double *)((int)pdVar7 + 4);
      }
      if (in_FPSCR_PR == '\0') {
        dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),SUB84(dVar22,0) * fVar30);
      }
      else {
        dVar22 = dVar22 * dVar31;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar35 = (double)CONCAT44(*(undefined4 *)(pdVar1 + 2),*(undefined4 *)(pdVar1 + 2));
        dVar34 = (double)CONCAT44(uVar19,SUB84(dVar34,0));
      }
      else {
        dVar35 = pdVar1[2];
        in_xd12 = dVar35;
        dVar34 = dVar33;
      }
      if (in_FPSCR_PR == '\0') {
        dVar35 = (double)CONCAT44((int)((ulonglong)dVar35 >> 0x20),
                                  (float)((ulonglong)dVar34 >> 0x20) * SUB84(dVar22,0) +
                                  SUB84(dVar35,0));
      }
      if (in_FPSCR_SZ == '\0') {
        dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)(pdVar10 + 2));
        dVar34 = (double)CONCAT44(uVar40,SUB84(dVar34,0));
      }
      else {
        in_xd2 = pdVar10[2];
        dVar34 = dVar37;
      }
      if (in_FPSCR_PR == '\0') {
        fVar20 = SUB84(dVar22,0) * fVar30;
      }
      else {
        fVar20 = SUB84(dVar22 * dVar31,0);
      }
      dVar22 = in_xd12;
      if (in_FPSCR_SZ == '\0') {
        dVar22 = (double)CONCAT44(SUB84(dVar35,0),fVar20);
      }
      if (in_FPSCR_PR == '\0') {
        dVar35 = (double)CONCAT44((float)((ulonglong)dVar34 >> 0x20) * SUB84(dVar22,0) +
                                  (float)((ulonglong)dVar35 >> 0x20),SUB84(dVar35,0));
      }
      fVar20 = SUB84(dVar38,0);
      dVar23 = in_xd14;
      if (in_FPSCR_SZ == '\0') {
        dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),fVar20);
        dVar23 = in_xd2;
      }
      fVar39 = (float)((ulonglong)dVar38 >> 0x20);
      fVar36 = (float)((ulonglong)dVar35 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) - fVar36,
                                  SUB84(dVar22,0) - fVar39);
      }
      else {
        dVar22 = (dVar22 - dVar38) - dVar35;
      }
      fVar21 = SUB84(dVar22,0);
      fVar17 = (float)((ulonglong)dVar22 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        *(float *)((int)puVar16 + -0x54) = fVar21;
        *(float *)((int)puVar16 + -0x58) = fVar17;
        dVar34 = (double)CONCAT44(fVar21,SUB84(dVar34,0));
      }
      else {
        *(double *)((int)puVar16 + -0x54) = dVar23;
        *(double *)((int)puVar16 + -0x58) = dVar22;
        dVar34 = dVar23;
      }
      fVar18 = (float)((ulonglong)dVar34 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar22 = (double)CONCAT44(fVar18 * fVar21 + fVar17 * fVar17,fVar21);
      }
      else {
        dVar22 = dVar22 * dVar22;
      }
      in_xd2 = dVar26;
      if (in_FPSCR_SZ == '\0') {
        dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),0x3f800000);
        in_xd2 = dVar23;
      }
      if (in_FPSCR_PR == '\0') {
        fVar21 = SUB84(dVar22,0) / SQRT((float)((ulonglong)dVar22 >> 0x20));
      }
      else {
        fVar21 = SUB84(SQRT(dVar22) / SQRT(dVar22),0);
      }
      if (in_FPSCR_SZ == '\0') {
        *(float *)((int)puVar16 + -0x48) = fVar21;
        dVar22 = (double)CONCAT44(*(undefined4 *)((int)puVar16 + -0x58),fVar21);
      }
      else {
        *(double *)((int)puVar16 + -0x48) = in_xd2;
        dVar22 = *(double *)((int)puVar16 + -0x58);
      }
      if (in_FPSCR_PR == '\0') {
        dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) * SUB84(dVar22,0) * fVar27,
                                  SUB84(dVar22,0));
      }
      else {
        dVar22 = dVar22 * dVar22 * dVar28;
      }
      fVar21 = (float)((ulonglong)dVar22 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        *(float *)((int)puVar16 + -0x4c) = fVar21;
        dVar34 = (double)CONCAT44(fVar18,*(undefined4 *)((int)puVar16 + -0x54));
      }
      else {
        *(double *)((int)puVar16 + -0x4c) = dVar22;
        in_xd0 = *(undefined8 *)((int)puVar16 + -0x54);
      }
      if (in_FPSCR_PR == '\0') {
        dVar23 = (double)CONCAT44(fVar21 * fVar32,SUB84(dVar22,0));
        fVar21 = SUB84(dVar34,0) * SUB84(dVar22,0) * fVar27;
      }
      else {
        dVar23 = dVar22 * dVar31;
        fVar21 = SUB84(dVar34 * dVar22 * dVar28,0);
      }
      in_fr1 = -fVar21;
      uVar11 = (undefined4)((ulonglong)dVar23 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        *(float *)((int)puVar16 + -0x50) = in_fr1;
        *(undefined4 *)((int)puVar16 + -0x34) = uVar11;
        dVar23 = (double)CONCAT44(uVar11,*(undefined4 *)((int)puVar16 + -0x50));
      }
      else {
        *(undefined8 *)((int)puVar16 + -0x50) = in_xd0;
        *(double *)((int)puVar16 + -0x34) = dVar23;
        in_xd2 = *(double *)((int)puVar16 + -0x50);
      }
      if (in_FPSCR_PR == '\0') {
        dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),SUB84(dVar23,0) * fVar32);
      }
      else {
        dVar23 = dVar23 * dVar31;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)puVar16 + -0x38) = SUB84(dVar23,0);
      }
      else {
        *(double *)((int)puVar16 + -0x38) = in_xd2;
      }
      iVar6 = param_4[2];
      *(int *)((int)puVar16 + -100) = iVar6;
      psVar8 = (short *)(iVar6 + iVar12);
      *(short **)((int)puVar16 + -0x60) = psVar8;
      uVar11 = (undefined4)((ulonglong)dVar23 >> 0x20);
      fVar21 = (float)(int)*psVar8;
      if (in_FPSCR_PR == '\0') {
        dVar22 = (double)CONCAT44(uVar11,fVar21 * fVar25);
      }
      else {
        dVar22 = (double)CONCAT44(uVar11,fVar21) * dVar26;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)puVar16 + -0x30) = SUB84(dVar22,0);
      }
      else {
        *(double *)((int)puVar16 + -0x30) = in_xd2;
      }
      uVar11 = (undefined4)((ulonglong)dVar22 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar22 = (double)CONCAT44(uVar11,(float)(int)*(short *)(*(int *)((int)puVar16 + -0x60) + 2)
                                         * fVar25);
      }
      else {
        dVar22 = (double)CONCAT44(uVar11,(float)(int)*(short *)(*(int *)((int)puVar16 + -0x60) + 2))
                 * dVar26;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)puVar16 + -0x3c) = SUB84(dVar22,0);
      }
      else {
        *(double *)((int)puVar16 + -0x3c) = in_xd2;
      }
      *(short **)((int)puVar16 + -0x60) =
           (short *)(*(int *)((int)puVar16 + -100) + *(int *)((int)puVar16 + -0x5c));
      uVar11 = (undefined4)((ulonglong)dVar22 >> 0x20);
      fVar21 = (float)(int)*(short *)(*(int *)((int)puVar16 + -100) + *(int *)((int)puVar16 + -0x5c)
                                     );
      if (in_FPSCR_PR == '\0') {
        dVar22 = (double)CONCAT44(uVar11,fVar21 * fVar25);
      }
      else {
        dVar22 = (double)CONCAT44(uVar11,fVar21) * dVar26;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)puVar16 + -0x40) = SUB84(dVar22,0);
      }
      else {
        *(double *)((int)puVar16 + -0x40) = in_xd2;
      }
      if (in_FPSCR_PR == '\0') {
        fVar21 = (float)(int)*(short *)(*(int *)((int)puVar16 + -0x60) + 2) * fVar25;
      }
      else {
        fVar21 = SUB84((double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                        (float)(int)*(short *)(*(int *)((int)puVar16 + -0x60) + 2))
                       * dVar26,0);
      }
      if (in_FPSCR_SZ == '\0') {
        *(float *)((int)puVar16 + -0x44) = fVar21;
      }
      else {
        *(double *)((int)puVar16 + -0x44) = in_xd2;
      }
      iVar6 = param_4[1];
      *(int *)((int)puVar16 + -0x60) = iVar6;
      *(undefined4 *)((int)puVar16 + -100) = *(undefined4 *)(iVar6 + iVar12);
      *(undefined4 *)((int)puVar16 + -0x60) =
           *(undefined4 *)(*(int *)((int)puVar16 + -0x60) + *(int *)((int)puVar16 + -0x5c));
      *(undefined4 *)(pdVar9 + 3) = *(undefined4 *)((int)puVar16 + -100);
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)((int)pdVar9 + 0x14) = *(undefined4 *)((int)puVar16 + -0x3c);
        *(undefined4 *)(pdVar9 + 2) = *(undefined4 *)((int)puVar16 + -0x30);
        pdVar9 = (double *)((int)pdVar9 + 0xc);
        *(undefined4 *)pdVar9 = uVar19;
        dVar22 = (double)CONCAT44(SUB84(dVar35,0),*(undefined4 *)((int)puVar16 + -0x50));
      }
      else {
        pdVar9[2] = *(double *)((int)puVar16 + -0x3c);
        pdVar9[1] = *(double *)((int)puVar16 + -0x30);
        *pdVar9 = dVar33;
        in_xd2 = *(double *)((int)puVar16 + -0x50);
        dVar22 = in_xd12;
      }
      if (in_FPSCR_PR == '\0') {
        dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) - SUB84(dVar22,0),
                                  SUB84(dVar22,0));
      }
      else {
        dVar22 = dVar22 - dVar22;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar9 = (double *)((int)pdVar9 + -4);
        *(int *)pdVar9 = (int)((ulonglong)dVar22 >> 0x20);
        dVar22 = (double)CONCAT44(fVar20,*(undefined4 *)((int)puVar16 + -0x4c));
      }
      else {
        pdVar9 = pdVar9 + -1;
        *pdVar9 = dVar22;
        in_xd2 = *(double *)((int)puVar16 + -0x4c);
        dVar22 = in_xd14;
      }
      if (in_FPSCR_PR == '\0') {
        dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) - SUB84(dVar22,0),
                                  SUB84(dVar22,0));
      }
      else {
        dVar22 = dVar22 - dVar22;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar9 = (double *)((int)pdVar9 + -4);
        *(int *)pdVar9 = (int)((ulonglong)dVar22 >> 0x20);
      }
      else {
        pdVar9 = pdVar9 + -1;
        *pdVar9 = dVar22;
      }
      *(undefined4 *)((int)pdVar9 + -4) = uVar4;
      *(undefined4 *)((int)pdVar9 + 0x34) = *(undefined4 *)((int)puVar16 + -0x60);
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)(pdVar9 + 6) = *(undefined4 *)((int)puVar16 + -0x44);
        *(undefined4 *)((int)pdVar9 + 0x2c) = *(undefined4 *)((int)puVar16 + -0x40);
        pdVar9 = pdVar9 + 5;
        *(undefined4 *)pdVar9 = uVar40;
        dVar22 = (double)CONCAT44(fVar36,*(undefined4 *)((int)puVar16 + -0x50));
      }
      else {
        *(undefined8 *)((int)pdVar9 + 0x2c) = *(undefined8 *)((int)puVar16 + -0x44);
        *(undefined8 *)((int)pdVar9 + 0x24) = *(undefined8 *)((int)puVar16 + -0x40);
        pdVar9 = (double *)((int)pdVar9 + 0x1c);
        *pdVar9 = dVar37;
        in_xd2 = *(double *)((int)puVar16 + -0x50);
        dVar22 = dVar35;
      }
      if (in_FPSCR_PR == '\0') {
        dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) - SUB84(dVar22,0),
                                  SUB84(dVar22,0));
      }
      else {
        dVar22 = dVar22 - dVar22;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar9 = (double *)((int)pdVar9 + -4);
        *(int *)pdVar9 = (int)((ulonglong)dVar22 >> 0x20);
        dVar22 = (double)CONCAT44(fVar39,*(undefined4 *)((int)puVar16 + -0x4c));
      }
      else {
        pdVar9 = pdVar9 + -1;
        *pdVar9 = dVar22;
        in_xd2 = *(double *)((int)puVar16 + -0x4c);
        dVar22 = dVar38;
      }
      if (in_FPSCR_PR == '\0') {
        dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) - SUB84(dVar22,0),
                                  SUB84(dVar22,0));
      }
      else {
        dVar22 = dVar22 - dVar22;
      }
      uVar11 = (undefined4)((ulonglong)dVar22 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        pdVar9 = (double *)((int)pdVar9 + -4);
        *(undefined4 *)pdVar9 = uVar11;
      }
      else {
        pdVar9 = pdVar9 + -1;
        *pdVar9 = dVar22;
      }
      *(undefined4 *)((int)pdVar9 + -4) = uVar4;
      *(undefined4 *)((int)pdVar9 + 0x34) = *(undefined4 *)((int)puVar16 + -100);
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)(pdVar9 + 6) = *(undefined4 *)((int)puVar16 + -0x3c);
        *(undefined4 *)((int)pdVar9 + 0x2c) = *(undefined4 *)((int)puVar16 + -0x30);
        pdVar9 = pdVar9 + 5;
        *(undefined4 *)pdVar9 = uVar19;
        dVar22 = (double)CONCAT44(uVar11,*(undefined4 *)((int)puVar16 + -0x38));
      }
      else {
        *(undefined8 *)((int)pdVar9 + 0x2c) = *(undefined8 *)((int)puVar16 + -0x3c);
        *(undefined8 *)((int)pdVar9 + 0x24) = *(undefined8 *)((int)puVar16 + -0x30);
        pdVar9 = (double *)((int)pdVar9 + 0x1c);
        *pdVar9 = dVar33;
        in_xd2 = *(double *)((int)puVar16 + -0x38);
      }
      if (in_FPSCR_PR == '\0') {
        dVar35 = (double)CONCAT44(fVar36,SUB84(dVar35,0) + SUB84(dVar22,0));
      }
      else {
        dVar35 = dVar35 + dVar22;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar9 = (double *)((int)pdVar9 + -4);
        *(int *)pdVar9 = SUB84(dVar35,0);
        dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                  *(undefined4 *)((int)puVar16 + -0x34));
      }
      else {
        pdVar9 = pdVar9 + -1;
        *pdVar9 = in_xd12;
        in_xd2 = *(double *)((int)puVar16 + -0x34);
      }
      if (in_FPSCR_PR == '\0') {
        dVar38 = (double)CONCAT44(fVar39,fVar20 + SUB84(dVar22,0));
      }
      else {
        dVar38 = dVar38 + dVar22;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar9 = (double *)((int)pdVar9 + -4);
        *(int *)pdVar9 = SUB84(dVar38,0);
      }
      else {
        pdVar9 = pdVar9 + -1;
        *pdVar9 = in_xd14;
      }
      *(undefined4 *)((int)pdVar9 + -4) = uVar4;
      *(undefined4 *)((int)pdVar9 + 0x34) = *(undefined4 *)((int)puVar16 + -0x60);
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)(pdVar9 + 6) = *(undefined4 *)((int)puVar16 + -0x44);
        *(undefined4 *)((int)pdVar9 + 0x2c) = *(undefined4 *)((int)puVar16 + -0x40);
        pdVar9 = pdVar9 + 5;
        *(undefined4 *)pdVar9 = uVar40;
        dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                  *(undefined4 *)((int)puVar16 + -0x38));
      }
      else {
        *(undefined8 *)((int)pdVar9 + 0x2c) = *(undefined8 *)((int)puVar16 + -0x44);
        *(undefined8 *)((int)pdVar9 + 0x24) = *(undefined8 *)((int)puVar16 + -0x40);
        pdVar9 = (double *)((int)pdVar9 + 0x1c);
        *pdVar9 = dVar37;
        in_xd2 = *(double *)((int)puVar16 + -0x38);
      }
      if (in_FPSCR_PR == '\0') {
        dVar35 = (double)CONCAT44((float)((ulonglong)dVar35 >> 0x20) + SUB84(dVar22,0),
                                  SUB84(dVar35,0));
      }
      else {
        dVar35 = dVar35 + dVar22;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar9 = (double *)((int)pdVar9 + -4);
        *(int *)pdVar9 = (int)((ulonglong)dVar35 >> 0x20);
        dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                  *(undefined4 *)((int)puVar16 + -0x34));
      }
      else {
        pdVar9 = pdVar9 + -1;
        *pdVar9 = dVar35;
        in_xd2 = *(double *)((int)puVar16 + -0x34);
      }
      if (in_FPSCR_PR == '\0') {
        in_dr14 = (double)CONCAT44((float)((ulonglong)dVar38 >> 0x20) + SUB84(dVar22,0),
                                   SUB84(dVar38,0));
      }
      else {
        in_dr14 = dVar38 + dVar22;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar9 = (double *)((int)pdVar9 + -4);
        *(int *)pdVar9 = (int)((ulonglong)in_dr14 >> 0x20);
      }
      else {
        pdVar9 = pdVar9 + -1;
        *pdVar9 = in_dr14;
      }
      *(undefined4 *)((int)pdVar9 + -4) = uVar2;
      pdVar9 = (double *)((int)pdVar9 + 0x1c);
      in_xd10 = dVar37;
    }
    iVar5 = iVar5 + 1;
    iVar12 = iVar12 + 4;
    *(int *)((int)puVar16 + -0x5c) = *(int *)((int)puVar16 + -0x5c) + 4;
    pdVar10 = pdVar15;
    if (param_3 < iVar5) {
      *param_5 = pdVar9;
      return;
    }
  } while( true );
}

