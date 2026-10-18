/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c038280) */

ulonglong FUN_8c037ff4(double param_1,double param_2,double param_3,double param_4)

{
  undefined8 uVar1;
  undefined4 uVar2;
  double *pdVar3;
  double *pdVar4;
  undefined8 *puVar5;
  undefined8 *puVar6;
  double *pdVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  undefined4 unaff_r12;
  uint uVar11;
  uint uVar12;
  uint uVar13;
  uint uVar14;
  undefined8 *unaff_r14;
  float fVar15;
  double in_dr0;
  double in_dr2;
  double dVar16;
  double dVar17;
  undefined8 uVar18;
  float fVar20;
  double dVar19;
  float fVar21;
  float fVar22;
  float fVar23;
  float fVar24;
  float in_fr13;
  float in_fr12;
  float in_fr15;
  float in_fr14;
  double in_xd0;
  undefined8 in_xd2;
  undefined8 in_xd4;
  undefined8 in_xd6;
  undefined8 in_xd8;
  byte in_MD;
  byte in_RB;
  byte in_BL;
  byte in_FD;
  byte in_M;
  byte in_Q;
  byte in_IMASK;
  byte in_S;
  bool bVar25;
  int iVar26;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  iVar10 = 3;
  iVar9 = 0;
  pdVar4 = DAT_8c03801c;
  pdVar7 = DAT_8c038020;
  iVar8 = DAT_8c038024;
  while( true ) {
    pdVar3 = pdVar4;
    fVar15 = SUB84(in_dr0,0);
    if (in_FPSCR_SZ == '\0') {
      fVar15 = *(float *)pdVar7;
    }
    else {
      in_xd0 = *pdVar7;
    }
    dVar19 = (double)CONCAT44(0x3f800000,fVar15);
    if (in_FPSCR_SZ == '\0') {
      dVar16 = (double)CONCAT44(*(float *)pdVar3,SUB84(in_dr2,0));
    }
    else {
      dVar16 = *pdVar3;
    }
    fVar20 = (float)((ulonglong)dVar16 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      bVar25 = fVar20 < 1.0;
    }
    else {
      bVar25 = dVar16 < dVar19;
    }
    if ((bool)((in_FPSCR_PR == '\0' && fVar15 < 1.0) ^ bVar25)) {
      if (in_FPSCR_PR == '\0') {
        fVar20 = fVar20 - fVar15;
        dVar16 = (double)CONCAT44(fVar20,SUB84(dVar16,0));
        dVar19 = (double)CONCAT44((1.0 - fVar15) / fVar20,fVar15);
      }
      else {
        dVar16 = dVar16 - (dVar19 - dVar19);
        dVar19 = (dVar19 - dVar19) / dVar16;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),*(float *)((int)pdVar7 + 4));
        pdVar7 = pdVar7 + 1;
        fVar15 = *(float *)((int)pdVar3 + 4);
        pdVar3 = pdVar3 + 1;
        param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),fVar15);
        uVar18 = CONCAT44(fVar15,SUB84(dVar16,0));
      }
      else {
        in_xd0 = *(double *)((int)pdVar7 + 4);
        pdVar7 = (double *)((int)pdVar7 + 0xc);
        in_xd4 = *(undefined8 *)((int)pdVar3 + 4);
        pdVar3 = (double *)((int)pdVar3 + 0xc);
        uVar18 = in_xd4;
      }
      fVar15 = (float)((ulonglong)dVar19 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar20 = (SUB84(param_1,0) - SUB84(dVar19,0)) * fVar15 + SUB84(dVar19,0);
      }
      else {
        fVar20 = SUB84((param_1 - dVar19) * dVar19 + dVar19,0);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar19 = (double)CONCAT44(fVar15,*(float *)pdVar7);
        pdVar7 = (double *)((int)pdVar7 + 4);
        pdVar4 = (double *)((int)pdVar3 + 4);
        dVar17 = (double)CONCAT44(*(float *)pdVar3,fVar20);
        dVar16 = (double)CONCAT44(*(float *)pdVar3,(int)uVar18);
      }
      else {
        in_xd0 = *pdVar7;
        pdVar7 = pdVar7 + 1;
        dVar17 = *pdVar3;
        pdVar4 = pdVar3 + 1;
        dVar16 = dVar17;
      }
      fVar15 = (float)((ulonglong)dVar19 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        param_1 = (double)CONCAT44((((float)((ulonglong)dVar17 >> 0x20) - SUB84(dVar19,0)) * fVar15
                                   + SUB84(dVar19,0)) * in_fr14 + in_fr12,
                                   SUB84(dVar17,0) * in_fr15 + in_fr13);
      }
      else {
        param_1 = (((dVar17 - dVar19) * dVar19 + dVar19) * (double)CONCAT44(in_fr14,in_fr15) +
                  (double)CONCAT44(in_fr12,in_fr13)) * (double)CONCAT44(in_fr14,in_fr15) +
                  (double)CONCAT44(in_fr12,in_fr13);
      }
      fVar20 = SUB84(param_3,0);
      if (in_FPSCR_SZ == '\0') {
        fVar20 = *(float *)PTR_DAT_8c0381c0;
      }
      else {
        in_xd8 = *(undefined8 *)PTR_DAT_8c0381c0;
      }
      if (in_FPSCR_PR == '\0') {
        dVar17 = (double)CONCAT44(unaff_r14,fVar20 * fVar15);
        iVar26 = (int)(fVar20 * fVar15);
      }
      else {
        dVar17 = (double)CONCAT44(unaff_r14,fVar20) * dVar19;
        iVar26 = (int)dVar17;
      }
      fVar20 = *(float *)((int)pdVar7 + 0xc);
      fVar22 = *(float *)((int)pdVar4 + 0xc);
      uVar11 = (uint)fVar20 >> 8 & 0xff;
      uVar12 = (uint)fVar20 >> 0x10 & 0xff;
      unaff_r14 = (undefined8 *)((ulonglong)dVar17 >> 0x20);
      fVar21 = *(float *)(pdVar7 + 2);
      fVar23 = *(float *)(pdVar4 + 2);
      uVar13 = (uint)fVar21 >> 8 & 0xff;
      uVar14 = (uint)fVar21 >> 0x10 & 0xff;
      uVar13 = ((uint)fVar21 & 0xff) +
               (int)(short)(iVar26 * (((uint)fVar23 & 0xff) - ((uint)fVar21 & 0xff)) >> 0x10) |
               (uVar13 + (int)(short)(iVar26 * (((uint)fVar23 >> 8 & 0xff) - uVar13) >> 0x10)) *
               0x100 | (uVar14 + (int)(short)(iVar26 * (((uint)fVar23 >> 0x10 & 0xff) - uVar14) >>
                                             0x10)) * 0x10000 |
               (((uint)fVar21 >> 0x18) +
               (int)(short)(iVar26 * (((uint)fVar23 >> 0x18) - ((uint)fVar21 >> 0x18)) >> 0x10)) *
               0x1000000;
      param_4 = (double)CONCAT44(uVar13,SUB84(param_4,0));
      if (in_FPSCR_SZ == '\0') {
        dVar19 = (double)CONCAT44(fVar15,*(float *)((int)pdVar7 + 0x14));
        pdVar7 = pdVar7 + 3;
        fVar15 = *(float *)((int)pdVar4 + 0x14);
        pdVar4 = pdVar4 + 3;
        param_2 = (double)CONCAT44((int)((ulonglong)param_2 >> 0x20),fVar15);
        uVar18 = CONCAT44(fVar15,SUB84(dVar16,0));
      }
      else {
        in_xd0 = *(double *)((int)pdVar7 + 0x14);
        pdVar7 = (double *)((int)pdVar7 + 0x1c);
        in_xd6 = *(undefined8 *)((int)pdVar4 + 0x14);
        pdVar4 = (double *)((int)pdVar4 + 0x1c);
        uVar18 = in_xd6;
      }
      fVar15 = (float)((ulonglong)dVar19 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar21 = (SUB84(param_2,0) - SUB84(dVar19,0)) * fVar15 + SUB84(dVar19,0);
      }
      else {
        fVar21 = SUB84((param_2 - dVar19) * dVar19 + dVar19,0);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar19 = (double)CONCAT44(fVar15,*(float *)pdVar7);
        pdVar3 = (double *)((int)pdVar4 + 4);
        dVar17 = (double)CONCAT44(*(float *)pdVar4,
                                  ((uint)fVar20 & 0xff) +
                                  (int)(short)(iVar26 * (((uint)fVar22 & 0xff) -
                                                        ((uint)fVar20 & 0xff)) >> 0x10) |
                                  (uVar11 + (int)(short)(iVar26 * (((uint)fVar22 >> 8 & 0xff) -
                                                                  uVar11) >> 0x10)) * 0x100 |
                                  (uVar12 + (int)(short)(iVar26 * (((uint)fVar22 >> 0x10 & 0xff) -
                                                                  uVar12) >> 0x10)) * 0x10000 |
                                  (((uint)fVar20 >> 0x18) +
                                  (int)(short)(iVar26 * (((uint)fVar22 >> 0x18) -
                                                        ((uint)fVar20 >> 0x18)) >> 0x10)) *
                                  0x1000000);
        dVar16 = (double)CONCAT44(*(float *)pdVar4,(int)uVar18);
      }
      else {
        in_xd0 = *pdVar7;
        dVar17 = *pdVar4;
        pdVar3 = pdVar4 + 1;
        dVar16 = dVar17;
      }
      if (in_FPSCR_PR == '\0') {
        param_3 = (double)CONCAT44(((float)((ulonglong)dVar17 >> 0x20) - SUB84(dVar19,0)) *
                                   (float)((ulonglong)dVar19 >> 0x20) + SUB84(dVar19,0),
                                   SUB84(dVar17,0));
      }
      else {
        param_3 = (dVar17 - dVar19) * dVar19 + dVar19;
      }
      pdVar3 = pdVar3 + -5;
      if (in_FPSCR_SZ == '\0') {
        *(int *)(iVar8 + 0x24) = (int)((ulonglong)param_3 >> 0x20);
        *(float *)(iVar8 + 0x20) = fVar21;
        *(uint *)(iVar8 + 0x1c) = uVar13;
        *(int *)(iVar8 + 0x18) = SUB84(param_3,0);
        *(int *)(iVar8 + 0x14) = (int)((ulonglong)param_1 >> 0x20);
        puVar5 = (undefined8 *)(iVar8 + 0x10);
        *(int *)puVar5 = SUB84(param_1,0);
      }
      else {
        *(double *)(iVar8 + 0x24) = param_3;
        *(undefined8 *)(iVar8 + 0x1c) = in_xd6;
        *(double *)(iVar8 + 0x14) = param_4;
        *(undefined8 *)(iVar8 + 0xc) = in_xd8;
        *(double *)(iVar8 + 4) = param_1;
        puVar5 = (undefined8 *)(iVar8 + -4);
        *puVar5 = in_xd4;
      }
      param_2 = (double)CONCAT44(0x3f800000,fVar21);
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)((int)puVar5 + -4) = 0x3f800000;
      }
      else {
        puVar5[-1] = param_2;
      }
      iVar8 = iVar8 + 0x28;
      iVar9 = iVar9 + 1;
    }
    in_dr0 = (double)CONCAT44(0x3f800000,SUB84(dVar19,0));
    if (in_FPSCR_SZ == '\0') {
      in_dr2 = (double)CONCAT44(*(float *)pdVar3,SUB84(dVar16,0));
    }
    else {
      in_dr2 = *pdVar3;
    }
    if (in_FPSCR_PR == '\0') {
      bVar25 = 1.0 < (float)((ulonglong)in_dr2 >> 0x20);
    }
    else {
      bVar25 = in_dr0 < in_dr2;
    }
    if (bVar25) {
      if (in_FPSCR_SZ == '\0') {
        fVar22 = *(float *)((int)pdVar3 + 0xc);
        fVar15 = *(float *)(pdVar3 + 2);
        fVar20 = *(float *)((int)pdVar3 + 0x14);
        param_1 = pdVar3[2];
        fVar23 = *(float *)(pdVar3 + 3);
        fVar24 = *(float *)((int)pdVar3 + 0x1c);
        param_4 = (double)CONCAT44(fVar24,SUB84(param_4,0));
        fVar21 = *(float *)(pdVar3 + 4);
        param_2 = (double)CONCAT44(fVar22,fVar21);
        param_3 = (double)CONCAT44(*(float *)((int)pdVar3 + 0x24),fVar23);
        *(float *)(iVar8 + 0x24) = *(float *)((int)pdVar3 + 0x24);
        *(float *)(iVar8 + 0x20) = fVar21;
        *(float *)(iVar8 + 0x1c) = fVar24;
        *(float *)(iVar8 + 0x18) = fVar23;
        *(float *)(iVar8 + 0x14) = fVar20;
        *(float *)(iVar8 + 0x10) = fVar15;
        *(float *)(iVar8 + 0xc) = fVar22;
      }
      else {
        param_2 = *(double *)((int)pdVar3 + 0xc);
        in_xd4 = *(undefined8 *)((int)pdVar3 + 0x14);
        param_1 = *(double *)((int)pdVar3 + 0x1c);
        in_xd8 = *(undefined8 *)((int)pdVar3 + 0x24);
        param_4 = *(double *)((int)pdVar3 + 0x2c);
        in_xd6 = *(undefined8 *)((int)pdVar3 + 0x34);
        param_3 = *(double *)((int)pdVar3 + 0x3c);
        *(double *)(iVar8 + 0x24) = param_3;
        *(undefined8 *)(iVar8 + 0x1c) = in_xd6;
        *(double *)(iVar8 + 0x14) = param_4;
        *(undefined8 *)(iVar8 + 0xc) = in_xd8;
        *(double *)(iVar8 + 4) = param_1;
        *(undefined8 *)(iVar8 + -4) = in_xd4;
        *(double *)(iVar8 + -0xc) = param_2;
      }
      iVar8 = iVar8 + 0x28;
      iVar9 = iVar9 + 1;
    }
    uVar2 = DAT_8c038294;
    iVar10 = iVar10 + -1;
    if (iVar10 == 0) break;
    pdVar4 = pdVar3 + 5;
    pdVar7 = pdVar3;
  }
  if (in_FPSCR_SZ == '\0') {
    puVar5 = (undefined8 *)((int)DAT_8c038298 + 4);
    uVar18 = CONCAT44(*(undefined4 *)DAT_8c038298,*(undefined4 *)((int)DAT_8c038298 + 0xc));
    dVar19 = (double)CONCAT44(*(undefined4 *)(DAT_8c038298 + 2),
                              *(undefined4 *)((int)DAT_8c038298 + 0x14));
    dVar16 = (double)CONCAT44(*(undefined4 *)(DAT_8c038298 + 3),SUB84(param_2,0));
  }
  else {
    uVar18 = *DAT_8c038298;
    in_xd0 = (double)DAT_8c038298[1];
    puVar5 = DAT_8c038298 + 2;
    in_xd2 = DAT_8c038298[3];
    dVar19 = (double)DAT_8c038298[4];
    in_xd4 = DAT_8c038298[5];
    dVar16 = (double)DAT_8c038298[6];
  }
  uVar1 = *puVar5;
  fVar15 = SUB84(param_4,0);
  if (in_FPSCR_PR == '\0') {
    dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),SUB84(dVar19,0) * fVar15);
    dVar16 = (double)CONCAT44((float)((ulonglong)dVar16 >> 0x20) * fVar15,SUB84(dVar16,0));
  }
  else {
    dVar19 = dVar19 * param_4;
    dVar16 = dVar16 * param_4;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)unaff_r14 + 0x1c) = (int)((ulonglong)dVar19 >> 0x20);
    *(int *)(unaff_r14 + 3) = (int)uVar18;
    *(int *)((int)unaff_r14 + 0x14) = (int)((ulonglong)dVar16 >> 0x20);
    *(int *)(unaff_r14 + 2) = SUB84(dVar19,0);
    *(int *)((int)unaff_r14 + 0xc) = (int)((ulonglong)uVar18 >> 0x20);
    *(int *)(unaff_r14 + 1) = (int)uVar1;
    puVar5 = (undefined8 *)((int)unaff_r14 + 4);
    *(int *)puVar5 = (int)((ulonglong)uVar1 >> 0x20);
  }
  else {
    unaff_r14[3] = dVar19;
    unaff_r14[2] = in_xd2;
    unaff_r14[1] = dVar16;
    *unaff_r14 = in_xd4;
    unaff_r14[-1] = uVar18;
    unaff_r14[-2] = in_xd0;
    puVar5 = unaff_r14 + -3;
    *puVar5 = uVar1;
  }
  *(undefined4 *)((int)puVar5 + -4) = uVar2;
  if (in_FPSCR_SZ == '\0') {
    puVar6 = (undefined8 *)((int)DAT_8c03829c + 4);
    uVar18 = CONCAT44(*(undefined4 *)DAT_8c03829c,*(undefined4 *)((int)DAT_8c03829c + 0xc));
    dVar19 = (double)CONCAT44(*(undefined4 *)(DAT_8c03829c + 2),
                              *(undefined4 *)((int)DAT_8c03829c + 0x14));
    dVar16 = (double)CONCAT44(*(undefined4 *)(DAT_8c03829c + 3),SUB84(dVar16,0));
  }
  else {
    uVar18 = *DAT_8c03829c;
    in_xd0 = (double)DAT_8c03829c[1];
    puVar6 = DAT_8c03829c + 2;
    in_xd2 = DAT_8c03829c[3];
    dVar19 = (double)DAT_8c03829c[4];
    in_xd4 = DAT_8c03829c[5];
    dVar16 = (double)DAT_8c03829c[6];
  }
  uVar1 = *puVar6;
  if (in_FPSCR_PR == '\0') {
    dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),SUB84(dVar19,0) * fVar15);
    dVar16 = (double)CONCAT44((float)((ulonglong)dVar16 >> 0x20) * fVar15,SUB84(dVar16,0));
  }
  else {
    dVar19 = dVar19 * param_4;
    dVar16 = dVar16 * param_4;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(puVar5 + 7) = (int)((ulonglong)dVar19 >> 0x20);
    *(int *)((int)puVar5 + 0x34) = (int)uVar18;
    *(int *)(puVar5 + 6) = (int)((ulonglong)dVar16 >> 0x20);
    *(int *)((int)puVar5 + 0x2c) = SUB84(dVar19,0);
    *(int *)(puVar5 + 5) = (int)((ulonglong)uVar18 >> 0x20);
    *(int *)((int)puVar5 + 0x24) = (int)uVar1;
    puVar5 = puVar5 + 4;
    *(int *)puVar5 = (int)((ulonglong)uVar1 >> 0x20);
  }
  else {
    *(double *)((int)puVar5 + 0x34) = dVar19;
    *(undefined8 *)((int)puVar5 + 0x2c) = in_xd2;
    *(double *)((int)puVar5 + 0x24) = dVar16;
    *(undefined8 *)((int)puVar5 + 0x1c) = in_xd4;
    *(undefined8 *)((int)puVar5 + 0x14) = uVar18;
    *(double *)((int)puVar5 + 0xc) = in_xd0;
    puVar5 = (undefined8 *)((int)puVar5 + 4);
    *puVar5 = uVar1;
  }
  *(undefined4 *)((int)puVar5 + -4) = uVar2;
  if (iVar9 != 3) {
    if (in_FPSCR_SZ == '\0') {
      puVar6 = (undefined8 *)((int)DAT_8c0382a0 + 4);
      uVar18 = CONCAT44(*(undefined4 *)DAT_8c0382a0,*(undefined4 *)((int)DAT_8c0382a0 + 0xc));
      dVar19 = (double)CONCAT44(*(undefined4 *)(DAT_8c0382a0 + 2),
                                *(undefined4 *)((int)DAT_8c0382a0 + 0x14));
      dVar16 = (double)CONCAT44(*(undefined4 *)(DAT_8c0382a0 + 3),SUB84(dVar16,0));
    }
    else {
      uVar18 = *DAT_8c0382a0;
      in_xd0 = (double)DAT_8c0382a0[1];
      puVar6 = DAT_8c0382a0 + 2;
      in_xd2 = DAT_8c0382a0[3];
      dVar19 = (double)DAT_8c0382a0[4];
      in_xd4 = DAT_8c0382a0[5];
      dVar16 = (double)DAT_8c0382a0[6];
    }
    uVar1 = *puVar6;
    if (in_FPSCR_PR == '\0') {
      dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),SUB84(dVar19,0) * fVar15);
      dVar16 = (double)CONCAT44((float)((ulonglong)dVar16 >> 0x20) * fVar15,SUB84(dVar16,0));
    }
    else {
      dVar19 = dVar19 * param_4;
      dVar16 = dVar16 * param_4;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(puVar5 + 7) = (int)((ulonglong)dVar19 >> 0x20);
      *(int *)((int)puVar5 + 0x34) = (int)uVar18;
      *(int *)(puVar5 + 6) = (int)((ulonglong)dVar16 >> 0x20);
      *(int *)((int)puVar5 + 0x2c) = SUB84(dVar19,0);
      *(int *)(puVar5 + 5) = (int)((ulonglong)uVar18 >> 0x20);
      *(int *)((int)puVar5 + 0x24) = (int)uVar1;
      puVar5 = puVar5 + 4;
      *(int *)puVar5 = (int)((ulonglong)uVar1 >> 0x20);
    }
    else {
      *(double *)((int)puVar5 + 0x34) = dVar19;
      *(undefined8 *)((int)puVar5 + 0x2c) = in_xd2;
      *(double *)((int)puVar5 + 0x24) = dVar16;
      *(undefined8 *)((int)puVar5 + 0x1c) = in_xd4;
      *(undefined8 *)((int)puVar5 + 0x14) = uVar18;
      *(double *)((int)puVar5 + 0xc) = in_xd0;
      puVar5 = (undefined8 *)((int)puVar5 + 4);
      *puVar5 = uVar1;
    }
    *(undefined4 *)((int)puVar5 + -4) = uVar2;
  }
  uVar2 = DAT_8c0382a4;
  if (in_FPSCR_SZ == '\0') {
    puVar6 = (undefined8 *)((int)DAT_8c0382a8 + 4);
    uVar18 = CONCAT44(*(undefined4 *)DAT_8c0382a8,*(undefined4 *)((int)DAT_8c0382a8 + 0xc));
    dVar19 = (double)CONCAT44(*(undefined4 *)(DAT_8c0382a8 + 2),
                              *(undefined4 *)((int)DAT_8c0382a8 + 0x14));
    dVar16 = (double)CONCAT44(*(undefined4 *)(DAT_8c0382a8 + 3),SUB84(dVar16,0));
  }
  else {
    uVar18 = *DAT_8c0382a8;
    in_xd0 = (double)DAT_8c0382a8[1];
    puVar6 = DAT_8c0382a8 + 2;
    in_xd2 = DAT_8c0382a8[3];
    dVar19 = (double)DAT_8c0382a8[4];
    in_xd4 = DAT_8c0382a8[5];
    dVar16 = (double)DAT_8c0382a8[6];
  }
  uVar1 = *puVar6;
  if (in_FPSCR_PR == '\0') {
    dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),SUB84(dVar19,0) * fVar15);
    dVar16 = (double)CONCAT44((float)((ulonglong)dVar16 >> 0x20) * fVar15,SUB84(dVar16,0));
  }
  else {
    dVar19 = dVar19 * param_4;
    dVar16 = dVar16 * param_4;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(puVar5 + 7) = (int)((ulonglong)dVar19 >> 0x20);
    *(int *)((int)puVar5 + 0x34) = (int)uVar18;
    *(int *)(puVar5 + 6) = (int)((ulonglong)dVar16 >> 0x20);
    *(int *)((int)puVar5 + 0x2c) = SUB84(dVar19,0);
    *(int *)(puVar5 + 5) = (int)((ulonglong)uVar18 >> 0x20);
    *(int *)((int)puVar5 + 0x24) = (int)uVar1;
    puVar5 = puVar5 + 4;
    *(int *)puVar5 = (int)((ulonglong)uVar1 >> 0x20);
  }
  else {
    *(double *)((int)puVar5 + 0x34) = dVar19;
    *(undefined8 *)((int)puVar5 + 0x2c) = in_xd2;
    *(double *)((int)puVar5 + 0x24) = dVar16;
    *(undefined8 *)((int)puVar5 + 0x1c) = in_xd4;
    *(undefined8 *)((int)puVar5 + 0x14) = uVar18;
    *(double *)((int)puVar5 + 0xc) = in_xd0;
    puVar5 = (undefined8 *)((int)puVar5 + 4);
    *puVar5 = uVar1;
  }
  *(undefined4 *)((int)puVar5 + -4) = uVar2;
  return CONCAT44(unaff_r12,
                  (uint)(iVar9 == 3) | (uint)in_S << 1 | (uint)in_IMASK << 4 | (uint)in_Q << 8 |
                  (uint)in_M << 9 | (uint)in_FD << 0xf | (uint)in_BL << 0x1c | (uint)in_RB << 0x1d |
                  (uint)in_MD << 0x1e) & 0xffffffffdfffffff | 0x20000000;
}

