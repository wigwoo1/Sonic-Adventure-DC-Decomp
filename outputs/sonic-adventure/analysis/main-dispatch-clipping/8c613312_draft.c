/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Restarted to delay deadcode elimination for space: stack */

void FUN_8c613312(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                 int param_5,undefined4 param_6,undefined4 param_7)

{
  double *pdVar1;
  int iVar2;
  int *piVar3;
  int iVar4;
  int iVar5;
  double *pdVar6;
  int *piVar7;
  int iVar8;
  undefined8 *unaff_r8;
  undefined8 *puVar9;
  int iVar10;
  int iVar11;
  double *pdVar12;
  int iVar13;
  undefined4 in_fr1;
  double dVar14;
  double dVar15;
  undefined4 uVar16;
  float fVar17;
  float fVar18;
  undefined4 uVar19;
  double in_dr2;
  double dVar20;
  double dVar21;
  double dVar22;
  double dVar23;
  double dVar24;
  double dVar25;
  double in_xd0;
  double in_xd2;
  double dVar26;
  double in_xd4;
  double in_xd6;
  bool bVar27;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 local_48 [8];
  undefined8 uStack_40;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined4 local_30;
  undefined4 uStack_2c;
  undefined4 uStack_28;
  int local_24 [3];
  
  iVar4 = DAT_8c6133ac;
  pdVar1 = DAT_8c6133a8;
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)DAT_8c6133a4);
    dVar24 = (double)CONCAT44(0x3f800000,param_1);
  }
  else {
    in_xd2 = *DAT_8c6133a4;
    dVar24 = (double)CONCAT44(0x3f800000,param_2);
  }
  if (in_FPSCR_PR == '\0') {
    dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) / SUB84(in_dr2,0),SUB84(dVar24,0));
  }
  else {
    dVar24 = dVar24 / in_dr2;
  }
  iVar10 = 0;
  local_24[0] = param_5;
  local_24[1] = param_6;
  local_24[2] = param_7;
  pdVar6 = (double *)local_48;
  piVar7 = local_24;
  pdVar12 = (double *)local_48;
  do {
    uVar19 = (undefined4)((ulonglong)in_dr2 >> 0x20);
    dVar23 = in_xd4;
    if ((*(uint *)(local_24[iVar10] + 0xc) & 1) == 0) {
      piVar3 = local_24 + iVar10;
      dVar23 = (double)CONCAT44(0x3f800000,param_2);
      if (in_FPSCR_SZ == '\0') {
        dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),0x3f800000);
        dVar23 = in_xd4;
      }
      iVar5 = *piVar3;
      if (in_FPSCR_SZ == '\0') {
        in_dr2 = (double)CONCAT44(uVar19,*(undefined4 *)(iVar5 + 0x18));
      }
      else {
        in_xd2 = *(double *)(iVar5 + 0x18);
      }
      if (in_FPSCR_PR == '\0') {
        dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),SUB84(dVar24,0) / SUB84(in_dr2,0)
                                 );
      }
      else {
        dVar24 = dVar24 / in_dr2;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar20 = (double)CONCAT44(*(undefined4 *)(iVar5 + 0x10),*(undefined4 *)(DAT_8c6133ac + 0xc))
        ;
      }
      else {
        in_xd2 = *(double *)(DAT_8c6133ac + 0xc);
        dVar20 = *(double *)(iVar5 + 0x10);
      }
      if (in_FPSCR_PR == '\0') {
        dVar20 = (double)CONCAT44((float)((ulonglong)dVar20 >> 0x20) - SUB84(dVar20,0),
                                  SUB84(dVar20,0));
      }
      else {
        dVar20 = dVar20 - dVar20;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),*(undefined4 *)DAT_8c6133b0);
      }
      else {
        in_xd2 = *DAT_8c6133b0;
      }
      fVar18 = SUB84(dVar24,0);
      if (in_FPSCR_PR == '\0') {
        dVar20 = (double)CONCAT44(((float)((ulonglong)dVar20 >> 0x20) * fVar18) / SUB84(dVar20,0),
                                  SUB84(dVar20,0));
      }
      else {
        dVar20 = (dVar20 * dVar24) / (dVar20 * dVar24);
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)pdVar12 = (int)((ulonglong)dVar20 >> 0x20);
        dVar20 = (double)(ulonglong)*(uint *)(DAT_8c6133ac + 0x10);
      }
      else {
        *pdVar12 = dVar20;
        in_xd2 = *(double *)(DAT_8c6133ac + 0x10);
      }
      iVar5 = *piVar3;
      if (in_FPSCR_SZ == '\0') {
        dVar20 = (double)CONCAT44(*(undefined4 *)(iVar5 + 0x14),SUB84(dVar20,0));
      }
      else {
        dVar20 = *(double *)(iVar5 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        dVar20 = (double)CONCAT44((float)((ulonglong)dVar20 >> 0x20) - SUB84(dVar20,0),
                                  SUB84(dVar20,0));
      }
      else {
        dVar20 = dVar20 - dVar20;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),*(undefined4 *)DAT_8c6133a8);
      }
      else {
        in_xd2 = *DAT_8c6133a8;
      }
      if (in_FPSCR_PR == '\0') {
        in_dr2 = (double)CONCAT44(((float)((ulonglong)dVar20 >> 0x20) * fVar18) / SUB84(dVar20,0),
                                  SUB84(dVar20,0));
      }
      else {
        in_dr2 = (dVar20 * dVar24) / (dVar20 * dVar24);
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar12 + 4) = (int)((ulonglong)in_dr2 >> 0x20);
        *(float *)(pdVar12 + 1) = fVar18;
      }
      else {
        *(double *)((int)pdVar12 + 4) = in_dr2;
        pdVar12[1] = dVar23;
      }
    }
    else {
      if (in_FPSCR_SZ == '\0') {
        uVar16 = *(undefined4 *)(*piVar7 + 0x10);
        in_dr2 = (double)CONCAT44(uVar19,uVar16);
        *(undefined4 *)pdVar6 = uVar16;
      }
      else {
        in_xd2 = *(double *)(*piVar7 + 0x10);
        *pdVar6 = in_xd2;
      }
      if (in_FPSCR_SZ == '\0') {
        uVar19 = *(undefined4 *)(*piVar7 + 0x14);
        in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),uVar19);
        *(undefined4 *)((int)pdVar6 + 4) = uVar19;
      }
      else {
        in_xd2 = *(double *)(*piVar7 + 0x14);
        *(double *)((int)pdVar6 + 4) = in_xd2;
      }
      if (in_FPSCR_SZ == '\0') {
        uVar19 = *(undefined4 *)(*piVar7 + 0x18);
        in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),uVar19);
        *(undefined4 *)(pdVar6 + 1) = uVar19;
      }
      else {
        in_xd2 = *(double *)(*piVar7 + 0x18);
        pdVar6[1] = in_xd2;
      }
    }
    iVar10 = iVar10 + 1;
    pdVar6 = (double *)((int)pdVar6 + 0xc);
    piVar7 = piVar7 + 1;
    pdVar12 = (double *)((int)pdVar12 + 0xc);
    in_xd4 = dVar23;
  } while (iVar10 < 3);
  if (in_FPSCR_SZ == '\0') {
    dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),uStack_38);
    dVar20 = (double)CONCAT44(uStack_34,(undefined4)uStack_40);
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),local_48._4_4_);
    dVar14 = (double)CONCAT44((undefined4)uStack_40,in_fr1);
    param_3 = local_48._4_4_;
  }
  else {
    dVar14 = (double)CONCAT44(uStack_40._4_4_,(undefined4)uStack_40);
    dVar23 = (double)CONCAT44(uStack_34,uStack_38);
    in_xd2 = (double)CONCAT44((undefined4)uStack_40,local_48._4_4_);
    dVar20 = (double)CONCAT44(local_30,uStack_34);
    uStack_40._0_4_ = uStack_34;
    in_xd6 = dVar14;
  }
  fVar18 = (float)((ulonglong)dVar20 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    fVar17 = SUB84(in_dr2,0) * fVar18;
    dVar14 = (double)CONCAT44((float)((ulonglong)dVar14 >> 0x20) * SUB84(dVar24,0),SUB84(dVar14,0));
  }
  else {
    fVar17 = SUB84(in_dr2 * dVar20,0);
    dVar14 = dVar14 * dVar24;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar25 = (double)CONCAT44(local_48._0_4_,param_4);
    dVar22 = (double)CONCAT44(uStack_40._4_4_,param_3);
    in_xd6 = (double)CONCAT44((undefined4)uStack_40,fVar17);
  }
  else {
    dVar25 = (double)CONCAT44(local_48._4_4_,local_48._0_4_);
    dVar22 = (double)CONCAT44(uStack_38,uStack_40._4_4_);
  }
  fVar17 = (float)((ulonglong)dVar22 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar21 = (double)CONCAT44((float)((ulonglong)in_xd6 >> 0x20) * fVar17,SUB84(in_xd6,0));
  }
  else {
    dVar21 = in_xd6 * dVar22;
  }
  dVar15 = dVar22;
  if (in_FPSCR_SZ == '\0') {
    dVar14 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),fVar17);
    dVar15 = in_xd0;
  }
  if (in_FPSCR_PR == '\0') {
    dVar14 = (double)CONCAT44((float)((ulonglong)dVar14 >> 0x20) - SUB84(dVar21,0),SUB84(dVar14,0));
  }
  else {
    dVar14 = dVar14 - dVar21;
  }
  uVar19 = (undefined4)((ulonglong)dVar25 >> 0x20);
  dVar26 = dVar25;
  if (in_FPSCR_SZ == '\0') {
    dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),uVar19);
    dVar26 = in_xd2;
  }
  if (in_FPSCR_PR == '\0') {
    dVar14 = (double)CONCAT44((int)((ulonglong)dVar14 >> 0x20),SUB84(dVar14,0) * SUB84(dVar22,0));
    fVar18 = SUB84(dVar21,0) * fVar18 - (float)((ulonglong)dVar21 >> 0x20);
  }
  else {
    dVar14 = dVar14 * dVar22;
    fVar18 = SUB84(dVar21 * dVar20 - dVar21 * dVar20,0);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar22 = (double)CONCAT44(uStack_2c,fVar18);
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
    dVar26 = (double)CONCAT44(uStack_2c,local_30);
  }
  if (in_FPSCR_PR == '\0') {
    dVar22 = (double)CONCAT44((float)((ulonglong)dVar14 >> 0x20) * SUB84(dVar22,0) +
                              (float)((ulonglong)dVar22 >> 0x20),SUB84(dVar22,0));
  }
  if (in_FPSCR_SZ == '\0') {
    dVar25 = (double)CONCAT44(uVar19,uVar19);
  }
  if (in_FPSCR_PR == '\0') {
    dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),SUB84(dVar25,0) * SUB84(dVar24,0));
  }
  else {
    dVar25 = dVar25 * dVar24;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),uStack_28);
  }
  else {
    dVar26 = (double)CONCAT44(local_24[0],uStack_28);
  }
  if (in_FPSCR_PR == '\0') {
    fVar18 = SUB84(dVar14,0) - SUB84(dVar25,0);
  }
  else {
    fVar18 = SUB84(dVar14 - dVar25,0);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar15 = (double)CONCAT44(fVar18,fVar18);
  }
  if (in_FPSCR_PR == '\0') {
    dVar22 = (double)((ulonglong)
                      (uint)((float)((ulonglong)dVar15 >> 0x20) * SUB84(dVar22,0) +
                            (float)((ulonglong)dVar22 >> 0x20)) << 0x20);
  }
  dVar25 = (double)((ulonglong)dVar22 & 0xffffffff00000000);
  dVar14 = dVar25;
  if (in_FPSCR_SZ == '\0') {
    dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),(int)((ulonglong)dVar22 >> 0x20));
    dVar14 = dVar23;
  }
  if (in_FPSCR_PR == '\0') {
    bVar27 = SUB84(dVar24,0) < 0.0;
  }
  else {
    bVar27 = dVar24 < dVar25;
  }
  if (((uint)bVar27 & *DAT_8c6136f8) == 0) {
    *DAT_8c6136f8 = 0;
    iVar5 = DAT_8c6136fc;
    iVar13 = 0;
    iVar2 = 0;
    iVar10 = 2;
    do {
      iVar8 = iVar2;
      if ((*(uint *)(local_24[iVar10] + 0xc) & 1 ^ *(uint *)(local_24[iVar8] + 0xc) & 1) == 1) {
        iVar10 = iVar10 * 0xc;
        pdVar6 = (double *)(iVar10 + (int)local_48);
        if (in_FPSCR_SZ == '\0') {
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                    *(undefined4 *)((int)&uStack_40 + iVar10));
        }
        else {
          dVar14 = *(double *)((int)&uStack_40 + iVar10);
        }
        iVar2 = iVar8 * 0xc;
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)DAT_8c613700);
        }
        else {
          dVar26 = *DAT_8c613700;
        }
        pdVar12 = (double *)(iVar2 + (int)local_48);
        if (in_FPSCR_PR == '\0') {
          fVar18 = SUB84(dVar25,0) - SUB84(dVar24,0);
        }
        else {
          fVar18 = SUB84(dVar25 - dVar24,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar23 = (double)CONCAT44(*(undefined4 *)((int)&uStack_40 + iVar2),fVar18);
        }
        else {
          dVar23 = *(double *)((int)&uStack_40 + iVar2);
        }
        if (in_FPSCR_PR == '\0') {
          dVar23 = (double)CONCAT44((float)((ulonglong)dVar23 >> 0x20) - SUB84(dVar24,0),
                                    SUB84(dVar23,0));
        }
        else {
          dVar23 = dVar23 - dVar24;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),SUB84(dVar23,0));
          dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(undefined4 *)pdVar12);
          dVar20 = (double)CONCAT44(*(undefined4 *)pdVar6,SUB84(dVar20,0));
          dVar22 = dVar26;
          dVar26 = dVar14;
        }
        else {
          dVar22 = *pdVar12;
          dVar20 = *pdVar6;
        }
        fVar18 = (float)((ulonglong)dVar20 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                    SUB84(dVar24,0) / (float)((ulonglong)dVar23 >> 0x20));
          fVar17 = SUB84(dVar23,0) - fVar18;
        }
        else {
          dVar24 = dVar24 / dVar23;
          fVar17 = SUB84(dVar23 - dVar20,0);
        }
        uVar19 = SUB84(dVar24,0);
        dVar14 = dVar26;
        dVar23 = dVar20;
        if (in_FPSCR_SZ == '\0') {
          dVar14 = (double)CONCAT44(uVar19,SUB84(dVar15,0));
          dVar23 = (double)CONCAT44(fVar18,fVar17);
        }
        if (in_FPSCR_PR == '\0') {
          dVar23 = (double)CONCAT44((float)((ulonglong)dVar14 >> 0x20) * SUB84(dVar23,0) +
                                    (float)((ulonglong)dVar23 >> 0x20),SUB84(dVar23,0));
        }
        uVar16 = (undefined4)((ulonglong)dVar24 >> 0x20);
        if (in_FPSCR_SZ == '\0') {
          dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),SUB84(dVar20,0));
          dVar20 = (double)CONCAT44(*(undefined4 *)(iVar4 + 0xc),*(undefined4 *)DAT_8c613704);
          dVar21 = (double)CONCAT44(uVar16,SUB84(dVar14,0));
        }
        else {
          dVar20 = *(double *)(iVar4 + 0xc);
          dVar22 = *DAT_8c613704;
          dVar21 = dVar24;
        }
        if (in_FPSCR_PR == '\0') {
          dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),
                                    SUB84(dVar20,0) * (float)((ulonglong)dVar23 >> 0x20));
        }
        else {
          dVar20 = dVar20 * dVar23;
        }
        iVar11 = iVar13 * 0x20 + iVar5;
        if (in_FPSCR_PR == '\0') {
          dVar20 = (double)CONCAT44((float)((ulonglong)dVar21 >> 0x20) * SUB84(dVar20,0) +
                                    (float)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0));
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(iVar11 + 0x10) = (int)((ulonglong)dVar20 >> 0x20);
          fVar18 = *(float *)(local_48 + iVar10 + 4);
          dVar24 = (double)CONCAT44(uVar16,fVar18);
          dVar25 = (double)CONCAT44(*(undefined4 *)(iVar4 + 0x10),
                                    *(undefined4 *)(local_48 + iVar2 + 4));
          dVar14 = dVar26;
          dVar20 = (double)CONCAT44(fVar18,SUB84(dVar23,0));
          dVar26 = (double)CONCAT44(uVar19,SUB84(dVar21,0));
        }
        else {
          *(double *)(iVar11 + 0x10) = dVar20;
          dVar14 = *(double *)(local_48 + iVar10 + 4);
          fVar18 = (float)((ulonglong)dVar14 >> 0x20);
          dVar22 = *(double *)(local_48 + iVar2 + 4);
          dVar25 = *(double *)(iVar4 + 0x10);
          dVar20 = dVar14;
        }
        if (in_FPSCR_PR == '\0') {
          fVar18 = SUB84(dVar25,0) - fVar18;
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),fVar18);
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                    (float)((ulonglong)dVar26 >> 0x20) * fVar18 + SUB84(dVar24,0));
        }
        else {
          dVar25 = dVar25 - dVar20;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)pdVar1);
          dVar15 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),SUB84(dVar26,0));
          dVar26 = dVar22;
        }
        else {
          dVar26 = *pdVar1;
          dVar15 = dVar24;
        }
        fVar18 = (float)((ulonglong)dVar15 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          fVar17 = SUB84(dVar25,0) * SUB84(dVar24,0);
          dVar25 = (double)CONCAT44(fVar18 * fVar17 + (float)((ulonglong)dVar25 >> 0x20),fVar17);
        }
        else {
          dVar25 = dVar25 * dVar24;
        }
        iVar10 = iVar13 + 1;
        iVar13 = iVar13 + 1;
        if (in_FPSCR_SZ == '\0') {
          *(int *)(iVar11 + 0x14) = (int)((ulonglong)dVar25 >> 0x20);
          *(float *)(iVar11 + 0x18) = fVar18;
          iVar13 = iVar10;
        }
        else {
          *(double *)(iVar11 + 0x14) = dVar25;
          *(double *)(iVar11 + 0x18) = dVar15;
        }
      }
      piVar7 = local_24 + iVar8;
      if ((*(uint *)(*piVar7 + 0xc) & 1) == 0) {
        iVar10 = iVar13 * 0x20 + iVar5;
        iVar13 = iVar13 + 1;
        if (in_FPSCR_SZ == '\0') {
          uVar19 = *(undefined4 *)(*piVar7 + 0x10);
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),uVar19);
          *(undefined4 *)(iVar10 + 0x10) = uVar19;
        }
        else {
          dVar26 = *(double *)(*piVar7 + 0x10);
          *(double *)(iVar10 + 0x10) = dVar26;
        }
        if (in_FPSCR_SZ == '\0') {
          uVar19 = *(undefined4 *)(*piVar7 + 0x14);
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),uVar19);
          *(undefined4 *)(iVar10 + 0x14) = uVar19;
        }
        else {
          dVar26 = *(double *)(*piVar7 + 0x14);
          *(double *)(iVar10 + 0x14) = dVar26;
        }
        if (in_FPSCR_SZ == '\0') {
          uVar19 = *(undefined4 *)(*piVar7 + 0x18);
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),uVar19);
          *(undefined4 *)(iVar10 + 0x18) = uVar19;
        }
        else {
          dVar26 = *(double *)(*piVar7 + 0x18);
          *(double *)(iVar10 + 0x18) = dVar26;
        }
      }
      iVar11 = DAT_8c613710;
      piVar7 = DAT_8c61370c;
      uVar19 = DAT_8c613708;
      iVar2 = iVar8 + 1;
      iVar10 = iVar8;
    } while (iVar8 + 1 < 3);
    if (iVar13 == 3) {
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)((int)unaff_r8 + 0x1c) = *(undefined4 *)(DAT_8c613710 + 0x1c);
        *(undefined4 *)(unaff_r8 + 3) = *(undefined4 *)(iVar11 + 0x18);
        *(undefined4 *)((int)unaff_r8 + 0x14) = *(undefined4 *)(iVar11 + 0x14);
        *(undefined4 *)(unaff_r8 + 2) = *(undefined4 *)(iVar11 + 0x10);
        *(undefined4 *)((int)unaff_r8 + 0xc) = *(undefined4 *)(iVar5 + 0x18);
        *(undefined4 *)(unaff_r8 + 1) = *(undefined4 *)(iVar5 + 0x14);
        puVar9 = (undefined8 *)((int)unaff_r8 + 4);
        *(undefined4 *)puVar9 = *(undefined4 *)(iVar5 + 0x10);
      }
      else {
        unaff_r8[3] = *(undefined8 *)(DAT_8c613710 + 0x1c);
        unaff_r8[2] = *(undefined8 *)(iVar11 + 0x18);
        unaff_r8[1] = *(undefined8 *)(iVar11 + 0x14);
        *unaff_r8 = *(undefined8 *)(iVar11 + 0x10);
        unaff_r8[-1] = *(undefined8 *)(iVar5 + 0x18);
        unaff_r8[-2] = *(undefined8 *)(iVar5 + 0x14);
        puVar9 = unaff_r8 + -3;
        *puVar9 = *(undefined8 *)(iVar5 + 0x10);
      }
      *(undefined4 *)((int)puVar9 + -4) = uVar19;
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)(puVar9 + 7) = *(undefined4 *)(iVar11 + 0x1c);
        *(undefined4 *)((int)puVar9 + 0x34) = *(undefined4 *)(iVar11 + 0x18);
        *(undefined4 *)(puVar9 + 6) = *(undefined4 *)(iVar11 + 0x14);
        *(undefined4 *)((int)puVar9 + 0x2c) = *(undefined4 *)(iVar11 + 0x10);
        *(undefined4 *)(puVar9 + 5) = *(undefined4 *)(iVar5 + 0x38);
        *(undefined4 *)((int)puVar9 + 0x24) = *(undefined4 *)(iVar5 + 0x34);
        puVar9 = puVar9 + 4;
        *(undefined4 *)puVar9 = *(undefined4 *)(iVar5 + 0x30);
      }
      else {
        *(undefined8 *)((int)puVar9 + 0x34) = *(undefined8 *)(iVar11 + 0x1c);
        *(undefined8 *)((int)puVar9 + 0x2c) = *(undefined8 *)(iVar11 + 0x18);
        *(undefined8 *)((int)puVar9 + 0x24) = *(undefined8 *)(iVar11 + 0x14);
        *(undefined8 *)((int)puVar9 + 0x1c) = *(undefined8 *)(iVar11 + 0x10);
        *(undefined8 *)((int)puVar9 + 0x14) = *(undefined8 *)(iVar5 + 0x38);
        *(undefined8 *)((int)puVar9 + 0xc) = *(undefined8 *)(iVar5 + 0x34);
        puVar9 = (undefined8 *)((int)puVar9 + 4);
        *puVar9 = *(undefined8 *)(iVar5 + 0x30);
      }
      *(undefined4 *)((int)puVar9 + -4) = uVar19;
      uVar19 = DAT_8c613714;
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)(puVar9 + 7) = *(undefined4 *)(iVar11 + 0x1c);
        *(undefined4 *)((int)puVar9 + 0x34) = *(undefined4 *)(iVar11 + 0x18);
        *(undefined4 *)(puVar9 + 6) = *(undefined4 *)(iVar11 + 0x14);
        *(undefined4 *)((int)puVar9 + 0x2c) = *(undefined4 *)(iVar11 + 0x10);
        *(undefined4 *)(puVar9 + 5) = *(undefined4 *)(iVar5 + 0x58);
        *(undefined4 *)((int)puVar9 + 0x24) = *(undefined4 *)(iVar5 + 0x54);
        puVar9 = puVar9 + 4;
        *(undefined4 *)puVar9 = *(undefined4 *)(iVar5 + 0x50);
      }
      else {
        *(undefined8 *)((int)puVar9 + 0x34) = *(undefined8 *)(iVar11 + 0x1c);
        *(undefined8 *)((int)puVar9 + 0x2c) = *(undefined8 *)(iVar11 + 0x18);
        *(undefined8 *)((int)puVar9 + 0x24) = *(undefined8 *)(iVar11 + 0x14);
        *(undefined8 *)((int)puVar9 + 0x1c) = *(undefined8 *)(iVar11 + 0x10);
        *(undefined8 *)((int)puVar9 + 0x14) = *(undefined8 *)(iVar5 + 0x58);
        *(undefined8 *)((int)puVar9 + 0xc) = *(undefined8 *)(iVar5 + 0x54);
        puVar9 = (undefined8 *)((int)puVar9 + 4);
        *puVar9 = *(undefined8 *)(iVar5 + 0x50);
      }
      *(undefined4 *)((int)puVar9 + -4) = uVar19;
      iVar4 = *piVar7 + 3;
    }
    else {
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)((int)unaff_r8 + 0x1c) = *(undefined4 *)(DAT_8c613710 + 0x1c);
        *(undefined4 *)(unaff_r8 + 3) = *(undefined4 *)(iVar11 + 0x18);
        *(undefined4 *)((int)unaff_r8 + 0x14) = *(undefined4 *)(iVar11 + 0x14);
        *(undefined4 *)(unaff_r8 + 2) = *(undefined4 *)(iVar11 + 0x10);
        *(undefined4 *)((int)unaff_r8 + 0xc) = *(undefined4 *)(iVar5 + 0x18);
        *(undefined4 *)(unaff_r8 + 1) = *(undefined4 *)(iVar5 + 0x14);
        puVar9 = (undefined8 *)((int)unaff_r8 + 4);
        *(undefined4 *)puVar9 = *(undefined4 *)(iVar5 + 0x10);
      }
      else {
        unaff_r8[3] = *(undefined8 *)(DAT_8c613710 + 0x1c);
        unaff_r8[2] = *(undefined8 *)(iVar11 + 0x18);
        unaff_r8[1] = *(undefined8 *)(iVar11 + 0x14);
        *unaff_r8 = *(undefined8 *)(iVar11 + 0x10);
        unaff_r8[-1] = *(undefined8 *)(iVar5 + 0x18);
        unaff_r8[-2] = *(undefined8 *)(iVar5 + 0x14);
        puVar9 = unaff_r8 + -3;
        *puVar9 = *(undefined8 *)(iVar5 + 0x10);
      }
      *(undefined4 *)((int)puVar9 + -4) = uVar19;
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)(puVar9 + 7) = *(undefined4 *)(iVar11 + 0x1c);
        *(undefined4 *)((int)puVar9 + 0x34) = *(undefined4 *)(iVar11 + 0x18);
        *(undefined4 *)(puVar9 + 6) = *(undefined4 *)(iVar11 + 0x14);
        *(undefined4 *)((int)puVar9 + 0x2c) = *(undefined4 *)(iVar11 + 0x10);
        *(undefined4 *)(puVar9 + 5) = *(undefined4 *)(iVar5 + 0x38);
        *(undefined4 *)((int)puVar9 + 0x24) = *(undefined4 *)(iVar5 + 0x34);
        puVar9 = puVar9 + 4;
        *(undefined4 *)puVar9 = *(undefined4 *)(iVar5 + 0x30);
      }
      else {
        *(undefined8 *)((int)puVar9 + 0x34) = *(undefined8 *)(iVar11 + 0x1c);
        *(undefined8 *)((int)puVar9 + 0x2c) = *(undefined8 *)(iVar11 + 0x18);
        *(undefined8 *)((int)puVar9 + 0x24) = *(undefined8 *)(iVar11 + 0x14);
        *(undefined8 *)((int)puVar9 + 0x1c) = *(undefined8 *)(iVar11 + 0x10);
        *(undefined8 *)((int)puVar9 + 0x14) = *(undefined8 *)(iVar5 + 0x38);
        *(undefined8 *)((int)puVar9 + 0xc) = *(undefined8 *)(iVar5 + 0x34);
        puVar9 = (undefined8 *)((int)puVar9 + 4);
        *puVar9 = *(undefined8 *)(iVar5 + 0x30);
      }
      *(undefined4 *)((int)puVar9 + -4) = uVar19;
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)(puVar9 + 7) = *(undefined4 *)(iVar11 + 0x1c);
        *(undefined4 *)((int)puVar9 + 0x34) = *(undefined4 *)(iVar11 + 0x18);
        *(undefined4 *)(puVar9 + 6) = *(undefined4 *)(iVar11 + 0x14);
        *(undefined4 *)((int)puVar9 + 0x2c) = *(undefined4 *)(iVar11 + 0x10);
        *(undefined4 *)(puVar9 + 5) = *(undefined4 *)(iVar5 + 0x78);
        *(undefined4 *)((int)puVar9 + 0x24) = *(undefined4 *)(iVar5 + 0x74);
        puVar9 = puVar9 + 4;
        *(undefined4 *)puVar9 = *(undefined4 *)(iVar5 + 0x70);
      }
      else {
        *(undefined8 *)((int)puVar9 + 0x34) = *(undefined8 *)(iVar11 + 0x1c);
        *(undefined8 *)((int)puVar9 + 0x2c) = *(undefined8 *)(iVar11 + 0x18);
        *(undefined8 *)((int)puVar9 + 0x24) = *(undefined8 *)(iVar11 + 0x14);
        *(undefined8 *)((int)puVar9 + 0x1c) = *(undefined8 *)(iVar11 + 0x10);
        *(undefined8 *)((int)puVar9 + 0x14) = *(undefined8 *)(iVar5 + 0x78);
        *(undefined8 *)((int)puVar9 + 0xc) = *(undefined8 *)(iVar5 + 0x74);
        puVar9 = (undefined8 *)((int)puVar9 + 4);
        *puVar9 = *(undefined8 *)(iVar5 + 0x70);
      }
      *(undefined4 *)((int)puVar9 + -4) = uVar19;
      uVar19 = DAT_8c613714;
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)(puVar9 + 7) = *(undefined4 *)(iVar11 + 0x1c);
        *(undefined4 *)((int)puVar9 + 0x34) = *(undefined4 *)(iVar11 + 0x18);
        *(undefined4 *)(puVar9 + 6) = *(undefined4 *)(iVar11 + 0x14);
        *(undefined4 *)((int)puVar9 + 0x2c) = *(undefined4 *)(iVar11 + 0x10);
        *(undefined4 *)(puVar9 + 5) = *(undefined4 *)(iVar5 + 0x58);
        *(undefined4 *)((int)puVar9 + 0x24) = *(undefined4 *)(iVar5 + 0x54);
        puVar9 = puVar9 + 4;
        *(undefined4 *)puVar9 = *(undefined4 *)(iVar5 + 0x50);
      }
      else {
        *(undefined8 *)((int)puVar9 + 0x34) = *(undefined8 *)(iVar11 + 0x1c);
        *(undefined8 *)((int)puVar9 + 0x2c) = *(undefined8 *)(iVar11 + 0x18);
        *(undefined8 *)((int)puVar9 + 0x24) = *(undefined8 *)(iVar11 + 0x14);
        *(undefined8 *)((int)puVar9 + 0x1c) = *(undefined8 *)(iVar11 + 0x10);
        *(undefined8 *)((int)puVar9 + 0x14) = *(undefined8 *)(iVar5 + 0x58);
        *(undefined8 *)((int)puVar9 + 0xc) = *(undefined8 *)(iVar5 + 0x54);
        puVar9 = (undefined8 *)((int)puVar9 + 4);
        *puVar9 = *(undefined8 *)(iVar5 + 0x50);
      }
      *(undefined4 *)((int)puVar9 + -4) = uVar19;
      iVar4 = *piVar7 + 4;
    }
    *piVar7 = iVar4;
  }
  return;
}

