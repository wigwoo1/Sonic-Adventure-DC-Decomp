/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c09eb8c) */
/* WARNING: Removing unreachable block (ram,0x8c09eb3a) */
/* WARNING: Removing unreachable block (ram,0x8c09eb4a) */
/* WARNING: Removing unreachable block (ram,0x8c09eb9c) */
/* WARNING: Restarted to delay deadcode elimination for space: register */

undefined4
FUN_8c09eb08(undefined4 param_1,double param_2,double param_3,double *param_4,int param_5,
            uint param_6)

{
  short sVar1;
  short sVar2;
  short sVar3;
  undefined4 uVar4;
  int iVar5;
  undefined4 uVar6;
  int in_r2;
  byte bVar7;
  short *psVar8;
  double *pdVar9;
  undefined4 unaff_r8;
  double *pdVar10;
  undefined4 unaff_r9;
  undefined8 *puVar11;
  undefined1 *puVar12;
  undefined4 in_fr1;
  float fVar13;
  double dVar15;
  float fVar17;
  float fVar18;
  double dVar16;
  float fVar19;
  float fVar20;
  undefined4 uVar21;
  undefined4 uVar22;
  double dVar23;
  float fVar26;
  double dVar24;
  double dVar25;
  float fVar27;
  float fVar28;
  double dVar29;
  float fVar30;
  undefined4 uVar31;
  undefined4 uVar32;
  float fVar33;
  double dVar34;
  float fVar35;
  float fVar36;
  undefined4 uVar37;
  float fVar38;
  float fVar39;
  undefined8 in_dr12;
  double dVar40;
  undefined8 in_dr14;
  double dVar41;
  undefined4 uVar42;
  double in_xd0;
  double in_xd2;
  undefined8 uVar43;
  double dVar44;
  double in_xd4;
  double dVar45;
  double in_xd6;
  double in_xd8;
  double dVar46;
  double in_xd10;
  float fVar47;
  double in_xd12;
  double dVar48;
  double in_xd14;
  double dVar49;
  float fVar50;
  bool bVar51;
  bool bVar52;
  bool bVar53;
  bool bVar54;
  bool bVar55;
  bool bVar56;
  bool bVar57;
  bool bVar58;
  bool bVar59;
  bool bVar60;
  bool bVar61;
  bool bVar62;
  bool bVar63;
  bool bVar64;
  bool bVar65;
  bool bVar66;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_20 [16];
  undefined8 local_10;
  undefined8 local_8;
  float fVar14;
  
  uVar4 = DAT_8c09edd0;
  uVar6 = DAT_8c09edcc;
  if (in_FPSCR_SZ == '\0') {
    local_8 = (double)CONCAT44((int)in_dr14,(undefined4)local_8);
                    /* WARNING: Ignoring partial resolution of indirect */
    local_8._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
    puVar11 = &local_10;
    local_10 = CONCAT44((int)in_dr12,(int)((ulonglong)in_dr12 >> 0x20));
  }
  else {
    puVar11 = (undefined8 *)auStack_20;
    local_8 = in_xd14;
    local_10 = in_dr14;
  }
  *(undefined4 *)((int)puVar11 + -4) = unaff_r9;
  *(undefined4 *)((int)puVar11 + -8) = unaff_r8;
  psVar8 = (short *)(param_5 * 0x14 + *(int *)((int)param_4 + 0x1c));
  if (in_FPSCR_SZ == '\0') {
    dVar44 = (double)CONCAT44(DAT_8c09ebec,param_1);
  }
  else {
    dVar44 = (double)CONCAT44(uRam8c09ebf0,DAT_8c09ebec);
  }
  if (in_FPSCR_PR == '\0') {
    dVar15 = (double)CONCAT44((float)(int)psVar8[4],in_fr1);
  }
  else {
    dVar15 = (double)(int)psVar8[4];
  }
  fVar18 = (float)((ulonglong)dVar44 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    fVar17 = (float)((ulonglong)dVar15 >> 0x20) * fVar18;
  }
  else {
    fVar17 = (float)((ulonglong)(dVar15 * dVar44) >> 0x20);
  }
  if (in_FPSCR_PR == '\0') {
    dVar15 = (double)CONCAT44(fVar17,(float)(int)psVar8[5] * fVar18);
  }
  else {
    dVar15 = (double)CONCAT44(fVar17,(float)(int)psVar8[5]) * dVar44;
  }
  if (in_FPSCR_PR == '\0') {
    fVar17 = (float)(int)psVar8[6] * fVar18;
  }
  else {
    fVar17 = (float)((ulonglong)((double)(int)psVar8[6] * dVar44) >> 0x20);
  }
  if (in_FPSCR_PR == '\0') {
    dVar23 = (double)CONCAT44(fVar17,(float)(int)psVar8[7] * fVar18);
  }
  else {
    dVar23 = (double)CONCAT44(fVar17,(float)(int)psVar8[7]) * dVar44;
  }
  dVar16 = dVar15;
  dVar49 = dVar23;
  dVar29 = dVar44;
  if (((param_6 & 4) != 0) &&
     (dVar16 = dVar23, dVar49 = dVar15, dVar29 = dVar15, in_FPSCR_SZ == '\0')) {
    uVar22 = (undefined4)((ulonglong)dVar15 >> 0x20);
    dVar16 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),SUB84(dVar15,0));
    dVar49 = (double)CONCAT44(uVar22,SUB84(dVar23,0));
    dVar29 = (double)CONCAT44(uVar22,SUB84(dVar44,0));
  }
  dVar23 = dVar29;
  dVar44 = in_xd0;
  dVar15 = in_xd2;
  if (((param_6 & 8) != 0) &&
     (dVar23 = in_xd0, dVar44 = in_xd2, dVar15 = in_xd0, in_FPSCR_SZ == '\0')) {
    uVar22 = SUB84(dVar16,0);
    dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),SUB84(dVar49,0));
    dVar49 = (double)CONCAT44((int)((ulonglong)dVar49 >> 0x20),uVar22);
    dVar23 = (double)CONCAT44(uVar22,SUB84(dVar29,0));
    dVar44 = in_xd0;
    dVar15 = in_xd2;
  }
  sVar1 = *psVar8;
  sVar2 = psVar8[1];
  sVar3 = psVar8[3];
  iVar5 = -(int)psVar8[2];
  uVar22 = (undefined4)((ulonglong)dVar16 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    pdVar10 = (double *)((int)puVar11 + -0xc);
    *(undefined4 *)pdVar10 = uVar22;
  }
  else {
    pdVar10 = (double *)((int)puVar11 + -0x10);
    *pdVar10 = dVar16;
  }
  if (in_FPSCR_SZ == '\0') {
    pdVar10 = (double *)((int)pdVar10 + -4);
    *(int *)pdVar10 = SUB84(dVar16,0);
  }
  else {
    pdVar10 = pdVar10 + -1;
    *pdVar10 = dVar44;
  }
  if (in_FPSCR_SZ == '\0') {
    pdVar10 = (double *)((int)pdVar10 + -4);
    *(undefined4 *)pdVar10 = uVar22;
  }
  else {
    pdVar10 = pdVar10 + -1;
    *pdVar10 = dVar16;
  }
  uVar22 = (undefined4)((ulonglong)dVar49 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)pdVar10 + -4) = SUB84(dVar49,0);
    *(undefined4 *)(pdVar10 + -1) = uVar22;
    pdVar10 = (double *)((int)pdVar10 + -0xc);
    *(int *)pdVar10 = SUB84(dVar16,0);
  }
  else {
    pdVar10[-1] = dVar15;
    pdVar10[-2] = dVar49;
    pdVar10 = pdVar10 + -3;
    *pdVar10 = dVar44;
  }
  if (in_FPSCR_PR == '\0') {
    fVar18 = (float)iVar5;
  }
  else {
    fVar18 = (float)((ulonglong)(double)iVar5 >> 0x20);
  }
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)((int)pdVar10 + -4) = uVar22;
    pdVar10 = pdVar10 + -1;
    *(int *)pdVar10 = SUB84(dVar49,0);
  }
  else {
    pdVar10[-1] = dVar49;
    pdVar10 = pdVar10 + -2;
    *pdVar10 = dVar15;
  }
  fVar17 = (float)-(int)sVar3;
  if (in_FPSCR_SZ == '\0') {
    dVar23 = (double)CONCAT44(*(undefined4 *)((int)param_4 + 0xc),SUB84(dVar23,0));
  }
  else {
    dVar23 = *(double *)((int)param_4 + 0xc);
  }
  if (in_FPSCR_PR == '\0') {
    fVar26 = (float)(sVar1 + iVar5);
  }
  else {
    fVar26 = (float)((ulonglong)(double)(sVar1 + iVar5) >> 0x20);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(undefined4 *)(param_4 + 2));
  }
  else {
    in_xd4 = param_4[2];
  }
  fVar20 = (float)((int)sVar2 + -(int)sVar3);
  fVar27 = (float)((ulonglong)dVar23 >> 0x20);
  fVar36 = SUB84(dVar23,0);
  if (in_FPSCR_PR == '\0') {
    dVar16 = (double)CONCAT44(fVar18 * fVar27,fVar17 * fVar36);
    dVar23 = (double)CONCAT44(fVar26 * fVar27,fVar20 * fVar36);
  }
  else {
    dVar16 = (double)CONCAT44(fVar18,fVar17) * dVar23 * dVar23;
    dVar23 = (double)CONCAT44(fVar26,fVar20) * dVar23 * dVar23;
  }
  uVar22 = (undefined4)((ulonglong)dVar16 >> 0x20);
  fVar18 = SUB84(dVar23,0);
  uVar32 = (undefined4)((ulonglong)dVar23 >> 0x20);
  dVar49 = in_xd14;
  if ((param_6 & 1) == 0) {
    dVar29 = dVar16;
    dVar41 = dVar23;
    dVar40 = dVar23;
    dVar45 = dVar15;
    dVar46 = dVar44;
    dVar48 = dVar15;
    if (in_FPSCR_SZ == '\0') {
      dVar29 = (double)CONCAT44(uVar22,fVar18);
      dVar41 = (double)CONCAT44(uVar32,SUB84(dVar16,0));
      dVar45 = in_xd4;
      dVar46 = in_xd8;
      dVar48 = in_xd12;
    }
  }
  else {
    if (in_FPSCR_PR == '\0') {
      fVar17 = ((float)(*(uint *)((int)param_4 + 0x14) & 0xffff) * 6.2831855) / 65536.0;
      fVar26 = (float)sin(fVar17);
      fVar17 = (float)cos(fVar17);
      param_3 = (double)CONCAT44((float)fVar26,(float)fVar17);
    }
    dVar49 = dVar23;
    dVar46 = dVar23;
    dVar29 = dVar16;
    if (in_FPSCR_SZ == '\0') {
      param_2 = (double)CONCAT44((int)((ulonglong)param_2 >> 0x20),SUB84(dVar16,0));
      dVar16 = (double)CONCAT44(uVar22,uVar22);
      dVar49 = (double)CONCAT44(uVar32,uVar32);
      dVar46 = in_xd8;
      dVar29 = dVar44;
      dVar44 = in_xd6;
    }
    in_xd6 = dVar44;
    dVar44 = dVar29;
    fVar17 = SUB84(param_3,0);
    fVar26 = (float)((ulonglong)param_3 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      dVar16 = (double)CONCAT44((float)((ulonglong)dVar16 >> 0x20) * fVar17,SUB84(dVar16,0) * fVar26
                               );
      dVar34 = (double)CONCAT44((float)((ulonglong)dVar49 >> 0x20) * fVar17,SUB84(dVar49,0) * fVar26
                               );
    }
    else {
      dVar16 = dVar16 * param_3 * param_3;
      dVar34 = dVar49 * param_3 * param_3;
    }
    dVar29 = in_xd6;
    dVar49 = dVar15;
    if (in_FPSCR_SZ == '\0') {
      dVar29 = (double)CONCAT44(SUB84(param_2,0),fVar18);
      dVar49 = in_xd14;
    }
    if (in_FPSCR_PR == '\0') {
      param_2 = (double)CONCAT44((int)((ulonglong)param_2 >> 0x20),SUB84(param_2,0) * fVar26);
      dVar23 = (double)CONCAT44(uVar32,fVar18 * fVar26);
      dVar41 = (double)CONCAT44((float)((ulonglong)dVar29 >> 0x20) * fVar17,SUB84(dVar29,0) * fVar17
                               );
    }
    else {
      param_2 = param_2 * param_3;
      dVar23 = dVar23 * param_3;
      dVar41 = dVar29 * param_3 * param_3;
    }
    fVar18 = (float)((ulonglong)dVar16 >> 0x20);
    dVar29 = dVar16;
    if (in_FPSCR_SZ == '\0') {
      dVar29 = (double)CONCAT44(fVar18,fVar36);
    }
    if (in_FPSCR_PR == '\0') {
      dVar29 = (double)CONCAT44((float)((ulonglong)dVar29 >> 0x20) - SUB84(dVar23,0),SUB84(dVar29,0)
                               );
    }
    else {
      dVar29 = dVar29 - dVar23;
    }
    dVar45 = dVar44;
    if (in_FPSCR_SZ == '\0') {
      dVar29 = (double)CONCAT44((int)((ulonglong)dVar29 >> 0x20),SUB84(dVar16,0));
      dVar45 = in_xd4;
    }
    fVar17 = (float)((ulonglong)dVar41 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      dVar29 = (double)CONCAT44((int)((ulonglong)dVar29 >> 0x20),SUB84(dVar29,0) + SUB84(dVar41,0));
      dVar16 = (double)CONCAT44(fVar18 - SUB84(param_2,0),SUB84(dVar16,0) + fVar17);
    }
    else {
      dVar29 = dVar29 + dVar41;
      dVar16 = (dVar16 - param_2) + dVar41;
    }
    fVar18 = (float)((ulonglong)dVar34 >> 0x20);
    dVar40 = dVar34;
    if (in_FPSCR_SZ == '\0') {
      dVar40 = (double)CONCAT44(fVar18,(int)in_dr12);
    }
    if (in_FPSCR_PR == '\0') {
      dVar40 = (double)CONCAT44((float)((ulonglong)dVar40 >> 0x20) - SUB84(dVar23,0),SUB84(dVar40,0)
                               );
    }
    else {
      dVar40 = dVar40 - dVar23;
    }
    dVar48 = dVar46;
    if (in_FPSCR_SZ == '\0') {
      dVar40 = (double)CONCAT44((int)((ulonglong)dVar40 >> 0x20),SUB84(dVar34,0));
      dVar48 = in_xd12;
    }
    if (in_FPSCR_PR == '\0') {
      dVar40 = (double)CONCAT44((int)((ulonglong)dVar40 >> 0x20),SUB84(dVar40,0) + SUB84(dVar41,0));
      dVar41 = (double)CONCAT44(fVar18 - SUB84(param_2,0),SUB84(dVar34,0) + fVar17);
    }
    else {
      dVar40 = dVar40 + dVar41;
      dVar41 = (dVar34 - param_2) + dVar41;
    }
  }
  if (in_FPSCR_SZ == '\0') {
    dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(undefined4 *)param_4);
    param_2 = (double)CONCAT44((int)((ulonglong)param_2 >> 0x20),*(undefined4 *)((int)param_4 + 4));
    param_4 = param_4 + 1;
  }
  else {
    dVar15 = *param_4;
    in_xd6 = param_4[1];
    param_4 = param_4 + 2;
  }
  fVar18 = SUB84(param_2,0);
  if (in_FPSCR_PR == '\0') {
    dVar16 = (double)CONCAT44((float)((ulonglong)dVar16 >> 0x20) + SUB84(dVar23,0),
                              SUB84(dVar16,0) + fVar18);
  }
  else {
    dVar16 = dVar16 + dVar23 + param_2;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar23 = (double)CONCAT44(*(undefined4 *)param_4,SUB84(dVar23,0));
  }
  else {
    dVar23 = *param_4;
  }
  fVar17 = SUB84(dVar23,0);
  if (in_FPSCR_PR == '\0') {
    param_2 = (double)CONCAT44((float)((ulonglong)dVar29 >> 0x20) + fVar17,SUB84(dVar29,0) + fVar18)
    ;
  }
  else {
    param_2 = dVar29 + dVar23 + param_2;
  }
  fVar26 = (float)((ulonglong)dVar23 >> 0x20);
  dVar29 = dVar23;
  if (in_FPSCR_SZ == '\0') {
    dVar29 = (double)CONCAT44(fVar26,fVar18);
  }
  if (in_FPSCR_PR == '\0') {
    dVar41 = (double)CONCAT44((float)((ulonglong)dVar41 >> 0x20) + fVar17,
                              SUB84(dVar41,0) + SUB84(dVar29,0));
  }
  else {
    dVar41 = dVar41 + dVar23 + dVar29;
  }
  dVar34 = dVar23;
  if (in_FPSCR_SZ == '\0') {
    dVar34 = (double)CONCAT44(fVar26,SUB84(param_3,0));
  }
  if (in_FPSCR_PR == '\0') {
    dVar40 = (double)CONCAT44((float)((ulonglong)dVar40 >> 0x20) + fVar17,
                              SUB84(dVar40,0) + SUB84(dVar29,0));
  }
  else {
    dVar40 = dVar40 + dVar23 + dVar29;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar34 = (double)CONCAT44((int)((ulonglong)dVar34 >> 0x20),*(undefined4 *)DAT_8c09ec80);
    dVar23 = (double)CONCAT44(fVar26,*(undefined4 *)DAT_8c09ec84);
  }
  else {
    in_xd10 = *DAT_8c09ec80;
    dVar49 = *DAT_8c09ec84;
  }
                    /* WARNING: Ignoring partial resolution of indirect */
  fVar20 = SUB84(dVar44,0);
  fVar36 = (float)((ulonglong)dVar44 >> 0x20);
  fVar27 = (float)((ulonglong)dVar15 >> 0x20);
  fVar28 = SUB84(dVar45,0);
  fVar30 = (float)((ulonglong)dVar45 >> 0x20);
  fVar33 = (float)((ulonglong)in_xd6 >> 0x20);
  fVar35 = SUB84(dVar46,0);
  fVar39 = (float)((ulonglong)dVar46 >> 0x20);
  fVar47 = (float)((ulonglong)in_xd10 >> 0x20);
  fVar18 = SUB84(dVar48,0);
  fVar17 = (float)((ulonglong)dVar48 >> 0x20);
  fVar50 = (float)((ulonglong)dVar49 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    fVar14 = SUB84(dVar16,0);
    fVar19 = (float)((ulonglong)dVar16 >> 0x20);
    fVar13 = fVar18 * fVar26;
    fVar38 = fVar17 * fVar26;
    fVar26 = fVar27 * fVar14 + fVar33 * fVar19 + fVar47 * 1.0 + fVar50 * fVar26;
    fVar13 = fVar20 * fVar14 + fVar28 * fVar19 + fVar35 * 1.0 + fVar13;
    fVar13 = fVar36 * fVar14 + fVar30 * fVar19 + fVar39 * 1.0 + fVar38;
  }
  if (in_FPSCR_PR == '\0') {
    dVar24 = (double)CONCAT44(fVar26,1.0 / fVar26);
    bVar51 = SUB84(dVar34,0) < fVar26;
    bVar52 = fVar26 < SUB84(dVar23,0);
  }
  else {
    dVar24 = (double)CONCAT44(fVar26,0x3f800000) / (double)CONCAT44(fVar26,0x3f800000);
    bVar51 = dVar34 < dVar24;
    bVar52 = dVar24 < dVar23;
  }
  dVar25 = dVar15;
  if (in_FPSCR_SZ == '\0') {
    dVar25 = (double)CONCAT44(SUB84(dVar24,0),SUB84(dVar24,0));
  }
  fVar26 = (float)((ulonglong)dVar29 >> 0x20);
                    /* WARNING: Ignoring partial resolution of indirect */
  if (in_FPSCR_PR == '\0') {
    fVar14 = SUB84(param_2,0);
    fVar19 = (float)((ulonglong)param_2 >> 0x20);
    fVar13 = fVar18 * fVar26;
    fVar38 = fVar17 * fVar26;
    fVar26 = fVar27 * fVar14 + fVar33 * fVar19 + fVar47 * 1.0 + fVar50 * fVar26;
    fVar27 = fVar20 * fVar14 + fVar28 * fVar19 + fVar35 * 1.0 + fVar13;
    fVar27 = fVar36 * fVar14 + fVar30 * fVar19 + fVar39 * 1.0 + fVar38;
  }
  if (in_FPSCR_PR == '\0') {
    dVar29 = (double)CONCAT44(fVar26,1.0 / fVar26);
    bVar53 = SUB84(dVar34,0) < fVar26;
  }
  else {
    dVar29 = (double)CONCAT44(fVar26,0x3f800000) / (double)CONCAT44(fVar26,0x3f800000);
    fVar26 = (float)((ulonglong)dVar29 >> 0x20);
    bVar53 = dVar34 < dVar29;
  }
  if (in_FPSCR_PR == '\0') {
    bVar54 = fVar26 < SUB84(dVar23,0);
  }
  else {
    bVar54 = dVar29 < dVar23;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)DAT_8c09ec80);
    dVar29 = (double)CONCAT44(SUB84(dVar29,0),*(undefined4 *)DAT_8c09ec84);
    dVar24 = in_xd6;
  }
  else {
    dVar15 = *DAT_8c09ec80;
    dVar24 = *DAT_8c09ec84;
    dVar29 = in_xd6;
  }
  fVar26 = (float)((ulonglong)dVar34 >> 0x20);
                    /* WARNING: Ignoring partial resolution of indirect */
  fVar27 = (float)((ulonglong)dVar15 >> 0x20);
  fVar33 = (float)((ulonglong)dVar24 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    fVar14 = SUB84(dVar41,0);
    fVar19 = (float)((ulonglong)dVar41 >> 0x20);
    fVar13 = fVar18 * fVar26;
    fVar38 = fVar17 * fVar26;
    fVar26 = fVar27 * fVar14 + fVar33 * fVar19 + fVar47 * 1.0 + fVar50 * fVar26;
    fVar13 = fVar20 * fVar14 + fVar28 * fVar19 + fVar35 * 1.0 + fVar13;
    fVar13 = fVar36 * fVar14 + fVar30 * fVar19 + fVar39 * 1.0 + fVar38;
  }
  fVar13 = SUB84(dVar25,0);
  if (in_FPSCR_PR == '\0') {
    dVar15 = (double)CONCAT44(fVar26,1.0 / fVar26);
    bVar55 = fVar13 < fVar26;
  }
  else {
    dVar15 = (double)CONCAT44(fVar26,0x3f800000) / (double)CONCAT44(fVar26,0x3f800000);
    fVar26 = (float)((ulonglong)dVar15 >> 0x20);
    bVar55 = dVar25 < dVar15;
  }
  if (in_FPSCR_PR == '\0') {
    bVar56 = fVar26 < SUB84(dVar29,0);
  }
  else {
    bVar56 = dVar15 < dVar29;
  }
  if (in_FPSCR_SZ == '\0') {
    in_xd10 = (double)CONCAT44(SUB84(dVar15,0),SUB84(dVar15,0));
  }
  fVar26 = (float)((ulonglong)dVar23 >> 0x20);
                    /* WARNING: Ignoring partial resolution of indirect */
  if (in_FPSCR_PR == '\0') {
    fVar38 = SUB84(dVar40,0);
    fVar14 = (float)((ulonglong)dVar40 >> 0x20);
    fVar18 = fVar18 * fVar26;
    fVar17 = fVar17 * fVar26;
    fVar26 = fVar27 * fVar38 + fVar33 * fVar14 + fVar47 * 1.0 + fVar50 * fVar26;
    fVar18 = fVar20 * fVar38 + fVar28 * fVar14 + fVar35 * 1.0 + fVar18;
    fVar18 = fVar36 * fVar38 + fVar30 * fVar14 + fVar39 * 1.0 + fVar17;
  }
  if (in_FPSCR_PR == '\0') {
    dVar15 = (double)CONCAT44(fVar26,1.0 / fVar26);
    bVar57 = fVar13 < fVar26;
  }
  else {
    dVar15 = (double)CONCAT44(fVar26,0x3f800000) / (double)CONCAT44(fVar26,0x3f800000);
    fVar26 = (float)((ulonglong)dVar15 >> 0x20);
    bVar57 = dVar25 < dVar15;
  }
  if (in_FPSCR_PR == '\0') {
    bVar58 = fVar26 < SUB84(dVar29,0);
  }
  else {
    bVar58 = dVar15 < dVar29;
  }
  bVar7 = ((bVar52 << 1 | bVar54) << 1 | bVar56) << 1 | bVar58;
  dVar23 = dVar49;
  if (in_FPSCR_SZ == '\0') {
    dVar23 = (double)CONCAT44(SUB84(dVar15,0),SUB84(dVar15,0));
  }
  if (((byte)(((bVar51 << 1 | bVar53) << 1 | bVar55) << 1 | bVar57) == 0xf) || (bVar7 == 0xf)) {
    uVar6 = 0;
  }
  else {
    if (in_FPSCR_SZ == '\0') {
      fVar13 = *DAT_8c09ed34;
      dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),fVar13);
      dVar29 = (double)CONCAT44((int)((ulonglong)dVar29 >> 0x20),fVar13);
      in_xd10 = (double)CONCAT44((int)((ulonglong)in_xd10 >> 0x20),*DAT_8c09ed38);
      dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(undefined4 *)DAT_8c09ed3c);
    }
    else {
      dVar49 = *DAT_8c09ed3c;
    }
    if (in_FPSCR_PR == '\0') {
      dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),fVar13 * SUB84(in_xd10,0));
      dVar29 = (double)CONCAT44((int)((ulonglong)dVar29 >> 0x20),SUB84(dVar29,0) * SUB84(dVar23,0));
    }
    else {
      dVar25 = dVar25 * in_xd10;
      dVar29 = dVar29 * dVar23;
    }
    if (in_FPSCR_SZ == '\0') {
      in_xd10 = (double)CONCAT44((int)((ulonglong)in_xd10 >> 0x20),DAT_8c09ed34[3]);
      dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),DAT_8c09ed34[4]);
    }
    else {
      dVar49 = *(double *)(DAT_8c09ed34 + 5);
    }
    fVar17 = (float)((ulonglong)dVar25 >> 0x20);
    fVar18 = SUB84(dVar25,0);
    fVar26 = SUB84(dVar29,0);
    fVar20 = (float)((ulonglong)dVar29 >> 0x20);
    fVar36 = SUB84(in_xd10,0);
    fVar28 = SUB84(dVar23,0);
    fVar27 = (float)((ulonglong)in_xd10 >> 0x20);
    fVar30 = (float)((ulonglong)dVar23 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      dVar34 = (double)CONCAT44((float)((ulonglong)dVar16 >> 0x20) * fVar17 * fVar18 + fVar36,
                                SUB84(dVar16,0) * fVar17 * fVar26 + fVar28);
      dVar16 = (double)CONCAT44((float)((ulonglong)param_2 >> 0x20) * fVar20 * fVar18 + fVar36,
                                SUB84(param_2,0) * fVar20 * fVar26 + fVar28);
      dVar15 = (double)CONCAT44((float)((ulonglong)dVar41 >> 0x20) * fVar27 * fVar18 + fVar36,
                                SUB84(dVar41,0) * fVar27 * fVar26 + fVar28);
      dVar41 = (double)CONCAT44((float)((ulonglong)dVar40 >> 0x20) * fVar30 * fVar18 + fVar36,
                                SUB84(dVar40,0) * fVar30 * fVar26 + fVar28);
    }
    else {
      dVar34 = dVar16 * dVar25 * dVar25 * dVar25 * dVar29 + in_xd10 + dVar23;
      dVar16 = param_2 * dVar29 * dVar29 * dVar25 * dVar29 + in_xd10 + dVar23;
      dVar15 = dVar41 * in_xd10 * in_xd10 * dVar25 * dVar29 + in_xd10 + dVar23;
      dVar41 = dVar40 * dVar23 * dVar23 * dVar25 * dVar29 + in_xd10 + dVar23;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar23 = (double)CONCAT44(fVar30,*(undefined4 *)DAT_8c09ed40);
      dVar29 = (double)CONCAT44(fVar20,*(undefined4 *)((int)DAT_8c09ed40 + 4));
      in_xd10 = (double)CONCAT44(fVar27,*(undefined4 *)(DAT_8c09ed40 + 1));
      dVar25 = (double)CONCAT44(fVar17,*(undefined4 *)((int)DAT_8c09ed40 + 0xc));
    }
    else {
      dVar49 = *DAT_8c09ed40;
    }
    fVar18 = SUB84(dVar34,0);
    fVar17 = SUB84(dVar25,0);
    if (in_FPSCR_PR == '\0') {
      bVar51 = fVar17 < fVar18;
    }
    else {
      bVar51 = dVar25 < dVar34;
    }
    fVar26 = SUB84(dVar29,0);
    if (in_FPSCR_PR == '\0') {
      bVar52 = fVar18 < fVar26;
    }
    else {
      bVar52 = dVar34 < dVar29;
    }
    fVar20 = (float)((ulonglong)dVar34 >> 0x20);
    fVar36 = SUB84(in_xd10,0);
    if (in_FPSCR_PR == '\0') {
      bVar53 = fVar36 < fVar20;
    }
    else {
      bVar53 = in_xd10 < dVar34;
    }
    fVar27 = SUB84(dVar23,0);
    if (in_FPSCR_PR == '\0') {
      bVar54 = fVar20 < fVar27;
    }
    else {
      bVar54 = dVar34 < dVar23;
    }
    fVar28 = SUB84(dVar16,0);
    if (in_FPSCR_PR == '\0') {
      bVar55 = fVar17 < fVar28;
    }
    else {
      bVar55 = dVar25 < dVar16;
    }
    if (in_FPSCR_PR == '\0') {
      bVar56 = fVar28 < fVar26;
    }
    else {
      bVar56 = dVar16 < dVar29;
    }
    fVar30 = (float)((ulonglong)dVar16 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      bVar57 = fVar36 < fVar30;
    }
    else {
      bVar57 = in_xd10 < dVar16;
    }
    if (in_FPSCR_PR == '\0') {
      bVar58 = fVar30 < fVar27;
    }
    else {
      bVar58 = dVar16 < dVar23;
    }
    fVar33 = SUB84(dVar15,0);
    if (in_FPSCR_PR == '\0') {
      bVar59 = fVar17 < fVar33;
    }
    else {
      bVar59 = dVar25 < dVar15;
    }
    if (in_FPSCR_PR == '\0') {
      bVar60 = fVar33 < fVar26;
    }
    else {
      bVar60 = dVar15 < dVar29;
    }
    fVar35 = (float)((ulonglong)dVar15 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      bVar61 = fVar36 < fVar35;
    }
    else {
      bVar61 = in_xd10 < dVar15;
    }
    if (in_FPSCR_PR == '\0') {
      bVar62 = fVar35 < fVar27;
    }
    else {
      bVar62 = dVar15 < dVar23;
    }
    fVar39 = SUB84(dVar41,0);
    if (in_FPSCR_PR == '\0') {
      bVar63 = fVar17 < fVar39;
    }
    else {
      bVar63 = dVar25 < dVar41;
    }
    if (in_FPSCR_PR == '\0') {
      bVar64 = fVar39 < fVar26;
    }
    else {
      bVar64 = dVar41 < dVar29;
    }
    fVar17 = (float)((ulonglong)dVar41 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      bVar65 = fVar36 < fVar17;
    }
    else {
      bVar65 = in_xd10 < dVar41;
    }
    if (in_FPSCR_PR == '\0') {
      bVar66 = fVar17 < fVar27;
    }
    else {
      bVar66 = dVar41 < dVar23;
    }
    if ((DAT_8c09ed44 &
         ((((DAT_8c09ed48 << 1 | (uint)bVar51) << 1 | (uint)bVar52) << 1 | (uint)bVar53) << 1 |
         (uint)bVar54) &
         ((((DAT_8c09ed48 << 1 | (uint)bVar55) << 1 | (uint)bVar56) << 1 | (uint)bVar57) << 1 |
         (uint)bVar58) &
         ((((DAT_8c09ed48 << 1 | (uint)bVar59) << 1 | (uint)bVar60) << 1 | (uint)bVar61) << 1 |
         (uint)bVar62) &
        ((((DAT_8c09ed48 << 1 | (uint)bVar63) << 1 | (uint)bVar64) << 1 | (uint)bVar65) << 1 |
        (uint)bVar66)) == 0) {
      uVar22 = (undefined4)((ulonglong)dVar25 >> 0x20);
      uVar32 = (undefined4)((ulonglong)dVar29 >> 0x20);
      uVar37 = (undefined4)((ulonglong)in_xd10 >> 0x20);
      uVar42 = (undefined4)((ulonglong)dVar23 >> 0x20);
      if (bVar7 == 0) {
        if (in_FPSCR_SZ == '\0') {
          uVar21 = *(undefined4 *)((int)pdVar10 + 0x1c);
          uVar31 = *(undefined4 *)(pdVar10 + 3);
          dVar29 = (double)CONCAT44(uVar32,uVar31);
          *(undefined4 *)(in_r2 + 0x18) = 0x3f800000;
          *(undefined4 *)(in_r2 + 0x14) = uVar31;
          *(undefined4 *)(in_r2 + 0x10) = uVar21;
          *(undefined4 *)(in_r2 + 0xc) = uVar22;
          *(float *)(in_r2 + 8) = fVar18;
          pdVar9 = (double *)(in_r2 + 4);
          *(float *)pdVar9 = fVar20;
        }
        else {
          uVar43 = *(undefined8 *)((int)pdVar10 + 0x1c);
          dVar23 = pdVar10[3];
          *(double *)(in_r2 + 0x14) = dVar49;
          *(double *)(in_r2 + 0xc) = dVar23;
          *(undefined8 *)(in_r2 + 4) = uVar43;
          *(double *)(in_r2 + -4) = dVar25;
          *(double *)(in_r2 + -0xc) = dVar44;
          pdVar9 = (double *)(in_r2 + -0x14);
          *pdVar9 = dVar34;
        }
        *(undefined4 *)((int)pdVar9 + -4) = uVar6;
        if (in_FPSCR_SZ == '\0') {
          uVar22 = *(undefined4 *)((int)pdVar10 + 0x14);
          uVar32 = *(undefined4 *)(pdVar10 + 2);
          *(undefined4 *)((int)pdVar9 + 0x34) = 0x3f800000;
          *(undefined4 *)(pdVar9 + 6) = uVar32;
          *(undefined4 *)((int)pdVar9 + 0x2c) = uVar22;
          *(int *)(pdVar9 + 5) = (int)((ulonglong)dVar29 >> 0x20);
          *(float *)((int)pdVar9 + 0x24) = fVar28;
          pdVar9 = pdVar9 + 4;
          *(float *)pdVar9 = fVar30;
        }
        else {
          dVar44 = *(double *)((int)pdVar10 + 0x14);
          dVar23 = pdVar10[2];
          pdVar9[6] = dVar49;
          pdVar9[5] = dVar23;
          pdVar9[4] = dVar44;
          pdVar9[3] = dVar29;
          pdVar9[2] = dVar45;
          pdVar9 = pdVar9 + 1;
          *pdVar9 = dVar16;
        }
        *(undefined4 *)((int)pdVar9 + -4) = uVar6;
        if (in_FPSCR_SZ == '\0') {
          uVar22 = *(undefined4 *)((int)pdVar10 + 0xc);
          uVar32 = *(undefined4 *)(pdVar10 + 1);
          *(undefined4 *)((int)pdVar9 + 0x34) = 0x3f800000;
          *(undefined4 *)(pdVar9 + 6) = uVar32;
          *(undefined4 *)((int)pdVar9 + 0x2c) = uVar22;
          *(undefined4 *)(pdVar9 + 5) = uVar37;
          *(float *)((int)pdVar9 + 0x24) = fVar33;
          pdVar9 = pdVar9 + 4;
          *(float *)pdVar9 = fVar35;
        }
        else {
          dVar44 = *(double *)((int)pdVar10 + 0xc);
          dVar23 = pdVar10[1];
          pdVar9[6] = dVar49;
          pdVar9[5] = dVar23;
          pdVar9[4] = dVar44;
          pdVar9[3] = in_xd10;
          pdVar9[2] = dVar46;
          pdVar9 = pdVar9 + 1;
          *pdVar9 = dVar15;
        }
        *(undefined4 *)((int)pdVar9 + -4) = uVar6;
        if (in_FPSCR_SZ == '\0') {
          uVar6 = *(undefined4 *)((int)pdVar10 + 4);
          uVar22 = *(undefined4 *)pdVar10;
          *(undefined4 *)((int)pdVar9 + 0x34) = 0x3f800000;
          *(undefined4 *)(pdVar9 + 6) = uVar22;
          *(undefined4 *)((int)pdVar9 + 0x2c) = uVar6;
          *(undefined4 *)(pdVar9 + 5) = uVar42;
          *(float *)((int)pdVar9 + 0x24) = fVar39;
          pdVar9 = pdVar9 + 4;
          *(float *)pdVar9 = fVar17;
        }
        else {
          dVar44 = *(double *)((int)pdVar10 + 4);
          dVar15 = *pdVar10;
          pdVar9[6] = dVar49;
          pdVar9[5] = dVar15;
          pdVar9[4] = dVar44;
          pdVar9[3] = (double)CONCAT44(uVar42,0x3f800000);
          pdVar9[2] = dVar48;
          pdVar9 = pdVar9 + 1;
          *pdVar9 = dVar41;
        }
        *(undefined4 *)((int)pdVar9 + -4) = uVar4;
        uVar6 = 1;
      }
      else {
        if (in_FPSCR_SZ == '\0') {
          uVar6 = *(undefined4 *)((int)pdVar10 + 4);
          *(undefined4 *)((int)puVar11 + -0x34) = *(undefined4 *)pdVar10;
          *(undefined4 *)((int)puVar11 + -0x38) = uVar6;
          *(undefined4 *)((int)puVar11 + -0x3c) = uVar42;
          *(float *)((int)puVar11 + -0x40) = fVar39;
          *(float *)((int)puVar11 + -0x44) = fVar17;
          uVar6 = *(undefined4 *)((int)pdVar10 + 0xc);
          *(undefined4 *)((int)puVar11 + -0x54) = *(undefined4 *)(pdVar10 + 1);
          *(undefined4 *)((int)puVar11 + -0x58) = uVar6;
          *(undefined4 *)((int)puVar11 + -0x5c) = uVar37;
          *(float *)((int)puVar11 + -0x60) = fVar33;
          *(float *)((int)puVar11 + -100) = fVar35;
          uVar6 = *(undefined4 *)((int)pdVar10 + 0x14);
          *(undefined4 *)((int)puVar11 + -0x74) = *(undefined4 *)(pdVar10 + 2);
          *(undefined4 *)((int)puVar11 + -0x78) = uVar6;
          *(undefined4 *)((int)puVar11 + -0x7c) = uVar32;
          *(float *)((int)puVar11 + -0x80) = fVar28;
          *(float *)((int)puVar11 + -0x84) = fVar30;
          uVar6 = *(undefined4 *)((int)pdVar10 + 0x1c);
          *(undefined4 *)((int)puVar11 + -0x94) = *(undefined4 *)(pdVar10 + 3);
          *(undefined4 *)((int)puVar11 + -0x98) = uVar6;
          *(undefined4 *)((int)puVar11 + -0x9c) = uVar22;
          *(float *)((int)puVar11 + -0xa0) = fVar18;
          puVar12 = (undefined1 *)((int)puVar11 + -0xa4);
          *(float *)((int)puVar11 + -0xa4) = fVar20;
        }
        else {
          dVar49 = pdVar10[1];
          *(double *)((int)puVar11 + -0x38) = *pdVar10;
          *(double *)((int)puVar11 + -0x40) = dVar49;
          *(double *)((int)puVar11 + -0x48) = dVar23;
          *(double *)((int)puVar11 + -0x50) = dVar48;
          *(double *)((int)puVar11 + -0x58) = dVar41;
          dVar23 = pdVar10[3];
          *(double *)((int)puVar11 + -0x6c) = pdVar10[2];
          *(double *)((int)puVar11 + -0x74) = dVar23;
          *(double *)((int)puVar11 + -0x7c) = in_xd10;
          *(double *)((int)puVar11 + -0x84) = dVar46;
          *(double *)((int)puVar11 + -0x8c) = dVar15;
          dVar15 = pdVar10[5];
          *(double *)((int)puVar11 + -0xa0) = pdVar10[4];
          *(double *)((int)puVar11 + -0xa8) = dVar15;
          *(double *)((int)puVar11 + -0xb0) = dVar29;
          *(double *)((int)puVar11 + -0xb8) = dVar45;
          *(double *)((int)puVar11 + -0xc0) = dVar16;
          dVar15 = pdVar10[7];
          *(double *)((int)puVar11 + -0xd4) = pdVar10[6];
          *(double *)((int)puVar11 + -0xdc) = dVar15;
          *(double *)((int)puVar11 + -0xe4) = dVar25;
          *(double *)((int)puVar11 + -0xec) = dVar44;
          puVar12 = (undefined1 *)((int)puVar11 + -0xf4);
          *(double *)((int)puVar11 + -0xf4) = dVar34;
        }
        *(undefined4 *)(puVar12 + -8) = in_PR;
        (*(code *)PTR_FUN_8c09ee40)(bVar7,puVar12 + -4);
        uVar6 = 1;
      }
    }
    else {
      uVar6 = 0;
    }
  }
  return uVar6;
}

