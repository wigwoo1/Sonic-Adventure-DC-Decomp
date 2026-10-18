/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c612916) */
/* WARNING: Removing unreachable block (ram,0x8c6128e0) */
/* WARNING: Removing unreachable block (ram,0x8c6127f8) */
/* WARNING: Removing unreachable block (ram,0x8c6127c2) */
/* WARNING: Removing unreachable block (ram,0x8c612788) */
/* WARNING: Removing unreachable block (ram,0x8c6126d0) */
/* WARNING: Removing unreachable block (ram,0x8c612794) */
/* WARNING: Removing unreachable block (ram,0x8c6127ce) */
/* WARNING: Removing unreachable block (ram,0x8c612808) */
/* WARNING: Removing unreachable block (ram,0x8c6128ec) */
/* WARNING: Removing unreachable block (ram,0x8c612926) */
/* WARNING: Removing unreachable block (ram,0x8c6126ee) */
/* WARNING: Removing unreachable block (ram,0x8c6128a4) */
/* WARNING: Removing unreachable block (ram,0x8c61286a) */
/* WARNING: Removing unreachable block (ram,0x8c612876) */
/* WARNING: Removing unreachable block (ram,0x8c6128b0) */

void FUN_8c6124c0(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                 undefined4 param_5,int param_6)

