/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c612482) */
/* WARNING: Removing unreachable block (ram,0x8c612444) */
/* WARNING: Removing unreachable block (ram,0x8c61235a) */
/* WARNING: Removing unreachable block (ram,0x8c61231c) */
/* WARNING: Removing unreachable block (ram,0x8c6122de) */
/* WARNING: Removing unreachable block (ram,0x8c612216) */
/* WARNING: Removing unreachable block (ram,0x8c6122ea) */
/* WARNING: Removing unreachable block (ram,0x8c612328) */
/* WARNING: Removing unreachable block (ram,0x8c612366) */
/* WARNING: Removing unreachable block (ram,0x8c612450) */
/* WARNING: Removing unreachable block (ram,0x8c61248e) */
/* WARNING: Removing unreachable block (ram,0x8c612232) */
/* WARNING: Removing unreachable block (ram,0x8c612404) */
/* WARNING: Removing unreachable block (ram,0x8c6123c6) */
/* WARNING: Removing unreachable block (ram,0x8c6123d2) */
/* WARNING: Removing unreachable block (ram,0x8c612410) */
/* WARNING: Restarted to delay deadcode elimination for space: stack */

void FUN_8c611f24(double param_1,undefined4 param_2,undefined4 param_3,int param_4,
                 undefined4 param_5,undefined4 param_6)

