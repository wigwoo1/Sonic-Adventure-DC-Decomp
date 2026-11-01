/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c0348a2) */
/* WARNING: Removing unreachable block (ram,0x8c034624) */
/* WARNING: Removing unreachable block (ram,0x8c03467a) */
/* WARNING: Removing unreachable block (ram,0x8c034702) */
/* WARNING: Removing unreachable block (ram,0x8c0345ce) */

undefined4
FUN_8c0344dc(double param_1,undefined4 param_2,undefined *param_3,int param_4,double *param_5)

{
  double *pdVar1;
  undefined *puVar2;
  undefined *puVar3;
  char *pcVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  uint *puVar9;
  undefined8 *puVar10;
  double in_dr2;
  undefined4 uVar13;
  double dVar11;
  double dVar12;
  float fVar14;
  float fVar15;
  undefined4 uVar16;
  float fVar17;
  float fVar18;
  undefined8 in_dr12;
  double dVar19;
  undefined8 uVar20;
  undefined8 in_dr14;
  double dVar21;
  double in_xd2;
  undefined8 in_xd4;
  double in_xd12;
  double in_xd14;
  bool bVar22;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_38 [16];
  undefined8 uStack_28;
  undefined8 uStack_20;
  
  fVar18 = (float)((ulonglong)in_xd4 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    uStack_20 = (double)CONCAT44((int)in_dr14,(undefined4)uStack_20);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_20._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
    puVar10 = &uStack_28;
    uStack_28 = CONCAT44((int)in_dr12,(int)((ulonglong)in_dr12 >> 0x20));
  }
  else {
    puVar10 = (undefined8 *)auStack_38;
    uStack_20 = in_xd14;
    uStack_28 = in_dr14;
  }
  *(undefined4 *)((int)puVar10 + -4) = in_PR;
  iVar7 = *(int *)(param_4 + 0x38);
  iVar6 = *(int *)(iVar7 + 0x18);
  iVar8 = *(int *)(iVar7 + 0xc);
  if ((((iVar6 == 0) || ((int)(uint)*(ushort *)(iVar6 + 4) < (int)*(short *)(iVar7 + 0x16))) ||
      (puVar9 = (uint *)(*(short *)(iVar7 + 0x16) * 0x34 + *(int *)(iVar6 + 0xc)), iVar8 == 0)) ||
     (puVar9 == (uint *)0x0)) {
    return 0;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar19 = (double)CONCAT44(puVar9[5],*(undefined4 *)(iVar8 + 0x14));
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),puVar9[7]);
    *(uint *)((int)puVar10 + -0x2c) = puVar9[7];
  }
  else {
    dVar19 = *(double *)(puVar9 + 5);
    in_xd12 = *(double *)(iVar8 + 0x14);
    in_xd2 = *(double *)(puVar9 + 7);
    *(double *)((int)puVar10 + -0x2c) = in_xd2;
  }
  iVar6 = *(int *)(*(int *)(iVar6 + 0x10) + 0x20);
  (*(code *)PTR_FUN_8c0346a4)(param_4,(undefined1 *)((int)puVar10 + -0x10));
  pcVar4 = DAT_8c0346ac;
  puVar2 = PTR_FUN_8c0346a8;
  uVar5 = *puVar9 & 0xf;
  dVar21 = 0.0078125;
  if (uVar5 == 0) {
    (*(code *)PTR_FUN_8c0346a4)(iVar6,puVar9,(undefined1 *)((int)puVar10 + -0x1c));
    fVar17 = SUB84(dVar19,0);
    if (in_FPSCR_SZ == '\0') {
      dVar19 = (double)CONCAT44(*(undefined4 *)((int)puVar10 + -0x10),
                                *(undefined4 *)((int)puVar10 + -0x1c));
    }
    else {
      dVar19 = *(double *)((int)puVar10 + -0x10);
    }
    if (in_FPSCR_PR == '\0') {
      dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) - SUB84(dVar19,0),SUB84(dVar19,0)
                               );
    }
    else {
      dVar19 = dVar19 - dVar19;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)param_5 = (int)((ulonglong)dVar19 >> 0x20);
      dVar19 = (double)CONCAT44(*(undefined4 *)((int)puVar10 + -0xc),
                                *(undefined4 *)((int)puVar10 + -0x18));
    }
    else {
      *param_5 = dVar19;
      dVar19 = *(double *)((int)puVar10 + -0xc);
    }
    if (in_FPSCR_PR == '\0') {
      dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) - SUB84(dVar19,0),SUB84(dVar19,0)
                               );
    }
    else {
      dVar19 = dVar19 - dVar19;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)param_5 + 4) = (int)((ulonglong)dVar19 >> 0x20);
      dVar19 = (double)CONCAT44(*(undefined4 *)((int)puVar10 + -8),
                                *(undefined4 *)((int)puVar10 + -0x14));
    }
    else {
      *(double *)((int)param_5 + 4) = dVar19;
      dVar19 = *(double *)((int)puVar10 + -8);
    }
    if (in_FPSCR_PR == '\0') {
      dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) - SUB84(dVar19,0),SUB84(dVar19,0)
                               );
    }
    else {
      dVar19 = dVar19 - dVar19;
    }
    uVar16 = (undefined4)((ulonglong)dVar19 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(param_5 + 1) = uVar16;
      fVar14 = *(float *)((int)param_5 + 4);
      dVar11 = *param_5;
      dVar19 = (double)CONCAT44(uVar16,fVar14);
    }
    else {
      param_5[1] = dVar19;
      dVar11 = *(double *)((int)param_5 + 4);
      fVar18 = (float)((ulonglong)*param_5 >> 0x20);
      fVar14 = (float)((ulonglong)dVar11 >> 0x20);
    }
    if (in_FPSCR_PR == '\0') {
      fVar14 = SUB84(dVar19,0) * fVar14;
    }
    else {
      fVar14 = SUB84(dVar19 * dVar11,0);
    }
    if (in_FPSCR_SZ == '\0') {
      dVar19 = (double)CONCAT44(*(undefined4 *)(param_5 + 1),param_2);
      fVar18 = SUB84(dVar11,0);
    }
    else {
      dVar19 = param_5[1];
    }
    fVar15 = (float)((ulonglong)dVar19 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      fVar14 = fVar15 * fVar15 + fVar18 * SUB84(dVar11,0) + fVar14;
    }
    if (fVar14 == 0.0) {
      if (in_FPSCR_SZ == '\0') {
        *(int *)param_5 = SUB84(dVar21,0);
        *(int *)((int)param_5 + 4) = SUB84(dVar21,0);
        *(int *)(param_5 + 1) = (int)((ulonglong)dVar21 >> 0x20);
      }
      else {
        *param_5 = in_xd14;
        *(double *)((int)param_5 + 4) = in_xd14;
        param_5[1] = dVar21;
      }
      goto LAB_8c0348be;
    }
  }
  else if (uVar5 == 1) {
    (*(code *)PTR_FUN_8c0346a4)(iVar6,puVar9,(undefined1 *)((int)puVar10 + -0x1c));
    fVar15 = SUB84(param_1,0);
    fVar17 = SUB84(dVar19,0);
    if (in_FPSCR_SZ == '\0') {
      dVar19 = (double)CONCAT44(*(undefined4 *)((int)puVar10 + -0x10),
                                *(undefined4 *)((int)puVar10 + -0x1c));
    }
    else {
      dVar19 = *(double *)((int)puVar10 + -0x10);
    }
    if (in_FPSCR_PR == '\0') {
      dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) - SUB84(dVar19,0),SUB84(dVar19,0)
                               );
    }
    else {
      dVar19 = dVar19 - dVar19;
    }
    uVar16 = SUB84(dVar21,0);
    if (in_FPSCR_SZ == '\0') {
      *(int *)param_5 = (int)((ulonglong)dVar19 >> 0x20);
      *(undefined4 *)((int)param_5 + 4) = uVar16;
      dVar11 = (double)CONCAT44(*(undefined4 *)((int)puVar10 + -8),
                                *(undefined4 *)((int)puVar10 + -0x14));
    }
    else {
      *param_5 = dVar19;
      *(double *)((int)param_5 + 4) = in_xd14;
      dVar11 = *(double *)((int)puVar10 + -8);
    }
    if (in_FPSCR_PR == '\0') {
      dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) - SUB84(dVar11,0),SUB84(dVar11,0)
                               );
    }
    else {
      dVar11 = dVar11 - dVar11;
    }
    uVar13 = (undefined4)((ulonglong)dVar11 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(param_5 + 1) = uVar13;
      fVar14 = *(float *)((int)param_5 + 4);
      dVar19 = (double)CONCAT44(fVar14,param_2);
      fVar15 = *(float *)param_5;
      dVar11 = (double)CONCAT44(uVar13,fVar14);
    }
    else {
      param_5[1] = dVar11;
      dVar19 = *(double *)((int)param_5 + 4);
      fVar18 = (float)((ulonglong)*param_5 >> 0x20);
      fVar14 = (float)((ulonglong)dVar19 >> 0x20);
    }
    if (in_FPSCR_PR == '\0') {
      fVar14 = SUB84(dVar11,0) * fVar14;
    }
    else {
      fVar14 = SUB84(dVar11 * dVar19,0);
    }
    if (in_FPSCR_SZ == '\0') {
      dVar11 = (double)CONCAT44(*(undefined4 *)(param_5 + 1),fVar15);
      fVar18 = fVar15;
    }
    else {
      dVar11 = param_5[1];
    }
    fVar15 = (float)((ulonglong)dVar11 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      fVar14 = fVar15 * fVar15 + fVar18 * SUB84(dVar11,0) + fVar14;
    }
    if (fVar14 == 0.0) {
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)param_5 = uVar16;
        *(undefined4 *)((int)param_5 + 4) = uVar16;
        *(int *)(param_5 + 1) = (int)((ulonglong)dVar21 >> 0x20);
      }
      else {
        *param_5 = in_xd14;
        *(double *)((int)param_5 + 4) = in_xd14;
        param_5[1] = dVar21;
      }
      goto LAB_8c0348be;
    }
  }
  else if (uVar5 == 2) {
    (*(code *)PTR_FUN_8c0346b0)(iVar6,puVar9,(undefined1 *)((int)puVar10 + -0x1c));
    fVar17 = SUB84(dVar19,0);
    if (in_FPSCR_SZ == '\0') {
      dVar19 = (double)CONCAT44(*(undefined4 *)((int)puVar10 + -0x10),
                                *(undefined4 *)((int)puVar10 + -0x1c));
    }
    else {
      dVar19 = *(double *)((int)puVar10 + -0x10);
    }
    if (in_FPSCR_PR == '\0') {
      dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) - SUB84(dVar19,0),SUB84(dVar19,0)
                               );
    }
    else {
      dVar19 = dVar19 - dVar19;
    }
    uVar16 = SUB84(dVar21,0);
    if (in_FPSCR_SZ == '\0') {
      *(int *)param_5 = (int)((ulonglong)dVar19 >> 0x20);
      *(undefined4 *)((int)param_5 + 4) = uVar16;
      dVar11 = (double)CONCAT44(*(undefined4 *)((int)puVar10 + -8),
                                *(undefined4 *)((int)puVar10 + -0x14));
    }
    else {
      *param_5 = dVar19;
      *(double *)((int)param_5 + 4) = in_xd14;
      dVar11 = *(double *)((int)puVar10 + -8);
    }
    if (in_FPSCR_PR == '\0') {
      dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) - SUB84(dVar11,0),SUB84(dVar11,0)
                               );
    }
    else {
      dVar11 = dVar11 - dVar11;
    }
    uVar13 = (undefined4)((ulonglong)dVar11 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(param_5 + 1) = uVar13;
      fVar14 = *(float *)((int)param_5 + 4);
      param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),fVar14);
      dVar19 = (double)CONCAT44(*(undefined4 *)param_5,param_2);
      dVar11 = (double)CONCAT44(uVar13,fVar14);
    }
    else {
      param_5[1] = dVar11;
      dVar19 = *param_5;
      fVar14 = SUB84(param_1,0);
    }
    if (in_FPSCR_PR == '\0') {
      fVar14 = SUB84(dVar11,0) * fVar14;
    }
    else {
      fVar14 = SUB84(dVar11 * param_1,0);
    }
    fVar18 = (float)((ulonglong)dVar19 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      fVar15 = *(float *)(param_5 + 1);
      dVar11 = (double)((ulonglong)(uint)fVar18 << 0x20);
    }
    else {
      fVar15 = (float)((ulonglong)param_5[1] >> 0x20);
      dVar11 = dVar19;
    }
    if (in_FPSCR_PR == '\0') {
      fVar14 = fVar15 * fVar15 + (float)((ulonglong)dVar11 >> 0x20) * fVar18 + fVar14;
    }
    if (fVar14 == 0.0) {
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)param_5 = uVar16;
        *(undefined4 *)((int)param_5 + 4) = uVar16;
        *(int *)(param_5 + 1) = (int)((ulonglong)dVar21 >> 0x20);
      }
      else {
        *param_5 = in_xd14;
        *(double *)((int)param_5 + 4) = in_xd14;
        param_5[1] = dVar21;
      }
      goto LAB_8c0348be;
    }
  }
  else if (uVar5 == 3) {
    (*(code *)PTR_FUN_8c034820)(iVar6,puVar9,(undefined1 *)((int)puVar10 + -0x1c));
    fVar17 = SUB84(dVar19,0);
    if (in_FPSCR_SZ == '\0') {
      dVar19 = (double)CONCAT44(*(undefined4 *)((int)puVar10 + -0x10),
                                *(undefined4 *)((int)puVar10 + -0x1c));
    }
    else {
      dVar19 = *(double *)((int)puVar10 + -0x10);
    }
    if (in_FPSCR_PR == '\0') {
      dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) - SUB84(dVar19,0),SUB84(dVar19,0)
                               );
    }
    else {
      dVar19 = dVar19 - dVar19;
    }
    uVar16 = SUB84(dVar21,0);
    if (in_FPSCR_SZ == '\0') {
      *(int *)param_5 = (int)((ulonglong)dVar19 >> 0x20);
      *(undefined4 *)((int)param_5 + 4) = uVar16;
      dVar11 = (double)CONCAT44(*(undefined4 *)((int)puVar10 + -8),
                                *(undefined4 *)((int)puVar10 + -0x14));
    }
    else {
      *param_5 = dVar19;
      *(double *)((int)param_5 + 4) = in_xd14;
      dVar11 = *(double *)((int)puVar10 + -8);
    }
    if (in_FPSCR_PR == '\0') {
      dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) - SUB84(dVar11,0),SUB84(dVar11,0)
                               );
    }
    else {
      dVar11 = dVar11 - dVar11;
    }
    uVar13 = (undefined4)((ulonglong)dVar11 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(param_5 + 1) = uVar13;
      fVar14 = *(float *)((int)param_5 + 4);
      param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),fVar14);
      dVar19 = (double)CONCAT44(*(undefined4 *)param_5,param_2);
      dVar11 = (double)CONCAT44(uVar13,fVar14);
    }
    else {
      param_5[1] = dVar11;
      dVar19 = *param_5;
      fVar14 = SUB84(param_1,0);
    }
    if (in_FPSCR_PR == '\0') {
      fVar14 = SUB84(dVar11,0) * fVar14;
    }
    else {
      fVar14 = SUB84(dVar11 * param_1,0);
    }
    fVar18 = (float)((ulonglong)dVar19 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      fVar15 = *(float *)(param_5 + 1);
      dVar11 = (double)((ulonglong)(uint)fVar18 << 0x20);
    }
    else {
      fVar15 = (float)((ulonglong)param_5[1] >> 0x20);
      dVar11 = dVar19;
    }
    if (in_FPSCR_PR == '\0') {
      fVar14 = fVar15 * fVar15 + (float)((ulonglong)dVar11 >> 0x20) * fVar18 + fVar14;
    }
    if (fVar14 == 0.0) {
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)param_5 = uVar16;
        *(undefined4 *)((int)param_5 + 4) = uVar16;
        *(int *)(param_5 + 1) = (int)((ulonglong)dVar21 >> 0x20);
      }
      else {
        *param_5 = in_xd14;
        *(double *)((int)param_5 + 4) = in_xd14;
        param_5[1] = dVar21;
      }
      goto LAB_8c0348be;
    }
  }
  else {
    if (uVar5 == 4) {
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)((int)puVar10 + -0x10) = *(undefined4 *)(param_4 + 0x20);
        *(undefined4 *)((int)puVar10 + -0xc) = *(undefined4 *)(param_4 + 0x24);
        in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(param_4 + 0x28));
        *(undefined4 *)((int)puVar10 + -8) = *(undefined4 *)(param_4 + 0x28);
      }
      else {
        *(undefined8 *)((int)puVar10 + -0x10) = *(undefined8 *)(param_4 + 0x20);
        *(undefined8 *)((int)puVar10 + -0xc) = *(undefined8 *)(param_4 + 0x24);
        in_xd2 = *(double *)(param_4 + 0x28);
        *(double *)((int)puVar10 + -8) = in_xd2;
      }
      (*(code *)PTR_FUN_8c034824)(iVar6,puVar9,(undefined1 *)((int)puVar10 + -0x1c));
      uVar16 = SUB84(param_1,0);
      if (((*puVar9 & 0x20) == 0) && ((*puVar9 & 0x2000) == 0)) {
        iVar6 = -puVar9[9] - *(int *)(iVar6 + 0x18);
      }
      else {
        iVar6 = -puVar9[9];
      }
      (*(code *)PTR_FUN_8c034828)
                ((undefined1 *)((int)puVar10 + -0x10),(undefined1 *)((int)puVar10 + -0x1c),iVar6,
                 (undefined1 *)((int)puVar10 + -0x28));
      iVar8 = DAT_8c034844;
      iVar7 = DAT_8c034838;
      dVar11 = in_xd12;
      if (in_FPSCR_SZ == '\0') {
        in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),SUB84(dVar19,0));
        dVar11 = in_xd2;
      }
      if (in_FPSCR_PR == '\0') {
        fVar18 = SUB84(in_dr2,0) + (float)((ulonglong)dVar19 >> 0x20);
      }
      else {
        fVar18 = SUB84(in_dr2 + dVar19,0);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar12 = (double)CONCAT44(*(undefined4 *)((int)puVar10 + -0x20),fVar18);
        dVar11 = (double)CONCAT44(fVar18,uVar16);
      }
      else {
        dVar12 = *(double *)((int)puVar10 + -0x20);
      }
      if (in_FPSCR_PR == '\0') {
        dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) *
                                  (float)((ulonglong)dVar12 >> 0x20),SUB84(dVar11,0));
      }
      else {
        dVar11 = dVar11 * dVar12;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar1 = (double *)((int)puVar10 + -0x2c);
      }
      else {
        pdVar1 = (double *)((int)puVar10 + -0x28);
      }
      dVar12 = *pdVar1;
      if (in_FPSCR_PR == '\0') {
        fVar17 = SUB84(dVar19,0) + SUB84(dVar12,0);
      }
      else {
        fVar17 = SUB84(dVar19 + dVar12,0);
      }
      dVar19 = in_xd12;
      if (in_FPSCR_SZ == '\0') {
        uVar16 = (undefined4)((ulonglong)dVar11 >> 0x20);
        dVar11 = (double)CONCAT44(uVar16,uVar16);
        dVar19 = (double)CONCAT44(fVar17,param_2);
      }
      if (in_FPSCR_PR == '\0') {
        dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) *
                                  (float)((ulonglong)dVar12 >> 0x20),SUB84(dVar19,0));
      }
      else {
        dVar19 = dVar19 * dVar12;
      }
      dVar12 = (double)((ulonglong)dVar12 & 0xffffffff);
      fVar18 = (float)((ulonglong)dVar19 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar14 = SUB84(dVar11,0) - fVar18;
        bVar22 = 0.0 < fVar14;
        dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) + fVar18,fVar14);
      }
      else {
        bVar22 = dVar12 < dVar11 - dVar19;
        dVar11 = (dVar11 - dVar19) + dVar19;
      }
      fVar18 = (float)((ulonglong)dVar11 >> 0x20);
      if (bVar22) {
        if (in_FPSCR_PR == '\0') {
          bVar22 = 0.0 < fVar18;
        }
        else {
          bVar22 = dVar12 < dVar11;
        }
        if (bVar22) {
          iVar8 = 0;
          if (-1 < *pcVar4) {
            (*(code *)PTR_FUN_8c03482c)(DAT_8c034830,PTR_s_side1_8c034834);
          }
        }
        else {
          iVar8 = iVar7;
          if (-1 < *pcVar4) {
            (*(code *)PTR_FUN_8c03482c)(DAT_8c034830,PTR_s_side2_8c03483c);
          }
        }
      }
      else {
        if (in_FPSCR_PR == '\0') {
          bVar22 = 0.0 < fVar18;
        }
        else {
          bVar22 = dVar12 < dVar11;
        }
        if (bVar22) {
          iVar8 = 0x4000;
          if (-1 < *pcVar4) {
            (*(code *)PTR_FUN_8c03482c)(DAT_8c034830,PTR_s_side3_8c034840);
          }
        }
        else if (-1 < *pcVar4) {
          (*(code *)PTR_FUN_8c03482c)(DAT_8c034830,PTR_s_side4_8c034848);
        }
      }
      fVar14 = SUB84(dVar12,0);
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)puVar10 + -0x28) = SUB84(dVar21,0);
        *(int *)((int)puVar10 + -0x24) = SUB84(dVar21,0);
        *(int *)((int)puVar10 + -0x20) = (int)((ulonglong)dVar21 >> 0x20);
      }
      else {
        *(double *)((int)puVar10 + -0x28) = in_xd14;
        *(double *)((int)puVar10 + -0x24) = in_xd14;
        *(double *)((int)puVar10 + -0x20) = dVar21;
      }
      (*(code *)PTR_FUN_8c034850)(PTR_DAT_8c03484c);
      *(int *)((int)puVar10 + -0x30) = iVar8 - iVar6;
      if (iVar8 - iVar6 != 0) {
        (*(code *)PTR_FUN_8c034854)(0,*(undefined4 *)((int)puVar10 + -0x30));
      }
      (*(code *)PTR_FUN_8c034858)(0,(undefined1 *)((int)puVar10 + -0x28),param_5);
      (*(code *)PTR_FUN_8c03485c)(1);
      goto LAB_8c0348be;
    }
    if (uVar5 != 5) {
      return 0;
    }
    (*(code *)PTR_FUN_8c034920)(iVar6,puVar9,(undefined1 *)((int)puVar10 + -0x1c));
    fVar17 = SUB84(dVar19,0);
    if (in_FPSCR_SZ == '\0') {
      dVar19 = (double)CONCAT44(*(undefined4 *)((int)puVar10 + -0x10),
                                *(undefined4 *)((int)puVar10 + -0x1c));
    }
    else {
      dVar19 = *(double *)((int)puVar10 + -0x10);
    }
    if (in_FPSCR_PR == '\0') {
      dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) - SUB84(dVar19,0),SUB84(dVar19,0)
                               );
    }
    else {
      dVar19 = dVar19 - dVar19;
    }
    uVar16 = SUB84(dVar21,0);
    if (in_FPSCR_SZ == '\0') {
      *(int *)param_5 = (int)((ulonglong)dVar19 >> 0x20);
      *(undefined4 *)((int)param_5 + 4) = uVar16;
      dVar19 = (double)CONCAT44(*(undefined4 *)((int)puVar10 + -8),
                                *(undefined4 *)((int)puVar10 + -0x14));
    }
    else {
      *param_5 = dVar19;
      *(double *)((int)param_5 + 4) = in_xd14;
      dVar19 = *(double *)((int)puVar10 + -8);
    }
    if (in_FPSCR_PR == '\0') {
      dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) - SUB84(dVar19,0),SUB84(dVar19,0)
                               );
    }
    else {
      dVar19 = dVar19 - dVar19;
    }
    uVar13 = (undefined4)((ulonglong)dVar19 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(param_5 + 1) = uVar13;
      fVar14 = *(float *)((int)param_5 + 4);
      dVar11 = (double)CONCAT44(*(undefined4 *)param_5,fVar14);
      dVar19 = (double)CONCAT44(uVar13,fVar14);
    }
    else {
      param_5[1] = dVar19;
      dVar11 = *param_5;
      fVar14 = SUB84(dVar11,0);
    }
    if (in_FPSCR_PR == '\0') {
      fVar14 = SUB84(dVar19,0) * fVar14;
    }
    else {
      fVar14 = SUB84(dVar19 * dVar11,0);
    }
    fVar18 = (float)((ulonglong)dVar11 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      dVar11 = (double)((ulonglong)(uint)fVar18 << 0x20);
      dVar19 = (double)CONCAT44(*(undefined4 *)(param_5 + 1),param_2);
    }
    else {
      dVar19 = param_5[1];
    }
    fVar15 = (float)((ulonglong)dVar19 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      fVar14 = fVar15 * fVar15 + (float)((ulonglong)dVar11 >> 0x20) * fVar18 + fVar14;
    }
    if (fVar14 == 0.0) {
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)param_5 = uVar16;
        *(undefined4 *)((int)param_5 + 4) = uVar16;
        *(int *)(param_5 + 1) = (int)((ulonglong)dVar21 >> 0x20);
      }
      else {
        *param_5 = in_xd14;
        *(double *)((int)param_5 + 4) = in_xd14;
        param_5[1] = dVar21;
      }
      goto LAB_8c0348be;
    }
  }
  (*(code *)puVar2)(param_5);
