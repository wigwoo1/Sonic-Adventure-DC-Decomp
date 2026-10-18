/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c617882) */
/* WARNING: Removing unreachable block (ram,0x8c61784c) */
/* WARNING: Removing unreachable block (ram,0x8c61776e) */
/* WARNING: Removing unreachable block (ram,0x8c617738) */
/* WARNING: Removing unreachable block (ram,0x8c617700) */
/* WARNING: Removing unreachable block (ram,0x8c61764a) */
/* WARNING: Removing unreachable block (ram,0x8c61770c) */
/* WARNING: Removing unreachable block (ram,0x8c617744) */
/* WARNING: Removing unreachable block (ram,0x8c61777e) */
/* WARNING: Removing unreachable block (ram,0x8c617858) */
/* WARNING: Removing unreachable block (ram,0x8c617892) */
/* WARNING: Removing unreachable block (ram,0x8c617668) */
/* WARNING: Removing unreachable block (ram,0x8c617810) */
/* WARNING: Removing unreachable block (ram,0x8c6177d8) */
/* WARNING: Removing unreachable block (ram,0x8c6177e4) */
/* WARNING: Removing unreachable block (ram,0x8c61781c) */

void FUN_8c6173ba(double param_1,undefined4 param_2,undefined4 param_3,double param_4,
                 undefined4 param_5,undefined4 param_6,undefined4 param_7)

