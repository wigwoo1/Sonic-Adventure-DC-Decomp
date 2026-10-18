/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Restarted to delay deadcode elimination for space: register */

double * FUN_8c0382ac(undefined4 param_1,double param_2,undefined4 param_3,float param_4,int param_5
                     )

{
  ushort uVar1;
  short *psVar2;
  undefined4 uVar3;
  double dVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  uint uVar8;
  double *pdVar9;
  int iVar10;
  int extraout_r1;
  int in_r2;
  int extraout_r3;
  ushort *puVar11;
  int iVar12;
  double *pdVar13;
  double *pdVar14;
  double *pdVar15;
  undefined4 uVar16;
  double *unaff_r14;
  float fVar17;
  float fVar19;
  double dVar20;
  float fVar21;
  double dVar23;
  float fVar25;
  double dVar24;
  double in_dr12;
  float in_fr15;
  undefined4 in_fr14;
  double in_xd0;
  double in_xd2;
  double in_xd4;
  float in_xf6;
  float in_xf7;
  double in_xd8;
  float fVar27;
  float fVar28;
  undefined8 in_xd10;
  float fVar29;
  float in_xf12;
  float in_xf13;
  float fVar30;
  double in_xd14;
  float fVar31;
  int in_GBR;
  bool bVar32;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  float fVar18;
  float fVar22;
  float fVar26;
  
  iVar5 = *(int *)(in_GBR + 0x3c);
  iVar10 = *(int *)(param_5 + 4);
  puVar11 = *(ushort **)(in_r2 + 4);
  *(int *)(in_GBR + 0x44) = (int)*(short *)(in_r2 + 2);
  psVar2 = DAT_8c0382d8;
  if (in_FPSCR_SZ != '\0') {
    in_xd10 = *(undefined8 *)PTR_DAT_8c0382dc;
  }
  iVar6 = *(int *)(in_GBR + 0x38);
  while( true ) {
    uVar16 = DAT_8c038408;
    iVar7 = *(int *)(in_GBR + 0x54);
    if (iVar7 == 0) {
      uVar1 = *puVar11;
      iVar7 = (int)(short)uVar1;
      puVar11 = puVar11 + 1;
      if (iVar7 < 0) {
        iVar7 = (int)(short)(uVar1 & DAT_8c038406);
      }
    }
    iVar7 = iVar7 + -2;
    iVar12 = 0;
    uVar1 = *puVar11;
    *(undefined4 *)(in_GBR + 0x48) = *(undefined4 *)((short)uVar1 * 4 + iVar6);
    pdVar13 = (double *)((short)uVar1 * 0x10 + iVar5);
    *psVar2 = uVar1 * 0xc;
    uVar1 = puVar11[1];
    puVar11 = puVar11 + 2;
    *(undefined4 *)(in_GBR + 0x4c) = *(undefined4 *)((short)uVar1 * 4 + iVar6);
    pdVar14 = (double *)((short)uVar1 * 0x10 + iVar5);
    psVar2[1] = uVar1 * 0xc;
    if (in_FPSCR_SZ == '\0') {
      param_4 = *(float *)(pdVar13 + 1);
      in_dr12 = (double)CONCAT44(*(undefined4 *)(pdVar14 + 1),SUB84(in_dr12,0));
    }
    else {
      in_xd8 = pdVar13[1];
      in_dr12 = pdVar14[1];
    }
    if (in_FPSCR_PR == '\0') {
      bVar32 = 0.0 < param_4;
    }
    else {
      bVar32 = 5.26354424712089e-315 < (double)CONCAT44(param_3,param_4);
    }
    uVar8 = (uint)!bVar32;
    if (in_FPSCR_PR == '\0') {
      bVar32 = 1.0 < param_4;
    }
    else {
      bVar32 = 5.26354424712089e-315 < (double)CONCAT44(param_3,param_4);
    }
    if (bVar32) {
      uVar8 = uVar8 | 0x10;
    }
    fVar25 = (float)((ulonglong)in_dr12 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      bVar32 = 0.0 < fVar25;
    }
    else {
      bVar32 = 5.26354424712089e-315 < in_dr12;
    }
    if (!bVar32) {
      uVar8 = uVar8 | 2;
    }
    if (in_FPSCR_PR == '\0') {
      bVar32 = 1.0 < fVar25;
    }
    else {
      bVar32 = 5.26354424712089e-315 < in_dr12;
    }
    if (bVar32) {
      uVar8 = uVar8 | 0x20;
    }
    do {
      uVar1 = *puVar11;
      puVar11 = puVar11 + 1;
      *(undefined4 *)(in_GBR + 0x50) = *(undefined4 *)((short)uVar1 * 4 + iVar6);
      pdVar15 = (double *)((short)uVar1 * 0x10 + iVar5);
      psVar2[2] = uVar1 * 0xc;
      uVar3 = DAT_8c038464;
      if (in_FPSCR_SZ == '\0') {
        in_fr15 = *(float *)(pdVar15 + 1);
      }
      else {
        in_xd14 = pdVar15[1];
      }
      if (in_FPSCR_PR == '\0') {
        bVar32 = 0.0 < in_fr15;
      }
      else {
        bVar32 = 5.26354424712089e-315 < (double)CONCAT44(in_fr14,in_fr15);
      }
      if (!bVar32) {
        uVar8 = uVar8 | 4;
      }
      if (in_FPSCR_PR == '\0') {
        bVar32 = 1.0 < in_fr15;
      }
      else {
        bVar32 = 5.26354424712089e-315 < (double)CONCAT44(in_fr14,in_fr15);
      }
      if (bVar32) {
        uVar8 = uVar8 | 0x40;
      }
      fVar25 = (float)((ulonglong)in_xd2 >> 0x20);
      fVar21 = (float)((ulonglong)in_xd4 >> 0x20);
      fVar19 = SUB84(in_xd8,0);
      fVar27 = (float)((ulonglong)in_xd8 >> 0x20);
      fVar28 = (float)in_xd10;
      fVar29 = (float)((ulonglong)in_xd10 >> 0x20);
      fVar30 = SUB84(in_xd14,0);
      fVar31 = (float)((ulonglong)in_xd14 >> 0x20);
      if (uVar8 == 0) {
        if (iVar12 == 0) {
          pdVar9 = (double *)(*psVar2 + iVar10);
          if (in_FPSCR_SZ == '\0') {
            dVar20 = (double)CONCAT44(*(undefined4 *)pdVar9,*(undefined4 *)((int)pdVar9 + 4));
            dVar23 = (double)((ulonglong)*(uint *)(pdVar9 + 1) << 0x20);
          }
          else {
            dVar20 = *pdVar9;
            in_xd0 = pdVar9[1];
            dVar23 = pdVar9[2];
          }
          dVar23 = (double)((ulonglong)dVar23 & 0xffffffff00000000);
                    /* WARNING: Ignoring partial resolution of indirect */
          if (in_FPSCR_PR == '\0') {
            fVar18 = SUB84(dVar20,0);
            fVar22 = (float)((ulonglong)dVar20 >> 0x20);
            fVar26 = (float)((ulonglong)dVar23 >> 0x20);
            fVar17 = SUB84(in_xd0,0) * fVar18 + SUB84(in_xd4,0) * fVar22 + fVar19 * 0.0 +
                     in_xf12 * fVar26;
            fVar21 = (float)((ulonglong)in_xd0 >> 0x20) * fVar18 + fVar21 * fVar22 + fVar27 * 0.0 +
                     in_xf13 * fVar26;
            fVar21 = SUB84(in_xd2,0) * fVar18 + in_xf6 * fVar22 + fVar28 * 0.0 + fVar30 * fVar26;
            fVar25 = fVar25 * fVar18 + in_xf7 * fVar22 + fVar29 * 0.0 + fVar31 * fVar26;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(undefined4 *)DAT_8c03840c);
          }
          else {
            in_xd2 = *DAT_8c03840c;
          }
          fVar25 = SUB84(dVar23,0);
          if (in_FPSCR_PR == '\0') {
            dVar23 = (double)CONCAT44((float)((ulonglong)dVar20 >> 0x20) * fVar25 + fVar25,
                                      SUB84(dVar20,0) * fVar25 + fVar25);
          }
          else {
            dVar23 = (dVar20 * dVar23 + dVar23) * dVar23 + dVar23;
          }
          if (in_FPSCR_SZ == '\0') {
            param_1 = (undefined4)((ulonglong)dVar23 >> 0x20);
            dVar20 = (double)CONCAT44(*(undefined4 *)pdVar13,*(undefined4 *)((int)pdVar13 + 4));
            dVar24 = (double)CONCAT44(*(undefined4 *)(pdVar13 + 1),
                                      *(undefined4 *)((int)pdVar13 + 0xc));
            dVar4 = in_xd0;
            param_2 = (double)CONCAT44(SUB84(dVar23,0),SUB84(param_2,0));
          }
          else {
            dVar20 = *pdVar13;
            dVar24 = pdVar13[2];
            in_xd2 = pdVar13[3];
            dVar4 = pdVar13[1];
            param_2 = in_xd0;
            in_xd4 = dVar23;
          }
          in_xd0 = dVar4;
          if (in_FPSCR_SZ == '\0') {
            *(undefined4 *)((int)unaff_r14 + 0x1c) = *(undefined4 *)(in_GBR + 0x48);
            *(int *)(unaff_r14 + 3) = SUB84(dVar24,0);
            *(int *)((int)unaff_r14 + 0x14) = (int)((ulonglong)param_2 >> 0x20);
            *(undefined4 *)(unaff_r14 + 2) = param_1;
            *(int *)((int)unaff_r14 + 0xc) = (int)((ulonglong)dVar24 >> 0x20);
            *(int *)(unaff_r14 + 1) = SUB84(dVar20,0);
            pdVar13 = (double *)((int)unaff_r14 + 4);
            *(int *)pdVar13 = (int)((ulonglong)dVar20 >> 0x20);
          }
          else {
            unaff_r14[3] = (double)CONCAT44(*(undefined4 *)(in_GBR + 0x48),param_1);
            unaff_r14[2] = in_xd2;
            unaff_r14[1] = param_2;
            *unaff_r14 = in_xd4;
            unaff_r14[-1] = dVar24;
            unaff_r14[-2] = in_xd0;
            pdVar13 = unaff_r14 + -3;
            *pdVar13 = dVar20;
          }
          *(undefined4 *)((int)pdVar13 + -4) = uVar16;
          unaff_r14 = (double *)((int)pdVar13 + 0x1c);
          iVar12 = 1;
        }
        pdVar13 = (double *)(psVar2[1] + iVar10);
        if (in_FPSCR_SZ == '\0') {
          dVar20 = (double)CONCAT44(*(undefined4 *)pdVar13,*(undefined4 *)((int)pdVar13 + 4));
          dVar23 = (double)((ulonglong)*(uint *)(pdVar13 + 1) << 0x20);
        }
        else {
          dVar20 = *pdVar13;
          in_xd0 = pdVar13[1];
          dVar23 = pdVar13[2];
        }
        dVar23 = (double)((ulonglong)dVar23 & 0xffffffff00000000);
                    /* WARNING: Ignoring partial resolution of indirect */
        if (in_FPSCR_PR == '\0') {
          fVar21 = SUB84(dVar20,0);
          fVar17 = (float)((ulonglong)dVar20 >> 0x20);
          fVar18 = (float)((ulonglong)dVar23 >> 0x20);
          fVar25 = SUB84(in_xd0,0) * fVar21 + SUB84(in_xd4,0) * fVar17 + fVar19 * 0.0 +
                   in_xf12 * fVar18;
          fVar25 = (float)((ulonglong)in_xd0 >> 0x20) * fVar21 +
                   (float)((ulonglong)in_xd4 >> 0x20) * fVar17 + fVar27 * 0.0 + in_xf13 * fVar18;
          fVar25 = SUB84(in_xd2,0) * fVar21 + in_xf6 * fVar17 + fVar28 * 0.0 + fVar30 * fVar18;
          fVar25 = (float)((ulonglong)in_xd2 >> 0x20) * fVar21 + in_xf7 * fVar17 + fVar29 * 0.0 +
                   fVar31 * fVar18;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(undefined4 *)DAT_8c03840c);
        }
        else {
          in_xd2 = *DAT_8c03840c;
        }
        fVar25 = SUB84(dVar23,0);
        if (in_FPSCR_PR == '\0') {
          dVar23 = (double)CONCAT44((float)((ulonglong)dVar20 >> 0x20) * fVar25 + fVar25,
                                    SUB84(dVar20,0) * fVar25 + fVar25);
        }
        else {
          dVar23 = (dVar20 * dVar23 + dVar23) * dVar23 + dVar23;
        }
        if (in_FPSCR_SZ == '\0') {
          param_1 = (undefined4)((ulonglong)dVar23 >> 0x20);
          dVar20 = (double)CONCAT44(*(undefined4 *)pdVar14,*(undefined4 *)((int)pdVar14 + 4));
          dVar24 = (double)CONCAT44(*(undefined4 *)(pdVar14 + 1),*(undefined4 *)((int)pdVar14 + 0xc)
                                   );
          dVar4 = in_xd0;
          param_2 = (double)CONCAT44(SUB84(dVar23,0),SUB84(param_2,0));
        }
        else {
          dVar20 = *pdVar14;
          dVar24 = pdVar14[2];
          in_xd2 = pdVar14[3];
          dVar4 = pdVar14[1];
          param_2 = in_xd0;
          in_xd4 = dVar23;
        }
        in_xd0 = dVar4;
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)((int)unaff_r14 + 0x1c) = *(undefined4 *)(in_GBR + 0x4c);
          *(int *)(unaff_r14 + 3) = SUB84(dVar24,0);
          *(int *)((int)unaff_r14 + 0x14) = (int)((ulonglong)param_2 >> 0x20);
          *(undefined4 *)(unaff_r14 + 2) = param_1;
          *(int *)((int)unaff_r14 + 0xc) = (int)((ulonglong)dVar24 >> 0x20);
          *(int *)(unaff_r14 + 1) = SUB84(dVar20,0);
          pdVar13 = (double *)((int)unaff_r14 + 4);
          *(int *)pdVar13 = (int)((ulonglong)dVar20 >> 0x20);
        }
        else {
          unaff_r14[3] = (double)CONCAT44(*(undefined4 *)(in_GBR + 0x4c),param_1);
          unaff_r14[2] = in_xd2;
          unaff_r14[1] = param_2;
          *unaff_r14 = in_xd4;
          unaff_r14[-1] = dVar24;
          unaff_r14[-2] = in_xd0;
          pdVar13 = unaff_r14 + -3;
          *pdVar13 = dVar20;
        }
        *(undefined4 *)((int)pdVar13 + -4) = uVar16;
        unaff_r14 = (double *)((int)pdVar13 + 0x1c);
        iVar12 = iVar12 + 1;
      }
      else {
        if (iVar12 != 0) {
          pdVar13 = (double *)(psVar2[1] + iVar10);
          if (in_FPSCR_SZ == '\0') {
            dVar20 = (double)CONCAT44(*(undefined4 *)pdVar13,*(undefined4 *)((int)pdVar13 + 4));
            dVar23 = (double)((ulonglong)*(uint *)(pdVar13 + 1) << 0x20);
          }
          else {
            dVar20 = *pdVar13;
            in_xd0 = pdVar13[1];
            dVar23 = pdVar13[2];
          }
          dVar23 = (double)((ulonglong)dVar23 & 0xffffffff00000000);
                    /* WARNING: Ignoring partial resolution of indirect */
          if (in_FPSCR_PR == '\0') {
            fVar17 = SUB84(dVar20,0);
            fVar18 = (float)((ulonglong)dVar20 >> 0x20);
            fVar22 = (float)((ulonglong)dVar23 >> 0x20);
            fVar19 = SUB84(in_xd0,0) * fVar17 + SUB84(in_xd4,0) * fVar18 + fVar19 * 0.0 +
                     in_xf12 * fVar22;
            fVar21 = (float)((ulonglong)in_xd0 >> 0x20) * fVar17 + fVar21 * fVar18 + fVar27 * 0.0 +
                     in_xf13 * fVar22;
            fVar21 = SUB84(in_xd2,0) * fVar17 + in_xf6 * fVar18 + fVar28 * 0.0 + fVar30 * fVar22;
            fVar25 = fVar25 * fVar17 + in_xf7 * fVar18 + fVar29 * 0.0 + fVar31 * fVar22;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(undefined4 *)DAT_8c038460);
          }
          else {
            in_xd2 = *DAT_8c038460;
          }
          fVar25 = SUB84(dVar23,0);
          if (in_FPSCR_PR == '\0') {
            dVar23 = (double)CONCAT44((float)((ulonglong)dVar20 >> 0x20) * fVar25 + fVar25,
                                      SUB84(dVar20,0) * fVar25 + fVar25);
          }
          else {
            dVar23 = (dVar20 * dVar23 + dVar23) * dVar23 + dVar23;
          }
          if (in_FPSCR_SZ == '\0') {
            param_1 = (undefined4)((ulonglong)dVar23 >> 0x20);
            dVar20 = (double)CONCAT44(*(undefined4 *)pdVar14,*(undefined4 *)((int)pdVar14 + 4));
            dVar24 = (double)CONCAT44(*(undefined4 *)(pdVar14 + 1),
                                      *(undefined4 *)((int)pdVar14 + 0xc));
            dVar4 = in_xd0;
            param_2 = (double)CONCAT44(SUB84(dVar23,0),SUB84(param_2,0));
          }
          else {
            dVar20 = *pdVar14;
            dVar24 = pdVar14[2];
            in_xd2 = pdVar14[3];
            dVar4 = pdVar14[1];
            param_2 = in_xd0;
            in_xd4 = dVar23;
          }
          in_xd0 = dVar4;
          if (in_FPSCR_SZ == '\0') {
            *(undefined4 *)((int)unaff_r14 + 0x1c) = *(undefined4 *)(in_GBR + 0x4c);
            *(int *)(unaff_r14 + 3) = SUB84(dVar24,0);
            *(int *)((int)unaff_r14 + 0x14) = (int)((ulonglong)param_2 >> 0x20);
            *(undefined4 *)(unaff_r14 + 2) = param_1;
            *(int *)((int)unaff_r14 + 0xc) = (int)((ulonglong)dVar24 >> 0x20);
            *(int *)(unaff_r14 + 1) = SUB84(dVar20,0);
            pdVar13 = (double *)((int)unaff_r14 + 4);
            *(int *)pdVar13 = (int)((ulonglong)dVar20 >> 0x20);
          }
          else {
            unaff_r14[3] = (double)CONCAT44(*(undefined4 *)(in_GBR + 0x4c),param_1);
            unaff_r14[2] = in_xd2;
            unaff_r14[1] = param_2;
            *unaff_r14 = in_xd4;
            unaff_r14[-1] = dVar24;
            unaff_r14[-2] = in_xd0;
            pdVar13 = unaff_r14 + -3;
            *pdVar13 = dVar20;
          }
          *(undefined4 *)((int)pdVar13 + -4) = uVar3;
          unaff_r14 = (double *)((int)pdVar13 + 0x1c);
          iVar12 = 0;
        }
        if ((((uVar8 & 0x11) == 0) || ((uVar8 & 0x22) == 0)) || ((uVar8 & 0x44) == 0)) {
          FUN_8c038516();
          iVar10 = extraout_r1;
          iVar6 = extraout_r3;
          uVar16 = DAT_8c0384a0;
        }
      }
      *(undefined4 *)(in_GBR + 0x48) = *(undefined4 *)(in_GBR + 0x4c);
      *(undefined4 *)(in_GBR + 0x4c) = *(undefined4 *)(in_GBR + 0x50);
      *psVar2 = psVar2[1];
      psVar2[1] = psVar2[2];
      uVar3 = DAT_8c0384f8;
      uVar8 = uVar8 >> 1 & 0x33;
      iVar7 = iVar7 + -1;
      pdVar13 = pdVar14;
      pdVar14 = pdVar15;
    } while (iVar7 != 0);
    if (iVar12 != 0) {
      pdVar13 = (double *)(psVar2[2] + iVar10);
      if (in_FPSCR_SZ == '\0') {
        dVar20 = (double)CONCAT44(*(undefined4 *)pdVar13,*(undefined4 *)((int)pdVar13 + 4));
        dVar23 = (double)((ulonglong)*(uint *)(pdVar13 + 1) << 0x20);
      }
      else {
        dVar20 = *pdVar13;
        in_xd0 = pdVar13[1];
        dVar23 = pdVar13[2];
      }
      dVar23 = (double)((ulonglong)dVar23 & 0xffffffff00000000);
                    /* WARNING: Ignoring partial resolution of indirect */
      if (in_FPSCR_PR == '\0') {
        fVar21 = SUB84(dVar20,0);
        fVar19 = (float)((ulonglong)dVar20 >> 0x20);
        fVar27 = (float)((ulonglong)dVar23 >> 0x20);
        fVar25 = SUB84(in_xd0,0) * fVar21 + SUB84(in_xd4,0) * fVar19 + SUB84(in_xd8,0) * 0.0 +
                 in_xf12 * fVar27;
        fVar25 = (float)((ulonglong)in_xd0 >> 0x20) * fVar21 +
                 (float)((ulonglong)in_xd4 >> 0x20) * fVar19 +
                 (float)((ulonglong)in_xd8 >> 0x20) * 0.0 + in_xf13 * fVar27;
        fVar25 = SUB84(in_xd2,0) * fVar21 + in_xf6 * fVar19 + (float)in_xd10 * 0.0 +
                 SUB84(in_xd14,0) * fVar27;
        fVar25 = (float)((ulonglong)in_xd2 >> 0x20) * fVar21 + in_xf7 * fVar19 +
                 (float)((ulonglong)in_xd10 >> 0x20) * 0.0 +
                 (float)((ulonglong)in_xd14 >> 0x20) * fVar27;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(undefined4 *)DAT_8c0384f4);
      }
      else {
        in_xd2 = *DAT_8c0384f4;
      }
      fVar25 = SUB84(dVar23,0);
      if (in_FPSCR_PR == '\0') {
        dVar23 = (double)CONCAT44((float)((ulonglong)dVar20 >> 0x20) * fVar25 + fVar25,
                                  SUB84(dVar20,0) * fVar25 + fVar25);
      }
      else {
        dVar23 = (dVar20 * dVar23 + dVar23) * dVar23 + dVar23;
      }
      if (in_FPSCR_SZ == '\0') {
        param_1 = (undefined4)((ulonglong)dVar23 >> 0x20);
        dVar20 = (double)CONCAT44(*(undefined4 *)pdVar15,*(undefined4 *)((int)pdVar15 + 4));
        dVar24 = (double)CONCAT44(*(undefined4 *)(pdVar15 + 1),*(undefined4 *)((int)pdVar15 + 0xc));
        dVar4 = in_xd0;
        param_2 = (double)CONCAT44(SUB84(dVar23,0),SUB84(param_2,0));
      }
      else {
        dVar20 = *pdVar15;
        dVar24 = pdVar15[2];
        in_xd2 = pdVar15[3];
        dVar4 = pdVar15[1];
        param_2 = in_xd0;
        in_xd4 = dVar23;
      }
      in_xd0 = dVar4;
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)((int)unaff_r14 + 0x1c) = *(undefined4 *)(in_GBR + 0x50);
        *(int *)(unaff_r14 + 3) = SUB84(dVar24,0);
        *(int *)((int)unaff_r14 + 0x14) = (int)((ulonglong)param_2 >> 0x20);
        *(undefined4 *)(unaff_r14 + 2) = param_1;
        *(int *)((int)unaff_r14 + 0xc) = (int)((ulonglong)dVar24 >> 0x20);
        *(int *)(unaff_r14 + 1) = SUB84(dVar20,0);
        pdVar13 = (double *)((int)unaff_r14 + 4);
        *(int *)pdVar13 = (int)((ulonglong)dVar20 >> 0x20);
      }
      else {
        unaff_r14[3] = (double)CONCAT44(*(undefined4 *)(in_GBR + 0x50),param_1);
        unaff_r14[2] = in_xd2;
        unaff_r14[1] = param_2;
        *unaff_r14 = in_xd4;
        unaff_r14[-1] = dVar24;
        unaff_r14[-2] = in_xd0;
        pdVar13 = unaff_r14 + -3;
        *pdVar13 = dVar20;
      }
      *(undefined4 *)((int)pdVar13 + -4) = uVar3;
      unaff_r14 = (double *)((int)pdVar13 + 0x1c);
    }
    iVar7 = *(int *)(in_GBR + 0x44) + -1;
    if (iVar7 < 1) break;
    *(int *)(in_GBR + 0x44) = iVar7;
  }
  return unaff_r14;
}