LAB_8c0348be:
  fVar18 = SUB84(dVar19,0);
  if (-1 < *pcVar4) {
    if (in_FPSCR_SZ == '\0') {
      param_3 = *(undefined **)((int)puVar10 + -0x14);
      fVar14 = *(float *)(param_5 + 1);
      uVar20 = CONCAT44(*(undefined4 *)((int)puVar10 + -0x18),fVar17);
      puVar2 = param_3;
      puVar3 = DAT_8c034924;
    }
    else {
      in_xd14 = *(double *)((int)puVar10 + -0x14);
      uVar20 = *(undefined8 *)((int)puVar10 + -0x18);
      puVar2 = DAT_8c034924;
      puVar3 = PTR_FUN_8c034928;
    }
    if (in_FPSCR_PR == '\0') {
      param_3 = (undefined *)((float)puVar3 * fVar14 + (float)param_3);
    }
    if (in_FPSCR_SZ == '\0') {
      fVar18 = *(float *)((int)puVar10 + -0x1c);
      uVar20 = CONCAT44((int)((ulonglong)uVar20 >> 0x20),fVar18);
      fVar14 = *(float *)param_5;
      fVar17 = fVar18;
    }
    else {
      in_xd12 = *(double *)((int)puVar10 + -0x1c);
      fVar17 = (float)uVar20;
    }
    if (in_FPSCR_PR == '\0') {
      fVar18 = (float)puVar3 * fVar14 + fVar18;
    }
    uVar16 = (undefined4)((ulonglong)uVar20 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      uVar20 = CONCAT44(uVar16,param_3);
      in_xd14 = (double)CONCAT44(puVar2,fVar18);
      in_xd12 = (double)CONCAT44(fVar17,uVar16);
    }
    (*(code *)PTR_FUN_8c034928)(in_xd12,in_xd14,uVar20);
  }
  return 1;
}

