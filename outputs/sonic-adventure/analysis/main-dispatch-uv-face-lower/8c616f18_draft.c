/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c617376) */
/* WARNING: Removing unreachable block (ram,0x8c617340) */
/* WARNING: Removing unreachable block (ram,0x8c617262) */
/* WARNING: Removing unreachable block (ram,0x8c61722c) */
/* WARNING: Removing unreachable block (ram,0x8c6171f4) */
/* WARNING: Removing unreachable block (ram,0x8c61713c) */
/* WARNING: Removing unreachable block (ram,0x8c617200) */
/* WARNING: Removing unreachable block (ram,0x8c617238) */
/* WARNING: Removing unreachable block (ram,0x8c617272) */
/* WARNING: Removing unreachable block (ram,0x8c61734c) */
/* WARNING: Removing unreachable block (ram,0x8c617386) */
/* WARNING: Removing unreachable block (ram,0x8c61715a) */
/* WARNING: Removing unreachable block (ram,0x8c617304) */
/* WARNING: Removing unreachable block (ram,0x8c6172cc) */
/* WARNING: Removing unreachable block (ram,0x8c6172d8) */
/* WARNING: Removing unreachable block (ram,0x8c617310) */

void FUN_8c616f18(double param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                 undefined4 param_5,int param_6)

