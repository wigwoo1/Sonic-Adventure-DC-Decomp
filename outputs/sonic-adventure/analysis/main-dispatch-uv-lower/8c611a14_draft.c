/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c611ee6) */
/* WARNING: Removing unreachable block (ram,0x8c611ea8) */
/* WARNING: Removing unreachable block (ram,0x8c611dbe) */
/* WARNING: Removing unreachable block (ram,0x8c611d80) */
/* WARNING: Removing unreachable block (ram,0x8c611d42) */
/* WARNING: Removing unreachable block (ram,0x8c611c7a) */
/* WARNING: Removing unreachable block (ram,0x8c611d4e) */
/* WARNING: Removing unreachable block (ram,0x8c611d8c) */
/* WARNING: Removing unreachable block (ram,0x8c611dca) */
/* WARNING: Removing unreachable block (ram,0x8c611eb4) */
/* WARNING: Removing unreachable block (ram,0x8c611ef2) */
/* WARNING: Removing unreachable block (ram,0x8c611c96) */
/* WARNING: Removing unreachable block (ram,0x8c611e68) */
/* WARNING: Removing unreachable block (ram,0x8c611e2a) */
/* WARNING: Removing unreachable block (ram,0x8c611e36) */
/* WARNING: Removing unreachable block (ram,0x8c611e74) */
/* WARNING: Restarted to delay deadcode elimination for space: stack */

void FUN_8c611a14(double param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                 int param_5,undefined4 param_6,undefined4 param_7,int param_8)