{
  int iVar1;
  int iVar2;
  short *psVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int *piVar7;
  int *piVar8;
  double *pdVar9;
  float *pfVar10;
  int iVar11;
  int unaff_r8;
  int *piVar12;
  int iVar13;
  double *pdVar14;
  double *pdVar15;
  double in_dr0;
  double dVar16;
  undefined8 uVar17;
  double dVar18;
  float fVar19;
  float fVar20;
  undefined4 uVar21;
  double in_dr2;
  double dVar22;
  ulonglong uVar23;
  double dVar24;
  undefined4 uVar26;
  double dVar25;
  float fVar27;
  uint uVar28;
  double dVar29;
  double dVar30;
  double in_xd0;
  double in_xd2;
  double dVar31;
  double in_xd4;
  double in_xd6;
  bool bVar32;
  short sVar33;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  int local_6c;
  int local_68;
  short local_54 [6];
  undefined1 local_48 [8];
  undefined8 uStack_40;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined4 local_30;
  undefined4 uStack_2c;
  undefined4 uStack_28;
  int local_24 [3];
  
  piVar7 = DAT_8c612064._4_4_;
  iVar5 = DAT_8c612060;
  dVar29 = 5.26354424712089e-315;
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)DAT_8c61205c);
    param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),0x3f800000);
  }
  else {
    in_xd2 = *DAT_8c61205c;
    in_xd4 = in_xd6;
  }
  if (in_FPSCR_PR == '\0') {
    fVar27 = SUB84(param_1,0) / SUB84(in_dr2,0);
  }
  else {
    fVar27 = SUB84(param_1 / in_dr2,0);
  }
  local_24[0] = param_4;
  local_24[1] = param_5;
  local_24[2] = param_6;
  local_6c = 0;
  piVar12 = local_24;
  pdVar14 = (double *)local_48;
  pdVar15 = (double *)local_48;
  if (in_FPSCR_SZ == '\0') {
    dVar25 = (double)CONCAT44((undefined4)DAT_8c612064,fVar27);
  }
  else {
    dVar25 = (double)CONCAT44(DAT_8c612064._4_4_,(undefined4)DAT_8c612064);
  }
  do {
    piVar8 = local_24 + local_6c;
    pdVar9 = (double *)*piVar8;
    if (in_FPSCR_SZ == '\0') {
      dVar22 = (double)CONCAT44(*(undefined4 *)DAT_8c61206c,*(undefined4 *)pdVar9);
    }
    else {
      dVar22 = *DAT_8c61206c;
      in_xd2 = *pdVar9;
    }
    if (in_FPSCR_PR == '\0') {
      dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) * SUB84(dVar22,0),SUB84(dVar22,0)
                               );
    }
    else {
      dVar22 = dVar22 * dVar22;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)DAT_8c612070);
      in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),*(undefined4 *)DAT_8c612074);
    }
    else {
      in_xd2 = *DAT_8c612070;
      in_xd0 = *DAT_8c612074;
    }
    if (in_FPSCR_PR == '\0') {
      fVar27 = SUB84(dVar22,0) - (float)((ulonglong)dVar22 >> 0x20);
    }
    else {
      fVar27 = SUB84(dVar22 - dVar22,0);
    }
    if (in_FPSCR_SZ == '\0') {
      dVar22 = (double)CONCAT44(*(undefined4 *)((int)pdVar9 + 4),fVar27);
    }
    else {
      dVar22 = *(double *)((int)pdVar9 + 4);
    }
    if (in_FPSCR_PR == '\0') {
      dVar16 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                SUB84(in_dr0,0) * (float)((ulonglong)dVar22 >> 0x20));
    }
    else {
      dVar16 = in_dr0 * dVar22;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar22 = (double)CONCAT44(*(undefined4 *)(pdVar9 + 1),SUB84(dVar22,0));
    }
    else {
      dVar22 = pdVar9[1];
    }
    if (in_FPSCR_PR == '\0') {
      dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),SUB84(dVar22,0) - SUB84(dVar16,0));
    }
    else {
      dVar22 = dVar22 - dVar16;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),*(undefined4 *)(DAT_8c61206c + 1));
    }
    else {
      in_xd0 = DAT_8c61206c[1];
    }
    if (in_FPSCR_PR == '\0') {
      fVar27 = SUB84(dVar16,0) * (float)((ulonglong)dVar22 >> 0x20);
      fVar19 = SUB84(dVar22,0) - fVar27;
    }
    else {
      fVar27 = SUB84(dVar16 * dVar22,0);
      fVar19 = SUB84(dVar22 - dVar16 * dVar22,0);
    }
    if (in_FPSCR_SZ == '\0') {
      *(float *)((int)pdVar9 + 0x1c) = fVar19;
    }
    else {
      *(double *)((int)pdVar9 + 0x1c) = in_xd2;
    }
    iVar2 = DAT_8c61238c;
    pfVar10 = (float *)*piVar8;
    pdVar9 = (double *)*piVar7;
    if (in_FPSCR_SZ == '\0') {
      dVar22 = (double)CONCAT44(pfVar10[1],fVar27);
      dVar16 = (double)CONCAT44(*(undefined4 *)pdVar9,*(undefined4 *)(pdVar9 + 2));
    }
    else {
      dVar16 = *pdVar9;
      dVar22 = *(double *)(pfVar10 + 1);
      in_xd2 = pdVar9[2];
    }
    if (in_FPSCR_PR == '\0') {
      dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),
                                SUB84(dVar16,0) * (float)((ulonglong)dVar22 >> 0x20));
    }
    else {
      dVar16 = dVar16 * dVar22;
    }
    if (in_FPSCR_SZ == '\0') {
      fVar27 = *pfVar10;
    }
    else {
      fVar27 = (float)((ulonglong)*(undefined8 *)pfVar10 >> 0x20);
    }
    if (in_FPSCR_PR == '\0') {
      dVar16 = (double)(ulonglong)
                       (uint)(fVar27 * (float)((ulonglong)dVar16 >> 0x20) + SUB84(dVar16,0));
    }
    if (in_FPSCR_SZ == '\0') {
      fVar27 = pfVar10[2];
      dVar22 = (double)CONCAT44(*(undefined4 *)(pdVar9 + 4),SUB84(dVar16,0));
    }
    else {
      fVar27 = (float)((ulonglong)*(undefined8 *)(pfVar10 + 2) >> 0x20);
      dVar22 = pdVar9[4];
    }
    if (in_FPSCR_PR == '\0') {
      dVar22 = (double)(ulonglong)
                       (uint)(fVar27 * (float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0));
    }
    uVar28 = (uint)((ulonglong)dVar25 >> 0x20);
    dVar30 = dVar25;
    dVar16 = dVar25;
    if (in_FPSCR_SZ == '\0') {
      dVar30 = (double)CONCAT44(uVar28,SUB84(dVar22,0));
      dVar16 = (double)((ulonglong)uVar28 << 0x20);
    }
    fVar27 = (float)((ulonglong)dVar30 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      fVar27 = (float)((ulonglong)dVar16 >> 0x20) * SUB84(dVar30,0) + fVar27;
      dVar30 = (double)CONCAT44(fVar27,SUB84(dVar30,0));
      sVar33 = (short)(int)fVar27;
    }
    else {
      sVar33 = (short)(int)dVar30;
    }
    local_54[local_6c * 2] = sVar33;
    iVar6 = *piVar7;
    if (in_FPSCR_SZ == '\0') {
      dVar22 = (double)CONCAT44(pfVar10[1],*(undefined4 *)(iVar6 + 0x24));
      dVar30 = (double)CONCAT44(fVar27,*(undefined4 *)(iVar6 + 0x14));
    }
    else {
      in_xd0 = *(double *)(iVar6 + 0x24);
      dVar22 = *(double *)(pfVar10 + 1);
      in_xd2 = *(double *)(iVar6 + 0x14);
    }
    if (in_FPSCR_PR == '\0') {
      fVar27 = SUB84(dVar30,0) * (float)((ulonglong)dVar22 >> 0x20);
    }
    else {
      fVar27 = SUB84(dVar30 * dVar22,0);
    }
    if (in_FPSCR_SZ == '\0') {
      uVar17 = CONCAT44(*pfVar10,SUB84(dVar22,0));
      uVar23 = CONCAT44(*(undefined4 *)(iVar6 + 4),fVar27);
    }
    else {
      uVar17 = *(undefined8 *)pfVar10;
      uVar23 = *(ulonglong *)(iVar6 + 4);
    }
    if (in_FPSCR_PR == '\0') {
      uVar23 = (ulonglong)
               (uint)((float)((ulonglong)uVar17 >> 0x20) * (float)(uVar23 >> 0x20) + (float)uVar23);
    }
    if (in_FPSCR_SZ == '\0') {
      uVar17 = CONCAT44(pfVar10[2],(int)uVar17);
      dVar22 = (double)CONCAT44(uVar28,(int)uVar23);
    }
    else {
      uVar17 = *(undefined8 *)(pfVar10 + 2);
      dVar22 = dVar25;
    }
    if (in_FPSCR_PR == '\0') {
      dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                (float)((ulonglong)uVar17 >> 0x20) * (float)uVar17 + SUB84(dVar22,0)
                               );
    }
    in_dr0 = dVar25;
    if (in_FPSCR_SZ == '\0') {
      in_dr0 = (double)CONCAT44(uVar28,(float)uVar17);
    }
    fVar27 = (float)((ulonglong)dVar22 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      fVar27 = (float)((ulonglong)in_dr0 >> 0x20) * SUB84(dVar22,0) + fVar27;
      dVar22 = (double)CONCAT44(fVar27,SUB84(dVar22,0));
      sVar33 = (short)(int)fVar27;
    }
    else {
      sVar33 = (short)(int)dVar22;
    }
    local_54[local_6c * 2 + 1] = sVar33;
    if ((*(uint *)(*piVar8 + 0xc) & 1) == 0) {
      dVar16 = in_xd6;
      if (in_FPSCR_SZ == '\0') {
        dVar16 = (double)CONCAT44(SUB84(dVar29,0),SUB84(dVar29,0));
      }
      piVar8 = local_24 + local_6c;
      iVar6 = *piVar8;
      if (in_FPSCR_SZ == '\0') {
        dVar22 = (double)CONCAT44(fVar27,*(undefined4 *)(iVar6 + 0x18));
      }
      else {
        in_xd2 = *(double *)(iVar6 + 0x18);
      }
      if (in_FPSCR_PR == '\0') {
        dVar29 = (double)CONCAT44((float)((ulonglong)dVar16 >> 0x20) / SUB84(dVar22,0),
                                  SUB84(dVar16,0));
      }
      else {
        dVar29 = dVar16 / dVar22;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar22 = (double)CONCAT44(*(undefined4 *)(iVar6 + 0x10),*(undefined4 *)(iVar5 + 0xc));
      }
      else {
        in_xd2 = *(double *)(iVar5 + 0xc);
        dVar22 = *(double *)(iVar6 + 0x10);
      }
      if (in_FPSCR_PR == '\0') {
        dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) - SUB84(dVar22,0),
                                  SUB84(dVar22,0));
      }
      else {
        dVar22 = dVar22 - dVar22;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)DAT_8c612078);
      }
      else {
        in_xd2 = *DAT_8c612078;
      }
      fVar27 = (float)((ulonglong)dVar29 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar22 = (double)CONCAT44(((float)((ulonglong)dVar22 >> 0x20) * fVar27) / SUB84(dVar22,0),
                                  SUB84(dVar22,0));
      }
      else {
        dVar22 = (dVar22 * dVar29) / (dVar22 * dVar29);
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)pdVar14 = (int)((ulonglong)dVar22 >> 0x20);
        dVar22 = (double)(ulonglong)*(uint *)(iVar5 + 0x10);
      }
      else {
        *pdVar14 = dVar22;
        in_xd2 = *(double *)(iVar5 + 0x10);
      }
      iVar6 = *piVar8;
      if (in_FPSCR_SZ == '\0') {
        dVar22 = (double)CONCAT44(*(undefined4 *)(iVar6 + 0x14),SUB84(dVar22,0));
      }
      else {
        dVar22 = *(double *)(iVar6 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) - SUB84(dVar22,0),
                                  SUB84(dVar22,0));
      }
      else {
        dVar22 = dVar22 - dVar22;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)DAT_8c61207c);
      }
      else {
        in_xd2 = *DAT_8c61207c;
      }
      if (in_FPSCR_PR == '\0') {
        dVar22 = (double)CONCAT44(((float)((ulonglong)dVar22 >> 0x20) * fVar27) / SUB84(dVar22,0),
                                  SUB84(dVar22,0));
      }
      else {
        dVar22 = (dVar22 * dVar29) / (dVar22 * dVar29);
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar14 + 4) = (int)((ulonglong)dVar22 >> 0x20);
        *(float *)(pdVar14 + 1) = fVar27;
      }
      else {
        *(double *)((int)pdVar14 + 4) = dVar22;
        pdVar14[1] = dVar29;
      }
    }
    else {
      if (in_FPSCR_SZ == '\0') {
        uVar21 = *(undefined4 *)(*piVar12 + 0x10);
        dVar22 = (double)CONCAT44(fVar27,uVar21);
        *(undefined4 *)pdVar15 = uVar21;
      }
      else {
        in_xd2 = *(double *)(*piVar12 + 0x10);
        *pdVar15 = in_xd2;
      }
      if (in_FPSCR_SZ == '\0') {
        uVar21 = *(undefined4 *)(*piVar12 + 0x14);
        dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),uVar21);
        *(undefined4 *)((int)pdVar15 + 4) = uVar21;
      }
      else {
        in_xd2 = *(double *)(*piVar12 + 0x14);
        *(double *)((int)pdVar15 + 4) = in_xd2;
      }
      if (in_FPSCR_SZ == '\0') {
        uVar21 = *(undefined4 *)(*piVar12 + 0x18);
        dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),uVar21);
        *(undefined4 *)(pdVar15 + 1) = uVar21;
      }
      else {
        in_xd2 = *(double *)(*piVar12 + 0x18);
        pdVar15[1] = in_xd2;
      }
    }
    pdVar15 = (double *)((int)pdVar15 + 0xc);
    local_6c = local_6c + 1;
    piVar12 = piVar12 + 1;
    pdVar14 = (double *)((int)pdVar14 + 0xc);
  } while (local_6c < 3);
  if (in_FPSCR_SZ == '\0') {
    dVar25 = (double)CONCAT44(uStack_38,SUB84(dVar25,0));
    dVar29 = (double)CONCAT44(uStack_34,(undefined4)uStack_40);
    dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),local_48._4_4_);
    dVar16 = (double)CONCAT44((undefined4)uStack_40,SUB84(in_dr0,0));
    param_2 = local_48._4_4_;
  }
  else {
    dVar16 = (double)CONCAT44(uStack_40._4_4_,(undefined4)uStack_40);
    dVar25 = (double)CONCAT44(uStack_34,uStack_38);
    in_xd2 = (double)CONCAT44((undefined4)uStack_40,local_48._4_4_);
    dVar29 = (double)CONCAT44(local_30,uStack_34);
    uStack_40._0_4_ = uStack_34;
    in_xd6 = dVar16;
  }
  fVar19 = (float)((ulonglong)dVar29 >> 0x20);
  fVar27 = (float)((ulonglong)dVar25 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    fVar20 = SUB84(dVar22,0) * fVar19;
    dVar16 = (double)CONCAT44((float)((ulonglong)dVar16 >> 0x20) * fVar27,SUB84(dVar16,0));
  }
  else {
    fVar20 = SUB84(dVar22 * dVar29,0);
    dVar16 = dVar16 * dVar25;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar30 = (double)CONCAT44(local_48._0_4_,param_3);
    dVar22 = (double)CONCAT44(uStack_40._4_4_,param_2);
    in_xd6 = (double)CONCAT44((undefined4)uStack_40,fVar20);
  }
  else {
    dVar30 = (double)CONCAT44(local_48._4_4_,local_48._0_4_);
    dVar22 = (double)CONCAT44(uStack_38,uStack_40._4_4_);
  }
  fVar20 = (float)((ulonglong)dVar22 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar24 = (double)CONCAT44((float)((ulonglong)in_xd6 >> 0x20) * fVar20,SUB84(in_xd6,0));
  }
  else {
    dVar24 = in_xd6 * dVar22;
  }
  dVar18 = dVar22;
  if (in_FPSCR_SZ == '\0') {
    dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),fVar20);
    dVar18 = in_xd0;
  }
  if (in_FPSCR_PR == '\0') {
    dVar16 = (double)CONCAT44((float)((ulonglong)dVar16 >> 0x20) - SUB84(dVar24,0),SUB84(dVar16,0));
  }
  else {
    dVar16 = dVar16 - dVar24;
  }
  uVar21 = (undefined4)((ulonglong)dVar30 >> 0x20);
  dVar31 = dVar30;
  if (in_FPSCR_SZ == '\0') {
    dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),uVar21);
    dVar31 = in_xd2;
  }
  if (in_FPSCR_PR == '\0') {
    dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),SUB84(dVar24,0) * fVar19);
  }
  else {
    dVar24 = dVar24 * dVar29;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar30 = (double)CONCAT44(uVar21,uVar21);
  }
  if (in_FPSCR_PR == '\0') {
    dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),SUB84(dVar16,0) * SUB84(dVar22,0));
    fVar19 = SUB84(dVar24,0) - (float)((ulonglong)dVar24 >> 0x20);
  }
  else {
    dVar16 = dVar16 * dVar22;
    fVar19 = SUB84(dVar24 - dVar24,0);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar22 = (double)CONCAT44(uStack_2c,fVar19);
  }
  else {
    dVar22 = (double)CONCAT44(uStack_28,uStack_2c);
  }
  if (in_FPSCR_PR == '\0') {
    dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) * SUB84(dVar22,0),SUB84(dVar22,0));
  }
  else {
    dVar22 = dVar22 * dVar22;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),local_30);
  }
  else {
    dVar31 = (double)CONCAT44(uStack_2c,local_30);
  }
  if (in_FPSCR_PR == '\0') {
    dVar22 = (double)CONCAT44((float)((ulonglong)dVar16 >> 0x20) * SUB84(dVar22,0) +
                              (float)((ulonglong)dVar22 >> 0x20),SUB84(dVar22,0));
    dVar30 = (double)CONCAT44((int)((ulonglong)dVar30 >> 0x20),SUB84(dVar30,0) * fVar27);
  }
  else {
    dVar30 = dVar30 * dVar25;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),uStack_28);
  }
  else {
    dVar31 = (double)CONCAT44(local_24[0],uStack_28);
  }
  if (in_FPSCR_PR == '\0') {
    fVar27 = SUB84(dVar16,0) - SUB84(dVar30,0);
  }
  else {
    fVar27 = SUB84(dVar16 - dVar30,0);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar18 = (double)CONCAT44(fVar27,fVar27);
  }
  if (in_FPSCR_PR == '\0') {
    dVar22 = (double)((ulonglong)
                      (uint)((float)((ulonglong)dVar18 >> 0x20) * SUB84(dVar22,0) +
                            (float)((ulonglong)dVar22 >> 0x20)) << 0x20);
  }
  dVar30 = (double)((ulonglong)dVar22 & 0xffffffff00000000);
  dVar16 = dVar30;
  if (in_FPSCR_SZ == '\0') {
    dVar16 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),SUB84(dVar25,0));
  }
  if (in_FPSCR_PR == '\0') {
    bVar32 = (float)((ulonglong)dVar16 >> 0x20) < 0.0;
  }
  else {
    bVar32 = dVar16 < dVar30;
  }
  if (((uint)bVar32 & *DAT_8c612388) == 0) {
    *DAT_8c612388 = 0;
    psVar3 = DAT_8c612390;
    iVar11 = 0;
    local_68 = 2;
    iVar6 = 0;
    do {
      iVar13 = iVar6;
      if ((*(uint *)(local_24[local_68] + 0xc) & 1 ^ *(uint *)(local_24[iVar13] + 0xc) & 1) == 1) {
        iVar6 = local_68 * 0xc;
        pdVar14 = (double *)(iVar6 + (int)local_48);
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(*(undefined4 *)((int)&uStack_40 + iVar6),SUB84(dVar16,0));
          dVar30 = (double)CONCAT44((int)((ulonglong)dVar30 >> 0x20),*(undefined4 *)DAT_8c612394);
        }
        else {
          dVar25 = *(double *)((int)&uStack_40 + iVar6);
          dVar31 = *DAT_8c612394;
        }
        iVar1 = iVar13 * 0xc;
        fVar27 = (float)((ulonglong)dVar25 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          fVar19 = SUB84(dVar30,0) - fVar27;
        }
        else {
          fVar19 = SUB84(dVar30 - dVar25,0);
        }
        pdVar15 = (double *)(iVar1 + (int)local_48);
        if (in_FPSCR_SZ == '\0') {
          dVar22 = (double)CONCAT44(*(undefined4 *)((int)&uStack_40 + iVar1),fVar19);
        }
        else {
          dVar22 = *(double *)((int)&uStack_40 + iVar1);
        }
        if (in_FPSCR_PR == '\0') {
          dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) - fVar27,SUB84(dVar22,0));
        }
        else {
          dVar22 = dVar22 - dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          uVar21 = SUB84(dVar22,0);
          dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)pdVar15);
          dVar30 = (double)CONCAT44(*(undefined4 *)pdVar14,SUB84(dVar29,0));
          dVar29 = dVar31;
          dVar31 = (double)CONCAT44(uVar21,SUB84(dVar25,0));
        }
        else {
          dVar29 = *pdVar15;
          dVar30 = *pdVar14;
        }
        fVar27 = (float)((ulonglong)dVar30 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar16 = (double)CONCAT44((float)((ulonglong)dVar31 >> 0x20) /
                                    (float)((ulonglong)dVar22 >> 0x20),SUB84(dVar31,0));
          fVar19 = SUB84(dVar22,0) - fVar27;
        }
        else {
          dVar16 = dVar31 / dVar22;
          fVar19 = SUB84(dVar22 - dVar30,0);
        }
        uVar21 = (undefined4)((ulonglong)dVar16 >> 0x20);
        dVar22 = dVar16;
        dVar25 = dVar30;
        if (in_FPSCR_SZ == '\0') {
          dVar22 = (double)CONCAT44(uVar21,SUB84(dVar18,0));
          dVar25 = (double)CONCAT44(fVar27,fVar19);
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) * SUB84(dVar25,0) +
                                    (float)((ulonglong)dVar25 >> 0x20),SUB84(dVar25,0));
        }
        dVar24 = in_xd4;
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),SUB84(dVar30,0));
          dVar24 = (double)CONCAT44(SUB84(dVar16,0),SUB84(dVar22,0));
        }
        iVar4 = iVar11 * 0x20 + iVar2;
        if (in_FPSCR_SZ == '\0') {
          dVar22 = (double)CONCAT44(*(undefined4 *)(iVar5 + 0xc),*(undefined4 *)DAT_8c612398);
        }
        else {
          dVar29 = *DAT_8c612398;
          dVar22 = *(double *)(iVar5 + 0xc);
        }
        if (in_FPSCR_PR == '\0') {
          fVar27 = SUB84(dVar22,0) * (float)((ulonglong)dVar25 >> 0x20);
          dVar22 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * fVar27 +
                                    (float)((ulonglong)dVar22 >> 0x20),fVar27);
        }
        else {
          dVar22 = dVar22 * dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(iVar4 + 0x10) = (int)((ulonglong)dVar22 >> 0x20);
          fVar27 = *(float *)(local_48 + iVar6 + 4);
          dVar30 = (double)CONCAT44(fVar27,*(undefined4 *)(local_48 + iVar1 + 4));
          dVar25 = (double)CONCAT44(fVar27,SUB84(dVar25,0));
          dVar22 = (double)CONCAT44(uVar21,SUB84(dVar24,0));
        }
        else {
          *(double *)(iVar4 + 0x10) = dVar22;
          dVar30 = *(double *)(local_48 + iVar6 + 4);
          fVar27 = (float)((ulonglong)dVar30 >> 0x20);
          dVar29 = *(double *)(local_48 + iVar1 + 4);
          dVar25 = dVar30;
          dVar22 = dVar16;
        }
        if (in_FPSCR_PR == '\0') {
          fVar27 = SUB84(dVar30,0) - fVar27;
          dVar30 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) * fVar27 +
                                    (float)((ulonglong)dVar30 >> 0x20),fVar27);
        }
        else {
          dVar30 = dVar30 - dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          uVar26 = (undefined4)((ulonglong)dVar30 >> 0x20);
          dVar30 = (double)CONCAT44(uVar26,SUB84(dVar25,0));
          dVar25 = (double)CONCAT44(uVar26,*(undefined4 *)DAT_8c61239c);
          dVar31 = dVar29;
        }
        else {
          dVar31 = *DAT_8c61239c;
          dVar25 = dVar30;
        }
        if (in_FPSCR_PR == '\0') {
          fVar27 = SUB84(dVar25,0) * (float)((ulonglong)dVar30 >> 0x20);
        }
        else {
          fVar27 = SUB84(dVar25 * dVar30,0);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar17 = CONCAT44(*(undefined4 *)(iVar5 + 0x10),fVar27);
          dVar25 = (double)CONCAT44(SUB84(dVar16,0),SUB84(dVar22,0));
        }
        else {
          uVar17 = *(undefined8 *)(iVar5 + 0x10);
          dVar25 = in_xd4;
        }
        fVar27 = (float)((ulonglong)dVar25 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          uVar17 = CONCAT44(fVar27 * (float)uVar17 + (float)((ulonglong)uVar17 >> 0x20),
                            (float)uVar17);
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(iVar4 + 0x14) = (int)((ulonglong)uVar17 >> 0x20);
          *(float *)(iVar4 + 0x18) = fVar27;
        }
        else {
          *(undefined8 *)(iVar4 + 0x14) = uVar17;
          *(double *)(iVar4 + 0x18) = dVar25;
        }
        iVar6 = local_24[local_68];
        if (in_FPSCR_SZ == '\0') {
          dVar29 = (double)CONCAT44(*(undefined4 *)(iVar6 + 0x1c),SUB84(dVar30,0));
          dVar18 = (double)CONCAT44(uVar21,SUB84(dVar25,0));
        }
        else {
          dVar29 = *(double *)(iVar6 + 0x1c);
          dVar18 = dVar16;
        }
        iVar6 = local_24[iVar13];
        fVar27 = (float)((ulonglong)dVar29 >> 0x20);
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(fVar27,*(undefined4 *)(iVar6 + 0x1c));
        }
        else {
          dVar31 = *(double *)(iVar6 + 0x1c);
          dVar25 = dVar29;
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),SUB84(dVar25,0) - fVar27);
        }
        else {
          dVar25 = dVar25 - dVar29;
        }
        fVar27 = (float)((ulonglong)dVar18 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44(fVar27 * SUB84(dVar25,0) + (float)((ulonglong)dVar25 >> 0x20),
                                    SUB84(dVar25,0));
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(iVar4 + 0x1c) = (int)((ulonglong)dVar25 >> 0x20);
        }
        else {
          *(double *)(iVar4 + 0x1c) = dVar25;
        }
        iVar6 = (int)local_54[iVar13 * 2] - (int)local_54[local_68 * 2];
        if (in_FPSCR_PR == '\0') {
          sVar33 = (short)(int)(fVar27 * (float)iVar6 + (float)(int)local_54[local_68 * 2]);
        }
        else {
          sVar33 = (short)(int)(double)iVar6;
        }
        psVar3[iVar11 * 2] = sVar33;
        iVar6 = (int)local_54[iVar13 * 2 + 1] - (int)local_54[local_68 * 2 + 1];
        if (in_FPSCR_PR == '\0') {
          fVar19 = (float)iVar6;
          fVar27 = fVar27 * fVar19 + (float)(int)local_54[local_68 * 2 + 1];
          dVar30 = (double)CONCAT44(fVar19,fVar27);
          sVar33 = (short)(int)fVar27;
        }
        else {
          dVar30 = (double)iVar6;
          sVar33 = (short)(int)dVar30;
        }
        (psVar3 + iVar11 * 2)[1] = sVar33;
        iVar11 = iVar11 + 1;
      }
      if ((*(uint *)(local_24[iVar13] + 0xc) & 1) == 0) {
        piVar7 = local_24 + iVar13;
        iVar6 = iVar11 * 0x20 + iVar2;
        if (in_FPSCR_SZ == '\0') {
          uVar21 = *(undefined4 *)(*piVar7 + 0x10);
          dVar30 = (double)CONCAT44((int)((ulonglong)dVar30 >> 0x20),uVar21);
          *(undefined4 *)(iVar6 + 0x10) = uVar21;
        }
        else {
          dVar31 = *(double *)(*piVar7 + 0x10);
          *(double *)(iVar6 + 0x10) = dVar31;
        }
        if (in_FPSCR_SZ == '\0') {
          uVar21 = *(undefined4 *)(*piVar7 + 0x14);
          dVar30 = (double)CONCAT44((int)((ulonglong)dVar30 >> 0x20),uVar21);
          *(undefined4 *)(iVar6 + 0x14) = uVar21;
        }
        else {
          dVar31 = *(double *)(*piVar7 + 0x14);
          *(double *)(iVar6 + 0x14) = dVar31;
        }
        if (in_FPSCR_SZ == '\0') {
          uVar21 = *(undefined4 *)(*piVar7 + 0x18);
          dVar30 = (double)CONCAT44((int)((ulonglong)dVar30 >> 0x20),uVar21);
          *(undefined4 *)(iVar6 + 0x18) = uVar21;
        }
        else {
          dVar31 = *(double *)(*piVar7 + 0x18);
          *(double *)(iVar6 + 0x18) = dVar31;
        }
        if (in_FPSCR_SZ == '\0') {
          uVar21 = *(undefined4 *)(*piVar7 + 0x1c);
          dVar30 = (double)CONCAT44((int)((ulonglong)dVar30 >> 0x20),uVar21);
          *(undefined4 *)(iVar6 + 0x1c) = uVar21;
        }
        else {
          dVar31 = *(double *)(*piVar7 + 0x1c);
          *(double *)(iVar6 + 0x1c) = dVar31;
        }
        iVar6 = iVar11 * 2;
        iVar11 = iVar11 + 1;
        psVar3[iVar6] = local_54[iVar13 * 2];
        (psVar3 + iVar6)[1] = local_54[iVar13 * 2 + 1];
      }
      uVar21 = DAT_8c6123a4;
      piVar7 = DAT_8c6123a0;
      iVar6 = iVar13 + 1;
      local_68 = iVar13;
    } while (iVar13 + 1 < 3);
    if (in_FPSCR_SZ == '\0') {
      dVar29 = (double)CONCAT44((undefined4)DAT_8c6123a8,SUB84(dVar16,0));
    }
    else {
      dVar29 = (double)CONCAT44(DAT_8c6123a8._4_4_,(undefined4)DAT_8c6123a8);
    }
    uVar26 = (undefined4)((ulonglong)dVar30 >> 0x20);
    fVar27 = (float)((ulonglong)dVar29 >> 0x20);
    if (iVar11 == 3) {
      if (in_FPSCR_SZ == '\0') {
        dVar30 = (double)CONCAT44(uVar26,*(undefined4 *)(iVar2 + 0x1c));
        pdVar14 = (double *)(unaff_r8 + 0x18);
        *(undefined4 *)pdVar14 = *(undefined4 *)(iVar2 + 0x1c);
      }
      else {
        dVar31 = *(double *)(iVar2 + 0x1c);
        pdVar14 = (double *)(unaff_r8 + 0x14);
        *pdVar14 = dVar31;
      }
      uVar26 = (undefined4)((ulonglong)dVar30 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar25 = (double)CONCAT44(uVar26,(float)(int)psVar3[1] * fVar27);
      }
      else {
        dVar25 = (double)CONCAT44(uVar26,(float)(int)psVar3[1]) * dVar29;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar14 = (double *)((int)pdVar14 + -4);
        *(int *)pdVar14 = SUB84(dVar25,0);
      }
      else {
        pdVar14 = pdVar14 + -1;
        *pdVar14 = dVar31;
      }
      uVar26 = (undefined4)((ulonglong)dVar25 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar25 = (double)CONCAT44(uVar26,(float)(int)*psVar3 * fVar27);
      }
      else {
        dVar25 = (double)CONCAT44(uVar26,(float)(int)*psVar3) * dVar29;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar14 + -4) = SUB84(dVar25,0);
        *(undefined4 *)(pdVar14 + -1) = *(undefined4 *)(iVar2 + 0x18);
        *(undefined4 *)((int)pdVar14 + -0xc) = *(undefined4 *)(iVar2 + 0x14);
        dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)(iVar2 + 0x10));
        pdVar14 = pdVar14 + -2;
        *(undefined4 *)pdVar14 = *(undefined4 *)(iVar2 + 0x10);
      }
      else {
        pdVar14[-1] = dVar31;
        pdVar14[-2] = *(double *)(iVar2 + 0x18);
        pdVar14[-3] = *(double *)(iVar2 + 0x14);
        dVar31 = *(double *)(iVar2 + 0x10);
        pdVar14 = pdVar14 + -4;
        *pdVar14 = dVar31;
      }
      uVar26 = (undefined4)((ulonglong)dVar25 >> 0x20);
      *(undefined4 *)((int)pdVar14 + -4) = uVar21;
      if (in_FPSCR_SZ == '\0') {
        pdVar14 = (double *)((int)pdVar14 + 0x34);
        *(undefined4 *)pdVar14 = *(undefined4 *)(iVar2 + 0x3c);
      }
      else {
        dVar31 = *(double *)(iVar2 + 0x3c);
        pdVar14 = pdVar14 + 6;
        *pdVar14 = dVar31;
      }
      if (in_FPSCR_PR == '\0') {
        dVar25 = (double)CONCAT44(uVar26,(float)(int)psVar3[3] * fVar27);
      }
      else {
        dVar25 = (double)CONCAT44(uVar26,(float)(int)psVar3[3]) * dVar29;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar14 = (double *)((int)pdVar14 + -4);
        *(int *)pdVar14 = SUB84(dVar25,0);
      }
      else {
        pdVar14 = pdVar14 + -1;
        *pdVar14 = dVar31;
      }
      uVar26 = (undefined4)((ulonglong)dVar25 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar25 = (double)CONCAT44(uVar26,(float)(int)psVar3[2] * fVar27);
      }
      else {
        dVar25 = (double)CONCAT44(uVar26,(float)(int)psVar3[2]) * dVar29;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar14 + -4) = SUB84(dVar25,0);
        *(undefined4 *)(pdVar14 + -1) = *(undefined4 *)(iVar2 + 0x38);
        *(undefined4 *)((int)pdVar14 + -0xc) = *(undefined4 *)(iVar2 + 0x34);
        dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)(iVar2 + 0x30));
        pdVar14 = pdVar14 + -2;
        *(undefined4 *)pdVar14 = *(undefined4 *)(iVar2 + 0x30);
      }
      else {
        pdVar14[-1] = dVar31;
        pdVar14[-2] = *(double *)(iVar2 + 0x38);
        pdVar14[-3] = *(double *)(iVar2 + 0x34);
        dVar31 = *(double *)(iVar2 + 0x30);
        pdVar14 = pdVar14 + -4;
        *pdVar14 = dVar31;
      }
      uVar26 = (undefined4)((ulonglong)dVar25 >> 0x20);
      *(undefined4 *)((int)pdVar14 + -4) = uVar21;
      if (in_FPSCR_SZ == '\0') {
        pdVar14 = (double *)((int)pdVar14 + 0x34);
        *(undefined4 *)pdVar14 = *(undefined4 *)(iVar2 + 0x5c);
      }
      else {
        dVar31 = *(double *)(iVar2 + 0x5c);
        pdVar14 = pdVar14 + 6;
        *pdVar14 = dVar31;
      }
      if (in_FPSCR_PR == '\0') {
        dVar25 = (double)CONCAT44(uVar26,(float)(int)psVar3[5] * fVar27);
      }
      else {
        dVar25 = (double)CONCAT44(uVar26,(float)(int)psVar3[5]) * dVar29;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar14 = (double *)((int)pdVar14 + -4);
        *(int *)pdVar14 = SUB84(dVar25,0);
      }
      else {
        pdVar14 = pdVar14 + -1;
        *pdVar14 = dVar31;
      }
      uVar21 = DAT_8c6123a8._4_4_;
      if (in_FPSCR_PR == '\0') {
        fVar27 = (float)(int)psVar3[4] * fVar27;
      }
      else {
        fVar27 = SUB84((double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),(float)(int)psVar3[4]) *
                       dVar29,0);
      }
      if (in_FPSCR_SZ == '\0') {
        *(float *)((int)pdVar14 + -4) = fVar27;
        *(undefined4 *)(pdVar14 + -1) = *(undefined4 *)(iVar2 + 0x58);
        *(undefined4 *)((int)pdVar14 + -0xc) = *(undefined4 *)(iVar2 + 0x54);
        pdVar14 = pdVar14 + -2;
        *(undefined4 *)pdVar14 = *(undefined4 *)(iVar2 + 0x50);
      }
      else {
        pdVar14[-1] = dVar31;
        pdVar14[-2] = *(double *)(iVar2 + 0x58);
        pdVar14[-3] = *(double *)(iVar2 + 0x54);
        pdVar14 = pdVar14 + -4;
        *pdVar14 = *(double *)(iVar2 + 0x50);
      }
      *(undefined4 *)((int)pdVar14 + -4) = uVar21;
      iVar5 = *piVar7 + 3;
    }
    else {
      if (in_FPSCR_SZ == '\0') {
        dVar30 = (double)CONCAT44(uVar26,*(undefined4 *)(iVar2 + 0x1c));
        pdVar14 = (double *)(unaff_r8 + 0x18);
        *(undefined4 *)pdVar14 = *(undefined4 *)(iVar2 + 0x1c);
      }
      else {
        dVar31 = *(double *)(iVar2 + 0x1c);
        pdVar14 = (double *)(unaff_r8 + 0x14);
        *pdVar14 = dVar31;
      }
      uVar26 = (undefined4)((ulonglong)dVar30 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar25 = (double)CONCAT44(uVar26,(float)(int)psVar3[1] * fVar27);
      }
      else {
        dVar25 = (double)CONCAT44(uVar26,(float)(int)psVar3[1]) * dVar29;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar14 = (double *)((int)pdVar14 + -4);
        *(int *)pdVar14 = SUB84(dVar25,0);
      }
      else {
        pdVar14 = pdVar14 + -1;
        *pdVar14 = dVar31;
      }
      uVar26 = (undefined4)((ulonglong)dVar25 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar25 = (double)CONCAT44(uVar26,(float)(int)*psVar3 * fVar27);
      }
      else {
        dVar25 = (double)CONCAT44(uVar26,(float)(int)*psVar3) * dVar29;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar14 + -4) = SUB84(dVar25,0);
        *(undefined4 *)(pdVar14 + -1) = *(undefined4 *)(iVar2 + 0x18);
        *(undefined4 *)((int)pdVar14 + -0xc) = *(undefined4 *)(iVar2 + 0x14);
        dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)(iVar2 + 0x10));
        pdVar14 = pdVar14 + -2;
        *(undefined4 *)pdVar14 = *(undefined4 *)(iVar2 + 0x10);
      }
      else {
        pdVar14[-1] = dVar31;
        pdVar14[-2] = *(double *)(iVar2 + 0x18);
        pdVar14[-3] = *(double *)(iVar2 + 0x14);
        dVar31 = *(double *)(iVar2 + 0x10);
        pdVar14 = pdVar14 + -4;
        *pdVar14 = dVar31;
      }
      uVar26 = (undefined4)((ulonglong)dVar25 >> 0x20);
      *(undefined4 *)((int)pdVar14 + -4) = uVar21;
      if (in_FPSCR_SZ == '\0') {
        pdVar14 = (double *)((int)pdVar14 + 0x34);
        *(undefined4 *)pdVar14 = *(undefined4 *)(iVar2 + 0x3c);
      }
      else {
        dVar31 = *(double *)(iVar2 + 0x3c);
        pdVar14 = pdVar14 + 6;
        *pdVar14 = dVar31;
      }
      if (in_FPSCR_PR == '\0') {
        dVar25 = (double)CONCAT44(uVar26,(float)(int)psVar3[3] * fVar27);
      }
      else {
        dVar25 = (double)CONCAT44(uVar26,(float)(int)psVar3[3]) * dVar29;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar14 = (double *)((int)pdVar14 + -4);
        *(int *)pdVar14 = SUB84(dVar25,0);
      }
      else {
        pdVar14 = pdVar14 + -1;
        *pdVar14 = dVar31;
      }
      uVar26 = (undefined4)((ulonglong)dVar25 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar25 = (double)CONCAT44(uVar26,(float)(int)psVar3[2] * fVar27);
      }
      else {
        dVar25 = (double)CONCAT44(uVar26,(float)(int)psVar3[2]) * dVar29;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar14 + -4) = SUB84(dVar25,0);
        *(undefined4 *)(pdVar14 + -1) = *(undefined4 *)(iVar2 + 0x38);
        *(undefined4 *)((int)pdVar14 + -0xc) = *(undefined4 *)(iVar2 + 0x34);
        dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)(iVar2 + 0x30));
        pdVar14 = pdVar14 + -2;
        *(undefined4 *)pdVar14 = *(undefined4 *)(iVar2 + 0x30);
      }
      else {
        pdVar14[-1] = dVar31;
        pdVar14[-2] = *(double *)(iVar2 + 0x38);
        pdVar14[-3] = *(double *)(iVar2 + 0x34);
        dVar31 = *(double *)(iVar2 + 0x30);
        pdVar14 = pdVar14 + -4;
        *pdVar14 = dVar31;
      }
      uVar26 = (undefined4)((ulonglong)dVar25 >> 0x20);
      *(undefined4 *)((int)pdVar14 + -4) = uVar21;
      if (in_FPSCR_SZ == '\0') {
        pdVar14 = (double *)((int)pdVar14 + 0x34);
        *(undefined4 *)pdVar14 = *(undefined4 *)(iVar2 + 0x7c);
      }
      else {
        dVar31 = *(double *)(iVar2 + 0x7c);
        pdVar14 = pdVar14 + 6;
        *pdVar14 = dVar31;
      }
      if (in_FPSCR_PR == '\0') {
        dVar25 = (double)CONCAT44(uVar26,(float)(int)psVar3[7] * fVar27);
      }
      else {
        dVar25 = (double)CONCAT44(uVar26,(float)(int)psVar3[7]) * dVar29;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar14 = (double *)((int)pdVar14 + -4);
        *(int *)pdVar14 = SUB84(dVar25,0);
      }
      else {
        pdVar14 = pdVar14 + -1;
        *pdVar14 = dVar31;
      }
      uVar26 = (undefined4)((ulonglong)dVar25 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar25 = (double)CONCAT44(uVar26,(float)(int)psVar3[6] * fVar27);
      }
      else {
        dVar25 = (double)CONCAT44(uVar26,(float)(int)psVar3[6]) * dVar29;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar14 + -4) = SUB84(dVar25,0);
        *(undefined4 *)(pdVar14 + -1) = *(undefined4 *)(iVar2 + 0x78);
        *(undefined4 *)((int)pdVar14 + -0xc) = *(undefined4 *)(iVar2 + 0x74);
        dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)(iVar2 + 0x70));
        pdVar14 = pdVar14 + -2;
        *(undefined4 *)pdVar14 = *(undefined4 *)(iVar2 + 0x70);
      }
      else {
        pdVar14[-1] = dVar31;
        pdVar14[-2] = *(double *)(iVar2 + 0x78);
        pdVar14[-3] = *(double *)(iVar2 + 0x74);
        dVar31 = *(double *)(iVar2 + 0x70);
        pdVar14 = pdVar14 + -4;
        *pdVar14 = dVar31;
      }
      uVar26 = (undefined4)((ulonglong)dVar25 >> 0x20);
      *(undefined4 *)((int)pdVar14 + -4) = uVar21;
      if (in_FPSCR_SZ == '\0') {
        pdVar14 = (double *)((int)pdVar14 + 0x34);
        *(undefined4 *)pdVar14 = *(undefined4 *)(iVar2 + 0x5c);
      }
      else {
        dVar31 = *(double *)(iVar2 + 0x5c);
        pdVar14 = pdVar14 + 6;
        *pdVar14 = dVar31;
      }
      if (in_FPSCR_PR == '\0') {
        dVar25 = (double)CONCAT44(uVar26,(float)(int)psVar3[5] * fVar27);
      }
      else {
        dVar25 = (double)CONCAT44(uVar26,(float)(int)psVar3[5]) * dVar29;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar14 = (double *)((int)pdVar14 + -4);
        *(int *)pdVar14 = SUB84(dVar25,0);
      }
      else {
        pdVar14 = pdVar14 + -1;
        *pdVar14 = dVar31;
      }
      uVar21 = DAT_8c612554;
      if (in_FPSCR_PR == '\0') {
        fVar27 = (float)(int)psVar3[4] * fVar27;
      }
      else {
        fVar27 = SUB84((double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),(float)(int)psVar3[4]) *
                       dVar29,0);
      }
      if (in_FPSCR_SZ == '\0') {
        *(float *)((int)pdVar14 + -4) = fVar27;
        *(undefined4 *)(pdVar14 + -1) = *(undefined4 *)(iVar2 + 0x58);
        *(undefined4 *)((int)pdVar14 + -0xc) = *(undefined4 *)(iVar2 + 0x54);
        pdVar14 = pdVar14 + -2;
        *(undefined4 *)pdVar14 = *(undefined4 *)(iVar2 + 0x50);
      }
      else {
        pdVar14[-1] = dVar31;
        pdVar14[-2] = *(double *)(iVar2 + 0x58);
        pdVar14[-3] = *(double *)(iVar2 + 0x54);
        pdVar14 = pdVar14 + -4;
        *pdVar14 = *(double *)(iVar2 + 0x50);
      }
      *(undefined4 *)((int)pdVar14 + -4) = uVar21;
      iVar5 = *piVar7 + 4;
    }
    *piVar7 = iVar5;
  }
  return;
}

