/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c038b18) */
/* WARNING: Removing unreachable block (ram,0x8c038808) */
/* WARNING: Removing unreachable block (ram,0x8c0388c2) */

ulonglong FUN_8c038802(undefined4 param_1,undefined8 param_2,undefined4 param_3)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  byte bVar5;
  undefined4 *puVar8;
  double *pdVar9;
  undefined4 in_r1;
  double *pdVar10;
  int iVar11;
  int iVar12;
  int iVar13;
  double *unaff_r8;
  double *unaff_r9;
  double *pdVar14;
  double *unaff_r10;
  uint uVar15;
  uint uVar16;
  uint uVar17;
  int unaff_r14;
  undefined8 *puVar18;
  undefined4 in_fr1;
  float fVar19;
  undefined8 uVar20;
  double in_dr2;
  double dVar22;
  undefined8 uVar23;
  undefined4 uVar25;
  double dVar26;
  float fVar27;
  double dVar28;
  undefined4 uVar30;
  float fVar31;
  float fVar32;
  double dVar33;
  double dVar34;
  float fVar35;
  float fVar36;
  double dVar37;
  float fVar38;
  undefined4 in_fr15;
  double dVar39;
  undefined8 uVar40;
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
  bool bVar41;
  int iVar42;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  byte bVar6;
  byte bVar7;
  undefined4 uVar21;
  undefined4 uVar24;
  float fVar29;
  
  puVar8 = DAT_8c0388dc;
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
  uVar25 = (undefined4)((ulonglong)param_2 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    dVar39 = (double)CONCAT44(*(undefined4 *)DAT_8c0388d4,in_fr15);
    pdVar9 = (double *)((int)DAT_8c0388d4 + 4);
  }
  else {
    dVar39 = *DAT_8c0388d4;
    pdVar9 = DAT_8c0388d4 + 1;
  }
  fVar19 = (float)((ulonglong)dVar39 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar33 = (double)CONCAT44(uVar25,1.0 / fVar19);
  }
  else {
    dVar33 = (double)CONCAT44(uVar25,0x3f800000) / dVar39;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar39 = (double)CONCAT44(fVar19,*(undefined4 *)pdVar9);
  }
  if (in_FPSCR_PR == '\0') {
    dVar34 = (double)CONCAT44(1.0 / SUB84(dVar39,0),param_3);
  }
  else {
    dVar34 = (double)CONCAT44(0x3f800000,param_3) / dVar39;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar37 = (double)CONCAT44(*(undefined4 *)DAT_8c0388d8,*(undefined4 *)((int)DAT_8c0388d8 + 4));
  }
  else {
    dVar37 = *DAT_8c0388d8;
  }
  *DAT_8c0388dc = *(undefined4 *)(in_GBR + 0x48);
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)unaff_r8);
    dVar26 = (double)CONCAT44(*(undefined4 *)((int)unaff_r8 + 4),*(undefined4 *)(unaff_r8 + 1));
    pdVar9 = (double *)((int)unaff_r8 + 0xc);
  }
  else {
    in_xd2 = *unaff_r8;
    dVar26 = unaff_r8[1];
    in_xd4 = unaff_r8[2];
    pdVar9 = unaff_r8 + 3;
  }
  puVar8[-1] = *(undefined4 *)pdVar9;
  fVar32 = (float)((ulonglong)dVar26 >> 0x20);
  fVar19 = SUB84(dVar26,0);
  if (in_FPSCR_SZ == '\0') {
    puVar8[-2] = SUB84(in_dr2,0);
    puVar8[-3] = fVar32;
    pdVar9 = (double *)(puVar8 + -4);
    *(float *)pdVar9 = fVar19;
  }
  else {
    *(double *)(puVar8 + -3) = in_xd2;
    *(double *)(puVar8 + -5) = dVar26;
    pdVar9 = (double *)(puVar8 + -7);
    *pdVar9 = in_xd4;
  }
  fVar38 = (float)((ulonglong)dVar37 >> 0x20);
  fVar31 = SUB84(dVar33,0);
  if (in_FPSCR_PR == '\0') {
    fVar29 = 1.0 / fVar19;
    dVar28 = (double)CONCAT44(fVar29,param_1);
    dVar22 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),
                              (SUB84(in_dr2,0) - fVar38) * fVar29 * fVar31);
  }
  else {
    dVar28 = (double)CONCAT44(0x3f800000,param_1) / dVar26;
    fVar29 = (float)((ulonglong)dVar28 >> 0x20);
    dVar22 = (in_dr2 - dVar37) * dVar28 * dVar33;
  }
  if (in_FPSCR_SZ == '\0') {
    pdVar9 = (double *)((int)pdVar9 + -4);
    *(int *)pdVar9 = SUB84(dVar22,0);
  }
  else {
    pdVar9 = pdVar9 + -1;
    *pdVar9 = in_xd2;
  }
  fVar36 = SUB84(dVar37,0);
  fVar35 = (float)((ulonglong)dVar34 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar26 = (double)CONCAT44((fVar32 - fVar36) * fVar29 * fVar35,fVar19);
  }
  else {
    dVar26 = (dVar26 - dVar37) * dVar28 * dVar34;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)pdVar9 + -4) = (int)((ulonglong)dVar26 >> 0x20);
    pdVar9 = pdVar9 + -1;
    *(float *)pdVar9 = fVar29;
  }
  else {
    pdVar9[-1] = dVar26;
    pdVar9 = pdVar9 + -2;
    *pdVar9 = dVar28;
  }
  *(float *)((int)pdVar9 + 0x44) = *(float *)(in_GBR + 0x4c);
  if (in_FPSCR_SZ == '\0') {
    dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)unaff_r9);
    dVar26 = (double)CONCAT44(*(undefined4 *)((int)unaff_r9 + 4),*(undefined4 *)(unaff_r9 + 1));
    pdVar14 = (double *)((int)unaff_r9 + 0xc);
  }
  else {
    in_xd2 = *unaff_r9;
    dVar26 = unaff_r9[1];
    in_xd4 = unaff_r9[2];
    pdVar14 = unaff_r9 + 3;
  }
  *(float *)(pdVar9 + 8) = *(float *)pdVar14;
  fVar32 = (float)((ulonglong)dVar26 >> 0x20);
  fVar19 = SUB84(dVar26,0);
  if (in_FPSCR_SZ == '\0') {
    *(float *)((int)pdVar9 + 0x3c) = SUB84(dVar22,0);
    *(float *)(pdVar9 + 7) = fVar32;
    pdVar9 = (double *)((int)pdVar9 + 0x34);
    *(float *)pdVar9 = fVar19;
  }
  else {
    pdVar9[7] = in_xd2;
    pdVar9[6] = dVar26;
    pdVar9 = pdVar9 + 5;
    *pdVar9 = in_xd4;
  }
  if (in_FPSCR_PR == '\0') {
    fVar27 = 1.0 / fVar19;
    dVar28 = (double)CONCAT44(fVar29,fVar27);
    dVar33 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                              (SUB84(dVar22,0) - fVar38) * fVar27 * fVar31);
  }
  else {
    dVar28 = (double)CONCAT44(fVar29,0x3f800000) / dVar26;
    fVar27 = SUB84(dVar28,0);
    dVar33 = (dVar22 - dVar37) * dVar28 * dVar33;
  }
  if (in_FPSCR_SZ == '\0') {
    pdVar9 = (double *)((int)pdVar9 + -4);
    *(float *)pdVar9 = SUB84(dVar33,0);
  }
  else {
    pdVar9 = pdVar9 + -1;
    *pdVar9 = in_xd2;
  }
  if (in_FPSCR_PR == '\0') {
    dVar26 = (double)CONCAT44((fVar32 - fVar36) * fVar27 * fVar35,fVar19);
  }
  else {
    dVar26 = (dVar26 - dVar37) * dVar28 * dVar34;
  }
  if (in_FPSCR_SZ == '\0') {
    *(float *)((int)pdVar9 + -4) = (float)((ulonglong)dVar26 >> 0x20);
    pdVar9 = pdVar9 + -1;
    *(float *)pdVar9 = fVar27;
  }
  else {
    pdVar9[-1] = dVar26;
    pdVar9 = pdVar9 + -2;
    *pdVar9 = in_xd6;
  }
  *(float *)((int)pdVar9 + 0x44) = *(float *)(in_GBR + 0x50);
  if (in_FPSCR_SZ == '\0') {
    dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),*(undefined4 *)unaff_r10);
    dVar26 = (double)CONCAT44(*(undefined4 *)((int)unaff_r10 + 4),*(undefined4 *)(unaff_r10 + 1));
    pdVar14 = (double *)((int)unaff_r10 + 0xc);
  }
  else {
    in_xd2 = *unaff_r10;
    dVar26 = unaff_r10[1];
    in_xd4 = unaff_r10[2];
    pdVar14 = unaff_r10 + 3;
  }
  *(float *)(pdVar9 + 8) = *(float *)pdVar14;
  fVar32 = (float)((ulonglong)dVar26 >> 0x20);
  fVar19 = SUB84(dVar26,0);
  if (in_FPSCR_SZ == '\0') {
    *(float *)((int)pdVar9 + 0x3c) = SUB84(dVar33,0);
    *(float *)(pdVar9 + 7) = fVar32;
    pdVar9 = (double *)((int)pdVar9 + 0x34);
    *(float *)pdVar9 = fVar19;
  }
  else {
    pdVar9[7] = in_xd2;
    pdVar9[6] = dVar26;
    pdVar9 = pdVar9 + 5;
    *pdVar9 = in_xd4;
  }
  if (in_FPSCR_PR == '\0') {
    fVar29 = 1.0 / fVar19;
    dVar22 = (double)CONCAT44(fVar29,fVar31);
    dVar33 = (double)(ulonglong)(uint)((SUB84(dVar33,0) - fVar38) * fVar29 * fVar31);
  }
  else {
    dVar22 = (double)CONCAT44(0x3f800000,fVar31) / dVar26;
    fVar29 = (float)((ulonglong)dVar22 >> 0x20);
    dVar33 = (dVar33 - dVar37) * dVar22 * dVar22;
  }
  if (in_FPSCR_SZ == '\0') {
    pdVar9 = (double *)((int)pdVar9 + -4);
    *(float *)pdVar9 = SUB84(dVar33,0);
  }
  else {
    pdVar9 = pdVar9 + -1;
    *pdVar9 = in_xd2;
  }
  if (in_FPSCR_PR == '\0') {
    dVar34 = (double)CONCAT44((fVar32 - fVar36) * fVar29 * fVar35,fVar19);
  }
  else {
    dVar34 = (dVar26 - dVar37) * dVar22 * dVar34;
  }
  if (in_FPSCR_SZ == '\0') {
    *(float *)((int)pdVar9 + -4) = (float)((ulonglong)dVar34 >> 0x20);
    *(float *)(pdVar9 + -1) = fVar29;
  }
  else {
    pdVar9[-1] = dVar34;
    pdVar9[-2] = dVar22;
  }
  dVar26 = (double)CONCAT44(0x3f800000,in_fr1);
  if (in_FPSCR_PR == '\0') {
    bVar41 = 1.0 < (float)((ulonglong)dVar28 >> 0x20);
  }
  else {
    bVar41 = dVar26 < dVar28;
  }
  if (!bVar41) {
    if (in_FPSCR_PR == '\0') {
      bVar41 = 1.0 < fVar27;
    }
    else {
      bVar41 = dVar26 < dVar28;
    }
    if (!bVar41) {
      if (in_FPSCR_PR == '\0') {
        bVar41 = 1.0 < fVar29;
      }
      else {
        bVar41 = dVar26 < dVar22;
      }
      if (bVar41 != true) {
        return CONCAT44(in_r1,(uint)bVar41 | uVar1 << 1 | uVar2 << 4 | (uint)bVar7 << 8 | uVar3 << 9
                              | uVar4 << 0xf | (uint)bVar5 << 0x1c | (uint)bVar6 << 0x1e) |
               0x20000000;
      }
    }
  }
  iVar13 = 3;
  iVar12 = 0;
  pdVar14 = DAT_8c038908;
  pdVar9 = DAT_8c03890c;
  iVar11 = DAT_8c038910;
  while( true ) {
    pdVar10 = pdVar14;
    fVar19 = SUB84(dVar26,0);
    if (in_FPSCR_SZ == '\0') {
      fVar19 = *(float *)pdVar9;
    }
    dVar26 = (double)CONCAT44(0x3f800000,fVar19);
    if (in_FPSCR_SZ == '\0') {
      dVar33 = (double)CONCAT44(*(float *)pdVar10,SUB84(dVar33,0));
    }
    else {
      dVar33 = *pdVar10;
    }
    fVar32 = (float)((ulonglong)dVar33 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      bVar41 = fVar32 < 1.0;
    }
    else {
      bVar41 = dVar33 < dVar26;
    }
    if ((bool)((in_FPSCR_PR == '\0' && fVar19 < 1.0) ^ bVar41)) {
      if (in_FPSCR_PR == '\0') {
        fVar32 = fVar32 - fVar19;
        dVar33 = (double)CONCAT44(fVar32,SUB84(dVar33,0));
        dVar26 = (double)CONCAT44((1.0 - fVar19) / fVar32,fVar19);
      }
      else {
        dVar33 = dVar33 - (dVar26 - dVar26);
        dVar26 = (dVar26 - dVar26) / dVar33;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),*(float *)((int)pdVar9 + 4));
        pdVar9 = pdVar9 + 1;
        uVar25 = *(undefined4 *)((int)pdVar10 + 4);
        pdVar10 = pdVar10 + 1;
        dVar34 = (double)CONCAT44((int)((ulonglong)dVar34 >> 0x20),uVar25);
        dVar33 = (double)CONCAT44(uVar25,SUB84(dVar33,0));
      }
      else {
        pdVar9 = (double *)((int)pdVar9 + 0xc);
        in_xd4 = *(double *)((int)pdVar10 + 4);
        pdVar10 = (double *)((int)pdVar10 + 0xc);
        dVar33 = in_xd4;
      }
      fVar19 = (float)((ulonglong)dVar26 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar32 = (SUB84(dVar34,0) - SUB84(dVar26,0)) * fVar19 + SUB84(dVar26,0);
      }
      else {
        fVar32 = SUB84((dVar34 - dVar26) * dVar26 + dVar26,0);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar26 = (double)CONCAT44(fVar19,*(float *)pdVar9);
        pdVar9 = (double *)((int)pdVar9 + 4);
        pdVar14 = (double *)((int)pdVar10 + 4);
        dVar34 = (double)CONCAT44(*(float *)pdVar10,fVar32);
        dVar33 = (double)CONCAT44(*(float *)pdVar10,SUB84(dVar33,0));
      }
      else {
        pdVar9 = pdVar9 + 1;
        dVar34 = *pdVar10;
        pdVar14 = pdVar10 + 1;
        dVar33 = dVar34;
      }
      fVar19 = (float)((ulonglong)dVar26 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar34 = (double)CONCAT44((((float)((ulonglong)dVar34 >> 0x20) - SUB84(dVar26,0)) * fVar19 +
                                  SUB84(dVar26,0)) * (float)((ulonglong)dVar39 >> 0x20) + fVar38,
                                  SUB84(dVar34,0) * SUB84(dVar39,0) + fVar36);
      }
      else {
        dVar34 = (((dVar34 - dVar26) * dVar26 + dVar26) * dVar39 + dVar37) * dVar39 + dVar37;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)PTR_DAT_8c038a88);
      }
      else {
        in_xd8 = *(undefined8 *)PTR_DAT_8c038a88;
      }
      if (in_FPSCR_PR == '\0') {
        fVar19 = SUB84(dVar22,0) * fVar19;
        dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),fVar19);
        iVar42 = (int)fVar19;
      }
      else {
        dVar22 = dVar22 * dVar26;
        iVar42 = (int)dVar22;
      }
      fVar19 = *(float *)((int)pdVar9 + 0xc);
      fVar32 = *(float *)((int)pdVar14 + 0xc);
      uVar15 = (uint)fVar19 >> 8 & 0xff;
      uVar16 = (uint)fVar19 >> 0x10 & 0xff;
      uVar15 = ((uint)fVar19 & 0xff) +
               (int)(short)(iVar42 * (((uint)fVar32 & 0xff) - ((uint)fVar19 & 0xff)) >> 0x10) |
               (uVar15 + (int)(short)(iVar42 * (((uint)fVar32 >> 8 & 0xff) - uVar15) >> 0x10)) *
               0x100 | (uVar16 + (int)(short)(iVar42 * (((uint)fVar32 >> 0x10 & 0xff) - uVar16) >>
                                             0x10)) * 0x10000 |
               (((uint)fVar19 >> 0x18) +
               (int)(short)(iVar42 * (((uint)fVar32 >> 0x18) - ((uint)fVar19 >> 0x18)) >> 0x10)) *
               0x1000000;
      dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),uVar15);
      fVar19 = *(float *)(pdVar9 + 2);
      fVar32 = *(float *)(pdVar14 + 2);
      uVar16 = (uint)fVar19 >> 8 & 0xff;
      uVar17 = (uint)fVar19 >> 0x10 & 0xff;
      uVar16 = ((uint)fVar19 & 0xff) +
               (int)(short)(iVar42 * (((uint)fVar32 & 0xff) - ((uint)fVar19 & 0xff)) >> 0x10) |
               (uVar16 + (int)(short)(iVar42 * (((uint)fVar32 >> 8 & 0xff) - uVar16) >> 0x10)) *
               0x100 | (uVar17 + (int)(short)(iVar42 * (((uint)fVar32 >> 0x10 & 0xff) - uVar17) >>
                                             0x10)) * 0x10000 |
               (((uint)fVar19 >> 0x18) +
               (int)(short)(iVar42 * (((uint)fVar32 >> 0x18) - ((uint)fVar19 >> 0x18)) >> 0x10)) *
               0x1000000;
      pdVar10 = (double *)((int)pdVar14 + -0xc);
      if (in_FPSCR_SZ == '\0') {
        *(uint *)(iVar11 + 0x1c) = uVar16;
        *(uint *)(iVar11 + 0x18) = uVar15;
        *(int *)(iVar11 + 0x14) = (int)((ulonglong)dVar34 >> 0x20);
        pdVar9 = (double *)(iVar11 + 0x10);
        *(int *)pdVar9 = SUB84(dVar34,0);
      }
      else {
        *(ulonglong *)(iVar11 + 0x1c) = CONCAT44(uVar16,unaff_r14);
        *(undefined8 *)(iVar11 + 0x14) = in_xd8;
        *(double *)(iVar11 + 0xc) = dVar34;
        pdVar9 = (double *)(iVar11 + 4);
        *pdVar9 = in_xd4;
      }
      dVar28 = (double)CONCAT44(0x3f800000,SUB84(dVar28,0));
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)((int)pdVar9 + -4) = 0x3f800000;
      }
      else {
        pdVar9[-1] = dVar28;
      }
      iVar11 = iVar11 + 0x28;
      iVar12 = iVar12 + 1;
    }
    dVar26 = (double)CONCAT44(0x3f800000,SUB84(dVar26,0));
    if (in_FPSCR_SZ == '\0') {
      dVar33 = (double)CONCAT44(*(float *)pdVar10,SUB84(dVar33,0));
    }
    else {
      dVar33 = *pdVar10;
    }
    if (in_FPSCR_PR == '\0') {
      bVar41 = 1.0 < (float)((ulonglong)dVar33 >> 0x20);
    }
    else {
      bVar41 = dVar26 < dVar33;
    }
    if (bVar41) {
      if (in_FPSCR_SZ == '\0') {
        uVar30 = *(undefined4 *)((int)pdVar10 + 0xc);
        dVar28 = (double)CONCAT44(uVar30,SUB84(dVar28,0));
        uVar25 = *(undefined4 *)(pdVar10 + 2);
        fVar19 = *(float *)((int)pdVar10 + 0x14);
        dVar34 = pdVar10[2];
        fVar32 = *(float *)(pdVar10 + 3);
        dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),fVar32);
        *(float *)(iVar11 + 0x1c) = *(float *)((int)pdVar10 + 0x1c);
        *(float *)(iVar11 + 0x18) = fVar32;
        *(float *)(iVar11 + 0x14) = fVar19;
        *(undefined4 *)(iVar11 + 0x10) = uVar25;
        *(undefined4 *)(iVar11 + 0xc) = uVar30;
      }
      else {
        dVar28 = *(double *)((int)pdVar10 + 0xc);
        in_xd4 = *(double *)((int)pdVar10 + 0x14);
        dVar34 = *(double *)((int)pdVar10 + 0x1c);
        in_xd8 = *(undefined8 *)((int)pdVar10 + 0x24);
        *(undefined8 *)(iVar11 + 0x1c) = *(undefined8 *)((int)pdVar10 + 0x2c);
        *(undefined8 *)(iVar11 + 0x14) = in_xd8;
        *(double *)(iVar11 + 0xc) = dVar34;
        *(double *)(iVar11 + 4) = in_xd4;
        *(double *)(iVar11 + -4) = dVar28;
      }
      iVar11 = iVar11 + 0x28;
      iVar12 = iVar12 + 1;
    }
    uVar25 = DAT_8c038b2c;
    iVar13 = iVar13 + -1;
    if (iVar13 == 0) break;
    pdVar14 = pdVar10 + 5;
    pdVar9 = pdVar10;
  }
  if (in_FPSCR_SZ == '\0') {
    uVar24 = *(undefined4 *)DAT_8c038b30;
    uVar30 = *(undefined4 *)((int)DAT_8c038b30 + 4);
    uVar21 = *(undefined4 *)(DAT_8c038b30 + 1);
    *(undefined4 *)(unaff_r14 + 0x18) = *(undefined4 *)((int)DAT_8c038b30 + 0xc);
    *(undefined4 *)(unaff_r14 + 0xc) = uVar24;
    *(undefined4 *)(unaff_r14 + 8) = uVar30;
    puVar18 = (undefined8 *)(unaff_r14 + 4);
    *(undefined4 *)puVar18 = uVar21;
  }
  else {
    uVar23 = *DAT_8c038b30;
    uVar40 = DAT_8c038b30[1];
    uVar20 = DAT_8c038b30[2];
    *(undefined8 *)(unaff_r14 + 0x14) = DAT_8c038b30[3];
    *(undefined8 *)(unaff_r14 + 4) = uVar23;
    *(undefined8 *)(unaff_r14 + -4) = uVar40;
    puVar18 = (undefined8 *)(unaff_r14 + -0xc);
    *puVar18 = uVar20;
  }
  *(undefined4 *)((int)puVar18 + -4) = uVar25;
  if (in_FPSCR_SZ == '\0') {
    uVar24 = *(undefined4 *)DAT_8c038b34;
    uVar30 = *(undefined4 *)((int)DAT_8c038b34 + 4);
    uVar21 = *(undefined4 *)(DAT_8c038b34 + 1);
    *(undefined4 *)((int)puVar18 + 0x34) = *(undefined4 *)((int)DAT_8c038b34 + 0xc);
    *(undefined4 *)(puVar18 + 5) = uVar24;
    *(undefined4 *)((int)puVar18 + 0x24) = uVar30;
    puVar18 = puVar18 + 4;
    *(undefined4 *)puVar18 = uVar21;
  }
  else {
    uVar23 = *DAT_8c038b34;
    uVar40 = DAT_8c038b34[1];
    uVar20 = DAT_8c038b34[2];
    puVar18[6] = DAT_8c038b34[3];
    puVar18[4] = uVar23;
    puVar18[3] = uVar40;
    puVar18 = puVar18 + 2;
    *puVar18 = uVar20;
  }
  *(undefined4 *)((int)puVar18 + -4) = uVar25;
  if (iVar12 != 3) {
    if (in_FPSCR_SZ == '\0') {
      uVar24 = *(undefined4 *)DAT_8c038b38;
      uVar30 = *(undefined4 *)((int)DAT_8c038b38 + 4);
      uVar21 = *(undefined4 *)(DAT_8c038b38 + 1);
      *(undefined4 *)((int)puVar18 + 0x34) = *(undefined4 *)((int)DAT_8c038b38 + 0xc);
      *(undefined4 *)(puVar18 + 5) = uVar24;
      *(undefined4 *)((int)puVar18 + 0x24) = uVar30;
      puVar18 = puVar18 + 4;
      *(undefined4 *)puVar18 = uVar21;
    }
    else {
      uVar23 = *DAT_8c038b38;
      uVar40 = DAT_8c038b38[1];
      uVar20 = DAT_8c038b38[2];
      puVar18[6] = DAT_8c038b38[3];
      puVar18[4] = uVar23;
      puVar18[3] = uVar40;
      puVar18 = puVar18 + 2;
      *puVar18 = uVar20;
    }
    *(undefined4 *)((int)puVar18 + -4) = uVar25;
  }
  uVar25 = DAT_8c038b40;
  if (in_FPSCR_SZ == '\0') {
    uVar30 = *(undefined4 *)DAT_8c038b3c;
    uVar20 = *(undefined8 *)((int)DAT_8c038b3c + 4);
    *(undefined4 *)((int)puVar18 + 0x34) = *(undefined4 *)((int)DAT_8c038b3c + 0xc);
    *(undefined4 *)(puVar18 + 5) = uVar30;
    *(int *)((int)puVar18 + 0x24) = (int)uVar20;
    puVar18 = puVar18 + 4;
    *(int *)puVar18 = (int)((ulonglong)uVar20 >> 0x20);
  }
  else {
    uVar23 = *DAT_8c038b3c;
    uVar40 = DAT_8c038b3c[1];
    uVar20 = DAT_8c038b3c[2];
    puVar18[6] = DAT_8c038b3c[3];
    puVar18[4] = uVar23;
    puVar18[3] = uVar40;
    puVar18 = puVar18 + 2;
    *puVar18 = uVar20;
  }
  *(undefined4 *)((int)puVar18 + -4) = uVar25;
  return CONCAT44(in_r1,(uint)(iVar12 == 3) | uVar1 << 1 | uVar2 << 4 | (uint)bVar7 << 8 |
                        uVar3 << 9 | uVar4 << 0xf | (uint)bVar5 << 0x1c | (uint)bVar6 << 0x1e) |
         0x20000000;
}