{
  int iVar1;
  int iVar2;
  short *psVar3;
  double *pdVar4;
  int iVar5;
  int iVar6;
  short *psVar7;
  short *psVar8;
  int iVar9;
  int *piVar10;
  double *pdVar11;
  int *piVar12;
  int unaff_r8;
  double *pdVar13;
  int iVar14;
  int iVar15;
  double *pdVar16;
  float fVar17;
  double in_dr0;
  double dVar18;
  double dVar19;
  double dVar20;
  float fVar21;
  undefined4 uVar22;
  float fVar23;
  undefined4 uVar24;
  double in_dr2;
  double dVar25;
  double dVar26;
  double dVar27;
  undefined8 uVar28;
  double dVar29;
  double in_xd0;
  double in_xd2;
  double dVar30;
  double in_xd4;
  double in_xd6;
  bool bVar31;
  short sVar32;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  int local_5c;
  undefined1 local_48 [8];
  undefined8 uStack_40;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined4 local_30;
  undefined4 uStack_2c;
  undefined4 uStack_28;
  int local_24 [3];
  
  iVar9 = DAT_8c611adc;
  pdVar13 = DAT_8c611ad8;
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)DAT_8c611ad4);
    param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),0x3f800000);
  }
  else {
    in_xd2 = *DAT_8c611ad4;
    in_xd4 = (double)CONCAT44(0x3f800000,param_2);
  }
  if (in_FPSCR_PR == '\0') {
    param_1 = (double)(ulonglong)(uint)(SUB84(param_1,0) / SUB84(in_dr2,0));
  }
  else {
    param_1 = param_1 / in_dr2;
  }
  iVar15 = 0;
  local_24[0] = param_5;
  local_24[1] = param_6;
  local_24[2] = param_7;
  pdVar11 = (double *)local_48;
  piVar12 = local_24;
  pdVar16 = (double *)local_48;
  do {
    piVar10 = local_24 + iVar15;
    pdVar4 = (double *)*piVar10;
    if (in_FPSCR_SZ == '\0') {
      dVar25 = (double)CONCAT44(*(undefined4 *)pdVar13,*(undefined4 *)pdVar4);
    }
    else {
      dVar25 = *pdVar13;
      in_xd2 = *pdVar4;
    }
    if (in_FPSCR_PR == '\0') {
      dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar25,0),SUB84(dVar25,0)
                               );
    }
    else {
      dVar25 = dVar25 * dVar25;
    }
    if (in_FPSCR_SZ == '\0') {
      in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),*(undefined4 *)((int)pdVar13 + 4));
      dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)DAT_8c611ae0);
    }
    else {
      in_xd0 = *(double *)((int)pdVar13 + 4);
      in_xd2 = *DAT_8c611ae0;
    }
    if (in_FPSCR_PR == '\0') {
      fVar21 = SUB84(dVar25,0) - (float)((ulonglong)dVar25 >> 0x20);
    }
    else {
      fVar21 = SUB84(dVar25 - dVar25,0);
    }
    if (in_FPSCR_SZ == '\0') {
      dVar25 = (double)CONCAT44(*(undefined4 *)((int)pdVar4 + 4),fVar21);
    }
    else {
      dVar25 = *(double *)((int)pdVar4 + 4);
    }
    if (in_FPSCR_PR == '\0') {
      dVar18 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                SUB84(in_dr0,0) * (float)((ulonglong)dVar25 >> 0x20));
    }
    else {
      dVar18 = in_dr0 * dVar25;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar25 = (double)CONCAT44(*(undefined4 *)(pdVar4 + 1),SUB84(dVar25,0));
    }
    else {
      dVar25 = pdVar4[1];
    }
    if (in_FPSCR_PR == '\0') {
      dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),SUB84(dVar25,0) - SUB84(dVar18,0));
    }
    else {
      dVar25 = dVar25 - dVar18;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar18 = (double)CONCAT44((int)((ulonglong)dVar18 >> 0x20),*(undefined4 *)(pdVar13 + 1));
    }
    else {
      in_xd0 = pdVar13[1];
    }
    fVar21 = (float)((ulonglong)dVar25 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      fVar17 = SUB84(dVar18,0) * fVar21;
      in_dr0 = (double)CONCAT44((int)((ulonglong)dVar18 >> 0x20),fVar17);
      dVar25 = (double)CONCAT44(fVar21,SUB84(dVar25,0) - fVar17);
    }
    else {
      in_dr0 = dVar18 * dVar25;
      fVar17 = SUB84(in_dr0,0);
      dVar25 = dVar25 - in_dr0;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)pdVar4 + 0x1c) = SUB84(dVar25,0);
    }
    else {
      *(double *)((int)pdVar4 + 0x1c) = in_xd2;
    }
    iVar2 = DAT_8c611df0;
    uVar24 = (undefined4)((ulonglong)dVar25 >> 0x20);
    if ((*(uint *)(*piVar10 + 0xc) & 1) == 0) {
      piVar10 = local_24 + iVar15;
      dVar18 = (double)CONCAT44(0x3f800000,param_2);
      if (in_FPSCR_SZ == '\0') {
        dVar18 = (double)CONCAT44(0x3f800000,SUB84(param_1,0));
      }
      iVar5 = *piVar10;
      if (in_FPSCR_SZ == '\0') {
        dVar25 = (double)CONCAT44(uVar24,*(undefined4 *)(iVar5 + 0x18));
      }
      else {
        in_xd2 = *(double *)(iVar5 + 0x18);
      }
      if (in_FPSCR_PR == '\0') {
        param_1 = (double)CONCAT44((float)((ulonglong)dVar18 >> 0x20) / SUB84(dVar25,0),
                                   SUB84(dVar18,0));
      }
      else {
        param_1 = dVar18 / dVar25;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar25 = (double)CONCAT44(*(undefined4 *)(iVar5 + 0x10),*(undefined4 *)(iVar9 + 0xc));
      }
      else {
        in_xd2 = *(double *)(iVar9 + 0xc);
        dVar25 = *(double *)(iVar5 + 0x10);
      }
      if (in_FPSCR_PR == '\0') {
        dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) - SUB84(dVar25,0),
                                  SUB84(dVar25,0));
      }
      else {
        dVar25 = dVar25 - dVar25;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)DAT_8c611ae4);
      }
      else {
        in_xd2 = *DAT_8c611ae4;
      }
      fVar21 = (float)((ulonglong)param_1 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar25 = (double)CONCAT44(((float)((ulonglong)dVar25 >> 0x20) * fVar21) / SUB84(dVar25,0),
                                  SUB84(dVar25,0));
      }
      else {
        dVar25 = (dVar25 * param_1) / (dVar25 * param_1);
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)pdVar11 = (int)((ulonglong)dVar25 >> 0x20);
        dVar25 = (double)(ulonglong)*(uint *)(iVar9 + 0x10);
      }
      else {
        *pdVar11 = dVar25;
        in_xd2 = *(double *)(iVar9 + 0x10);
      }
      iVar5 = *piVar10;
      if (in_FPSCR_SZ == '\0') {
        dVar25 = (double)CONCAT44(*(undefined4 *)(iVar5 + 0x14),SUB84(dVar25,0));
      }
      else {
        dVar25 = *(double *)(iVar5 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) - SUB84(dVar25,0),
                                  SUB84(dVar25,0));
      }
      else {
        dVar25 = dVar25 - dVar25;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)DAT_8c611ae8);
      }
      else {
        in_xd2 = *DAT_8c611ae8;
      }
      if (in_FPSCR_PR == '\0') {
        dVar25 = (double)CONCAT44(((float)((ulonglong)dVar25 >> 0x20) * fVar21) / SUB84(dVar25,0),
                                  SUB84(dVar25,0));
      }
      else {
        dVar25 = (dVar25 * param_1) / (dVar25 * param_1);
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar11 + 4) = (int)((ulonglong)dVar25 >> 0x20);
        *(float *)(pdVar11 + 1) = fVar21;
      }
      else {
        *(double *)((int)pdVar11 + 4) = dVar25;
        pdVar11[1] = param_1;
      }
    }
    else {
      if (in_FPSCR_SZ == '\0') {
        uVar22 = *(undefined4 *)(*piVar12 + 0x10);
        dVar25 = (double)CONCAT44(uVar24,uVar22);
        *(undefined4 *)pdVar16 = uVar22;
      }
      else {
        in_xd2 = *(double *)(*piVar12 + 0x10);
        *pdVar16 = in_xd2;
      }
      if (in_FPSCR_SZ == '\0') {
        uVar24 = *(undefined4 *)(*piVar12 + 0x14);
        dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),uVar24);
        *(undefined4 *)((int)pdVar16 + 4) = uVar24;
      }
      else {
        in_xd2 = *(double *)(*piVar12 + 0x14);
        *(double *)((int)pdVar16 + 4) = in_xd2;
      }
      if (in_FPSCR_SZ == '\0') {
        uVar24 = *(undefined4 *)(*piVar12 + 0x18);
        dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),uVar24);
        *(undefined4 *)(pdVar16 + 1) = uVar24;
      }
      else {
        in_xd2 = *(double *)(*piVar12 + 0x18);
        pdVar16[1] = in_xd2;
      }
    }
    iVar15 = iVar15 + 1;
    piVar12 = piVar12 + 1;
    pdVar16 = (double *)((int)pdVar16 + 0xc);
    pdVar11 = (double *)((int)pdVar11 + 0xc);
  } while (iVar15 < 3);
  if (in_FPSCR_SZ == '\0') {
    dVar27 = (double)CONCAT44(uStack_38,SUB84(param_1,0));
    dVar18 = (double)CONCAT44(uStack_34,(undefined4)uStack_40);
    dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),local_48._4_4_);
    dVar19 = (double)CONCAT44((undefined4)uStack_40,fVar17);
    param_3 = local_48._4_4_;
  }
  else {
    dVar19 = (double)CONCAT44(uStack_40._4_4_,(undefined4)uStack_40);
    dVar27 = (double)CONCAT44(uStack_34,uStack_38);
    in_xd2 = (double)CONCAT44((undefined4)uStack_40,local_48._4_4_);
    dVar18 = (double)CONCAT44(local_30,uStack_34);
    uStack_40._0_4_ = uStack_34;
    in_xd6 = dVar19;
  }
  fVar17 = (float)((ulonglong)dVar18 >> 0x20);
  fVar21 = (float)((ulonglong)dVar27 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    fVar23 = SUB84(dVar25,0) * fVar17;
    dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) * fVar21,SUB84(dVar19,0));
  }
  else {
    fVar23 = SUB84(dVar25 * dVar18,0);
    dVar19 = dVar19 * dVar27;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar29 = (double)CONCAT44(local_48._0_4_,param_4);
    dVar25 = (double)CONCAT44(uStack_40._4_4_,param_3);
    in_xd6 = (double)CONCAT44((undefined4)uStack_40,fVar23);
  }
  else {
    dVar29 = (double)CONCAT44(local_48._4_4_,local_48._0_4_);
    dVar25 = (double)CONCAT44(uStack_38,uStack_40._4_4_);
  }
  fVar23 = (float)((ulonglong)dVar25 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar26 = (double)CONCAT44((float)((ulonglong)in_xd6 >> 0x20) * fVar23,SUB84(in_xd6,0));
  }
  else {
    dVar26 = in_xd6 * dVar25;
  }
  dVar20 = dVar25;
  if (in_FPSCR_SZ == '\0') {
    dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),fVar23);
    dVar20 = in_xd0;
  }
  if (in_FPSCR_PR == '\0') {
    dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) - SUB84(dVar26,0),SUB84(dVar19,0));
  }
  else {
    dVar19 = dVar19 - dVar26;
  }
  uVar24 = (undefined4)((ulonglong)dVar29 >> 0x20);
  dVar30 = dVar29;
  if (in_FPSCR_SZ == '\0') {
    dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),uVar24);
    dVar30 = in_xd2;
  }
  if (in_FPSCR_PR == '\0') {
    dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),SUB84(dVar26,0) * fVar17);
  }
  else {
    dVar26 = dVar26 * dVar18;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar29 = (double)CONCAT44(uVar24,uVar24);
  }
  if (in_FPSCR_PR == '\0') {
    dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),SUB84(dVar19,0) * SUB84(dVar25,0));
    fVar17 = SUB84(dVar26,0) - (float)((ulonglong)dVar26 >> 0x20);
  }
  else {
    dVar19 = dVar19 * dVar25;
    fVar17 = SUB84(dVar26 - dVar26,0);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar25 = (double)CONCAT44(uStack_2c,fVar17);
  }
  else {
    dVar25 = (double)CONCAT44(uStack_28,uStack_2c);
  }
  if (in_FPSCR_PR == '\0') {
    dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar25,0),SUB84(dVar25,0));
  }
  else {
    dVar25 = dVar25 * dVar25;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),local_30);
  }
  else {
    dVar30 = (double)CONCAT44(uStack_2c,local_30);
  }
  if (in_FPSCR_PR == '\0') {
    dVar25 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) * SUB84(dVar25,0) +
                              (float)((ulonglong)dVar25 >> 0x20),SUB84(dVar25,0));
    dVar29 = (double)CONCAT44((int)((ulonglong)dVar29 >> 0x20),SUB84(dVar29,0) * fVar21);
  }
  else {
    dVar29 = dVar29 * dVar27;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),uStack_28);
  }
  else {
    dVar30 = (double)CONCAT44(local_24[0],uStack_28);
  }
  if (in_FPSCR_PR == '\0') {
    fVar21 = SUB84(dVar19,0) - SUB84(dVar29,0);
  }
  else {
    fVar21 = SUB84(dVar19 - dVar29,0);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar20 = (double)CONCAT44(fVar21,fVar21);
  }
  if (in_FPSCR_PR == '\0') {
    dVar25 = (double)((ulonglong)
                      (uint)((float)((ulonglong)dVar20 >> 0x20) * SUB84(dVar25,0) +
                            (float)((ulonglong)dVar25 >> 0x20)) << 0x20);
  }
  dVar29 = (double)((ulonglong)dVar25 & 0xffffffff00000000);
  dVar19 = dVar29;
  if (in_FPSCR_SZ == '\0') {
    dVar19 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),SUB84(dVar27,0));
  }
  if (in_FPSCR_PR == '\0') {
    bVar31 = (float)((ulonglong)dVar19 >> 0x20) < 0.0;
  }
  else {
    bVar31 = dVar19 < dVar29;
  }
  if (((uint)bVar31 & *DAT_8c611dec) == 0) {
    *DAT_8c611dec = 0;
    psVar3 = DAT_8c611df4;
    iVar5 = 0;
    local_5c = 2;
    iVar15 = 0;
    do {
      iVar14 = iVar15;
      if ((*(uint *)(local_24[local_5c] + 0xc) & 1 ^ *(uint *)(local_24[iVar14] + 0xc) & 1) == 1) {
        iVar15 = local_5c * 0xc;
        pdVar13 = (double *)(iVar15 + (int)local_48);
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(*(undefined4 *)((int)&uStack_40 + iVar15),SUB84(dVar19,0));
          dVar29 = (double)CONCAT44((int)((ulonglong)dVar29 >> 0x20),*(undefined4 *)DAT_8c611df8);
        }
        else {
          dVar25 = *(double *)((int)&uStack_40 + iVar15);
          dVar30 = *DAT_8c611df8;
        }
        iVar1 = iVar14 * 0xc;
        fVar21 = (float)((ulonglong)dVar25 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          fVar17 = SUB84(dVar29,0) - fVar21;
        }
        else {
          fVar17 = SUB84(dVar29 - dVar25,0);
        }
        pdVar11 = (double *)(iVar1 + (int)local_48);
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)((int)&uStack_40 + iVar1),fVar17);
        }
        else {
          dVar27 = *(double *)((int)&uStack_40 + iVar1);
        }
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) - fVar21,SUB84(dVar27,0));
        }
        else {
          dVar27 = dVar27 - dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          uVar24 = SUB84(dVar27,0);
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),*(undefined4 *)pdVar11);
          dVar29 = (double)CONCAT44(*(undefined4 *)pdVar13,SUB84(dVar18,0));
          dVar18 = dVar30;
          dVar30 = (double)CONCAT44(uVar24,SUB84(dVar25,0));
        }
        else {
          dVar18 = *pdVar11;
          dVar29 = *pdVar13;
        }
        fVar21 = (float)((ulonglong)dVar29 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar19 = (double)CONCAT44((float)((ulonglong)dVar30 >> 0x20) /
                                    (float)((ulonglong)dVar27 >> 0x20),SUB84(dVar30,0));
          fVar17 = SUB84(dVar27,0) - fVar21;
        }
        else {
          dVar19 = dVar30 / dVar27;
          fVar17 = SUB84(dVar27 - dVar29,0);
        }
        uVar24 = (undefined4)((ulonglong)dVar19 >> 0x20);
        dVar27 = dVar19;
        dVar25 = dVar29;
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(uVar24,SUB84(dVar20,0));
          dVar25 = (double)CONCAT44(fVar21,fVar17);
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) * SUB84(dVar25,0) +
                                    (float)((ulonglong)dVar25 >> 0x20),SUB84(dVar25,0));
        }
        dVar26 = in_xd4;
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),SUB84(dVar29,0));
          dVar26 = (double)CONCAT44(SUB84(dVar19,0),SUB84(dVar27,0));
        }
        iVar6 = iVar5 * 0x20 + iVar2;
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)(iVar9 + 0xc),*(undefined4 *)DAT_8c611dfc);
        }
        else {
          dVar18 = *DAT_8c611dfc;
          dVar27 = *(double *)(iVar9 + 0xc);
        }
        if (in_FPSCR_PR == '\0') {
          fVar21 = SUB84(dVar27,0) * (float)((ulonglong)dVar25 >> 0x20);
          dVar27 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) * fVar21 +
                                    (float)((ulonglong)dVar27 >> 0x20),fVar21);
        }
        else {
          dVar27 = dVar27 * dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(iVar6 + 0x10) = (int)((ulonglong)dVar27 >> 0x20);
          fVar21 = *(float *)(local_48 + iVar15 + 4);
          dVar29 = (double)CONCAT44(fVar21,*(undefined4 *)(local_48 + iVar1 + 4));
          dVar25 = (double)CONCAT44(fVar21,SUB84(dVar25,0));
          dVar27 = (double)CONCAT44(uVar24,SUB84(dVar26,0));
        }
        else {
          *(double *)(iVar6 + 0x10) = dVar27;
          dVar29 = *(double *)(local_48 + iVar15 + 4);
          fVar21 = (float)((ulonglong)dVar29 >> 0x20);
          dVar18 = *(double *)(local_48 + iVar1 + 4);
          dVar25 = dVar29;
          dVar27 = dVar19;
        }
        if (in_FPSCR_PR == '\0') {
          fVar21 = SUB84(dVar29,0) - fVar21;
          dVar29 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) * fVar21 +
                                    (float)((ulonglong)dVar29 >> 0x20),fVar21);
        }
        else {
          dVar29 = dVar29 - dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          uVar22 = (undefined4)((ulonglong)dVar29 >> 0x20);
          dVar29 = (double)CONCAT44(uVar22,SUB84(dVar25,0));
          dVar25 = (double)CONCAT44(uVar22,*(undefined4 *)DAT_8c611e00);
          dVar30 = dVar18;
        }
        else {
          dVar30 = *DAT_8c611e00;
          dVar25 = dVar29;
        }
        if (in_FPSCR_PR == '\0') {
          fVar21 = SUB84(dVar25,0) * (float)((ulonglong)dVar29 >> 0x20);
        }
        else {
          fVar21 = SUB84(dVar25 * dVar29,0);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar28 = CONCAT44(*(undefined4 *)(iVar9 + 0x10),fVar21);
          dVar25 = (double)CONCAT44(SUB84(dVar19,0),SUB84(dVar27,0));
        }
        else {
          uVar28 = *(undefined8 *)(iVar9 + 0x10);
          dVar25 = in_xd4;
        }
        fVar21 = (float)((ulonglong)dVar25 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          uVar28 = CONCAT44(fVar21 * (float)uVar28 + (float)((ulonglong)uVar28 >> 0x20),
                            (float)uVar28);
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(iVar6 + 0x14) = (int)((ulonglong)uVar28 >> 0x20);
          *(float *)(iVar6 + 0x18) = fVar21;
        }
        else {
          *(undefined8 *)(iVar6 + 0x14) = uVar28;
          *(double *)(iVar6 + 0x18) = dVar25;
        }
        iVar15 = local_24[local_5c];
        if (in_FPSCR_SZ == '\0') {
          dVar18 = (double)CONCAT44(*(undefined4 *)(iVar15 + 0x1c),SUB84(dVar29,0));
          dVar20 = (double)CONCAT44(uVar24,SUB84(dVar25,0));
        }
        else {
          dVar18 = *(double *)(iVar15 + 0x1c);
          dVar20 = dVar19;
        }
        iVar15 = local_24[iVar14];
        fVar21 = (float)((ulonglong)dVar18 >> 0x20);
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(fVar21,*(undefined4 *)(iVar15 + 0x1c));
        }
        else {
          dVar30 = *(double *)(iVar15 + 0x1c);
          dVar25 = dVar18;
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),SUB84(dVar25,0) - fVar21);
        }
        else {
          dVar25 = dVar25 - dVar18;
        }
        fVar21 = (float)((ulonglong)dVar20 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44(fVar21 * SUB84(dVar25,0) + (float)((ulonglong)dVar25 >> 0x20),
                                    SUB84(dVar25,0));
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(iVar6 + 0x1c) = (int)((ulonglong)dVar25 >> 0x20);
        }
        else {
          *(double *)(iVar6 + 0x1c) = dVar25;
        }
        psVar8 = (short *)(local_5c * 4 + param_8);
        sVar32 = *psVar8;
        psVar7 = (short *)(iVar14 * 4 + param_8);
        iVar15 = (int)*psVar7 - (int)sVar32;
        if (in_FPSCR_PR == '\0') {
          sVar32 = (short)(int)(fVar21 * (float)iVar15 + (float)(int)sVar32);
        }
        else {
          sVar32 = (short)(int)(double)iVar15;
        }
        psVar3[iVar5 * 2] = sVar32;
        sVar32 = psVar8[1];
        iVar15 = (int)psVar7[1] - (int)sVar32;
        if (in_FPSCR_PR == '\0') {
          fVar17 = (float)iVar15;
          fVar21 = fVar21 * fVar17 + (float)(int)sVar32;
          dVar29 = (double)CONCAT44(fVar17,fVar21);
          sVar32 = (short)(int)fVar21;
        }
        else {
          dVar29 = (double)iVar15;
          sVar32 = (short)(int)dVar29;
        }
        (psVar3 + iVar5 * 2)[1] = sVar32;
        iVar5 = iVar5 + 1;
      }
      if ((*(uint *)(local_24[iVar14] + 0xc) & 1) == 0) {
        piVar12 = local_24 + iVar14;
        iVar15 = iVar5 * 0x20 + iVar2;
        if (in_FPSCR_SZ == '\0') {
          uVar24 = *(undefined4 *)(*piVar12 + 0x10);
          dVar29 = (double)CONCAT44((int)((ulonglong)dVar29 >> 0x20),uVar24);
          *(undefined4 *)(iVar15 + 0x10) = uVar24;
        }
        else {
          dVar30 = *(double *)(*piVar12 + 0x10);
          *(double *)(iVar15 + 0x10) = dVar30;
        }
        if (in_FPSCR_SZ == '\0') {
          uVar24 = *(undefined4 *)(*piVar12 + 0x14);
          dVar29 = (double)CONCAT44((int)((ulonglong)dVar29 >> 0x20),uVar24);
          *(undefined4 *)(iVar15 + 0x14) = uVar24;
        }
        else {
          dVar30 = *(double *)(*piVar12 + 0x14);
          *(double *)(iVar15 + 0x14) = dVar30;
        }
        if (in_FPSCR_SZ == '\0') {
          uVar24 = *(undefined4 *)(*piVar12 + 0x18);
          dVar29 = (double)CONCAT44((int)((ulonglong)dVar29 >> 0x20),uVar24);
          *(undefined4 *)(iVar15 + 0x18) = uVar24;
        }
        else {
          dVar30 = *(double *)(*piVar12 + 0x18);
          *(double *)(iVar15 + 0x18) = dVar30;
        }
        if (in_FPSCR_SZ == '\0') {
          uVar24 = *(undefined4 *)(*piVar12 + 0x1c);
          dVar29 = (double)CONCAT44((int)((ulonglong)dVar29 >> 0x20),uVar24);
          *(undefined4 *)(iVar15 + 0x1c) = uVar24;
        }
        else {
          dVar30 = *(double *)(*piVar12 + 0x1c);
          *(double *)(iVar15 + 0x1c) = dVar30;
        }
        psVar8 = (short *)(iVar14 * 4 + param_8);
        iVar15 = iVar5 * 2;
        iVar5 = iVar5 + 1;
        psVar3[iVar15] = *psVar8;
        (psVar3 + iVar15)[1] = psVar8[1];
      }
      uVar24 = DAT_8c611e08;
      piVar12 = DAT_8c611e04;
      iVar15 = iVar14 + 1;
      local_5c = iVar14;
    } while (iVar14 + 1 < 3);
    if (in_FPSCR_SZ == '\0') {
      dVar25 = (double)CONCAT44((undefined4)DAT_8c611e0c,SUB84(dVar19,0));
    }
    else {
      dVar25 = (double)CONCAT44(DAT_8c611e0c._4_4_,(undefined4)DAT_8c611e0c);
    }
    uVar22 = (undefined4)((ulonglong)dVar29 >> 0x20);
    fVar21 = (float)((ulonglong)dVar25 >> 0x20);
    if (iVar5 == 3) {
      if (in_FPSCR_SZ == '\0') {
        dVar29 = (double)CONCAT44(uVar22,*(undefined4 *)(iVar2 + 0x1c));
        pdVar13 = (double *)(unaff_r8 + 0x18);
        *(undefined4 *)pdVar13 = *(undefined4 *)(iVar2 + 0x1c);
      }
      else {
        dVar30 = *(double *)(iVar2 + 0x1c);
        pdVar13 = (double *)(unaff_r8 + 0x14);
        *pdVar13 = dVar30;
      }
      uVar22 = (undefined4)((ulonglong)dVar29 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar18 = (double)CONCAT44(uVar22,(float)(int)psVar3[1] * fVar21);
      }
      else {
        dVar18 = (double)CONCAT44(uVar22,(float)(int)psVar3[1]) * dVar25;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar13 = (double *)((int)pdVar13 + -4);
        *(int *)pdVar13 = SUB84(dVar18,0);
      }
      else {
        pdVar13 = pdVar13 + -1;
        *pdVar13 = dVar30;
      }
      uVar22 = (undefined4)((ulonglong)dVar18 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar18 = (double)CONCAT44(uVar22,(float)(int)*psVar3 * fVar21);
      }
      else {
        dVar18 = (double)CONCAT44(uVar22,(float)(int)*psVar3) * dVar25;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar13 + -4) = SUB84(dVar18,0);
        *(undefined4 *)(pdVar13 + -1) = *(undefined4 *)(iVar2 + 0x18);
        *(undefined4 *)((int)pdVar13 + -0xc) = *(undefined4 *)(iVar2 + 0x14);
        dVar18 = (double)CONCAT44((int)((ulonglong)dVar18 >> 0x20),*(undefined4 *)(iVar2 + 0x10));
        pdVar13 = pdVar13 + -2;
        *(undefined4 *)pdVar13 = *(undefined4 *)(iVar2 + 0x10);
      }
      else {
        pdVar13[-1] = dVar30;
        pdVar13[-2] = *(double *)(iVar2 + 0x18);
        pdVar13[-3] = *(double *)(iVar2 + 0x14);
        dVar30 = *(double *)(iVar2 + 0x10);
        pdVar13 = pdVar13 + -4;
        *pdVar13 = dVar30;
      }
      uVar22 = (undefined4)((ulonglong)dVar18 >> 0x20);
      *(undefined4 *)((int)pdVar13 + -4) = uVar24;
      if (in_FPSCR_SZ == '\0') {
        pdVar13 = (double *)((int)pdVar13 + 0x34);
        *(undefined4 *)pdVar13 = *(undefined4 *)(iVar2 + 0x3c);
      }
      else {
        dVar30 = *(double *)(iVar2 + 0x3c);
        pdVar13 = pdVar13 + 6;
        *pdVar13 = dVar30;
      }
      if (in_FPSCR_PR == '\0') {
        dVar18 = (double)CONCAT44(uVar22,(float)(int)psVar3[3] * fVar21);
      }
      else {
        dVar18 = (double)CONCAT44(uVar22,(float)(int)psVar3[3]) * dVar25;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar13 = (double *)((int)pdVar13 + -4);
        *(int *)pdVar13 = SUB84(dVar18,0);
      }
      else {
        pdVar13 = pdVar13 + -1;
        *pdVar13 = dVar30;
      }
      uVar22 = (undefined4)((ulonglong)dVar18 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar18 = (double)CONCAT44(uVar22,(float)(int)psVar3[2] * fVar21);
      }
      else {
        dVar18 = (double)CONCAT44(uVar22,(float)(int)psVar3[2]) * dVar25;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar13 + -4) = SUB84(dVar18,0);
        *(undefined4 *)(pdVar13 + -1) = *(undefined4 *)(iVar2 + 0x38);
        *(undefined4 *)((int)pdVar13 + -0xc) = *(undefined4 *)(iVar2 + 0x34);
        dVar18 = (double)CONCAT44((int)((ulonglong)dVar18 >> 0x20),*(undefined4 *)(iVar2 + 0x30));
        pdVar13 = pdVar13 + -2;
        *(undefined4 *)pdVar13 = *(undefined4 *)(iVar2 + 0x30);
      }
      else {
        pdVar13[-1] = dVar30;
        pdVar13[-2] = *(double *)(iVar2 + 0x38);
        pdVar13[-3] = *(double *)(iVar2 + 0x34);
        dVar30 = *(double *)(iVar2 + 0x30);
        pdVar13 = pdVar13 + -4;
        *pdVar13 = dVar30;
      }
      uVar22 = (undefined4)((ulonglong)dVar18 >> 0x20);
      *(undefined4 *)((int)pdVar13 + -4) = uVar24;
      if (in_FPSCR_SZ == '\0') {
        pdVar13 = (double *)((int)pdVar13 + 0x34);
        *(undefined4 *)pdVar13 = *(undefined4 *)(iVar2 + 0x5c);
      }
      else {
        dVar30 = *(double *)(iVar2 + 0x5c);
        pdVar13 = pdVar13 + 6;
        *pdVar13 = dVar30;
      }
      if (in_FPSCR_PR == '\0') {
        dVar18 = (double)CONCAT44(uVar22,(float)(int)psVar3[5] * fVar21);
      }
      else {
        dVar18 = (double)CONCAT44(uVar22,(float)(int)psVar3[5]) * dVar25;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar13 = (double *)((int)pdVar13 + -4);
        *(int *)pdVar13 = SUB84(dVar18,0);
      }
      else {
        pdVar13 = pdVar13 + -1;
        *pdVar13 = dVar30;
      }
      uVar24 = DAT_8c611e0c._4_4_;
      if (in_FPSCR_PR == '\0') {
        fVar21 = (float)(int)psVar3[4] * fVar21;
      }
      else {
        fVar21 = SUB84((double)CONCAT44((int)((ulonglong)dVar18 >> 0x20),(float)(int)psVar3[4]) *
                       dVar25,0);
      }
      if (in_FPSCR_SZ == '\0') {
        *(float *)((int)pdVar13 + -4) = fVar21;
        *(undefined4 *)(pdVar13 + -1) = *(undefined4 *)(iVar2 + 0x58);
        *(undefined4 *)((int)pdVar13 + -0xc) = *(undefined4 *)(iVar2 + 0x54);
        pdVar13 = pdVar13 + -2;
        *(undefined4 *)pdVar13 = *(undefined4 *)(iVar2 + 0x50);
      }
      else {
        pdVar13[-1] = dVar30;
        pdVar13[-2] = *(double *)(iVar2 + 0x58);
        pdVar13[-3] = *(double *)(iVar2 + 0x54);
        pdVar13 = pdVar13 + -4;
        *pdVar13 = *(double *)(iVar2 + 0x50);
      }
      *(undefined4 *)((int)pdVar13 + -4) = uVar24;
      iVar9 = *piVar12 + 3;
    }
    else {
      if (in_FPSCR_SZ == '\0') {
        dVar29 = (double)CONCAT44(uVar22,*(undefined4 *)(iVar2 + 0x1c));
        pdVar13 = (double *)(unaff_r8 + 0x18);
        *(undefined4 *)pdVar13 = *(undefined4 *)(iVar2 + 0x1c);
      }
      else {
        dVar30 = *(double *)(iVar2 + 0x1c);
        pdVar13 = (double *)(unaff_r8 + 0x14);
        *pdVar13 = dVar30;
      }
      uVar22 = (undefined4)((ulonglong)dVar29 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar18 = (double)CONCAT44(uVar22,(float)(int)psVar3[1] * fVar21);
      }
      else {
        dVar18 = (double)CONCAT44(uVar22,(float)(int)psVar3[1]) * dVar25;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar13 = (double *)((int)pdVar13 + -4);
        *(int *)pdVar13 = SUB84(dVar18,0);
      }
      else {
        pdVar13 = pdVar13 + -1;
        *pdVar13 = dVar30;
      }
      uVar22 = (undefined4)((ulonglong)dVar18 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar18 = (double)CONCAT44(uVar22,(float)(int)*psVar3 * fVar21);
      }
      else {
        dVar18 = (double)CONCAT44(uVar22,(float)(int)*psVar3) * dVar25;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar13 + -4) = SUB84(dVar18,0);
        *(undefined4 *)(pdVar13 + -1) = *(undefined4 *)(iVar2 + 0x18);
        *(undefined4 *)((int)pdVar13 + -0xc) = *(undefined4 *)(iVar2 + 0x14);
        dVar18 = (double)CONCAT44((int)((ulonglong)dVar18 >> 0x20),*(undefined4 *)(iVar2 + 0x10));
        pdVar13 = pdVar13 + -2;
        *(undefined4 *)pdVar13 = *(undefined4 *)(iVar2 + 0x10);
      }
      else {
        pdVar13[-1] = dVar30;
        pdVar13[-2] = *(double *)(iVar2 + 0x18);
        pdVar13[-3] = *(double *)(iVar2 + 0x14);
        dVar30 = *(double *)(iVar2 + 0x10);
        pdVar13 = pdVar13 + -4;
        *pdVar13 = dVar30;
      }
      uVar22 = (undefined4)((ulonglong)dVar18 >> 0x20);
      *(undefined4 *)((int)pdVar13 + -4) = uVar24;
      if (in_FPSCR_SZ == '\0') {
        pdVar13 = (double *)((int)pdVar13 + 0x34);
        *(undefined4 *)pdVar13 = *(undefined4 *)(iVar2 + 0x3c);
      }
      else {
        dVar30 = *(double *)(iVar2 + 0x3c);
        pdVar13 = pdVar13 + 6;
        *pdVar13 = dVar30;
      }
      if (in_FPSCR_PR == '\0') {
        dVar18 = (double)CONCAT44(uVar22,(float)(int)psVar3[3] * fVar21);
      }
      else {
        dVar18 = (double)CONCAT44(uVar22,(float)(int)psVar3[3]) * dVar25;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar13 = (double *)((int)pdVar13 + -4);
        *(int *)pdVar13 = SUB84(dVar18,0);
      }
      else {
        pdVar13 = pdVar13 + -1;
        *pdVar13 = dVar30;
      }
      uVar22 = (undefined4)((ulonglong)dVar18 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar18 = (double)CONCAT44(uVar22,(float)(int)psVar3[2] * fVar21);
      }
      else {
        dVar18 = (double)CONCAT44(uVar22,(float)(int)psVar3[2]) * dVar25;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar13 + -4) = SUB84(dVar18,0);
        *(undefined4 *)(pdVar13 + -1) = *(undefined4 *)(iVar2 + 0x38);
        *(undefined4 *)((int)pdVar13 + -0xc) = *(undefined4 *)(iVar2 + 0x34);
        dVar18 = (double)CONCAT44((int)((ulonglong)dVar18 >> 0x20),*(undefined4 *)(iVar2 + 0x30));
        pdVar13 = pdVar13 + -2;
        *(undefined4 *)pdVar13 = *(undefined4 *)(iVar2 + 0x30);
      }
      else {
        pdVar13[-1] = dVar30;
        pdVar13[-2] = *(double *)(iVar2 + 0x38);
        pdVar13[-3] = *(double *)(iVar2 + 0x34);
        dVar30 = *(double *)(iVar2 + 0x30);
        pdVar13 = pdVar13 + -4;
        *pdVar13 = dVar30;
      }
      uVar22 = (undefined4)((ulonglong)dVar18 >> 0x20);
      *(undefined4 *)((int)pdVar13 + -4) = uVar24;
      if (in_FPSCR_SZ == '\0') {
        pdVar13 = (double *)((int)pdVar13 + 0x34);
        *(undefined4 *)pdVar13 = *(undefined4 *)(iVar2 + 0x7c);
      }
      else {
        dVar30 = *(double *)(iVar2 + 0x7c);
        pdVar13 = pdVar13 + 6;
        *pdVar13 = dVar30;
      }
      if (in_FPSCR_PR == '\0') {
        dVar18 = (double)CONCAT44(uVar22,(float)(int)psVar3[7] * fVar21);
      }
      else {
        dVar18 = (double)CONCAT44(uVar22,(float)(int)psVar3[7]) * dVar25;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar13 = (double *)((int)pdVar13 + -4);
        *(int *)pdVar13 = SUB84(dVar18,0);
      }
      else {
        pdVar13 = pdVar13 + -1;
        *pdVar13 = dVar30;
      }
      uVar22 = (undefined4)((ulonglong)dVar18 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar18 = (double)CONCAT44(uVar22,(float)(int)psVar3[6] * fVar21);
      }
      else {
        dVar18 = (double)CONCAT44(uVar22,(float)(int)psVar3[6]) * dVar25;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar13 + -4) = SUB84(dVar18,0);
        *(undefined4 *)(pdVar13 + -1) = *(undefined4 *)(iVar2 + 0x78);
        *(undefined4 *)((int)pdVar13 + -0xc) = *(undefined4 *)(iVar2 + 0x74);
        dVar18 = (double)CONCAT44((int)((ulonglong)dVar18 >> 0x20),*(undefined4 *)(iVar2 + 0x70));
        pdVar13 = pdVar13 + -2;
        *(undefined4 *)pdVar13 = *(undefined4 *)(iVar2 + 0x70);
      }
      else {
        pdVar13[-1] = dVar30;
        pdVar13[-2] = *(double *)(iVar2 + 0x78);
        pdVar13[-3] = *(double *)(iVar2 + 0x74);
        dVar30 = *(double *)(iVar2 + 0x70);
        pdVar13 = pdVar13 + -4;
        *pdVar13 = dVar30;
      }
      uVar22 = (undefined4)((ulonglong)dVar18 >> 0x20);
      *(undefined4 *)((int)pdVar13 + -4) = uVar24;
      if (in_FPSCR_SZ == '\0') {
        pdVar13 = (double *)((int)pdVar13 + 0x34);
        *(undefined4 *)pdVar13 = *(undefined4 *)(iVar2 + 0x5c);
      }
      else {
        dVar30 = *(double *)(iVar2 + 0x5c);
        pdVar13 = pdVar13 + 6;
        *pdVar13 = dVar30;
      }
      if (in_FPSCR_PR == '\0') {
        dVar18 = (double)CONCAT44(uVar22,(float)(int)psVar3[5] * fVar21);
      }
      else {
        dVar18 = (double)CONCAT44(uVar22,(float)(int)psVar3[5]) * dVar25;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar13 = (double *)((int)pdVar13 + -4);
        *(int *)pdVar13 = SUB84(dVar18,0);
      }
      else {
        pdVar13 = pdVar13 + -1;
        *pdVar13 = dVar30;
      }
      uVar24 = DAT_8c612058;
      if (in_FPSCR_PR == '\0') {
        fVar21 = (float)(int)psVar3[4] * fVar21;
      }
      else {
        fVar21 = SUB84((double)CONCAT44((int)((ulonglong)dVar18 >> 0x20),(float)(int)psVar3[4]) *
                       dVar25,0);
      }
      if (in_FPSCR_SZ == '\0') {
        *(float *)((int)pdVar13 + -4) = fVar21;
        *(undefined4 *)(pdVar13 + -1) = *(undefined4 *)(iVar2 + 0x58);
        *(undefined4 *)((int)pdVar13 + -0xc) = *(undefined4 *)(iVar2 + 0x54);
        pdVar13 = pdVar13 + -2;
        *(undefined4 *)pdVar13 = *(undefined4 *)(iVar2 + 0x50);
      }
      else {
        pdVar13[-1] = dVar30;
        pdVar13[-2] = *(double *)(iVar2 + 0x58);
        pdVar13[-3] = *(double *)(iVar2 + 0x54);
        pdVar13 = pdVar13 + -4;
        *pdVar13 = *(double *)(iVar2 + 0x50);
      }
      *(undefined4 *)((int)pdVar13 + -4) = uVar24;
      iVar9 = *piVar12 + 4;
    }
    *piVar12 = iVar9;
  }
  return;
}

