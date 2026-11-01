/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c09e906) */
/* WARNING: Removing unreachable block (ram,0x8c09e8b2) */
/* WARNING: Removing unreachable block (ram,0x8c09e8c2) */
/* WARNING: Removing unreachable block (ram,0x8c09e916) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* WARNING: Restarted to delay deadcode elimination for space: register */

undefined4
FUN_8c09e880(undefined4 param_1,undefined4 param_2,double *param_3,int param_4,uint param_5)

{
  short sVar1;
  short sVar2;
  short sVar3;
  short sVar4;
  undefined4 uVar5;
  int iVar6;
  int in_r2;
  double *pdVar7;
  short *psVar8;
  undefined4 unaff_r8;
  double *pdVar9;
  undefined4 unaff_r9;
  undefined1 *puVar10;
  undefined1 *puVar11;
  undefined4 *puVar12;
  double *pdVar13;
  float fVar16;
  double dVar14;
  undefined4 uVar17;
  float fVar18;
  double dVar15;
  float fVar19;
  undefined4 uVar20;
  undefined4 uVar21;
  double dVar22;
  float fVar23;
  float fVar24;
  double dVar25;
  double dVar26;
  float fVar27;
  undefined4 uVar28;
  double dVar29;
  float fVar30;
  double dVar31;
  double dVar32;
  float fVar33;
  float fVar34;
  double dVar35;
  double dVar36;
  undefined4 in_fr13;
  float fVar37;
  double dVar38;
  float fVar39;
  double in_dr14;
  double in_xd0;
  double in_xd2;
  undefined8 uVar40;
  double in_xd4;
  double in_xd6;
  double in_xd8;
  double dVar41;
  undefined8 in_xd10;
  double in_xd12;
  double dVar42;
  double in_xd14;
  bool bVar43;
  bool bVar44;
  bool bVar45;
  bool bVar46;
  bool bVar47;
  bool bVar48;
  bool bVar49;
  bool bVar50;
  bool bVar51;
  bool bVar52;
  bool bVar53;
  bool bVar54;
  bool bVar55;
  bool bVar56;
  bool bVar57;
  bool bVar58;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_20 [16];
  undefined1 local_10 [12];
  undefined4 uStack_4;
  
  uVar5 = DAT_8c09eb04;
  uVar21 = DAT_8c09eb00;
  if (in_FPSCR_SZ == '\0') {
    stack0xfffffff8 = CONCAT44(SUB84(in_dr14,0),local_10._8_4_);
    puVar10 = local_10;
  }
  else {
    puVar10 = auStack_20;
  }
  *(undefined4 *)(puVar10 + -4) = unaff_r9;
  *(undefined4 *)(puVar10 + -8) = unaff_r8;
  psVar8 = (short *)(param_4 * 0x14 + *(int *)((int)param_3 + 0x1c));
  if (in_FPSCR_SZ == '\0') {
    dVar22 = (double)CONCAT44(DAT_8c09e948,param_1);
  }
  else {
    dVar22 = (double)CONCAT44(_DAT_8c09e94c,DAT_8c09e948);
  }
  fVar18 = (float)((ulonglong)dVar22 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    fVar16 = (float)(int)psVar8[4] * fVar18;
  }
  else {
    fVar16 = (float)((ulonglong)((double)(int)psVar8[4] * dVar22) >> 0x20);
  }
  if (in_FPSCR_PR == '\0') {
    dVar14 = (double)CONCAT44(fVar16,(float)(int)psVar8[5] * fVar18);
  }
  else {
    dVar14 = (double)CONCAT44(fVar16,(float)(int)psVar8[5]) * dVar22;
  }
  if (in_FPSCR_PR == '\0') {
    fVar16 = (float)(int)psVar8[6] * fVar18;
  }
  else {
    fVar16 = (float)((ulonglong)((double)(int)psVar8[6] * dVar22) >> 0x20);
  }
  if (in_FPSCR_PR == '\0') {
    dVar22 = (double)CONCAT44(fVar16,(float)(int)psVar8[7] * fVar18);
  }
  else {
    dVar22 = (double)CONCAT44(fVar16,(float)(int)psVar8[7]) * dVar22;
  }
  dVar15 = dVar14;
  dVar25 = dVar22;
  if (((param_5 & 4) != 0) && (dVar15 = dVar22, dVar25 = dVar14, in_FPSCR_SZ == '\0')) {
    dVar15 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),SUB84(dVar14,0));
    dVar25 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),SUB84(dVar22,0));
  }
  dVar22 = in_xd0;
  dVar14 = in_xd2;
  if (((param_5 & 8) != 0) && (dVar22 = in_xd2, dVar14 = in_xd0, in_FPSCR_SZ == '\0')) {
    uVar17 = SUB84(dVar15,0);
    dVar15 = (double)CONCAT44((int)((ulonglong)dVar15 >> 0x20),SUB84(dVar25,0));
    dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),uVar17);
    dVar22 = in_xd0;
    dVar14 = in_xd2;
  }
  sVar1 = *psVar8;
  sVar2 = psVar8[1];
  sVar3 = psVar8[2];
  sVar4 = psVar8[3];
  uVar17 = (undefined4)((ulonglong)dVar15 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(puVar10 + -0xc) = uVar17;
    pdVar9 = (double *)(puVar10 + -0x10);
    *(int *)pdVar9 = SUB84(dVar15,0);
  }
  else {
    *(double *)(puVar10 + -0x10) = dVar15;
    pdVar9 = (double *)(puVar10 + -0x18);
    *pdVar9 = dVar22;
  }
  iVar6 = -(int)sVar3;
  if (in_FPSCR_SZ == '\0') {
    pdVar9 = (double *)((int)pdVar9 + -4);
    *(undefined4 *)pdVar9 = uVar17;
  }
  else {
    pdVar9 = pdVar9 + -1;
    *pdVar9 = dVar15;
  }
  if (in_FPSCR_SZ == '\0') {
    pdVar9 = (double *)((int)pdVar9 + -4);
    *(int *)pdVar9 = SUB84(dVar25,0);
  }
  else {
    pdVar9 = pdVar9 + -1;
    *pdVar9 = dVar14;
  }
  uVar17 = (undefined4)((ulonglong)dVar25 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    pdVar9 = (double *)((int)pdVar9 + -4);
    *(undefined4 *)pdVar9 = uVar17;
  }
  else {
    pdVar9 = pdVar9 + -1;
    *pdVar9 = dVar25;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)pdVar9 + -4) = SUB84(dVar15,0);
    pdVar9 = pdVar9 + -1;
    *(undefined4 *)pdVar9 = uVar17;
  }
  else {
    pdVar9[-1] = dVar22;
    pdVar9 = pdVar9 + -2;
    *pdVar9 = dVar25;
  }
  if (in_FPSCR_PR == '\0') {
    fVar18 = (float)iVar6;
  }
  else {
    fVar18 = (float)((ulonglong)(double)iVar6 >> 0x20);
  }
  if (in_FPSCR_SZ == '\0') {
    pdVar9 = (double *)((int)pdVar9 + -4);
    *(int *)pdVar9 = SUB84(dVar25,0);
  }
  else {
    pdVar9 = pdVar9 + -1;
    *pdVar9 = dVar14;
  }
  fVar16 = (float)-(int)sVar4;
  if (in_FPSCR_SZ == '\0') {
    dVar25 = (double)CONCAT44(*(undefined4 *)((int)param_3 + 0xc),
                              *(undefined4 *)((int)param_3 + 0xc));
  }
  else {
    dVar25 = *(double *)((int)param_3 + 0xc);
    in_xd4 = *(double *)((int)param_3 + 0xc);
  }
  fVar23 = (float)((ulonglong)dVar25 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar15 = (double)CONCAT44(fVar18 * fVar23,fVar16);
    fVar23 = (float)(sVar1 + iVar6) * fVar23;
  }
  else {
    dVar15 = (double)CONCAT44(fVar18,fVar16) * dVar25;
    fVar23 = (float)((ulonglong)((double)(sVar1 + iVar6) * dVar25) >> 0x20);
  }
  fVar18 = (float)((int)sVar2 + -(int)sVar4);
  if (in_FPSCR_PR == '\0') {
    dVar15 = (double)CONCAT44((int)((ulonglong)dVar15 >> 0x20),SUB84(dVar15,0) * SUB84(dVar25,0));
    dVar25 = (double)CONCAT44(fVar23,fVar18 * SUB84(dVar25,0));
  }
  else {
    dVar15 = dVar15 * dVar25;
    dVar25 = (double)CONCAT44(fVar23,fVar18) * dVar25;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar26 = (double)CONCAT44(*(undefined4 *)param_3,*(undefined4 *)((int)param_3 + 4));
    fVar18 = *(float *)(param_3 + 1);
    param_3 = (double *)((int)param_3 + 0xc);
  }
  else {
    dVar26 = *param_3;
    in_xd4 = param_3[1];
    fVar18 = (float)((ulonglong)param_3[2] >> 0x20);
    param_3 = param_3 + 3;
  }
                    /* WARNING: Ignoring partial resolution of indirect */
  if (in_FPSCR_PR == '\0') {
    fVar19 = SUB84(dVar26,0);
    fVar34 = (float)((ulonglong)dVar26 >> 0x20);
    fVar16 = SUB84(in_xd12,0) * fVar18;
    fVar23 = (float)((ulonglong)in_xd12 >> 0x20) * fVar18;
    fVar18 = (float)((ulonglong)dVar14 >> 0x20) * fVar19 +
             (float)((ulonglong)in_xd6 >> 0x20) * fVar34 + (float)((ulonglong)in_xd10 >> 0x20) * 1.0
             + (float)((ulonglong)in_xd14 >> 0x20) * fVar18;
    fVar16 = SUB84(dVar22,0) * fVar19 + SUB84(in_xd4,0) * fVar34 + SUB84(in_xd8,0) * 1.0 + fVar16;
    fVar16 = (float)((ulonglong)dVar22 >> 0x20) * fVar19 +
             (float)((ulonglong)in_xd4 >> 0x20) * fVar34 + (float)((ulonglong)in_xd8 >> 0x20) * 1.0
             + fVar23;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar31 = (double)CONCAT44(*(undefined4 *)_DAT_8c09e94c,*(undefined4 *)DAT_8c09e950);
  }
  else {
    dVar31 = *_DAT_8c09e94c;
    in_xd8 = *DAT_8c09e950;
  }
  if (in_FPSCR_PR == '\0') {
    dVar29 = (double)CONCAT44(fVar18,1.0 / fVar18);
  }
  else {
    dVar29 = (double)CONCAT44(fVar18,0x3f800000) / (double)CONCAT44(fVar18,0x3f800000);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar35 = (double)CONCAT44(*(undefined4 *)PTR_DAT_8c09e954,param_2);
  }
  else {
    dVar35 = *(double *)PTR_DAT_8c09e954;
  }
  if (in_FPSCR_PR == '\0') {
    fVar18 = (float)((ulonglong)dVar29 >> 0x20) + (float)((ulonglong)dVar35 >> 0x20);
    dVar29 = (double)CONCAT44(fVar18,SUB84(dVar29,0));
    bVar43 = (float)((ulonglong)dVar31 >> 0x20) < fVar18;
  }
  else {
    dVar29 = dVar29 + dVar35;
    fVar18 = (float)((ulonglong)dVar29 >> 0x20);
    bVar43 = dVar31 < dVar29;
  }
  if (!bVar43) {
    if (in_FPSCR_PR == '\0') {
      bVar43 = fVar18 < SUB84(dVar31,0);
    }
    else {
      bVar43 = dVar29 < dVar31;
    }
    if (!bVar43) {
      if (in_FPSCR_PR == '\0') {
        dVar35 = (double)CONCAT44(1.0 / fVar18,SUB84(dVar35,0));
      }
      else {
        dVar35 = (double)CONCAT44(0x3f800000,SUB84(dVar35,0)) / dVar29;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar36 = (double)CONCAT44(*(undefined4 *)DAT_8c09e9d4,SUB84(dVar35,0));
        dVar35 = (double)CONCAT44((int)((ulonglong)dVar35 >> 0x20),SUB84(dVar29,0));
      }
      else {
        dVar36 = *DAT_8c09e9d4;
      }
      if (in_FPSCR_PR == '\0') {
        fVar18 = (float)((ulonglong)dVar36 >> 0x20) * SUB84(dVar35,0);
        dVar36 = (double)CONCAT44(fVar18,SUB84(dVar36,0));
        dVar15 = (double)CONCAT44((float)((ulonglong)dVar15 >> 0x20) * fVar18,SUB84(dVar15,0));
      }
      else {
        dVar36 = dVar36 * dVar35;
        fVar18 = (float)((ulonglong)dVar36 >> 0x20);
        dVar15 = dVar15 * dVar36;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar31 = (double)CONCAT44(*(undefined4 *)DAT_8c09e9d8,SUB84(dVar31,0));
      }
      else {
        dVar31 = *DAT_8c09e9d8;
      }
      if (in_FPSCR_PR == '\0') {
        dVar15 = (double)CONCAT44((int)((ulonglong)dVar15 >> 0x20),SUB84(dVar15,0) * fVar18);
      }
      else {
        dVar15 = dVar15 * dVar36;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar31 = (double)CONCAT44((int)((ulonglong)dVar31 >> 0x20),*(undefined4 *)DAT_8c09e9dc);
      }
      else {
        in_xd8 = *DAT_8c09e9dc;
      }
      if (in_FPSCR_PR == '\0') {
        dVar29 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * fVar18,
                                  SUB84(dVar25,0) * fVar18);
        fVar16 = (float)((ulonglong)dVar31 >> 0x20) * fVar18;
        dVar25 = (double)CONCAT44(fVar16,SUB84(dVar31,0) * fVar18);
        dVar26 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) * fVar16,SUB84(dVar26,0));
      }
      else {
        dVar29 = dVar25 * dVar36 * dVar36;
        dVar25 = dVar31 * dVar36 * dVar36;
        dVar26 = dVar26 * dVar25;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar31 = (double)CONCAT44(*(undefined4 *)((int)DAT_8c09e9d4 + 0xc),SUB84(dVar36,0));
        pdVar7 = DAT_8c09e9d4 + 2;
      }
      else {
        dVar31 = *(double *)((int)DAT_8c09e9d4 + 0xc);
        pdVar7 = (double *)((int)DAT_8c09e9d4 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),SUB84(dVar26,0) * SUB84(dVar25,0)
                                 );
      }
      else {
        dVar26 = dVar26 * dVar25;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar31 = (double)CONCAT44((int)((ulonglong)dVar31 >> 0x20),*(undefined4 *)pdVar7);
      }
      if (in_FPSCR_PR == '\0') {
        dVar26 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) +
                                  (float)((ulonglong)dVar31 >> 0x20),SUB84(dVar26,0));
      }
      else {
        dVar26 = dVar26 + dVar31;
      }
      uVar17 = (undefined4)((ulonglong)dVar35 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        puVar11 = puVar10 + -0x2c;
        *(undefined4 *)(puVar10 + -0x2c) = uVar17;
      }
      else {
        puVar11 = puVar10 + -0x30;
        *(double *)(puVar10 + -0x30) = dVar35;
      }
      if (in_FPSCR_PR == '\0') {
        dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),SUB84(dVar26,0) + SUB84(dVar31,0)
                                 );
      }
      else {
        dVar26 = dVar26 + dVar31;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(puVar11 + -4) = SUB84(dVar26,0);
        puVar12 = (undefined4 *)(puVar11 + -8);
        *(int *)(puVar11 + -8) = (int)((ulonglong)dVar26 >> 0x20);
      }
      else {
        *(double *)(puVar11 + -8) = in_xd4;
        puVar12 = (undefined4 *)(puVar11 + -0x10);
        *(double *)(puVar11 + -0x10) = dVar26;
      }
      uVar20 = (undefined4)((ulonglong)dVar15 >> 0x20);
      fVar18 = SUB84(dVar29,0);
      uVar28 = (undefined4)((ulonglong)dVar29 >> 0x20);
      dVar25 = in_xd14;
      if ((param_5 & 1) == 0) {
        dVar36 = dVar15;
        dVar26 = dVar29;
        dVar38 = dVar29;
        dVar41 = dVar22;
        dVar42 = dVar14;
        if (in_FPSCR_SZ == '\0') {
          dVar36 = (double)CONCAT44(uVar20,fVar18);
          dVar26 = (double)CONCAT44(uVar28,SUB84(dVar15,0));
          dVar14 = in_xd4;
          dVar41 = in_xd8;
          dVar42 = in_xd12;
        }
      }
      else {
        if (in_FPSCR_PR == '\0') {
          fVar16 = ((float)(*(uint *)(param_3 + 1) & 0xffff) * 6.2831855) / 65536.0;
          fVar23 = (float)sin(fVar16);
          fVar16 = (float)cos(fVar16);
          dVar31 = (double)CONCAT44((float)fVar23,(float)fVar16);
        }
        dVar25 = dVar29;
        dVar41 = dVar29;
        dVar38 = dVar15;
        dVar36 = dVar22;
        if (in_FPSCR_SZ == '\0') {
          dVar35 = (double)CONCAT44(uVar17,SUB84(dVar15,0));
          dVar15 = (double)CONCAT44(uVar20,uVar20);
          dVar25 = (double)CONCAT44(uVar28,uVar28);
          dVar41 = in_xd8;
          dVar38 = dVar22;
          dVar36 = in_xd6;
        }
        dVar22 = dVar38;
        fVar16 = SUB84(dVar31,0);
        fVar23 = (float)((ulonglong)dVar31 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar15 = (double)CONCAT44((float)((ulonglong)dVar15 >> 0x20) * fVar16,
                                    SUB84(dVar15,0) * fVar23);
          dVar32 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * fVar16,
                                    SUB84(dVar25,0) * fVar23);
        }
        else {
          dVar15 = dVar15 * dVar31 * dVar31;
          dVar32 = dVar25 * dVar31 * dVar31;
        }
        dVar25 = dVar14;
        if (in_FPSCR_SZ == '\0') {
          dVar36 = (double)CONCAT44(SUB84(dVar35,0),fVar18);
          dVar25 = in_xd14;
        }
        if (in_FPSCR_PR == '\0') {
          dVar35 = (double)CONCAT44((int)((ulonglong)dVar35 >> 0x20),SUB84(dVar35,0) * fVar23);
          dVar29 = (double)CONCAT44(uVar28,fVar18 * fVar23);
          in_dr14 = (double)CONCAT44((float)((ulonglong)dVar36 >> 0x20) * fVar16,
                                     SUB84(dVar36,0) * fVar16);
        }
        else {
          dVar35 = dVar35 * dVar31;
          dVar29 = dVar29 * dVar31;
          in_dr14 = dVar36 * dVar31 * dVar31;
        }
        fVar18 = (float)((ulonglong)dVar15 >> 0x20);
        dVar36 = dVar15;
        if (in_FPSCR_SZ == '\0') {
          dVar36 = (double)CONCAT44(fVar18,SUB84(dVar26,0));
        }
        if (in_FPSCR_PR == '\0') {
          dVar36 = (double)CONCAT44((float)((ulonglong)dVar36 >> 0x20) - SUB84(dVar29,0),
                                    SUB84(dVar36,0));
        }
        else {
          dVar36 = dVar36 - dVar29;
        }
        dVar14 = dVar22;
        if (in_FPSCR_SZ == '\0') {
          dVar36 = (double)CONCAT44((int)((ulonglong)dVar36 >> 0x20),SUB84(dVar15,0));
          dVar14 = in_xd4;
        }
        fVar16 = (float)((ulonglong)in_dr14 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar36 = (double)CONCAT44((int)((ulonglong)dVar36 >> 0x20),
                                    SUB84(dVar36,0) + SUB84(in_dr14,0));
          dVar15 = (double)CONCAT44(fVar18 - SUB84(dVar35,0),SUB84(dVar15,0) + fVar16);
        }
        else {
          dVar36 = dVar36 + in_dr14;
          dVar15 = (dVar15 - dVar35) + in_dr14;
        }
        fVar18 = (float)((ulonglong)dVar32 >> 0x20);
        dVar38 = dVar32;
        if (in_FPSCR_SZ == '\0') {
          dVar38 = (double)CONCAT44(fVar18,in_fr13);
        }
        if (in_FPSCR_PR == '\0') {
          dVar38 = (double)CONCAT44((float)((ulonglong)dVar38 >> 0x20) - SUB84(dVar29,0),
                                    SUB84(dVar38,0));
        }
        else {
          dVar38 = dVar38 - dVar29;
        }
        dVar42 = dVar41;
        if (in_FPSCR_SZ == '\0') {
          dVar38 = (double)CONCAT44((int)((ulonglong)dVar38 >> 0x20),SUB84(dVar32,0));
          dVar42 = in_xd12;
        }
        if (in_FPSCR_PR == '\0') {
          dVar38 = (double)CONCAT44((int)((ulonglong)dVar38 >> 0x20),
                                    SUB84(dVar38,0) + SUB84(in_dr14,0));
          dVar26 = (double)CONCAT44(fVar18 - SUB84(dVar35,0),SUB84(dVar32,0) + fVar16);
        }
        else {
          dVar38 = dVar38 + in_dr14;
          dVar26 = (dVar32 - dVar35) + in_dr14;
        }
      }
      if (in_FPSCR_SZ == '\0') {
        dVar29 = (double)CONCAT44((int)((ulonglong)dVar29 >> 0x20),*puVar12);
        dVar35 = (double)CONCAT44((int)((ulonglong)dVar35 >> 0x20),puVar12[1]);
        pdVar13 = (double *)(puVar12 + 2);
      }
      else {
        pdVar13 = (double *)(puVar12 + 4);
      }
      fVar18 = SUB84(dVar29,0);
      fVar16 = SUB84(dVar35,0);
      if (in_FPSCR_PR == '\0') {
        dVar32 = (double)CONCAT44((float)((ulonglong)dVar15 >> 0x20) + fVar18,
                                  SUB84(dVar15,0) + fVar16);
        dVar36 = (double)CONCAT44((float)((ulonglong)dVar36 >> 0x20) + fVar18,
                                  SUB84(dVar36,0) + fVar16);
        dVar15 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) + fVar18,
                                  SUB84(dVar26,0) + fVar16);
        dVar26 = (double)CONCAT44((float)((ulonglong)dVar38 >> 0x20) + fVar18,
                                  SUB84(dVar38,0) + fVar16);
      }
      else {
        dVar32 = dVar15 + dVar29 + dVar35;
        dVar36 = dVar36 + dVar29 + dVar35;
        dVar15 = dVar26 + dVar29 + dVar35;
        dVar26 = dVar38 + dVar29 + dVar35;
      }
      if (in_FPSCR_SZ == '\0') {
        in_dr14 = (double)CONCAT44((int)((ulonglong)in_dr14 >> 0x20),*(undefined4 *)DAT_8c09ea68);
        dVar35 = (double)CONCAT44((int)((ulonglong)dVar35 >> 0x20),
                                  *(undefined4 *)((int)DAT_8c09ea68 + 4));
        dVar31 = (double)CONCAT44((int)((ulonglong)dVar31 >> 0x20),*(undefined4 *)(DAT_8c09ea68 + 1)
                                 );
        dVar29 = (double)CONCAT44((int)((ulonglong)dVar29 >> 0x20),
                                  *(undefined4 *)((int)DAT_8c09ea68 + 0xc));
      }
      else {
        dVar25 = *DAT_8c09ea68;
      }
      fVar18 = SUB84(dVar32,0);
      fVar16 = SUB84(dVar29,0);
      if (in_FPSCR_PR == '\0') {
        bVar43 = fVar16 < fVar18;
      }
      else {
        bVar43 = dVar29 < dVar32;
      }
      fVar23 = SUB84(dVar35,0);
      if (in_FPSCR_PR == '\0') {
        bVar44 = fVar18 < fVar23;
      }
      else {
        bVar44 = dVar32 < dVar35;
      }
      fVar19 = (float)((ulonglong)dVar32 >> 0x20);
      fVar34 = SUB84(dVar31,0);
      if (in_FPSCR_PR == '\0') {
        bVar45 = fVar34 < fVar19;
      }
      else {
        bVar45 = dVar31 < dVar32;
      }
      fVar39 = SUB84(in_dr14,0);
      if (in_FPSCR_PR == '\0') {
        bVar46 = fVar19 < fVar39;
      }
      else {
        bVar46 = dVar32 < in_dr14;
      }
      fVar24 = SUB84(dVar36,0);
      if (in_FPSCR_PR == '\0') {
        bVar47 = fVar16 < fVar24;
      }
      else {
        bVar47 = dVar29 < dVar36;
      }
      if (in_FPSCR_PR == '\0') {
        bVar48 = fVar24 < fVar23;
      }
      else {
        bVar48 = dVar36 < dVar35;
      }
      fVar27 = (float)((ulonglong)dVar36 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        bVar49 = fVar34 < fVar27;
      }
      else {
        bVar49 = dVar31 < dVar36;
      }
      if (in_FPSCR_PR == '\0') {
        bVar50 = fVar27 < fVar39;
      }
      else {
        bVar50 = dVar36 < in_dr14;
      }
      fVar30 = SUB84(dVar15,0);
      if (in_FPSCR_PR == '\0') {
        bVar51 = fVar16 < fVar30;
      }
      else {
        bVar51 = dVar29 < dVar15;
      }
      if (in_FPSCR_PR == '\0') {
        bVar52 = fVar30 < fVar23;
      }
      else {
        bVar52 = dVar15 < dVar35;
      }
      fVar33 = (float)((ulonglong)dVar15 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        bVar53 = fVar34 < fVar33;
      }
      else {
        bVar53 = dVar31 < dVar15;
      }
      if (in_FPSCR_PR == '\0') {
        bVar54 = fVar33 < fVar39;
      }
      else {
        bVar54 = dVar15 < in_dr14;
      }
      fVar37 = SUB84(dVar26,0);
      if (in_FPSCR_PR == '\0') {
        bVar55 = fVar16 < fVar37;
      }
      else {
        bVar55 = dVar29 < dVar26;
      }
      if (in_FPSCR_PR == '\0') {
        bVar56 = fVar37 < fVar23;
      }
      else {
        bVar56 = dVar26 < dVar35;
      }
      fVar23 = (float)((ulonglong)dVar26 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        bVar57 = fVar34 < fVar23;
      }
      else {
        bVar57 = dVar31 < dVar26;
      }
      if (in_FPSCR_PR == '\0') {
        bVar58 = fVar23 < fVar39;
      }
      else {
        bVar58 = dVar26 < in_dr14;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar31 = (double)CONCAT44(*(undefined4 *)pdVar13,fVar16);
      }
      else {
        dVar31 = *pdVar13;
      }
      if ((DAT_8c09ea6c &
           ((((DAT_8c09ea70 << 1 | (uint)bVar43) << 1 | (uint)bVar44) << 1 | (uint)bVar45) << 1 |
           (uint)bVar46) &
           ((((DAT_8c09ea70 << 1 | (uint)bVar47) << 1 | (uint)bVar48) << 1 | (uint)bVar49) << 1 |
           (uint)bVar50) &
           ((((DAT_8c09ea70 << 1 | (uint)bVar51) << 1 | (uint)bVar52) << 1 | (uint)bVar53) << 1 |
           (uint)bVar54) &
          ((((DAT_8c09ea70 << 1 | (uint)bVar55) << 1 | (uint)bVar56) << 1 | (uint)bVar57) << 1 |
          (uint)bVar58)) != 0) {
        return 0;
      }
      uVar17 = (undefined4)((ulonglong)dVar31 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        uVar20 = *(undefined4 *)((int)pdVar9 + 0x1c);
        dVar31 = (double)CONCAT44(uVar17,uVar20);
        uVar28 = *(undefined4 *)(pdVar9 + 3);
        *(undefined4 *)(in_r2 + 0x18) = 0x3f800000;
        *(undefined4 *)(in_r2 + 0x14) = uVar28;
        *(undefined4 *)(in_r2 + 0x10) = uVar20;
        *(undefined4 *)(in_r2 + 0xc) = uVar17;
        *(float *)(in_r2 + 8) = fVar18;
        pdVar7 = (double *)(in_r2 + 4);
        *(float *)pdVar7 = fVar19;
      }
      else {
        uVar40 = *(undefined8 *)((int)pdVar9 + 0x1c);
        dVar29 = pdVar9[3];
        *(double *)(in_r2 + 0x14) = dVar25;
        *(double *)(in_r2 + 0xc) = dVar29;
        *(undefined8 *)(in_r2 + 4) = uVar40;
        *(double *)(in_r2 + -4) = dVar31;
        *(double *)(in_r2 + -0xc) = dVar22;
        pdVar7 = (double *)(in_r2 + -0x14);
        *pdVar7 = dVar32;
      }
      *(undefined4 *)((int)pdVar7 + -4) = uVar21;
      if (in_FPSCR_SZ == '\0') {
        uVar20 = *(undefined4 *)((int)pdVar9 + 0x14);
        dVar31 = (double)CONCAT44(uVar17,uVar20);
        uVar28 = *(undefined4 *)(pdVar9 + 2);
        *(undefined4 *)((int)pdVar7 + 0x34) = 0x3f800000;
        *(undefined4 *)(pdVar7 + 6) = uVar28;
        *(undefined4 *)((int)pdVar7 + 0x2c) = uVar20;
        *(undefined4 *)(pdVar7 + 5) = uVar17;
        *(float *)((int)pdVar7 + 0x24) = fVar24;
        pdVar7 = pdVar7 + 4;
        *(float *)pdVar7 = fVar27;
      }
      else {
        dVar22 = *(double *)((int)pdVar9 + 0x14);
        dVar29 = pdVar9[2];
        pdVar7[6] = dVar25;
        pdVar7[5] = dVar29;
        pdVar7[4] = dVar22;
        uVar17 = (undefined4)((ulonglong)dVar31 >> 0x20);
        pdVar7[3] = dVar31;
        pdVar7[2] = dVar14;
        pdVar7 = pdVar7 + 1;
        *pdVar7 = dVar36;
      }
      *(undefined4 *)((int)pdVar7 + -4) = uVar21;
      if (in_FPSCR_SZ == '\0') {
        uVar20 = *(undefined4 *)((int)pdVar9 + 0xc);
        dVar31 = (double)CONCAT44(uVar17,uVar20);
        uVar28 = *(undefined4 *)(pdVar9 + 1);
        *(undefined4 *)((int)pdVar7 + 0x34) = 0x3f800000;
        *(undefined4 *)(pdVar7 + 6) = uVar28;
        *(undefined4 *)((int)pdVar7 + 0x2c) = uVar20;
        *(undefined4 *)(pdVar7 + 5) = uVar17;
        *(float *)((int)pdVar7 + 0x24) = fVar30;
        pdVar7 = pdVar7 + 4;
        *(float *)pdVar7 = fVar33;
      }
      else {
        dVar22 = *(double *)((int)pdVar9 + 0xc);
        dVar14 = pdVar9[1];
        pdVar7[6] = dVar25;
        pdVar7[5] = dVar14;
        pdVar7[4] = dVar22;
        uVar17 = (undefined4)((ulonglong)dVar31 >> 0x20);
        pdVar7[3] = dVar31;
        pdVar7[2] = dVar41;
        pdVar7 = pdVar7 + 1;
        *pdVar7 = dVar15;
      }
      *(undefined4 *)((int)pdVar7 + -4) = uVar21;
      if (in_FPSCR_SZ == '\0') {
        uVar21 = *(undefined4 *)((int)pdVar9 + 4);
        uVar20 = *(undefined4 *)pdVar9;
        *(undefined4 *)((int)pdVar7 + 0x34) = 0x3f800000;
        *(undefined4 *)(pdVar7 + 6) = uVar20;
        *(undefined4 *)((int)pdVar7 + 0x2c) = uVar21;
        *(undefined4 *)(pdVar7 + 5) = uVar17;
        *(float *)((int)pdVar7 + 0x24) = fVar37;
        pdVar7 = pdVar7 + 4;
        *(float *)pdVar7 = fVar23;
      }
      else {
        dVar22 = *(double *)((int)pdVar9 + 4);
        dVar14 = *pdVar9;
        pdVar7[6] = dVar25;
        pdVar7[5] = dVar14;
        pdVar7[4] = dVar22;
        pdVar7[3] = dVar31;
        pdVar7[2] = dVar42;
        pdVar7 = pdVar7 + 1;
        *pdVar7 = dVar26;
      }
      *(undefined4 *)((int)pdVar7 + -4) = uVar5;
      return 1;
    }
  }
  return 0;
}