{
  short sVar1;
  int iVar2;
  int iVar3;
  short *psVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  undefined2 *puVar8;
  undefined2 *puVar9;
  double *pdVar10;
  double *pdVar11;
  int unaff_r8;
  double *pdVar12;
  int iVar13;
  int *piVar14;
  int iVar15;
  undefined4 *puVar16;
  undefined4 *puVar17;
  undefined4 in_fr1;
  undefined4 uVar18;
  float fVar19;
  double in_dr2;
  double dVar20;
  double dVar21;
  double dVar22;
  ulonglong uVar23;
  undefined8 uVar24;
  undefined4 uVar25;
  double dVar26;
  double dVar27;
  float fVar29;
  undefined8 in_dr12;
  double dVar30;
  double in_xd0;
  double in_xd2;
  double in_xd4;
  double dVar31;
  double in_xd6;
  double in_xd8;
  double in_xd10;
  bool bVar32;
  undefined2 uVar33;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  int aiStack_48 [10];
  undefined1 local_20 [4];
  undefined4 local_1c;
  undefined4 uVar28;
  
  iVar2 = DAT_8c617298;
  iVar6 = DAT_8c616fc8;
  if (in_FPSCR_SZ == '\0') {
    puVar17 = &local_1c;
    _local_20 = CONCAT44((int)((ulonglong)in_dr12 >> 0x20),local_20);
  }
  else {
    puVar17 = (undefined4 *)local_20;
  }
  puVar16 = (undefined4 *)((int)puVar17 + -0xc);
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)DAT_8c616fc4);
    dVar26 = 0.007812501848093234;
  }
  else {
    in_xd2 = *DAT_8c616fc4;
    dVar26 = in_xd6;
  }
  if (in_FPSCR_PR == '\0') {
    dVar26 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) / SUB84(in_dr2,0),SUB84(dVar26,0));
  }
  else {
    dVar26 = dVar26 / in_dr2;
  }
  pdVar12 = (double *)((int)puVar17 + -0x30);
  *(undefined4 **)((int)puVar17 + -0x44) = puVar16;
  *puVar16 = param_3;
  *(undefined4 *)((int)puVar17 + -8) = param_4;
  *(undefined4 *)((int)puVar17 + -4) = param_5;
  iVar13 = 0;
  piVar14 = *(int **)((int)puVar17 + -0x44);
  pdVar10 = pdVar12;
  pdVar11 = pdVar12;
  do {
    uVar28 = (undefined4)((ulonglong)in_dr2 >> 0x20);
    dVar31 = in_xd4;
    if ((*(uint *)(puVar16[iVar13] + 0xc) & 1) == 0) {
      dVar31 = in_xd6;
      if (in_FPSCR_SZ == '\0') {
        param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),SUB84(dVar26,0));
        dVar31 = in_xd4;
      }
      *(undefined4 **)((int)puVar17 + -0x40) = puVar16 + iVar13;
      iVar7 = puVar16[iVar13];
      *(int *)((int)puVar17 + -0x44) = iVar7;
      if (in_FPSCR_SZ == '\0') {
        in_dr2 = (double)CONCAT44(uVar28,*(undefined4 *)(iVar7 + 0x18));
      }
      else {
        in_xd2 = *(double *)(iVar7 + 0x18);
      }
      if (in_FPSCR_PR == '\0') {
        param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),
                                   SUB84(param_1,0) / SUB84(in_dr2,0));
      }
      else {
        param_1 = param_1 / in_dr2;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar20 = (double)CONCAT44(*(undefined4 *)(iVar7 + 0x10),*(undefined4 *)(DAT_8c616fc8 + 0xc))
        ;
      }
      else {
        in_xd2 = *(double *)(DAT_8c616fc8 + 0xc);
        dVar20 = *(double *)(iVar7 + 0x10);
      }
      if (in_FPSCR_PR == '\0') {
        dVar20 = (double)CONCAT44((float)((ulonglong)dVar20 >> 0x20) - SUB84(dVar20,0),
                                  SUB84(dVar20,0));
      }
      else {
        dVar20 = dVar20 - dVar20;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),*(undefined4 *)DAT_8c616fcc);
      }
      else {
        in_xd2 = *DAT_8c616fcc;
      }
      fVar29 = SUB84(param_1,0);
      if (in_FPSCR_PR == '\0') {
        dVar20 = (double)CONCAT44(((float)((ulonglong)dVar20 >> 0x20) * fVar29) / SUB84(dVar20,0),
                                  SUB84(dVar20,0));
      }
      else {
        dVar20 = (dVar20 * param_1) / (dVar20 * param_1);
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)pdVar10 = (int)((ulonglong)dVar20 >> 0x20);
      }
      else {
        *pdVar10 = dVar20;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar20 = (double)(ulonglong)*(uint *)(DAT_8c616fc8 + 0x10);
      }
      else {
        in_xd2 = *(double *)(DAT_8c616fc8 + 0x10);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar20 = (double)CONCAT44(*(undefined4 *)(**(int **)((int)puVar17 + -0x40) + 0x14),
                                  SUB84(dVar20,0));
      }
      else {
        dVar20 = *(double *)(**(int **)((int)puVar17 + -0x40) + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        dVar20 = (double)CONCAT44((float)((ulonglong)dVar20 >> 0x20) - SUB84(dVar20,0),
                                  SUB84(dVar20,0));
      }
      else {
        dVar20 = dVar20 - dVar20;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),*(undefined4 *)DAT_8c616fd0);
      }
      else {
        in_xd2 = *DAT_8c616fd0;
      }
      if (in_FPSCR_PR == '\0') {
        in_dr2 = (double)CONCAT44(((float)((ulonglong)dVar20 >> 0x20) * fVar29) / SUB84(dVar20,0),
                                  SUB84(dVar20,0));
      }
      else {
        in_dr2 = (dVar20 * param_1) / (dVar20 * param_1);
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar10 + 4) = (int)((ulonglong)in_dr2 >> 0x20);
        *(float *)(pdVar10 + 1) = fVar29;
      }
      else {
        *(double *)((int)pdVar10 + 4) = in_dr2;
        pdVar10[1] = dVar31;
      }
    }
    else {
      if (in_FPSCR_SZ == '\0') {
        uVar18 = *(undefined4 *)(*piVar14 + 0x10);
        in_dr2 = (double)CONCAT44(uVar28,uVar18);
        *(undefined4 *)pdVar11 = uVar18;
      }
      else {
        in_xd2 = *(double *)(*piVar14 + 0x10);
        *pdVar11 = in_xd2;
      }
      if (in_FPSCR_SZ == '\0') {
        uVar28 = *(undefined4 *)(*piVar14 + 0x14);
        in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),uVar28);
        *(undefined4 *)((int)pdVar11 + 4) = uVar28;
      }
      else {
        in_xd2 = *(double *)(*piVar14 + 0x14);
        *(double *)((int)pdVar11 + 4) = in_xd2;
      }
      if (in_FPSCR_SZ == '\0') {
        uVar28 = *(undefined4 *)(*piVar14 + 0x18);
        in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),uVar28);
        *(undefined4 *)(pdVar11 + 1) = uVar28;
      }
      else {
        in_xd2 = *(double *)(*piVar14 + 0x18);
        pdVar11[1] = in_xd2;
      }
    }
    iVar13 = iVar13 + 1;
    piVar14 = piVar14 + 1;
    pdVar11 = (double *)((int)pdVar11 + 0xc);
    pdVar10 = (double *)((int)pdVar10 + 0xc);
    in_xd4 = dVar31;
  } while (iVar13 < 3);
  if (in_FPSCR_SZ == '\0') {
    uVar28 = *(undefined4 *)((int)puVar17 + -0x28);
    param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),
                               *(undefined4 *)((int)puVar17 + -0x20));
    fVar29 = *(float *)((int)puVar17 + -0x2c);
    dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),*(undefined4 *)((int)puVar17 + -0x1c)
                             );
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),fVar29);
    dVar27 = (double)CONCAT44(uVar28,in_fr1);
    dVar20 = *pdVar12;
    dVar22 = (double)CONCAT44(uVar28,param_2);
  }
  else {
    dVar27 = *(double *)((int)puVar17 + -0x28);
    in_xd10 = *pdVar12;
    dVar31 = *(double *)((int)puVar17 + -0x20);
    in_xd2 = *(double *)((int)puVar17 + -0x2c);
    in_xd6 = *(double *)((int)puVar17 + -0x1c);
    fVar29 = (float)((ulonglong)in_xd2 >> 0x20);
    uVar28 = (undefined4)((ulonglong)dVar27 >> 0x20);
    dVar20 = in_xd2;
    dVar22 = dVar27;
  }
  if (in_FPSCR_PR == '\0') {
    dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) * SUB84(param_1,0),SUB84(dVar27,0))
    ;
    fVar19 = SUB84(in_dr2,0) * SUB84(dVar26,0);
  }
  else {
    dVar27 = dVar27 * param_1;
    fVar19 = SUB84(in_dr2 * dVar26,0);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar22 = (double)CONCAT44(uVar28,*(undefined4 *)((int)puVar17 + -0x24));
    dVar21 = (double)CONCAT44(SUB84(dVar20,0),fVar19);
  }
  else {
    in_xd8 = *(double *)((int)puVar17 + -0x24);
    dVar21 = in_xd10;
  }
  if (in_FPSCR_PR == '\0') {
    dVar21 = (double)CONCAT44((float)((ulonglong)dVar21 >> 0x20) * SUB84(dVar26,0),SUB84(dVar21,0));
  }
  else {
    dVar21 = dVar21 * dVar26;
  }
  if (in_FPSCR_SZ == '\0') {
    in_xd10 = (double)CONCAT44(SUB84(dVar20,0),(int)in_dr12);
  }
  if (in_FPSCR_PR == '\0') {
    dVar30 = (double)CONCAT44((float)((ulonglong)in_xd10 >> 0x20) * SUB84(param_1,0),
                              SUB84(in_xd10,0));
  }
  else {
    dVar30 = in_xd10 * param_1;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),SUB84(dVar22,0));
    in_xd8 = in_xd0;
  }
  if (in_FPSCR_PR == '\0') {
    dVar20 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) - SUB84(dVar21,0),
                              SUB84(dVar27,0) * fVar29);
  }
  else {
    dVar20 = dVar27 * dVar20 - dVar21;
  }
  dVar27 = dVar22;
  if (in_FPSCR_SZ == '\0') {
    dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),(int)((ulonglong)dVar22 >> 0x20));
    dVar27 = in_xd2;
  }
  if (in_FPSCR_PR == '\0') {
    fVar29 = SUB84(dVar21,0) * SUB84(dVar22,0);
    dVar22 = (double)CONCAT44((float)((ulonglong)dVar21 >> 0x20) - fVar29,fVar29);
  }
  else {
    dVar22 = dVar21 * dVar22 - dVar21 * dVar22;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)((int)puVar17 + -0x14)
                             );
  }
  else {
    dVar27 = *(double *)((int)puVar17 + -0x14);
  }
  if (in_FPSCR_PR == '\0') {
    fVar29 = SUB84(dVar22,0) * (float)((ulonglong)dVar22 >> 0x20);
  }
  else {
    fVar29 = SUB84(dVar22 * dVar22,0);
  }
  if (in_FPSCR_SZ == '\0') {
    uVar23 = CONCAT44((int)*(ulonglong *)((int)puVar17 + -0x18),fVar29);
  }
  else {
    uVar23 = *(ulonglong *)((int)puVar17 + -0x18);
  }
  if (in_FPSCR_PR == '\0') {
    uVar23 = (ulonglong)
             (uint)((float)((ulonglong)dVar20 >> 0x20) * (float)(uVar23 >> 0x20) + (float)uVar23);
    fVar29 = SUB84(dVar20,0) - (float)((ulonglong)dVar30 >> 0x20);
  }
  else {
    fVar29 = SUB84(dVar20 - dVar30,0);
  }
  if (in_FPSCR_SZ == '\0') {
    uVar23 = CONCAT44(*(undefined4 *)((int)puVar17 + -0x10),(int)uVar23);
    in_xd8 = (double)CONCAT44(fVar29,fVar29);
  }
  else {
    uVar23 = *(ulonglong *)((int)puVar17 + -0x10);
  }
  if (in_FPSCR_PR == '\0') {
    fVar29 = (float)(uVar23 >> 0x20);
    uVar23 = CONCAT44(fVar29,(float)((ulonglong)in_xd8 >> 0x20) * fVar29 + (float)uVar23);
  }
  dVar20 = dVar27;
  if (in_FPSCR_SZ == '\0') {
    param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),(int)uVar23);
    dVar20 = dVar31;
  }
  dVar31 = (double)(uVar23 & 0xffffffff00000000);
  if (in_FPSCR_PR == '\0') {
    bVar32 = SUB84(param_1,0) < 0.0;
  }
  else {
    bVar32 = param_1 < dVar31;
  }
  if (((uint)bVar32 & *DAT_8c617294) == 0) {
    *DAT_8c617294 = 0;
    iVar13 = DAT_8c61729c;
    iVar15 = 0;
    *(undefined4 *)((int)puVar17 + -0x44) = 2;
    iVar7 = 0;
    do {
      *(int *)((int)puVar17 + -0x40) = *(int *)((int)puVar17 + -0x44) * 4;
      if ((*(uint *)(puVar16[*(int *)((int)puVar17 + -0x44)] + 0xc) & 1 ^
          *(uint *)(puVar16[iVar7] + 0xc) & 1) == 1) {
        iVar3 = *(int *)((int)puVar17 + -0x44) * 0xc + (int)pdVar12;
        *(int *)((int)puVar17 + -0x3c) = iVar3;
        if (in_FPSCR_SZ == '\0') {
          param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),*(undefined4 *)(iVar3 + 8));
          dVar31 = (double)CONCAT44((int)((ulonglong)dVar31 >> 0x20),*(undefined4 *)DAT_8c6172a0);
          dVar22 = dVar20;
        }
        else {
          dVar22 = *(double *)(iVar3 + 8);
          dVar27 = *DAT_8c6172a0;
        }
        if (in_FPSCR_PR == '\0') {
          fVar29 = SUB84(dVar31,0) - SUB84(param_1,0);
        }
        else {
          fVar29 = SUB84(dVar31 - param_1,0);
        }
        iVar3 = iVar7 * 0xc + (int)pdVar12;
        *(int *)((int)puVar17 + -0x38) = iVar3;
        if (in_FPSCR_SZ == '\0') {
          dVar31 = (double)CONCAT44(*(undefined4 *)(iVar3 + 8),fVar29);
        }
        else {
          dVar31 = *(double *)(iVar3 + 8);
        }
        if (in_FPSCR_PR == '\0') {
          dVar31 = (double)CONCAT44((float)((ulonglong)dVar31 >> 0x20) - SUB84(param_1,0),
                                    SUB84(dVar31,0));
        }
        else {
          dVar31 = dVar31 - param_1;
        }
        dVar20 = dVar27;
        if (in_FPSCR_SZ == '\0') {
          param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),SUB84(dVar31,0));
          dVar20 = dVar22;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar31 = (double)CONCAT44((int)((ulonglong)dVar31 >> 0x20),
                                    *(undefined4 *)*(double **)((int)puVar17 + -0x38));
          dVar22 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),
                                    *(undefined4 *)*(double **)((int)puVar17 + -0x3c));
        }
        else {
          dVar27 = **(double **)((int)puVar17 + -0x38);
          in_xd6 = **(double **)((int)puVar17 + -0x3c);
          dVar22 = dVar26;
        }
        if (in_FPSCR_PR == '\0') {
          param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),
                                     SUB84(param_1,0) / (float)((ulonglong)dVar31 >> 0x20));
          fVar29 = SUB84(dVar31,0) - SUB84(dVar22,0);
        }
        else {
          param_1 = param_1 / dVar31;
          fVar29 = SUB84(dVar31 - dVar22,0);
        }
        uVar28 = SUB84(param_1,0);
        dVar21 = dVar20;
        dVar31 = in_xd6;
        if (in_FPSCR_SZ == '\0') {
          dVar21 = (double)CONCAT44(uVar28,SUB84(in_xd8,0));
          dVar31 = (double)CONCAT44(SUB84(dVar22,0),fVar29);
        }
        if (in_FPSCR_PR == '\0') {
          dVar31 = (double)CONCAT44((float)((ulonglong)dVar21 >> 0x20) * SUB84(dVar31,0) +
                                    (float)((ulonglong)dVar31 >> 0x20),SUB84(dVar31,0));
        }
        uVar18 = (undefined4)((ulonglong)dVar22 >> 0x20);
        dVar26 = dVar22;
        if (in_FPSCR_SZ == '\0') {
          dVar22 = (double)CONCAT44(uVar18,SUB84(dVar21,0));
          dVar26 = (double)CONCAT44(uVar18,(int)((ulonglong)dVar31 >> 0x20));
          dVar31 = in_xd6;
        }
        iVar3 = iVar15 * 0x20 + iVar2;
        *(int *)((int)puVar17 + -0x44) = iVar3;
        if (in_FPSCR_SZ == '\0') {
          dVar21 = (double)CONCAT44(*(undefined4 *)(iVar6 + 0xc),*(undefined4 *)DAT_8c6172a4);
        }
        else {
          dVar27 = *DAT_8c6172a4;
          dVar21 = *(double *)(iVar6 + 0xc);
        }
        if (in_FPSCR_PR == '\0') {
          fVar29 = SUB84(dVar21,0) * SUB84(dVar26,0);
          dVar21 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) * fVar29 +
                                    (float)((ulonglong)dVar21 >> 0x20),fVar29);
        }
        else {
          dVar21 = dVar21 * dVar26;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(iVar3 + 0x10) = (int)((ulonglong)dVar21 >> 0x20);
          dVar22 = (double)CONCAT44(uVar28,SUB84(dVar22,0));
        }
        else {
          *(double *)(iVar3 + 0x10) = dVar21;
          dVar22 = dVar20;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar26 = (double)CONCAT44(uVar18,*(undefined4 *)(*(int *)((int)puVar17 + -0x3c) + 4));
        }
        else {
          dVar31 = *(double *)(*(int *)((int)puVar17 + -0x3c) + 4);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar21 = (double)CONCAT44(SUB84(dVar26,0),
                                    *(undefined4 *)(*(int *)((int)puVar17 + -0x38) + 4));
        }
        else {
          dVar27 = *(double *)(*(int *)((int)puVar17 + -0x38) + 4);
          dVar21 = dVar31;
        }
        if (in_FPSCR_PR == '\0') {
          in_xd6 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),
                                    SUB84(dVar21,0) - SUB84(dVar26,0));
        }
        else {
          in_xd6 = dVar21 - dVar26;
        }
        if (in_FPSCR_PR == '\0') {
          in_xd6 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) * SUB84(in_xd6,0) +
                                    (float)((ulonglong)in_xd6 >> 0x20),SUB84(in_xd6,0));
        }
        if (in_FPSCR_SZ == '\0') {
          uVar18 = (undefined4)((ulonglong)in_xd6 >> 0x20);
          dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),uVar18);
          in_xd6 = dVar31;
          dVar31 = (double)CONCAT44(uVar18,*(undefined4 *)DAT_8c6172a8);
        }
        else {
          dVar27 = *DAT_8c6172a8;
          dVar31 = in_xd6;
        }
        if (in_FPSCR_PR == '\0') {
          fVar29 = SUB84(dVar31,0) * SUB84(dVar26,0);
        }
        else {
          fVar29 = SUB84(dVar31 * dVar26,0);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar24 = CONCAT44(*(undefined4 *)(iVar6 + 0x10),fVar29);
          dVar31 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),SUB84(dVar22,0));
        }
        else {
          uVar24 = *(undefined8 *)(iVar6 + 0x10);
          dVar31 = dVar26;
        }
        fVar29 = (float)((ulonglong)dVar31 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          uVar24 = CONCAT44(fVar29 * (float)uVar24 + (float)((ulonglong)uVar24 >> 0x20),
                            (float)uVar24);
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(*(int *)((int)puVar17 + -0x44) + 0x14) = (int)((ulonglong)uVar24 >> 0x20);
        }
        else {
          *(undefined8 *)(*(int *)((int)puVar17 + -0x44) + 0x14) = uVar24;
        }
        psVar4 = (short *)(iVar7 * 4 + param_6);
        if (in_FPSCR_SZ == '\0') {
          *(float *)(*(int *)((int)puVar17 + -0x44) + 0x18) = fVar29;
          in_xd8 = (double)CONCAT44(uVar28,SUB84(dVar31,0));
        }
        else {
          *(double *)(*(int *)((int)puVar17 + -0x44) + 0x18) = dVar31;
          in_xd8 = dVar20;
        }
        puVar8 = (undefined2 *)(iVar15 * 4 + iVar13);
        *(undefined2 **)((int)puVar17 + -0x3c) = puVar8;
        *(short **)((int)puVar17 + -0x38) = (short *)(*(int *)((int)puVar17 + -0x40) + param_6);
        sVar1 = *(short *)(*(int *)((int)puVar17 + -0x40) + param_6);
        *(short *)((int)puVar17 + -0x34) = sVar1;
        *(short **)((int)puVar17 + -0x44) = psVar4;
        iVar3 = (int)*psVar4 - (int)*(short *)((int)puVar17 + -0x34);
        fVar29 = (float)((ulonglong)in_xd8 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          uVar33 = (undefined2)(int)(fVar29 * (float)iVar3 + (float)(int)sVar1);
        }
        else {
          uVar33 = (undefined2)(int)(double)iVar3;
        }
        *puVar8 = uVar33;
        iVar15 = iVar15 + 1;
        sVar1 = *(short *)(*(int *)((int)puVar17 + -0x38) + 2);
        *(short *)((int)puVar17 + -0x40) = sVar1;
        if (in_FPSCR_PR == '\0') {
          fVar19 = (float)((int)*(short *)(*(int *)((int)puVar17 + -0x44) + 2) -
                          (int)*(short *)((int)puVar17 + -0x40));
          fVar29 = fVar29 * fVar19 + (float)(int)sVar1;
          dVar31 = (double)CONCAT44(fVar19,fVar29);
          uVar33 = (undefined2)(int)fVar29;
        }
        else {
          dVar31 = (double)((int)*(short *)(*(int *)((int)puVar17 + -0x44) + 2) -
                           (int)*(short *)((int)puVar17 + -0x40));
          uVar33 = (undefined2)(int)dVar31;
        }
        *(undefined2 *)(*(int *)((int)puVar17 + -0x3c) + 2) = uVar33;
      }
      if ((*(uint *)(puVar16[iVar7] + 0xc) & 1) == 0) {
        iVar5 = iVar15 * 0x20 + iVar2;
        *(int *)((int)puVar17 + -0x44) = iVar5;
        *(undefined4 **)((int)puVar17 + -0x40) = puVar16 + iVar7;
        iVar3 = puVar16[iVar7];
        if (in_FPSCR_SZ == '\0') {
          uVar28 = *(undefined4 *)(iVar3 + 0x10);
          dVar31 = (double)CONCAT44((int)((ulonglong)dVar31 >> 0x20),uVar28);
          *(undefined4 *)(iVar5 + 0x10) = uVar28;
        }
        else {
          dVar27 = *(double *)(iVar3 + 0x10);
          *(double *)(iVar5 + 0x10) = dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          uVar28 = *(undefined4 *)(**(int **)((int)puVar17 + -0x40) + 0x14);
          dVar31 = (double)CONCAT44((int)((ulonglong)dVar31 >> 0x20),uVar28);
          *(undefined4 *)(*(int *)((int)puVar17 + -0x44) + 0x14) = uVar28;
        }
        else {
          dVar27 = *(double *)(**(int **)((int)puVar17 + -0x40) + 0x14);
          *(double *)(*(int *)((int)puVar17 + -0x44) + 0x14) = dVar27;
        }
        puVar8 = (undefined2 *)(iVar7 * 4 + param_6);
        if (in_FPSCR_SZ == '\0') {
          uVar28 = *(undefined4 *)(**(int **)((int)puVar17 + -0x40) + 0x18);
          dVar31 = (double)CONCAT44((int)((ulonglong)dVar31 >> 0x20),uVar28);
          *(undefined4 *)(*(int *)((int)puVar17 + -0x44) + 0x18) = uVar28;
        }
        else {
          dVar27 = *(double *)(**(int **)((int)puVar17 + -0x40) + 0x18);
          *(double *)(*(int *)((int)puVar17 + -0x44) + 0x18) = dVar27;
        }
        puVar9 = (undefined2 *)(iVar15 * 4 + iVar13);
        *(undefined2 **)((int)puVar17 + -0x44) = puVar9;
        *(undefined2 **)((int)puVar17 + -0x40) = puVar8;
        iVar15 = iVar15 + 1;
        *puVar9 = *puVar8;
        *(undefined2 *)(*(int *)((int)puVar17 + -0x44) + 2) =
             *(undefined2 *)(*(int *)((int)puVar17 + -0x40) + 2);
      }
      uVar28 = DAT_8c6172b0;
      piVar14 = DAT_8c6172ac;
      *(int *)((int)puVar17 + -0x44) = iVar7;
      iVar7 = iVar7 + 1;
    } while (iVar7 < 3);
    if (in_FPSCR_SZ == '\0') {
      param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),(undefined4)DAT_8c6172b4);
    }
    uVar18 = (undefined4)((ulonglong)dVar31 >> 0x20);
    uVar25 = (undefined4)((ulonglong)param_1 >> 0x20);
    fVar29 = SUB84(param_1,0);
    if (iVar15 == 3) {
      *(int *)((int)puVar17 + -0x44) = iVar13;
      if (in_FPSCR_PR == '\0') {
        dVar26 = (double)CONCAT44(uVar18,(float)(int)*(short *)(iVar13 + 2) * fVar29);
      }
      else {
        dVar26 = (double)CONCAT44(uVar18,(float)(int)*(short *)(iVar13 + 2)) * param_1;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar10 = (double *)(unaff_r8 + 0x14);
        *(int *)pdVar10 = SUB84(dVar26,0);
      }
      else {
        pdVar10 = (double *)(unaff_r8 + 0x10);
        *pdVar10 = dVar27;
      }
      uVar18 = (undefined4)((ulonglong)dVar26 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar26 = (double)CONCAT44(uVar18,(float)(int)**(short **)((int)puVar17 + -0x44) * fVar29);
      }
      else {
        dVar26 = (double)CONCAT44(uVar18,(float)(int)**(short **)((int)puVar17 + -0x44)) * param_1;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar10 + -4) = SUB84(dVar26,0);
        *(undefined4 *)(pdVar10 + -1) = *(undefined4 *)(iVar2 + 0x18);
        *(undefined4 *)((int)pdVar10 + -0xc) = *(undefined4 *)(iVar2 + 0x14);
        dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),*(undefined4 *)(iVar2 + 0x10));
        pdVar10 = pdVar10 + -2;
        *(undefined4 *)pdVar10 = *(undefined4 *)(iVar2 + 0x10);
      }
      else {
        pdVar10[-1] = dVar27;
        pdVar10[-2] = *(double *)(iVar2 + 0x18);
        pdVar10[-3] = *(double *)(iVar2 + 0x14);
        dVar27 = *(double *)(iVar2 + 0x10);
        pdVar10 = pdVar10 + -4;
        *pdVar10 = dVar27;
      }
      *(undefined4 *)((int)pdVar10 + -4) = uVar28;
      *(int *)((int)puVar17 + -0x44) = iVar13 + 4;
      uVar18 = (undefined4)((ulonglong)dVar26 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar26 = (double)CONCAT44(uVar18,(float)(int)*(short *)(iVar13 + 6) * fVar29);
      }
      else {
        dVar26 = (double)CONCAT44(uVar18,(float)(int)*(short *)(iVar13 + 6)) * param_1;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar10 = pdVar10 + 6;
        *(int *)pdVar10 = SUB84(dVar26,0);
      }
      else {
        pdVar10 = (double *)((int)pdVar10 + 0x2c);
        *pdVar10 = dVar27;
      }
      uVar18 = (undefined4)((ulonglong)dVar26 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar26 = (double)CONCAT44(uVar18,(float)(int)**(short **)((int)puVar17 + -0x44) * fVar29);
      }
      else {
        dVar26 = (double)CONCAT44(uVar18,(float)(int)**(short **)((int)puVar17 + -0x44)) * param_1;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar10 + -4) = SUB84(dVar26,0);
        *(undefined4 *)(pdVar10 + -1) = *(undefined4 *)(iVar2 + 0x38);
        *(undefined4 *)((int)pdVar10 + -0xc) = *(undefined4 *)(iVar2 + 0x34);
        dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),*(undefined4 *)(iVar2 + 0x30));
        pdVar10 = pdVar10 + -2;
        *(undefined4 *)pdVar10 = *(undefined4 *)(iVar2 + 0x30);
      }
      else {
        pdVar10[-1] = dVar27;
        pdVar10[-2] = *(double *)(iVar2 + 0x38);
        pdVar10[-3] = *(double *)(iVar2 + 0x34);
        dVar27 = *(double *)(iVar2 + 0x30);
        pdVar10 = pdVar10 + -4;
        *pdVar10 = dVar27;
      }
      *(undefined4 *)((int)pdVar10 + -4) = uVar28;
      if (in_FPSCR_SZ == '\0') {
        pdVar10 = (double *)((int)pdVar10 + 0x34);
        *(undefined4 *)pdVar10 = uVar25;
      }
      else {
        pdVar10 = pdVar10 + 6;
        *pdVar10 = param_1;
      }
      uVar28 = (undefined4)((ulonglong)dVar26 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar26 = (double)CONCAT44(uVar28,(float)(int)*(short *)(iVar13 + 10) * fVar29);
      }
      else {
        dVar26 = (double)CONCAT44(uVar28,(float)(int)*(short *)(iVar13 + 10)) * param_1;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar10 = (double *)((int)pdVar10 + -4);
        *(int *)pdVar10 = SUB84(dVar26,0);
      }
      else {
        pdVar10 = pdVar10 + -1;
        *pdVar10 = dVar27;
      }
      uVar28 = DAT_8c6172b4._4_4_;
      if (in_FPSCR_PR == '\0') {
        fVar29 = (float)(int)*(short *)(iVar13 + 8) * fVar29;
      }
      else {
        fVar29 = SUB84((double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),
                                        (float)(int)*(short *)(iVar13 + 8)) * param_1,0);
      }
      if (in_FPSCR_SZ == '\0') {
        *(float *)((int)pdVar10 + -4) = fVar29;
        *(undefined4 *)(pdVar10 + -1) = *(undefined4 *)(iVar2 + 0x58);
        *(undefined4 *)((int)pdVar10 + -0xc) = *(undefined4 *)(iVar2 + 0x54);
        pdVar10 = pdVar10 + -2;
        *(undefined4 *)pdVar10 = *(undefined4 *)(iVar2 + 0x50);
      }
      else {
        pdVar10[-1] = dVar27;
        pdVar10[-2] = *(double *)(iVar2 + 0x58);
        pdVar10[-3] = *(double *)(iVar2 + 0x54);
        pdVar10 = pdVar10 + -4;
        *pdVar10 = *(double *)(iVar2 + 0x50);
      }
      *(undefined4 *)((int)pdVar10 + -4) = uVar28;
      iVar6 = *piVar14 + 3;
    }
    else {
      *(int *)((int)puVar17 + -0x44) = iVar13;
      if (in_FPSCR_PR == '\0') {
        dVar26 = (double)CONCAT44(uVar18,(float)(int)*(short *)(iVar13 + 2) * fVar29);
      }
      else {
        dVar26 = (double)CONCAT44(uVar18,(float)(int)*(short *)(iVar13 + 2)) * param_1;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar10 = (double *)(unaff_r8 + 0x14);
        *(int *)pdVar10 = SUB84(dVar26,0);
      }
      else {
        pdVar10 = (double *)(unaff_r8 + 0x10);
        *pdVar10 = dVar27;
      }
      uVar18 = (undefined4)((ulonglong)dVar26 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar26 = (double)CONCAT44(uVar18,(float)(int)**(short **)((int)puVar17 + -0x44) * fVar29);
      }
      else {
        dVar26 = (double)CONCAT44(uVar18,(float)(int)**(short **)((int)puVar17 + -0x44)) * param_1;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar10 + -4) = SUB84(dVar26,0);
        *(undefined4 *)(pdVar10 + -1) = *(undefined4 *)(iVar2 + 0x18);
        *(undefined4 *)((int)pdVar10 + -0xc) = *(undefined4 *)(iVar2 + 0x14);
        dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),*(undefined4 *)(iVar2 + 0x10));
        pdVar10 = pdVar10 + -2;
        *(undefined4 *)pdVar10 = *(undefined4 *)(iVar2 + 0x10);
      }
      else {
        pdVar10[-1] = dVar27;
        pdVar10[-2] = *(double *)(iVar2 + 0x18);
        pdVar10[-3] = *(double *)(iVar2 + 0x14);
        dVar27 = *(double *)(iVar2 + 0x10);
        pdVar10 = pdVar10 + -4;
        *pdVar10 = dVar27;
      }
      *(undefined4 *)((int)pdVar10 + -4) = uVar28;
      *(int *)((int)puVar17 + -0x44) = iVar13 + 4;
      uVar18 = (undefined4)((ulonglong)dVar26 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar26 = (double)CONCAT44(uVar18,(float)(int)*(short *)(iVar13 + 6) * fVar29);
      }
      else {
        dVar26 = (double)CONCAT44(uVar18,(float)(int)*(short *)(iVar13 + 6)) * param_1;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar10 = pdVar10 + 6;
        *(int *)pdVar10 = SUB84(dVar26,0);
      }
      else {
        pdVar10 = (double *)((int)pdVar10 + 0x2c);
        *pdVar10 = dVar27;
      }
      uVar18 = (undefined4)((ulonglong)dVar26 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar26 = (double)CONCAT44(uVar18,(float)(int)**(short **)((int)puVar17 + -0x44) * fVar29);
      }
      else {
        dVar26 = (double)CONCAT44(uVar18,(float)(int)**(short **)((int)puVar17 + -0x44)) * param_1;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar10 + -4) = SUB84(dVar26,0);
        *(undefined4 *)(pdVar10 + -1) = *(undefined4 *)(iVar2 + 0x38);
        *(undefined4 *)((int)pdVar10 + -0xc) = *(undefined4 *)(iVar2 + 0x34);
        dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),*(undefined4 *)(iVar2 + 0x30));
        pdVar10 = pdVar10 + -2;
        *(undefined4 *)pdVar10 = *(undefined4 *)(iVar2 + 0x30);
      }
      else {
        pdVar10[-1] = dVar27;
        pdVar10[-2] = *(double *)(iVar2 + 0x38);
        pdVar10[-3] = *(double *)(iVar2 + 0x34);
        dVar27 = *(double *)(iVar2 + 0x30);
        pdVar10 = pdVar10 + -4;
        *pdVar10 = dVar27;
      }
      *(undefined4 *)((int)pdVar10 + -4) = uVar28;
      if (in_FPSCR_SZ == '\0') {
        pdVar10 = (double *)((int)pdVar10 + 0x34);
        *(undefined4 *)pdVar10 = uVar25;
      }
      else {
        pdVar10 = pdVar10 + 6;
        *pdVar10 = param_1;
      }
      *(int *)((int)puVar17 + -0x44) = iVar13 + 0xc;
      uVar18 = (undefined4)((ulonglong)dVar26 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar26 = (double)CONCAT44(uVar18,(float)(int)*(short *)(iVar13 + 0xe) * fVar29);
      }
      else {
        dVar26 = (double)CONCAT44(uVar18,(float)(int)*(short *)(iVar13 + 0xe)) * param_1;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar10 = (double *)((int)pdVar10 + -4);
        *(int *)pdVar10 = SUB84(dVar26,0);
      }
      else {
        pdVar10 = pdVar10 + -1;
        *pdVar10 = dVar27;
      }
      uVar18 = (undefined4)((ulonglong)dVar26 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar26 = (double)CONCAT44(uVar18,(float)(int)**(short **)((int)puVar17 + -0x44) * fVar29);
      }
      else {
        dVar26 = (double)CONCAT44(uVar18,(float)(int)**(short **)((int)puVar17 + -0x44)) * param_1;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar10 + -4) = SUB84(dVar26,0);
        *(undefined4 *)(pdVar10 + -1) = *(undefined4 *)(iVar2 + 0x78);
        *(undefined4 *)((int)pdVar10 + -0xc) = *(undefined4 *)(iVar2 + 0x74);
        dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),*(undefined4 *)(iVar2 + 0x70));
        pdVar10 = pdVar10 + -2;
        *(undefined4 *)pdVar10 = *(undefined4 *)(iVar2 + 0x70);
      }
      else {
        pdVar10[-1] = dVar27;
        pdVar10[-2] = *(double *)(iVar2 + 0x78);
        pdVar10[-3] = *(double *)(iVar2 + 0x74);
        dVar27 = *(double *)(iVar2 + 0x70);
        pdVar10 = pdVar10 + -4;
        *pdVar10 = dVar27;
      }
      *(undefined4 *)((int)pdVar10 + -4) = uVar28;
      if (in_FPSCR_SZ == '\0') {
        pdVar10 = (double *)((int)pdVar10 + 0x34);
        *(undefined4 *)pdVar10 = uVar25;
      }
      else {
        pdVar10 = pdVar10 + 6;
        *pdVar10 = param_1;
      }
      uVar28 = (undefined4)((ulonglong)dVar26 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar26 = (double)CONCAT44(uVar28,(float)(int)*(short *)(iVar13 + 10) * fVar29);
      }
      else {
        dVar26 = (double)CONCAT44(uVar28,(float)(int)*(short *)(iVar13 + 10)) * param_1;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar10 = (double *)((int)pdVar10 + -4);
        *(int *)pdVar10 = SUB84(dVar26,0);
      }
      else {
        pdVar10 = pdVar10 + -1;
        *pdVar10 = dVar27;
      }
      uVar28 = DAT_8c6174bc;
      if (in_FPSCR_PR == '\0') {
        fVar29 = (float)(int)*(short *)(iVar13 + 8) * fVar29;
      }
      else {
        fVar29 = SUB84((double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),
                                        (float)(int)*(short *)(iVar13 + 8)) * param_1,0);
      }
      if (in_FPSCR_SZ == '\0') {
        *(float *)((int)pdVar10 + -4) = fVar29;
        *(undefined4 *)(pdVar10 + -1) = *(undefined4 *)(iVar2 + 0x58);
        *(undefined4 *)((int)pdVar10 + -0xc) = *(undefined4 *)(iVar2 + 0x54);
        pdVar10 = pdVar10 + -2;
        *(undefined4 *)pdVar10 = *(undefined4 *)(iVar2 + 0x50);
      }
      else {
        pdVar10[-1] = dVar27;
        pdVar10[-2] = *(double *)(iVar2 + 0x58);
        pdVar10[-3] = *(double *)(iVar2 + 0x54);
        pdVar10 = pdVar10 + -4;
        *pdVar10 = *(double *)(iVar2 + 0x50);
      }
      *(undefined4 *)((int)pdVar10 + -4) = uVar28;
      iVar6 = *piVar14 + 4;
    }
    *piVar14 = iVar6;
  }
  return;
}