{
  short sVar1;
  int *piVar2;
  double *pdVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  undefined2 *puVar9;
  float *pfVar10;
  int iVar11;
  undefined4 *puVar12;
  int unaff_r8;
  double *pdVar13;
  double *pdVar14;
  double *pdVar15;
  int *piVar16;
  int iVar17;
  undefined4 *puVar18;
  double in_dr0;
  double dVar19;
  float fVar22;
  undefined8 uVar20;
  double dVar21;
  double in_dr2;
  double dVar23;
  double dVar24;
  undefined4 uVar25;
  float fVar26;
  undefined4 uVar27;
  undefined4 uVar28;
  double dVar29;
  double dVar30;
  undefined8 in_dr12;
  double dVar31;
  double in_xd0;
  double in_xd2;
  double dVar32;
  double in_xd4;
  double in_xd6;
  double dVar33;
  bool bVar34;
  undefined2 uVar35;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  short local_5c [10];
  int aiStack_48 [10];
  undefined1 local_20 [4];
  undefined4 local_1c;
  
  piVar2 = DAT_8c6174c8._4_4_;
  iVar6 = DAT_8c6174c4;
  if (in_FPSCR_SZ == '\0') {
    puVar18 = &local_1c;
    _local_20 = CONCAT44((int)((ulonglong)in_dr12 >> 0x20),local_20);
  }
  else {
    puVar18 = (undefined4 *)local_20;
  }
  puVar12 = (undefined4 *)((int)puVar18 + -0xc);
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)DAT_8c6174c0);
    dVar29 = (double)CONCAT44(0x3f800000,param_2);
  }
  else {
    in_xd2 = *DAT_8c6174c0;
    dVar29 = (double)CONCAT44(0x3f800000,param_3);
  }
  if (in_FPSCR_PR == '\0') {
    dVar29 = (double)CONCAT44((float)((ulonglong)dVar29 >> 0x20) / SUB84(in_dr2,0),SUB84(dVar29,0));
  }
  else {
    dVar29 = dVar29 / in_dr2;
  }
  pdVar13 = (double *)((int)puVar18 + -0x30);
  *(undefined4 **)((int)puVar18 + -0x4c) = puVar12;
  *puVar12 = param_5;
  *(undefined4 *)((int)puVar18 + -8) = param_6;
  *(undefined4 *)((int)puVar18 + -4) = param_7;
  *(undefined4 *)((int)puVar18 + -0x50) = 0;
  piVar16 = *(int **)((int)puVar18 + -0x4c);
  *(double **)((int)puVar18 + -0x4c) = pdVar13;
  pdVar14 = pdVar13;
  pdVar15 = pdVar13;
  if (in_FPSCR_SZ == '\0') {
    param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),(undefined4)DAT_8c6174c8);
  }
  else {
    in_xd4 = (double)CONCAT44(DAT_8c6174c8._4_4_,(undefined4)DAT_8c6174c8);
  }
  do {
    *(int *)((int)puVar18 + -0x44) = *(int *)((int)puVar18 + -0x50) * 4;
    *(undefined4 **)((int)puVar18 + -0x48) = puVar12 + *(int *)((int)puVar18 + -0x50);
    pfVar10 = (float *)puVar12[*(int *)((int)puVar18 + -0x50)];
    *(undefined1 **)((int)puVar18 + -0x40) =
         (undefined1 *)((int)puVar18 + *(int *)((int)puVar18 + -0x44) + -0x3c);
    *(undefined1 **)((int)puVar18 + -0x54) =
         (undefined1 *)((int)puVar18 + *(int *)((int)puVar18 + -0x44) + -0x3c);
    pdVar3 = (double *)*piVar2;
    if (in_FPSCR_SZ == '\0') {
      dVar19 = (double)CONCAT44(pfVar10[1],SUB84(in_dr0,0));
      dVar23 = (double)CONCAT44(*(undefined4 *)pdVar3,*(undefined4 *)(pdVar3 + 2));
    }
    else {
      dVar19 = *(double *)(pfVar10 + 1);
      in_xd2 = pdVar3[2];
      dVar23 = *pdVar3;
    }
    if (in_FPSCR_PR == '\0') {
      dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),
                                SUB84(dVar23,0) * (float)((ulonglong)dVar19 >> 0x20));
    }
    else {
      dVar23 = dVar23 * dVar19;
    }
    if (in_FPSCR_SZ == '\0') {
      fVar22 = *pfVar10;
    }
    else {
      fVar22 = (float)((ulonglong)*(undefined8 *)pfVar10 >> 0x20);
    }
    if (in_FPSCR_PR == '\0') {
      dVar23 = (double)(ulonglong)
                       (uint)(fVar22 * (float)((ulonglong)dVar23 >> 0x20) + SUB84(dVar23,0));
    }
    if (in_FPSCR_SZ == '\0') {
      fVar22 = pfVar10[2];
      dVar19 = (double)CONCAT44(*(undefined4 *)(pdVar3 + 4),SUB84(dVar23,0));
    }
    else {
      fVar22 = (float)((ulonglong)*(undefined8 *)(pfVar10 + 2) >> 0x20);
      dVar19 = pdVar3[4];
    }
    if (in_FPSCR_PR == '\0') {
      dVar19 = (double)(ulonglong)
                       (uint)(fVar22 * (float)((ulonglong)dVar19 >> 0x20) + SUB84(dVar19,0));
    }
    uVar28 = SUB84(param_1,0);
    dVar33 = in_xd4;
    dVar23 = in_xd4;
    if (in_FPSCR_SZ == '\0') {
      dVar33 = (double)CONCAT44(uVar28,SUB84(dVar19,0));
      dVar23 = (double)((longlong)param_1 << 0x20);
    }
    if (in_FPSCR_PR == '\0') {
      uVar35 = (undefined2)
               (int)((float)((ulonglong)dVar23 >> 0x20) * SUB84(dVar33,0) +
                    (float)((ulonglong)dVar33 >> 0x20));
    }
    else {
      uVar35 = (undefined2)(int)dVar33;
    }
    **(undefined2 **)((int)puVar18 + -0x54) = uVar35;
    iVar7 = *piVar2;
    if (in_FPSCR_SZ == '\0') {
      dVar19 = (double)CONCAT44(pfVar10[1],*(undefined4 *)(iVar7 + 0x24));
      dVar23 = (double)CONCAT44(*(undefined4 *)(iVar7 + 4),*(undefined4 *)(iVar7 + 0x14));
    }
    else {
      in_xd0 = *(double *)(iVar7 + 0x24);
      dVar19 = *(double *)(pfVar10 + 1);
      in_xd2 = *(double *)(iVar7 + 0x14);
      dVar23 = *(double *)(iVar7 + 4);
    }
    if (in_FPSCR_PR == '\0') {
      dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),
                                SUB84(dVar23,0) * (float)((ulonglong)dVar19 >> 0x20));
    }
    else {
      dVar23 = dVar23 * dVar19;
    }
    if (in_FPSCR_SZ == '\0') {
      uVar20 = CONCAT44(*pfVar10,SUB84(dVar19,0));
    }
    else {
      uVar20 = *(undefined8 *)pfVar10;
    }
    if (in_FPSCR_PR == '\0') {
      dVar23 = (double)(ulonglong)
                       (uint)((float)((ulonglong)uVar20 >> 0x20) *
                              (float)((ulonglong)dVar23 >> 0x20) + SUB84(dVar23,0));
    }
    if (in_FPSCR_SZ == '\0') {
      uVar20 = CONCAT44(pfVar10[2],(int)uVar20);
      dVar19 = (double)CONCAT44(uVar28,SUB84(dVar23,0));
    }
    else {
      uVar20 = *(undefined8 *)(pfVar10 + 2);
      dVar19 = in_xd4;
    }
    if (in_FPSCR_PR == '\0') {
      dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),
                                (float)((ulonglong)uVar20 >> 0x20) * (float)uVar20 + SUB84(dVar19,0)
                               );
    }
    in_dr0 = in_xd4;
    if (in_FPSCR_SZ == '\0') {
      in_dr0 = (double)CONCAT44(uVar28,(float)uVar20);
    }
    fVar22 = (float)((ulonglong)dVar19 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      fVar22 = (float)((ulonglong)in_dr0 >> 0x20) * SUB84(dVar19,0) + fVar22;
      dVar19 = (double)CONCAT44(fVar22,SUB84(dVar19,0));
      uVar35 = (undefined2)(int)fVar22;
    }
    else {
      uVar35 = (undefined2)(int)dVar19;
    }
    *(undefined2 *)(*(int *)((int)puVar18 + -0x40) + 2) = uVar35;
    iVar7 = DAT_8c6177a4;
    dVar23 = in_xd6;
    if ((*(uint *)(**(int **)((int)puVar18 + -0x48) + 0xc) & 1) == 0) {
      iVar11 = *(int *)((int)puVar18 + -0x50);
      dVar23 = (double)CONCAT44(0x3f800000,param_3);
      if (in_FPSCR_SZ == '\0') {
        dVar29 = (double)CONCAT44((int)((ulonglong)dVar29 >> 0x20),0x3f800000);
        dVar23 = in_xd6;
      }
      iVar8 = puVar12[iVar11];
      *(int *)((int)puVar18 + -0x48) = iVar8;
      if (in_FPSCR_SZ == '\0') {
        dVar19 = (double)CONCAT44(fVar22,*(undefined4 *)(iVar8 + 0x18));
      }
      else {
        in_xd2 = *(double *)(iVar8 + 0x18);
      }
      if (in_FPSCR_PR == '\0') {
        dVar29 = (double)CONCAT44((int)((ulonglong)dVar29 >> 0x20),SUB84(dVar29,0) / SUB84(dVar19,0)
                                 );
      }
      else {
        dVar29 = dVar29 / dVar19;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar19 = (double)CONCAT44(*(undefined4 *)(iVar8 + 0x10),*(undefined4 *)(iVar6 + 0xc));
      }
      else {
        in_xd2 = *(double *)(iVar6 + 0xc);
        dVar19 = *(double *)(iVar8 + 0x10);
      }
      if (in_FPSCR_PR == '\0') {
        dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) - SUB84(dVar19,0),
                                  SUB84(dVar19,0));
      }
      else {
        dVar19 = dVar19 - dVar19;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),*(undefined4 *)DAT_8c6174d0);
      }
      else {
        in_xd2 = *DAT_8c6174d0;
      }
      fVar22 = SUB84(dVar29,0);
      if (in_FPSCR_PR == '\0') {
        dVar19 = (double)CONCAT44(((float)((ulonglong)dVar19 >> 0x20) * fVar22) / SUB84(dVar19,0),
                                  SUB84(dVar19,0));
      }
      else {
        dVar19 = (dVar19 * dVar29) / (dVar19 * dVar29);
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)pdVar15 = (int)((ulonglong)dVar19 >> 0x20);
        dVar19 = (double)(ulonglong)*(uint *)(iVar6 + 0x10);
      }
      else {
        *pdVar15 = dVar19;
        in_xd2 = *(double *)(iVar6 + 0x10);
      }
      iVar11 = puVar12[iVar11];
      if (in_FPSCR_SZ == '\0') {
        dVar19 = (double)CONCAT44(*(undefined4 *)(iVar11 + 0x14),SUB84(dVar19,0));
      }
      else {
        dVar19 = *(double *)(iVar11 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) - SUB84(dVar19,0),
                                  SUB84(dVar19,0));
      }
      else {
        dVar19 = dVar19 - dVar19;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),*(undefined4 *)DAT_8c6174d4);
      }
      else {
        in_xd2 = *DAT_8c6174d4;
      }
      if (in_FPSCR_PR == '\0') {
        dVar19 = (double)CONCAT44(((float)((ulonglong)dVar19 >> 0x20) * fVar22) / SUB84(dVar19,0),
                                  SUB84(dVar19,0));
      }
      else {
        dVar19 = (dVar19 * dVar29) / (dVar19 * dVar29);
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar15 + 4) = (int)((ulonglong)dVar19 >> 0x20);
        *(float *)(pdVar15 + 1) = fVar22;
      }
      else {
        *(double *)((int)pdVar15 + 4) = dVar19;
        pdVar15[1] = dVar23;
      }
    }
    else {
      if (in_FPSCR_SZ == '\0') {
        uVar28 = *(undefined4 *)(*piVar16 + 0x10);
        dVar19 = (double)CONCAT44(fVar22,uVar28);
        *(undefined4 *)pdVar14 = uVar28;
      }
      else {
        in_xd2 = *(double *)(*piVar16 + 0x10);
        *pdVar14 = in_xd2;
      }
      if (in_FPSCR_SZ == '\0') {
        uVar28 = *(undefined4 *)(*piVar16 + 0x14);
        dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),uVar28);
        *(undefined4 *)((int)pdVar14 + 4) = uVar28;
      }
      else {
        in_xd2 = *(double *)(*piVar16 + 0x14);
        *(double *)((int)pdVar14 + 4) = in_xd2;
      }
      if (in_FPSCR_SZ == '\0') {
        uVar28 = *(undefined4 *)(*piVar16 + 0x18);
        dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),uVar28);
        *(undefined4 *)(pdVar14 + 1) = uVar28;
      }
      else {
        in_xd2 = *(double *)(*piVar16 + 0x18);
        pdVar14[1] = in_xd2;
      }
    }
    iVar11 = *(int *)((int)puVar18 + -0x50);
    pdVar14 = (double *)((int)pdVar14 + 0xc);
    *(int *)((int)puVar18 + -0x50) = iVar11 + 1;
    piVar16 = piVar16 + 1;
    pdVar15 = (double *)((int)pdVar15 + 0xc);
    in_xd6 = dVar23;
  } while (iVar11 + 1 < 3);
  pdVar14 = *(double **)((int)puVar18 + -0x4c);
  if (in_FPSCR_SZ == '\0') {
    uVar28 = *(undefined4 *)(pdVar14 + 1);
    dVar29 = (double)CONCAT44((int)((ulonglong)dVar29 >> 0x20),uVar28);
    param_4 = (double)CONCAT44((int)((ulonglong)param_4 >> 0x20),
                               *(undefined4 *)((int)puVar18 + -0x20));
    param_3 = *(undefined4 *)((int)pdVar14 + 4);
    param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),
                               *(undefined4 *)((int)puVar18 + -0x1c));
    dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),param_3);
    dVar33 = (double)CONCAT44(uVar28,SUB84(in_dr0,0));
  }
  else {
    dVar33 = pdVar14[1];
    in_xd2 = *(double *)((int)pdVar14 + 4);
    in_xd4 = *(double *)((int)puVar18 + -0x1c);
    uVar28 = SUB84(dVar29,0);
    dVar23 = dVar33;
  }
  if (in_FPSCR_PR == '\0') {
    fVar22 = SUB84(dVar19,0) * SUB84(param_1,0);
    dVar33 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) * SUB84(param_4,0),SUB84(dVar33,0))
    ;
  }
  else {
    fVar22 = SUB84(dVar19 * param_1,0);
    dVar33 = dVar33 * param_4;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar30 = (double)CONCAT44(*(undefined4 *)pdVar14,SUB84(param_4,0));
    dVar19 = (double)CONCAT44(*(undefined4 *)((int)puVar18 + -0x24),param_3);
    dVar24 = (double)CONCAT44(uVar28,fVar22);
  }
  else {
    dVar30 = *pdVar14;
    dVar19 = *(double *)((int)puVar18 + -0x24);
    dVar24 = dVar23;
  }
  fVar22 = (float)((ulonglong)dVar19 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * fVar22,SUB84(dVar24,0));
  }
  else {
    dVar24 = dVar24 * dVar19;
  }
  dVar21 = dVar19;
  if (in_FPSCR_SZ == '\0') {
    dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),fVar22);
    dVar21 = in_xd0;
  }
  if (in_FPSCR_PR == '\0') {
    dVar33 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) - SUB84(dVar24,0),SUB84(dVar33,0));
  }
  else {
    dVar33 = dVar33 - dVar24;
  }
  uVar28 = (undefined4)((ulonglong)dVar30 >> 0x20);
  dVar32 = dVar30;
  if (in_FPSCR_SZ == '\0') {
    dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),uVar28);
    dVar32 = in_xd2;
  }
  if (in_FPSCR_PR == '\0') {
    dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),SUB84(dVar24,0) * SUB84(param_1,0));
  }
  else {
    dVar24 = dVar24 * param_1;
  }
  dVar31 = dVar30;
  if (in_FPSCR_SZ == '\0') {
    dVar31 = (double)CONCAT44(uVar28,(int)in_dr12);
  }
  if (in_FPSCR_PR == '\0') {
    dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),SUB84(dVar33,0) * SUB84(dVar19,0));
    fVar22 = SUB84(dVar24,0) - (float)((ulonglong)dVar24 >> 0x20);
  }
  else {
    dVar33 = dVar33 * dVar19;
    fVar22 = SUB84(dVar24 - dVar24,0);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar19 = (double)CONCAT44(*(undefined4 *)((int)puVar18 + -0x14),fVar22);
  }
  else {
    dVar19 = *(double *)((int)puVar18 + -0x14);
  }
  if (in_FPSCR_PR == '\0') {
    dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) * SUB84(dVar19,0),SUB84(dVar19,0));
  }
  else {
    dVar19 = dVar19 * dVar19;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),*(undefined4 *)((int)puVar18 + -0x18)
                             );
  }
  else {
    dVar32 = *(double *)((int)puVar18 + -0x18);
  }
  if (in_FPSCR_PR == '\0') {
    dVar19 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) * SUB84(dVar19,0) +
                              (float)((ulonglong)dVar19 >> 0x20),SUB84(dVar19,0));
    dVar31 = (double)CONCAT44((float)((ulonglong)dVar31 >> 0x20) * SUB84(dVar30,0),SUB84(dVar31,0));
  }
  else {
    dVar31 = dVar31 * dVar30;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),*(undefined4 *)((int)puVar18 + -0x10)
                             );
  }
  else {
    dVar32 = *(double *)((int)puVar18 + -0x10);
  }
  if (in_FPSCR_PR == '\0') {
    fVar22 = SUB84(dVar33,0) - (float)((ulonglong)dVar31 >> 0x20);
  }
  else {
    fVar22 = SUB84(dVar33 - dVar31,0);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar21 = (double)CONCAT44(fVar22,fVar22);
  }
  if (in_FPSCR_PR == '\0') {
    dVar19 = (double)((ulonglong)
                      (uint)((float)((ulonglong)dVar21 >> 0x20) * SUB84(dVar19,0) +
                            (float)((ulonglong)dVar19 >> 0x20)) << 0x20);
  }
  dVar33 = (double)((ulonglong)dVar19 & 0xffffffff00000000);
  dVar30 = dVar33;
  if (in_FPSCR_SZ == '\0') {
    param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),(int)((ulonglong)dVar19 >> 0x20));
    dVar30 = in_xd4;
  }
  if (in_FPSCR_PR == '\0') {
    bVar34 = SUB84(param_1,0) < 0.0;
  }
  else {
    bVar34 = param_1 < dVar33;
  }
  if (((uint)bVar34 & *DAT_8c6177a0) == 0) {
    *DAT_8c6177a0 = 0;
    iVar11 = DAT_8c6177a8;
    iVar17 = 0;
    *(undefined4 *)((int)puVar18 + -0x4c) = 2;
    iVar8 = 0;
    do {
      *(int *)((int)puVar18 + -0x50) = *(int *)((int)puVar18 + -0x4c) * 4;
      if ((*(uint *)(puVar12[*(int *)((int)puVar18 + -0x4c)] + 0xc) & 1 ^
          *(uint *)(puVar12[iVar8] + 0xc) & 1) == 1) {
        iVar4 = *(int *)((int)puVar18 + -0x4c) * 0xc + (int)pdVar13;
        *(int *)((int)puVar18 + -0x48) = iVar4;
        if (in_FPSCR_SZ == '\0') {
          param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),*(undefined4 *)(iVar4 + 8));
          dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),*(undefined4 *)DAT_8c6177ac);
          dVar19 = dVar30;
        }
        else {
          dVar19 = *(double *)(iVar4 + 8);
          dVar32 = *DAT_8c6177ac;
        }
        if (in_FPSCR_PR == '\0') {
          fVar22 = SUB84(dVar33,0) - SUB84(param_1,0);
        }
        else {
          fVar22 = SUB84(dVar33 - param_1,0);
        }
        iVar4 = iVar8 * 0xc + (int)pdVar13;
        *(int *)((int)puVar18 + -0x44) = iVar4;
        if (in_FPSCR_SZ == '\0') {
          dVar33 = (double)CONCAT44(*(undefined4 *)(iVar4 + 8),fVar22);
        }
        else {
          dVar33 = *(double *)(iVar4 + 8);
        }
        if (in_FPSCR_PR == '\0') {
          dVar33 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) - SUB84(param_1,0),
                                    SUB84(dVar33,0));
        }
        else {
          dVar33 = dVar33 - param_1;
        }
        dVar30 = dVar32;
        if (in_FPSCR_SZ == '\0') {
          param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),SUB84(dVar33,0));
          dVar30 = dVar19;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),
                                    *(undefined4 *)*(double **)((int)puVar18 + -0x44));
          dVar19 = (double)CONCAT44((int)((ulonglong)dVar29 >> 0x20),
                                    *(undefined4 *)*(double **)((int)puVar18 + -0x48));
        }
        else {
          dVar32 = **(double **)((int)puVar18 + -0x44);
          dVar23 = **(double **)((int)puVar18 + -0x48);
          dVar19 = dVar29;
        }
        if (in_FPSCR_PR == '\0') {
          param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),
                                     SUB84(param_1,0) / (float)((ulonglong)dVar33 >> 0x20));
          fVar22 = SUB84(dVar33,0) - SUB84(dVar19,0);
        }
        else {
          param_1 = param_1 / dVar33;
          fVar22 = SUB84(dVar33 - dVar19,0);
        }
        uVar28 = SUB84(param_1,0);
        dVar24 = dVar30;
        dVar33 = dVar23;
        if (in_FPSCR_SZ == '\0') {
          dVar24 = (double)CONCAT44(uVar28,SUB84(dVar21,0));
          dVar33 = (double)CONCAT44(SUB84(dVar19,0),fVar22);
        }
        if (in_FPSCR_PR == '\0') {
          dVar33 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar33,0) +
                                    (float)((ulonglong)dVar33 >> 0x20),SUB84(dVar33,0));
        }
        uVar25 = (undefined4)((ulonglong)dVar19 >> 0x20);
        dVar29 = dVar19;
        if (in_FPSCR_SZ == '\0') {
          dVar19 = (double)CONCAT44(uVar25,SUB84(dVar24,0));
          dVar29 = (double)CONCAT44(uVar25,(int)((ulonglong)dVar33 >> 0x20));
          dVar33 = dVar23;
        }
        iVar4 = iVar17 * 0x20 + iVar11;
        *(int *)((int)puVar18 + -0x4c) = iVar4;
        if (in_FPSCR_SZ == '\0') {
          dVar23 = (double)CONCAT44(*(undefined4 *)(iVar6 + 0xc),*(undefined4 *)DAT_8c6177b0);
        }
        else {
          dVar32 = *DAT_8c6177b0;
          dVar23 = *(double *)(iVar6 + 0xc);
        }
        if (in_FPSCR_PR == '\0') {
          fVar22 = SUB84(dVar23,0) * SUB84(dVar29,0);
          dVar23 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) * fVar22 +
                                    (float)((ulonglong)dVar23 >> 0x20),fVar22);
        }
        else {
          dVar23 = dVar23 * dVar29;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(iVar4 + 0x10) = (int)((ulonglong)dVar23 >> 0x20);
          dVar19 = (double)CONCAT44(uVar28,SUB84(dVar19,0));
        }
        else {
          *(double *)(iVar4 + 0x10) = dVar23;
          dVar19 = dVar30;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar29 = (double)CONCAT44(uVar25,*(undefined4 *)(*(int *)((int)puVar18 + -0x48) + 4));
        }
        else {
          dVar33 = *(double *)(*(int *)((int)puVar18 + -0x48) + 4);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar23 = (double)CONCAT44(SUB84(dVar29,0),
                                    *(undefined4 *)(*(int *)((int)puVar18 + -0x44) + 4));
        }
        else {
          dVar32 = *(double *)(*(int *)((int)puVar18 + -0x44) + 4);
          dVar23 = dVar33;
        }
        if (in_FPSCR_PR == '\0') {
          dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),
                                    SUB84(dVar23,0) - SUB84(dVar29,0));
        }
        else {
          dVar23 = dVar23 - dVar29;
        }
        if (in_FPSCR_PR == '\0') {
          dVar23 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) * SUB84(dVar23,0) +
                                    (float)((ulonglong)dVar23 >> 0x20),SUB84(dVar23,0));
        }
        if (in_FPSCR_SZ == '\0') {
          uVar25 = (undefined4)((ulonglong)dVar23 >> 0x20);
          dVar29 = (double)CONCAT44((int)((ulonglong)dVar29 >> 0x20),uVar25);
          dVar23 = dVar33;
          dVar33 = (double)CONCAT44(uVar25,*(undefined4 *)DAT_8c6177b4);
        }
        else {
          dVar32 = *DAT_8c6177b4;
          dVar33 = dVar23;
        }
        if (in_FPSCR_PR == '\0') {
          fVar22 = SUB84(dVar33,0) * SUB84(dVar29,0);
        }
        else {
          fVar22 = SUB84(dVar33 * dVar29,0);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar20 = CONCAT44(*(undefined4 *)(iVar6 + 0x10),fVar22);
          dVar19 = (double)CONCAT44((int)((ulonglong)dVar29 >> 0x20),SUB84(dVar19,0));
        }
        else {
          uVar20 = *(undefined8 *)(iVar6 + 0x10);
          dVar19 = dVar29;
        }
        fVar22 = (float)((ulonglong)dVar19 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          uVar20 = CONCAT44(fVar22 * (float)uVar20 + (float)((ulonglong)uVar20 >> 0x20),
                            (float)uVar20);
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(*(int *)((int)puVar18 + -0x4c) + 0x14) = (int)((ulonglong)uVar20 >> 0x20);
        }
        else {
          *(undefined8 *)(*(int *)((int)puVar18 + -0x4c) + 0x14) = uVar20;
        }
        if (in_FPSCR_SZ == '\0') {
          *(float *)(*(int *)((int)puVar18 + -0x4c) + 0x18) = fVar22;
          dVar21 = (double)CONCAT44(uVar28,SUB84(dVar19,0));
        }
        else {
          *(double *)(*(int *)((int)puVar18 + -0x4c) + 0x18) = dVar19;
          dVar21 = dVar30;
        }
        puVar9 = (undefined2 *)(iVar17 * 4 + iVar7);
        *(undefined2 **)((int)puVar18 + -0x40) = puVar9;
        *(short **)((int)puVar18 + -0x48) =
             (short *)((int)puVar18 + *(int *)((int)puVar18 + -0x50) + -0x3c);
        sVar1 = *(short *)((int)puVar18 + *(int *)((int)puVar18 + -0x50) + -0x3c);
        *(short *)((int)puVar18 + -0x44) = sVar1;
        *(short **)((int)puVar18 + -0x4c) = (short *)((int)puVar18 + iVar8 * 4 + -0x3c);
        iVar4 = (int)*(short *)((int)puVar18 + iVar8 * 4 + -0x3c) -
                (int)*(short *)((int)puVar18 + -0x44);
        fVar22 = (float)((ulonglong)dVar21 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          uVar35 = (undefined2)(int)(fVar22 * (float)iVar4 + (float)(int)sVar1);
        }
        else {
          uVar35 = (undefined2)(int)(double)iVar4;
        }
        *puVar9 = uVar35;
        iVar17 = iVar17 + 1;
        sVar1 = *(short *)(*(int *)((int)puVar18 + -0x48) + 2);
        *(short *)((int)puVar18 + -0x50) = sVar1;
        if (in_FPSCR_PR == '\0') {
          fVar26 = (float)((int)*(short *)(*(int *)((int)puVar18 + -0x4c) + 2) -
                          (int)*(short *)((int)puVar18 + -0x50));
          fVar22 = fVar22 * fVar26 + (float)(int)sVar1;
          dVar33 = (double)CONCAT44(fVar26,fVar22);
          uVar35 = (undefined2)(int)fVar22;
        }
        else {
          dVar33 = (double)((int)*(short *)(*(int *)((int)puVar18 + -0x4c) + 2) -
                           (int)*(short *)((int)puVar18 + -0x50));
          uVar35 = (undefined2)(int)dVar33;
        }
        *(undefined2 *)(*(int *)((int)puVar18 + -0x40) + 2) = uVar35;
      }
      if ((*(uint *)(puVar12[iVar8] + 0xc) & 1) == 0) {
        iVar5 = iVar17 * 0x20 + iVar11;
        *(int *)((int)puVar18 + -0x50) = iVar5;
        *(undefined4 **)((int)puVar18 + -0x4c) = puVar12 + iVar8;
        iVar4 = puVar12[iVar8];
        if (in_FPSCR_SZ == '\0') {
          uVar28 = *(undefined4 *)(iVar4 + 0x10);
          dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),uVar28);
          *(undefined4 *)(iVar5 + 0x10) = uVar28;
        }
        else {
          dVar32 = *(double *)(iVar4 + 0x10);
          *(double *)(iVar5 + 0x10) = dVar32;
        }
        if (in_FPSCR_SZ == '\0') {
          uVar28 = *(undefined4 *)(**(int **)((int)puVar18 + -0x4c) + 0x14);
          dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),uVar28);
          *(undefined4 *)(*(int *)((int)puVar18 + -0x50) + 0x14) = uVar28;
        }
        else {
          dVar32 = *(double *)(**(int **)((int)puVar18 + -0x4c) + 0x14);
          *(double *)(*(int *)((int)puVar18 + -0x50) + 0x14) = dVar32;
        }
        if (in_FPSCR_SZ == '\0') {
          uVar28 = *(undefined4 *)(**(int **)((int)puVar18 + -0x4c) + 0x18);
          dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),uVar28);
          *(undefined4 *)(*(int *)((int)puVar18 + -0x50) + 0x18) = uVar28;
        }
        else {
          dVar32 = *(double *)(**(int **)((int)puVar18 + -0x4c) + 0x18);
          *(double *)(*(int *)((int)puVar18 + -0x50) + 0x18) = dVar32;
        }
        puVar9 = (undefined2 *)(iVar17 * 4 + iVar7);
        *(undefined2 **)((int)puVar18 + -0x50) = puVar9;
        *(undefined2 **)((int)puVar18 + -0x4c) = (undefined2 *)((int)puVar18 + iVar8 * 4 + -0x3c);
        iVar17 = iVar17 + 1;
        *puVar9 = *(undefined2 *)((int)puVar18 + iVar8 * 4 + -0x3c);
        *(undefined2 *)(*(int *)((int)puVar18 + -0x50) + 2) =
             *(undefined2 *)(*(int *)((int)puVar18 + -0x4c) + 2);
      }
      uVar28 = DAT_8c6177bc;
      piVar2 = DAT_8c6177b8;
      *(int *)((int)puVar18 + -0x4c) = iVar8;
      iVar8 = iVar8 + 1;
    } while (iVar8 < 3);
    if (in_FPSCR_SZ == '\0') {
      param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),(undefined4)DAT_8c6177c0);
    }
    uVar25 = (undefined4)((ulonglong)dVar33 >> 0x20);
    uVar27 = (undefined4)((ulonglong)param_1 >> 0x20);
    fVar22 = SUB84(param_1,0);
    if (iVar17 == 3) {
      *(int *)((int)puVar18 + -0x50) = iVar7;
      if (in_FPSCR_PR == '\0') {
        dVar29 = (double)CONCAT44(uVar25,(float)(int)*(short *)(iVar7 + 2) * fVar22);
      }
      else {
        dVar29 = (double)CONCAT44(uVar25,(float)(int)*(short *)(iVar7 + 2)) * param_1;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar14 = (double *)(unaff_r8 + 0x14);
        *(int *)pdVar14 = SUB84(dVar29,0);
      }
      else {
        pdVar14 = (double *)(unaff_r8 + 0x10);
        *pdVar14 = dVar32;
      }
      uVar25 = (undefined4)((ulonglong)dVar29 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar29 = (double)CONCAT44(uVar25,(float)(int)**(short **)((int)puVar18 + -0x50) * fVar22);
      }
      else {
        dVar29 = (double)CONCAT44(uVar25,(float)(int)**(short **)((int)puVar18 + -0x50)) * param_1;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar14 + -4) = SUB84(dVar29,0);
        *(undefined4 *)(pdVar14 + -1) = *(undefined4 *)(iVar11 + 0x18);
        *(undefined4 *)((int)pdVar14 + -0xc) = *(undefined4 *)(iVar11 + 0x14);
        dVar29 = (double)CONCAT44((int)((ulonglong)dVar29 >> 0x20),*(undefined4 *)(iVar11 + 0x10));
        pdVar14 = pdVar14 + -2;
        *(undefined4 *)pdVar14 = *(undefined4 *)(iVar11 + 0x10);
      }
      else {
        pdVar14[-1] = dVar32;
        pdVar14[-2] = *(double *)(iVar11 + 0x18);
        pdVar14[-3] = *(double *)(iVar11 + 0x14);
        dVar32 = *(double *)(iVar11 + 0x10);
        pdVar14 = pdVar14 + -4;
        *pdVar14 = dVar32;
      }
      *(undefined4 *)((int)pdVar14 + -4) = uVar28;
      *(int *)((int)puVar18 + -0x50) = iVar7 + 4;
      uVar25 = (undefined4)((ulonglong)dVar29 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar29 = (double)CONCAT44(uVar25,(float)(int)*(short *)(iVar7 + 6) * fVar22);
      }
      else {
        dVar29 = (double)CONCAT44(uVar25,(float)(int)*(short *)(iVar7 + 6)) * param_1;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar14 = pdVar14 + 6;
        *(int *)pdVar14 = SUB84(dVar29,0);
      }
      else {
        pdVar14 = (double *)((int)pdVar14 + 0x2c);
        *pdVar14 = dVar32;
      }
      uVar25 = (undefined4)((ulonglong)dVar29 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar29 = (double)CONCAT44(uVar25,(float)(int)**(short **)((int)puVar18 + -0x50) * fVar22);
      }
      else {
        dVar29 = (double)CONCAT44(uVar25,(float)(int)**(short **)((int)puVar18 + -0x50)) * param_1;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar14 + -4) = SUB84(dVar29,0);
        *(undefined4 *)(pdVar14 + -1) = *(undefined4 *)(iVar11 + 0x38);
        *(undefined4 *)((int)pdVar14 + -0xc) = *(undefined4 *)(iVar11 + 0x34);
        dVar29 = (double)CONCAT44((int)((ulonglong)dVar29 >> 0x20),*(undefined4 *)(iVar11 + 0x30));
        pdVar14 = pdVar14 + -2;
        *(undefined4 *)pdVar14 = *(undefined4 *)(iVar11 + 0x30);
      }
      else {
        pdVar14[-1] = dVar32;
        pdVar14[-2] = *(double *)(iVar11 + 0x38);
        pdVar14[-3] = *(double *)(iVar11 + 0x34);
        dVar32 = *(double *)(iVar11 + 0x30);
        pdVar14 = pdVar14 + -4;
        *pdVar14 = dVar32;
      }
      *(undefined4 *)((int)pdVar14 + -4) = uVar28;
      if (in_FPSCR_SZ == '\0') {
        pdVar14 = (double *)((int)pdVar14 + 0x34);
        *(undefined4 *)pdVar14 = uVar27;
      }
      else {
        pdVar14 = pdVar14 + 6;
        *pdVar14 = param_1;
      }
      uVar28 = (undefined4)((ulonglong)dVar29 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar29 = (double)CONCAT44(uVar28,(float)(int)*(short *)(iVar7 + 10) * fVar22);
      }
      else {
        dVar29 = (double)CONCAT44(uVar28,(float)(int)*(short *)(iVar7 + 10)) * param_1;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar14 = (double *)((int)pdVar14 + -4);
        *(int *)pdVar14 = SUB84(dVar29,0);
      }
      else {
        pdVar14 = pdVar14 + -1;
        *pdVar14 = dVar32;
      }
      uVar28 = DAT_8c6177c0._4_4_;
      if (in_FPSCR_PR == '\0') {
        fVar22 = (float)(int)*(short *)(iVar7 + 8) * fVar22;
      }
      else {
        fVar22 = SUB84((double)CONCAT44((int)((ulonglong)dVar29 >> 0x20),
                                        (float)(int)*(short *)(iVar7 + 8)) * param_1,0);
      }
      if (in_FPSCR_SZ == '\0') {
        *(float *)((int)pdVar14 + -4) = fVar22;
        *(undefined4 *)(pdVar14 + -1) = *(undefined4 *)(iVar11 + 0x58);
        *(undefined4 *)((int)pdVar14 + -0xc) = *(undefined4 *)(iVar11 + 0x54);
        pdVar14 = pdVar14 + -2;
        *(undefined4 *)pdVar14 = *(undefined4 *)(iVar11 + 0x50);
      }
      else {
        pdVar14[-1] = dVar32;
        pdVar14[-2] = *(double *)(iVar11 + 0x58);
        pdVar14[-3] = *(double *)(iVar11 + 0x54);
        pdVar14 = pdVar14 + -4;
        *pdVar14 = *(double *)(iVar11 + 0x50);
      }
      *(undefined4 *)((int)pdVar14 + -4) = uVar28;
      iVar6 = *piVar2 + 3;
    }
    else {
      *(int *)((int)puVar18 + -0x50) = iVar7;
      if (in_FPSCR_PR == '\0') {
        dVar29 = (double)CONCAT44(uVar25,(float)(int)*(short *)(iVar7 + 2) * fVar22);
      }
      else {
        dVar29 = (double)CONCAT44(uVar25,(float)(int)*(short *)(iVar7 + 2)) * param_1;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar14 = (double *)(unaff_r8 + 0x14);
        *(int *)pdVar14 = SUB84(dVar29,0);
      }
      else {
        pdVar14 = (double *)(unaff_r8 + 0x10);
        *pdVar14 = dVar32;
      }
      uVar25 = (undefined4)((ulonglong)dVar29 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar29 = (double)CONCAT44(uVar25,(float)(int)**(short **)((int)puVar18 + -0x50) * fVar22);
      }
      else {
        dVar29 = (double)CONCAT44(uVar25,(float)(int)**(short **)((int)puVar18 + -0x50)) * param_1;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar14 + -4) = SUB84(dVar29,0);
        *(undefined4 *)(pdVar14 + -1) = *(undefined4 *)(iVar11 + 0x18);
        *(undefined4 *)((int)pdVar14 + -0xc) = *(undefined4 *)(iVar11 + 0x14);
        dVar29 = (double)CONCAT44((int)((ulonglong)dVar29 >> 0x20),*(undefined4 *)(iVar11 + 0x10));
        pdVar14 = pdVar14 + -2;
        *(undefined4 *)pdVar14 = *(undefined4 *)(iVar11 + 0x10);
      }
      else {
        pdVar14[-1] = dVar32;
        pdVar14[-2] = *(double *)(iVar11 + 0x18);
        pdVar14[-3] = *(double *)(iVar11 + 0x14);
        dVar32 = *(double *)(iVar11 + 0x10);
        pdVar14 = pdVar14 + -4;
        *pdVar14 = dVar32;
      }
      *(undefined4 *)((int)pdVar14 + -4) = uVar28;
      *(int *)((int)puVar18 + -0x50) = iVar7 + 4;
      uVar25 = (undefined4)((ulonglong)dVar29 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar29 = (double)CONCAT44(uVar25,(float)(int)*(short *)(iVar7 + 6) * fVar22);
      }
      else {
        dVar29 = (double)CONCAT44(uVar25,(float)(int)*(short *)(iVar7 + 6)) * param_1;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar14 = pdVar14 + 6;
        *(int *)pdVar14 = SUB84(dVar29,0);
      }
      else {
        pdVar14 = (double *)((int)pdVar14 + 0x2c);
        *pdVar14 = dVar32;
      }
      uVar25 = (undefined4)((ulonglong)dVar29 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar29 = (double)CONCAT44(uVar25,(float)(int)**(short **)((int)puVar18 + -0x50) * fVar22);
      }
      else {
        dVar29 = (double)CONCAT44(uVar25,(float)(int)**(short **)((int)puVar18 + -0x50)) * param_1;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar14 + -4) = SUB84(dVar29,0);
        *(undefined4 *)(pdVar14 + -1) = *(undefined4 *)(iVar11 + 0x38);
        *(undefined4 *)((int)pdVar14 + -0xc) = *(undefined4 *)(iVar11 + 0x34);
        dVar29 = (double)CONCAT44((int)((ulonglong)dVar29 >> 0x20),*(undefined4 *)(iVar11 + 0x30));
        pdVar14 = pdVar14 + -2;
        *(undefined4 *)pdVar14 = *(undefined4 *)(iVar11 + 0x30);
      }
      else {
        pdVar14[-1] = dVar32;
        pdVar14[-2] = *(double *)(iVar11 + 0x38);
        pdVar14[-3] = *(double *)(iVar11 + 0x34);
        dVar32 = *(double *)(iVar11 + 0x30);
        pdVar14 = pdVar14 + -4;
        *pdVar14 = dVar32;
      }
      *(undefined4 *)((int)pdVar14 + -4) = uVar28;
      if (in_FPSCR_SZ == '\0') {
        pdVar14 = (double *)((int)pdVar14 + 0x34);
        *(undefined4 *)pdVar14 = uVar27;
      }
      else {
        pdVar14 = pdVar14 + 6;
        *pdVar14 = param_1;
      }
      *(int *)((int)puVar18 + -0x50) = iVar7 + 0xc;
      uVar25 = (undefined4)((ulonglong)dVar29 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar29 = (double)CONCAT44(uVar25,(float)(int)*(short *)(iVar7 + 0xe) * fVar22);
      }
      else {
        dVar29 = (double)CONCAT44(uVar25,(float)(int)*(short *)(iVar7 + 0xe)) * param_1;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar14 = (double *)((int)pdVar14 + -4);
        *(int *)pdVar14 = SUB84(dVar29,0);
      }
      else {
        pdVar14 = pdVar14 + -1;
        *pdVar14 = dVar32;
      }
      uVar25 = (undefined4)((ulonglong)dVar29 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar29 = (double)CONCAT44(uVar25,(float)(int)**(short **)((int)puVar18 + -0x50) * fVar22);
      }
      else {
        dVar29 = (double)CONCAT44(uVar25,(float)(int)**(short **)((int)puVar18 + -0x50)) * param_1;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar14 + -4) = SUB84(dVar29,0);
        *(undefined4 *)(pdVar14 + -1) = *(undefined4 *)(iVar11 + 0x78);
        *(undefined4 *)((int)pdVar14 + -0xc) = *(undefined4 *)(iVar11 + 0x74);
        dVar29 = (double)CONCAT44((int)((ulonglong)dVar29 >> 0x20),*(undefined4 *)(iVar11 + 0x70));
        pdVar14 = pdVar14 + -2;
        *(undefined4 *)pdVar14 = *(undefined4 *)(iVar11 + 0x70);
      }
      else {
        pdVar14[-1] = dVar32;
        pdVar14[-2] = *(double *)(iVar11 + 0x78);
        pdVar14[-3] = *(double *)(iVar11 + 0x74);
        dVar32 = *(double *)(iVar11 + 0x70);
        pdVar14 = pdVar14 + -4;
        *pdVar14 = dVar32;
      }
      *(undefined4 *)((int)pdVar14 + -4) = uVar28;
      if (in_FPSCR_SZ == '\0') {
        pdVar14 = (double *)((int)pdVar14 + 0x34);
        *(undefined4 *)pdVar14 = uVar27;
      }
      else {
        pdVar14 = pdVar14 + 6;
        *pdVar14 = param_1;
      }
      uVar28 = (undefined4)((ulonglong)dVar29 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar29 = (double)CONCAT44(uVar28,(float)(int)*(short *)(iVar7 + 10) * fVar22);
      }
      else {
        dVar29 = (double)CONCAT44(uVar28,(float)(int)*(short *)(iVar7 + 10)) * param_1;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar14 = (double *)((int)pdVar14 + -4);
        *(int *)pdVar14 = SUB84(dVar29,0);
      }
      else {
        pdVar14 = pdVar14 + -1;
        *pdVar14 = dVar32;
      }
      uVar28 = DAT_8c617954;
      if (in_FPSCR_PR == '\0') {
        fVar22 = (float)(int)*(short *)(iVar7 + 8) * fVar22;
      }
      else {
        fVar22 = SUB84((double)CONCAT44((int)((ulonglong)dVar29 >> 0x20),
                                        (float)(int)*(short *)(iVar7 + 8)) * param_1,0);
      }
      if (in_FPSCR_SZ == '\0') {
        *(float *)((int)pdVar14 + -4) = fVar22;
        *(undefined4 *)(pdVar14 + -1) = *(undefined4 *)(iVar11 + 0x58);
        *(undefined4 *)((int)pdVar14 + -0xc) = *(undefined4 *)(iVar11 + 0x54);
        pdVar14 = pdVar14 + -2;
        *(undefined4 *)pdVar14 = *(undefined4 *)(iVar11 + 0x50);
      }
      else {
        pdVar14[-1] = dVar32;
        pdVar14[-2] = *(double *)(iVar11 + 0x58);
        pdVar14[-3] = *(double *)(iVar11 + 0x54);
        pdVar14 = pdVar14 + -4;
        *pdVar14 = *(double *)(iVar11 + 0x50);
      }
      *(undefined4 *)((int)pdVar14 + -4) = uVar28;
      iVar6 = *piVar2 + 4;
    }
    *piVar2 = iVar6;
  }
  return;
}

