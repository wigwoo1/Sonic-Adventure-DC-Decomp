/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Restarted to delay deadcode elimination for space: stack */

void FUN_8c612e64(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                 int param_5,undefined4 param_6,undefined4 param_7)

{
  int iVar1;
  double *pdVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  int iVar6;
  double *pdVar7;
  int unaff_r8;
  double *pdVar8;
  int iVar9;
  int iVar10;
  int *piVar11;
  double *pdVar12;
  int iVar13;
  int iVar14;
  float fVar15;
  double in_dr0;
  double dVar16;
  double dVar17;
  double dVar18;
  float fVar19;
  undefined4 uVar20;
  undefined4 uVar21;
  double in_dr2;
  double dVar22;
  double dVar23;
  undefined4 uVar25;
  undefined8 uVar24;
  double dVar26;
  double dVar27;
  double dVar28;
  double in_xd0;
  double in_xd2;
  double dVar29;
  double in_xd4;
  bool bVar30;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined4 local_48;
  undefined1 auStack_44 [8];
  undefined4 local_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined4 local_30;
  undefined4 uStack_2c;
  undefined4 uStack_28;
  int local_24 [3];
  
  iVar4 = DAT_8c612f38;
  pdVar8 = DAT_8c612f34;
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)DAT_8c612f30);
    dVar26 = (double)CONCAT44(0x3f800000,param_1);
  }
  else {
    in_xd2 = *DAT_8c612f30;
    dVar26 = (double)CONCAT44(0x3f800000,param_2);
  }
  if (in_FPSCR_PR == '\0') {
    dVar26 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) / SUB84(in_dr2,0),SUB84(dVar26,0));
  }
  else {
    dVar26 = dVar26 / in_dr2;
  }
  iVar10 = 0;
  local_24[0] = param_5;
  local_24[1] = param_6;
  local_24[2] = param_7;
  pdVar7 = (double *)&local_48;
  piVar11 = local_24;
  pdVar12 = (double *)&local_48;
  do {
    piVar5 = local_24 + iVar10;
    pdVar2 = (double *)*piVar5;
    if (in_FPSCR_SZ == '\0') {
      dVar22 = (double)CONCAT44(*(undefined4 *)pdVar8,*(undefined4 *)pdVar2);
    }
    else {
      dVar22 = *pdVar8;
      in_xd2 = *pdVar2;
    }
    if (in_FPSCR_PR == '\0') {
      dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) * SUB84(dVar22,0),SUB84(dVar22,0)
                               );
    }
    else {
      dVar22 = dVar22 * dVar22;
    }
    if (in_FPSCR_SZ == '\0') {
      in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),*(undefined4 *)((int)pdVar8 + 4));
      dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)DAT_8c612f3c);
    }
    else {
      in_xd0 = *(double *)((int)pdVar8 + 4);
      in_xd2 = *DAT_8c612f3c;
    }
    if (in_FPSCR_PR == '\0') {
      fVar19 = SUB84(dVar22,0) - (float)((ulonglong)dVar22 >> 0x20);
    }
    else {
      fVar19 = SUB84(dVar22 - dVar22,0);
    }
    if (in_FPSCR_SZ == '\0') {
      dVar22 = (double)CONCAT44(*(undefined4 *)((int)pdVar2 + 4),fVar19);
    }
    else {
      dVar22 = *(double *)((int)pdVar2 + 4);
    }
    if (in_FPSCR_PR == '\0') {
      dVar16 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                SUB84(in_dr0,0) * (float)((ulonglong)dVar22 >> 0x20));
    }
    else {
      dVar16 = in_dr0 * dVar22;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar22 = (double)CONCAT44(*(undefined4 *)(pdVar2 + 1),SUB84(dVar22,0));
    }
    else {
      dVar22 = pdVar2[1];
    }
    if (in_FPSCR_PR == '\0') {
      dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),SUB84(dVar22,0) - SUB84(dVar16,0));
    }
    else {
      dVar22 = dVar22 - dVar16;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),*(undefined4 *)(pdVar8 + 1));
    }
    else {
      in_xd0 = pdVar8[1];
    }
    fVar19 = (float)((ulonglong)dVar22 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      fVar15 = SUB84(dVar16,0) * fVar19;
      in_dr0 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),fVar15);
      dVar22 = (double)CONCAT44(fVar19,SUB84(dVar22,0) - fVar15);
    }
    else {
      in_dr0 = dVar16 * dVar22;
      fVar15 = SUB84(in_dr0,0);
      dVar22 = dVar22 - in_dr0;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)pdVar2 + 0x1c) = SUB84(dVar22,0);
    }
    else {
      *(double *)((int)pdVar2 + 0x1c) = in_xd2;
    }
    uVar21 = (undefined4)((ulonglong)dVar22 >> 0x20);
    dVar16 = in_xd4;
    if ((*(uint *)(*piVar5 + 0xc) & 1) == 0) {
      piVar5 = local_24 + iVar10;
      dVar16 = (double)CONCAT44(0x3f800000,param_2);
      if (in_FPSCR_SZ == '\0') {
        dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),0x3f800000);
        dVar16 = in_xd4;
      }
      iVar3 = *piVar5;
      if (in_FPSCR_SZ == '\0') {
        dVar22 = (double)CONCAT44(uVar21,*(undefined4 *)(iVar3 + 0x18));
      }
      else {
        in_xd2 = *(double *)(iVar3 + 0x18);
      }
      if (in_FPSCR_PR == '\0') {
        dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),SUB84(dVar26,0) / SUB84(dVar22,0)
                                 );
      }
      else {
        dVar26 = dVar26 / dVar22;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar22 = (double)CONCAT44(*(undefined4 *)(iVar3 + 0x10),*(undefined4 *)(iVar4 + 0xc));
      }
      else {
        in_xd2 = *(double *)(iVar4 + 0xc);
        dVar22 = *(double *)(iVar3 + 0x10);
      }
      if (in_FPSCR_PR == '\0') {
        dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) - SUB84(dVar22,0),
                                  SUB84(dVar22,0));
      }
      else {
        dVar22 = dVar22 - dVar22;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)DAT_8c612f40);
      }
      else {
        in_xd2 = *DAT_8c612f40;
      }
      fVar19 = SUB84(dVar26,0);
      if (in_FPSCR_PR == '\0') {
        dVar22 = (double)CONCAT44(((float)((ulonglong)dVar22 >> 0x20) * fVar19) / SUB84(dVar22,0),
                                  SUB84(dVar22,0));
      }
      else {
        dVar22 = (dVar22 * dVar26) / (dVar22 * dVar26);
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)pdVar12 = (int)((ulonglong)dVar22 >> 0x20);
        dVar22 = (double)(ulonglong)*(uint *)(iVar4 + 0x10);
      }
      else {
        *pdVar12 = dVar22;
        in_xd2 = *(double *)(iVar4 + 0x10);
      }
      iVar3 = *piVar5;
      if (in_FPSCR_SZ == '\0') {
        dVar22 = (double)CONCAT44(*(undefined4 *)(iVar3 + 0x14),SUB84(dVar22,0));
      }
      else {
        dVar22 = *(double *)(iVar3 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) - SUB84(dVar22,0),
                                  SUB84(dVar22,0));
      }
      else {
        dVar22 = dVar22 - dVar22;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)DAT_8c612f44);
      }
      else {
        in_xd2 = *DAT_8c612f44;
      }
      if (in_FPSCR_PR == '\0') {
        dVar22 = (double)CONCAT44(((float)((ulonglong)dVar22 >> 0x20) * fVar19) / SUB84(dVar22,0),
                                  SUB84(dVar22,0));
      }
      else {
        dVar22 = (dVar22 * dVar26) / (dVar22 * dVar26);
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar12 + 4) = (int)((ulonglong)dVar22 >> 0x20);
        *(float *)(pdVar12 + 1) = fVar19;
      }
      else {
        *(double *)((int)pdVar12 + 4) = dVar22;
        pdVar12[1] = dVar16;
      }
    }
    else {
      if (in_FPSCR_SZ == '\0') {
        uVar20 = *(undefined4 *)(*piVar11 + 0x10);
        dVar22 = (double)CONCAT44(uVar21,uVar20);
        *(undefined4 *)pdVar7 = uVar20;
      }
      else {
        in_xd2 = *(double *)(*piVar11 + 0x10);
        *pdVar7 = in_xd2;
      }
      if (in_FPSCR_SZ == '\0') {
        uVar21 = *(undefined4 *)(*piVar11 + 0x14);
        dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),uVar21);
        *(undefined4 *)((int)pdVar7 + 4) = uVar21;
      }
      else {
        in_xd2 = *(double *)(*piVar11 + 0x14);
        *(double *)((int)pdVar7 + 4) = in_xd2;
      }
      if (in_FPSCR_SZ == '\0') {
        uVar21 = *(undefined4 *)(*piVar11 + 0x18);
        dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),uVar21);
        *(undefined4 *)(pdVar7 + 1) = uVar21;
      }
      else {
        in_xd2 = *(double *)(*piVar11 + 0x18);
        pdVar7[1] = in_xd2;
      }
    }
    iVar10 = iVar10 + 1;
    pdVar7 = (double *)((int)pdVar7 + 0xc);
    piVar11 = piVar11 + 1;
    pdVar12 = (double *)((int)pdVar12 + 0xc);
    in_xd4 = dVar16;
  } while (iVar10 < 3);
  if (in_FPSCR_SZ == '\0') {
    dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),uStack_38);
    dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),auStack_44._0_4_);
    dVar17 = (double)CONCAT44(auStack_44._4_4_,fVar15);
    dVar27 = (double)CONCAT44(auStack_44._4_4_,uStack_34);
    param_3 = auStack_44._0_4_;
  }
  else {
    dVar17 = (double)CONCAT44(local_3c,auStack_44._4_4_);
    dVar16 = (double)CONCAT44(uStack_34,uStack_38);
    in_xd2 = (double)CONCAT44(auStack_44._4_4_,auStack_44._0_4_);
    auStack_44._4_4_ = local_3c;
    dVar27 = dVar17;
  }
  if (in_FPSCR_PR == '\0') {
    fVar19 = SUB84(dVar22,0) * SUB84(dVar27,0);
    dVar17 = (double)CONCAT44((float)((ulonglong)dVar17 >> 0x20) * SUB84(dVar26,0),SUB84(dVar17,0));
  }
  else {
    fVar19 = SUB84(dVar22 * dVar27,0);
    dVar17 = dVar17 * dVar26;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar28 = (double)CONCAT44(local_48,param_4);
    dVar22 = (double)CONCAT44(local_3c,param_3);
    dVar23 = (double)CONCAT44(auStack_44._4_4_,fVar19);
  }
  else {
    dVar28 = (double)CONCAT44(auStack_44._0_4_,local_48);
    dVar22 = (double)CONCAT44(uStack_38,local_3c);
    dVar23 = dVar27;
  }
  fVar19 = (float)((ulonglong)dVar22 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar23 = (double)CONCAT44((float)((ulonglong)dVar23 >> 0x20) * fVar19,SUB84(dVar23,0));
  }
  else {
    dVar23 = dVar23 * dVar22;
  }
  dVar18 = dVar22;
  if (in_FPSCR_SZ == '\0') {
    dVar17 = (double)CONCAT44((int)((ulonglong)dVar17 >> 0x20),fVar19);
    dVar18 = in_xd0;
  }
  if (in_FPSCR_PR == '\0') {
    dVar17 = (double)CONCAT44((float)((ulonglong)dVar17 >> 0x20) - SUB84(dVar23,0),SUB84(dVar17,0));
  }
  else {
    dVar17 = dVar17 - dVar23;
  }
  uVar21 = (undefined4)((ulonglong)dVar28 >> 0x20);
  dVar29 = dVar28;
  if (in_FPSCR_SZ == '\0') {
    dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),uVar21);
    dVar29 = in_xd2;
  }
  if (in_FPSCR_PR == '\0') {
    dVar17 = (double)CONCAT44((int)((ulonglong)dVar17 >> 0x20),SUB84(dVar17,0) * SUB84(dVar22,0));
    fVar19 = SUB84(dVar23,0) * SUB84(dVar27,0) - (float)((ulonglong)dVar23 >> 0x20);
  }
  else {
    dVar17 = dVar17 * dVar22;
    fVar19 = SUB84(dVar23 * dVar27 - dVar23 * dVar27,0);
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
    dVar29 = (double)CONCAT44(uStack_2c,local_30);
  }
  if (in_FPSCR_PR == '\0') {
    dVar22 = (double)CONCAT44((float)((ulonglong)dVar17 >> 0x20) * SUB84(dVar22,0) +
                              (float)((ulonglong)dVar22 >> 0x20),SUB84(dVar22,0));
  }
  if (in_FPSCR_SZ == '\0') {
    dVar28 = (double)CONCAT44(uVar21,uVar21);
  }
  if (in_FPSCR_PR == '\0') {
    dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),SUB84(dVar28,0) * SUB84(dVar26,0));
  }
  else {
    dVar28 = dVar28 * dVar26;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),uStack_28);
  }
  else {
    dVar29 = (double)CONCAT44(local_24[0],uStack_28);
  }
  if (in_FPSCR_PR == '\0') {
    fVar19 = SUB84(dVar17,0) - SUB84(dVar28,0);
  }
  else {
    fVar19 = SUB84(dVar17 - dVar28,0);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar18 = (double)CONCAT44(fVar19,fVar19);
  }
  if (in_FPSCR_PR == '\0') {
    dVar22 = (double)((ulonglong)
                      (uint)((float)((ulonglong)dVar18 >> 0x20) * SUB84(dVar22,0) +
                            (float)((ulonglong)dVar22 >> 0x20)) << 0x20);
  }
  dVar17 = (double)((ulonglong)dVar22 & 0xffffffff00000000);
  dVar28 = dVar17;
  if (in_FPSCR_SZ == '\0') {
    dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),(int)((ulonglong)dVar22 >> 0x20));
    dVar28 = dVar16;
  }
  if (in_FPSCR_PR == '\0') {
    bVar30 = SUB84(dVar26,0) < 0.0;
  }
  else {
    bVar30 = dVar26 < dVar17;
  }
  if (((uint)bVar30 & *DAT_8c6131d0) == 0) {
    *DAT_8c6131d0 = 0;
    iVar3 = DAT_8c6131d4;
    iVar10 = 0;
    iVar6 = 2;
    iVar13 = 0;
    do {
      if ((*(uint *)(local_24[iVar6] + 0xc) & 1 ^ *(uint *)(local_24[iVar13] + 0xc) & 1) == 1) {
        iVar1 = iVar6 * 0xc;
        pdVar8 = (double *)(iVar1 + (int)&local_48);
        if (in_FPSCR_SZ == '\0') {
          dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),
                                    *(undefined4 *)(auStack_44 + iVar1 + 4));
        }
        else {
          dVar28 = *(double *)(auStack_44 + iVar1 + 4);
        }
        iVar14 = iVar13 * 0xc;
        if (in_FPSCR_SZ == '\0') {
          dVar17 = (double)CONCAT44((int)((ulonglong)dVar17 >> 0x20),*(undefined4 *)DAT_8c6131d8);
        }
        else {
          dVar29 = *DAT_8c6131d8;
        }
        pdVar7 = (double *)(iVar14 + (int)&local_48);
        if (in_FPSCR_PR == '\0') {
          fVar19 = SUB84(dVar17,0) - SUB84(dVar26,0);
        }
        else {
          fVar19 = SUB84(dVar17 - dVar26,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar22 = (double)CONCAT44(*(undefined4 *)(auStack_44 + iVar14 + 4),fVar19);
        }
        else {
          dVar22 = *(double *)(auStack_44 + iVar14 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) - SUB84(dVar26,0),
                                    SUB84(dVar22,0));
        }
        else {
          dVar22 = dVar22 - dVar26;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),SUB84(dVar22,0));
          dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)pdVar7);
          dVar27 = (double)CONCAT44(*(undefined4 *)pdVar8,SUB84(dVar27,0));
          dVar16 = dVar29;
        }
        else {
          dVar16 = *pdVar7;
          dVar27 = *pdVar8;
          dVar28 = dVar29;
        }
        fVar19 = (float)((ulonglong)dVar27 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),
                                    SUB84(dVar26,0) / (float)((ulonglong)dVar22 >> 0x20));
          fVar15 = SUB84(dVar22,0) - fVar19;
        }
        else {
          dVar26 = dVar26 / dVar22;
          fVar15 = SUB84(dVar22 - dVar27,0);
        }
        uVar21 = SUB84(dVar26,0);
        dVar17 = dVar28;
        dVar22 = dVar27;
        if (in_FPSCR_SZ == '\0') {
          dVar17 = (double)CONCAT44(uVar21,SUB84(dVar18,0));
          dVar22 = (double)CONCAT44(fVar19,fVar15);
        }
        if (in_FPSCR_PR == '\0') {
          dVar22 = (double)CONCAT44((float)((ulonglong)dVar17 >> 0x20) * SUB84(dVar22,0) +
                                    (float)((ulonglong)dVar22 >> 0x20),SUB84(dVar22,0));
        }
        uVar20 = (undefined4)((ulonglong)dVar26 >> 0x20);
        if (in_FPSCR_SZ == '\0') {
          dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),SUB84(dVar27,0));
          dVar27 = (double)CONCAT44(*(undefined4 *)(iVar4 + 0xc),*(undefined4 *)DAT_8c6131dc);
          dVar17 = (double)CONCAT44(uVar20,SUB84(dVar17,0));
        }
        else {
          dVar27 = *(double *)(iVar4 + 0xc);
          dVar16 = *DAT_8c6131dc;
          dVar17 = dVar26;
        }
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),
                                    SUB84(dVar27,0) * (float)((ulonglong)dVar22 >> 0x20));
        }
        else {
          dVar27 = dVar27 * dVar22;
        }
        iVar9 = iVar10 * 0x20 + iVar3;
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44((float)((ulonglong)dVar17 >> 0x20) * SUB84(dVar27,0) +
                                    (float)((ulonglong)dVar27 >> 0x20),SUB84(dVar27,0));
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(iVar9 + 0x10) = (int)((ulonglong)dVar27 >> 0x20);
          fVar19 = *(float *)(auStack_44 + iVar1);
          dVar23 = (double)CONCAT44(fVar19,*(undefined4 *)(auStack_44 + iVar14));
          dVar22 = (double)CONCAT44(fVar19,SUB84(dVar22,0));
          dVar27 = (double)CONCAT44(uVar21,SUB84(dVar17,0));
        }
        else {
          *(double *)(iVar9 + 0x10) = dVar27;
          dVar23 = *(double *)(auStack_44 + iVar1);
          fVar19 = (float)((ulonglong)dVar23 >> 0x20);
          dVar16 = *(double *)(auStack_44 + iVar14);
          dVar22 = dVar23;
          dVar27 = dVar28;
        }
        if (in_FPSCR_PR == '\0') {
          fVar19 = SUB84(dVar23,0) - fVar19;
          dVar23 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) * fVar19 +
                                    (float)((ulonglong)dVar23 >> 0x20),fVar19);
        }
        else {
          dVar23 = dVar23 - dVar22;
        }
        if (in_FPSCR_SZ == '\0') {
          uVar25 = (undefined4)((ulonglong)dVar23 >> 0x20);
          dVar23 = (double)CONCAT44(uVar25,SUB84(dVar22,0));
          dVar22 = (double)CONCAT44(uVar20,SUB84(dVar27,0));
          dVar27 = (double)CONCAT44(uVar25,*(undefined4 *)DAT_8c6131e0);
        }
        else {
          dVar16 = *DAT_8c6131e0;
          dVar22 = dVar26;
          dVar27 = dVar23;
        }
        if (in_FPSCR_PR == '\0') {
          fVar19 = SUB84(dVar27,0) * (float)((ulonglong)dVar23 >> 0x20);
        }
        else {
          fVar19 = SUB84(dVar27 * dVar23,0);
        }
        if (in_FPSCR_SZ == '\0') {
          uVar24 = CONCAT44(*(undefined4 *)(iVar4 + 0x10),fVar19);
        }
        else {
          uVar24 = *(undefined8 *)(iVar4 + 0x10);
        }
        fVar19 = (float)((ulonglong)dVar22 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          uVar24 = CONCAT44(fVar19 * (float)uVar24 + (float)((ulonglong)uVar24 >> 0x20),
                            (float)uVar24);
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(iVar9 + 0x14) = (int)((ulonglong)uVar24 >> 0x20);
          *(float *)(iVar9 + 0x18) = fVar19;
          dVar18 = (double)CONCAT44(uVar21,SUB84(dVar22,0));
        }
        else {
          *(undefined8 *)(iVar9 + 0x14) = uVar24;
          *(double *)(iVar9 + 0x18) = dVar22;
          dVar18 = dVar28;
        }
        iVar10 = iVar10 + 1;
        pdVar8 = (double *)(local_24[iVar6] + 0x1c);
        iVar6 = local_24[iVar13];
        if (in_FPSCR_SZ == '\0') {
          fVar19 = *(float *)pdVar8;
          dVar17 = (double)CONCAT44(fVar19,*(undefined4 *)(iVar6 + 0x1c));
          dVar29 = dVar16;
          dVar27 = (double)CONCAT44(fVar19,SUB84(dVar23,0));
        }
        else {
          dVar17 = *pdVar8;
          dVar29 = *(double *)(iVar6 + 0x1c);
          fVar19 = (float)((ulonglong)dVar17 >> 0x20);
          dVar27 = dVar17;
        }
        if (in_FPSCR_PR == '\0') {
          fVar19 = SUB84(dVar17,0) - fVar19;
          dVar17 = (double)CONCAT44((float)((ulonglong)dVar18 >> 0x20) * fVar19 +
                                    (float)((ulonglong)dVar17 >> 0x20),fVar19);
        }
        else {
          dVar17 = dVar17 - dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(iVar9 + 0x1c) = (int)((ulonglong)dVar17 >> 0x20);
        }
        else {
          *(double *)(iVar9 + 0x1c) = dVar17;
        }
      }
      if ((*(uint *)(local_24[iVar13] + 0xc) & 1) == 0) {
        piVar11 = local_24 + iVar13;
        if (in_FPSCR_SZ == '\0') {
          dVar17 = (double)CONCAT44((int)((ulonglong)dVar17 >> 0x20),
                                    *(undefined4 *)(*piVar11 + 0x10));
        }
        else {
          dVar29 = *(double *)(*piVar11 + 0x10);
        }
        iVar6 = iVar10 * 0x20 + iVar3;
        if (in_FPSCR_SZ == '\0') {
          *(int *)(iVar6 + 0x10) = SUB84(dVar17,0);
        }
        else {
          *(double *)(iVar6 + 0x10) = dVar29;
        }
        iVar10 = iVar10 + 1;
        if (in_FPSCR_SZ == '\0') {
          uVar21 = *(undefined4 *)(*piVar11 + 0x14);
          dVar17 = (double)CONCAT44((int)((ulonglong)dVar17 >> 0x20),uVar21);
          *(undefined4 *)(iVar6 + 0x14) = uVar21;
        }
        else {
          dVar29 = *(double *)(*piVar11 + 0x14);
          *(double *)(iVar6 + 0x14) = dVar29;
        }
        if (in_FPSCR_SZ == '\0') {
          uVar21 = *(undefined4 *)(*piVar11 + 0x18);
          dVar17 = (double)CONCAT44((int)((ulonglong)dVar17 >> 0x20),uVar21);
          *(undefined4 *)(iVar6 + 0x18) = uVar21;
        }
        else {
          dVar29 = *(double *)(*piVar11 + 0x18);
          *(double *)(iVar6 + 0x18) = dVar29;
        }
        if (in_FPSCR_SZ == '\0') {
          uVar21 = *(undefined4 *)(*piVar11 + 0x1c);
          dVar17 = (double)CONCAT44((int)((ulonglong)dVar17 >> 0x20),uVar21);
          *(undefined4 *)(iVar6 + 0x1c) = uVar21;
        }
        else {
          dVar29 = *(double *)(*piVar11 + 0x1c);
          *(double *)(iVar6 + 0x1c) = dVar29;
        }
      }
      iVar1 = DAT_8c6131ec;
      uVar21 = DAT_8c6131e8;
      piVar11 = DAT_8c6131e4;
      iVar14 = iVar13 + 1;
      iVar6 = iVar13;
      iVar13 = iVar14;
    } while (iVar14 < 3);
    if (iVar10 == 3) {
      if (in_FPSCR_SZ == '\0') {
        dVar26 = (double)CONCAT44(*(undefined4 *)(DAT_8c6131ec + 0x1c),*(undefined4 *)(iVar3 + 0x1c)
                                 );
      }
      else {
        dVar26 = *(double *)(DAT_8c6131ec + 0x1c);
      }
      if (in_FPSCR_PR == '\0') {
        dVar26 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) * SUB84(dVar26,0),
                                  SUB84(dVar26,0));
      }
      else {
        dVar26 = dVar26 * dVar26;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar8 = (double *)(unaff_r8 + 0x1c);
        *(int *)pdVar8 = (int)((ulonglong)dVar26 >> 0x20);
        dVar26 = (double)CONCAT44(*(undefined4 *)(iVar1 + 0x18),*(undefined4 *)(iVar3 + 0x1c));
      }
      else {
        pdVar8 = (double *)(unaff_r8 + 0x18);
        *pdVar8 = dVar26;
        dVar26 = *(double *)(iVar1 + 0x18);
      }
      if (in_FPSCR_PR == '\0') {
        dVar26 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) * SUB84(dVar26,0),
                                  SUB84(dVar26,0));
      }
      else {
        dVar26 = dVar26 * dVar26;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar8 = (double *)((int)pdVar8 + -4);
        *(int *)pdVar8 = (int)((ulonglong)dVar26 >> 0x20);
        dVar26 = (double)CONCAT44(*(undefined4 *)(iVar1 + 0x14),*(undefined4 *)(iVar3 + 0x1c));
      }
      else {
        pdVar8 = pdVar8 + -1;
        *pdVar8 = dVar26;
        dVar26 = *(double *)(iVar1 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        dVar26 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) * SUB84(dVar26,0),
                                  SUB84(dVar26,0));
      }
      else {
        dVar26 = dVar26 * dVar26;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar8 + -4) = (int)((ulonglong)dVar26 >> 0x20);
        *(undefined4 *)(pdVar8 + -1) = *(undefined4 *)(iVar1 + 0x10);
        *(undefined4 *)((int)pdVar8 + -0xc) = *(undefined4 *)(iVar3 + 0x18);
        *(undefined4 *)(pdVar8 + -2) = *(undefined4 *)(iVar3 + 0x14);
        pdVar8 = (double *)((int)pdVar8 + -0x14);
        *(undefined4 *)pdVar8 = *(undefined4 *)(iVar3 + 0x10);
      }
      else {
        pdVar8[-1] = dVar26;
        pdVar8[-2] = *(double *)(iVar1 + 0x10);
        pdVar8[-3] = *(double *)(iVar3 + 0x18);
        pdVar8[-4] = *(double *)(iVar3 + 0x14);
        pdVar8 = pdVar8 + -5;
        *pdVar8 = *(double *)(iVar3 + 0x10);
      }
      *(undefined4 *)((int)pdVar8 + -4) = uVar21;
      if (in_FPSCR_SZ == '\0') {
        dVar26 = (double)CONCAT44(*(undefined4 *)(iVar1 + 0x1c),*(undefined4 *)(iVar3 + 0x3c));
      }
      else {
        dVar26 = *(double *)(iVar1 + 0x1c);
      }
      if (in_FPSCR_PR == '\0') {
        dVar26 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) * SUB84(dVar26,0),
                                  SUB84(dVar26,0));
      }
      else {
        dVar26 = dVar26 * dVar26;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar8 = pdVar8 + 7;
        *(int *)pdVar8 = (int)((ulonglong)dVar26 >> 0x20);
        dVar26 = (double)CONCAT44(*(undefined4 *)(iVar1 + 0x18),*(undefined4 *)(iVar3 + 0x3c));
      }
      else {
        pdVar8 = (double *)((int)pdVar8 + 0x34);
        *pdVar8 = dVar26;
        dVar26 = *(double *)(iVar1 + 0x18);
      }
      if (in_FPSCR_PR == '\0') {
        dVar26 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) * SUB84(dVar26,0),
                                  SUB84(dVar26,0));
      }
      else {
        dVar26 = dVar26 * dVar26;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar8 = (double *)((int)pdVar8 + -4);
        *(int *)pdVar8 = (int)((ulonglong)dVar26 >> 0x20);
        dVar26 = (double)CONCAT44(*(undefined4 *)(iVar1 + 0x14),*(undefined4 *)(iVar3 + 0x3c));
      }
      else {
        pdVar8 = pdVar8 + -1;
        *pdVar8 = dVar26;
        dVar26 = *(double *)(iVar1 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        dVar26 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) * SUB84(dVar26,0),
                                  SUB84(dVar26,0));
      }
      else {
        dVar26 = dVar26 * dVar26;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar8 + -4) = (int)((ulonglong)dVar26 >> 0x20);
        *(undefined4 *)(pdVar8 + -1) = *(undefined4 *)(iVar1 + 0x10);
        *(undefined4 *)((int)pdVar8 + -0xc) = *(undefined4 *)(iVar3 + 0x38);
        *(undefined4 *)(pdVar8 + -2) = *(undefined4 *)(iVar3 + 0x34);
        pdVar8 = (double *)((int)pdVar8 + -0x14);
        *(undefined4 *)pdVar8 = *(undefined4 *)(iVar3 + 0x30);
      }
      else {
        pdVar8[-1] = dVar26;
        pdVar8[-2] = *(double *)(iVar1 + 0x10);
        pdVar8[-3] = *(double *)(iVar3 + 0x38);
        pdVar8[-4] = *(double *)(iVar3 + 0x34);
        pdVar8 = pdVar8 + -5;
        *pdVar8 = *(double *)(iVar3 + 0x30);
      }
      *(undefined4 *)((int)pdVar8 + -4) = uVar21;
      uVar21 = DAT_8c6131f0;
      if (in_FPSCR_SZ == '\0') {
        dVar26 = (double)CONCAT44(*(undefined4 *)(iVar1 + 0x1c),*(undefined4 *)(iVar3 + 0x5c));
      }
      else {
        dVar26 = *(double *)(iVar1 + 0x1c);
      }
      if (in_FPSCR_PR == '\0') {
        dVar26 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) * SUB84(dVar26,0),
                                  SUB84(dVar26,0));
      }
      else {
        dVar26 = dVar26 * dVar26;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar8 = pdVar8 + 7;
        *(int *)pdVar8 = (int)((ulonglong)dVar26 >> 0x20);
        dVar26 = (double)CONCAT44(*(undefined4 *)(iVar1 + 0x18),*(undefined4 *)(iVar3 + 0x5c));
      }
      else {
        pdVar8 = (double *)((int)pdVar8 + 0x34);
        *pdVar8 = dVar26;
        dVar26 = *(double *)(iVar1 + 0x18);
      }
      if (in_FPSCR_PR == '\0') {
        dVar26 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) * SUB84(dVar26,0),
                                  SUB84(dVar26,0));
      }
      else {
        dVar26 = dVar26 * dVar26;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar8 = (double *)((int)pdVar8 + -4);
        *(int *)pdVar8 = (int)((ulonglong)dVar26 >> 0x20);
        dVar26 = (double)CONCAT44(*(undefined4 *)(iVar1 + 0x14),*(undefined4 *)(iVar3 + 0x5c));
      }
      else {
        pdVar8 = pdVar8 + -1;
        *pdVar8 = dVar26;
        dVar26 = *(double *)(iVar1 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        dVar26 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) * SUB84(dVar26,0),
                                  SUB84(dVar26,0));
      }
      else {
        dVar26 = dVar26 * dVar26;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar8 + -4) = (int)((ulonglong)dVar26 >> 0x20);
        *(undefined4 *)(pdVar8 + -1) = *(undefined4 *)(iVar1 + 0x10);
        *(undefined4 *)((int)pdVar8 + -0xc) = *(undefined4 *)(iVar3 + 0x58);
        *(undefined4 *)(pdVar8 + -2) = *(undefined4 *)(iVar3 + 0x54);
        pdVar8 = (double *)((int)pdVar8 + -0x14);
        *(undefined4 *)pdVar8 = *(undefined4 *)(iVar3 + 0x50);
      }
      else {
        pdVar8[-1] = dVar26;
        pdVar8[-2] = *(double *)(iVar1 + 0x10);
        pdVar8[-3] = *(double *)(iVar3 + 0x58);
        pdVar8[-4] = *(double *)(iVar3 + 0x54);
        pdVar8 = pdVar8 + -5;
        *pdVar8 = *(double *)(iVar3 + 0x50);
      }
      *(undefined4 *)((int)pdVar8 + -4) = uVar21;
      iVar4 = *piVar11 + 3;
    }
    else {
      if (in_FPSCR_SZ == '\0') {
        dVar26 = (double)CONCAT44(*(undefined4 *)(DAT_8c6131ec + 0x1c),*(undefined4 *)(iVar3 + 0x1c)
                                 );
      }
      else {
        dVar26 = *(double *)(DAT_8c6131ec + 0x1c);
      }
      if (in_FPSCR_PR == '\0') {
        dVar26 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) * SUB84(dVar26,0),
                                  SUB84(dVar26,0));
      }
      else {
        dVar26 = dVar26 * dVar26;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar8 = (double *)(unaff_r8 + 0x1c);
        *(int *)pdVar8 = (int)((ulonglong)dVar26 >> 0x20);
        dVar26 = (double)CONCAT44(*(undefined4 *)(iVar1 + 0x18),*(undefined4 *)(iVar3 + 0x1c));
      }
      else {
        pdVar8 = (double *)(unaff_r8 + 0x18);
        *pdVar8 = dVar26;
        dVar26 = *(double *)(iVar1 + 0x18);
      }
      if (in_FPSCR_PR == '\0') {
        dVar26 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) * SUB84(dVar26,0),
                                  SUB84(dVar26,0));
      }
      else {
        dVar26 = dVar26 * dVar26;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar8 = (double *)((int)pdVar8 + -4);
        *(int *)pdVar8 = (int)((ulonglong)dVar26 >> 0x20);
        dVar26 = (double)CONCAT44(*(undefined4 *)(iVar1 + 0x14),*(undefined4 *)(iVar3 + 0x1c));
      }
      else {
        pdVar8 = pdVar8 + -1;
        *pdVar8 = dVar26;
        dVar26 = *(double *)(iVar1 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        dVar26 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) * SUB84(dVar26,0),
                                  SUB84(dVar26,0));
      }
      else {
        dVar26 = dVar26 * dVar26;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar8 + -4) = (int)((ulonglong)dVar26 >> 0x20);
        *(undefined4 *)(pdVar8 + -1) = *(undefined4 *)(iVar1 + 0x10);
        *(undefined4 *)((int)pdVar8 + -0xc) = *(undefined4 *)(iVar3 + 0x18);
        *(undefined4 *)(pdVar8 + -2) = *(undefined4 *)(iVar3 + 0x14);
        pdVar8 = (double *)((int)pdVar8 + -0x14);
        *(undefined4 *)pdVar8 = *(undefined4 *)(iVar3 + 0x10);
      }
      else {
        pdVar8[-1] = dVar26;
        pdVar8[-2] = *(double *)(iVar1 + 0x10);
        pdVar8[-3] = *(double *)(iVar3 + 0x18);
        pdVar8[-4] = *(double *)(iVar3 + 0x14);
        pdVar8 = pdVar8 + -5;
        *pdVar8 = *(double *)(iVar3 + 0x10);
      }
      *(undefined4 *)((int)pdVar8 + -4) = uVar21;
      if (in_FPSCR_SZ == '\0') {
        dVar26 = (double)CONCAT44(*(undefined4 *)(iVar1 + 0x1c),*(undefined4 *)(iVar3 + 0x3c));
      }
      else {
        dVar26 = *(double *)(iVar1 + 0x1c);
      }
      if (in_FPSCR_PR == '\0') {
        dVar26 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) * SUB84(dVar26,0),
                                  SUB84(dVar26,0));
      }
      else {
        dVar26 = dVar26 * dVar26;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar8 = pdVar8 + 7;
        *(int *)pdVar8 = (int)((ulonglong)dVar26 >> 0x20);
        dVar26 = (double)CONCAT44(*(undefined4 *)(iVar1 + 0x18),*(undefined4 *)(iVar3 + 0x3c));
      }
      else {
        pdVar8 = (double *)((int)pdVar8 + 0x34);
        *pdVar8 = dVar26;
        dVar26 = *(double *)(iVar1 + 0x18);
      }
      if (in_FPSCR_PR == '\0') {
        dVar26 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) * SUB84(dVar26,0),
                                  SUB84(dVar26,0));
      }
      else {
        dVar26 = dVar26 * dVar26;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar8 = (double *)((int)pdVar8 + -4);
        *(int *)pdVar8 = (int)((ulonglong)dVar26 >> 0x20);
        dVar26 = (double)CONCAT44(*(undefined4 *)(iVar1 + 0x14),*(undefined4 *)(iVar3 + 0x3c));
      }
      else {
        pdVar8 = pdVar8 + -1;
        *pdVar8 = dVar26;
        dVar26 = *(double *)(iVar1 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        dVar26 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) * SUB84(dVar26,0),
                                  SUB84(dVar26,0));
      }
      else {
        dVar26 = dVar26 * dVar26;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar8 + -4) = (int)((ulonglong)dVar26 >> 0x20);
        *(undefined4 *)(pdVar8 + -1) = *(undefined4 *)(iVar1 + 0x10);
        *(undefined4 *)((int)pdVar8 + -0xc) = *(undefined4 *)(iVar3 + 0x38);
        *(undefined4 *)(pdVar8 + -2) = *(undefined4 *)(iVar3 + 0x34);
        pdVar8 = (double *)((int)pdVar8 + -0x14);
        *(undefined4 *)pdVar8 = *(undefined4 *)(iVar3 + 0x30);
      }
      else {
        pdVar8[-1] = dVar26;
        pdVar8[-2] = *(double *)(iVar1 + 0x10);
        pdVar8[-3] = *(double *)(iVar3 + 0x38);
        pdVar8[-4] = *(double *)(iVar3 + 0x34);
        pdVar8 = pdVar8 + -5;
        *pdVar8 = *(double *)(iVar3 + 0x30);
      }
      *(undefined4 *)((int)pdVar8 + -4) = uVar21;
      if (in_FPSCR_SZ == '\0') {
        dVar26 = (double)CONCAT44(*(undefined4 *)(iVar1 + 0x1c),*(undefined4 *)(iVar3 + 0x7c));
      }
      else {
        dVar26 = *(double *)(iVar1 + 0x1c);
      }
      if (in_FPSCR_PR == '\0') {
        dVar26 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) * SUB84(dVar26,0),
                                  SUB84(dVar26,0));
      }
      else {
        dVar26 = dVar26 * dVar26;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar8 = pdVar8 + 7;
        *(int *)pdVar8 = (int)((ulonglong)dVar26 >> 0x20);
        dVar26 = (double)CONCAT44(*(undefined4 *)(iVar1 + 0x18),*(undefined4 *)(iVar3 + 0x7c));
      }
      else {
        pdVar8 = (double *)((int)pdVar8 + 0x34);
        *pdVar8 = dVar26;
        dVar26 = *(double *)(iVar1 + 0x18);
      }
      if (in_FPSCR_PR == '\0') {
        dVar26 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) * SUB84(dVar26,0),
                                  SUB84(dVar26,0));
      }
      else {
        dVar26 = dVar26 * dVar26;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar8 = (double *)((int)pdVar8 + -4);
        *(int *)pdVar8 = (int)((ulonglong)dVar26 >> 0x20);
        dVar26 = (double)CONCAT44(*(undefined4 *)(iVar1 + 0x14),*(undefined4 *)(iVar3 + 0x7c));
      }
      else {
        pdVar8 = pdVar8 + -1;
        *pdVar8 = dVar26;
        dVar26 = *(double *)(iVar1 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        dVar26 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) * SUB84(dVar26,0),
                                  SUB84(dVar26,0));
      }
      else {
        dVar26 = dVar26 * dVar26;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar8 + -4) = (int)((ulonglong)dVar26 >> 0x20);
        *(undefined4 *)(pdVar8 + -1) = *(undefined4 *)(iVar1 + 0x10);
        *(undefined4 *)((int)pdVar8 + -0xc) = *(undefined4 *)(iVar3 + 0x78);
        *(undefined4 *)(pdVar8 + -2) = *(undefined4 *)(iVar3 + 0x74);
        pdVar8 = (double *)((int)pdVar8 + -0x14);
        *(undefined4 *)pdVar8 = *(undefined4 *)(iVar3 + 0x70);
      }
      else {
        pdVar8[-1] = dVar26;
        pdVar8[-2] = *(double *)(iVar1 + 0x10);
        pdVar8[-3] = *(double *)(iVar3 + 0x78);
        pdVar8[-4] = *(double *)(iVar3 + 0x74);
        pdVar8 = pdVar8 + -5;
        *pdVar8 = *(double *)(iVar3 + 0x70);
      }
      *(undefined4 *)((int)pdVar8 + -4) = uVar21;
      uVar21 = DAT_8c6133a0;
      if (in_FPSCR_SZ == '\0') {
        dVar26 = (double)CONCAT44(*(undefined4 *)(iVar1 + 0x1c),*(undefined4 *)(iVar3 + 0x5c));
      }
      else {
        dVar26 = *(double *)(iVar1 + 0x1c);
      }
      if (in_FPSCR_PR == '\0') {
        dVar26 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) * SUB84(dVar26,0),
                                  SUB84(dVar26,0));
      }
      else {
        dVar26 = dVar26 * dVar26;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar8 = pdVar8 + 7;
        *(int *)pdVar8 = (int)((ulonglong)dVar26 >> 0x20);
        dVar26 = (double)CONCAT44(*(undefined4 *)(iVar1 + 0x18),*(undefined4 *)(iVar3 + 0x5c));
      }
      else {
        pdVar8 = (double *)((int)pdVar8 + 0x34);
        *pdVar8 = dVar26;
        dVar26 = *(double *)(iVar1 + 0x18);
      }
      if (in_FPSCR_PR == '\0') {
        dVar26 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) * SUB84(dVar26,0),
                                  SUB84(dVar26,0));
      }
      else {
        dVar26 = dVar26 * dVar26;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar8 = (double *)((int)pdVar8 + -4);
        *(int *)pdVar8 = (int)((ulonglong)dVar26 >> 0x20);
        dVar26 = (double)CONCAT44(*(undefined4 *)(iVar1 + 0x14),*(undefined4 *)(iVar3 + 0x5c));
      }
      else {
        pdVar8 = pdVar8 + -1;
        *pdVar8 = dVar26;
        dVar26 = *(double *)(iVar1 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        dVar26 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) * SUB84(dVar26,0),
                                  SUB84(dVar26,0));
      }
      else {
        dVar26 = dVar26 * dVar26;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar8 + -4) = (int)((ulonglong)dVar26 >> 0x20);
        *(undefined4 *)(pdVar8 + -1) = *(undefined4 *)(iVar1 + 0x10);
        *(undefined4 *)((int)pdVar8 + -0xc) = *(undefined4 *)(iVar3 + 0x58);
        *(undefined4 *)(pdVar8 + -2) = *(undefined4 *)(iVar3 + 0x54);
        pdVar8 = (double *)((int)pdVar8 + -0x14);
        *(undefined4 *)pdVar8 = *(undefined4 *)(iVar3 + 0x50);
      }
      else {
        pdVar8[-1] = dVar26;
        pdVar8[-2] = *(double *)(iVar1 + 0x10);
        pdVar8[-3] = *(double *)(iVar3 + 0x58);
        pdVar8[-4] = *(double *)(iVar3 + 0x54);
        pdVar8 = pdVar8 + -5;
        *pdVar8 = *(double *)(iVar3 + 0x50);
      }
      *(undefined4 *)((int)pdVar8 + -4) = uVar21;
      iVar4 = *piVar11 + 4;
    }
    *piVar11 = iVar4;
  }
  return;
}

