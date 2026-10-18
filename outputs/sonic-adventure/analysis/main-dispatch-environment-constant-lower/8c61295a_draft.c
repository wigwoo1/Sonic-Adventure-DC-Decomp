/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c612e20) */
/* WARNING: Removing unreachable block (ram,0x8c612de8) */
/* WARNING: Removing unreachable block (ram,0x8c612d12) */
/* WARNING: Removing unreachable block (ram,0x8c612cda) */
/* WARNING: Removing unreachable block (ram,0x8c612ca4) */
/* WARNING: Removing unreachable block (ram,0x8c612bee) */
/* WARNING: Removing unreachable block (ram,0x8c612cb0) */
/* WARNING: Removing unreachable block (ram,0x8c612ce8) */
/* WARNING: Removing unreachable block (ram,0x8c612d22) */
/* WARNING: Removing unreachable block (ram,0x8c612df6) */
/* WARNING: Removing unreachable block (ram,0x8c612e30) */
/* WARNING: Removing unreachable block (ram,0x8c612c0c) */
/* WARNING: Removing unreachable block (ram,0x8c612db0) */
/* WARNING: Removing unreachable block (ram,0x8c612d7a) */
/* WARNING: Removing unreachable block (ram,0x8c612d86) */
/* WARNING: Removing unreachable block (ram,0x8c612dbe) */

void FUN_8c61295a(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                 undefined4 param_5,undefined4 param_6)

