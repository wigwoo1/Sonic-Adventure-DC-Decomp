/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c639258) */
/* WARNING: Removing unreachable block (ram,0x8c6391ca) */
/* WARNING: Removing unreachable block (ram,0x8c6397e0) */
/* WARNING: Removing unreachable block (ram,0x8c639572) */
/* WARNING: Removing unreachable block (ram,0x8c639512) */
/* WARNING: Removing unreachable block (ram,0x8c6390bc) */
/* WARNING: Removing unreachable block (ram,0x8c639560) */
/* WARNING: Removing unreachable block (ram,0x8c639606) */
/* WARNING: Removing unreachable block (ram,0x8c639184) */
/* WARNING: Removing unreachable block (ram,0x8c6391dc) */
/* WARNING: Removing unreachable block (ram,0x8c6393fe) */

undefined4
FUN_8c6390b0(undefined4 param_1,float param_2,undefined4 param_3,float param_4,double *param_5)

{
  double *pdVar1;
  double *pdVar2;
  double *pdVar3;
  undefined1 *puVar4;
  undefined1 *puVar5;
  undefined1 *puVar6;
  double *pdVar7;
  float fVar8;
  undefined4 uVar9;
  undefined4 in_fr3;
  undefined4 uVar10;
  double dVar11;
  float fVar13;
  undefined4 uVar14;
  float fVar15;
  float fVar16;
  float fVar17;
  float fVar18;
  undefined4 uVar19;
  double dVar20;
  double dVar21;
  undefined4 uVar22;
  float fVar23;
  float fVar24;
  float fVar25;
  double dVar26;
  undefined4 uVar28;
  float fVar29;
  undefined4 uVar30;
  double dVar31;
  double dVar32;
  float fVar35;
  double dVar33;
  undefined4 uVar36;
  float fVar37;
  undefined4 uVar38;
  double dVar39;
  double dVar40;
  double dVar41;
  undefined4 uVar42;
  float fVar43;
  float fVar44;
  float fVar45;
  undefined8 in_dr12;
  double dVar46;
  undefined8 in_dr14;
  double in_xd0;
  double dVar47;
  double in_xd2;
  double in_xd4;
  double in_xd6;
  double dVar48;
  double in_xd8;
  double dVar49;
  double in_xd10;
  double in_xd12;
  double in_xd14;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  double dStack_18;
  undefined8 uStack_10;
  float fVar12;
  double dVar27;
  undefined4 uVar34;
  
  fVar8 = (float)(*(code *)PTR_FUN_8c6390e4)();
  if (fVar8 == 0.0) {
    uVar34 = uRam8c6390ec;
    if (in_FPSCR_SZ == '\0') {
      uVar34 = DAT_8c6390e8;
    }
    dVar47 = (double)CONCAT44(uVar34,DAT_8c6390e8);
    if (param_5 == (double *)0x0) {
      uVar34 = 0;
    }
    else {
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)((int)param_5 + 0x3c) = uVar34;
        *(undefined4 *)(param_5 + 7) = uVar34;
        *(undefined4 *)((int)param_5 + 0x34) = uVar34;
        *(undefined4 *)(param_5 + 6) = uVar34;
        *(undefined4 *)((int)param_5 + 0x2c) = uVar34;
        *(undefined4 *)(param_5 + 5) = uVar34;
        *(undefined4 *)((int)param_5 + 0x24) = uVar34;
        *(undefined4 *)(param_5 + 4) = uVar34;
        *(undefined4 *)((int)param_5 + 0x1c) = uVar34;
        *(undefined4 *)(param_5 + 3) = uVar34;
        *(undefined4 *)((int)param_5 + 0x14) = uVar34;
        *(undefined4 *)(param_5 + 2) = uVar34;
        *(undefined4 *)((int)param_5 + 0xc) = uVar34;
        *(undefined4 *)(param_5 + 1) = uVar34;
        *(undefined4 *)((int)param_5 + 4) = uVar34;
        *(undefined4 *)param_5 = uVar34;
      }
      else {
        param_5[7] = dVar47;
        param_5[6] = dVar47;
        param_5[5] = dVar47;
        param_5[4] = dVar47;
        param_5[3] = dVar47;
        param_5[2] = dVar47;
        param_5[1] = dVar47;
        *param_5 = dVar47;
        param_5[-1] = dVar47;
        param_5[-2] = dVar47;
        param_5[-3] = dVar47;
        param_5[-4] = dVar47;
        param_5[-5] = dVar47;
        param_5[-6] = dVar47;
        param_5[-7] = dVar47;
        param_5[-8] = dVar47;
      }
      uVar34 = 0;
    }
  }
  else {
    if (in_FPSCR_PR == '\0') {
      fVar8 = 1.0 / fVar8;
    }
    else {
      fVar8 = SUB84((double)CONCAT44(fVar8,0x3f800000) / (double)CONCAT44(fVar8,0x3f800000),0);
    }
    if (in_FPSCR_SZ == '\0') {
      uStack_10 = CONCAT44((int)((ulonglong)in_dr12 >> 0x20),(undefined4)uStack_10);
      pdVar3 = (double *)&uStack_10;
    }
    else {
      uStack_10 = in_dr12;
      pdVar3 = &dStack_18;
      dStack_18 = in_xd12;
    }
    if (in_FPSCR_SZ == '\0') {
      puVar4 = (undefined1 *)((int)pdVar3 + -4);
      *(int *)((int)pdVar3 + -4) = (int)((ulonglong)in_dr14 >> 0x20);
    }
    else {
      puVar4 = (undefined1 *)((int)pdVar3 + -8);
      *(undefined8 *)((int)pdVar3 + -8) = in_dr14;
    }
    if (param_5 == (double *)0x0) {
      fVar24 = (float)in_dr14;
      fVar25 = SUB84(in_xd12,0);
      fVar35 = SUB84(in_xd14,0);
      fVar12 = (float)((ulonglong)in_xd10 >> 0x20);
      dVar11 = in_xd10;
      dVar47 = in_xd0;
      dVar41 = in_xd6;
      fVar13 = fVar35;
      dVar31 = in_xd4;
      dVar26 = in_xd14;
      dVar27 = in_xd10;
      if (in_FPSCR_SZ == '\0') {
        dVar11 = (double)CONCAT44(param_4,param_1);
        dVar47 = (double)CONCAT44(fVar8,fVar8);
        fVar12 = param_4;
        dVar41 = (double)CONCAT44(param_2,param_4);
        fVar13 = param_2;
        dVar31 = (double)CONCAT44(param_1,param_3);
        dVar26 = (double)CONCAT44(fVar24,param_2);
        dVar27 = (double)CONCAT44(param_4,in_fr3);
      }
      fVar16 = (float)((ulonglong)dVar26 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar20 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) * fVar13,
                                  SUB84(dVar11,0) * SUB84(dVar27,0));
        dVar47 = (double)CONCAT44((float)((ulonglong)dVar47 >> 0x20) * fVar13,
                                  SUB84(dVar47,0) * fVar16);
        dVar26 = (double)CONCAT44(fVar12 * SUB84(dVar11,0),SUB84(dVar27,0) * fVar16);
      }
      else {
        dVar20 = dVar11 * dVar26 * dVar27 * dVar11;
        dVar47 = dVar47 * dVar26 * dVar26;
        dVar26 = dVar27 * dVar11 * dVar26;
      }
      fVar12 = (float)((ulonglong)dVar41 >> 0x20);
      dVar27 = in_xd8;
      dVar11 = dVar41;
      if (in_FPSCR_SZ == '\0') {
        dVar27 = (double)CONCAT44(SUB84(dVar31,0),fVar12);
        dVar11 = in_xd6;
      }
      fVar18 = (float)((ulonglong)dVar20 >> 0x20);
      fVar16 = (float)((ulonglong)dVar47 >> 0x20);
      fVar13 = SUB84(dVar26,0);
      if (in_FPSCR_PR == '\0') {
        fVar45 = SUB84(dVar27,0) * SUB84(dVar20,0);
        dVar41 = (double)CONCAT44(fVar12 * fVar16,SUB84(dVar41,0));
        dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) * fVar18 + fVar45,fVar45);
        dVar31 = (double)CONCAT44((int)((ulonglong)dVar31 >> 0x20),SUB84(dVar31,0) * fVar13);
      }
      else {
        dVar27 = dVar27 * dVar20 * dVar20;
        dVar41 = dVar41 * dVar47;
        dVar27 = dVar27 + dVar27;
        dVar31 = dVar31 * dVar26;
      }
      dVar48 = in_xd10;
      if (in_FPSCR_SZ == '\0') {
        dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),SUB84(dVar41,0));
        dVar48 = dVar11;
      }
      fVar12 = (float)((ulonglong)dVar26 >> 0x20);
      fVar45 = SUB84(dVar31,0);
      if (in_FPSCR_PR == '\0') {
        fVar17 = SUB84(dVar41,0) * SUB84(dVar47,0);
        fVar23 = SUB84(dVar27,0) * fVar12;
        dVar31 = (double)CONCAT44(((((float)((ulonglong)dVar27 >> 0x20) + fVar17) - fVar23) -
                                  (float)((ulonglong)dVar41 >> 0x20)) - fVar45,fVar23);
      }
      else {
        dVar41 = dVar41 * dVar47;
        fVar17 = SUB84(dVar41,0);
        dVar27 = dVar27 * dVar26 + dVar41;
        dVar31 = ((dVar27 - dVar27) - dVar41) - dVar31;
      }
      dVar27 = in_xd8;
      dVar41 = in_xd2;
      if (in_FPSCR_SZ == '\0') {
        dVar27 = (double)CONCAT44(fVar45,fVar45);
        dVar41 = (double)CONCAT44(fVar13,fVar17);
      }
      fVar17 = (float)((ulonglong)dVar41 >> 0x20);
      fVar45 = SUB84(dVar41,0);
      if (in_FPSCR_PR == '\0') {
        fVar23 = SUB84(dVar27,0) * fVar13;
        fVar15 = SUB84(dVar20,0) * fVar17 + (float)((ulonglong)dVar27 >> 0x20) * fVar18;
        dVar27 = (double)CONCAT44(fVar18,fVar15);
        fVar12 = (((SUB84(dVar47,0) * fVar45 + fVar15) - fVar12 * fVar45) - fVar16 * fVar17) -
                 fVar23;
      }
      else {
        dVar26 = dVar26 * dVar41;
        fVar13 = SUB84(dVar26,0);
        dVar11 = dVar27 * dVar20 * dVar41 * dVar26;
        dVar27 = dVar20 * dVar41 + dVar11;
        dVar26 = (dVar47 * dVar41 * dVar41 + dVar27) - dVar26;
        fVar23 = SUB84(dVar11,0);
        fVar12 = SUB84((dVar26 - dVar26) - dVar11,0);
      }
      dVar47 = dVar31;
      if (in_FPSCR_SZ == '\0') {
        dVar47 = (double)CONCAT44((int)((ulonglong)dVar31 >> 0x20),fVar12);
      }
      fVar16 = (float)((ulonglong)dVar47 >> 0x20);
      fVar12 = -SUB84(dVar47,0);
      dVar11 = in_xd14;
      dVar47 = in_xd12;
      dVar41 = in_xd10;
      dVar26 = in_xd8;
      if (in_FPSCR_SZ == '\0') {
        dVar11 = (double)CONCAT44(fVar24,(int)in_dr12);
        dVar47 = (double)CONCAT44((int)in_dr12,fVar45);
        dVar41 = (double)CONCAT44(fVar45,fVar23);
        dVar26 = (double)CONCAT44(fVar23,SUB84(dVar31,0));
      }
      fVar45 = (float)((ulonglong)dVar47 >> 0x20);
      fVar18 = (float)((ulonglong)dVar26 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar41 = (double)CONCAT44((int)((ulonglong)dVar41 >> 0x20),SUB84(dVar41,0) * fVar45);
        dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) * fVar18,SUB84(dVar11,0));
      }
      else {
        dVar41 = dVar41 * dVar47;
        dVar11 = dVar11 * dVar26;
      }
      dVar31 = in_xd8;
      if (in_FPSCR_SZ == '\0') {
        dVar31 = (double)CONCAT44(SUB84(dVar41,0),fVar13);
      }
      if (in_FPSCR_PR == '\0') {
        dVar41 = (double)CONCAT44((float)((ulonglong)dVar41 >> 0x20) * fVar45,SUB84(dVar41,0));
      }
      else {
        dVar41 = dVar41 * dVar47;
      }
      dVar20 = dVar11;
      if (in_FPSCR_SZ == '\0') {
        dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),(int)((ulonglong)dVar11 >> 0x20))
        ;
        dVar20 = in_xd4;
      }
      if (in_FPSCR_PR == '\0') {
        dVar26 = (double)CONCAT44(fVar18 * SUB84(dVar11,0),SUB84(dVar26,0));
      }
      else {
        dVar26 = dVar26 * dVar11;
      }
      fVar13 = SUB84(dVar26,0);
      fVar18 = SUB84(dVar27,0);
      dVar21 = dVar20;
      dVar49 = dVar20;
      dVar33 = dVar41;
      dVar32 = dVar41;
      if (in_FPSCR_SZ == '\0') {
        fVar13 = (float)((ulonglong)dVar41 >> 0x20);
        dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),fVar13);
        dVar41 = (double)CONCAT44(fVar13,SUB84(dVar11,0));
        dVar21 = (double)CONCAT44(fVar18,fVar18);
        dVar49 = in_xd8;
        dVar33 = (double)CONCAT44(fVar13,SUB84(dVar47,0));
        dVar32 = dVar48;
      }
      fVar17 = (float)((ulonglong)dVar31 >> 0x20);
      fVar23 = (float)((ulonglong)dVar27 >> 0x20);
      fVar15 = (float)((ulonglong)dVar26 >> 0x20);
      fVar45 = SUB84(dVar31,0);
      if (in_FPSCR_PR == '\0') {
        fVar43 = SUB84(dVar41,0) * fVar18;
        fVar37 = SUB84(dVar33,0) * fVar13;
        fVar29 = (((SUB84(dVar21,0) * fVar23 + (float)((ulonglong)dVar41 >> 0x20) * fVar17 + fVar43)
                  - fVar37) - (float)((ulonglong)dVar33 >> 0x20) * fVar15) -
                 (float)((ulonglong)dVar21 >> 0x20) * fVar45;
      }
      else {
        dVar41 = dVar41 * dVar31 * dVar27;
        dVar47 = dVar33 * dVar26 * dVar26;
        fVar43 = SUB84(dVar41,0);
        fVar37 = SUB84(dVar47,0);
        dVar47 = ((dVar21 * dVar27 * dVar31 + dVar41 + dVar41) - dVar47) - dVar47;
        fVar29 = SUB84(dVar47 - dVar47,0);
      }
      dVar41 = in_xd2;
      dVar47 = in_xd0;
      if (in_FPSCR_SZ == '\0') {
        dVar41 = (double)CONCAT44(fVar45,fVar43);
        dVar47 = (double)CONCAT44(fVar12,fVar37);
      }
      fVar29 = -fVar29;
      fVar37 = (float)((ulonglong)dVar47 >> 0x20);
      fVar44 = (float)((ulonglong)dVar41 >> 0x20);
      fVar43 = SUB84(dVar41,0);
      if (in_FPSCR_PR == '\0') {
        fVar17 = fVar17 * fVar44;
        dVar26 = (double)CONCAT44(fVar15 * fVar44,fVar13 * fVar43);
        dVar31 = (double)CONCAT44(fVar17,fVar45 * fVar37);
        dVar41 = (double)CONCAT44(fVar23 * fVar37 + fVar17 + fVar18 * fVar43,fVar18 * fVar43);
      }
      else {
        dVar26 = dVar26 * dVar41 * dVar41;
        dVar31 = dVar31 * dVar41 * dVar47;
        dVar41 = dVar27 * dVar47 * dVar41 + dVar31;
        dVar41 = dVar41 + dVar41;
      }
      fVar13 = SUB84(dVar26,0);
      if (in_FPSCR_PR == '\0') {
        dVar41 = (double)CONCAT44((float)((ulonglong)dVar41 >> 0x20) - fVar13,SUB84(dVar41,0));
      }
      else {
        dVar41 = dVar41 - dVar26;
      }
      dVar27 = dVar49;
      if (in_FPSCR_SZ == '\0') {
        dVar27 = (double)CONCAT44(fVar29,-SUB84(dVar31,0));
      }
      if (in_FPSCR_PR == '\0') {
        fVar18 = (float)((ulonglong)dVar41 >> 0x20) - (float)((ulonglong)dVar26 >> 0x20);
        dVar41 = (double)CONCAT44(fVar18,SUB84(dVar41,0));
        dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),SUB84(dVar27,0) + fVar18);
      }
      else {
        dVar41 = dVar41 - dVar26;
        dVar27 = dVar27 + dVar41;
      }
      fVar18 = SUB84(dVar41,0);
      dVar41 = (double)CONCAT44(fVar8,fVar18);
      if (in_FPSCR_PR == '\0') {
        dVar31 = (double)CONCAT44(fVar16 * fVar8,fVar12 * fVar8);
        dVar41 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) * fVar8,SUB84(dVar27,0) * fVar8
                                 );
      }
      else {
        dVar31 = (double)CONCAT44(fVar16,fVar12) * dVar41 * dVar41;
        dVar41 = dVar27 * dVar41 * dVar41;
      }
      fVar16 = SUB84(dVar31,0);
      fVar12 = SUB84(dVar49,0);
      if (in_FPSCR_SZ == '\0') {
        *(int *)(puVar4 + -4) = (int)((ulonglong)dVar31 >> 0x20);
        *(float *)(puVar4 + -8) = fVar16;
        *(int *)(puVar4 + -0xc) = (int)((ulonglong)dVar41 >> 0x20);
        puVar5 = puVar4 + -0x10;
        *(int *)(puVar4 + -0x10) = SUB84(dVar41,0);
        dVar11 = (double)CONCAT44(fVar18,fVar13);
        dVar41 = (double)CONCAT44(fVar16,SUB84(dVar41,0));
        dVar31 = (double)CONCAT44(fVar13,SUB84(dVar47,0));
        dVar26 = (double)CONCAT44(fVar18,fVar29);
        fVar25 = fVar18;
        dVar47 = (double)CONCAT44(fVar43,fVar18);
        dVar27 = (double)CONCAT44(fVar29,fVar16);
      }
      else {
        *(double *)(puVar4 + -8) = dVar31;
        *(double *)(puVar4 + -0x10) = in_xd0;
        *(double *)(puVar4 + -0x18) = dVar41;
        puVar5 = puVar4 + -0x20;
        *(double *)(puVar4 + -0x20) = in_xd2;
        dVar11 = dVar20;
        dVar41 = in_xd0;
        fVar16 = fVar12;
        dVar31 = dVar32;
        dVar26 = dVar20;
        dVar47 = in_xd12;
        dVar27 = dVar49;
      }
      if (in_FPSCR_PR == '\0') {
        fVar18 = (float)((ulonglong)dVar27 >> 0x20);
        fVar13 = SUB84(dVar11,0) * fVar18;
        dVar27 = (double)CONCAT44(fVar18 * fVar25,fVar16 * SUB84(dVar11,0));
        fVar16 = (float)((ulonglong)dVar47 >> 0x20);
        dVar47 = (double)CONCAT44(fVar16,fVar25 * SUB84(dVar41,0));
        dVar41 = (double)CONCAT44((float)((ulonglong)dVar41 >> 0x20) * fVar16,
                                  SUB84(dVar41,0) * fVar16);
      }
      else {
        fVar13 = SUB84(dVar11 * dVar27 * dVar11,0);
        dVar27 = dVar27 * dVar11 * dVar47;
        dVar47 = dVar47 * dVar41;
        dVar41 = dVar41 * dVar47 * dVar47;
      }
      fVar25 = (float)((ulonglong)dVar26 >> 0x20);
      dVar48 = dVar49;
      dVar11 = in_xd10;
      if (in_FPSCR_SZ == '\0') {
        dVar48 = dVar32;
        dVar11 = (double)CONCAT44(SUB84(dVar31,0),fVar43);
        dVar32 = (double)CONCAT44(fVar13,SUB84(dVar47,0));
      }
      fVar16 = SUB84(dVar27,0);
      fVar18 = (float)((ulonglong)dVar27 >> 0x20);
      fVar13 = (float)((ulonglong)dVar41 >> 0x20);
      fVar45 = (float)((ulonglong)dVar32 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar23 = SUB84(dVar26,0) * SUB84(dVar41,0);
        fVar29 = (float)((ulonglong)dVar11 >> 0x20) * fVar18;
        dVar11 = (double)CONCAT44(fVar29,SUB84(dVar11,0));
        fVar15 = SUB84(dVar31,0) * fVar13;
        dVar31 = (double)(ulonglong)(uint)fVar15;
        fVar17 = SUB84(dVar26,0) * fVar45;
        dVar47 = (double)CONCAT44((((fVar25 * fVar16 + fVar23 + fVar29) - fVar15) - fVar17) -
                                  fVar25 * SUB84(dVar32,0),fVar23);
      }
      else {
        dVar33 = dVar26 * dVar27 * dVar41;
        dVar11 = dVar11 * dVar27;
        dVar31 = dVar31 * dVar41;
        dVar47 = dVar26 * dVar32 * dVar32;
        fVar17 = SUB84(dVar47,0);
        dVar47 = (((dVar33 + dVar33 + dVar11) - dVar31) - dVar47) - dVar47;
      }
      dVar33 = in_xd2;
      dVar26 = in_xd0;
      if (in_FPSCR_SZ == '\0') {
        dVar33 = (double)((ulonglong)dVar31 & 0xffffffff);
        dVar26 = (double)CONCAT44(fVar16,fVar17);
      }
      fVar25 = (float)((ulonglong)dVar26 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar27 = (double)CONCAT44(fVar18,fVar16 * fVar25);
      }
      else {
        dVar27 = dVar27 * dVar26;
      }
      dVar31 = in_xd10;
      if (in_FPSCR_SZ == '\0') {
        dVar31 = (double)CONCAT44(SUB84(dVar33,0),SUB84(dVar33,0));
      }
      fVar18 = SUB84(dVar26,0);
      fVar16 = (float)((ulonglong)dVar27 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar23 = SUB84(dVar41,0) * fVar18;
        dVar41 = (double)CONCAT44(fVar13,fVar23);
        fVar17 = SUB84(dVar31,0) * fVar13;
        fVar25 = SUB84(dVar32,0) * fVar25;
        dVar26 = (double)CONCAT44(fVar16,(((SUB84(dVar27,0) + fVar23 +
                                           (float)((ulonglong)dVar31 >> 0x20) * fVar16) - fVar17) -
                                         fVar45 * fVar18) - fVar25);
      }
      else {
        dVar41 = dVar41 * dVar26;
        fVar13 = (float)((ulonglong)dVar41 >> 0x20);
        dVar31 = dVar31 * dVar27 * dVar41;
        fVar17 = SUB84(dVar31,0);
        dVar26 = dVar32 * dVar26 * dVar26;
        fVar25 = SUB84(dVar26,0);
        dVar26 = (((dVar27 + dVar41 + dVar31) - dVar31) - dVar26) - dVar26;
      }
      dVar31 = dVar47;
      if (in_FPSCR_SZ == '\0') {
        dVar41 = (double)CONCAT44(fVar13,(int)((ulonglong)dVar47 >> 0x20));
        dVar31 = in_xd2;
      }
      fVar16 = (float)((ulonglong)dVar26 >> 0x20);
      fVar13 = -SUB84(dVar26,0);
      dVar33 = in_xd14;
      dVar26 = in_xd12;
      dVar27 = in_xd10;
      dVar32 = dVar49;
      if (in_FPSCR_SZ == '\0') {
        dVar33 = (double)CONCAT44(fVar24,fVar17);
        dVar26 = (double)CONCAT44(SUB84(dVar11,0),fVar18);
        dVar27 = (double)CONCAT44(fVar17,SUB84(dVar47,0));
        dVar32 = (double)CONCAT44(fVar18,fVar25);
      }
      fVar18 = (float)((ulonglong)dVar33 >> 0x20);
      fVar25 = (float)((ulonglong)dVar27 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar26 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) * fVar25,SUB84(dVar26,0));
        dVar33 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) * fVar18,
                                  SUB84(dVar32,0) * fVar18);
      }
      else {
        dVar26 = dVar26 * dVar27;
        dVar33 = dVar32 * dVar33 * dVar33;
      }
      dVar47 = dVar26;
      if (in_FPSCR_SZ == '\0') {
        dVar27 = (double)CONCAT44(fVar25,(int)((ulonglong)dVar26 >> 0x20));
        dVar47 = dVar48;
      }
      fVar25 = SUB84(dVar26,0);
      if (in_FPSCR_PR == '\0') {
        dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) * fVar25,SUB84(dVar27,0));
      }
      else {
        dVar27 = dVar27 * dVar26;
      }
      fVar23 = SUB84(dVar33,0);
      fVar15 = SUB84(dVar27,0);
      fVar45 = (float)((ulonglong)dVar47 >> 0x20);
      fVar18 = SUB84(dVar20,0);
      fVar17 = (float)((ulonglong)dVar20 >> 0x20);
      dVar21 = dVar49;
      dVar32 = dVar47;
      dVar26 = dVar47;
      dVar11 = dVar20;
      dVar48 = dVar20;
      if (in_FPSCR_SZ == '\0') {
        dVar21 = (double)CONCAT44(fVar25,fVar15);
        dVar32 = in_xd12;
        dVar26 = (double)CONCAT44(fVar15,fVar23);
        dVar11 = in_xd10;
        fVar45 = fVar15;
        fVar18 = fVar25;
        fVar17 = fVar23;
        dVar48 = (double)CONCAT44(fVar23,fVar25);
      }
      fVar25 = (float)((ulonglong)dVar41 >> 0x20);
      fVar29 = (float)((ulonglong)dVar33 >> 0x20);
      fVar37 = (float)((ulonglong)dVar27 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar43 = SUB84(dVar26,0) * fVar23;
        fVar44 = SUB84(dVar21,0) * fVar16;
        fVar45 = fVar45 * fVar25;
        dVar39 = (double)CONCAT44(fVar45,fVar43);
        fVar18 = fVar18 * fVar29;
        dVar26 = (double)CONCAT44(-(((((float)((ulonglong)dVar21 >> 0x20) * fVar15 + fVar43 + fVar44
                                      ) - fVar45) - fVar18) - fVar17 * fVar37),fVar44);
      }
      else {
        dVar21 = dVar21 * dVar27 * (double)CONCAT44(fVar16,fVar13) + dVar26 * dVar33;
        dVar39 = dVar26 * dVar33 * dVar41;
        dVar26 = dVar48 * dVar33 * dVar27;
        fVar18 = SUB84(dVar26,0);
        dVar26 = -((((dVar21 + dVar21) - dVar39) - dVar26) - dVar26);
      }
      dVar21 = dVar31;
      dVar48 = in_xd0;
      if (in_FPSCR_SZ == '\0') {
        dVar21 = (double)CONCAT44(SUB84(dVar41,0),SUB84(dVar39,0));
        dVar48 = (double)CONCAT44(fVar13,fVar18);
      }
      fVar45 = (float)((ulonglong)dVar48 >> 0x20);
      fVar18 = SUB84(dVar48,0);
      fVar17 = (float)((ulonglong)dVar21 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar23 = fVar23 * fVar45 + fVar15 * fVar18;
        fVar25 = fVar25 * fVar17;
        dVar41 = (double)CONCAT44(fVar25,SUB84(dVar41,0));
        fVar29 = fVar29 * fVar18;
        dVar33 = (double)CONCAT44(fVar29,fVar23);
        fVar37 = fVar37 * fVar45;
        dVar48 = (double)CONCAT44(fVar37,fVar15 * fVar18);
        dVar27 = (double)CONCAT44((((fVar16 * fVar17 + fVar23) - fVar25) - fVar29) - fVar37,fVar13);
      }
      else {
        dVar39 = dVar33 * dVar48 + dVar27 * dVar48;
        dVar41 = dVar41 * dVar21;
        dVar33 = dVar39 * dVar48;
        dVar48 = dVar27 * dVar48 * dVar48;
        dVar27 = ((((double)CONCAT44(fVar16,fVar13) * dVar21 + dVar39) - dVar41) - dVar33) - dVar48;
      }
      dVar21 = (double)CONCAT44(fVar8,fVar18);
      if (in_FPSCR_PR == '\0') {
        fVar13 = SUB84(dVar41,0) * fVar8;
        dVar26 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) * fVar8,SUB84(dVar26,0));
        dVar21 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) * fVar8,SUB84(dVar27,0) * fVar8
                                 );
      }
      else {
        fVar13 = SUB84(dVar41 * dVar21,0);
        dVar26 = dVar26 * dVar21;
        dVar21 = dVar27 * dVar21 * dVar21;
      }
      uVar34 = SUB84(dVar21,0);
      if (in_FPSCR_SZ == '\0') {
        *(float *)(puVar5 + -4) = fVar13;
        *(undefined4 *)(puVar5 + -8) = uVar34;
        *(int *)(puVar5 + -0xc) = (int)((ulonglong)dVar26 >> 0x20);
        puVar6 = puVar5 + -0x10;
        *(int *)(puVar5 + -0x10) = (int)((ulonglong)dVar21 >> 0x20);
        fVar25 = SUB84(dVar48,0);
        dVar33 = (double)CONCAT44(fVar25,SUB84(dVar33,0));
        dVar41 = (double)CONCAT44(uVar34,uVar34);
        dVar27 = (double)CONCAT44(fVar25,fVar25);
        dVar48 = (double)CONCAT44(fVar13,fVar13);
      }
      else {
        *(double *)(puVar5 + -8) = dVar31;
        *(double *)(puVar5 + -0x10) = in_xd0;
        *(double *)(puVar5 + -0x18) = dVar26;
        puVar6 = puVar5 + -0x20;
        *(double *)(puVar5 + -0x20) = dVar21;
        fVar25 = SUB84(dVar47,0);
        dVar33 = dVar47;
        dVar41 = in_xd0;
        dVar27 = dVar47;
        dVar48 = dVar31;
      }
      fVar16 = (float)((ulonglong)dVar27 >> 0x20);
      fVar13 = (float)((ulonglong)dVar48 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar39 = (double)CONCAT44((float)((ulonglong)dVar41 >> 0x20) * fVar16,
                                  SUB84(dVar41,0) * fVar25);
        dVar21 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) * fVar13,
                                  SUB84(dVar33,0) * SUB84(dVar48,0));
        dVar41 = (double)CONCAT44(fVar13 * SUB84(dVar33,0),SUB84(dVar48,0) * fVar16);
      }
      else {
        dVar39 = dVar41 * dVar27 * dVar27;
        dVar41 = dVar48 * dVar33 * dVar48;
        dVar21 = dVar33 * dVar48 * dVar41;
        dVar41 = dVar41 * dVar27;
      }
      fVar13 = (float)((ulonglong)in_xd14 >> 0x20);
      dVar40 = dVar47;
      dVar46 = in_xd14;
      dVar48 = dVar49;
      dVar33 = in_xd14;
      if (in_FPSCR_SZ == '\0') {
        fVar12 = SUB84(dVar27,0);
        dVar40 = (double)CONCAT44(fVar12,fVar24);
        dVar46 = dVar11;
        dVar48 = (double)CONCAT44(fVar18,fVar12);
        fVar13 = fVar24;
        fVar35 = fVar18;
        dVar33 = (double)CONCAT44(fVar24,fVar18);
      }
      fVar25 = (float)((ulonglong)dVar39 >> 0x20);
      fVar45 = (float)((ulonglong)dVar21 >> 0x20);
      fVar16 = SUB84(dVar41,0);
      fVar18 = (float)((ulonglong)dVar41 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar13 = fVar13 * SUB84(dVar21,0);
        fVar35 = fVar35 * fVar18;
        dVar11 = (double)CONCAT44(fVar13,fVar35);
        fVar17 = SUB84(dVar40,0) * SUB84(dVar39,0);
        dVar27 = (double)CONCAT44(((((float)((ulonglong)dVar48 >> 0x20) * fVar25 + fVar12 * fVar45 +
                                    fVar13) - (float)((ulonglong)dVar40 >> 0x20) * fVar16) - fVar35)
                                  - fVar17,fVar12 * fVar45);
      }
      else {
        dVar48 = dVar48 * dVar39 * dVar21;
        dVar11 = dVar33 * dVar21 * dVar41;
        dVar27 = dVar40 * dVar41 * dVar39;
        fVar17 = SUB84(dVar27,0);
        dVar27 = (((dVar48 + dVar48 + dVar33 * dVar21) - dVar40 * dVar41) - dVar11) - dVar27;
      }
      dVar40 = dVar49;
      dVar48 = dVar49;
      dVar33 = dVar46;
      if (in_FPSCR_SZ == '\0') {
        dVar40 = (double)((ulonglong)dVar26 & 0xffffffff);
        dVar48 = (double)CONCAT44(SUB84(dVar11,0),SUB84(dVar11,0));
        dVar33 = (double)CONCAT44(fVar17,fVar17);
      }
      fVar13 = SUB84(dVar33,0);
      fVar12 = (float)((ulonglong)dVar33 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar17 = SUB84(dVar21,0) * fVar12;
        dVar26 = (double)CONCAT44(fVar45,fVar17);
        fVar35 = SUB84(dVar48,0) * fVar16;
        fVar18 = fVar18 * fVar13;
        dVar41 = (double)CONCAT44(fVar18,fVar16);
        fVar12 = SUB84(dVar39,0) * fVar12;
        dVar11 = (double)CONCAT44((((fVar25 * fVar13 + (float)((ulonglong)dVar48 >> 0x20) * fVar45 +
                                    fVar17) - fVar35) - fVar18) - fVar12,fVar12);
      }
      else {
        dVar26 = dVar21 * dVar33;
        dVar11 = dVar48 * dVar21 * dVar41;
        fVar17 = SUB84(dVar26,0);
        dVar41 = dVar41 * dVar33;
        fVar35 = SUB84(dVar11,0);
        dVar11 = ((dVar39 * dVar33 + dVar48 * dVar21 + dVar26) - dVar11) * dVar33 - dVar41;
        dVar11 = dVar11 - dVar11;
      }
      dVar48 = dVar27;
      if (in_FPSCR_SZ == '\0') {
        dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),SUB84(dVar41,0));
        dVar11 = (double)CONCAT44((int)((ulonglong)dVar11 >> 0x20),(int)((ulonglong)dVar27 >> 0x20))
        ;
        dVar48 = in_xd0;
      }
      if (in_FPSCR_PR == '\0') {
        dVar11 = (double)CONCAT44(-(float)((ulonglong)dVar11 >> 0x20),SUB84(dVar11,0));
      }
      else {
        dVar11 = -dVar11;
      }
      fVar12 = SUB84(dVar26,0);
      if (in_FPSCR_SZ == '\0') {
        fVar25 = SUB84(dVar27,0);
        dVar20 = (double)CONCAT44(fVar17,fVar35);
        dVar47 = (double)CONCAT44(fVar25,fVar13);
        dVar41 = (double)CONCAT44(fVar12,fVar25);
        dVar27 = (double)CONCAT44(SUB84(dVar11,0),fVar17);
      }
      else {
        fVar17 = SUB84(dVar48,0);
        fVar25 = SUB84(dVar31,0);
        dVar41 = dVar31;
        dVar27 = dVar48;
      }
      fVar35 = (float)((ulonglong)dVar20 >> 0x20);
      fVar13 = (float)((ulonglong)dVar27 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar20 = (double)CONCAT44((float)((ulonglong)dVar41 >> 0x20) * fVar35,fVar25 * fVar35);
        fVar25 = SUB84(dVar47,0);
        dVar47 = (double)CONCAT44((float)((ulonglong)dVar47 >> 0x20) * fVar13,fVar25);
        dVar27 = (double)CONCAT44(fVar13 * fVar25,fVar17);
      }
      else {
        dVar20 = dVar41 * dVar20 * dVar20;
        dVar47 = dVar47 * dVar27;
        fVar25 = SUB84(dVar47,0);
        dVar27 = dVar27 * dVar47;
      }
      uVar34 = SUB84(dVar40,0);
      dVar41 = dVar32;
      dVar33 = in_xd14;
      if (in_FPSCR_SZ == '\0') {
        dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),(int)((ulonglong)dVar47 >> 0x20))
        ;
        in_xd14 = (double)CONCAT44(fVar24,uVar34);
        dVar32 = (double)CONCAT44(uVar34,uVar34);
        dVar41 = dVar49;
        dVar33 = (double)CONCAT44(fVar24,fVar25);
      }
      fVar13 = (float)((ulonglong)dVar26 >> 0x20);
      fVar35 = (float)((ulonglong)dVar20 >> 0x20);
      fVar25 = (float)((ulonglong)dVar27 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar16 = SUB84(dVar33,0) * SUB84(dVar27,0);
        fVar18 = SUB84(dVar32,0) * fVar12;
        fVar24 = SUB84(in_xd14,0) * fVar35;
        dVar47 = (double)CONCAT44(((((float)((ulonglong)dVar32 >> 0x20) * fVar13 + fVar16 +
                                    (float)((ulonglong)dVar33 >> 0x20) * SUB84(dVar20,0)) - fVar18)
                                  - fVar24) - (float)((ulonglong)in_xd14 >> 0x20) * fVar25,fVar18);
      }
      else {
        dVar33 = dVar33 * dVar27 * dVar20;
        fVar16 = SUB84(dVar33,0);
        dVar33 = (dVar32 * dVar26 + dVar33) * dVar26 + dVar33;
        dVar47 = in_xd14 * dVar20 * dVar27;
        fVar24 = SUB84(dVar47,0);
        dVar47 = ((dVar33 - dVar33) - dVar47) - dVar47;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar41 = (double)CONCAT44(SUB84(dVar47,0),fVar16);
        dVar46 = (double)CONCAT44(fVar16,fVar24);
      }
      fVar24 = (float)((ulonglong)dVar41 >> 0x20);
      fVar16 = (float)((ulonglong)dVar46 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar47 = (double)CONCAT44(-(float)((ulonglong)dVar47 >> 0x20),SUB84(dVar47,0));
        fVar18 = SUB84(dVar27,0) * SUB84(dVar46,0);
        fVar12 = fVar12 * fVar24;
        dVar41 = (double)CONCAT44(fVar25 * fVar16,fVar18);
        dVar26 = (double)CONCAT44(((fVar13 * fVar24 + fVar18 + SUB84(dVar20,0) * fVar16) - fVar12) -
                                  fVar35 * SUB84(dVar46,0),fVar12);
      }
      else {
        dVar47 = -dVar47;
        dVar26 = (dVar26 * dVar41 + dVar27 * dVar46) * dVar41 + dVar20 * dVar46;
        dVar41 = dVar27 * dVar46 * dVar46;
        dVar26 = (dVar26 - dVar26) - dVar20 * dVar46 * dVar46;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),(int)((ulonglong)dVar47 >> 0x20))
        ;
        dVar47 = dVar31;
      }
      if (in_FPSCR_PR == '\0') {
        dVar26 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) -
                                  (float)((ulonglong)dVar41 >> 0x20),SUB84(dVar26,0));
      }
      else {
        dVar26 = dVar26 - dVar41;
      }
      dVar41 = (double)CONCAT44(fVar8,SUB84(dVar41,0));
      if (in_FPSCR_PR == '\0') {
        dVar31 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) * fVar8,SUB84(dVar11,0) * fVar8
                                 );
        dVar41 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) * fVar8,SUB84(dVar26,0) * fVar8
                                 );
      }
      else {
        dVar31 = dVar11 * dVar41 * dVar41;
        dVar41 = dVar26 * dVar41 * dVar41;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(puVar6 + -4) = SUB84(dVar31,0);
        *(int *)(puVar6 + -8) = (int)((ulonglong)dVar31 >> 0x20);
        *(int *)(puVar6 + -0xc) = SUB84(dVar41,0);
        *(int *)(puVar6 + -0x10) = (int)((ulonglong)dVar41 >> 0x20);
      }
      else {
        *(double *)(puVar6 + -8) = dVar48;
        *(double *)(puVar6 + -0x10) = dVar31;
        *(double *)(puVar6 + -0x18) = dVar47;
        *(double *)(puVar6 + -0x20) = dVar41;
      }
      uVar34 = 1;
    }
    else {
      if (in_FPSCR_SZ == '\0') {
        fVar12 = *(float *)(param_5 + 5);
        dVar26 = (double)CONCAT44(*(undefined4 *)(param_5 + 7),*(undefined4 *)((int)param_5 + 0x3c))
        ;
        dVar31 = (double)CONCAT44(*(undefined4 *)(param_5 + 2),*(undefined4 *)((int)param_5 + 0x14))
        ;
        dVar41 = (double)CONCAT44(*(undefined4 *)(param_5 + 3),*(undefined4 *)((int)param_5 + 0x1c))
        ;
        dVar47 = (double)CONCAT44(*(undefined4 *)((int)param_5 + 0x24),
                                  *(undefined4 *)((int)param_5 + 0x24));
        dVar11 = (double)CONCAT44(fVar12,*(undefined4 *)((int)param_5 + 0x34));
        dVar27 = (double)CONCAT44(fVar12,*(undefined4 *)((int)param_5 + 0x2c));
      }
      else {
        dVar47 = param_5[5];
        dVar11 = param_5[6];
        in_xd2 = param_5[7];
        dVar26 = param_5[10];
        in_xd6 = param_5[0xb];
        dVar31 = param_5[2];
        in_xd8 = param_5[3];
        dVar41 = param_5[4];
        in_xd10 = param_5[5];
        fVar12 = (float)((ulonglong)dVar11 >> 0x20);
        dVar27 = dVar11;
        in_xd0 = dVar47;
      }
      fVar13 = (float)((ulonglong)dVar26 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar20 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) * SUB84(dVar26,0),
                                  SUB84(dVar11,0) * SUB84(dVar27,0));
        dVar47 = (double)CONCAT44((float)((ulonglong)dVar47 >> 0x20) * SUB84(dVar26,0),
                                  SUB84(dVar47,0) * fVar13);
        dVar26 = (double)CONCAT44(fVar12 * SUB84(dVar11,0),SUB84(dVar27,0) * fVar13);
      }
      else {
        dVar20 = dVar11 * dVar26 * dVar27 * dVar11;
        dVar47 = dVar47 * dVar26 * dVar26;
        dVar26 = dVar27 * dVar11 * dVar26;
      }
      fVar12 = (float)((ulonglong)dVar41 >> 0x20);
      dVar27 = in_xd8;
      dVar11 = dVar41;
      if (in_FPSCR_SZ == '\0') {
        dVar27 = (double)CONCAT44(SUB84(dVar31,0),fVar12);
        dVar11 = in_xd6;
      }
      fVar35 = (float)((ulonglong)dVar20 >> 0x20);
      fVar25 = (float)((ulonglong)dVar47 >> 0x20);
      fVar13 = SUB84(dVar26,0);
      if (in_FPSCR_PR == '\0') {
        fVar24 = SUB84(dVar27,0) * SUB84(dVar20,0);
        dVar41 = (double)CONCAT44(fVar12 * fVar25,SUB84(dVar41,0));
        dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) * fVar35 + fVar24,fVar24);
        dVar31 = (double)CONCAT44((int)((ulonglong)dVar31 >> 0x20),SUB84(dVar31,0) * fVar13);
      }
      else {
        dVar27 = dVar27 * dVar20 * dVar20;
        dVar41 = dVar41 * dVar47;
        dVar27 = dVar27 + dVar27;
        dVar31 = dVar31 * dVar26;
      }
      dVar48 = in_xd10;
      if (in_FPSCR_SZ == '\0') {
        dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),SUB84(dVar41,0));
        dVar48 = dVar11;
      }
      fVar12 = (float)((ulonglong)dVar26 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar24 = SUB84(dVar27,0) * fVar12;
        dVar31 = (double)CONCAT44(((((float)((ulonglong)dVar27 >> 0x20) +
                                    SUB84(dVar41,0) * SUB84(dVar47,0)) - fVar24) -
                                  (float)((ulonglong)dVar41 >> 0x20)) - SUB84(dVar31,0),fVar24);
      }
      else {
        dVar27 = dVar27 * dVar26 + dVar41 * dVar47;
        dVar31 = ((dVar27 - dVar27) - dVar41 * dVar47) - dVar31;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar41 = (double)CONCAT44(*(undefined4 *)(param_5 + 1),*(undefined4 *)((int)param_5 + 0xc));
        dVar27 = (double)CONCAT44(*(undefined4 *)((int)param_5 + 4),
                                  *(undefined4 *)((int)param_5 + 4));
      }
      else {
        dVar27 = param_5[1];
        dVar41 = param_5[2];
        in_xd10 = param_5[3];
        in_xd8 = dVar27;
      }
      fVar24 = (float)((ulonglong)dVar41 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar16 = SUB84(dVar20,0) * fVar24 + (float)((ulonglong)dVar27 >> 0x20) * fVar35;
        dVar11 = (double)CONCAT44(fVar35,fVar16);
        fVar12 = (((SUB84(dVar47,0) * SUB84(dVar41,0) + fVar16) - fVar12 * SUB84(dVar41,0)) -
                 fVar25 * fVar24) - SUB84(dVar27,0) * fVar13;
      }
      else {
        dVar27 = dVar27 * dVar20 * dVar41;
        dVar11 = dVar20 * dVar41 + dVar27;
        dVar26 = dVar26 * dVar41;
        fVar13 = SUB84(dVar26,0);
        dVar47 = (dVar47 * dVar41 * dVar41 + dVar11) - dVar26;
        fVar12 = SUB84((dVar47 - dVar47) - dVar27 * dVar26,0);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar31 = (double)CONCAT44((int)((ulonglong)dVar31 >> 0x20),fVar12);
      }
      fVar12 = (float)((ulonglong)dVar31 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        dVar26 = (double)CONCAT44(*(undefined4 *)(param_5 + 4),*(undefined4 *)((int)param_5 + 0x24))
        ;
        dVar41 = (double)CONCAT44(*(undefined4 *)(param_5 + 5),*(undefined4 *)((int)param_5 + 0x2c))
        ;
        dVar47 = (double)CONCAT44(*(undefined4 *)(param_5 + 6),*(undefined4 *)((int)param_5 + 0x34))
        ;
        dVar27 = (double)CONCAT44(*(undefined4 *)(param_5 + 7),*(undefined4 *)((int)param_5 + 0x3c))
        ;
      }
      else {
        dVar26 = param_5[4];
        dVar48 = param_5[5];
        dVar41 = param_5[6];
        in_xd8 = param_5[7];
        dVar47 = param_5[8];
        in_xd10 = param_5[9];
        dVar27 = param_5[10];
      }
      fVar35 = (float)((ulonglong)dVar47 >> 0x20);
      fVar25 = (float)((ulonglong)dVar26 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar41 = (double)CONCAT44((int)((ulonglong)dVar41 >> 0x20),SUB84(dVar41,0) * fVar35);
        dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) * fVar25,SUB84(dVar27,0));
      }
      else {
        dVar41 = dVar41 * dVar47;
        dVar27 = dVar27 * dVar26;
      }
      dVar20 = in_xd8;
      if (in_FPSCR_SZ == '\0') {
        dVar20 = (double)CONCAT44(SUB84(dVar41,0),fVar13);
      }
      if (in_FPSCR_PR == '\0') {
        dVar41 = (double)CONCAT44((float)((ulonglong)dVar41 >> 0x20) * fVar35,SUB84(dVar41,0));
      }
      else {
        dVar41 = dVar41 * dVar47;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar11 = (double)CONCAT44((int)((ulonglong)dVar11 >> 0x20),(int)((ulonglong)dVar27 >> 0x20))
        ;
      }
      if (in_FPSCR_PR == '\0') {
        dVar26 = (double)CONCAT44(fVar25 * SUB84(dVar27,0),SUB84(dVar26,0));
      }
      else {
        dVar26 = dVar26 * dVar27;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),(int)((ulonglong)dVar41 >> 0x20))
        ;
        uVar34 = *(undefined4 *)(param_5 + 2);
        dVar27 = (double)CONCAT44(*(undefined4 *)(param_5 + 3),*(undefined4 *)((int)param_5 + 0x1c))
        ;
        dVar47 = (double)CONCAT44(*(undefined4 *)(param_5 + 3),*(undefined4 *)((int)param_5 + 0x1c))
        ;
        dVar32 = (double)CONCAT44(uVar34,uVar34);
        dVar41 = dVar48;
      }
      else {
        dVar32 = param_5[2];
        dVar27 = param_5[4];
        dVar47 = param_5[4];
        in_xd10 = param_5[5];
        in_xd8 = dVar32;
      }
      fVar13 = (float)((ulonglong)dVar20 >> 0x20);
      fVar25 = (float)((ulonglong)dVar11 >> 0x20);
      fVar35 = (float)((ulonglong)dVar26 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar24 = (((SUB84(dVar32,0) * fVar25 + (float)((ulonglong)dVar47 >> 0x20) * fVar13 +
                   SUB84(dVar47,0) * SUB84(dVar11,0)) - SUB84(dVar27,0) * SUB84(dVar26,0)) -
                 (float)((ulonglong)dVar27 >> 0x20) * fVar35) -
                 (float)((ulonglong)dVar32 >> 0x20) * SUB84(dVar20,0);
      }
      else {
        dVar48 = dVar47 * dVar20 * dVar11;
        dVar47 = dVar27 * dVar26 * dVar26;
        dVar47 = ((dVar32 * dVar11 * dVar20 + dVar48 + dVar48) - dVar47) - dVar47;
        fVar24 = SUB84(dVar47 - dVar47,0);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar47 = (double)CONCAT44(*(undefined4 *)param_5,*(undefined4 *)((int)param_5 + 4));
        dVar27 = (double)CONCAT44(*(undefined4 *)(param_5 + 1),*(undefined4 *)((int)param_5 + 0xc));
      }
      else {
        dVar47 = *param_5;
        in_xd10 = param_5[1];
        dVar27 = param_5[2];
      }
      fVar16 = (float)((ulonglong)dVar47 >> 0x20);
      fVar45 = (float)((ulonglong)dVar27 >> 0x20);
      fVar18 = SUB84(dVar27,0);
      if (in_FPSCR_PR == '\0') {
        fVar13 = fVar13 * fVar45;
        fVar17 = SUB84(dVar11,0) * fVar18;
        dVar48 = (double)CONCAT44(fVar35 * fVar45,SUB84(dVar26,0) * fVar18);
        dVar26 = (double)CONCAT44(fVar13,SUB84(dVar20,0) * fVar16);
        dVar47 = (double)CONCAT44(fVar25 * fVar16 + fVar13 + fVar17,fVar17);
      }
      else {
        dVar48 = dVar26 * dVar27 * dVar27;
        dVar26 = dVar20 * dVar27 * dVar47;
        dVar47 = dVar11 * dVar47 * dVar27 + dVar26;
        dVar47 = dVar47 + dVar47;
      }
      if (in_FPSCR_PR == '\0') {
        dVar47 = (double)CONCAT44((float)((ulonglong)dVar47 >> 0x20) - SUB84(dVar48,0),
                                  SUB84(dVar47,0));
      }
      else {
        dVar47 = dVar47 - dVar48;
      }
      dVar27 = in_xd8;
      if (in_FPSCR_SZ == '\0') {
        dVar27 = (double)CONCAT44(-fVar24,-SUB84(dVar26,0));
      }
      if (in_FPSCR_PR == '\0') {
        fVar13 = (float)((ulonglong)dVar47 >> 0x20) - (float)((ulonglong)dVar48 >> 0x20);
        dVar47 = (double)CONCAT44(fVar13,SUB84(dVar47,0));
        dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),SUB84(dVar27,0) + fVar13);
      }
      else {
        dVar47 = dVar47 - dVar48;
        dVar27 = dVar27 + dVar47;
      }
      dVar47 = (double)CONCAT44(fVar8,SUB84(dVar47,0));
      if (in_FPSCR_PR == '\0') {
        dVar31 = (double)CONCAT44(fVar12 * fVar8,-SUB84(dVar31,0) * fVar8);
        dVar47 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) * fVar8,SUB84(dVar27,0) * fVar8
                                 );
      }
      else {
        dVar31 = (double)CONCAT44(fVar12,-SUB84(dVar31,0)) * dVar47 * dVar47;
        dVar47 = dVar27 * dVar47 * dVar47;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(puVar4 + -4) = (int)((ulonglong)dVar31 >> 0x20);
        *(int *)(puVar4 + -8) = SUB84(dVar31,0);
        *(int *)(puVar4 + -0xc) = (int)((ulonglong)dVar47 >> 0x20);
        puVar5 = puVar4 + -0x10;
        *(int *)(puVar4 + -0x10) = SUB84(dVar47,0);
        fVar13 = *(float *)((int)param_5 + 0x24);
        dVar11 = (double)CONCAT44(*(undefined4 *)(param_5 + 4),fVar13);
        fVar12 = *(float *)((int)param_5 + 0x34);
        dVar27 = (double)CONCAT44(*(undefined4 *)(param_5 + 6),fVar12);
        dVar26 = (double)CONCAT44(*(undefined4 *)(param_5 + 2),*(undefined4 *)((int)param_5 + 0x14))
        ;
        dVar31 = (double)CONCAT44(*(undefined4 *)(param_5 + 3),*(undefined4 *)((int)param_5 + 0x1c))
        ;
        dVar47 = (double)CONCAT44(fVar13,*(undefined4 *)((int)param_5 + 0x2c));
        dVar20 = (double)CONCAT44(fVar12,*(undefined4 *)((int)param_5 + 0x3c));
      }
      else {
        *(double *)(puVar4 + -8) = dVar31;
        *(double *)(puVar4 + -0x10) = in_xd0;
        *(double *)(puVar4 + -0x18) = dVar47;
        puVar5 = puVar4 + -0x20;
        *(double *)(puVar4 + -0x20) = in_xd2;
        dVar11 = param_5[4];
        dVar47 = param_5[5];
        in_xd2 = param_5[7];
        dVar27 = param_5[8];
        dVar20 = param_5[9];
        dVar41 = param_5[0xb];
        dVar26 = param_5[2];
        in_xd8 = param_5[3];
        dVar31 = param_5[4];
        in_xd10 = param_5[5];
        fVar13 = SUB84(dVar11,0);
        fVar12 = SUB84(dVar27,0);
        in_xd0 = dVar47;
      }
      if (in_FPSCR_PR == '\0') {
        fVar35 = (float)((ulonglong)dVar11 >> 0x20);
        fVar25 = SUB84(dVar20,0) * fVar35;
        dVar11 = (double)CONCAT44(fVar35 * fVar12,fVar13 * SUB84(dVar20,0));
        fVar13 = (float)((ulonglong)dVar27 >> 0x20);
        dVar27 = (double)CONCAT44(fVar13,fVar12 * SUB84(dVar47,0));
        dVar47 = (double)CONCAT44((float)((ulonglong)dVar47 >> 0x20) * fVar13,
                                  SUB84(dVar47,0) * fVar13);
      }
      else {
        fVar25 = SUB84(dVar20 * dVar11 * dVar20,0);
        dVar11 = dVar11 * dVar20 * dVar27;
        dVar27 = dVar27 * dVar47;
        dVar47 = dVar47 * dVar27 * dVar27;
      }
      fVar12 = (float)((ulonglong)dVar26 >> 0x20);
      dVar20 = in_xd10;
      if (in_FPSCR_SZ == '\0') {
        dVar41 = (double)CONCAT44(fVar25,SUB84(dVar27,0));
        dVar20 = (double)CONCAT44(SUB84(dVar31,0),fVar18);
      }
      fVar25 = (float)((ulonglong)dVar11 >> 0x20);
      fVar13 = (float)((ulonglong)dVar47 >> 0x20);
      fVar35 = (float)((ulonglong)dVar41 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar24 = SUB84(dVar26,0) * SUB84(dVar47,0);
        dVar26 = (double)CONCAT44((((fVar12 * SUB84(dVar11,0) + fVar24 +
                                    (float)((ulonglong)dVar20 >> 0x20) * fVar25) -
                                   SUB84(dVar31,0) * fVar13) - SUB84(dVar26,0) * fVar35) -
                                  fVar12 * SUB84(dVar41,0),fVar24);
      }
      else {
        dVar27 = dVar26 * dVar11 * dVar47;
        dVar26 = dVar26 * dVar41 * dVar41;
        dVar26 = (((dVar27 + dVar27 + dVar20 * dVar11) - dVar31 * dVar47) - dVar26) - dVar26;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar31 = (double)CONCAT44(*(undefined4 *)param_5,*(undefined4 *)((int)param_5 + 4));
        uVar34 = *(undefined4 *)((int)param_5 + 0xc);
      }
      else {
        dVar31 = *param_5;
        in_xd8 = param_5[1];
        uVar34 = SUB84(param_5[2],0);
        in_xd10 = param_5[3];
      }
      fVar12 = (float)((ulonglong)dVar31 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar11 = (double)CONCAT44(fVar25,SUB84(dVar11,0) * fVar12);
      }
      else {
        dVar11 = dVar11 * dVar31;
      }
      if (in_FPSCR_SZ == '\0') {
        in_xd10 = (double)CONCAT44(uVar34,uVar34);
      }
      fVar25 = (float)((ulonglong)dVar11 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar24 = SUB84(dVar47,0) * SUB84(dVar31,0);
        dVar47 = (double)CONCAT44(fVar13,fVar24);
        dVar31 = (double)CONCAT44(fVar25,(((SUB84(dVar11,0) + fVar24 +
                                           (float)((ulonglong)in_xd10 >> 0x20) * fVar25) -
                                          SUB84(in_xd10,0) * fVar13) - fVar35 * SUB84(dVar31,0)) -
                                         SUB84(dVar41,0) * fVar12);
      }
      else {
        dVar47 = dVar47 * dVar31;
        fVar13 = (float)((ulonglong)dVar47 >> 0x20);
        dVar27 = in_xd10 * dVar11 * dVar47;
        dVar31 = dVar41 * dVar31 * dVar31;
        dVar31 = (((dVar11 + dVar47 + dVar27) - dVar27) - dVar31) - dVar31;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar47 = (double)CONCAT44(fVar13,(int)((ulonglong)dVar26 >> 0x20));
        dVar26 = in_xd2;
      }
      fVar12 = (float)((ulonglong)dVar31 >> 0x20);
      dVar41 = (double)CONCAT44(fVar12,-SUB84(dVar31,0));
      if (in_FPSCR_SZ == '\0') {
        dVar20 = (double)CONCAT44(*(undefined4 *)(param_5 + 4),*(undefined4 *)((int)param_5 + 0x24))
        ;
        dVar11 = (double)CONCAT44(*(undefined4 *)(param_5 + 5),*(undefined4 *)((int)param_5 + 0x2c))
        ;
        dVar27 = (double)CONCAT44(*(undefined4 *)(param_5 + 6),*(undefined4 *)((int)param_5 + 0x34))
        ;
        dVar48 = (double)CONCAT44(*(undefined4 *)(param_5 + 7),*(undefined4 *)((int)param_5 + 0x3c))
        ;
      }
      else {
        dVar20 = param_5[4];
        dVar11 = param_5[6];
        dVar27 = param_5[8];
        in_xd8 = param_5[9];
        dVar48 = param_5[10];
      }
      fVar25 = (float)((ulonglong)dVar48 >> 0x20);
      fVar13 = (float)((ulonglong)dVar11 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) * fVar13,SUB84(dVar27,0));
        dVar48 = (double)CONCAT44((float)((ulonglong)dVar20 >> 0x20) * fVar25,
                                  SUB84(dVar20,0) * fVar25);
      }
      else {
        dVar27 = dVar27 * dVar11;
        dVar48 = dVar20 * dVar48 * dVar48;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar11 = (double)CONCAT44(fVar13,(int)((ulonglong)dVar27 >> 0x20));
      }
      if (in_FPSCR_PR == '\0') {
        dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) * SUB84(dVar27,0),
                                  SUB84(dVar11,0));
      }
      else {
        dVar11 = dVar11 * dVar27;
      }
      if (in_FPSCR_SZ == '\0') {
        fVar35 = *(float *)(param_5 + 2);
        fVar25 = *(float *)((int)param_5 + 0x14);
        dVar20 = (double)CONCAT44(fVar35,fVar25);
        fVar13 = *(float *)(param_5 + 3);
        dVar27 = (double)CONCAT44(fVar13,fVar35);
        dVar32 = (double)CONCAT44(fVar25,fVar13);
      }
      else {
        dVar20 = param_5[2];
        dVar32 = param_5[3];
        dVar27 = param_5[4];
        fVar35 = (float)((ulonglong)dVar20 >> 0x20);
        fVar25 = SUB84(dVar20,0);
        fVar13 = (float)((ulonglong)dVar27 >> 0x20);
        in_xd8 = dVar32;
      }
      fVar24 = (float)((ulonglong)dVar47 >> 0x20);
      fVar16 = (float)((ulonglong)dVar48 >> 0x20);
      fVar18 = (float)((ulonglong)dVar11 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar45 = SUB84(dVar32,0) * fVar12;
        dVar27 = (double)CONCAT44(-(((((float)((ulonglong)dVar32 >> 0x20) * SUB84(dVar11,0) +
                                       SUB84(dVar27,0) * SUB84(dVar48,0) + fVar45) - fVar13 * fVar24
                                     ) - fVar25 * fVar16) - fVar35 * fVar18),fVar45);
      }
      else {
        dVar32 = dVar32 * dVar11 * dVar41 + dVar27 * dVar48;
        dVar20 = dVar20 * dVar48 * dVar11;
        dVar27 = -((((dVar32 + dVar32) - dVar27 * dVar48 * dVar47) - dVar20) - dVar20);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar20 = (double)CONCAT44(*(undefined4 *)param_5,*(undefined4 *)((int)param_5 + 4));
        dVar32 = (double)CONCAT44(*(undefined4 *)(param_5 + 1),*(undefined4 *)((int)param_5 + 0xc));
      }
      else {
        dVar20 = *param_5;
        in_xd8 = param_5[1];
        dVar32 = param_5[2];
      }
      fVar25 = (float)((ulonglong)dVar20 >> 0x20);
      fVar13 = SUB84(dVar20,0);
      fVar35 = (float)((ulonglong)dVar32 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar24 = fVar24 * fVar35;
        dVar47 = (double)CONCAT44(fVar24,SUB84(dVar47,0));
        dVar41 = (double)CONCAT44((((fVar12 * fVar35 +
                                    SUB84(dVar48,0) * fVar25 + SUB84(dVar11,0) * fVar13) - fVar24) -
                                  fVar16 * fVar13) - fVar18 * fVar25,-SUB84(dVar31,0));
      }
      else {
        dVar31 = dVar48 * dVar20 + dVar11 * dVar20;
        dVar47 = dVar47 * dVar32;
        dVar41 = (((dVar41 * dVar32 + dVar31) - dVar47) - dVar31 * dVar20) -
                 dVar11 * dVar20 * dVar20;
      }
      dVar31 = (double)CONCAT44(fVar8,fVar13);
      if (in_FPSCR_PR == '\0') {
        fVar12 = SUB84(dVar47,0) * fVar8;
        dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) * fVar8,SUB84(dVar27,0));
        dVar31 = (double)CONCAT44((float)((ulonglong)dVar41 >> 0x20) * fVar8,SUB84(dVar41,0) * fVar8
                                 );
      }
      else {
        fVar12 = SUB84(dVar47 * dVar31,0);
        dVar27 = dVar27 * dVar31;
        dVar31 = dVar41 * dVar31 * dVar31;
      }
      if (in_FPSCR_SZ == '\0') {
        *(float *)(puVar5 + -4) = fVar12;
        *(int *)(puVar5 + -8) = SUB84(dVar31,0);
        *(int *)(puVar5 + -0xc) = (int)((ulonglong)dVar27 >> 0x20);
        puVar6 = puVar5 + -0x10;
        *(int *)(puVar5 + -0x10) = (int)((ulonglong)dVar31 >> 0x20);
        dVar31 = (double)CONCAT44(*(undefined4 *)(param_5 + 1),*(undefined4 *)((int)param_5 + 0xc));
        fVar12 = *(float *)((int)param_5 + 0x1c);
        dVar41 = (double)CONCAT44(*(undefined4 *)(param_5 + 3),fVar12);
        pdVar2 = param_5 + 4;
        dVar47 = (double)CONCAT44(*(undefined4 *)((int)param_5 + 4),
                                  *(undefined4 *)((int)param_5 + 4));
        dVar27 = (double)CONCAT44(fVar12,*(undefined4 *)((int)param_5 + 0x14));
      }
      else {
        *(double *)(puVar5 + -8) = dVar26;
        *(double *)(puVar5 + -0x10) = in_xd0;
        *(double *)(puVar5 + -0x18) = dVar27;
        puVar6 = puVar5 + -0x20;
        *(double *)(puVar5 + -0x20) = dVar31;
        dVar47 = param_5[1];
        dVar31 = param_5[2];
        dVar26 = param_5[3];
        dVar41 = param_5[6];
        dVar27 = param_5[7];
        pdVar2 = param_5 + 8;
        fVar12 = SUB84(dVar41,0);
        in_xd0 = dVar47;
      }
      fVar25 = (float)((ulonglong)dVar41 >> 0x20);
      fVar13 = (float)((ulonglong)dVar31 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar11 = (double)CONCAT44((float)((ulonglong)dVar47 >> 0x20) * fVar25,
                                  SUB84(dVar47,0) * fVar12);
        dVar47 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) * fVar13,
                                  SUB84(dVar27,0) * SUB84(dVar31,0));
        dVar20 = (double)CONCAT44(fVar13 * SUB84(dVar27,0),SUB84(dVar31,0) * fVar25);
      }
      else {
        dVar11 = dVar47 * dVar41 * dVar41;
        dVar20 = dVar31 * dVar27 * dVar31;
        dVar47 = dVar27 * dVar31 * dVar20;
        dVar20 = dVar20 * dVar41;
      }
      if (in_FPSCR_SZ == '\0') {
        fVar13 = *(float *)((int)pdVar2 + 0x14);
        fVar12 = *(float *)(pdVar2 + 3);
        fVar25 = *(float *)((int)pdVar2 + 0x1c);
        dVar31 = (double)CONCAT44(fVar12,fVar25);
        dVar41 = (double)CONCAT44(fVar25,fVar13);
        dVar27 = (double)CONCAT44(fVar13,fVar12);
      }
      else {
        dVar27 = pdVar2[3];
        dVar31 = pdVar2[4];
        dVar41 = pdVar2[5];
        fVar25 = SUB84(dVar31,0);
        fVar13 = SUB84(dVar41,0);
        fVar12 = (float)((ulonglong)dVar31 >> 0x20);
        in_xd8 = dVar41;
      }
      fVar35 = (float)((ulonglong)dVar11 >> 0x20);
      fVar18 = (float)((ulonglong)dVar47 >> 0x20);
      fVar24 = SUB84(dVar20,0);
      fVar16 = (float)((ulonglong)dVar20 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar41 = (double)CONCAT44(((((float)((ulonglong)dVar41 >> 0x20) * fVar35 + fVar13 * fVar18 +
                                    fVar12 * SUB84(dVar47,0)) -
                                   (float)((ulonglong)dVar27 >> 0x20) * fVar24) - fVar25 * fVar16) -
                                  SUB84(dVar27,0) * SUB84(dVar11,0),fVar13 * fVar18);
      }
      else {
        dVar41 = dVar41 * dVar11 * dVar47;
        dVar41 = (((dVar41 + dVar41 + dVar31 * dVar47) - dVar27 * dVar20) - dVar31 * dVar47 * dVar20
                 ) - dVar27 * dVar20 * dVar11;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar27 = (double)CONCAT44(*(undefined4 *)(param_5 + 5),*(undefined4 *)((int)param_5 + 0x2c))
        ;
        dVar31 = (double)CONCAT44(*(undefined4 *)((int)param_5 + 0x24),
                                  *(undefined4 *)((int)param_5 + 0x24));
      }
      else {
        dVar31 = param_5[5];
        dVar27 = param_5[6];
        in_xd8 = dVar31;
      }
      fVar12 = (float)((ulonglong)dVar27 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar13 = SUB84(dVar47,0) * fVar12;
        dVar32 = (double)CONCAT44(fVar18,fVar13);
        fVar16 = fVar16 * SUB84(dVar27,0);
        dVar48 = (double)CONCAT44(fVar16,fVar24);
        fVar12 = SUB84(dVar11,0) * fVar12;
        dVar47 = (double)CONCAT44((((fVar35 * SUB84(dVar27,0) +
                                     (float)((ulonglong)dVar31 >> 0x20) * fVar18 + fVar13) -
                                   SUB84(dVar31,0) * fVar24) - fVar16) - fVar12,fVar12);
      }
      else {
        dVar32 = dVar47 * dVar27;
        dVar48 = dVar20 * dVar27;
        dVar47 = ((dVar11 * dVar27 + dVar31 * dVar47 + dVar32) - dVar31 * dVar47 * dVar20) * dVar27
                 - dVar48;
        dVar47 = dVar47 - dVar47;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),SUB84(dVar48,0));
        dVar47 = (double)CONCAT44((int)((ulonglong)dVar47 >> 0x20),(int)((ulonglong)dVar41 >> 0x20))
        ;
        dVar41 = in_xd0;
      }
      if (in_FPSCR_PR == '\0') {
        dVar47 = (double)CONCAT44(-(float)((ulonglong)dVar47 >> 0x20),SUB84(dVar47,0));
      }
      else {
        dVar47 = -dVar47;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar11 = (double)CONCAT44(*(undefined4 *)param_5,*(undefined4 *)((int)param_5 + 4));
        dVar27 = (double)CONCAT44(*(undefined4 *)(param_5 + 1),*(undefined4 *)((int)param_5 + 0xc));
        dVar31 = (double)CONCAT44(*(undefined4 *)(param_5 + 2),*(undefined4 *)((int)param_5 + 0x14))
        ;
        dVar20 = (double)CONCAT44(*(undefined4 *)(param_5 + 3),*(undefined4 *)((int)param_5 + 0x1c))
        ;
        pdVar2 = param_5 + 4;
      }
      else {
        dVar11 = *param_5;
        dVar27 = param_5[2];
        dVar31 = param_5[4];
        in_xd8 = param_5[5];
        dVar20 = param_5[6];
        pdVar2 = param_5 + 8;
      }
      fVar13 = (float)((ulonglong)dVar31 >> 0x20);
      fVar12 = (float)((ulonglong)dVar11 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar31 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) * fVar13,
                                  SUB84(dVar27,0) * fVar13);
        dVar27 = (double)CONCAT44((float)((ulonglong)dVar20 >> 0x20) * fVar12,SUB84(dVar20,0));
        dVar11 = (double)CONCAT44(fVar12 * SUB84(dVar20,0),SUB84(dVar11,0));
      }
      else {
        dVar31 = dVar27 * dVar31 * dVar31;
        dVar27 = dVar20 * dVar11;
        dVar11 = dVar11 * dVar27;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar11 = (double)CONCAT44((int)((ulonglong)dVar11 >> 0x20),(int)((ulonglong)dVar27 >> 0x20))
        ;
        uVar34 = *(undefined4 *)(pdVar2 + 2);
        dVar20 = (double)CONCAT44(*(undefined4 *)(pdVar2 + 3),*(undefined4 *)((int)pdVar2 + 0x1c));
        dVar27 = (double)CONCAT44(*(undefined4 *)(pdVar2 + 3),*(undefined4 *)((int)pdVar2 + 0x1c));
        dVar48 = (double)CONCAT44(uVar34,uVar34);
      }
      else {
        dVar48 = pdVar2[2];
        dVar20 = pdVar2[4];
        dVar27 = pdVar2[4];
        in_xd8 = dVar48;
      }
      fVar12 = (float)((ulonglong)dVar32 >> 0x20);
      fVar25 = (float)((ulonglong)dVar31 >> 0x20);
      fVar13 = (float)((ulonglong)dVar11 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar35 = SUB84(dVar48,0) * SUB84(dVar32,0);
        dVar27 = (double)CONCAT44(((((float)((ulonglong)dVar48 >> 0x20) * fVar12 +
                                     SUB84(dVar20,0) * SUB84(dVar11,0) +
                                    (float)((ulonglong)dVar20 >> 0x20) * SUB84(dVar31,0)) - fVar35)
                                  - SUB84(dVar27,0) * fVar25) -
                                  (float)((ulonglong)dVar27 >> 0x20) * fVar13,fVar35);
      }
      else {
        dVar20 = dVar20 * dVar11 * dVar31;
        dVar20 = (dVar48 * dVar32 + dVar20) * dVar32 + dVar20;
        dVar27 = dVar27 * dVar31 * dVar11;
        dVar27 = ((dVar20 - dVar20) - dVar27) - dVar27;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar20 = (double)CONCAT44(*(undefined4 *)(param_5 + 4),*(undefined4 *)((int)param_5 + 0x24))
        ;
        dVar48 = (double)CONCAT44(*(undefined4 *)(param_5 + 5),*(undefined4 *)((int)param_5 + 0x2c))
        ;
      }
      else {
        dVar20 = param_5[4];
        dVar48 = param_5[6];
      }
      fVar35 = (float)((ulonglong)dVar20 >> 0x20);
      fVar24 = (float)((ulonglong)dVar48 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar27 = (double)CONCAT44(-(float)((ulonglong)dVar27 >> 0x20),SUB84(dVar27,0));
        fVar18 = SUB84(dVar11,0) * SUB84(dVar48,0);
        fVar16 = SUB84(dVar32,0) * fVar35;
        dVar11 = (double)CONCAT44(fVar13 * fVar24,fVar18);
        dVar31 = (double)CONCAT44(((fVar12 * fVar35 + fVar18 + SUB84(dVar31,0) * fVar24) - fVar16) -
                                  fVar25 * SUB84(dVar48,0),fVar16);
      }
      else {
        dVar27 = -dVar27;
        dVar20 = (dVar32 * dVar20 + dVar11 * dVar48) * dVar20 + dVar31 * dVar48;
        dVar11 = dVar11 * dVar48 * dVar48;
        dVar31 = (dVar20 - dVar20) - dVar31 * dVar48 * dVar48;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar31 = (double)CONCAT44((int)((ulonglong)dVar31 >> 0x20),(int)((ulonglong)dVar27 >> 0x20))
        ;
        dVar27 = dVar26;
      }
      if (in_FPSCR_PR == '\0') {
        dVar31 = (double)CONCAT44((float)((ulonglong)dVar31 >> 0x20) -
                                  (float)((ulonglong)dVar11 >> 0x20),SUB84(dVar31,0));
      }
      else {
        dVar31 = dVar31 - dVar11;
      }
      dVar26 = (double)CONCAT44(fVar8,SUB84(dVar11,0));
      if (in_FPSCR_PR == '\0') {
        dVar47 = (double)CONCAT44((float)((ulonglong)dVar47 >> 0x20) * fVar8,SUB84(dVar47,0) * fVar8
                                 );
        dVar26 = (double)CONCAT44((float)((ulonglong)dVar31 >> 0x20) * fVar8,SUB84(dVar31,0) * fVar8
                                 );
      }
      else {
        dVar47 = dVar47 * dVar26 * dVar26;
        dVar26 = dVar31 * dVar26 * dVar26;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(puVar6 + -4) = SUB84(dVar47,0);
        *(int *)(puVar6 + -8) = (int)((ulonglong)dVar47 >> 0x20);
        *(int *)(puVar6 + -0xc) = SUB84(dVar26,0);
        pdVar7 = (double *)(puVar6 + -0x10);
        *(int *)(puVar6 + -0x10) = (int)((ulonglong)dVar26 >> 0x20);
        dVar11 = (double)CONCAT44(*(undefined4 *)param_5,*(undefined4 *)((int)param_5 + 4));
        dVar26 = (double)CONCAT44(*(undefined4 *)param_5,*(undefined4 *)((int)param_5 + 4));
        dVar31 = (double)CONCAT44(*(undefined4 *)(param_5 + 2),*(undefined4 *)((int)param_5 + 0x14))
        ;
        dVar47 = (double)CONCAT44(*(undefined4 *)(param_5 + 3),*(undefined4 *)((int)param_5 + 0x1c))
        ;
        pdVar2 = param_5 + 4;
        dVar20 = (double)CONCAT44(*(undefined4 *)((int)param_5 + 0xc),
                                  *(undefined4 *)((int)param_5 + 0xc));
      }
      else {
        *(double *)(puVar6 + -8) = dVar41;
        *(double *)(puVar6 + -0x10) = dVar47;
        *(double *)(puVar6 + -0x18) = dVar27;
        pdVar7 = (double *)(puVar6 + -0x20);
        *(double *)(puVar6 + -0x20) = dVar26;
        dVar11 = *param_5;
        dVar26 = *param_5;
        in_xd8 = param_5[1];
        dVar41 = param_5[1];
        dVar20 = param_5[3];
        dVar31 = param_5[4];
        dVar47 = param_5[6];
        pdVar2 = param_5 + 8;
        dVar27 = dVar20;
      }
      fVar12 = SUB84(dVar31,0);
      fVar13 = (float)((ulonglong)dVar31 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar48 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) * SUB84(dVar47,0),
                                  SUB84(dVar11,0));
        dVar47 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) * fVar12,
                                  SUB84(dVar26,0) * SUB84(dVar47,0));
        dVar26 = (double)CONCAT44((float)((ulonglong)dVar20 >> 0x20) * fVar13,
                                  SUB84(dVar20,0) * fVar12);
        dVar31 = (double)CONCAT44(fVar13 * SUB84(dVar11,0),fVar12);
      }
      else {
        dVar48 = dVar11 * dVar47;
        dVar47 = dVar26 * dVar31 * dVar47;
        dVar26 = dVar20 * dVar31 * dVar31;
        dVar31 = dVar31 * dVar48;
      }
      pdVar1 = pdVar2 + 2;
      if (in_FPSCR_SZ == '\0') {
        dVar31 = (double)CONCAT44((int)((ulonglong)dVar31 >> 0x20),(int)((ulonglong)dVar48 >> 0x20))
        ;
        dVar48 = (double)CONCAT44(*(undefined4 *)pdVar1,*(undefined4 *)((int)pdVar2 + 0x14));
        dVar20 = (double)CONCAT44(*(undefined4 *)pdVar1,*(undefined4 *)((int)pdVar2 + 0x14));
        dVar11 = (double)CONCAT44(*(undefined4 *)(pdVar2 + 3),*(undefined4 *)((int)pdVar2 + 0x1c));
        dVar32 = (double)CONCAT44(*(undefined4 *)(pdVar2 + 3),*(undefined4 *)((int)pdVar2 + 0x1c));
        pdVar2 = pdVar2 + 4;
      }
      else {
        dVar48 = *pdVar1;
        dVar20 = *pdVar1;
        dVar11 = pdVar2[4];
        dVar32 = pdVar2[4];
        in_xd8 = pdVar2[5];
        pdVar2 = pdVar2 + 6;
      }
      fVar12 = (float)((ulonglong)dVar47 >> 0x20);
      fVar25 = (float)((ulonglong)dVar26 >> 0x20);
      fVar13 = SUB84(dVar26,0);
      fVar35 = (float)((ulonglong)dVar31 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar24 = SUB84(dVar48,0) * fVar25;
        dVar11 = (double)CONCAT44(((((float)((ulonglong)dVar48 >> 0x20) * SUB84(dVar47,0) +
                                     SUB84(dVar11,0) * fVar12 + fVar24) -
                                   (float)((ulonglong)dVar20 >> 0x20) * fVar13) -
                                  SUB84(dVar32,0) * fVar35) - SUB84(dVar20,0) * SUB84(dVar31,0),
                                  fVar24);
      }
      else {
        dVar11 = dVar48 * dVar47 * dVar26 + dVar11 * dVar47;
        dVar11 = (((dVar11 + dVar11) - dVar20 * dVar26) - dVar32 * dVar31) -
                 dVar20 * dVar26 * dVar31;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar20 = (double)CONCAT44(*(undefined4 *)(pdVar2 + -4),*(undefined4 *)((int)pdVar2 + -0x1c))
        ;
        dVar48 = (double)CONCAT44(*(undefined4 *)((int)pdVar2 + -0x14),
                                  *(undefined4 *)((int)pdVar2 + -0x14));
      }
      else {
        dVar20 = pdVar2[-4];
        in_xd8 = pdVar2[-3];
        dVar48 = pdVar2[-1];
      }
      fVar24 = (float)((ulonglong)dVar20 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar25 = fVar25 * SUB84(dVar20,0);
        dVar26 = (double)CONCAT44(fVar25,fVar13);
        dVar32 = (double)CONCAT44(fVar35,SUB84(dVar31,0) * SUB84(dVar20,0));
        dVar47 = (double)CONCAT44(fVar12,((SUB84(dVar47,0) * fVar24 +
                                           (float)((ulonglong)dVar48 >> 0x20) * fVar12 + fVar25) -
                                         fVar24 * fVar13) - SUB84(dVar48,0) * fVar35);
      }
      else {
        dVar48 = dVar48 * dVar47 * dVar20;
        dVar26 = dVar26 * dVar20;
        dVar32 = dVar31 * dVar20 * dVar26;
        dVar47 = ((dVar47 * dVar20 + dVar48 + dVar26) - dVar20 * dVar26) - dVar48 * dVar31;
      }
      uVar34 = SUB84(dVar26,0);
      dVar31 = dVar32;
      if (in_FPSCR_SZ == '\0') {
        uVar34 = (undefined4)((ulonglong)dVar32 >> 0x20);
        dVar31 = dVar27;
      }
      if (in_FPSCR_PR == '\0') {
        dVar47 = (double)CONCAT44((int)((ulonglong)dVar47 >> 0x20),SUB84(dVar47,0) - SUB84(dVar32,0)
                                 );
      }
      else {
        dVar47 = dVar47 - dVar32;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar11 = (double)CONCAT44((int)((ulonglong)dVar11 >> 0x20),uVar34);
      }
      fVar12 = (float)((ulonglong)dVar47 >> 0x20);
      dVar26 = (double)CONCAT44(fVar12,-SUB84(dVar47,0));
      if (in_FPSCR_SZ == '\0') {
        dVar48 = (double)CONCAT44(*(undefined4 *)param_5,*(undefined4 *)((int)param_5 + 4));
        dVar20 = (double)CONCAT44(*(undefined4 *)(param_5 + 1),*(undefined4 *)((int)param_5 + 0xc));
        dVar27 = (double)CONCAT44(*(undefined4 *)(param_5 + 2),*(undefined4 *)((int)param_5 + 0x14))
        ;
        dVar32 = (double)CONCAT44(*(undefined4 *)(param_5 + 3),*(undefined4 *)((int)param_5 + 0x1c))
        ;
        pdVar2 = param_5 + 4;
      }
      else {
        dVar48 = *param_5;
        dVar20 = param_5[2];
        dVar27 = param_5[4];
        in_xd8 = param_5[5];
        dVar32 = param_5[6];
        pdVar2 = param_5 + 8;
      }
      fVar25 = (float)((ulonglong)dVar32 >> 0x20);
      fVar13 = (float)((ulonglong)dVar20 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar33 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) * fVar13,SUB84(dVar27,0));
        dVar32 = (double)CONCAT44((float)((ulonglong)dVar48 >> 0x20) * fVar25,
                                  SUB84(dVar48,0) * fVar25);
        dVar20 = (double)CONCAT44(fVar13 * SUB84(dVar27,0),SUB84(dVar20,0));
      }
      else {
        dVar33 = dVar27 * dVar20;
        dVar32 = dVar48 * dVar32 * dVar32;
        dVar20 = dVar20 * dVar33;
      }
      pdVar1 = pdVar2 + 2;
      if (in_FPSCR_SZ == '\0') {
        dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),(int)((ulonglong)dVar33 >> 0x20))
        ;
        dVar48 = (double)CONCAT44(*(undefined4 *)pdVar1,*(undefined4 *)((int)pdVar2 + 0x14));
        dVar27 = (double)CONCAT44(*(undefined4 *)pdVar1,*(undefined4 *)((int)pdVar2 + 0x14));
        dVar33 = (double)CONCAT44(*(undefined4 *)(pdVar2 + 3),*(undefined4 *)(pdVar2 + 3));
      }
      else {
        dVar48 = *pdVar1;
        dVar27 = *pdVar1;
        in_xd8 = pdVar2[3];
        dVar33 = pdVar2[4];
      }
      fVar25 = (float)((ulonglong)dVar20 >> 0x20);
      fVar13 = (float)((ulonglong)dVar32 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar35 = SUB84(dVar48,0) * SUB84(dVar20,0);
        dVar27 = (double)CONCAT44(-(((((float)((ulonglong)dVar48 >> 0x20) * SUB84(dVar32,0) +
                                       (float)((ulonglong)dVar33 >> 0x20) * fVar12 + fVar35) -
                                     (float)((ulonglong)dVar27 >> 0x20) * fVar25) -
                                    SUB84(dVar33,0) * SUB84(dVar11,0)) - SUB84(dVar27,0) * fVar13),
                                  fVar35);
      }
      else {
        dVar33 = dVar33 * dVar26;
        dVar48 = dVar48 * dVar32 * dVar20 + dVar33;
        dVar27 = -((((dVar48 + dVar48) - dVar27 * dVar20) - dVar33 * dVar11) -
                  dVar27 * dVar20 * dVar32);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar48 = (double)CONCAT44(*(undefined4 *)(param_5 + 4),*(undefined4 *)((int)param_5 + 0x24))
        ;
        dVar33 = (double)CONCAT44(*(undefined4 *)(param_5 + 5),*(undefined4 *)((int)param_5 + 0x2c))
        ;
      }
      else {
        dVar48 = param_5[4];
        dVar33 = param_5[6];
      }
      fVar24 = (float)((ulonglong)dVar33 >> 0x20);
      fVar35 = (float)((ulonglong)dVar48 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar18 = SUB84(dVar32,0) * fVar35;
        fVar16 = SUB84(dVar11,0) * fVar24;
        dVar11 = (double)CONCAT44((int)((ulonglong)dVar11 >> 0x20),fVar16);
        fVar13 = fVar13 * SUB84(dVar48,0);
        dVar21 = (double)CONCAT44(fVar13,fVar18);
        dVar47 = (double)CONCAT44((((fVar12 * fVar24 + fVar18 + SUB84(dVar20,0) * SUB84(dVar48,0)) -
                                   fVar25 * fVar35) - fVar16) - fVar13,-SUB84(dVar47,0));
      }
      else {
        dVar47 = dVar20 * dVar48 * dVar48;
        dVar11 = dVar11 * dVar33;
        dVar21 = dVar32 * dVar48 * dVar48;
        dVar47 = (((dVar26 * dVar33 + dVar32 * dVar48 + dVar47) - dVar47) - dVar11) - dVar21;
      }
      dVar26 = (double)CONCAT44(fVar8,SUB84(dVar21,0));
      if (in_FPSCR_SZ == '\0') {
        dVar11 = (double)CONCAT44((int)((ulonglong)dVar11 >> 0x20),SUB84(dVar47,0));
        dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),(int)((ulonglong)dVar47 >> 0x20))
        ;
        dVar47 = in_xd8;
        dVar41 = dVar31;
      }
      if (in_FPSCR_PR == '\0') {
        dVar31 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) * fVar8,SUB84(dVar11,0) * fVar8
                                 );
        dVar26 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) * fVar8,SUB84(dVar27,0) * fVar8
                                 );
      }
      else {
        dVar31 = dVar11 * dVar26 * dVar26;
        dVar26 = dVar27 * dVar26 * dVar26;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)param_5 + 0x2c) = SUB84(dVar31,0);
        *(int *)((int)param_5 + 0x3c) = SUB84(dVar26,0);
        *(int *)(param_5 + 7) = (int)((ulonglong)dVar26 >> 0x20);
        *(int *)(param_5 + 5) = (int)((ulonglong)dVar31 >> 0x20);
        uVar34 = *(undefined4 *)pdVar7;
        uVar9 = *(undefined4 *)((int)pdVar7 + 4);
        uVar10 = *(undefined4 *)(pdVar7 + 1);
        uVar14 = *(undefined4 *)((int)pdVar7 + 0xc);
        uVar19 = *(undefined4 *)(pdVar7 + 2);
        uVar22 = *(undefined4 *)((int)pdVar7 + 0x14);
        uVar28 = *(undefined4 *)((int)pdVar7 + 0x1c);
        uVar30 = *(undefined4 *)(pdVar7 + 4);
        uVar36 = *(undefined4 *)((int)pdVar7 + 0x24);
        uVar38 = *(undefined4 *)(pdVar7 + 5);
        uVar42 = *(undefined4 *)((int)pdVar7 + 0x2c);
        *(undefined4 *)((int)param_5 + 0x24) = *(undefined4 *)(pdVar7 + 3);
        *(undefined4 *)(param_5 + 4) = uVar28;
        *(undefined4 *)((int)param_5 + 0x1c) = uVar34;
        *(undefined4 *)(param_5 + 3) = uVar9;
        *(undefined4 *)((int)param_5 + 0x14) = uVar30;
        *(undefined4 *)(param_5 + 2) = uVar36;
        *(undefined4 *)((int)param_5 + 0xc) = uVar10;
        *(undefined4 *)(param_5 + 1) = uVar14;
        *(undefined4 *)((int)param_5 + 4) = uVar38;
        *(undefined4 *)param_5 = uVar42;
        *(undefined4 *)((int)param_5 + 0x34) = uVar19;
        *(undefined4 *)(param_5 + 6) = uVar22;
        uVar34 = DAT_8c6398b0;
      }
      else {
        *(double *)((int)param_5 + 0x2c) = dVar41;
        param_5[7] = dVar47;
        param_5[6] = dVar26;
        param_5[5] = dVar31;
        dVar20 = *pdVar7;
        dVar47 = pdVar7[1];
        dVar48 = pdVar7[2];
        dVar41 = pdVar7[3];
        dVar32 = pdVar7[4];
        dVar31 = pdVar7[5];
        dVar26 = pdVar7[7];
        dVar33 = pdVar7[8];
        dVar27 = pdVar7[9];
        dVar21 = pdVar7[10];
        dVar11 = pdVar7[0xb];
        param_5[4] = pdVar7[6];
        param_5[3] = dVar26;
        param_5[2] = dVar20;
        param_5[1] = dVar47;
        *param_5 = dVar33;
        param_5[-1] = dVar27;
        param_5[-2] = dVar48;
        param_5[-3] = dVar41;
        param_5[-4] = dVar21;
        param_5[-5] = dVar11;
        param_5[6] = dVar32;
        param_5[5] = dVar31;
        uVar34 = DAT_8c6398b0;
      }
    }
  }
  return uVar34;
}

