/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c038280) */
/* WARNING: Removing unreachable block (ram,0x8c037f4a) */
/* WARNING: Removing unreachable block (ram,0x8c037ee8) */
/* WARNING: Removing unreachable block (ram,0x8c037f0c) */
/* WARNING: Removing unreachable block (ram,0x8c037f88) */
/* WARNING: Removing unreachable block (ram,0x8c037fd2) */

ulonglong FUN_8c037ee0(undefined4 param_1,undefined4 param_2,undefined8 param_3,undefined4 param_4)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  byte bVar5;
  double *pdVar8;
  undefined4 in_r1;
  double *pdVar9;
  double *pdVar10;
  int iVar11;
  uint uVar12;
  uint uVar13;
  uint uVar14;
  uint uVar15;
  int iVar16;
  int iVar17;
  double *unaff_r8;
  double *unaff_r9;
  double *unaff_r10;
  short *unaff_r11;
  uint uVar18;
  uint uVar19;
  uint uVar20;
  uint uVar21;
  double *unaff_r14;
  undefined4 in_fr1;
  double dVar22;
  float fVar23;
  float fVar24;
  undefined8 in_dr2;
  double dVar25;
  undefined8 uVar26;
  undefined4 uVar27;
  double dVar28;
  float fVar29;
  undefined4 uVar30;
  float fVar31;
  undefined4 uVar32;
  double dVar33;
  undefined4 uVar35;
  float fVar36;
  undefined4 uVar37;
  double dVar38;
  float fVar39;
  double dVar40;
  undefined4 uVar41;
  float fVar42;
  double dVar43;
  float fVar44;
  undefined4 in_fr15;
  double dVar45;
  double in_xd0;
  double in_xd2;
  double in_xd4;
  double in_xd6;
  undefined8 in_xd8;
  int in_GBR;
  byte in_MD;
  byte in_RB;
  byte in_BL;
  byte in_FD;
  byte in_M;
  byte in_Q;
  byte in_IMASK;
  byte in_S;
  byte in_T;
  bool bVar46;
  int iVar47;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  byte bVar6;
  byte bVar7;
  float fVar34;
  
  pdVar9 = DAT_8c037ff0;
  uVar1 = (uint)in_T | (uint)in_S << 1;
  uVar2 = uVar1 | (uint)in_IMASK << 4;
  uVar3 = uVar2 | (uint)in_Q << 8;
  uVar4 = uVar3 | (uint)in_M << 9;
  bVar6 = (byte)(((uint)in_BL << 0x1c) >> 0x18);
  uVar1 = (uVar1 & 2) >> 1;
  uVar2 = (uVar2 & 0xf0) >> 4;
  bVar7 = (byte)(uVar3 >> 8) & 1;
  uVar3 = (uVar4 & 0x200) >> 9;
  uVar4 = ((uVar4 | (uint)in_FD << 0xf) & 0x8000) >> 0xf;
  bVar5 = (bVar6 & 0x10) >> 4;
  bVar6 = ((bVar6 | (byte)(((uint)in_RB << 0x1d) >> 0x18) | (byte)(((uint)in_MD << 0x1e) >> 0x18)) &
          0x40) >> 6;
  uVar27 = (undefined4)((ulonglong)param_3 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    dVar45 = (double)CONCAT44(*(undefined4 *)DAT_8c037fe8,in_fr15);
    pdVar8 = (double *)((int)DAT_8c037fe8 + 4);
  }
  else {
    dVar45 = *DAT_8c037fe8;
    pdVar8 = DAT_8c037fe8 + 1;
  }
  fVar23 = (float)((ulonglong)dVar45 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar38 = (double)CONCAT44(uVar27,1.0 / fVar23);
  }
  else {
    dVar38 = (double)CONCAT44(uVar27,0x3f800000) / dVar45;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar45 = (double)CONCAT44(fVar23,*(undefined4 *)pdVar8);
  }
  if (in_FPSCR_PR == '\0') {
    dVar40 = (double)CONCAT44(1.0 / SUB84(dVar45,0),param_4);
  }
  else {
    dVar40 = (double)CONCAT44(0x3f800000,param_4) / dVar45;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar43 = (double)CONCAT44(*(undefined4 *)DAT_8c037fec,*(undefined4 *)((int)DAT_8c037fec + 4));
  }
  else {
    dVar43 = *DAT_8c037fec;
  }
  uVar27 = (undefined4)((ulonglong)in_dr2 >> 0x20);
  fVar23 = (float)(int)*unaff_r11;
  if (in_FPSCR_PR == '\0') {
    dVar28 = (double)CONCAT44((float)(int)unaff_r11[1],param_1);
  }
  else {
    dVar28 = (double)(int)unaff_r11[1];
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)DAT_8c037ff0 = (int)((ulonglong)dVar28 >> 0x20);
    pdVar9 = (double *)((int)pdVar9 + -4);
    *(float *)pdVar9 = fVar23;
  }
  else {
    *DAT_8c037ff0 = dVar28;
    pdVar9 = pdVar9 + -1;
    *pdVar9 = in_xd2;
  }
  *(undefined4 *)((int)pdVar9 + -4) = *(undefined4 *)(in_GBR + 0x48);
  if (in_FPSCR_SZ == '\0') {
    fVar23 = *(float *)unaff_r8;
    dVar28 = (double)CONCAT44(*(undefined4 *)((int)unaff_r8 + 4),*(undefined4 *)(unaff_r8 + 1));
    pdVar8 = (double *)((int)unaff_r8 + 0xc);
  }
  else {
    in_xd2 = *unaff_r8;
    dVar28 = unaff_r8[1];
    in_xd4 = unaff_r8[2];
    pdVar8 = unaff_r8 + 3;
  }
  *(undefined4 *)(pdVar9 + -1) = *(undefined4 *)pdVar8;
  fVar29 = (float)((ulonglong)dVar28 >> 0x20);
  fVar24 = SUB84(dVar28,0);
  if (in_FPSCR_SZ == '\0') {
    *(float *)((int)pdVar9 + -0xc) = fVar23;
    *(float *)(pdVar9 + -2) = fVar29;
    pdVar9 = (double *)((int)pdVar9 + -0x14);
    *(float *)pdVar9 = fVar24;
  }
  else {
    pdVar9[-2] = in_xd2;
    pdVar9[-3] = dVar28;
    pdVar9 = pdVar9 + -4;
    *pdVar9 = in_xd4;
  }
  fVar44 = (float)((ulonglong)dVar43 >> 0x20);
  fVar36 = SUB84(dVar38,0);
  if (in_FPSCR_PR == '\0') {
    fVar34 = 1.0 / fVar24;
    dVar33 = (double)CONCAT44(fVar34,param_2);
    dVar25 = (double)CONCAT44(uVar27,(fVar23 - fVar44) * fVar34 * fVar36);
  }
  else {
    dVar33 = (double)CONCAT44(0x3f800000,param_2) / dVar28;
    fVar34 = (float)((ulonglong)dVar33 >> 0x20);
    dVar25 = ((double)CONCAT44(uVar27,fVar23) - dVar43) * dVar33 * dVar38;
  }
  if (in_FPSCR_SZ == '\0') {
    pdVar9 = (double *)((int)pdVar9 + -4);
    *(int *)pdVar9 = SUB84(dVar25,0);
  }
  else {
    pdVar9 = pdVar9 + -1;
    *pdVar9 = in_xd2;
  }
  fVar42 = SUB84(dVar43,0);
  fVar23 = (float)((ulonglong)dVar40 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar28 = (double)CONCAT44((fVar29 - fVar42) * fVar34 * fVar23,fVar24);
  }
  else {
    dVar28 = (dVar28 - dVar43) * dVar33 * dVar40;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)pdVar9 + -4) = (int)((ulonglong)dVar28 >> 0x20);
    pdVar9 = pdVar9 + -1;
    *(float *)pdVar9 = fVar34;
  }
  else {
    pdVar9[-1] = dVar28;
    pdVar9 = pdVar9 + -2;
    *pdVar9 = dVar33;
  }
  uVar27 = (undefined4)((ulonglong)dVar25 >> 0x20);
  fVar24 = (float)(int)unaff_r11[2];
  if (in_FPSCR_PR == '\0') {
    dVar28 = (double)CONCAT44((float)(int)unaff_r11[3],SUB84(dVar28,0));
  }
  else {
    dVar28 = (double)(int)unaff_r11[3];
  }
  if (in_FPSCR_SZ == '\0') {
    *(float *)((int)pdVar9 + 0x4c) = (float)((ulonglong)dVar28 >> 0x20);
    pdVar9 = pdVar9 + 9;
    *(float *)pdVar9 = fVar24;
  }
  else {
    *(double *)((int)pdVar9 + 0x4c) = dVar28;
    pdVar9 = (double *)((int)pdVar9 + 0x44);
    *pdVar9 = in_xd2;
  }
  *(float *)((int)pdVar9 + -4) = *(float *)(in_GBR + 0x4c);
  if (in_FPSCR_SZ == '\0') {
    fVar24 = *(float *)unaff_r9;
    dVar28 = (double)CONCAT44(*(undefined4 *)((int)unaff_r9 + 4),*(undefined4 *)(unaff_r9 + 1));
    pdVar8 = (double *)((int)unaff_r9 + 0xc);
  }
  else {
    in_xd2 = *unaff_r9;
    dVar28 = unaff_r9[1];
    in_xd4 = unaff_r9[2];
    pdVar8 = unaff_r9 + 3;
  }
  *(float *)(pdVar9 + -1) = *(float *)pdVar8;
  fVar39 = (float)((ulonglong)dVar28 >> 0x20);
  fVar29 = SUB84(dVar28,0);
  if (in_FPSCR_SZ == '\0') {
    *(float *)((int)pdVar9 + -0xc) = fVar24;
    *(float *)(pdVar9 + -2) = fVar39;
    pdVar9 = (double *)((int)pdVar9 + -0x14);
    *(float *)pdVar9 = fVar29;
  }
  else {
    pdVar9[-2] = in_xd2;
    pdVar9[-3] = dVar28;
    pdVar9 = pdVar9 + -4;
    *pdVar9 = in_xd4;
  }
  if (in_FPSCR_PR == '\0') {
    fVar31 = 1.0 / fVar29;
    dVar33 = (double)CONCAT44(fVar34,fVar31);
    dVar38 = (double)CONCAT44(uVar27,(fVar24 - fVar44) * fVar31 * fVar36);
  }
  else {
    dVar33 = (double)CONCAT44(fVar34,0x3f800000) / dVar28;
    fVar31 = SUB84(dVar33,0);
    dVar38 = ((double)CONCAT44(uVar27,fVar24) - dVar43) * dVar33 * dVar38;
  }
  if (in_FPSCR_SZ == '\0') {
    pdVar9 = (double *)((int)pdVar9 + -4);
    *(float *)pdVar9 = SUB84(dVar38,0);
  }
  else {
    pdVar9 = pdVar9 + -1;
    *pdVar9 = in_xd2;
  }
  if (in_FPSCR_PR == '\0') {
    dVar28 = (double)CONCAT44((fVar39 - fVar42) * fVar31 * fVar23,fVar29);
  }
  else {
    dVar28 = (dVar28 - dVar43) * dVar33 * dVar40;
  }
  if (in_FPSCR_SZ == '\0') {
    *(float *)((int)pdVar9 + -4) = (float)((ulonglong)dVar28 >> 0x20);
    pdVar9 = pdVar9 + -1;
    *(float *)pdVar9 = fVar31;
  }
  else {
    pdVar9[-1] = dVar28;
    pdVar9 = pdVar9 + -2;
    *pdVar9 = in_xd6;
  }
  fVar24 = (float)(int)unaff_r11[4];
  if (in_FPSCR_PR == '\0') {
    dVar28 = (double)CONCAT44((float)(int)unaff_r11[5],SUB84(dVar28,0));
  }
  else {
    dVar28 = (double)(int)unaff_r11[5];
  }
  if (in_FPSCR_SZ == '\0') {
    *(float *)((int)pdVar9 + 0x4c) = (float)((ulonglong)dVar28 >> 0x20);
    pdVar9 = pdVar9 + 9;
    *(float *)pdVar9 = fVar24;
  }
  else {
    *(double *)((int)pdVar9 + 0x4c) = dVar28;
    pdVar9 = (double *)((int)pdVar9 + 0x44);
    *pdVar9 = in_xd2;
  }
  *(float *)((int)pdVar9 + -4) = *(float *)(in_GBR + 0x50);
  if (in_FPSCR_SZ == '\0') {
    fVar24 = *(float *)unaff_r10;
    dVar28 = (double)CONCAT44(*(undefined4 *)((int)unaff_r10 + 4),*(undefined4 *)(unaff_r10 + 1));
    pdVar8 = (double *)((int)unaff_r10 + 0xc);
  }
  else {
    in_xd2 = *unaff_r10;
    dVar28 = unaff_r10[1];
    in_xd4 = unaff_r10[2];
    pdVar8 = unaff_r10 + 3;
  }
  *(float *)(pdVar9 + -1) = *(float *)pdVar8;
  fVar34 = (float)((ulonglong)dVar28 >> 0x20);
  fVar29 = SUB84(dVar28,0);
  if (in_FPSCR_SZ == '\0') {
    *(float *)((int)pdVar9 + -0xc) = fVar24;
    *(float *)(pdVar9 + -2) = fVar34;
    pdVar9 = (double *)((int)pdVar9 + -0x14);
    *(float *)pdVar9 = fVar29;
  }
  else {
    pdVar9[-2] = in_xd2;
    pdVar9[-3] = dVar28;
    pdVar9 = pdVar9 + -4;
    *pdVar9 = in_xd4;
  }
  if (in_FPSCR_PR == '\0') {
    fVar39 = 1.0 / fVar29;
    dVar25 = (double)CONCAT44(fVar39,fVar36);
    dVar38 = (double)(ulonglong)(uint)((fVar24 - fVar44) * fVar39 * fVar36);
  }
  else {
    dVar25 = (double)CONCAT44(0x3f800000,fVar36) / dVar28;
    fVar39 = (float)((ulonglong)dVar25 >> 0x20);
    dVar38 = ((double)CONCAT44((int)((ulonglong)dVar38 >> 0x20),fVar24) - dVar43) * dVar25 * dVar25;
  }
  if (in_FPSCR_SZ == '\0') {
    pdVar9 = (double *)((int)pdVar9 + -4);
    *(float *)pdVar9 = SUB84(dVar38,0);
  }
  else {
    pdVar9 = pdVar9 + -1;
    *pdVar9 = in_xd2;
  }
  if (in_FPSCR_PR == '\0') {
    dVar28 = (double)CONCAT44((fVar34 - fVar42) * fVar39 * fVar23,fVar29);
  }
  else {
    dVar28 = (dVar28 - dVar43) * dVar25 * dVar40;
  }
  if (in_FPSCR_SZ == '\0') {
    *(float *)((int)pdVar9 + -4) = (float)((ulonglong)dVar28 >> 0x20);
    *(float *)(pdVar9 + -1) = fVar39;
  }
  else {
    pdVar9[-1] = dVar28;
    pdVar9[-2] = dVar25;
  }
  dVar22 = (double)CONCAT44(0x3f800000,in_fr1);
  if (in_FPSCR_PR == '\0') {
    bVar46 = 1.0 < (float)((ulonglong)dVar33 >> 0x20);
  }
  else {
    bVar46 = dVar22 < dVar33;
  }
  if (!bVar46) {
    if (in_FPSCR_PR == '\0') {
      bVar46 = 1.0 < fVar31;
    }
    else {
      bVar46 = dVar22 < dVar33;
    }
    if (!bVar46) {
      if (in_FPSCR_PR == '\0') {
        bVar46 = 1.0 < fVar39;
      }
      else {
        bVar46 = dVar22 < dVar25;
      }
      if (bVar46 != true) {
        return CONCAT44(in_r1,(uint)bVar46 | uVar1 << 1 | uVar2 << 4 | (uint)bVar7 << 8 | uVar3 << 9
                              | uVar4 << 0xf | (uint)bVar5 << 0x1c | (uint)bVar6 << 0x1e) |
               0x20000000;
      }
    }
  }
  iVar17 = 3;
  iVar16 = 0;
  pdVar8 = DAT_8c03801c;
  pdVar9 = DAT_8c038020;
  iVar11 = DAT_8c038024;
  while( true ) {
    pdVar10 = pdVar8;
    fVar23 = SUB84(dVar22,0);
    if (in_FPSCR_SZ == '\0') {
      fVar23 = *(float *)pdVar9;
    }
    else {
      in_xd0 = *pdVar9;
    }
    dVar22 = (double)CONCAT44(0x3f800000,fVar23);
    if (in_FPSCR_SZ == '\0') {
      dVar38 = (double)CONCAT44(*(undefined4 *)pdVar10,SUB84(dVar38,0));
    }
    else {
      dVar38 = *pdVar10;
    }
    fVar24 = (float)((ulonglong)dVar38 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      bVar46 = fVar24 < 1.0;
    }
    else {
      bVar46 = dVar38 < dVar22;
    }
    if ((bool)((in_FPSCR_PR == '\0' && fVar23 < 1.0) ^ bVar46)) {
      if (in_FPSCR_PR == '\0') {
        fVar24 = fVar24 - fVar23;
        dVar38 = (double)CONCAT44(fVar24,SUB84(dVar38,0));
        dVar22 = (double)CONCAT44((1.0 - fVar23) / fVar24,fVar23);
      }
      else {
        dVar38 = dVar38 - (dVar22 - dVar22);
        dVar22 = (dVar22 - dVar22) / dVar38;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)((int)pdVar9 + 4))
        ;
        pdVar9 = pdVar9 + 1;
        uVar27 = *(undefined4 *)((int)pdVar10 + 4);
        pdVar10 = pdVar10 + 1;
        dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),uVar27);
        dVar38 = (double)CONCAT44(uVar27,SUB84(dVar38,0));
      }
      else {
        in_xd0 = *(double *)((int)pdVar9 + 4);
        pdVar9 = (double *)((int)pdVar9 + 0xc);
        in_xd4 = *(double *)((int)pdVar10 + 4);
        pdVar10 = (double *)((int)pdVar10 + 0xc);
        dVar38 = in_xd4;
      }
      fVar23 = (float)((ulonglong)dVar22 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar24 = (SUB84(dVar28,0) - SUB84(dVar22,0)) * fVar23 + SUB84(dVar22,0);
      }
      else {
        fVar24 = SUB84((dVar28 - dVar22) * dVar22 + dVar22,0);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar22 = (double)CONCAT44(fVar23,*(undefined4 *)pdVar9);
        pdVar9 = (double *)((int)pdVar9 + 4);
        pdVar8 = (double *)((int)pdVar10 + 4);
        dVar28 = (double)CONCAT44(*(undefined4 *)pdVar10,fVar24);
        dVar38 = (double)CONCAT44(*(undefined4 *)pdVar10,SUB84(dVar38,0));
      }
      else {
        in_xd0 = *pdVar9;
        pdVar9 = pdVar9 + 1;
        dVar28 = *pdVar10;
        pdVar8 = pdVar10 + 1;
        dVar38 = dVar28;
      }
      fVar23 = (float)((ulonglong)dVar22 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar28 = (double)CONCAT44((((float)((ulonglong)dVar28 >> 0x20) - SUB84(dVar22,0)) * fVar23 +
                                  SUB84(dVar22,0)) * (float)((ulonglong)dVar45 >> 0x20) + fVar44,
                                  SUB84(dVar28,0) * SUB84(dVar45,0) + fVar42);
      }
      else {
        dVar28 = (((dVar28 - dVar22) * dVar22 + dVar22) * dVar45 + dVar43) * dVar45 + dVar43;
      }
      fVar24 = SUB84(dVar25,0);
      if (in_FPSCR_SZ == '\0') {
        fVar24 = *(float *)PTR_DAT_8c0381c0;
      }
      else {
        in_xd8 = *(undefined8 *)PTR_DAT_8c0381c0;
      }
      if (in_FPSCR_PR == '\0') {
        dVar25 = (double)CONCAT44(unaff_r14,fVar24 * fVar23);
        iVar47 = (int)(fVar24 * fVar23);
      }
      else {
        dVar25 = (double)CONCAT44(unaff_r14,fVar24) * dVar22;
        iVar47 = (int)dVar25;
      }
      uVar12 = *(uint *)((int)pdVar9 + 0xc);
      uVar14 = *(uint *)((int)pdVar8 + 0xc);
      uVar18 = uVar12 >> 8 & 0xff;
      uVar19 = uVar12 >> 0x10 & 0xff;
      unaff_r14 = (double *)((ulonglong)dVar25 >> 0x20);
      uVar13 = *(uint *)(pdVar9 + 2);
      uVar15 = *(uint *)(pdVar8 + 2);
      uVar20 = uVar13 >> 8 & 0xff;
      uVar21 = uVar13 >> 0x10 & 0xff;
      uVar13 = (uVar13 & 0xff) + (int)(short)(iVar47 * ((uVar15 & 0xff) - (uVar13 & 0xff)) >> 0x10)
               | (uVar20 + (int)(short)(iVar47 * ((uVar15 >> 8 & 0xff) - uVar20) >> 0x10)) * 0x100 |
               (uVar21 + (int)(short)(iVar47 * ((uVar15 >> 0x10 & 0xff) - uVar21) >> 0x10)) *
               0x10000 | ((uVar13 >> 0x18) +
                         (int)(short)(iVar47 * ((uVar15 >> 0x18) - (uVar13 >> 0x18)) >> 0x10)) *
                         0x1000000;
      dVar40 = (double)CONCAT44(uVar13,SUB84(dVar40,0));
      if (in_FPSCR_SZ == '\0') {
        dVar22 = (double)CONCAT44(fVar23,*(undefined4 *)((int)pdVar9 + 0x14));
        pdVar9 = pdVar9 + 3;
        uVar27 = *(undefined4 *)((int)pdVar8 + 0x14);
        pdVar8 = pdVar8 + 3;
        dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),uVar27);
        dVar38 = (double)CONCAT44(uVar27,SUB84(dVar38,0));
      }
      else {
        in_xd0 = *(double *)((int)pdVar9 + 0x14);
        pdVar9 = (double *)((int)pdVar9 + 0x1c);
        in_xd6 = *(double *)((int)pdVar8 + 0x14);
        pdVar8 = (double *)((int)pdVar8 + 0x1c);
        dVar38 = in_xd6;
      }
      fVar23 = (float)((ulonglong)dVar22 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar24 = (SUB84(dVar33,0) - SUB84(dVar22,0)) * fVar23 + SUB84(dVar22,0);
      }
      else {
        fVar24 = SUB84((dVar33 - dVar22) * dVar22 + dVar22,0);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar22 = (double)CONCAT44(fVar23,*(undefined4 *)pdVar9);
        pdVar10 = (double *)((int)pdVar8 + 4);
        dVar33 = (double)CONCAT44(*(undefined4 *)pdVar8,
                                  (uVar12 & 0xff) +
                                  (int)(short)(iVar47 * ((uVar14 & 0xff) - (uVar12 & 0xff)) >> 0x10)
                                  | (uVar18 + (int)(short)(iVar47 * ((uVar14 >> 8 & 0xff) - uVar18)
                                                          >> 0x10)) * 0x100 |
                                  (uVar19 + (int)(short)(iVar47 * ((uVar14 >> 0x10 & 0xff) - uVar19)
                                                        >> 0x10)) * 0x10000 |
                                  ((uVar12 >> 0x18) +
                                  (int)(short)(iVar47 * ((uVar14 >> 0x18) - (uVar12 >> 0x18)) >>
                                              0x10)) * 0x1000000);
        dVar38 = (double)CONCAT44(*(undefined4 *)pdVar8,SUB84(dVar38,0));
      }
      else {
        in_xd0 = *pdVar9;
        dVar33 = *pdVar8;
        pdVar10 = pdVar8 + 1;
        dVar38 = dVar33;
      }
      if (in_FPSCR_PR == '\0') {
        dVar25 = (double)CONCAT44(((float)((ulonglong)dVar33 >> 0x20) - SUB84(dVar22,0)) *
                                  (float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                  SUB84(dVar33,0));
      }
      else {
        dVar25 = (dVar33 - dVar22) * dVar22 + dVar22;
      }
      pdVar10 = pdVar10 + -5;
      if (in_FPSCR_SZ == '\0') {
        *(int *)(iVar11 + 0x24) = (int)((ulonglong)dVar25 >> 0x20);
        *(float *)(iVar11 + 0x20) = fVar24;
        *(uint *)(iVar11 + 0x1c) = uVar13;
        *(int *)(iVar11 + 0x18) = SUB84(dVar25,0);
        *(int *)(iVar11 + 0x14) = (int)((ulonglong)dVar28 >> 0x20);
        pdVar9 = (double *)(iVar11 + 0x10);
        *(int *)pdVar9 = SUB84(dVar28,0);
      }
      else {
        *(double *)(iVar11 + 0x24) = dVar25;
        *(double *)(iVar11 + 0x1c) = in_xd6;
        *(double *)(iVar11 + 0x14) = dVar40;
        *(undefined8 *)(iVar11 + 0xc) = in_xd8;
        *(double *)(iVar11 + 4) = dVar28;
        pdVar9 = (double *)(iVar11 + -4);
        *pdVar9 = in_xd4;
      }
      dVar33 = (double)CONCAT44(0x3f800000,fVar24);
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)((int)pdVar9 + -4) = 0x3f800000;
      }
      else {
        pdVar9[-1] = dVar33;
      }
      iVar11 = iVar11 + 0x28;
      iVar16 = iVar16 + 1;
    }
    dVar22 = (double)CONCAT44(0x3f800000,SUB84(dVar22,0));
    if (in_FPSCR_SZ == '\0') {
      dVar38 = (double)CONCAT44(*(undefined4 *)pdVar10,SUB84(dVar38,0));
    }
    else {
      dVar38 = *pdVar10;
    }
    if (in_FPSCR_PR == '\0') {
      bVar46 = 1.0 < (float)((ulonglong)dVar38 >> 0x20);
    }
    else {
      bVar46 = dVar22 < dVar38;
    }
    if (bVar46) {
      if (in_FPSCR_SZ == '\0') {
        uVar35 = *(undefined4 *)((int)pdVar10 + 0xc);
        uVar27 = *(undefined4 *)(pdVar10 + 2);
        uVar30 = *(undefined4 *)((int)pdVar10 + 0x14);
        dVar28 = pdVar10[2];
        uVar37 = *(undefined4 *)(pdVar10 + 3);
        uVar41 = *(undefined4 *)((int)pdVar10 + 0x1c);
        dVar40 = (double)CONCAT44(uVar41,SUB84(dVar40,0));
        uVar32 = *(undefined4 *)(pdVar10 + 4);
        dVar33 = (double)CONCAT44(uVar35,uVar32);
        dVar25 = (double)CONCAT44(*(undefined4 *)((int)pdVar10 + 0x24),uVar37);
        *(undefined4 *)(iVar11 + 0x24) = *(undefined4 *)((int)pdVar10 + 0x24);
        *(undefined4 *)(iVar11 + 0x20) = uVar32;
        *(undefined4 *)(iVar11 + 0x1c) = uVar41;
        *(undefined4 *)(iVar11 + 0x18) = uVar37;
        *(undefined4 *)(iVar11 + 0x14) = uVar30;
        *(undefined4 *)(iVar11 + 0x10) = uVar27;
        *(undefined4 *)(iVar11 + 0xc) = uVar35;
      }
      else {
        dVar33 = *(double *)((int)pdVar10 + 0xc);
        in_xd4 = *(double *)((int)pdVar10 + 0x14);
        dVar28 = *(double *)((int)pdVar10 + 0x1c);
        in_xd8 = *(undefined8 *)((int)pdVar10 + 0x24);
        dVar40 = *(double *)((int)pdVar10 + 0x2c);
        in_xd6 = *(double *)((int)pdVar10 + 0x34);
        dVar25 = *(double *)((int)pdVar10 + 0x3c);
        *(double *)(iVar11 + 0x24) = dVar25;
        *(double *)(iVar11 + 0x1c) = in_xd6;
        *(double *)(iVar11 + 0x14) = dVar40;
        *(undefined8 *)(iVar11 + 0xc) = in_xd8;
        *(double *)(iVar11 + 4) = dVar28;
        *(double *)(iVar11 + -4) = in_xd4;
        *(double *)(iVar11 + -0xc) = dVar33;
      }
      iVar11 = iVar11 + 0x28;
      iVar16 = iVar16 + 1;
    }
    uVar27 = DAT_8c038294;
    iVar17 = iVar17 + -1;
    if (iVar17 == 0) break;
    pdVar8 = pdVar10 + 5;
    pdVar9 = pdVar10;
  }
  if (in_FPSCR_SZ == '\0') {
    pdVar9 = (double *)((int)DAT_8c038298 + 4);
    dVar38 = (double)CONCAT44(*(undefined4 *)DAT_8c038298,*(undefined4 *)((int)DAT_8c038298 + 0xc));
    dVar45 = (double)CONCAT44(*(undefined4 *)(DAT_8c038298 + 2),
                              *(undefined4 *)((int)DAT_8c038298 + 0x14));
    dVar43 = (double)CONCAT44(*(undefined4 *)(DAT_8c038298 + 3),SUB84(dVar33,0));
  }
  else {
    dVar38 = *DAT_8c038298;
    in_xd0 = DAT_8c038298[1];
    pdVar9 = DAT_8c038298 + 2;
    in_xd2 = DAT_8c038298[3];
    dVar45 = DAT_8c038298[4];
    in_xd4 = DAT_8c038298[5];
    dVar43 = DAT_8c038298[6];
  }
  dVar28 = *pdVar9;
  fVar23 = SUB84(dVar40,0);
  if (in_FPSCR_PR == '\0') {
    dVar45 = (double)CONCAT44((int)((ulonglong)dVar45 >> 0x20),SUB84(dVar45,0) * fVar23);
    dVar43 = (double)CONCAT44((float)((ulonglong)dVar43 >> 0x20) * fVar23,SUB84(dVar43,0));
  }
  else {
    dVar45 = dVar45 * dVar40;
    dVar43 = dVar43 * dVar40;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)unaff_r14 + 0x1c) = (int)((ulonglong)dVar45 >> 0x20);
    *(int *)(unaff_r14 + 3) = SUB84(dVar38,0);
    *(int *)((int)unaff_r14 + 0x14) = (int)((ulonglong)dVar43 >> 0x20);
    *(int *)(unaff_r14 + 2) = SUB84(dVar45,0);
    *(int *)((int)unaff_r14 + 0xc) = (int)((ulonglong)dVar38 >> 0x20);
    *(int *)(unaff_r14 + 1) = SUB84(dVar28,0);
    pdVar9 = (double *)((int)unaff_r14 + 4);
    *(int *)pdVar9 = (int)((ulonglong)dVar28 >> 0x20);
  }
  else {
    unaff_r14[3] = dVar45;
    unaff_r14[2] = in_xd2;
    unaff_r14[1] = dVar43;
    *unaff_r14 = in_xd4;
    unaff_r14[-1] = dVar38;
    unaff_r14[-2] = in_xd0;
    pdVar9 = unaff_r14 + -3;
    *pdVar9 = dVar28;
  }
  *(undefined4 *)((int)pdVar9 + -4) = uVar27;
  if (in_FPSCR_SZ == '\0') {
    pdVar8 = (double *)((int)DAT_8c03829c + 4);
    uVar26 = CONCAT44(*(undefined4 *)DAT_8c03829c,*(undefined4 *)((int)DAT_8c03829c + 0xc));
    dVar45 = (double)CONCAT44(*(undefined4 *)(DAT_8c03829c + 2),
                              *(undefined4 *)((int)DAT_8c03829c + 0x14));
    dVar38 = (double)CONCAT44(*(undefined4 *)(DAT_8c03829c + 3),SUB84(dVar43,0));
  }
  else {
    uVar26 = *DAT_8c03829c;
    in_xd0 = (double)DAT_8c03829c[1];
    pdVar8 = (double *)(DAT_8c03829c + 2);
    in_xd2 = (double)DAT_8c03829c[3];
    dVar45 = (double)DAT_8c03829c[4];
    in_xd4 = (double)DAT_8c03829c[5];
    dVar38 = (double)DAT_8c03829c[6];
  }
  dVar43 = *pdVar8;
  if (in_FPSCR_PR == '\0') {
    dVar45 = (double)CONCAT44((int)((ulonglong)dVar45 >> 0x20),SUB84(dVar45,0) * fVar23);
    dVar38 = (double)CONCAT44((float)((ulonglong)dVar38 >> 0x20) * fVar23,SUB84(dVar38,0));
  }
  else {
    dVar45 = dVar45 * dVar40;
    dVar38 = dVar38 * dVar40;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(pdVar9 + 7) = (int)((ulonglong)dVar45 >> 0x20);
    *(int *)((int)pdVar9 + 0x34) = (int)uVar26;
    *(int *)(pdVar9 + 6) = (int)((ulonglong)dVar38 >> 0x20);
    *(int *)((int)pdVar9 + 0x2c) = SUB84(dVar45,0);
    *(int *)(pdVar9 + 5) = (int)((ulonglong)uVar26 >> 0x20);
    *(int *)((int)pdVar9 + 0x24) = SUB84(dVar43,0);
    pdVar9 = pdVar9 + 4;
    *(int *)pdVar9 = (int)((ulonglong)dVar43 >> 0x20);
  }
  else {
    *(double *)((int)pdVar9 + 0x34) = dVar45;
    *(double *)((int)pdVar9 + 0x2c) = in_xd2;
    *(double *)((int)pdVar9 + 0x24) = dVar38;
    *(double *)((int)pdVar9 + 0x1c) = in_xd4;
    *(undefined8 *)((int)pdVar9 + 0x14) = uVar26;
    *(double *)((int)pdVar9 + 0xc) = in_xd0;
    pdVar9 = (double *)((int)pdVar9 + 4);
    *pdVar9 = dVar43;
  }
  *(undefined4 *)((int)pdVar9 + -4) = uVar27;
  if (iVar16 != 3) {
    if (in_FPSCR_SZ == '\0') {
      pdVar8 = (double *)((int)DAT_8c0382a0 + 4);
      uVar26 = CONCAT44(*(undefined4 *)DAT_8c0382a0,*(undefined4 *)((int)DAT_8c0382a0 + 0xc));
      dVar45 = (double)CONCAT44(*(undefined4 *)(DAT_8c0382a0 + 2),
                                *(undefined4 *)((int)DAT_8c0382a0 + 0x14));
      dVar38 = (double)CONCAT44(*(undefined4 *)(DAT_8c0382a0 + 3),SUB84(dVar38,0));
    }
    else {
      uVar26 = *DAT_8c0382a0;
      in_xd0 = (double)DAT_8c0382a0[1];
      pdVar8 = (double *)(DAT_8c0382a0 + 2);
      in_xd2 = (double)DAT_8c0382a0[3];
      dVar45 = (double)DAT_8c0382a0[4];
      in_xd4 = (double)DAT_8c0382a0[5];
      dVar38 = (double)DAT_8c0382a0[6];
    }
    dVar43 = *pdVar8;
    if (in_FPSCR_PR == '\0') {
      dVar45 = (double)CONCAT44((int)((ulonglong)dVar45 >> 0x20),SUB84(dVar45,0) * fVar23);
      dVar38 = (double)CONCAT44((float)((ulonglong)dVar38 >> 0x20) * fVar23,SUB84(dVar38,0));
    }
    else {
      dVar45 = dVar45 * dVar40;
      dVar38 = dVar38 * dVar40;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(pdVar9 + 7) = (int)((ulonglong)dVar45 >> 0x20);
      *(int *)((int)pdVar9 + 0x34) = (int)uVar26;
      *(int *)(pdVar9 + 6) = (int)((ulonglong)dVar38 >> 0x20);
      *(int *)((int)pdVar9 + 0x2c) = SUB84(dVar45,0);
      *(int *)(pdVar9 + 5) = (int)((ulonglong)uVar26 >> 0x20);
      *(int *)((int)pdVar9 + 0x24) = SUB84(dVar43,0);
      pdVar9 = pdVar9 + 4;
      *(int *)pdVar9 = (int)((ulonglong)dVar43 >> 0x20);
    }
    else {
      *(double *)((int)pdVar9 + 0x34) = dVar45;
      *(double *)((int)pdVar9 + 0x2c) = in_xd2;
      *(double *)((int)pdVar9 + 0x24) = dVar38;
      *(double *)((int)pdVar9 + 0x1c) = in_xd4;
      *(undefined8 *)((int)pdVar9 + 0x14) = uVar26;
      *(double *)((int)pdVar9 + 0xc) = in_xd0;
      pdVar9 = (double *)((int)pdVar9 + 4);
      *pdVar9 = dVar43;
    }
    *(undefined4 *)((int)pdVar9 + -4) = uVar27;
  }
  uVar27 = DAT_8c0382a4;
  if (in_FPSCR_SZ == '\0') {
    pdVar8 = (double *)((int)DAT_8c0382a8 + 4);
    uVar26 = CONCAT44(*(undefined4 *)DAT_8c0382a8,*(undefined4 *)((int)DAT_8c0382a8 + 0xc));
    dVar45 = (double)CONCAT44(*(undefined4 *)(DAT_8c0382a8 + 2),
                              *(undefined4 *)((int)DAT_8c0382a8 + 0x14));
    dVar38 = (double)CONCAT44(*(undefined4 *)(DAT_8c0382a8 + 3),SUB84(dVar38,0));
  }
  else {
    uVar26 = *DAT_8c0382a8;
    in_xd0 = (double)DAT_8c0382a8[1];
    pdVar8 = (double *)(DAT_8c0382a8 + 2);
    in_xd2 = (double)DAT_8c0382a8[3];
    dVar45 = (double)DAT_8c0382a8[4];
    in_xd4 = (double)DAT_8c0382a8[5];
    dVar38 = (double)DAT_8c0382a8[6];
  }
  dVar43 = *pdVar8;
  if (in_FPSCR_PR == '\0') {
    dVar45 = (double)CONCAT44((int)((ulonglong)dVar45 >> 0x20),SUB84(dVar45,0) * fVar23);
    dVar38 = (double)CONCAT44((float)((ulonglong)dVar38 >> 0x20) * fVar23,SUB84(dVar38,0));
  }
  else {
    dVar45 = dVar45 * dVar40;
    dVar38 = dVar38 * dVar40;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(pdVar9 + 7) = (int)((ulonglong)dVar45 >> 0x20);
    *(int *)((int)pdVar9 + 0x34) = (int)uVar26;
    *(int *)(pdVar9 + 6) = (int)((ulonglong)dVar38 >> 0x20);
    *(int *)((int)pdVar9 + 0x2c) = SUB84(dVar45,0);
    *(int *)(pdVar9 + 5) = (int)((ulonglong)uVar26 >> 0x20);
    *(int *)((int)pdVar9 + 0x24) = SUB84(dVar43,0);
    pdVar9 = pdVar9 + 4;
    *(int *)pdVar9 = (int)((ulonglong)dVar43 >> 0x20);
  }
  else {
    *(double *)((int)pdVar9 + 0x34) = dVar45;
    *(double *)((int)pdVar9 + 0x2c) = in_xd2;
    *(double *)((int)pdVar9 + 0x24) = dVar38;
    *(double *)((int)pdVar9 + 0x1c) = in_xd4;
    *(undefined8 *)((int)pdVar9 + 0x14) = uVar26;
    *(double *)((int)pdVar9 + 0xc) = in_xd0;
    pdVar9 = (double *)((int)pdVar9 + 4);
    *pdVar9 = dVar43;
  }
  *(undefined4 *)((int)pdVar9 + -4) = uVar27;
  return CONCAT44(in_r1,(uint)(iVar16 == 3) | uVar1 << 1 | uVar2 << 4 | (uint)bVar7 << 8 |
                        uVar3 << 9 | uVar4 << 0xf | (uint)bVar5 << 0x1c | (uint)bVar6 << 0x1e) |
         0x20000000;
}