{
  int *piVar1;
  short *psVar2;
  double *pdVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  float *pfVar8;
  int iVar9;
  undefined4 *puVar10;
  int unaff_r8;
  double *pdVar11;
  int *piVar12;
  int iVar13;
  double *pdVar14;
  double *pdVar15;
  undefined4 *puVar16;
  double in_dr0;
  double dVar17;
  float fVar19;
  undefined8 uVar18;
  float fVar20;
  double in_dr2;
  double dVar21;
  double dVar22;
  double dVar23;
  undefined4 uVar24;
  uint uVar26;
  double dVar25;
  double dVar27;
  double dVar28;
  undefined4 uVar30;
  undefined8 in_dr12;
  double in_xd0;
  double in_xd2;
  double in_xd4;
  double in_xd6;
  double in_xd8;
  double in_xd10;
  bool bVar31;
  undefined2 uVar32;
  short sVar33;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  short local_5c [10];
  int aiStack_48 [10];
  undefined1 local_20 [4];
  undefined4 local_1c;
  undefined4 uVar29;
  
  piVar1 = DAT_8c612a6c._4_4_;
  iVar6 = DAT_8c612a68;
  if (in_FPSCR_SZ == '\0') {
    puVar16 = &local_1c;
    _local_20 = CONCAT44((int)((ulonglong)in_dr12 >> 0x20),local_20);
  }
  else {
    puVar16 = (undefined4 *)local_20;
  }
  puVar10 = (undefined4 *)((int)puVar16 + -0xc);
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)DAT_8c612a64);
    dVar27 = (double)CONCAT44(0x3f800000,param_1);
  }
  else {
    in_xd2 = *DAT_8c612a64;
    dVar27 = in_xd4;
  }
  if (in_FPSCR_PR == '\0') {
    dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) / SUB84(in_dr2,0),SUB84(dVar27,0));
  }
  else {
    dVar27 = dVar27 / in_dr2;
  }
  pdVar11 = (double *)((int)puVar16 + -0x30);
  *(undefined4 **)((int)puVar16 + -0x50) = puVar10;
  *puVar10 = param_4;
  *(undefined4 *)((int)puVar16 + -8) = param_5;
  *(undefined4 *)((int)puVar16 + -4) = param_6;
  *(undefined4 *)((int)puVar16 + -0x4c) = 0;
  piVar12 = *(int **)((int)puVar16 + -0x50);
  *(double **)((int)puVar16 + -0x50) = pdVar11;
  pdVar14 = pdVar11;
  pdVar15 = pdVar11;
  if (in_FPSCR_SZ == '\0') {
    dVar23 = (double)CONCAT44((undefined4)DAT_8c612a6c,0x3f800000);
  }
  else {
    dVar23 = (double)CONCAT44(DAT_8c612a6c._4_4_,(undefined4)DAT_8c612a6c);
  }
  do {
    *(int *)((int)puVar16 + -0x48) = *(int *)((int)puVar16 + -0x4c) * 4;
    *(undefined4 **)((int)puVar16 + -0x40) = puVar10 + *(int *)((int)puVar16 + -0x4c);
    pfVar8 = (float *)puVar10[*(int *)((int)puVar16 + -0x4c)];
    *(undefined1 **)((int)puVar16 + -0x44) =
         (undefined1 *)((int)puVar16 + *(int *)((int)puVar16 + -0x48) + -0x3c);
    *(undefined1 **)((int)puVar16 + -0x54) =
         (undefined1 *)((int)puVar16 + *(int *)((int)puVar16 + -0x48) + -0x3c);
    pdVar3 = (double *)*piVar1;
    if (in_FPSCR_SZ == '\0') {
      dVar17 = (double)CONCAT44(pfVar8[1],SUB84(in_dr0,0));
      dVar21 = (double)CONCAT44(*(undefined4 *)pdVar3,*(undefined4 *)(pdVar3 + 2));
    }
    else {
      dVar17 = *(double *)(pfVar8 + 1);
      in_xd2 = pdVar3[2];
      dVar21 = *pdVar3;
    }
    if (in_FPSCR_PR == '\0') {
      dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),
                                SUB84(dVar21,0) * (float)((ulonglong)dVar17 >> 0x20));
    }
    else {
      dVar21 = dVar21 * dVar17;
    }
    if (in_FPSCR_SZ == '\0') {
      fVar19 = *pfVar8;
    }
    else {
      fVar19 = (float)((ulonglong)*(undefined8 *)pfVar8 >> 0x20);
    }
    if (in_FPSCR_PR == '\0') {
      dVar21 = (double)(ulonglong)
                       (uint)(fVar19 * (float)((ulonglong)dVar21 >> 0x20) + SUB84(dVar21,0));
    }
    if (in_FPSCR_SZ == '\0') {
      fVar19 = pfVar8[2];
      dVar17 = (double)CONCAT44(*(undefined4 *)(pdVar3 + 4),SUB84(dVar21,0));
    }
    else {
      fVar19 = (float)((ulonglong)*(undefined8 *)(pfVar8 + 2) >> 0x20);
      dVar17 = pdVar3[4];
    }
    if (in_FPSCR_PR == '\0') {
      dVar17 = (double)(ulonglong)
                       (uint)(fVar19 * (float)((ulonglong)dVar17 >> 0x20) + SUB84(dVar17,0));
    }
    uVar26 = (uint)((ulonglong)dVar23 >> 0x20);
    dVar25 = dVar23;
    dVar21 = dVar23;
    if (in_FPSCR_SZ == '\0') {
      dVar25 = (double)CONCAT44(uVar26,SUB84(dVar17,0));
      dVar21 = (double)((ulonglong)uVar26 << 0x20);
    }
    if (in_FPSCR_PR == '\0') {
      uVar32 = (undefined2)
               (int)((float)((ulonglong)dVar21 >> 0x20) * SUB84(dVar25,0) +
                    (float)((ulonglong)dVar25 >> 0x20));
    }
    else {
      uVar32 = (undefined2)(int)dVar25;
    }
    **(undefined2 **)((int)puVar16 + -0x54) = uVar32;
    iVar7 = *piVar1;
    if (in_FPSCR_SZ == '\0') {
      dVar17 = (double)CONCAT44(pfVar8[1],*(undefined4 *)(iVar7 + 0x24));
      dVar21 = (double)CONCAT44(*(undefined4 *)(iVar7 + 4),*(undefined4 *)(iVar7 + 0x14));
    }
    else {
      in_xd0 = *(double *)(iVar7 + 0x24);
      dVar17 = *(double *)(pfVar8 + 1);
      in_xd2 = *(double *)(iVar7 + 0x14);
      dVar21 = *(double *)(iVar7 + 4);
    }
    if (in_FPSCR_PR == '\0') {
      dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),
                                SUB84(dVar21,0) * (float)((ulonglong)dVar17 >> 0x20));
    }
    else {
      dVar21 = dVar21 * dVar17;
    }
    if (in_FPSCR_SZ == '\0') {
      uVar18 = CONCAT44(*pfVar8,SUB84(dVar17,0));
    }
    else {
      uVar18 = *(undefined8 *)pfVar8;
    }
    if (in_FPSCR_PR == '\0') {
      dVar21 = (double)(ulonglong)
                       (uint)((float)((ulonglong)uVar18 >> 0x20) *
                              (float)((ulonglong)dVar21 >> 0x20) + SUB84(dVar21,0));
    }
    if (in_FPSCR_SZ == '\0') {
      uVar18 = CONCAT44(pfVar8[2],(int)uVar18);
      dVar17 = (double)CONCAT44(uVar26,SUB84(dVar21,0));
    }
    else {
      uVar18 = *(undefined8 *)(pfVar8 + 2);
      dVar17 = dVar23;
    }
    if (in_FPSCR_PR == '\0') {
      dVar17 = (double)CONCAT44((int)((ulonglong)dVar17 >> 0x20),
                                (float)((ulonglong)uVar18 >> 0x20) * (float)uVar18 + SUB84(dVar17,0)
                               );
    }
    in_dr0 = dVar23;
    if (in_FPSCR_SZ == '\0') {
      in_dr0 = (double)CONCAT44(uVar26,(float)uVar18);
    }
    fVar19 = (float)((ulonglong)dVar17 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      fVar19 = (float)((ulonglong)in_dr0 >> 0x20) * SUB84(dVar17,0) + fVar19;
      dVar17 = (double)CONCAT44(fVar19,SUB84(dVar17,0));
      uVar32 = (undefined2)(int)fVar19;
    }
    else {
      uVar32 = (undefined2)(int)dVar17;
    }
    *(undefined2 *)(*(int *)((int)puVar16 + -0x44) + 2) = uVar32;
    psVar2 = DAT_8c612d48;
    dVar21 = in_xd6;
    if ((*(uint *)(**(int **)((int)puVar16 + -0x40) + 0xc) & 1) == 0) {
      dVar21 = in_xd4;
      if (in_FPSCR_SZ == '\0') {
        dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),SUB84(dVar23,0));
        dVar21 = in_xd6;
      }
      *(undefined4 **)((int)puVar16 + -0x48) = puVar10 + *(int *)((int)puVar16 + -0x4c);
      iVar7 = puVar10[*(int *)((int)puVar16 + -0x4c)];
      if (in_FPSCR_SZ == '\0') {
        dVar17 = (double)CONCAT44(fVar19,*(undefined4 *)(iVar7 + 0x18));
      }
      else {
        in_xd2 = *(double *)(iVar7 + 0x18);
      }
      if (in_FPSCR_PR == '\0') {
        dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),SUB84(dVar27,0) / SUB84(dVar17,0)
                                 );
      }
      else {
        dVar27 = dVar27 / dVar17;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar17 = (double)CONCAT44(*(undefined4 *)(iVar7 + 0x10),*(undefined4 *)(iVar6 + 0xc));
      }
      else {
        in_xd2 = *(double *)(iVar6 + 0xc);
        dVar17 = *(double *)(iVar7 + 0x10);
      }
      if (in_FPSCR_PR == '\0') {
        dVar17 = (double)CONCAT44((float)((ulonglong)dVar17 >> 0x20) - SUB84(dVar17,0),
                                  SUB84(dVar17,0));
      }
      else {
        dVar17 = dVar17 - dVar17;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar17 = (double)CONCAT44((int)((ulonglong)dVar17 >> 0x20),*(undefined4 *)DAT_8c612a74);
      }
      else {
        in_xd2 = *DAT_8c612a74;
      }
      fVar19 = SUB84(dVar27,0);
      if (in_FPSCR_PR == '\0') {
        dVar17 = (double)CONCAT44(((float)((ulonglong)dVar17 >> 0x20) * fVar19) / SUB84(dVar17,0),
                                  SUB84(dVar17,0));
      }
      else {
        dVar17 = (dVar17 * dVar27) / (dVar17 * dVar27);
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)pdVar15 = (int)((ulonglong)dVar17 >> 0x20);
      }
      else {
        *pdVar15 = dVar17;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar17 = (double)(ulonglong)*(uint *)(iVar6 + 0x10);
      }
      else {
        in_xd2 = *(double *)(iVar6 + 0x10);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar17 = (double)CONCAT44(*(undefined4 *)(**(int **)((int)puVar16 + -0x48) + 0x14),
                                  SUB84(dVar17,0));
      }
      else {
        dVar17 = *(double *)(**(int **)((int)puVar16 + -0x48) + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        dVar17 = (double)CONCAT44((float)((ulonglong)dVar17 >> 0x20) - SUB84(dVar17,0),
                                  SUB84(dVar17,0));
      }
      else {
        dVar17 = dVar17 - dVar17;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar17 = (double)CONCAT44((int)((ulonglong)dVar17 >> 0x20),*(undefined4 *)DAT_8c612a78);
      }
      else {
        in_xd2 = *DAT_8c612a78;
      }
      if (in_FPSCR_PR == '\0') {
        dVar17 = (double)CONCAT44(((float)((ulonglong)dVar17 >> 0x20) * fVar19) / SUB84(dVar17,0),
                                  SUB84(dVar17,0));
      }
      else {
        dVar17 = (dVar17 * dVar27) / (dVar17 * dVar27);
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar15 + 4) = (int)((ulonglong)dVar17 >> 0x20);
        *(float *)(pdVar15 + 1) = fVar19;
      }
      else {
        *(double *)((int)pdVar15 + 4) = dVar17;
        pdVar15[1] = dVar21;
      }
    }
    else {
      if (in_FPSCR_SZ == '\0') {
        uVar29 = *(undefined4 *)(*piVar12 + 0x10);
        dVar17 = (double)CONCAT44(fVar19,uVar29);
        *(undefined4 *)pdVar14 = uVar29;
      }
      else {
        in_xd2 = *(double *)(*piVar12 + 0x10);
        *pdVar14 = in_xd2;
      }
      if (in_FPSCR_SZ == '\0') {
        uVar29 = *(undefined4 *)(*piVar12 + 0x14);
        dVar17 = (double)CONCAT44((int)((ulonglong)dVar17 >> 0x20),uVar29);
        *(undefined4 *)((int)pdVar14 + 4) = uVar29;
      }
      else {
        in_xd2 = *(double *)(*piVar12 + 0x14);
        *(double *)((int)pdVar14 + 4) = in_xd2;
      }
      if (in_FPSCR_SZ == '\0') {
        uVar29 = *(undefined4 *)(*piVar12 + 0x18);
        dVar17 = (double)CONCAT44((int)((ulonglong)dVar17 >> 0x20),uVar29);
        *(undefined4 *)(pdVar14 + 1) = uVar29;
      }
      else {
        in_xd2 = *(double *)(*piVar12 + 0x18);
        pdVar14[1] = in_xd2;
      }
    }
    iVar7 = *(int *)((int)puVar16 + -0x4c);
    pdVar14 = (double *)((int)pdVar14 + 0xc);
    *(int *)((int)puVar16 + -0x4c) = iVar7 + 1;
    piVar12 = piVar12 + 1;
    pdVar15 = (double *)((int)pdVar15 + 0xc);
    in_xd6 = dVar21;
  } while (iVar7 + 1 < 3);
  pdVar14 = *(double **)((int)puVar16 + -0x50);
  if (in_FPSCR_SZ == '\0') {
    uVar29 = *(undefined4 *)(pdVar14 + 1);
    dVar25 = (double)CONCAT44(*(undefined4 *)((int)puVar16 + -0x20),SUB84(dVar23,0));
    uVar30 = *(undefined4 *)((int)pdVar14 + 4);
    dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),*(undefined4 *)((int)puVar16 + -0x1c)
                             );
    dVar17 = (double)CONCAT44((int)((ulonglong)dVar17 >> 0x20),uVar30);
    dVar28 = (double)CONCAT44(uVar29,SUB84(in_dr0,0));
    dVar23 = (double)CONCAT44(uVar30,param_3);
    dVar22 = (double)CONCAT44(uVar29,param_2);
  }
  else {
    dVar28 = pdVar14[1];
    dVar25 = *(double *)((int)puVar16 + -0x20);
    in_xd2 = *(double *)((int)pdVar14 + 4);
    dVar21 = *(double *)((int)puVar16 + -0x1c);
    uVar30 = (undefined4)((ulonglong)in_xd2 >> 0x20);
    uVar29 = (undefined4)((ulonglong)dVar28 >> 0x20);
    dVar23 = in_xd2;
    dVar22 = dVar28;
  }
  fVar19 = (float)((ulonglong)dVar25 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    fVar20 = SUB84(dVar17,0) * SUB84(dVar27,0);
    dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) * fVar19,SUB84(dVar28,0));
  }
  else {
    fVar20 = SUB84(dVar17 * dVar27,0);
    dVar28 = dVar28 * dVar25;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar23 = (double)CONCAT44(uVar30,*(undefined4 *)pdVar14);
    dVar22 = (double)CONCAT44(uVar29,fVar20);
    dVar17 = (double)CONCAT44(uVar29,*(undefined4 *)((int)puVar16 + -0x24));
  }
  else {
    in_xd10 = *pdVar14;
    in_xd8 = *(double *)((int)puVar16 + -0x24);
    dVar17 = dVar22;
  }
  if (in_FPSCR_PR == '\0') {
    dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) * SUB84(dVar17,0),SUB84(dVar22,0));
  }
  else {
    dVar22 = dVar22 * dVar17;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),SUB84(dVar17,0));
    in_xd8 = in_xd0;
  }
  if (in_FPSCR_PR == '\0') {
    dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) - SUB84(dVar22,0),SUB84(dVar28,0));
  }
  else {
    dVar28 = dVar28 - dVar22;
  }
  dVar17 = in_xd10;
  if (in_FPSCR_SZ == '\0') {
    dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),SUB84(dVar23,0));
    dVar17 = in_xd2;
  }
  if (in_FPSCR_PR == '\0') {
    dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),SUB84(dVar22,0) * SUB84(dVar27,0));
  }
  else {
    dVar22 = dVar22 * dVar27;
  }
  if (in_FPSCR_SZ == '\0') {
    in_xd10 = (double)CONCAT44(SUB84(dVar23,0),(int)in_dr12);
  }
  if (in_FPSCR_PR == '\0') {
    dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),
                              SUB84(dVar28,0) * (float)((ulonglong)dVar23 >> 0x20));
    fVar20 = SUB84(dVar22,0) - (float)((ulonglong)dVar22 >> 0x20);
  }
  else {
    dVar28 = dVar28 * dVar23;
    fVar20 = SUB84(dVar22 - dVar22,0);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar23 = (double)CONCAT44(*(undefined4 *)((int)puVar16 + -0x14),fVar20);
  }
  else {
    dVar23 = *(double *)((int)puVar16 + -0x14);
  }
  if (in_FPSCR_PR == '\0') {
    dVar23 = (double)CONCAT44((float)((ulonglong)dVar23 >> 0x20) * SUB84(dVar23,0),SUB84(dVar23,0));
  }
  else {
    dVar23 = dVar23 * dVar23;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(undefined4 *)((int)puVar16 + -0x18)
                             );
  }
  else {
    dVar17 = *(double *)((int)puVar16 + -0x18);
  }
  if (in_FPSCR_PR == '\0') {
    dVar23 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) * SUB84(dVar23,0) +
                              (float)((ulonglong)dVar23 >> 0x20),SUB84(dVar23,0));
    dVar22 = (double)CONCAT44((float)((ulonglong)in_xd10 >> 0x20) * fVar19,SUB84(in_xd10,0));
  }
  else {
    dVar22 = in_xd10 * dVar25;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(undefined4 *)((int)puVar16 + -0x10)
                             );
  }
  else {
    dVar17 = *(double *)((int)puVar16 + -0x10);
  }
  if (in_FPSCR_PR == '\0') {
    fVar19 = SUB84(dVar28,0) - (float)((ulonglong)dVar22 >> 0x20);
  }
  else {
    fVar19 = SUB84(dVar28 - dVar22,0);
  }
  if (in_FPSCR_SZ == '\0') {
    in_xd8 = (double)CONCAT44(fVar19,fVar19);
  }
  if (in_FPSCR_PR == '\0') {
    dVar23 = (double)((ulonglong)
                      (uint)((float)((ulonglong)in_xd8 >> 0x20) * SUB84(dVar23,0) +
                            (float)((ulonglong)dVar23 >> 0x20)) << 0x20);
  }
  dVar28 = (double)((ulonglong)dVar23 & 0xffffffff00000000);
  dVar22 = dVar28;
  if (in_FPSCR_SZ == '\0') {
    dVar22 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),SUB84(dVar25,0));
  }
  if (in_FPSCR_PR == '\0') {
    bVar31 = (float)((ulonglong)dVar22 >> 0x20) < 0.0;
  }
  else {
    bVar31 = dVar22 < dVar28;
  }
  if (((uint)bVar31 & *DAT_8c612d44) == 0) {
    *DAT_8c612d44 = 0;
    iVar7 = DAT_8c612d4c;
    iVar9 = 0;
    *(undefined4 *)((int)puVar16 + -0x50) = 2;
    iVar13 = 0;
    do {
      *(int *)((int)puVar16 + -0x4c) = *(int *)((int)puVar16 + -0x50) * 4;
      if ((*(uint *)(puVar10[*(int *)((int)puVar16 + -0x50)] + 0xc) & 1 ^
          *(uint *)(puVar10[iVar13] + 0xc) & 1) == 1) {
        iVar4 = *(int *)((int)puVar16 + -0x50) * 0xc + (int)pdVar11;
        *(int *)((int)puVar16 + -0x44) = iVar4;
        if (in_FPSCR_SZ == '\0') {
          dVar23 = (double)CONCAT44(*(undefined4 *)(iVar4 + 8),SUB84(dVar22,0));
          dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),*(undefined4 *)DAT_8c612d50);
        }
        else {
          dVar23 = *(double *)(iVar4 + 8);
          dVar17 = *DAT_8c612d50;
        }
        fVar19 = (float)((ulonglong)dVar23 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          fVar20 = SUB84(dVar28,0) - fVar19;
        }
        else {
          fVar20 = SUB84(dVar28 - dVar23,0);
        }
        iVar4 = iVar13 * 0xc + (int)pdVar11;
        *(int *)((int)puVar16 + -0x48) = iVar4;
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(*(undefined4 *)(iVar4 + 8),fVar20);
        }
        else {
          dVar25 = *(double *)(iVar4 + 8);
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) - fVar19,SUB84(dVar25,0));
        }
        else {
          dVar25 = dVar25 - dVar23;
        }
        dVar22 = dVar17;
        if (in_FPSCR_SZ == '\0') {
          dVar22 = (double)CONCAT44(SUB84(dVar25,0),SUB84(dVar23,0));
        }
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                    *(undefined4 *)*(double **)((int)puVar16 + -0x48));
          dVar23 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),
                                    *(undefined4 *)*(double **)((int)puVar16 + -0x44));
        }
        else {
          dVar17 = **(double **)((int)puVar16 + -0x48);
          dVar21 = **(double **)((int)puVar16 + -0x44);
          dVar23 = dVar27;
        }
        if (in_FPSCR_PR == '\0') {
          dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) /
                                    (float)((ulonglong)dVar25 >> 0x20),SUB84(dVar22,0));
          fVar19 = SUB84(dVar25,0) - SUB84(dVar23,0);
        }
        else {
          dVar22 = dVar22 / dVar25;
          fVar19 = SUB84(dVar25 - dVar23,0);
        }
        uVar29 = (undefined4)((ulonglong)dVar22 >> 0x20);
        dVar28 = dVar22;
        dVar25 = dVar21;
        if (in_FPSCR_SZ == '\0') {
          dVar28 = (double)CONCAT44(uVar29,SUB84(in_xd8,0));
          dVar25 = (double)CONCAT44(SUB84(dVar23,0),fVar19);
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) * SUB84(dVar25,0) +
                                    (float)((ulonglong)dVar25 >> 0x20),SUB84(dVar25,0));
        }
        uVar30 = (undefined4)((ulonglong)dVar23 >> 0x20);
        dVar27 = dVar23;
        if (in_FPSCR_SZ == '\0') {
          dVar23 = (double)CONCAT44(uVar30,SUB84(dVar28,0));
          dVar27 = (double)CONCAT44(uVar30,(int)((ulonglong)dVar25 >> 0x20));
          dVar25 = dVar21;
        }
        iVar4 = iVar9 * 0x20 + iVar7;
        *(int *)((int)puVar16 + -0x50) = iVar4;
        if (in_FPSCR_SZ == '\0') {
          dVar21 = (double)CONCAT44(*(undefined4 *)(iVar6 + 0xc),*(undefined4 *)DAT_8c612d54);
        }
        else {
          dVar17 = *DAT_8c612d54;
          dVar21 = *(double *)(iVar6 + 0xc);
        }
        if (in_FPSCR_PR == '\0') {
          fVar19 = SUB84(dVar21,0) * SUB84(dVar27,0);
          dVar21 = (double)CONCAT44((float)((ulonglong)dVar23 >> 0x20) * fVar19 +
                                    (float)((ulonglong)dVar21 >> 0x20),fVar19);
        }
        else {
          dVar21 = dVar21 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(iVar4 + 0x10) = (int)((ulonglong)dVar21 >> 0x20);
          dVar23 = (double)CONCAT44(uVar29,SUB84(dVar23,0));
        }
        else {
          *(double *)(iVar4 + 0x10) = dVar21;
          dVar23 = dVar22;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(uVar30,*(undefined4 *)(*(int *)((int)puVar16 + -0x44) + 4));
        }
        else {
          dVar25 = *(double *)(*(int *)((int)puVar16 + -0x44) + 4);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar21 = (double)CONCAT44(SUB84(dVar27,0),
                                    *(undefined4 *)(*(int *)((int)puVar16 + -0x48) + 4));
        }
        else {
          dVar17 = *(double *)(*(int *)((int)puVar16 + -0x48) + 4);
          dVar21 = dVar25;
        }
        if (in_FPSCR_PR == '\0') {
          dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),
                                    SUB84(dVar21,0) - SUB84(dVar27,0));
        }
        else {
          dVar21 = dVar21 - dVar27;
        }
        if (in_FPSCR_PR == '\0') {
          dVar21 = (double)CONCAT44((float)((ulonglong)dVar23 >> 0x20) * SUB84(dVar21,0) +
                                    (float)((ulonglong)dVar21 >> 0x20),SUB84(dVar21,0));
        }
        if (in_FPSCR_SZ == '\0') {
          uVar30 = (undefined4)((ulonglong)dVar21 >> 0x20);
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),uVar30);
          dVar21 = dVar25;
          dVar25 = (double)CONCAT44(uVar30,*(undefined4 *)DAT_8c612d58);
        }
        else {
          dVar17 = *DAT_8c612d58;
          dVar25 = dVar21;
        }
        if (in_FPSCR_PR == '\0') {
          fVar19 = SUB84(dVar25,0) * SUB84(dVar27,0);
        }
        else {
          fVar19 = SUB84(dVar25 * dVar27,0);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar18 = CONCAT44(*(undefined4 *)(iVar6 + 0x10),fVar19);
          dVar23 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),SUB84(dVar23,0));
        }
        else {
          uVar18 = *(undefined8 *)(iVar6 + 0x10);
          dVar23 = dVar27;
        }
        fVar19 = (float)((ulonglong)dVar23 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          uVar18 = CONCAT44(fVar19 * (float)uVar18 + (float)((ulonglong)uVar18 >> 0x20),
                            (float)uVar18);
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(*(int *)((int)puVar16 + -0x50) + 0x14) = (int)((ulonglong)uVar18 >> 0x20);
        }
        else {
          *(undefined8 *)(*(int *)((int)puVar16 + -0x50) + 0x14) = uVar18;
        }
        if (in_FPSCR_SZ == '\0') {
          *(float *)(*(int *)((int)puVar16 + -0x50) + 0x18) = fVar19;
        }
        else {
          *(double *)(*(int *)((int)puVar16 + -0x50) + 0x18) = dVar23;
        }
        in_xd8 = dVar22;
        if (in_FPSCR_SZ == '\0') {
          in_xd8 = (double)CONCAT44(uVar29,SUB84(dVar23,0));
        }
        *(short **)((int)puVar16 + -0x48) = psVar2 + iVar9 * 2;
        *(short **)((int)puVar16 + -0x44) =
             (short *)((int)puVar16 + *(int *)((int)puVar16 + -0x4c) + -0x3c);
        sVar33 = *(short *)((int)puVar16 + *(int *)((int)puVar16 + -0x4c) + -0x3c);
        *(short *)((int)puVar16 + -0x50) = sVar33;
        *(short **)((int)puVar16 + -0x40) = (short *)((int)puVar16 + iVar13 * 4 + -0x3c);
        iVar4 = (int)*(short *)((int)puVar16 + iVar13 * 4 + -0x3c) -
                (int)*(short *)((int)puVar16 + -0x50);
        fVar19 = (float)((ulonglong)in_xd8 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          sVar33 = (short)(int)(fVar19 * (float)iVar4 + (float)(int)sVar33);
        }
        else {
          sVar33 = (short)(int)(double)iVar4;
        }
        psVar2[iVar9 * 2] = sVar33;
        iVar9 = iVar9 + 1;
        sVar33 = *(short *)(*(int *)((int)puVar16 + -0x44) + 2);
        *(short *)((int)puVar16 + -0x4c) = sVar33;
        if (in_FPSCR_PR == '\0') {
          fVar20 = (float)((int)*(short *)(*(int *)((int)puVar16 + -0x40) + 2) -
                          (int)*(short *)((int)puVar16 + -0x4c));
          fVar19 = fVar19 * fVar20 + (float)(int)sVar33;
          dVar28 = (double)CONCAT44(fVar20,fVar19);
          uVar32 = (undefined2)(int)fVar19;
        }
        else {
          dVar28 = (double)((int)*(short *)(*(int *)((int)puVar16 + -0x40) + 2) -
                           (int)*(short *)((int)puVar16 + -0x4c));
          uVar32 = (undefined2)(int)dVar28;
        }
        *(undefined2 *)(*(int *)((int)puVar16 + -0x48) + 2) = uVar32;
      }
      if ((*(uint *)(puVar10[iVar13] + 0xc) & 1) == 0) {
        iVar5 = iVar9 * 0x20 + iVar7;
        *(int *)((int)puVar16 + -0x4c) = iVar5;
        *(undefined4 **)((int)puVar16 + -0x50) = puVar10 + iVar13;
        iVar4 = puVar10[iVar13];
        if (in_FPSCR_SZ == '\0') {
          uVar29 = *(undefined4 *)(iVar4 + 0x10);
          dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),uVar29);
          *(undefined4 *)(iVar5 + 0x10) = uVar29;
        }
        else {
          dVar17 = *(double *)(iVar4 + 0x10);
          *(double *)(iVar5 + 0x10) = dVar17;
        }
        if (in_FPSCR_SZ == '\0') {
          uVar29 = *(undefined4 *)(**(int **)((int)puVar16 + -0x50) + 0x14);
          dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),uVar29);
          *(undefined4 *)(*(int *)((int)puVar16 + -0x4c) + 0x14) = uVar29;
        }
        else {
          dVar17 = *(double *)(**(int **)((int)puVar16 + -0x50) + 0x14);
          *(double *)(*(int *)((int)puVar16 + -0x4c) + 0x14) = dVar17;
        }
        if (in_FPSCR_SZ == '\0') {
          uVar29 = *(undefined4 *)(**(int **)((int)puVar16 + -0x50) + 0x18);
          dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),uVar29);
          *(undefined4 *)(*(int *)((int)puVar16 + -0x4c) + 0x18) = uVar29;
        }
        else {
          dVar17 = *(double *)(**(int **)((int)puVar16 + -0x50) + 0x18);
          *(double *)(*(int *)((int)puVar16 + -0x4c) + 0x18) = dVar17;
        }
        iVar4 = iVar9 * 2;
        *(short **)((int)puVar16 + -0x4c) = psVar2 + iVar4;
        *(short **)((int)puVar16 + -0x50) = (short *)((int)puVar16 + iVar13 * 4 + -0x3c);
        iVar9 = iVar9 + 1;
        psVar2[iVar4] = *(short *)((int)puVar16 + iVar13 * 4 + -0x3c);
        *(undefined2 *)(*(int *)((int)puVar16 + -0x4c) + 2) =
             *(undefined2 *)(*(int *)((int)puVar16 + -0x50) + 2);
      }
      uVar29 = DAT_8c612d60;
      piVar1 = DAT_8c612d5c;
      *(int *)((int)puVar16 + -0x50) = iVar13;
      iVar13 = iVar13 + 1;
    } while (iVar13 < 3);
    if (in_FPSCR_SZ == '\0') {
      dVar27 = (double)CONCAT44((undefined4)DAT_8c612d64,SUB84(dVar22,0));
    }
    else {
      dVar27 = (double)CONCAT44(DAT_8c612d64._4_4_,(undefined4)DAT_8c612d64);
    }
    uVar24 = SUB84(dVar27,0);
    uVar30 = (undefined4)((ulonglong)dVar28 >> 0x20);
    fVar19 = (float)((ulonglong)dVar27 >> 0x20);
    if (iVar9 == 3) {
      if (in_FPSCR_SZ == '\0') {
        pdVar14 = (double *)(unaff_r8 + 0x18);
        *(undefined4 *)pdVar14 = uVar24;
      }
      else {
        pdVar14 = (double *)(unaff_r8 + 0x14);
        *pdVar14 = in_xd4;
      }
      if (in_FPSCR_PR == '\0') {
        dVar23 = (double)CONCAT44(uVar30,(float)(int)psVar2[1] * fVar19);
      }
      else {
        dVar23 = (double)CONCAT44(uVar30,(float)(int)psVar2[1]) * dVar27;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar14 = (double *)((int)pdVar14 + -4);
        *(int *)pdVar14 = SUB84(dVar23,0);
      }
      else {
        pdVar14 = pdVar14 + -1;
        *pdVar14 = dVar17;
      }
      uVar30 = (undefined4)((ulonglong)dVar23 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar23 = (double)CONCAT44(uVar30,(float)(int)*psVar2 * fVar19);
      }
      else {
        dVar23 = (double)CONCAT44(uVar30,(float)(int)*psVar2) * dVar27;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar14 + -4) = SUB84(dVar23,0);
        *(undefined4 *)(pdVar14 + -1) = *(undefined4 *)(iVar7 + 0x18);
        *(undefined4 *)((int)pdVar14 + -0xc) = *(undefined4 *)(iVar7 + 0x14);
        dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(undefined4 *)(iVar7 + 0x10));
        pdVar14 = pdVar14 + -2;
        *(undefined4 *)pdVar14 = *(undefined4 *)(iVar7 + 0x10);
      }
      else {
        pdVar14[-1] = dVar17;
        pdVar14[-2] = *(double *)(iVar7 + 0x18);
        pdVar14[-3] = *(double *)(iVar7 + 0x14);
        dVar17 = *(double *)(iVar7 + 0x10);
        pdVar14 = pdVar14 + -4;
        *pdVar14 = dVar17;
      }
      *(undefined4 *)((int)pdVar14 + -4) = uVar29;
      if (in_FPSCR_SZ == '\0') {
        pdVar14 = (double *)((int)pdVar14 + 0x34);
        *(undefined4 *)pdVar14 = uVar24;
      }
      else {
        pdVar14 = pdVar14 + 6;
        *pdVar14 = in_xd4;
      }
      uVar30 = (undefined4)((ulonglong)dVar23 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar23 = (double)CONCAT44(uVar30,(float)(int)psVar2[3] * fVar19);
      }
      else {
        dVar23 = (double)CONCAT44(uVar30,(float)(int)psVar2[3]) * dVar27;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar14 = (double *)((int)pdVar14 + -4);
        *(int *)pdVar14 = SUB84(dVar23,0);
      }
      else {
        pdVar14 = pdVar14 + -1;
        *pdVar14 = dVar17;
      }
      uVar30 = (undefined4)((ulonglong)dVar23 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar23 = (double)CONCAT44(uVar30,(float)(int)psVar2[2] * fVar19);
      }
      else {
        dVar23 = (double)CONCAT44(uVar30,(float)(int)psVar2[2]) * dVar27;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar14 + -4) = SUB84(dVar23,0);
        *(undefined4 *)(pdVar14 + -1) = *(undefined4 *)(iVar7 + 0x38);
        *(undefined4 *)((int)pdVar14 + -0xc) = *(undefined4 *)(iVar7 + 0x34);
        dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(undefined4 *)(iVar7 + 0x30));
        pdVar14 = pdVar14 + -2;
        *(undefined4 *)pdVar14 = *(undefined4 *)(iVar7 + 0x30);
      }
      else {
        pdVar14[-1] = dVar17;
        pdVar14[-2] = *(double *)(iVar7 + 0x38);
        pdVar14[-3] = *(double *)(iVar7 + 0x34);
        dVar17 = *(double *)(iVar7 + 0x30);
        pdVar14 = pdVar14 + -4;
        *pdVar14 = dVar17;
      }
      *(undefined4 *)((int)pdVar14 + -4) = uVar29;
      if (in_FPSCR_SZ == '\0') {
        pdVar14 = (double *)((int)pdVar14 + 0x34);
        *(undefined4 *)pdVar14 = uVar24;
      }
      else {
        pdVar14 = pdVar14 + 6;
        *pdVar14 = in_xd4;
      }
      uVar29 = (undefined4)((ulonglong)dVar23 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar23 = (double)CONCAT44(uVar29,(float)(int)psVar2[5] * fVar19);
      }
      else {
        dVar23 = (double)CONCAT44(uVar29,(float)(int)psVar2[5]) * dVar27;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar14 = (double *)((int)pdVar14 + -4);
        *(int *)pdVar14 = SUB84(dVar23,0);
      }
      else {
        pdVar14 = pdVar14 + -1;
        *pdVar14 = dVar17;
      }
      uVar29 = DAT_8c612d64._4_4_;
      if (in_FPSCR_PR == '\0') {
        fVar19 = (float)(int)psVar2[4] * fVar19;
      }
      else {
        fVar19 = SUB84((double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),(float)(int)psVar2[4]) *
                       dVar27,0);
      }
      if (in_FPSCR_SZ == '\0') {
        *(float *)((int)pdVar14 + -4) = fVar19;
        *(undefined4 *)(pdVar14 + -1) = *(undefined4 *)(iVar7 + 0x58);
        *(undefined4 *)((int)pdVar14 + -0xc) = *(undefined4 *)(iVar7 + 0x54);
        pdVar14 = pdVar14 + -2;
        *(undefined4 *)pdVar14 = *(undefined4 *)(iVar7 + 0x50);
      }
      else {
        pdVar14[-1] = dVar17;
        pdVar14[-2] = *(double *)(iVar7 + 0x58);
        pdVar14[-3] = *(double *)(iVar7 + 0x54);
        pdVar14 = pdVar14 + -4;
        *pdVar14 = *(double *)(iVar7 + 0x50);
      }
      *(undefined4 *)((int)pdVar14 + -4) = uVar29;
      iVar6 = *piVar1 + 3;
    }
    else {
      if (in_FPSCR_SZ == '\0') {
        pdVar14 = (double *)(unaff_r8 + 0x18);
        *(undefined4 *)pdVar14 = uVar24;
      }
      else {
        pdVar14 = (double *)(unaff_r8 + 0x14);
        *pdVar14 = in_xd4;
      }
      if (in_FPSCR_PR == '\0') {
        dVar23 = (double)CONCAT44(uVar30,(float)(int)psVar2[1] * fVar19);
      }
      else {
        dVar23 = (double)CONCAT44(uVar30,(float)(int)psVar2[1]) * dVar27;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar14 = (double *)((int)pdVar14 + -4);
        *(int *)pdVar14 = SUB84(dVar23,0);
      }
      else {
        pdVar14 = pdVar14 + -1;
        *pdVar14 = dVar17;
      }
      uVar30 = (undefined4)((ulonglong)dVar23 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar23 = (double)CONCAT44(uVar30,(float)(int)*psVar2 * fVar19);
      }
      else {
        dVar23 = (double)CONCAT44(uVar30,(float)(int)*psVar2) * dVar27;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar14 + -4) = SUB84(dVar23,0);
        *(undefined4 *)(pdVar14 + -1) = *(undefined4 *)(iVar7 + 0x18);
        *(undefined4 *)((int)pdVar14 + -0xc) = *(undefined4 *)(iVar7 + 0x14);
        dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(undefined4 *)(iVar7 + 0x10));
        pdVar14 = pdVar14 + -2;
        *(undefined4 *)pdVar14 = *(undefined4 *)(iVar7 + 0x10);
      }
      else {
        pdVar14[-1] = dVar17;
        pdVar14[-2] = *(double *)(iVar7 + 0x18);
        pdVar14[-3] = *(double *)(iVar7 + 0x14);
        dVar17 = *(double *)(iVar7 + 0x10);
        pdVar14 = pdVar14 + -4;
        *pdVar14 = dVar17;
      }
      *(undefined4 *)((int)pdVar14 + -4) = uVar29;
      if (in_FPSCR_SZ == '\0') {
        pdVar14 = (double *)((int)pdVar14 + 0x34);
        *(undefined4 *)pdVar14 = uVar24;
      }
      else {
        pdVar14 = pdVar14 + 6;
        *pdVar14 = in_xd4;
      }
      uVar30 = (undefined4)((ulonglong)dVar23 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar23 = (double)CONCAT44(uVar30,(float)(int)psVar2[3] * fVar19);
      }
      else {
        dVar23 = (double)CONCAT44(uVar30,(float)(int)psVar2[3]) * dVar27;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar14 = (double *)((int)pdVar14 + -4);
        *(int *)pdVar14 = SUB84(dVar23,0);
      }
      else {
        pdVar14 = pdVar14 + -1;
        *pdVar14 = dVar17;
      }
      uVar30 = (undefined4)((ulonglong)dVar23 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar23 = (double)CONCAT44(uVar30,(float)(int)psVar2[2] * fVar19);
      }
      else {
        dVar23 = (double)CONCAT44(uVar30,(float)(int)psVar2[2]) * dVar27;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar14 + -4) = SUB84(dVar23,0);
        *(undefined4 *)(pdVar14 + -1) = *(undefined4 *)(iVar7 + 0x38);
        *(undefined4 *)((int)pdVar14 + -0xc) = *(undefined4 *)(iVar7 + 0x34);
        dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(undefined4 *)(iVar7 + 0x30));
        pdVar14 = pdVar14 + -2;
        *(undefined4 *)pdVar14 = *(undefined4 *)(iVar7 + 0x30);
      }
      else {
        pdVar14[-1] = dVar17;
        pdVar14[-2] = *(double *)(iVar7 + 0x38);
        pdVar14[-3] = *(double *)(iVar7 + 0x34);
        dVar17 = *(double *)(iVar7 + 0x30);
        pdVar14 = pdVar14 + -4;
        *pdVar14 = dVar17;
      }
      *(undefined4 *)((int)pdVar14 + -4) = uVar29;
      if (in_FPSCR_SZ == '\0') {
        pdVar14 = (double *)((int)pdVar14 + 0x34);
        *(undefined4 *)pdVar14 = uVar24;
      }
      else {
        pdVar14 = pdVar14 + 6;
        *pdVar14 = in_xd4;
      }
      uVar30 = (undefined4)((ulonglong)dVar23 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar23 = (double)CONCAT44(uVar30,(float)(int)psVar2[7] * fVar19);
      }
      else {
        dVar23 = (double)CONCAT44(uVar30,(float)(int)psVar2[7]) * dVar27;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar14 = (double *)((int)pdVar14 + -4);
        *(int *)pdVar14 = SUB84(dVar23,0);
      }
      else {
        pdVar14 = pdVar14 + -1;
        *pdVar14 = dVar17;
      }
      uVar30 = (undefined4)((ulonglong)dVar23 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar23 = (double)CONCAT44(uVar30,(float)(int)psVar2[6] * fVar19);
      }
      else {
        dVar23 = (double)CONCAT44(uVar30,(float)(int)psVar2[6]) * dVar27;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar14 + -4) = SUB84(dVar23,0);
        *(undefined4 *)(pdVar14 + -1) = *(undefined4 *)(iVar7 + 0x78);
        *(undefined4 *)((int)pdVar14 + -0xc) = *(undefined4 *)(iVar7 + 0x74);
        dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(undefined4 *)(iVar7 + 0x70));
        pdVar14 = pdVar14 + -2;
        *(undefined4 *)pdVar14 = *(undefined4 *)(iVar7 + 0x70);
      }
      else {
        pdVar14[-1] = dVar17;
        pdVar14[-2] = *(double *)(iVar7 + 0x78);
        pdVar14[-3] = *(double *)(iVar7 + 0x74);
        dVar17 = *(double *)(iVar7 + 0x70);
        pdVar14 = pdVar14 + -4;
        *pdVar14 = dVar17;
      }
      *(undefined4 *)((int)pdVar14 + -4) = uVar29;
      if (in_FPSCR_SZ == '\0') {
        pdVar14 = (double *)((int)pdVar14 + 0x34);
        *(undefined4 *)pdVar14 = uVar24;
      }
      else {
        pdVar14 = pdVar14 + 6;
        *pdVar14 = in_xd4;
      }
      uVar29 = (undefined4)((ulonglong)dVar23 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar23 = (double)CONCAT44(uVar29,(float)(int)psVar2[5] * fVar19);
      }
      else {
        dVar23 = (double)CONCAT44(uVar29,(float)(int)psVar2[5]) * dVar27;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar14 = (double *)((int)pdVar14 + -4);
        *(int *)pdVar14 = SUB84(dVar23,0);
      }
      else {
        pdVar14 = pdVar14 + -1;
        *pdVar14 = dVar17;
      }
      uVar29 = DAT_8c612f2c;
      if (in_FPSCR_PR == '\0') {
        fVar19 = (float)(int)psVar2[4] * fVar19;
      }
      else {
        fVar19 = SUB84((double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),(float)(int)psVar2[4]) *
                       dVar27,0);
      }
      if (in_FPSCR_SZ == '\0') {
        *(float *)((int)pdVar14 + -4) = fVar19;
        *(undefined4 *)(pdVar14 + -1) = *(undefined4 *)(iVar7 + 0x58);
        *(undefined4 *)((int)pdVar14 + -0xc) = *(undefined4 *)(iVar7 + 0x54);
        pdVar14 = pdVar14 + -2;
        *(undefined4 *)pdVar14 = *(undefined4 *)(iVar7 + 0x50);
      }
      else {
        pdVar14[-1] = dVar17;
        pdVar14[-2] = *(double *)(iVar7 + 0x58);
        pdVar14[-3] = *(double *)(iVar7 + 0x54);
        pdVar14 = pdVar14 + -4;
        *pdVar14 = *(double *)(iVar7 + 0x50);
      }
      *(undefined4 *)((int)pdVar14 + -4) = uVar29;
      iVar6 = *piVar1 + 4;
    }
    *piVar1 = iVar6;
  }
  return;
}