{
  short sVar1;
  int iVar2;
  short *psVar3;
  int iVar4;
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
  float fVar20;
  double in_dr2;
  double dVar21;
  double dVar22;
  double dVar23;
  ulonglong uVar24;
  undefined8 uVar25;
  undefined4 uVar26;
  double dVar27;
  double dVar28;
  double dVar29;
  float fVar31;
  undefined8 in_dr12;
  double dVar32;
  double in_xd0;
  double in_xd2;
  double in_xd4;
  double in_xd6;
  double in_xd8;
  double in_xd10;
  bool bVar33;
  undefined2 uVar34;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  int aiStack_48 [10];
  undefined1 local_20 [4];
  undefined4 local_1c;
  undefined4 uVar30;
  
  iVar2 = DAT_8c612834;
  iVar6 = DAT_8c61255c;
  if (in_FPSCR_SZ == '\0') {
    puVar17 = &local_1c;
    _local_20 = CONCAT44((int)((ulonglong)in_dr12 >> 0x20),local_20);
  }
  else {
    puVar17 = (undefined4 *)local_20;
  }
  dVar27 = 5.26354424712089e-315;
  puVar16 = (undefined4 *)((int)puVar17 + -0xc);
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)DAT_8c612558);
    dVar28 = (double)CONCAT44(0x3f800000,param_1);
  }
  else {
    in_xd2 = *DAT_8c612558;
    dVar28 = in_xd4;
  }
  if (in_FPSCR_PR == '\0') {
    dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) / SUB84(in_dr2,0),SUB84(dVar28,0));
  }
  else {
    dVar28 = dVar28 / in_dr2;
  }
  pdVar12 = (double *)((int)puVar17 + -0x30);
  *(undefined4 **)((int)puVar17 + -0x44) = puVar16;
  *puVar16 = param_3;
  *(undefined4 *)((int)puVar17 + -8) = param_4;
  iVar13 = 0;
  *(undefined4 *)((int)puVar17 + -4) = param_5;
  piVar14 = *(int **)((int)puVar17 + -0x44);
  pdVar10 = pdVar12;
  pdVar11 = pdVar12;
  do {
    uVar30 = (undefined4)((ulonglong)in_dr2 >> 0x20);
    if ((*(uint *)(puVar16[iVar13] + 0xc) & 1) == 0) {
      dVar21 = in_xd4;
      if (in_FPSCR_SZ == '\0') {
        dVar21 = (double)CONCAT44(SUB84(dVar27,0),SUB84(dVar27,0));
      }
      *(undefined4 **)((int)puVar17 + -0x44) = puVar16 + iVar13;
      iVar7 = puVar16[iVar13];
      *(int *)((int)puVar17 + -0x40) = iVar7;
      if (in_FPSCR_SZ == '\0') {
        in_dr2 = (double)CONCAT44(uVar30,*(undefined4 *)(iVar7 + 0x18));
      }
      else {
        in_xd2 = *(double *)(iVar7 + 0x18);
      }
      if (in_FPSCR_PR == '\0') {
        dVar27 = (double)CONCAT44((float)((ulonglong)dVar21 >> 0x20) / SUB84(in_dr2,0),
                                  SUB84(dVar21,0));
      }
      else {
        dVar27 = dVar21 / in_dr2;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar21 = (double)CONCAT44(*(undefined4 *)(iVar7 + 0x10),*(undefined4 *)(DAT_8c61255c + 0xc))
        ;
      }
      else {
        in_xd2 = *(double *)(DAT_8c61255c + 0xc);
        dVar21 = *(double *)(iVar7 + 0x10);
      }
      if (in_FPSCR_PR == '\0') {
        dVar21 = (double)CONCAT44((float)((ulonglong)dVar21 >> 0x20) - SUB84(dVar21,0),
                                  SUB84(dVar21,0));
      }
      else {
        dVar21 = dVar21 - dVar21;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),*(undefined4 *)DAT_8c612560);
      }
      else {
        in_xd2 = *DAT_8c612560;
      }
      fVar31 = (float)((ulonglong)dVar27 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar21 = (double)CONCAT44(((float)((ulonglong)dVar21 >> 0x20) * fVar31) / SUB84(dVar21,0),
                                  SUB84(dVar21,0));
      }
      else {
        dVar21 = (dVar21 * dVar27) / (dVar21 * dVar27);
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)pdVar11 = (int)((ulonglong)dVar21 >> 0x20);
      }
      else {
        *pdVar11 = dVar21;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar21 = (double)(ulonglong)*(uint *)(DAT_8c61255c + 0x10);
      }
      else {
        in_xd2 = *(double *)(DAT_8c61255c + 0x10);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar21 = (double)CONCAT44(*(undefined4 *)(**(int **)((int)puVar17 + -0x44) + 0x14),
                                  SUB84(dVar21,0));
      }
      else {
        dVar21 = *(double *)(**(int **)((int)puVar17 + -0x44) + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        dVar21 = (double)CONCAT44((float)((ulonglong)dVar21 >> 0x20) - SUB84(dVar21,0),
                                  SUB84(dVar21,0));
      }
      else {
        dVar21 = dVar21 - dVar21;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),*(undefined4 *)DAT_8c612564);
      }
      else {
        in_xd2 = *DAT_8c612564;
      }
      if (in_FPSCR_PR == '\0') {
        in_dr2 = (double)CONCAT44(((float)((ulonglong)dVar21 >> 0x20) * fVar31) / SUB84(dVar21,0),
                                  SUB84(dVar21,0));
      }
      else {
        in_dr2 = (dVar21 * dVar27) / (dVar21 * dVar27);
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar11 + 4) = (int)((ulonglong)in_dr2 >> 0x20);
        *(float *)(pdVar11 + 1) = fVar31;
      }
      else {
        *(double *)((int)pdVar11 + 4) = in_dr2;
        pdVar11[1] = dVar27;
      }
    }
    else {
      if (in_FPSCR_SZ == '\0') {
        uVar18 = *(undefined4 *)(*piVar14 + 0x10);
        in_dr2 = (double)CONCAT44(uVar30,uVar18);
        *(undefined4 *)pdVar10 = uVar18;
      }
      else {
        in_xd2 = *(double *)(*piVar14 + 0x10);
        *pdVar10 = in_xd2;
      }
      if (in_FPSCR_SZ == '\0') {
        uVar30 = *(undefined4 *)(*piVar14 + 0x14);
        in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),uVar30);
        *(undefined4 *)((int)pdVar10 + 4) = uVar30;
      }
      else {
        in_xd2 = *(double *)(*piVar14 + 0x14);
        *(double *)((int)pdVar10 + 4) = in_xd2;
      }
      if (in_FPSCR_SZ == '\0') {
        uVar30 = *(undefined4 *)(*piVar14 + 0x18);
        in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),uVar30);
        *(undefined4 *)(pdVar10 + 1) = uVar30;
      }
      else {
        in_xd2 = *(double *)(*piVar14 + 0x18);
        pdVar10[1] = in_xd2;
      }
    }
    iVar13 = iVar13 + 1;
    piVar14 = piVar14 + 1;
    pdVar10 = (double *)((int)pdVar10 + 0xc);
    pdVar11 = (double *)((int)pdVar11 + 0xc);
  } while (iVar13 < 3);
  if (in_FPSCR_SZ == '\0') {
    uVar30 = *(undefined4 *)((int)puVar17 + -0x28);
    dVar21 = (double)CONCAT44(*(undefined4 *)((int)puVar17 + -0x20),SUB84(dVar27,0));
    fVar31 = *(float *)((int)puVar17 + -0x2c);
    dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),*(undefined4 *)((int)puVar17 + -0x1c)
                             );
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),fVar31);
    dVar29 = (double)CONCAT44(uVar30,in_fr1);
    dVar27 = *pdVar12;
    dVar23 = (double)CONCAT44(uVar30,param_2);
  }
  else {
    dVar29 = *(double *)((int)puVar17 + -0x28);
    in_xd10 = *pdVar12;
    dVar21 = *(double *)((int)puVar17 + -0x20);
    in_xd2 = *(double *)((int)puVar17 + -0x2c);
    in_xd6 = *(double *)((int)puVar17 + -0x1c);
    fVar31 = (float)((ulonglong)in_xd2 >> 0x20);
    uVar30 = (undefined4)((ulonglong)dVar29 >> 0x20);
    dVar27 = in_xd2;
    dVar23 = dVar29;
  }
  fVar20 = (float)((ulonglong)dVar21 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar29 = (double)CONCAT44((float)((ulonglong)dVar29 >> 0x20) * fVar20,SUB84(dVar29,0));
    fVar19 = SUB84(in_dr2,0) * SUB84(dVar28,0);
  }
  else {
    dVar29 = dVar29 * dVar21;
    fVar19 = SUB84(in_dr2 * dVar28,0);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar23 = (double)CONCAT44(uVar30,*(undefined4 *)((int)puVar17 + -0x24));
    dVar22 = (double)CONCAT44(SUB84(dVar27,0),fVar19);
  }
  else {
    in_xd8 = *(double *)((int)puVar17 + -0x24);
    dVar22 = in_xd10;
  }
  if (in_FPSCR_PR == '\0') {
    dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) * SUB84(dVar28,0),SUB84(dVar22,0));
  }
  else {
    dVar22 = dVar22 * dVar28;
  }
  if (in_FPSCR_SZ == '\0') {
    in_xd10 = (double)CONCAT44(SUB84(dVar27,0),(int)in_dr12);
  }
  if (in_FPSCR_PR == '\0') {
    dVar32 = (double)CONCAT44((float)((ulonglong)in_xd10 >> 0x20) * fVar20,SUB84(in_xd10,0));
  }
  else {
    dVar32 = in_xd10 * dVar21;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar29 = (double)CONCAT44((int)((ulonglong)dVar29 >> 0x20),SUB84(dVar23,0));
    in_xd8 = in_xd0;
  }
  if (in_FPSCR_PR == '\0') {
    dVar27 = (double)CONCAT44((float)((ulonglong)dVar29 >> 0x20) - SUB84(dVar22,0),
                              SUB84(dVar29,0) * fVar31);
  }
  else {
    dVar27 = dVar29 * dVar27 - dVar22;
  }
  dVar29 = dVar23;
  if (in_FPSCR_SZ == '\0') {
    dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),(int)((ulonglong)dVar23 >> 0x20));
    dVar29 = in_xd2;
  }
  if (in_FPSCR_PR == '\0') {
    fVar31 = SUB84(dVar22,0) * SUB84(dVar23,0);
    dVar23 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) - fVar31,fVar31);
  }
  else {
    dVar23 = dVar22 * dVar23 - dVar22 * dVar23;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(undefined4 *)((int)puVar17 + -0x14)
                             );
  }
  else {
    dVar29 = *(double *)((int)puVar17 + -0x14);
  }
  if (in_FPSCR_PR == '\0') {
    fVar31 = SUB84(dVar23,0) * (float)((ulonglong)dVar23 >> 0x20);
  }
  else {
    fVar31 = SUB84(dVar23 * dVar23,0);
  }
  if (in_FPSCR_SZ == '\0') {
    uVar24 = CONCAT44((int)*(ulonglong *)((int)puVar17 + -0x18),fVar31);
  }
  else {
    uVar24 = *(ulonglong *)((int)puVar17 + -0x18);
  }
  if (in_FPSCR_PR == '\0') {
    uVar24 = (ulonglong)
             (uint)((float)((ulonglong)dVar27 >> 0x20) * (float)(uVar24 >> 0x20) + (float)uVar24);
    fVar31 = SUB84(dVar27,0) - (float)((ulonglong)dVar32 >> 0x20);
  }
  else {
    fVar31 = SUB84(dVar27 - dVar32,0);
  }
  if (in_FPSCR_SZ == '\0') {
    uVar24 = CONCAT44(*(undefined4 *)((int)puVar17 + -0x10),(int)uVar24);
    in_xd8 = (double)CONCAT44(fVar31,fVar31);
  }
  else {
    uVar24 = *(ulonglong *)((int)puVar17 + -0x10);
  }
  if (in_FPSCR_PR == '\0') {
    fVar31 = (float)(uVar24 >> 0x20);
    uVar24 = CONCAT44(fVar31,(float)((ulonglong)in_xd8 >> 0x20) * fVar31 + (float)uVar24);
  }
  dVar27 = dVar29;
  if (in_FPSCR_SZ == '\0') {
    dVar27 = (double)CONCAT44((int)uVar24,SUB84(dVar21,0));
  }
  dVar21 = (double)(uVar24 & 0xffffffff00000000);
  if (in_FPSCR_PR == '\0') {
    bVar33 = (float)((ulonglong)dVar27 >> 0x20) < 0.0;
  }
  else {
    bVar33 = dVar27 < dVar21;
  }
  if (((uint)bVar33 & *DAT_8c612830) == 0) {
    *DAT_8c612830 = 0;
    iVar13 = DAT_8c612838;
    iVar15 = 0;
    *(undefined4 *)((int)puVar17 + -0x3c) = 2;
    iVar7 = 0;
    do {
      *(int *)((int)puVar17 + -0x38) = *(int *)((int)puVar17 + -0x3c) * 4;
      if ((*(uint *)(puVar16[*(int *)((int)puVar17 + -0x3c)] + 0xc) & 1 ^
          *(uint *)(puVar16[iVar7] + 0xc) & 1) == 1) {
        iVar4 = *(int *)((int)puVar17 + -0x3c) * 0xc + (int)pdVar12;
        *(int *)((int)puVar17 + -0x44) = iVar4;
        if (in_FPSCR_SZ == '\0') {
          dVar23 = (double)CONCAT44(*(undefined4 *)(iVar4 + 8),SUB84(dVar27,0));
          dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),*(undefined4 *)DAT_8c61283c);
        }
        else {
          dVar23 = *(double *)(iVar4 + 8);
          dVar29 = *DAT_8c61283c;
        }
        fVar31 = (float)((ulonglong)dVar23 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          fVar20 = SUB84(dVar21,0) - fVar31;
        }
        else {
          fVar20 = SUB84(dVar21 - dVar23,0);
        }
        iVar4 = iVar7 * 0xc + (int)pdVar12;
        *(int *)((int)puVar17 + -0x40) = iVar4;
        if (in_FPSCR_SZ == '\0') {
          dVar21 = (double)CONCAT44(*(undefined4 *)(iVar4 + 8),fVar20);
        }
        else {
          dVar21 = *(double *)(iVar4 + 8);
        }
        if (in_FPSCR_PR == '\0') {
          dVar21 = (double)CONCAT44((float)((ulonglong)dVar21 >> 0x20) - fVar31,SUB84(dVar21,0));
        }
        else {
          dVar21 = dVar21 - dVar23;
        }
        dVar27 = dVar29;
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(SUB84(dVar21,0),SUB84(dVar23,0));
        }
        if (in_FPSCR_SZ == '\0') {
          dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),
                                    *(undefined4 *)*(double **)((int)puVar17 + -0x40));
          dVar23 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),
                                    *(undefined4 *)*(double **)((int)puVar17 + -0x44));
        }
        else {
          dVar29 = **(double **)((int)puVar17 + -0x40);
          in_xd6 = **(double **)((int)puVar17 + -0x44);
          dVar23 = dVar28;
        }
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) /
                                    (float)((ulonglong)dVar21 >> 0x20),SUB84(dVar27,0));
          fVar31 = SUB84(dVar21,0) - SUB84(dVar23,0);
        }
        else {
          dVar27 = dVar27 / dVar21;
          fVar31 = SUB84(dVar21 - dVar23,0);
        }
        uVar30 = (undefined4)((ulonglong)dVar27 >> 0x20);
        dVar22 = dVar27;
        dVar21 = in_xd6;
        if (in_FPSCR_SZ == '\0') {
          dVar22 = (double)CONCAT44(uVar30,SUB84(in_xd8,0));
          dVar21 = (double)CONCAT44(SUB84(dVar23,0),fVar31);
        }
        if (in_FPSCR_PR == '\0') {
          dVar21 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) * SUB84(dVar21,0) +
                                    (float)((ulonglong)dVar21 >> 0x20),SUB84(dVar21,0));
        }
        uVar18 = (undefined4)((ulonglong)dVar23 >> 0x20);
        dVar28 = dVar23;
        if (in_FPSCR_SZ == '\0') {
          dVar23 = (double)CONCAT44(uVar18,SUB84(dVar22,0));
          dVar28 = (double)CONCAT44(uVar18,(int)((ulonglong)dVar21 >> 0x20));
          dVar21 = in_xd6;
        }
        iVar4 = iVar15 * 0x20 + iVar13;
        *(int *)((int)puVar17 + -0x3c) = iVar4;
        if (in_FPSCR_SZ == '\0') {
          dVar22 = (double)CONCAT44(*(undefined4 *)(iVar6 + 0xc),*(undefined4 *)DAT_8c612840);
        }
        else {
          dVar29 = *DAT_8c612840;
          dVar22 = *(double *)(iVar6 + 0xc);
        }
        if (in_FPSCR_PR == '\0') {
          fVar31 = SUB84(dVar22,0) * SUB84(dVar28,0);
          dVar22 = (double)CONCAT44((float)((ulonglong)dVar23 >> 0x20) * fVar31 +
                                    (float)((ulonglong)dVar22 >> 0x20),fVar31);
        }
        else {
          dVar22 = dVar22 * dVar28;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(iVar4 + 0x10) = (int)((ulonglong)dVar22 >> 0x20);
          dVar23 = (double)CONCAT44(uVar30,SUB84(dVar23,0));
        }
        else {
          *(double *)(iVar4 + 0x10) = dVar22;
          dVar23 = dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(uVar18,*(undefined4 *)(*(int *)((int)puVar17 + -0x44) + 4));
        }
        else {
          dVar21 = *(double *)(*(int *)((int)puVar17 + -0x44) + 4);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar22 = (double)CONCAT44(SUB84(dVar28,0),
                                    *(undefined4 *)(*(int *)((int)puVar17 + -0x40) + 4));
        }
        else {
          dVar29 = *(double *)(*(int *)((int)puVar17 + -0x40) + 4);
          dVar22 = dVar21;
        }
        if (in_FPSCR_PR == '\0') {
          in_xd6 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                    SUB84(dVar22,0) - SUB84(dVar28,0));
        }
        else {
          in_xd6 = dVar22 - dVar28;
        }
        if (in_FPSCR_PR == '\0') {
          in_xd6 = (double)CONCAT44((float)((ulonglong)dVar23 >> 0x20) * SUB84(in_xd6,0) +
                                    (float)((ulonglong)in_xd6 >> 0x20),SUB84(in_xd6,0));
        }
        if (in_FPSCR_SZ == '\0') {
          uVar18 = (undefined4)((ulonglong)in_xd6 >> 0x20);
          dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),uVar18);
          in_xd6 = dVar21;
          dVar21 = (double)CONCAT44(uVar18,*(undefined4 *)DAT_8c612844);
        }
        else {
          dVar29 = *DAT_8c612844;
          dVar21 = in_xd6;
        }
        if (in_FPSCR_PR == '\0') {
          fVar31 = SUB84(dVar21,0) * SUB84(dVar28,0);
        }
        else {
          fVar31 = SUB84(dVar21 * dVar28,0);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar25 = CONCAT44(*(undefined4 *)(iVar6 + 0x10),fVar31);
          dVar21 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),SUB84(dVar23,0));
        }
        else {
          uVar25 = *(undefined8 *)(iVar6 + 0x10);
          dVar21 = dVar28;
        }
        fVar31 = (float)((ulonglong)dVar21 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          uVar25 = CONCAT44(fVar31 * (float)uVar25 + (float)((ulonglong)uVar25 >> 0x20),
                            (float)uVar25);
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(*(int *)((int)puVar17 + -0x3c) + 0x14) = (int)((ulonglong)uVar25 >> 0x20);
        }
        else {
          *(undefined8 *)(*(int *)((int)puVar17 + -0x3c) + 0x14) = uVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          *(float *)(*(int *)((int)puVar17 + -0x3c) + 0x18) = fVar31;
        }
        else {
          *(double *)(*(int *)((int)puVar17 + -0x3c) + 0x18) = dVar21;
        }
        psVar3 = (short *)(iVar7 * 4 + param_6);
        in_xd8 = dVar27;
        if (in_FPSCR_SZ == '\0') {
          in_xd8 = (double)CONCAT44(uVar30,SUB84(dVar21,0));
        }
        puVar8 = (undefined2 *)(iVar15 * 4 + iVar2);
        *(undefined2 **)((int)puVar17 + -0x40) = puVar8;
        *(short **)((int)puVar17 + -0x3c) = (short *)(*(int *)((int)puVar17 + -0x38) + param_6);
        sVar1 = *(short *)(*(int *)((int)puVar17 + -0x38) + param_6);
        *(short *)((int)puVar17 + -0x44) = sVar1;
        *(short **)((int)puVar17 + -0x34) = psVar3;
        iVar4 = (int)*psVar3 - (int)*(short *)((int)puVar17 + -0x44);
        fVar31 = (float)((ulonglong)in_xd8 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          uVar34 = (undefined2)(int)(fVar31 * (float)iVar4 + (float)(int)sVar1);
        }
        else {
          uVar34 = (undefined2)(int)(double)iVar4;
        }
        *puVar8 = uVar34;
        iVar15 = iVar15 + 1;
        sVar1 = *(short *)(*(int *)((int)puVar17 + -0x3c) + 2);
        *(short *)((int)puVar17 + -0x44) = sVar1;
        if (in_FPSCR_PR == '\0') {
          fVar20 = (float)((int)*(short *)(*(int *)((int)puVar17 + -0x34) + 2) -
                          (int)*(short *)((int)puVar17 + -0x44));
          fVar31 = fVar31 * fVar20 + (float)(int)sVar1;
          dVar21 = (double)CONCAT44(fVar20,fVar31);
          uVar34 = (undefined2)(int)fVar31;
        }
        else {
          dVar21 = (double)((int)*(short *)(*(int *)((int)puVar17 + -0x34) + 2) -
                           (int)*(short *)((int)puVar17 + -0x44));
          uVar34 = (undefined2)(int)dVar21;
        }
        *(undefined2 *)(*(int *)((int)puVar17 + -0x40) + 2) = uVar34;
      }
      if ((*(uint *)(puVar16[iVar7] + 0xc) & 1) == 0) {
        iVar5 = iVar15 * 0x20 + iVar13;
        *(int *)((int)puVar17 + -0x44) = iVar5;
        *(undefined4 **)((int)puVar17 + -0x40) = puVar16 + iVar7;
        iVar4 = puVar16[iVar7];
        if (in_FPSCR_SZ == '\0') {
          uVar30 = *(undefined4 *)(iVar4 + 0x10);
          dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),uVar30);
          *(undefined4 *)(iVar5 + 0x10) = uVar30;
        }
        else {
          dVar29 = *(double *)(iVar4 + 0x10);
          *(double *)(iVar5 + 0x10) = dVar29;
        }
        if (in_FPSCR_SZ == '\0') {
          uVar30 = *(undefined4 *)(**(int **)((int)puVar17 + -0x40) + 0x14);
          dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),uVar30);
          *(undefined4 *)(*(int *)((int)puVar17 + -0x44) + 0x14) = uVar30;
        }
        else {
          dVar29 = *(double *)(**(int **)((int)puVar17 + -0x40) + 0x14);
          *(double *)(*(int *)((int)puVar17 + -0x44) + 0x14) = dVar29;
        }
        puVar8 = (undefined2 *)(iVar7 * 4 + param_6);
        if (in_FPSCR_SZ == '\0') {
          uVar30 = *(undefined4 *)(**(int **)((int)puVar17 + -0x40) + 0x18);
          dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),uVar30);
          *(undefined4 *)(*(int *)((int)puVar17 + -0x44) + 0x18) = uVar30;
        }
        else {
          dVar29 = *(double *)(**(int **)((int)puVar17 + -0x40) + 0x18);
          *(double *)(*(int *)((int)puVar17 + -0x44) + 0x18) = dVar29;
        }
        puVar9 = (undefined2 *)(iVar15 * 4 + iVar2);
        *(undefined2 **)((int)puVar17 + -0x40) = puVar9;
        *(undefined2 **)((int)puVar17 + -0x44) = puVar8;
        iVar15 = iVar15 + 1;
        *puVar9 = *puVar8;
        *(undefined2 *)(*(int *)((int)puVar17 + -0x40) + 2) =
             *(undefined2 *)(*(int *)((int)puVar17 + -0x44) + 2);
      }
      uVar30 = DAT_8c61284c;
      piVar14 = DAT_8c612848;
      *(int *)((int)puVar17 + -0x3c) = iVar7;
      iVar7 = iVar7 + 1;
    } while (iVar7 < 3);
    if (in_FPSCR_SZ == '\0') {
      dVar27 = (double)CONCAT44((undefined4)DAT_8c612850,SUB84(dVar27,0));
    }
    else {
      dVar27 = (double)CONCAT44(DAT_8c612850._4_4_,(undefined4)DAT_8c612850);
    }
    uVar26 = SUB84(dVar27,0);
    uVar18 = (undefined4)((ulonglong)dVar21 >> 0x20);
    fVar31 = (float)((ulonglong)dVar27 >> 0x20);
    if (iVar15 == 3) {
      if (in_FPSCR_SZ == '\0') {
        pdVar10 = (double *)(unaff_r8 + 0x18);
        *(undefined4 *)pdVar10 = uVar26;
      }
      else {
        pdVar10 = (double *)(unaff_r8 + 0x14);
        *pdVar10 = in_xd4;
      }
      *(int *)((int)puVar17 + -0x44) = iVar2;
      if (in_FPSCR_PR == '\0') {
        dVar28 = (double)CONCAT44(uVar18,(float)(int)*(short *)(iVar2 + 2) * fVar31);
      }
      else {
        dVar28 = (double)CONCAT44(uVar18,(float)(int)*(short *)(iVar2 + 2)) * dVar27;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar10 = (double *)((int)pdVar10 + -4);
        *(int *)pdVar10 = SUB84(dVar28,0);
      }
      else {
        pdVar10 = pdVar10 + -1;
        *pdVar10 = dVar29;
      }
      uVar18 = (undefined4)((ulonglong)dVar28 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar28 = (double)CONCAT44(uVar18,(float)(int)**(short **)((int)puVar17 + -0x44) * fVar31);
      }
      else {
        dVar28 = (double)CONCAT44(uVar18,(float)(int)**(short **)((int)puVar17 + -0x44)) * dVar27;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar10 + -4) = SUB84(dVar28,0);
        *(undefined4 *)(pdVar10 + -1) = *(undefined4 *)(iVar13 + 0x18);
        *(undefined4 *)((int)pdVar10 + -0xc) = *(undefined4 *)(iVar13 + 0x14);
        dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),*(undefined4 *)(iVar13 + 0x10));
        pdVar10 = pdVar10 + -2;
        *(undefined4 *)pdVar10 = *(undefined4 *)(iVar13 + 0x10);
      }
      else {
        pdVar10[-1] = dVar29;
        pdVar10[-2] = *(double *)(iVar13 + 0x18);
        pdVar10[-3] = *(double *)(iVar13 + 0x14);
        dVar29 = *(double *)(iVar13 + 0x10);
        pdVar10 = pdVar10 + -4;
        *pdVar10 = dVar29;
      }
      *(undefined4 *)((int)pdVar10 + -4) = uVar30;
      if (in_FPSCR_SZ == '\0') {
        pdVar10 = (double *)((int)pdVar10 + 0x34);
        *(undefined4 *)pdVar10 = uVar26;
      }
      else {
        pdVar10 = pdVar10 + 6;
        *pdVar10 = in_xd4;
      }
      *(int *)((int)puVar17 + -0x44) = iVar2 + 4;
      uVar18 = (undefined4)((ulonglong)dVar28 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar28 = (double)CONCAT44(uVar18,(float)(int)*(short *)(iVar2 + 6) * fVar31);
      }
      else {
        dVar28 = (double)CONCAT44(uVar18,(float)(int)*(short *)(iVar2 + 6)) * dVar27;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar10 = (double *)((int)pdVar10 + -4);
        *(int *)pdVar10 = SUB84(dVar28,0);
      }
      else {
        pdVar10 = pdVar10 + -1;
        *pdVar10 = dVar29;
      }
      uVar18 = (undefined4)((ulonglong)dVar28 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar28 = (double)CONCAT44(uVar18,(float)(int)**(short **)((int)puVar17 + -0x44) * fVar31);
      }
      else {
        dVar28 = (double)CONCAT44(uVar18,(float)(int)**(short **)((int)puVar17 + -0x44)) * dVar27;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar10 + -4) = SUB84(dVar28,0);
        *(undefined4 *)(pdVar10 + -1) = *(undefined4 *)(iVar13 + 0x38);
        *(undefined4 *)((int)pdVar10 + -0xc) = *(undefined4 *)(iVar13 + 0x34);
        dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),*(undefined4 *)(iVar13 + 0x30));
        pdVar10 = pdVar10 + -2;
        *(undefined4 *)pdVar10 = *(undefined4 *)(iVar13 + 0x30);
      }
      else {
        pdVar10[-1] = dVar29;
        pdVar10[-2] = *(double *)(iVar13 + 0x38);
        pdVar10[-3] = *(double *)(iVar13 + 0x34);
        dVar29 = *(double *)(iVar13 + 0x30);
        pdVar10 = pdVar10 + -4;
        *pdVar10 = dVar29;
      }
      *(undefined4 *)((int)pdVar10 + -4) = uVar30;
      if (in_FPSCR_SZ == '\0') {
        pdVar10 = (double *)((int)pdVar10 + 0x34);
        *(undefined4 *)pdVar10 = uVar26;
      }
      else {
        pdVar10 = pdVar10 + 6;
        *pdVar10 = in_xd4;
      }
      uVar30 = (undefined4)((ulonglong)dVar28 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar28 = (double)CONCAT44(uVar30,(float)(int)*(short *)(iVar2 + 10) * fVar31);
      }
      else {
        dVar28 = (double)CONCAT44(uVar30,(float)(int)*(short *)(iVar2 + 10)) * dVar27;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar10 = (double *)((int)pdVar10 + -4);
        *(int *)pdVar10 = SUB84(dVar28,0);
      }
      else {
        pdVar10 = pdVar10 + -1;
        *pdVar10 = dVar29;
      }
      uVar30 = DAT_8c612850._4_4_;
      if (in_FPSCR_PR == '\0') {
        fVar31 = (float)(int)*(short *)(iVar2 + 8) * fVar31;
      }
      else {
        fVar31 = SUB84((double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),
                                        (float)(int)*(short *)(iVar2 + 8)) * dVar27,0);
      }
      if (in_FPSCR_SZ == '\0') {
        *(float *)((int)pdVar10 + -4) = fVar31;
        *(undefined4 *)(pdVar10 + -1) = *(undefined4 *)(iVar13 + 0x58);
        *(undefined4 *)((int)pdVar10 + -0xc) = *(undefined4 *)(iVar13 + 0x54);
        pdVar10 = pdVar10 + -2;
        *(undefined4 *)pdVar10 = *(undefined4 *)(iVar13 + 0x50);
      }
      else {
        pdVar10[-1] = dVar29;
        pdVar10[-2] = *(double *)(iVar13 + 0x58);
        pdVar10[-3] = *(double *)(iVar13 + 0x54);
        pdVar10 = pdVar10 + -4;
        *pdVar10 = *(double *)(iVar13 + 0x50);
      }
      *(undefined4 *)((int)pdVar10 + -4) = uVar30;
      iVar6 = *piVar14 + 3;
    }
    else {
      if (in_FPSCR_SZ == '\0') {
        pdVar10 = (double *)(unaff_r8 + 0x18);
        *(undefined4 *)pdVar10 = uVar26;
      }
      else {
        pdVar10 = (double *)(unaff_r8 + 0x14);
        *pdVar10 = in_xd4;
      }
      *(int *)((int)puVar17 + -0x44) = iVar2;
      if (in_FPSCR_PR == '\0') {
        dVar28 = (double)CONCAT44(uVar18,(float)(int)*(short *)(iVar2 + 2) * fVar31);
      }
      else {
        dVar28 = (double)CONCAT44(uVar18,(float)(int)*(short *)(iVar2 + 2)) * dVar27;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar10 = (double *)((int)pdVar10 + -4);
        *(int *)pdVar10 = SUB84(dVar28,0);
      }
      else {
        pdVar10 = pdVar10 + -1;
        *pdVar10 = dVar29;
      }
      uVar18 = (undefined4)((ulonglong)dVar28 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar28 = (double)CONCAT44(uVar18,(float)(int)**(short **)((int)puVar17 + -0x44) * fVar31);
      }
      else {
        dVar28 = (double)CONCAT44(uVar18,(float)(int)**(short **)((int)puVar17 + -0x44)) * dVar27;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar10 + -4) = SUB84(dVar28,0);
        *(undefined4 *)(pdVar10 + -1) = *(undefined4 *)(iVar13 + 0x18);
        *(undefined4 *)((int)pdVar10 + -0xc) = *(undefined4 *)(iVar13 + 0x14);
        dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),*(undefined4 *)(iVar13 + 0x10));
        pdVar10 = pdVar10 + -2;
        *(undefined4 *)pdVar10 = *(undefined4 *)(iVar13 + 0x10);
      }
      else {
        pdVar10[-1] = dVar29;
        pdVar10[-2] = *(double *)(iVar13 + 0x18);
        pdVar10[-3] = *(double *)(iVar13 + 0x14);
        dVar29 = *(double *)(iVar13 + 0x10);
        pdVar10 = pdVar10 + -4;
        *pdVar10 = dVar29;
      }
      *(undefined4 *)((int)pdVar10 + -4) = uVar30;
      if (in_FPSCR_SZ == '\0') {
        pdVar10 = (double *)((int)pdVar10 + 0x34);
        *(undefined4 *)pdVar10 = uVar26;
      }
      else {
        pdVar10 = pdVar10 + 6;
        *pdVar10 = in_xd4;
      }
      *(int *)((int)puVar17 + -0x44) = iVar2 + 4;
      uVar18 = (undefined4)((ulonglong)dVar28 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar28 = (double)CONCAT44(uVar18,(float)(int)*(short *)(iVar2 + 6) * fVar31);
      }
      else {
        dVar28 = (double)CONCAT44(uVar18,(float)(int)*(short *)(iVar2 + 6)) * dVar27;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar10 = (double *)((int)pdVar10 + -4);
        *(int *)pdVar10 = SUB84(dVar28,0);
      }
      else {
        pdVar10 = pdVar10 + -1;
        *pdVar10 = dVar29;
      }
      uVar18 = (undefined4)((ulonglong)dVar28 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar28 = (double)CONCAT44(uVar18,(float)(int)**(short **)((int)puVar17 + -0x44) * fVar31);
      }
      else {
        dVar28 = (double)CONCAT44(uVar18,(float)(int)**(short **)((int)puVar17 + -0x44)) * dVar27;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar10 + -4) = SUB84(dVar28,0);
        *(undefined4 *)(pdVar10 + -1) = *(undefined4 *)(iVar13 + 0x38);
        *(undefined4 *)((int)pdVar10 + -0xc) = *(undefined4 *)(iVar13 + 0x34);
        dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),*(undefined4 *)(iVar13 + 0x30));
        pdVar10 = pdVar10 + -2;
        *(undefined4 *)pdVar10 = *(undefined4 *)(iVar13 + 0x30);
      }
      else {
        pdVar10[-1] = dVar29;
        pdVar10[-2] = *(double *)(iVar13 + 0x38);
        pdVar10[-3] = *(double *)(iVar13 + 0x34);
        dVar29 = *(double *)(iVar13 + 0x30);
        pdVar10 = pdVar10 + -4;
        *pdVar10 = dVar29;
      }
      *(undefined4 *)((int)pdVar10 + -4) = uVar30;
      if (in_FPSCR_SZ == '\0') {
        pdVar10 = (double *)((int)pdVar10 + 0x34);
        *(undefined4 *)pdVar10 = uVar26;
      }
      else {
        pdVar10 = pdVar10 + 6;
        *pdVar10 = in_xd4;
      }
      *(int *)((int)puVar17 + -0x44) = iVar2 + 0xc;
      uVar18 = (undefined4)((ulonglong)dVar28 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar28 = (double)CONCAT44(uVar18,(float)(int)*(short *)(iVar2 + 0xe) * fVar31);
      }
      else {
        dVar28 = (double)CONCAT44(uVar18,(float)(int)*(short *)(iVar2 + 0xe)) * dVar27;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar10 = (double *)((int)pdVar10 + -4);
        *(int *)pdVar10 = SUB84(dVar28,0);
      }
      else {
        pdVar10 = pdVar10 + -1;
        *pdVar10 = dVar29;
      }
      uVar18 = (undefined4)((ulonglong)dVar28 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar28 = (double)CONCAT44(uVar18,(float)(int)**(short **)((int)puVar17 + -0x44) * fVar31);
      }
      else {
        dVar28 = (double)CONCAT44(uVar18,(float)(int)**(short **)((int)puVar17 + -0x44)) * dVar27;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar10 + -4) = SUB84(dVar28,0);
        *(undefined4 *)(pdVar10 + -1) = *(undefined4 *)(iVar13 + 0x78);
        *(undefined4 *)((int)pdVar10 + -0xc) = *(undefined4 *)(iVar13 + 0x74);
        dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),*(undefined4 *)(iVar13 + 0x70));
        pdVar10 = pdVar10 + -2;
        *(undefined4 *)pdVar10 = *(undefined4 *)(iVar13 + 0x70);
      }
      else {
        pdVar10[-1] = dVar29;
        pdVar10[-2] = *(double *)(iVar13 + 0x78);
        pdVar10[-3] = *(double *)(iVar13 + 0x74);
        dVar29 = *(double *)(iVar13 + 0x70);
        pdVar10 = pdVar10 + -4;
        *pdVar10 = dVar29;
      }
      *(undefined4 *)((int)pdVar10 + -4) = uVar30;
      if (in_FPSCR_SZ == '\0') {
        pdVar10 = (double *)((int)pdVar10 + 0x34);
        *(undefined4 *)pdVar10 = uVar26;
      }
      else {
        pdVar10 = pdVar10 + 6;
        *pdVar10 = in_xd4;
      }
      uVar30 = (undefined4)((ulonglong)dVar28 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar28 = (double)CONCAT44(uVar30,(float)(int)*(short *)(iVar2 + 10) * fVar31);
      }
      else {
        dVar28 = (double)CONCAT44(uVar30,(float)(int)*(short *)(iVar2 + 10)) * dVar27;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar10 = (double *)((int)pdVar10 + -4);
        *(int *)pdVar10 = SUB84(dVar28,0);
      }
      else {
        pdVar10 = pdVar10 + -1;
        *pdVar10 = dVar29;
      }
      uVar30 = DAT_8c612a60;
      if (in_FPSCR_PR == '\0') {
        fVar31 = (float)(int)*(short *)(iVar2 + 8) * fVar31;
      }
      else {
        fVar31 = SUB84((double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),
                                        (float)(int)*(short *)(iVar2 + 8)) * dVar27,0);
      }
      if (in_FPSCR_SZ == '\0') {
        *(float *)((int)pdVar10 + -4) = fVar31;
        *(undefined4 *)(pdVar10 + -1) = *(undefined4 *)(iVar13 + 0x58);
        *(undefined4 *)((int)pdVar10 + -0xc) = *(undefined4 *)(iVar13 + 0x54);
        pdVar10 = pdVar10 + -2;
        *(undefined4 *)pdVar10 = *(undefined4 *)(iVar13 + 0x50);
      }
      else {
        pdVar10[-1] = dVar29;
        pdVar10[-2] = *(double *)(iVar13 + 0x58);
        pdVar10[-3] = *(double *)(iVar13 + 0x54);
        pdVar10 = pdVar10 + -4;
        *pdVar10 = *(double *)(iVar13 + 0x50);
      }
      *(undefined4 *)((int)pdVar10 + -4) = uVar30;
      iVar6 = *piVar14 + 4;
    }
    *piVar14 = iVar6;
  }
  return;
}

