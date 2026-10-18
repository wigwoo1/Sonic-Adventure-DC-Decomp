/* Ghidra draft only; not buildable/verified source. */

void FUN_8c617e46(double param_1,undefined4 param_2,ushort *param_3)

{
  ushort uVar1;
  int iVar2;
  int *piVar3;
  undefined4 *puVar4;
  int *piVar5;
  uint uVar6;
  double *pdVar7;
  int iVar8;
  double *pdVar9;
  double *pdVar10;
  int iVar11;
  uint *unaff_r8;
  double *pdVar12;
  int iVar13;
  int iVar14;
  ushort *puVar15;
  ushort *puVar16;
  ushort *puVar17;
  ushort *puVar18;
  double *pdVar19;
  double in_dr0;
  double dVar20;
  undefined4 uVar21;
  float fVar22;
  double in_dr2;
  double dVar23;
  uint uVar24;
  double dVar25;
  undefined4 uVar27;
  float fVar28;
  undefined8 uVar26;
  undefined8 uVar29;
  undefined8 uVar30;
  double in_xd2;
  double in_xd4;
  bool bVar31;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  piVar3 = DAT_8c617eb8;
  iVar2 = DAT_8c617eb4;
  puVar15 = *(ushort **)(param_3 + 2);
  uVar6 = *param_3 & DAT_8c617ebc;
  if (uVar6 == 0) {
    *DAT_8c617eb8 = *DAT_8c617eb8 + (uint)param_3[1] * 3;
    iVar13 = 0;
    while (puVar4 = DAT_8c618088, iVar13 < (int)(uint)param_3[1]) {
      uVar1 = *puVar15;
      puVar18 = puVar15 + 1;
      puVar16 = puVar15 + 2;
      iVar14 = *DAT_8c61807c;
      puVar15 = puVar15 + 3;
      pdVar7 = (double *)((uint)*puVar18 * 0x20 + iVar14);
      if (in_FPSCR_SZ == '\0') {
        dVar25 = (double)CONCAT44(*(undefined4 *)(pdVar7 + 2),SUB84(param_1,0));
      }
      else {
        dVar25 = pdVar7[2];
      }
      pdVar9 = (double *)((uint)uVar1 * 0x20 + iVar14);
      uVar21 = (undefined4)((ulonglong)dVar25 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        dVar23 = (double)CONCAT44(uVar21,*(undefined4 *)(pdVar9 + 2));
      }
      else {
        in_xd2 = pdVar9[2];
        dVar23 = dVar25;
      }
      if (in_FPSCR_PR == '\0') {
        dVar23 = (double)CONCAT44((float)((ulonglong)dVar23 >> 0x20) - SUB84(dVar23,0),
                                  SUB84(dVar23,0));
      }
      else {
        dVar23 = dVar23 - dVar23;
      }
      pdVar10 = (double *)((uint)*puVar16 * 0x20 + iVar14);
      if (in_FPSCR_SZ == '\0') {
        fVar28 = *(float *)((int)pdVar7 + 0x14);
        dVar25 = (double)CONCAT44(uVar21,fVar28);
        dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),
                                  *(undefined4 *)((int)pdVar10 + 0x14));
        in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar28);
      }
      else {
        in_xd2 = *(double *)((int)pdVar10 + 0x14);
        fVar28 = SUB84(dVar25,0);
      }
      if (in_FPSCR_PR == '\0') {
        fVar22 = SUB84(dVar23,0) - fVar28;
        dVar23 = (double)CONCAT44((float)((ulonglong)dVar23 >> 0x20) * fVar22,fVar22);
      }
      else {
        dVar23 = (dVar23 - dVar25) * (dVar23 - dVar25);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),
                                  *(undefined4 *)((int)pdVar9 + 0x14));
      }
      else {
        in_xd2 = *(double *)((int)pdVar9 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        dVar20 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),SUB84(in_dr0,0) - SUB84(dVar23,0)
                                 );
      }
      else {
        dVar20 = in_dr0 - dVar23;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(undefined4 *)(pdVar10 + 2));
      }
      else {
        in_xd2 = pdVar10[2];
      }
      if (in_FPSCR_PR == '\0') {
        dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),
                                  SUB84(dVar23,0) - (float)((ulonglong)dVar25 >> 0x20));
      }
      else {
        dVar23 = dVar23 - dVar25;
      }
      uVar6 = (uint)((ulonglong)dVar23 >> 0x20);
      param_1 = dVar23;
      if (in_FPSCR_SZ == '\0') {
        param_1 = (double)CONCAT44(uVar6,fVar28);
      }
      if (in_FPSCR_PR == '\0') {
        in_dr0 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0) * SUB84(dVar23,0)
                                 );
      }
      else {
        in_dr0 = dVar20 * dVar23;
      }
      if (in_FPSCR_PR == '\0') {
        fVar28 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
        param_1 = (double)CONCAT44(fVar28,SUB84(param_1,0));
        bVar31 = 0.0 < fVar28;
      }
      else {
        param_1 = param_1 - in_dr0;
        bVar31 = (double)((ulonglong)uVar6 << 0x20) < param_1;
      }
      iVar13 = iVar13 + 1;
      if (!bVar31) {
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(*(undefined4 *)DAT_8c618080,*(undefined4 *)pdVar9);
        }
        else {
          in_xd2 = *pdVar9;
          dVar25 = *DAT_8c618080;
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar25,0),
                                    SUB84(dVar25,0));
        }
        else {
          dVar25 = dVar25 * dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)DAT_8c618084);
          in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),*DAT_8c618088);
        }
        else {
          in_xd2 = *DAT_8c618084;
        }
        if (in_FPSCR_PR == '\0') {
          fVar28 = SUB84(dVar25,0) - (float)((ulonglong)dVar25 >> 0x20);
        }
        else {
          fVar28 = SUB84(dVar25 - dVar25,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(*(undefined4 *)((int)pdVar9 + 4),fVar28);
        }
        else {
          dVar25 = *(double *)((int)pdVar9 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar23 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                    SUB84(in_dr0,0) * (float)((ulonglong)dVar25 >> 0x20));
        }
        else {
          dVar23 = in_dr0 * dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(*(undefined4 *)(pdVar9 + 1),SUB84(dVar25,0));
        }
        else {
          dVar25 = pdVar9[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                    SUB84(dVar25,0) - SUB84(dVar23,0));
        }
        else {
          dVar25 = dVar25 - dVar23;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),
                                    *(undefined4 *)(DAT_8c618080 + 1));
        }
        if (in_FPSCR_PR == '\0') {
          fVar28 = SUB84(dVar23,0) * (float)((ulonglong)dVar25 >> 0x20);
          dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),fVar28);
          fVar28 = SUB84(dVar25,0) - fVar28;
        }
        else {
          dVar23 = dVar23 * dVar25;
          fVar28 = SUB84(dVar25 - dVar23,0);
        }
        if (in_FPSCR_SZ == '\0') {
          *(float *)((int)pdVar9 + 0x1c) = fVar28;
          dVar25 = (double)CONCAT44(*(undefined4 *)(iVar2 + 0x1c),fVar28);
        }
        else {
          *(double *)((int)pdVar9 + 0x1c) = in_xd2;
          dVar25 = *(double *)(iVar2 + 0x1c);
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar25,0),
                                    SUB84(dVar25,0));
        }
        else {
          dVar25 = dVar25 * dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar19 = (double *)(unaff_r8 + 7);
          *(uint *)pdVar19 = (uint)((ulonglong)dVar25 >> 0x20);
          dVar25 = (double)CONCAT44(*(undefined4 *)(iVar2 + 0x18),
                                    *(undefined4 *)((int)pdVar9 + 0x1c));
        }
        else {
          pdVar19 = (double *)(unaff_r8 + 6);
          *pdVar19 = dVar25;
          in_xd2 = *(double *)((int)pdVar9 + 0x1c);
          dVar25 = *(double *)(iVar2 + 0x18);
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar25,0),
                                    SUB84(dVar25,0));
        }
        else {
          dVar25 = dVar25 * dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar19 = (double *)((int)pdVar19 + -4);
          *(uint *)pdVar19 = (uint)((ulonglong)dVar25 >> 0x20);
          dVar25 = (double)CONCAT44(*(undefined4 *)(iVar2 + 0x14),
                                    *(undefined4 *)((int)pdVar9 + 0x1c));
          uVar6 = DAT_8c61808c;
        }
        else {
          pdVar19 = pdVar19 + -1;
          *pdVar19 = dVar25;
          in_xd2 = *(double *)((int)pdVar9 + 0x1c);
          dVar25 = *(double *)(iVar2 + 0x14);
          uVar6 = DAT_8c61808c;
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar25,0),
                                    SUB84(dVar25,0));
        }
        else {
          dVar25 = dVar25 * dVar25;
        }
        DAT_8c61808c = uVar6;
        if (in_FPSCR_SZ == '\0') {
          *(uint *)((int)pdVar19 + -4) = (uint)((ulonglong)dVar25 >> 0x20);
          *(uint *)(pdVar19 + -1) = *(uint *)(iVar2 + 0x10);
          *(uint *)((int)pdVar19 + -0xc) = *(uint *)(pdVar9 + 3);
          *(uint *)(pdVar19 + -2) = *(uint *)((int)pdVar9 + 0x14);
          pdVar19 = (double *)((int)pdVar19 + -0x14);
          *(uint *)pdVar19 = *(uint *)(pdVar9 + 2);
        }
        else {
          pdVar19[-1] = dVar25;
          pdVar19[-2] = *(double *)(iVar2 + 0x10);
          pdVar19[-3] = pdVar9[3];
          pdVar19[-4] = *(double *)((int)pdVar9 + 0x14);
          in_xd2 = pdVar9[2];
          pdVar19 = pdVar19 + -5;
          *pdVar19 = in_xd2;
        }
        *(uint *)((int)pdVar19 + -4) = uVar6;
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(*(undefined4 *)DAT_8c618080,*(undefined4 *)pdVar7);
        }
        else {
          in_xd2 = *pdVar7;
          dVar25 = *DAT_8c618080;
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar25,0),
                                    SUB84(dVar25,0));
        }
        else {
          dVar25 = dVar25 * dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*puVar4);
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)DAT_8c618084);
        }
        else {
          in_xd2 = *DAT_8c618084;
        }
        if (in_FPSCR_PR == '\0') {
          fVar28 = SUB84(dVar25,0) - (float)((ulonglong)dVar25 >> 0x20);
        }
        else {
          fVar28 = SUB84(dVar25 - dVar25,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(*(undefined4 *)((int)pdVar7 + 4),fVar28);
        }
        else {
          dVar25 = *(double *)((int)pdVar7 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),
                                    SUB84(dVar23,0) * (float)((ulonglong)dVar25 >> 0x20));
        }
        else {
          dVar23 = dVar23 * dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(*(undefined4 *)(pdVar7 + 1),SUB84(dVar25,0));
        }
        else {
          dVar25 = pdVar7[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                    SUB84(dVar25,0) - SUB84(dVar23,0));
        }
        else {
          dVar25 = dVar25 - dVar23;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*DAT_8c618090);
        }
        if (in_FPSCR_PR == '\0') {
          fVar28 = SUB84(dVar23,0) * (float)((ulonglong)dVar25 >> 0x20);
          dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),fVar28);
          fVar28 = SUB84(dVar25,0) - fVar28;
        }
        else {
          dVar23 = dVar23 * dVar25;
          fVar28 = SUB84(dVar25 - dVar23,0);
        }
        if (in_FPSCR_SZ == '\0') {
          *(float *)((int)pdVar7 + 0x1c) = fVar28;
          dVar25 = (double)CONCAT44(*(undefined4 *)(iVar2 + 0x1c),fVar28);
        }
        else {
          *(double *)((int)pdVar7 + 0x1c) = in_xd2;
          dVar25 = *(double *)(iVar2 + 0x1c);
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar25,0),
                                    SUB84(dVar25,0));
        }
        else {
          dVar25 = dVar25 * dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar19 = pdVar19 + 7;
          *(uint *)pdVar19 = (uint)((ulonglong)dVar25 >> 0x20);
          dVar25 = (double)CONCAT44(*(undefined4 *)(iVar2 + 0x18),
                                    *(undefined4 *)((int)pdVar7 + 0x1c));
        }
        else {
          pdVar19 = (double *)((int)pdVar19 + 0x34);
          *pdVar19 = dVar25;
          in_xd2 = *(double *)((int)pdVar7 + 0x1c);
          dVar25 = *(double *)(iVar2 + 0x18);
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar25,0),
                                    SUB84(dVar25,0));
        }
        else {
          dVar25 = dVar25 * dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar19 = (double *)((int)pdVar19 + -4);
          *(uint *)pdVar19 = (uint)((ulonglong)dVar25 >> 0x20);
          dVar25 = (double)CONCAT44(*(undefined4 *)(iVar2 + 0x14),
                                    *(undefined4 *)((int)pdVar7 + 0x1c));
        }
        else {
          pdVar19 = pdVar19 + -1;
          *pdVar19 = dVar25;
          in_xd2 = *(double *)((int)pdVar7 + 0x1c);
          dVar25 = *(double *)(iVar2 + 0x14);
        }
        uVar6 = DAT_8c61808c;
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar25,0),
                                    SUB84(dVar25,0));
        }
        else {
          dVar25 = dVar25 * dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          *(uint *)((int)pdVar19 + -4) = (uint)((ulonglong)dVar25 >> 0x20);
          *(uint *)(pdVar19 + -1) = *(uint *)(iVar2 + 0x10);
          *(uint *)((int)pdVar19 + -0xc) = *(uint *)(pdVar7 + 3);
          *(uint *)(pdVar19 + -2) = *(uint *)((int)pdVar7 + 0x14);
          pdVar19 = (double *)((int)pdVar19 + -0x14);
          *(uint *)pdVar19 = *(uint *)(pdVar7 + 2);
        }
        else {
          pdVar19[-1] = dVar25;
          pdVar19[-2] = *(double *)(iVar2 + 0x10);
          pdVar19[-3] = pdVar7[3];
          pdVar19[-4] = *(double *)((int)pdVar7 + 0x14);
          in_xd2 = pdVar7[2];
          pdVar19 = pdVar19 + -5;
          *pdVar19 = in_xd2;
        }
        *(uint *)((int)pdVar19 + -4) = uVar6;
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(*(undefined4 *)DAT_8c618080,*(undefined4 *)pdVar10);
        }
        else {
          in_xd2 = *pdVar10;
          dVar25 = *DAT_8c618080;
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar25,0),
                                    SUB84(dVar25,0));
        }
        else {
          dVar25 = dVar25 * dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*puVar4);
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)DAT_8c618084);
        }
        else {
          in_xd2 = *DAT_8c618084;
        }
        if (in_FPSCR_PR == '\0') {
          fVar28 = SUB84(dVar25,0) - (float)((ulonglong)dVar25 >> 0x20);
        }
        else {
          fVar28 = SUB84(dVar25 - dVar25,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(*(undefined4 *)((int)pdVar10 + 4),fVar28);
        }
        else {
          dVar25 = *(double *)((int)pdVar10 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),
                                    SUB84(dVar23,0) * (float)((ulonglong)dVar25 >> 0x20));
        }
        else {
          dVar23 = dVar23 * dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(*(undefined4 *)(pdVar10 + 1),SUB84(dVar25,0));
        }
        else {
          dVar25 = pdVar10[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                    SUB84(dVar25,0) - SUB84(dVar23,0));
        }
        else {
          dVar25 = dVar25 - dVar23;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*DAT_8c618090);
        }
        if (in_FPSCR_PR == '\0') {
          fVar28 = SUB84(dVar23,0) * (float)((ulonglong)dVar25 >> 0x20);
          in_dr0 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),fVar28);
          fVar28 = SUB84(dVar25,0) - fVar28;
        }
        else {
          in_dr0 = dVar23 * dVar25;
          fVar28 = SUB84(dVar25 - in_dr0,0);
        }
        if (in_FPSCR_SZ == '\0') {
          *(float *)((int)pdVar10 + 0x1c) = fVar28;
          dVar25 = (double)CONCAT44(*(undefined4 *)(iVar2 + 0x1c),fVar28);
        }
        else {
          *(double *)((int)pdVar10 + 0x1c) = in_xd2;
          dVar25 = *(double *)(iVar2 + 0x1c);
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar25,0),
                                    SUB84(dVar25,0));
        }
        else {
          dVar25 = dVar25 * dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar19 = pdVar19 + 7;
          *(uint *)pdVar19 = (uint)((ulonglong)dVar25 >> 0x20);
          dVar25 = (double)CONCAT44(*(undefined4 *)(iVar2 + 0x18),
                                    *(undefined4 *)((int)pdVar10 + 0x1c));
        }
        else {
          pdVar19 = (double *)((int)pdVar19 + 0x34);
          *pdVar19 = dVar25;
          in_xd2 = *(double *)((int)pdVar10 + 0x1c);
          dVar25 = *(double *)(iVar2 + 0x18);
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar25,0),
                                    SUB84(dVar25,0));
        }
        else {
          dVar25 = dVar25 * dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar19 = (double *)((int)pdVar19 + -4);
          *(uint *)pdVar19 = (uint)((ulonglong)dVar25 >> 0x20);
          dVar25 = (double)CONCAT44(*(undefined4 *)(iVar2 + 0x14),
                                    *(undefined4 *)((int)pdVar10 + 0x1c));
        }
        else {
          pdVar19 = pdVar19 + -1;
          *pdVar19 = dVar25;
          in_xd2 = *(double *)((int)pdVar10 + 0x1c);
          dVar25 = *(double *)(iVar2 + 0x14);
        }
        uVar6 = DAT_8c618094;
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar25,0),
                                    SUB84(dVar25,0));
        }
        else {
          dVar25 = dVar25 * dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          *(uint *)((int)pdVar19 + -4) = (uint)((ulonglong)dVar25 >> 0x20);
          *(uint *)(pdVar19 + -1) = *(uint *)(iVar2 + 0x10);
          *(uint *)((int)pdVar19 + -0xc) = *(uint *)(pdVar10 + 3);
          *(uint *)(pdVar19 + -2) = *(uint *)((int)pdVar10 + 0x14);
          pdVar19 = (double *)((int)pdVar19 + -0x14);
          *(uint *)pdVar19 = *(uint *)(pdVar10 + 2);
        }
        else {
          pdVar19[-1] = dVar25;
          pdVar19[-2] = *(double *)(iVar2 + 0x10);
          pdVar19[-3] = pdVar10[3];
          pdVar19[-4] = *(double *)((int)pdVar10 + 0x14);
          in_xd2 = pdVar10[2];
          pdVar19 = pdVar19 + -5;
          *pdVar19 = in_xd2;
        }
        *(uint *)((int)pdVar19 + -4) = uVar6;
        unaff_r8 = (uint *)((int)pdVar19 + 0x1c);
        *DAT_8c618098 = *DAT_8c618098 + 3;
      }
    }
  }
  else if (uVar6 == (int)DAT_8c617ea4) {
    *DAT_8c617eb8 = *DAT_8c617eb8 + (uint)param_3[1] * 4;
    for (iVar13 = 0; puVar4 = DAT_8c618328, uVar21 = SUB84(in_dr2,0), iVar13 < (int)(uint)param_3[1]
        ; iVar13 = iVar13 + 1) {
      uVar1 = *puVar15;
      puVar18 = puVar15 + 1;
      puVar16 = puVar15 + 2;
      iVar14 = *DAT_8c61831c;
      puVar17 = puVar15 + 3;
      puVar15 = puVar15 + 4;
      pdVar7 = (double *)((uint)*puVar18 * 0x20 + iVar14);
      pdVar9 = (double *)((uint)uVar1 * 0x20 + iVar14);
      if (in_FPSCR_SZ == '\0') {
        dVar25 = (double)CONCAT44(*(undefined4 *)(pdVar7 + 2),SUB84(param_1,0));
        uVar21 = *(undefined4 *)(pdVar9 + 2);
      }
      else {
        dVar25 = pdVar7[2];
        in_xd2 = pdVar9[2];
      }
      uVar27 = (undefined4)((ulonglong)dVar25 >> 0x20);
      dVar23 = dVar25;
      if (in_FPSCR_SZ == '\0') {
        dVar23 = (double)CONCAT44(uVar27,uVar21);
      }
      if (in_FPSCR_PR == '\0') {
        dVar23 = (double)CONCAT44((float)((ulonglong)dVar23 >> 0x20) - SUB84(dVar23,0),
                                  SUB84(dVar23,0));
      }
      else {
        dVar23 = dVar23 - dVar23;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar25 = (double)CONCAT44(uVar27,*(undefined4 *)((int)pdVar7 + 0x14));
      }
      pdVar10 = (double *)((uint)*puVar16 * 0x20 + iVar14);
      fVar28 = SUB84(dVar25,0);
      if (in_FPSCR_SZ == '\0') {
        dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),
                                  *(undefined4 *)((int)pdVar10 + 0x14));
        in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar28);
      }
      else {
        in_xd2 = *(double *)((int)pdVar10 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        fVar22 = SUB84(dVar23,0) - fVar28;
        dVar23 = (double)CONCAT44((float)((ulonglong)dVar23 >> 0x20) * fVar22,fVar22);
      }
      else {
        dVar23 = (dVar23 - dVar25) * (dVar23 - dVar25);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),
                                  *(undefined4 *)((int)pdVar9 + 0x14));
      }
      else {
        in_xd2 = *(double *)((int)pdVar9 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        dVar20 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),SUB84(in_dr0,0) - SUB84(dVar23,0)
                                 );
      }
      else {
        dVar20 = in_dr0 - dVar23;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(undefined4 *)(pdVar10 + 2));
      }
      else {
        in_xd2 = pdVar10[2];
      }
      if (in_FPSCR_PR == '\0') {
        dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),
                                  SUB84(dVar23,0) - (float)((ulonglong)dVar25 >> 0x20));
      }
      else {
        dVar23 = dVar23 - dVar25;
      }
      uVar6 = (uint)((ulonglong)dVar23 >> 0x20);
      param_1 = dVar23;
      if (in_FPSCR_SZ == '\0') {
        param_1 = (double)CONCAT44(uVar6,fVar28);
      }
      if (in_FPSCR_PR == '\0') {
        in_dr0 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0) * SUB84(dVar23,0)
                                 );
      }
      else {
        in_dr0 = dVar20 * dVar23;
      }
      in_dr2 = (double)((ulonglong)uVar6 << 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar28 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
        param_1 = (double)CONCAT44(fVar28,SUB84(param_1,0));
        bVar31 = 0.0 < fVar28;
      }
      else {
        param_1 = param_1 - in_dr0;
        bVar31 = in_dr2 < param_1;
      }
      pdVar19 = (double *)((uint)*puVar17 * 0x20 + iVar14);
      if (!bVar31) {
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(*(undefined4 *)DAT_8c618320,*(undefined4 *)pdVar9);
        }
        else {
          in_xd2 = *pdVar9;
          dVar25 = *DAT_8c618320;
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar25,0),
                                    SUB84(dVar25,0));
        }
        else {
          dVar25 = dVar25 * dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)DAT_8c618324);
          in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),*DAT_8c618328);
        }
        else {
          in_xd2 = *DAT_8c618324;
        }
        if (in_FPSCR_PR == '\0') {
          fVar28 = SUB84(dVar25,0) - (float)((ulonglong)dVar25 >> 0x20);
        }
        else {
          fVar28 = SUB84(dVar25 - dVar25,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(*(undefined4 *)((int)pdVar9 + 4),fVar28);
        }
        else {
          dVar25 = *(double *)((int)pdVar9 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar23 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                    SUB84(in_dr0,0) * (float)((ulonglong)dVar25 >> 0x20));
        }
        else {
          dVar23 = in_dr0 * dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(*(undefined4 *)(pdVar9 + 1),SUB84(dVar25,0));
        }
        else {
          dVar25 = pdVar9[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                    SUB84(dVar25,0) - SUB84(dVar23,0));
        }
        else {
          dVar25 = dVar25 - dVar23;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),
                                    *(undefined4 *)(DAT_8c618320 + 1));
        }
        if (in_FPSCR_PR == '\0') {
          fVar28 = SUB84(dVar23,0) * (float)((ulonglong)dVar25 >> 0x20);
          dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),fVar28);
          fVar28 = SUB84(dVar25,0) - fVar28;
        }
        else {
          dVar23 = dVar23 * dVar25;
          fVar28 = SUB84(dVar25 - dVar23,0);
        }
        if (in_FPSCR_SZ == '\0') {
          *(float *)((int)pdVar9 + 0x1c) = fVar28;
          dVar25 = (double)CONCAT44(*(undefined4 *)(iVar2 + 0x1c),fVar28);
        }
        else {
          *(double *)((int)pdVar9 + 0x1c) = in_xd2;
          dVar25 = *(double *)(iVar2 + 0x1c);
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar25,0),
                                    SUB84(dVar25,0));
        }
        else {
          dVar25 = dVar25 * dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar12 = (double *)(unaff_r8 + 7);
          *(uint *)pdVar12 = (uint)((ulonglong)dVar25 >> 0x20);
          dVar25 = (double)CONCAT44(*(undefined4 *)(iVar2 + 0x18),
                                    *(undefined4 *)((int)pdVar9 + 0x1c));
        }
        else {
          pdVar12 = (double *)(unaff_r8 + 6);
          *pdVar12 = dVar25;
          in_xd2 = *(double *)((int)pdVar9 + 0x1c);
          dVar25 = *(double *)(iVar2 + 0x18);
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar25,0),
                                    SUB84(dVar25,0));
        }
        else {
          dVar25 = dVar25 * dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar12 = (double *)((int)pdVar12 + -4);
          *(uint *)pdVar12 = (uint)((ulonglong)dVar25 >> 0x20);
          dVar25 = (double)CONCAT44(*(undefined4 *)(iVar2 + 0x14),
                                    *(undefined4 *)((int)pdVar9 + 0x1c));
          uVar6 = DAT_8c61832c;
        }
        else {
          pdVar12 = pdVar12 + -1;
          *pdVar12 = dVar25;
          in_xd2 = *(double *)((int)pdVar9 + 0x1c);
          dVar25 = *(double *)(iVar2 + 0x14);
          uVar6 = DAT_8c61832c;
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar25,0),
                                    SUB84(dVar25,0));
        }
        else {
          dVar25 = dVar25 * dVar25;
        }
        DAT_8c61832c = uVar6;
        if (in_FPSCR_SZ == '\0') {
          *(uint *)((int)pdVar12 + -4) = (uint)((ulonglong)dVar25 >> 0x20);
          *(uint *)(pdVar12 + -1) = *(uint *)(iVar2 + 0x10);
          *(uint *)((int)pdVar12 + -0xc) = *(uint *)(pdVar9 + 3);
          *(uint *)(pdVar12 + -2) = *(uint *)((int)pdVar9 + 0x14);
          pdVar12 = (double *)((int)pdVar12 + -0x14);
          *(uint *)pdVar12 = *(uint *)(pdVar9 + 2);
        }
        else {
          pdVar12[-1] = dVar25;
          pdVar12[-2] = *(double *)(iVar2 + 0x10);
          pdVar12[-3] = pdVar9[3];
          pdVar12[-4] = *(double *)((int)pdVar9 + 0x14);
          in_xd2 = pdVar9[2];
          pdVar12 = pdVar12 + -5;
          *pdVar12 = in_xd2;
        }
        *(uint *)((int)pdVar12 + -4) = uVar6;
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(*(undefined4 *)DAT_8c618320,*(undefined4 *)pdVar7);
        }
        else {
          in_xd2 = *pdVar7;
          dVar25 = *DAT_8c618320;
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar25,0),
                                    SUB84(dVar25,0));
        }
        else {
          dVar25 = dVar25 * dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*puVar4);
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)DAT_8c618324);
        }
        else {
          in_xd2 = *DAT_8c618324;
        }
        if (in_FPSCR_PR == '\0') {
          fVar28 = SUB84(dVar25,0) - (float)((ulonglong)dVar25 >> 0x20);
        }
        else {
          fVar28 = SUB84(dVar25 - dVar25,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(*(undefined4 *)((int)pdVar7 + 4),fVar28);
        }
        else {
          dVar25 = *(double *)((int)pdVar7 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),
                                    SUB84(dVar23,0) * (float)((ulonglong)dVar25 >> 0x20));
        }
        else {
          dVar23 = dVar23 * dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(*(undefined4 *)(pdVar7 + 1),SUB84(dVar25,0));
        }
        else {
          dVar25 = pdVar7[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                    SUB84(dVar25,0) - SUB84(dVar23,0));
        }
        else {
          dVar25 = dVar25 - dVar23;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*DAT_8c618330);
        }
        if (in_FPSCR_PR == '\0') {
          fVar28 = SUB84(dVar23,0) * (float)((ulonglong)dVar25 >> 0x20);
          dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),fVar28);
          fVar28 = SUB84(dVar25,0) - fVar28;
        }
        else {
          dVar23 = dVar23 * dVar25;
          fVar28 = SUB84(dVar25 - dVar23,0);
        }
        if (in_FPSCR_SZ == '\0') {
          *(float *)((int)pdVar7 + 0x1c) = fVar28;
          dVar25 = (double)CONCAT44(*(undefined4 *)(iVar2 + 0x1c),fVar28);
        }
        else {
          *(double *)((int)pdVar7 + 0x1c) = in_xd2;
          dVar25 = *(double *)(iVar2 + 0x1c);
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar25,0),
                                    SUB84(dVar25,0));
        }
        else {
          dVar25 = dVar25 * dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar12 = pdVar12 + 7;
          *(uint *)pdVar12 = (uint)((ulonglong)dVar25 >> 0x20);
          dVar25 = (double)CONCAT44(*(undefined4 *)(iVar2 + 0x18),
                                    *(undefined4 *)((int)pdVar7 + 0x1c));
        }
        else {
          pdVar12 = (double *)((int)pdVar12 + 0x34);
          *pdVar12 = dVar25;
          in_xd2 = *(double *)((int)pdVar7 + 0x1c);
          dVar25 = *(double *)(iVar2 + 0x18);
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar25,0),
                                    SUB84(dVar25,0));
        }
        else {
          dVar25 = dVar25 * dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar12 = (double *)((int)pdVar12 + -4);
          *(uint *)pdVar12 = (uint)((ulonglong)dVar25 >> 0x20);
          dVar25 = (double)CONCAT44(*(undefined4 *)(iVar2 + 0x14),
                                    *(undefined4 *)((int)pdVar7 + 0x1c));
        }
        else {
          pdVar12 = pdVar12 + -1;
          *pdVar12 = dVar25;
          in_xd2 = *(double *)((int)pdVar7 + 0x1c);
          dVar25 = *(double *)(iVar2 + 0x14);
        }
        uVar6 = DAT_8c61832c;
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar25,0),
                                    SUB84(dVar25,0));
        }
        else {
          dVar25 = dVar25 * dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          *(uint *)((int)pdVar12 + -4) = (uint)((ulonglong)dVar25 >> 0x20);
          *(uint *)(pdVar12 + -1) = *(uint *)(iVar2 + 0x10);
          *(uint *)((int)pdVar12 + -0xc) = *(uint *)(pdVar7 + 3);
          *(uint *)(pdVar12 + -2) = *(uint *)((int)pdVar7 + 0x14);
          pdVar12 = (double *)((int)pdVar12 + -0x14);
          *(uint *)pdVar12 = *(uint *)(pdVar7 + 2);
        }
        else {
          pdVar12[-1] = dVar25;
          pdVar12[-2] = *(double *)(iVar2 + 0x10);
          pdVar12[-3] = pdVar7[3];
          pdVar12[-4] = *(double *)((int)pdVar7 + 0x14);
          in_xd2 = pdVar7[2];
          pdVar12 = pdVar12 + -5;
          *pdVar12 = in_xd2;
        }
        *(uint *)((int)pdVar12 + -4) = uVar6;
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(*(undefined4 *)DAT_8c618320,*(undefined4 *)pdVar10);
        }
        else {
          in_xd2 = *pdVar10;
          dVar25 = *DAT_8c618320;
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar25,0),
                                    SUB84(dVar25,0));
        }
        else {
          dVar25 = dVar25 * dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*puVar4);
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)DAT_8c618324);
        }
        else {
          in_xd2 = *DAT_8c618324;
        }
        if (in_FPSCR_PR == '\0') {
          fVar28 = SUB84(dVar25,0) - (float)((ulonglong)dVar25 >> 0x20);
        }
        else {
          fVar28 = SUB84(dVar25 - dVar25,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(*(undefined4 *)((int)pdVar10 + 4),fVar28);
        }
        else {
          dVar25 = *(double *)((int)pdVar10 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),
                                    SUB84(dVar23,0) * (float)((ulonglong)dVar25 >> 0x20));
        }
        else {
          dVar23 = dVar23 * dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(*(undefined4 *)(pdVar10 + 1),SUB84(dVar25,0));
        }
        else {
          dVar25 = pdVar10[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                    SUB84(dVar25,0) - SUB84(dVar23,0));
        }
        else {
          dVar25 = dVar25 - dVar23;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*DAT_8c618330);
        }
        if (in_FPSCR_PR == '\0') {
          fVar28 = SUB84(dVar23,0) * (float)((ulonglong)dVar25 >> 0x20);
          dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),fVar28);
          fVar28 = SUB84(dVar25,0) - fVar28;
        }
        else {
          dVar23 = dVar23 * dVar25;
          fVar28 = SUB84(dVar25 - dVar23,0);
        }
        if (in_FPSCR_SZ == '\0') {
          *(float *)((int)pdVar10 + 0x1c) = fVar28;
          dVar25 = (double)CONCAT44(*(undefined4 *)(iVar2 + 0x1c),fVar28);
        }
        else {
          *(double *)((int)pdVar10 + 0x1c) = in_xd2;
          dVar25 = *(double *)(iVar2 + 0x1c);
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar25,0),
                                    SUB84(dVar25,0));
        }
        else {
          dVar25 = dVar25 * dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar12 = pdVar12 + 7;
          *(uint *)pdVar12 = (uint)((ulonglong)dVar25 >> 0x20);
          dVar25 = (double)CONCAT44(*(undefined4 *)(iVar2 + 0x18),
                                    *(undefined4 *)((int)pdVar10 + 0x1c));
        }
        else {
          pdVar12 = (double *)((int)pdVar12 + 0x34);
          *pdVar12 = dVar25;
          in_xd2 = *(double *)((int)pdVar10 + 0x1c);
          dVar25 = *(double *)(iVar2 + 0x18);
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar25,0),
                                    SUB84(dVar25,0));
        }
        else {
          dVar25 = dVar25 * dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar12 = (double *)((int)pdVar12 + -4);
          *(uint *)pdVar12 = (uint)((ulonglong)dVar25 >> 0x20);
          dVar25 = (double)CONCAT44(*(undefined4 *)(iVar2 + 0x14),
                                    *(undefined4 *)((int)pdVar10 + 0x1c));
        }
        else {
          pdVar12 = pdVar12 + -1;
          *pdVar12 = dVar25;
          in_xd2 = *(double *)((int)pdVar10 + 0x1c);
          dVar25 = *(double *)(iVar2 + 0x14);
        }
        uVar6 = DAT_8c61832c;
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar25,0),
                                    SUB84(dVar25,0));
        }
        else {
          dVar25 = dVar25 * dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          *(uint *)((int)pdVar12 + -4) = (uint)((ulonglong)dVar25 >> 0x20);
          *(uint *)(pdVar12 + -1) = *(uint *)(iVar2 + 0x10);
          *(uint *)((int)pdVar12 + -0xc) = *(uint *)(pdVar10 + 3);
          *(uint *)(pdVar12 + -2) = *(uint *)((int)pdVar10 + 0x14);
          pdVar12 = (double *)((int)pdVar12 + -0x14);
          *(uint *)pdVar12 = *(uint *)(pdVar10 + 2);
        }
        else {
          pdVar12[-1] = dVar25;
          pdVar12[-2] = *(double *)(iVar2 + 0x10);
          pdVar12[-3] = pdVar10[3];
          pdVar12[-4] = *(double *)((int)pdVar10 + 0x14);
          in_xd2 = pdVar10[2];
          pdVar12 = pdVar12 + -5;
          *pdVar12 = in_xd2;
        }
        *(uint *)((int)pdVar12 + -4) = uVar6;
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(*(undefined4 *)DAT_8c618320,*(undefined4 *)pdVar19);
        }
        else {
          in_xd2 = *pdVar19;
          dVar25 = *DAT_8c618320;
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar25,0),
                                    SUB84(dVar25,0));
        }
        else {
          dVar25 = dVar25 * dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*puVar4);
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)DAT_8c618324);
        }
        else {
          in_xd2 = *DAT_8c618324;
        }
        if (in_FPSCR_PR == '\0') {
          fVar28 = SUB84(dVar25,0) - (float)((ulonglong)dVar25 >> 0x20);
        }
        else {
          fVar28 = SUB84(dVar25 - dVar25,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(*(undefined4 *)((int)pdVar19 + 4),fVar28);
        }
        else {
          dVar25 = *(double *)((int)pdVar19 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),
                                    SUB84(dVar23,0) * (float)((ulonglong)dVar25 >> 0x20));
        }
        else {
          dVar23 = dVar23 * dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(*(undefined4 *)(pdVar19 + 1),SUB84(dVar25,0));
        }
        else {
          dVar25 = pdVar19[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                    SUB84(dVar25,0) - SUB84(dVar23,0));
        }
        else {
          dVar25 = dVar25 - dVar23;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*DAT_8c618330);
        }
        if (in_FPSCR_PR == '\0') {
          fVar28 = SUB84(dVar23,0) * (float)((ulonglong)dVar25 >> 0x20);
          in_dr0 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),fVar28);
          fVar28 = SUB84(dVar25,0) - fVar28;
        }
        else {
          in_dr0 = dVar23 * dVar25;
          fVar28 = SUB84(dVar25 - in_dr0,0);
        }
        if (in_FPSCR_SZ == '\0') {
          *(float *)((int)pdVar19 + 0x1c) = fVar28;
          dVar25 = (double)CONCAT44(*(undefined4 *)(iVar2 + 0x1c),fVar28);
        }
        else {
          *(double *)((int)pdVar19 + 0x1c) = in_xd2;
          dVar25 = *(double *)(iVar2 + 0x1c);
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar25,0),
                                    SUB84(dVar25,0));
        }
        else {
          dVar25 = dVar25 * dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar12 = pdVar12 + 7;
          *(uint *)pdVar12 = (uint)((ulonglong)dVar25 >> 0x20);
          dVar25 = (double)CONCAT44(*(undefined4 *)(iVar2 + 0x18),
                                    *(undefined4 *)((int)pdVar19 + 0x1c));
        }
        else {
          pdVar12 = (double *)((int)pdVar12 + 0x34);
          *pdVar12 = dVar25;
          in_xd2 = *(double *)((int)pdVar19 + 0x1c);
          dVar25 = *(double *)(iVar2 + 0x18);
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar25,0),
                                    SUB84(dVar25,0));
        }
        else {
          dVar25 = dVar25 * dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar12 = (double *)((int)pdVar12 + -4);
          *(uint *)pdVar12 = (uint)((ulonglong)dVar25 >> 0x20);
          dVar25 = (double)CONCAT44(*(undefined4 *)(iVar2 + 0x14),
                                    *(undefined4 *)((int)pdVar19 + 0x1c));
        }
        else {
          pdVar12 = pdVar12 + -1;
          *pdVar12 = dVar25;
          in_xd2 = *(double *)((int)pdVar19 + 0x1c);
          dVar25 = *(double *)(iVar2 + 0x14);
        }
        uVar6 = DAT_8c618334;
        if (in_FPSCR_PR == '\0') {
          in_dr2 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar25,0),
                                    SUB84(dVar25,0));
        }
        else {
          in_dr2 = dVar25 * dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          *(uint *)((int)pdVar12 + -4) = (uint)((ulonglong)in_dr2 >> 0x20);
          *(uint *)(pdVar12 + -1) = *(uint *)(iVar2 + 0x10);
          *(uint *)((int)pdVar12 + -0xc) = *(uint *)(pdVar19 + 3);
          *(uint *)(pdVar12 + -2) = *(uint *)((int)pdVar19 + 0x14);
          in_dr2 = (double)(ulonglong)*(uint *)(pdVar19 + 2);
          pdVar12 = (double *)((int)pdVar12 + -0x14);
          *(uint *)pdVar12 = *(uint *)(pdVar19 + 2);
        }
        else {
          pdVar12[-1] = in_dr2;
          pdVar12[-2] = *(double *)(iVar2 + 0x10);
          pdVar12[-3] = pdVar19[3];
          pdVar12[-4] = *(double *)((int)pdVar19 + 0x14);
          in_xd2 = pdVar19[2];
          pdVar12 = pdVar12 + -5;
          *pdVar12 = in_xd2;
        }
        *(uint *)((int)pdVar12 + -4) = uVar6;
        unaff_r8 = (uint *)((int)pdVar12 + 0x1c);
        *DAT_8c618338 = *DAT_8c618338 + 4;
      }
    }
  }
  else if (uVar6 == DAT_8c617ec0) {
    for (iVar13 = 0; piVar5 = DAT_8c61831c, uVar21 = SUB84(param_1,0),
        iVar13 < (int)(uint)param_3[1]; iVar13 = iVar13 + 1) {
      puVar18 = puVar15 + 1;
      uVar6 = (uint)*puVar15;
      *piVar3 = *piVar3 + uVar6;
      iVar14 = *piVar5;
      iVar8 = (uint)puVar15[2] * 0x20 + iVar14;
      if (in_FPSCR_SZ == '\0') {
        uVar21 = *(undefined4 *)(iVar8 + 0x10);
      }
      else {
        in_xd4 = *(double *)(iVar8 + 0x10);
      }
      iVar11 = (uint)*puVar18 * 0x20 + iVar14;
      if (in_FPSCR_SZ == '\0') {
        dVar25 = (double)CONCAT44(uVar21,*(undefined4 *)(iVar11 + 0x10));
      }
      else {
        in_xd2 = *(double *)(iVar11 + 0x10);
        dVar25 = in_xd4;
      }
      if (in_FPSCR_PR == '\0') {
        dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) - SUB84(dVar25,0),
                                  SUB84(dVar25,0));
      }
      else {
        dVar25 = dVar25 - dVar25;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar23 = (double)CONCAT44(*(undefined4 *)(iVar8 + 0x14),uVar21);
      }
      else {
        dVar23 = *(double *)(iVar8 + 0x14);
      }
      iVar14 = (uint)puVar15[3] * 0x20 + iVar14;
      fVar28 = (float)((ulonglong)dVar23 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)(iVar14 + 0x14));
        in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar28);
      }
      else {
        in_xd2 = *(double *)(iVar14 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        fVar28 = SUB84(dVar25,0) - fVar28;
        dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * fVar28,fVar28);
      }
      else {
        dVar25 = (dVar25 - dVar23) * (dVar25 - dVar23);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)(iVar11 + 0x14));
      }
      else {
        in_xd2 = *(double *)(iVar11 + 0x14);
      }
      if (in_FPSCR_PR == '\0') {
        dVar20 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),SUB84(in_dr0,0) - SUB84(dVar25,0)
                                 );
      }
      else {
        dVar20 = in_dr0 - dVar25;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)(iVar14 + 0x10));
      }
      else {
        in_xd2 = *(double *)(iVar14 + 0x10);
      }
      if (in_FPSCR_PR == '\0') {
        dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),SUB84(dVar25,0) - SUB84(dVar23,0)
                                 );
      }
      else {
        dVar25 = dVar25 - dVar23;
      }
      uVar24 = (uint)((ulonglong)dVar25 >> 0x20);
      param_1 = dVar25;
      if (in_FPSCR_SZ == '\0') {
        param_1 = (double)CONCAT44(uVar24,SUB84(dVar23,0));
      }
      if (in_FPSCR_PR == '\0') {
        in_dr0 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0) * SUB84(dVar25,0)
                                 );
      }
      else {
        in_dr0 = dVar20 * dVar25;
      }
      dVar25 = (double)((ulonglong)uVar24 << 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar28 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
        param_1 = (double)CONCAT44(fVar28,SUB84(param_1,0));
        bVar31 = 0.0 < fVar28;
      }
      else {
        param_1 = param_1 - in_dr0;
        bVar31 = dVar25 < param_1;
      }
      if (bVar31) {
        puVar15 = puVar18 + uVar6;
      }
      else {
        *DAT_8c6184c8 = *DAT_8c6184c8 + uVar6;
        iVar14 = 0;
        if (0 < (int)(uVar6 - 1)) {
          do {
            uVar1 = *puVar18;
            puVar18 = puVar18 + 1;
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44(*(undefined4 *)DAT_8c6184d0,SUB84(dVar25,0));
            }
            else {
              dVar25 = *DAT_8c6184d0;
            }
            pdVar7 = (double *)((uint)uVar1 * 0x20 + *DAT_8c6184cc);
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)pdVar7);
              in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),*DAT_8c6184d8);
            }
            else {
              in_xd2 = *pdVar7;
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar25,0),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = dVar25 * dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)DAT_8c6184d4
                                       );
            }
            else {
              in_xd2 = *DAT_8c6184d4;
            }
            if (in_FPSCR_PR == '\0') {
              fVar28 = SUB84(dVar25,0) - (float)((ulonglong)dVar25 >> 0x20);
            }
            else {
              fVar28 = SUB84(dVar25 - dVar25,0);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44(*(undefined4 *)((int)pdVar7 + 4),fVar28);
            }
            else {
              dVar25 = *(double *)((int)pdVar7 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar23 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                        SUB84(in_dr0,0) * (float)((ulonglong)dVar25 >> 0x20));
            }
            else {
              dVar23 = in_dr0 * dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44(*(undefined4 *)(pdVar7 + 1),SUB84(dVar25,0));
            }
            else {
              dVar25 = pdVar7[1];
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                        SUB84(dVar25,0) - SUB84(dVar23,0));
            }
            else {
              dVar25 = dVar25 - dVar23;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*DAT_8c6184dc);
            }
            if (in_FPSCR_PR == '\0') {
              fVar28 = SUB84(dVar23,0) * (float)((ulonglong)dVar25 >> 0x20);
              in_dr0 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),fVar28);
              fVar28 = SUB84(dVar25,0) - fVar28;
            }
            else {
              in_dr0 = dVar23 * dVar25;
              fVar28 = SUB84(dVar25 - in_dr0,0);
            }
            if (in_FPSCR_SZ == '\0') {
              *(float *)((int)pdVar7 + 0x1c) = fVar28;
              dVar25 = (double)CONCAT44(*(undefined4 *)(iVar2 + 0x1c),fVar28);
            }
            else {
              *(double *)((int)pdVar7 + 0x1c) = in_xd2;
              dVar25 = *(double *)(iVar2 + 0x1c);
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar25,0),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = dVar25 * dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar9 = (double *)(unaff_r8 + 7);
              *(uint *)pdVar9 = (uint)((ulonglong)dVar25 >> 0x20);
              dVar25 = (double)CONCAT44(*(undefined4 *)(iVar2 + 0x18),
                                        *(undefined4 *)((int)pdVar7 + 0x1c));
            }
            else {
              pdVar9 = (double *)(unaff_r8 + 6);
              *pdVar9 = dVar25;
              in_xd2 = *(double *)((int)pdVar7 + 0x1c);
              dVar25 = *(double *)(iVar2 + 0x18);
            }
            uVar24 = DAT_8c6184e0;
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar25,0),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = dVar25 * dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar9 = (double *)((int)pdVar9 + -4);
              *(uint *)pdVar9 = (uint)((ulonglong)dVar25 >> 0x20);
              dVar25 = (double)CONCAT44(*(undefined4 *)(iVar2 + 0x14),
                                        *(undefined4 *)((int)pdVar7 + 0x1c));
            }
            else {
              pdVar9 = pdVar9 + -1;
              *pdVar9 = dVar25;
              in_xd2 = *(double *)((int)pdVar7 + 0x1c);
              dVar25 = *(double *)(iVar2 + 0x14);
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar25,0),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = dVar25 * dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              *(uint *)((int)pdVar9 + -4) = (uint)((ulonglong)dVar25 >> 0x20);
              *(uint *)(pdVar9 + -1) = *(uint *)(iVar2 + 0x10);
              *(uint *)((int)pdVar9 + -0xc) = *(uint *)(pdVar7 + 3);
              *(uint *)(pdVar9 + -2) = *(uint *)((int)pdVar7 + 0x14);
              dVar25 = (double)(ulonglong)*(uint *)(pdVar7 + 2);
              pdVar9 = (double *)((int)pdVar9 + -0x14);
              *(uint *)pdVar9 = *(uint *)(pdVar7 + 2);
            }
            else {
              pdVar9[-1] = dVar25;
              pdVar9[-2] = *(double *)(iVar2 + 0x10);
              pdVar9[-3] = pdVar7[3];
              pdVar9[-4] = *(double *)((int)pdVar7 + 0x14);
              in_xd2 = pdVar7[2];
              pdVar9 = pdVar9 + -5;
              *pdVar9 = in_xd2;
            }
            *(uint *)((int)pdVar9 + -4) = uVar24;
            iVar14 = iVar14 + 1;
            unaff_r8 = (uint *)((int)pdVar9 + 0x1c);
          } while (iVar14 < (int)(uVar6 - 1));
        }
        puVar15 = puVar18 + 1;
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(*(undefined4 *)DAT_8c6184d0,SUB84(dVar25,0));
        }
        else {
          dVar25 = *DAT_8c6184d0;
        }
        pdVar7 = (double *)((uint)*puVar18 * 0x20 + *DAT_8c6184cc);
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)pdVar7);
          in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),*DAT_8c6184d8);
        }
        else {
          in_xd2 = *pdVar7;
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar25,0),
                                    SUB84(dVar25,0));
        }
        else {
          dVar25 = dVar25 * dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)DAT_8c6184d4);
        }
        else {
          in_xd2 = *DAT_8c6184d4;
        }
        if (in_FPSCR_PR == '\0') {
          fVar28 = SUB84(dVar25,0) - (float)((ulonglong)dVar25 >> 0x20);
        }
        else {
          fVar28 = SUB84(dVar25 - dVar25,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(*(undefined4 *)((int)pdVar7 + 4),fVar28);
        }
        else {
          dVar25 = *(double *)((int)pdVar7 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar23 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                    SUB84(in_dr0,0) * (float)((ulonglong)dVar25 >> 0x20));
        }
        else {
          dVar23 = in_dr0 * dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar25 = (double)CONCAT44(*(undefined4 *)(pdVar7 + 1),SUB84(dVar25,0));
        }
        else {
          dVar25 = pdVar7[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                    SUB84(dVar25,0) - SUB84(dVar23,0));
        }
        else {
          dVar25 = dVar25 - dVar23;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*DAT_8c6184dc);
        }
        if (in_FPSCR_PR == '\0') {
          fVar28 = SUB84(dVar23,0) * (float)((ulonglong)dVar25 >> 0x20);
          in_dr0 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),fVar28);
          fVar28 = SUB84(dVar25,0) - fVar28;
        }
        else {
          in_dr0 = dVar23 * dVar25;
          fVar28 = SUB84(dVar25 - in_dr0,0);
        }
        if (in_FPSCR_SZ == '\0') {
          *(float *)((int)pdVar7 + 0x1c) = fVar28;
          dVar25 = (double)CONCAT44(*(undefined4 *)(iVar2 + 0x1c),fVar28);
        }
        else {
          *(double *)((int)pdVar7 + 0x1c) = in_xd2;
          dVar25 = *(double *)(iVar2 + 0x1c);
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar25,0),
                                    SUB84(dVar25,0));
        }
        else {
          dVar25 = dVar25 * dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar9 = (double *)(unaff_r8 + 7);
          *(uint *)pdVar9 = (uint)((ulonglong)dVar25 >> 0x20);
          dVar25 = (double)CONCAT44(*(undefined4 *)(iVar2 + 0x18),
                                    *(undefined4 *)((int)pdVar7 + 0x1c));
        }
        else {
          pdVar9 = (double *)(unaff_r8 + 6);
          *pdVar9 = dVar25;
          in_xd2 = *(double *)((int)pdVar7 + 0x1c);
          dVar25 = *(double *)(iVar2 + 0x18);
        }
        uVar6 = DAT_8c6184e4;
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar25,0),
                                    SUB84(dVar25,0));
        }
        else {
          dVar25 = dVar25 * dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          pdVar9 = (double *)((int)pdVar9 + -4);
          *(uint *)pdVar9 = (uint)((ulonglong)dVar25 >> 0x20);
          dVar25 = (double)CONCAT44(*(undefined4 *)(iVar2 + 0x14),
                                    *(undefined4 *)((int)pdVar7 + 0x1c));
        }
        else {
          pdVar9 = pdVar9 + -1;
          *pdVar9 = dVar25;
          in_xd2 = *(double *)((int)pdVar7 + 0x1c);
          dVar25 = *(double *)(iVar2 + 0x14);
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar25,0),
                                    SUB84(dVar25,0));
        }
        else {
          dVar25 = dVar25 * dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          *(uint *)((int)pdVar9 + -4) = (uint)((ulonglong)dVar25 >> 0x20);
          *(uint *)(pdVar9 + -1) = *(uint *)(iVar2 + 0x10);
          *(uint *)((int)pdVar9 + -0xc) = *(uint *)(pdVar7 + 3);
          *(uint *)(pdVar9 + -2) = *(uint *)((int)pdVar7 + 0x14);
          pdVar9 = (double *)((int)pdVar9 + -0x14);
          *(uint *)pdVar9 = *(uint *)(pdVar7 + 2);
        }
        else {
          pdVar9[-1] = dVar25;
          pdVar9[-2] = *(double *)(iVar2 + 0x10);
          pdVar9[-3] = pdVar7[3];
          pdVar9[-4] = *(double *)((int)pdVar7 + 0x14);
          in_xd2 = pdVar7[2];
          pdVar9 = pdVar9 + -5;
          *pdVar9 = in_xd2;
        }
        *(uint *)((int)pdVar9 + -4) = uVar6;
        unaff_r8 = (uint *)((int)pdVar9 + 0x1c);
      }
    }
  }
  else if (uVar6 == DAT_8c617ebc) {
    *DAT_8c6184e8 = param_3;
    if (in_FPSCR_SZ == '\0') {
      uVar26 = CONCAT44(*(undefined4 *)(DAT_8c6184c0 + 0x10),*(undefined4 *)(DAT_8c6184c0 + 0x14));
      uVar29 = CONCAT44(*(undefined4 *)(DAT_8c6184c0 + 0x18),*(undefined4 *)(DAT_8c6184c0 + 0x1c));
      uVar30 = CONCAT44(*(undefined4 *)DAT_8c6184c4,param_2);
    }
    else {
      uVar26 = *(undefined8 *)(DAT_8c6184c0 + 0x10);
      uVar29 = *(undefined8 *)(DAT_8c6184c0 + 0x20);
      uVar30 = *DAT_8c6184c4;
    }
    (*(code *)PTR_FUN_8c6184ec)(uVar26,uVar29,uVar30,*DAT_8c6184e8);
  }
  return;
}

