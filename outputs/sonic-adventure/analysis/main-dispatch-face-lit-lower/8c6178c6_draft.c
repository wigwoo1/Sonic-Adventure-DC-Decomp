/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Restarted to delay deadcode elimination for space: stack */

void FUN_8c6178c6(undefined4 param_1,undefined4 param_2,undefined4 param_3,int param_4,
                 undefined4 param_5,undefined4 param_6,undefined8 *param_7)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  double *pdVar4;
  undefined8 *unaff_r8;
  undefined8 *puVar5;
  int iVar6;
  int iVar7;
  double *pdVar8;
  int iVar9;
  int *piVar10;
  int iVar11;
  int iVar12;
  undefined4 in_fr1;
  double dVar13;
  undefined4 uVar14;
  float fVar15;
  float fVar16;
  float fVar17;
  undefined4 uVar18;
  double in_dr2;
  double dVar19;
  double dVar20;
  double dVar21;
  undefined4 uVar22;
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
  
  iVar2 = DAT_8c61795c;
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)DAT_8c617958);
    dVar23 = (double)CONCAT44(0x3f800000,param_1);
  }
  else {
    in_xd2 = *DAT_8c617958;
    dVar23 = (double)CONCAT44(0x3f800000,param_2);
  }
  if (in_FPSCR_PR == '\0') {
    dVar23 = (double)CONCAT44((float)((ulonglong)dVar23 >> 0x20) / SUB84(in_dr2,0),SUB84(dVar23,0));
  }
  else {
    dVar23 = dVar23 / in_dr2;
  }
  local_24[0] = param_4;
  local_24[1] = param_5;
  local_24[2] = param_6;
  iVar6 = 0;
  pdVar4 = (double *)local_48;
  pdVar8 = (double *)local_48;
  piVar10 = local_24;
  do {
    uVar18 = (undefined4)((ulonglong)in_dr2 >> 0x20);
    dVar21 = in_xd4;
    if ((*(uint *)(local_24[iVar6] + 0xc) & 1) == 0) {
      piVar1 = local_24 + iVar6;
      dVar21 = (double)CONCAT44(0x3f800000,param_2);
      if (in_FPSCR_SZ == '\0') {
        dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),0x3f800000);
        dVar21 = in_xd4;
      }
      iVar3 = *piVar1;
      if (in_FPSCR_SZ == '\0') {
        in_dr2 = (double)CONCAT44(uVar18,*(undefined4 *)(iVar3 + 0x18));
      }
      else {
        in_xd2 = *(double *)(iVar3 + 0x18);
      }
      if (in_FPSCR_PR == '\0') {
        dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),SUB84(dVar23,0) / SUB84(in_dr2,0)
                                 );
      }
      else {
        dVar23 = dVar23 / in_dr2;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar19 = (double)CONCAT44(*(undefined4 *)(iVar3 + 0x10),*(undefined4 *)(DAT_8c61795c + 0xc))
        ;
      }
      else {
        in_xd2 = *(double *)(DAT_8c61795c + 0xc);
        dVar19 = *(double *)(iVar3 + 0x10);
      }
      if (in_FPSCR_PR == '\0') {
        dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) - SUB84(dVar19,0),
                                  SUB84(dVar19,0));
      }
      else {
        dVar19 = dVar19 - dVar19;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),*(undefined4 *)DAT_8c617960);
      }
      else {
        in_xd2 = *DAT_8c617960;
      }
      fVar16 = SUB84(dVar23,0);
      if (in_FPSCR_PR == '\0') {
        dVar19 = (double)CONCAT44(((float)((ulonglong)dVar19 >> 0x20) * fVar16) / SUB84(dVar19,0),
                                  SUB84(dVar19,0));
      }
      else {
        dVar19 = (dVar19 * dVar23) / (dVar19 * dVar23);
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)pdVar8 = (int)((ulonglong)dVar19 >> 0x20);
        dVar19 = (double)(ulonglong)*(uint *)(DAT_8c61795c + 0x10);
      }
      else {
        *pdVar8 = dVar19;
        in_xd2 = *(double *)(DAT_8c61795c + 0x10);
      }
      iVar3 = *piVar1;
      if (in_FPSCR_SZ == '\0') {
        dVar19 = (double)CONCAT44(*(undefined4 *)(iVar3 + 0x14),SUB84(dVar19,0));
      }
      else {
        dVar19 = *(double *)(iVar3 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) - SUB84(dVar19,0),
                                  SUB84(dVar19,0));
      }
      else {
        dVar19 = dVar19 - dVar19;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),*(undefined4 *)DAT_8c617964);
      }
      else {
        in_xd2 = *DAT_8c617964;
      }
      if (in_FPSCR_PR == '\0') {
        in_dr2 = (double)CONCAT44(((float)((ulonglong)dVar19 >> 0x20) * fVar16) / SUB84(dVar19,0),
                                  SUB84(dVar19,0));
      }
      else {
        in_dr2 = (dVar19 * dVar23) / (dVar19 * dVar23);
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar8 + 4) = (int)((ulonglong)in_dr2 >> 0x20);
        *(float *)(pdVar8 + 1) = fVar16;
      }
      else {
        *(double *)((int)pdVar8 + 4) = in_dr2;
        pdVar8[1] = dVar21;
      }
    }
    else {
      if (in_FPSCR_SZ == '\0') {
        uVar14 = *(undefined4 *)(*piVar10 + 0x10);
        in_dr2 = (double)CONCAT44(uVar18,uVar14);
        *(undefined4 *)pdVar4 = uVar14;
      }
      else {
        in_xd2 = *(double *)(*piVar10 + 0x10);
        *pdVar4 = in_xd2;
      }
      if (in_FPSCR_SZ == '\0') {
        uVar18 = *(undefined4 *)(*piVar10 + 0x14);
        in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),uVar18);
        *(undefined4 *)((int)pdVar4 + 4) = uVar18;
      }
      else {
        in_xd2 = *(double *)(*piVar10 + 0x14);
        *(double *)((int)pdVar4 + 4) = in_xd2;
      }
      if (in_FPSCR_SZ == '\0') {
        uVar18 = *(undefined4 *)(*piVar10 + 0x18);
        in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),uVar18);
        *(undefined4 *)(pdVar4 + 1) = uVar18;
      }
      else {
        in_xd2 = *(double *)(*piVar10 + 0x18);
        pdVar4[1] = in_xd2;
      }
    }
    iVar6 = iVar6 + 1;
    pdVar4 = (double *)((int)pdVar4 + 0xc);
    piVar10 = piVar10 + 1;
    pdVar8 = (double *)((int)pdVar8 + 0xc);
    in_xd4 = dVar21;
  } while (iVar6 < 3);
  if (in_FPSCR_SZ == '\0') {
    dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),local_48._4_4_);
    dVar19 = (double)CONCAT44(uStack_34,param_3);
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),local_48._4_4_);
    dVar13 = (double)CONCAT44((undefined4)uStack_40,in_fr1);
    fVar16 = (float)local_48._4_4_;
    dVar25 = (double)CONCAT44((undefined4)uStack_40,uStack_38);
  }
  else {
    dVar13 = (double)CONCAT44(uStack_40._4_4_,(undefined4)uStack_40);
    in_xd2 = (double)CONCAT44((undefined4)uStack_40,local_48._4_4_);
    dVar19 = (double)CONCAT44(local_30,uStack_34);
    fVar16 = SUB84(dVar23,0);
    uStack_40._0_4_ = uStack_40._4_4_;
    dVar21 = in_xd2;
    dVar25 = dVar13;
  }
  fVar17 = (float)((ulonglong)dVar19 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    fVar15 = SUB84(in_dr2,0) * fVar17;
    dVar13 = (double)CONCAT44((float)((ulonglong)dVar13 >> 0x20) * SUB84(dVar25,0),SUB84(dVar13,0));
  }
  else {
    fVar15 = SUB84(in_dr2 * dVar19,0);
    dVar13 = dVar13 * dVar25;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar24 = (double)CONCAT44(local_48._0_4_,uStack_40._4_4_);
    dVar20 = (double)CONCAT44((undefined4)uStack_40,fVar15);
  }
  else {
    dVar24 = (double)CONCAT44(local_48._4_4_,local_48._0_4_);
    in_xd6 = (double)CONCAT44(uStack_38,uStack_40._4_4_);
    dVar20 = dVar25;
  }
  if (in_FPSCR_PR == '\0') {
    dVar20 = (double)CONCAT44((float)((ulonglong)dVar20 >> 0x20) * SUB84(dVar24,0),SUB84(dVar20,0));
  }
  else {
    dVar20 = dVar20 * dVar24;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),SUB84(dVar24,0));
    in_xd6 = in_xd0;
  }
  if (in_FPSCR_PR == '\0') {
    dVar13 = (double)CONCAT44((float)((ulonglong)dVar13 >> 0x20) - SUB84(dVar20,0),SUB84(dVar13,0));
  }
  else {
    dVar13 = dVar13 - dVar20;
  }
  uVar18 = (undefined4)((ulonglong)dVar24 >> 0x20);
  dVar26 = dVar24;
  if (in_FPSCR_SZ == '\0') {
    dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),uVar18);
    dVar26 = in_xd2;
  }
  if (in_FPSCR_PR == '\0') {
    dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),SUB84(dVar13,0) * fVar16);
    fVar16 = SUB84(dVar20,0) * fVar17 - (float)((ulonglong)dVar20 >> 0x20);
  }
  else {
    dVar13 = dVar13 * dVar23;
    fVar16 = SUB84(dVar20 * dVar19 - dVar20 * dVar19,0);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar20 = (double)CONCAT44(uStack_2c,fVar16);
  }
  else {
    dVar20 = (double)CONCAT44(uStack_28,uStack_2c);
  }
  if (in_FPSCR_PR == '\0') {
    dVar20 = (double)CONCAT44((float)((ulonglong)dVar20 >> 0x20) * SUB84(dVar20,0),SUB84(dVar20,0));
  }
  else {
    dVar20 = dVar20 * dVar20;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),local_30);
  }
  else {
    dVar26 = (double)CONCAT44(uStack_2c,local_30);
  }
  if (in_FPSCR_PR == '\0') {
    dVar20 = (double)CONCAT44((float)((ulonglong)dVar13 >> 0x20) * SUB84(dVar20,0) +
                              (float)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0));
  }
  if (in_FPSCR_SZ == '\0') {
    dVar19 = (double)CONCAT44(fVar17,uVar18);
  }
  if (in_FPSCR_PR == '\0') {
    dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),SUB84(dVar19,0) * SUB84(dVar25,0));
  }
  else {
    dVar19 = dVar19 * dVar25;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),uStack_28);
  }
  else {
    dVar26 = (double)CONCAT44(local_24[0],uStack_28);
  }
  if (in_FPSCR_PR == '\0') {
    fVar16 = SUB84(dVar13,0) - SUB84(dVar19,0);
  }
  else {
    fVar16 = SUB84(dVar13 - dVar19,0);
  }
  if (in_FPSCR_SZ == '\0') {
    in_xd6 = (double)CONCAT44(fVar16,fVar16);
  }
  if (in_FPSCR_PR == '\0') {
    dVar20 = (double)((ulonglong)
                      (uint)((float)((ulonglong)in_xd6 >> 0x20) * SUB84(dVar20,0) +
                            (float)((ulonglong)dVar20 >> 0x20)) << 0x20);
  }
  dVar25 = (double)((ulonglong)dVar20 & 0xffffffff00000000);
  dVar19 = dVar25;
  if (in_FPSCR_SZ == '\0') {
    dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),(int)((ulonglong)dVar20 >> 0x20));
    dVar19 = dVar21;
  }
  if (in_FPSCR_PR == '\0') {
    bVar27 = SUB84(dVar23,0) < 0.0;
  }
  else {
    bVar27 = dVar23 < dVar25;
  }
  if (((uint)bVar27 & *DAT_8c617c14) == 0) {
    *DAT_8c617c14 = 0;
    iVar3 = DAT_8c617c18;
    iVar9 = 0;
    iVar6 = 2;
    iVar11 = 0;
    do {
      if ((*(uint *)(local_24[iVar6] + 0xc) & 1 ^ *(uint *)(local_24[iVar11] + 0xc) & 1) == 1) {
        iVar6 = iVar6 * 0xc;
        pdVar4 = (double *)(iVar6 + (int)local_48);
        if (in_FPSCR_SZ == '\0') {
          dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),
                                    *(undefined4 *)((int)&uStack_40 + iVar6));
        }
        else {
          dVar19 = *(double *)((int)&uStack_40 + iVar6);
        }
        iVar12 = iVar11 * 0xc;
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)DAT_8c617c1c);
        }
        else {
          dVar26 = *DAT_8c617c1c;
        }
        pdVar8 = (double *)(iVar12 + (int)local_48);
        if (in_FPSCR_PR == '\0') {
          fVar16 = SUB84(dVar25,0) - SUB84(dVar23,0);
        }
        else {
          fVar16 = SUB84(dVar25 - dVar23,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar21 = (double)CONCAT44(*(undefined4 *)((int)&uStack_40 + iVar12),fVar16);
        }
        else {
          dVar21 = *(double *)((int)&uStack_40 + iVar12);
        }
        if (in_FPSCR_PR == '\0') {
          dVar21 = (double)CONCAT44((float)((ulonglong)dVar21 >> 0x20) - SUB84(dVar23,0),
                                    SUB84(dVar21,0));
        }
        else {
          dVar21 = dVar21 - dVar23;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),SUB84(dVar21,0));
          dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),*(undefined4 *)pdVar8);
          dVar25 = (double)CONCAT44(*(undefined4 *)pdVar4,SUB84(dVar24,0));
          dVar13 = dVar26;
          dVar26 = dVar19;
        }
        else {
          dVar13 = *pdVar8;
          dVar25 = *pdVar4;
        }
        fVar16 = (float)((ulonglong)dVar25 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),
                                    SUB84(dVar23,0) / (float)((ulonglong)dVar21 >> 0x20));
          fVar17 = SUB84(dVar21,0) - fVar16;
        }
        else {
          dVar23 = dVar23 / dVar21;
          fVar17 = SUB84(dVar21 - dVar25,0);
        }
        uVar18 = SUB84(dVar23,0);
        dVar19 = dVar26;
        dVar21 = dVar25;
        if (in_FPSCR_SZ == '\0') {
          dVar19 = (double)CONCAT44(uVar18,SUB84(in_xd6,0));
          dVar21 = (double)CONCAT44(fVar16,fVar17);
        }
        if (in_FPSCR_PR == '\0') {
          dVar21 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) * SUB84(dVar21,0) +
                                    (float)((ulonglong)dVar21 >> 0x20),SUB84(dVar21,0));
        }
        uVar14 = (undefined4)((ulonglong)dVar23 >> 0x20);
        if (in_FPSCR_SZ == '\0') {
          dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),SUB84(dVar25,0));
          dVar25 = (double)CONCAT44(*(undefined4 *)(iVar2 + 0xc),*(undefined4 *)DAT_8c617c20);
          dVar20 = (double)CONCAT44(uVar14,SUB84(dVar19,0));
        }
        else {
          dVar25 = *(double *)(iVar2 + 0xc);
          dVar13 = *DAT_8c617c20;
          dVar20 = dVar23;
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                    SUB84(dVar25,0) * (float)((ulonglong)dVar21 >> 0x20));
        }
        else {
          dVar25 = dVar25 * dVar21;
        }
        iVar7 = iVar9 * 0x20 + iVar3;
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((float)((ulonglong)dVar20 >> 0x20) * SUB84(dVar25,0) +
                                    (float)((ulonglong)dVar25 >> 0x20),SUB84(dVar25,0));
        }
        uVar22 = (undefined4)((ulonglong)dVar25 >> 0x20);
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)(iVar7 + 0x10) = uVar22;
          fVar16 = *(float *)(local_48 + iVar6 + 4);
          dVar23 = (double)CONCAT44(uVar14,fVar16);
          dVar25 = (double)CONCAT44(uVar22,*(undefined4 *)(local_48 + iVar12 + 4));
          dVar19 = dVar26;
          dVar24 = (double)CONCAT44(fVar16,SUB84(dVar21,0));
          dVar26 = (double)CONCAT44(uVar18,SUB84(dVar20,0));
        }
        else {
          *(double *)(iVar7 + 0x10) = dVar25;
          dVar19 = *(double *)(local_48 + iVar6 + 4);
          fVar16 = (float)((ulonglong)dVar19 >> 0x20);
          dVar13 = *(double *)(local_48 + iVar12 + 4);
          dVar24 = dVar19;
        }
        if (in_FPSCR_PR == '\0') {
          fVar16 = SUB84(dVar25,0) - fVar16;
        }
        else {
          fVar16 = SUB84(dVar25 - dVar24,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(*(undefined4 *)(iVar2 + 0x10),fVar16);
        }
        else {
          dVar25 = *(double *)(iVar2 + 0x10);
        }
        if (in_FPSCR_PR == '\0') {
          dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),
                                    (float)((ulonglong)dVar26 >> 0x20) * SUB84(dVar25,0) +
                                    SUB84(dVar23,0));
        }
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)DAT_8c617c24);
          in_xd6 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),SUB84(dVar26,0));
          dVar26 = dVar13;
        }
        else {
          dVar26 = *DAT_8c617c24;
          in_xd6 = dVar23;
        }
        fVar16 = (float)((ulonglong)in_xd6 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          fVar17 = SUB84(dVar25,0) * SUB84(dVar23,0);
          dVar25 = (double)CONCAT44(fVar16 * fVar17 + (float)((ulonglong)dVar25 >> 0x20),fVar17);
        }
        else {
          dVar25 = dVar25 * dVar23;
        }
        iVar6 = iVar9 + 1;
        iVar9 = iVar9 + 1;
        if (in_FPSCR_SZ == '\0') {
          *(int *)(iVar7 + 0x14) = (int)((ulonglong)dVar25 >> 0x20);
          *(float *)(iVar7 + 0x18) = fVar16;
          iVar9 = iVar6;
        }
        else {
          *(double *)(iVar7 + 0x14) = dVar25;
          *(double *)(iVar7 + 0x18) = in_xd6;
        }
      }
      piVar10 = local_24 + iVar11;
      if ((*(uint *)(*piVar10 + 0xc) & 1) == 0) {
        iVar6 = iVar9 * 0x20 + iVar3;
        iVar9 = iVar9 + 1;
        if (in_FPSCR_SZ == '\0') {
          uVar18 = *(undefined4 *)(*piVar10 + 0x10);
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),uVar18);
          *(undefined4 *)(iVar6 + 0x10) = uVar18;
        }
        else {
          dVar26 = *(double *)(*piVar10 + 0x10);
          *(double *)(iVar6 + 0x10) = dVar26;
        }
        if (in_FPSCR_SZ == '\0') {
          uVar18 = *(undefined4 *)(*piVar10 + 0x14);
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),uVar18);
          *(undefined4 *)(iVar6 + 0x14) = uVar18;
        }
        else {
          dVar26 = *(double *)(*piVar10 + 0x14);
          *(double *)(iVar6 + 0x14) = dVar26;
        }
        if (in_FPSCR_SZ == '\0') {
          uVar18 = *(undefined4 *)(*piVar10 + 0x18);
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),uVar18);
          *(undefined4 *)(iVar6 + 0x18) = uVar18;
        }
        else {
          dVar26 = *(double *)(*piVar10 + 0x18);
          *(double *)(iVar6 + 0x18) = dVar26;
        }
      }
      uVar18 = DAT_8c617c2c;
      piVar10 = DAT_8c617c28;
      iVar12 = iVar11 + 1;
      iVar6 = iVar11;
      iVar11 = iVar12;
    } while (iVar12 < 3);
    if (iVar9 == 3) {
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)((int)unaff_r8 + 0xc) = *(undefined4 *)(iVar3 + 0x18);
        *(undefined4 *)(unaff_r8 + 1) = *(undefined4 *)(iVar3 + 0x14);
        puVar5 = (undefined8 *)((int)unaff_r8 + 4);
        *(undefined4 *)puVar5 = *(undefined4 *)(iVar3 + 0x10);
      }
      else {
        unaff_r8[1] = *(undefined8 *)(iVar3 + 0x18);
        *unaff_r8 = *(undefined8 *)(iVar3 + 0x14);
        puVar5 = unaff_r8 + -1;
        *puVar5 = *(undefined8 *)(iVar3 + 0x10);
      }
      *(undefined4 *)((int)puVar5 + -4) = uVar18;
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)(puVar5 + 5) = *(undefined4 *)(iVar3 + 0x38);
        *(undefined4 *)((int)puVar5 + 0x24) = *(undefined4 *)(iVar3 + 0x34);
        puVar5 = puVar5 + 4;
        *(undefined4 *)puVar5 = *(undefined4 *)(iVar3 + 0x30);
      }
      else {
        *(undefined8 *)((int)puVar5 + 0x24) = *(undefined8 *)(iVar3 + 0x38);
        *(undefined8 *)((int)puVar5 + 0x1c) = *(undefined8 *)(iVar3 + 0x34);
        puVar5 = (undefined8 *)((int)puVar5 + 0x14);
        *puVar5 = *(undefined8 *)(iVar3 + 0x30);
      }
      *(undefined4 *)((int)puVar5 + -4) = uVar18;
      uVar18 = DAT_8c617c30;
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)(puVar5 + 7) = *(undefined4 *)((int)param_7 + 0xc);
        *(undefined4 *)((int)puVar5 + 0x34) = *(undefined4 *)(param_7 + 1);
        *(undefined4 *)(puVar5 + 6) = *(undefined4 *)((int)param_7 + 4);
        *(undefined4 *)((int)puVar5 + 0x2c) = *(undefined4 *)param_7;
        *(undefined4 *)(puVar5 + 5) = *(undefined4 *)(iVar3 + 0x58);
        *(undefined4 *)((int)puVar5 + 0x24) = *(undefined4 *)(iVar3 + 0x54);
        puVar5 = puVar5 + 4;
        *(undefined4 *)puVar5 = *(undefined4 *)(iVar3 + 0x50);
      }
      else {
        *(undefined8 *)((int)puVar5 + 0x34) = *(undefined8 *)((int)param_7 + 0xc);
        *(undefined8 *)((int)puVar5 + 0x2c) = param_7[1];
        *(undefined8 *)((int)puVar5 + 0x24) = *(undefined8 *)((int)param_7 + 4);
        *(undefined8 *)((int)puVar5 + 0x1c) = *param_7;
        *(undefined8 *)((int)puVar5 + 0x14) = *(undefined8 *)(iVar3 + 0x58);
        *(undefined8 *)((int)puVar5 + 0xc) = *(undefined8 *)(iVar3 + 0x54);
        puVar5 = (undefined8 *)((int)puVar5 + 4);
        *puVar5 = *(undefined8 *)(iVar3 + 0x50);
      }
      *(undefined4 *)((int)puVar5 + -4) = uVar18;
      iVar2 = *piVar10 + 3;
    }
    else {
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)((int)unaff_r8 + 0xc) = *(undefined4 *)(iVar3 + 0x18);
        *(undefined4 *)(unaff_r8 + 1) = *(undefined4 *)(iVar3 + 0x14);
        puVar5 = (undefined8 *)((int)unaff_r8 + 4);
        *(undefined4 *)puVar5 = *(undefined4 *)(iVar3 + 0x10);
      }
      else {
        unaff_r8[1] = *(undefined8 *)(iVar3 + 0x18);
        *unaff_r8 = *(undefined8 *)(iVar3 + 0x14);
        puVar5 = unaff_r8 + -1;
        *puVar5 = *(undefined8 *)(iVar3 + 0x10);
      }
      *(undefined4 *)((int)puVar5 + -4) = uVar18;
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)(puVar5 + 5) = *(undefined4 *)(iVar3 + 0x38);
        *(undefined4 *)((int)puVar5 + 0x24) = *(undefined4 *)(iVar3 + 0x34);
        puVar5 = puVar5 + 4;
        *(undefined4 *)puVar5 = *(undefined4 *)(iVar3 + 0x30);
      }
      else {
        *(undefined8 *)((int)puVar5 + 0x24) = *(undefined8 *)(iVar3 + 0x38);
        *(undefined8 *)((int)puVar5 + 0x1c) = *(undefined8 *)(iVar3 + 0x34);
        puVar5 = (undefined8 *)((int)puVar5 + 0x14);
        *puVar5 = *(undefined8 *)(iVar3 + 0x30);
      }
      *(undefined4 *)((int)puVar5 + -4) = uVar18;
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)(puVar5 + 7) = *(undefined4 *)((int)param_7 + 0xc);
        *(undefined4 *)((int)puVar5 + 0x34) = *(undefined4 *)(param_7 + 1);
        *(undefined4 *)(puVar5 + 6) = *(undefined4 *)((int)param_7 + 4);
        *(undefined4 *)((int)puVar5 + 0x2c) = *(undefined4 *)param_7;
        *(undefined4 *)(puVar5 + 5) = *(undefined4 *)(iVar3 + 0x78);
        *(undefined4 *)((int)puVar5 + 0x24) = *(undefined4 *)(iVar3 + 0x74);
        puVar5 = puVar5 + 4;
        *(undefined4 *)puVar5 = *(undefined4 *)(iVar3 + 0x70);
      }
      else {
        *(undefined8 *)((int)puVar5 + 0x34) = *(undefined8 *)((int)param_7 + 0xc);
        *(undefined8 *)((int)puVar5 + 0x2c) = param_7[1];
        *(undefined8 *)((int)puVar5 + 0x24) = *(undefined8 *)((int)param_7 + 4);
        *(undefined8 *)((int)puVar5 + 0x1c) = *param_7;
        *(undefined8 *)((int)puVar5 + 0x14) = *(undefined8 *)(iVar3 + 0x78);
        *(undefined8 *)((int)puVar5 + 0xc) = *(undefined8 *)(iVar3 + 0x74);
        puVar5 = (undefined8 *)((int)puVar5 + 4);
        *puVar5 = *(undefined8 *)(iVar3 + 0x70);
      }
      *(undefined4 *)((int)puVar5 + -4) = uVar18;
      uVar18 = DAT_8c617c30;
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)(puVar5 + 7) = *(undefined4 *)((int)param_7 + 0xc);
        *(undefined4 *)((int)puVar5 + 0x34) = *(undefined4 *)(param_7 + 1);
        *(undefined4 *)(puVar5 + 6) = *(undefined4 *)((int)param_7 + 4);
        *(undefined4 *)((int)puVar5 + 0x2c) = *(undefined4 *)param_7;
        *(undefined4 *)(puVar5 + 5) = *(undefined4 *)(iVar3 + 0x58);
        *(undefined4 *)((int)puVar5 + 0x24) = *(undefined4 *)(iVar3 + 0x54);
        puVar5 = puVar5 + 4;
        *(undefined4 *)puVar5 = *(undefined4 *)(iVar3 + 0x50);
      }
      else {
        *(undefined8 *)((int)puVar5 + 0x34) = *(undefined8 *)((int)param_7 + 0xc);
        *(undefined8 *)((int)puVar5 + 0x2c) = param_7[1];
        *(undefined8 *)((int)puVar5 + 0x24) = *(undefined8 *)((int)param_7 + 4);
        *(undefined8 *)((int)puVar5 + 0x1c) = *param_7;
        *(undefined8 *)((int)puVar5 + 0x14) = *(undefined8 *)(iVar3 + 0x58);
        *(undefined8 *)((int)puVar5 + 0xc) = *(undefined8 *)(iVar3 + 0x54);
        puVar5 = (undefined8 *)((int)puVar5 + 4);
        *puVar5 = *(undefined8 *)(iVar3 + 0x50);
      }
      *(undefined4 *)((int)puVar5 + -4) = uVar18;
      iVar2 = *piVar10 + 4;
    }
    *piVar10 = iVar2;
  }
  return;
}

