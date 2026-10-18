/* Ghidra draft only; not buildable/verified source. */

void FUN_8c6184f0(double param_1,undefined4 param_2,ushort *param_3)

{
  ushort uVar1;
  double *pdVar2;
  int *piVar3;
  uint uVar4;
  double *pdVar5;
  ushort *puVar6;
  double *pdVar7;
  int iVar8;
  uint uVar9;
  int iVar10;
  uint *unaff_r8;
  double *pdVar11;
  int iVar12;
  ushort *puVar13;
  ushort *puVar14;
  ushort *puVar15;
  double *pdVar16;
  int iVar17;
  double *pdVar18;
  uint uVar19;
  int iVar20;
  double *pdVar21;
  uint uVar22;
  double in_dr0;
  double dVar23;
  double dVar24;
  double dVar25;
  float fVar28;
  undefined8 uVar26;
  undefined8 uVar29;
  undefined8 uVar30;
  double in_xd2;
  double in_xd4;
  bool bVar31;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined4 uVar27;
  
  uVar4 = *param_3 & DAT_8c618554;
  puVar13 = *(ushort **)(param_3 + 2);
  if (uVar4 == 0) {
    *DAT_8c61855c = *DAT_8c61855c + (uint)param_3[1] * 3;
    for (iVar12 = 0; pdVar7 = DAT_8c618764, pdVar2 = DAT_8c618760, uVar27 = SUB84(param_1,0),
        iVar12 < (int)(uint)param_3[1]; iVar12 = iVar12 + 1) {
      uVar1 = *puVar13;
      puVar6 = puVar13 + 1;
      puVar15 = puVar13 + 2;
      iVar8 = *DAT_8c618750;
      puVar13 = puVar13 + 3;
      pdVar5 = (double *)((uint)uVar1 * 0x20 + iVar8);
      pdVar16 = (double *)((uint)*puVar6 * 0x20 + iVar8);
      pdVar18 = (double *)((uint)*puVar15 * 0x20 + iVar8);
      if ((*(uint *)((int)pdVar5 + 0xc) & *(uint *)((int)pdVar16 + 0xc) &
           *(uint *)((int)pdVar18 + 0xc) & 0x3f) == 0) {
        if (((*(uint *)((int)pdVar5 + 0xc) | *(uint *)((int)pdVar16 + 0xc) |
             *(uint *)((int)pdVar18 + 0xc)) & 1) == 0) {
          if (in_FPSCR_SZ == '\0') {
            uVar27 = *(undefined4 *)(pdVar16 + 2);
            dVar24 = (double)CONCAT44(uVar27,*(undefined4 *)(pdVar5 + 2));
          }
          else {
            dVar24 = pdVar16[2];
            in_xd2 = pdVar5[2];
          }
          if (in_FPSCR_PR == '\0') {
            dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) - SUB84(dVar24,0),
                                      SUB84(dVar24,0));
          }
          else {
            dVar24 = dVar24 - dVar24;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                      *(undefined4 *)((int)pdVar18 + 0x14));
            dVar25 = (double)CONCAT44(*(undefined4 *)((int)pdVar16 + 0x14),uVar27);
          }
          else {
            in_xd2 = *(double *)((int)pdVar18 + 0x14);
            dVar25 = *(double *)((int)pdVar16 + 0x14);
          }
          fVar28 = (float)((ulonglong)dVar25 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),SUB84(dVar24,0) - fVar28);
          }
          else {
            dVar24 = dVar24 - dVar25;
          }
          if (in_FPSCR_SZ == '\0') {
            in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar28);
          }
          if (in_FPSCR_PR == '\0') {
            dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar24,0),
                                      SUB84(dVar24,0));
          }
          else {
            dVar24 = dVar24 * dVar24;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                      *(undefined4 *)((int)pdVar5 + 0x14));
          }
          else {
            in_xd2 = *(double *)((int)pdVar5 + 0x14);
          }
          if (in_FPSCR_PR == '\0') {
            dVar23 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                      SUB84(in_dr0,0) - SUB84(dVar24,0));
          }
          else {
            dVar23 = in_dr0 - dVar24;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),*(undefined4 *)(pdVar18 + 2))
            ;
          }
          else {
            in_xd2 = pdVar18[2];
          }
          if (in_FPSCR_PR == '\0') {
            dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                      SUB84(dVar24,0) - SUB84(dVar25,0));
          }
          else {
            dVar24 = dVar24 - dVar25;
          }
          uVar4 = (uint)((ulonglong)dVar24 >> 0x20);
          param_1 = dVar24;
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44(uVar4,SUB84(dVar25,0));
          }
          if (in_FPSCR_PR == '\0') {
            in_dr0 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),
                                      SUB84(dVar23,0) * SUB84(dVar24,0));
          }
          else {
            in_dr0 = dVar23 * dVar24;
          }
          if (in_FPSCR_PR == '\0') {
            fVar28 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
            param_1 = (double)CONCAT44(fVar28,SUB84(param_1,0));
            bVar31 = 0.0 < fVar28;
          }
          else {
            param_1 = param_1 - in_dr0;
            bVar31 = (double)((ulonglong)uVar4 << 0x20) < param_1;
          }
          if (!bVar31) {
            if (in_FPSCR_SZ == '\0') {
              dVar24 = (double)CONCAT44(*(undefined4 *)DAT_8c618754,*(undefined4 *)pdVar5);
            }
            else {
              in_xd2 = *pdVar5;
              dVar24 = *DAT_8c618754;
            }
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar24,0),
                                        SUB84(dVar24,0));
            }
            else {
              dVar24 = dVar24 * dVar24;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),*(undefined4 *)DAT_8c618758
                                       );
              in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),*DAT_8c61875c);
            }
            else {
              in_xd2 = *DAT_8c618758;
            }
            if (in_FPSCR_PR == '\0') {
              fVar28 = SUB84(dVar24,0) - (float)((ulonglong)dVar24 >> 0x20);
            }
            else {
              fVar28 = SUB84(dVar24 - dVar24,0);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar24 = (double)CONCAT44(*(undefined4 *)((int)pdVar5 + 4),fVar28);
            }
            else {
              dVar24 = *(double *)((int)pdVar5 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                        SUB84(in_dr0,0) * (float)((ulonglong)dVar24 >> 0x20));
            }
            else {
              dVar25 = in_dr0 * dVar24;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar24 = (double)CONCAT44(*(undefined4 *)(pdVar5 + 1),SUB84(dVar24,0));
            }
            else {
              dVar24 = pdVar5[1];
            }
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                        SUB84(dVar24,0) - SUB84(dVar25,0));
            }
            else {
              dVar24 = dVar24 - dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                        *(undefined4 *)(DAT_8c618754 + 1));
            }
            if (in_FPSCR_PR == '\0') {
              fVar28 = SUB84(dVar25,0) * (float)((ulonglong)dVar24 >> 0x20);
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),fVar28);
              fVar28 = SUB84(dVar24,0) - fVar28;
            }
            else {
              dVar25 = dVar25 * dVar24;
              fVar28 = SUB84(dVar24 - dVar25,0);
            }
            if (in_FPSCR_SZ == '\0') {
              *(float *)((int)pdVar5 + 0x1c) = fVar28;
              dVar24 = (double)CONCAT44(*(undefined4 *)pdVar2,fVar28);
            }
            else {
              *(double *)((int)pdVar5 + 0x1c) = in_xd2;
              dVar24 = *pdVar2;
            }
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar24,0),
                                        SUB84(dVar24,0));
            }
            else {
              dVar24 = dVar24 * dVar24;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar21 = (double *)(unaff_r8 + 7);
              *(uint *)pdVar21 = (uint)((ulonglong)dVar24 >> 0x20);
              dVar24 = (double)CONCAT44(*(undefined4 *)pdVar7,*(undefined4 *)((int)pdVar5 + 0x1c));
            }
            else {
              pdVar21 = (double *)(unaff_r8 + 6);
              *pdVar21 = dVar24;
              dVar24 = *pdVar7;
              in_xd2 = *(double *)((int)pdVar5 + 0x1c);
            }
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar24,0),
                                        SUB84(dVar24,0));
            }
            else {
              dVar24 = dVar24 * dVar24;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar21 = (double *)((int)pdVar21 + -4);
              *(uint *)pdVar21 = (uint)((ulonglong)dVar24 >> 0x20);
              dVar24 = (double)CONCAT44(*(undefined4 *)DAT_8c618768,
                                        *(undefined4 *)((int)pdVar5 + 0x1c));
              uVar4 = DAT_8c61876c;
            }
            else {
              pdVar21 = pdVar21 + -1;
              *pdVar21 = dVar24;
              in_xd2 = *(double *)((int)pdVar5 + 0x1c);
              dVar24 = *DAT_8c618768;
              uVar4 = DAT_8c61876c;
            }
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar24,0),
                                        SUB84(dVar24,0));
            }
            else {
              dVar24 = dVar24 * dVar24;
            }
            DAT_8c61876c = uVar4;
            if (in_FPSCR_SZ == '\0') {
              *(uint *)((int)pdVar21 + -4) = (uint)((ulonglong)dVar24 >> 0x20);
              *(uint *)(pdVar21 + -1) = *(uint *)((int)pdVar2 + -0xc);
              *(uint *)((int)pdVar21 + -0xc) = *(uint *)(pdVar5 + 3);
              *(uint *)(pdVar21 + -2) = *(uint *)((int)pdVar5 + 0x14);
              pdVar21 = (double *)((int)pdVar21 + -0x14);
              *(uint *)pdVar21 = *(uint *)(pdVar5 + 2);
            }
            else {
              pdVar21[-1] = dVar24;
              pdVar21[-2] = *(double *)((int)pdVar2 + -0xc);
              pdVar21[-3] = pdVar5[3];
              pdVar21[-4] = *(double *)((int)pdVar5 + 0x14);
              in_xd2 = pdVar5[2];
              pdVar21 = pdVar21 + -5;
              *pdVar21 = in_xd2;
            }
            *(uint *)((int)pdVar21 + -4) = uVar4;
            pdVar7 = DAT_8c618764;
            pdVar2 = DAT_8c618760;
            if (in_FPSCR_SZ == '\0') {
              dVar24 = (double)CONCAT44(*(undefined4 *)DAT_8c618754,*(undefined4 *)pdVar16);
            }
            else {
              in_xd2 = *pdVar16;
              dVar24 = *DAT_8c618754;
            }
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar24,0),
                                        SUB84(dVar24,0));
            }
            else {
              dVar24 = dVar24 * dVar24;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),*(undefined4 *)DAT_8c618758
                                       );
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*DAT_8c61875c);
            }
            else {
              in_xd2 = *DAT_8c618758;
            }
            if (in_FPSCR_PR == '\0') {
              fVar28 = SUB84(dVar24,0) - (float)((ulonglong)dVar24 >> 0x20);
            }
            else {
              fVar28 = SUB84(dVar24 - dVar24,0);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar24 = (double)CONCAT44(*(undefined4 *)((int)pdVar16 + 4),fVar28);
            }
            else {
              dVar24 = *(double *)((int)pdVar16 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                        SUB84(dVar25,0) * (float)((ulonglong)dVar24 >> 0x20));
            }
            else {
              dVar25 = dVar25 * dVar24;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar24 = (double)CONCAT44(*(undefined4 *)(pdVar16 + 1),SUB84(dVar24,0));
            }
            else {
              dVar24 = pdVar16[1];
            }
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                        SUB84(dVar24,0) - SUB84(dVar25,0));
            }
            else {
              dVar24 = dVar24 - dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                        *(undefined4 *)(DAT_8c618754 + 1));
            }
            if (in_FPSCR_PR == '\0') {
              fVar28 = SUB84(dVar25,0) * (float)((ulonglong)dVar24 >> 0x20);
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),fVar28);
              fVar28 = SUB84(dVar24,0) - fVar28;
            }
            else {
              dVar25 = dVar25 * dVar24;
              fVar28 = SUB84(dVar24 - dVar25,0);
            }
            if (in_FPSCR_SZ == '\0') {
              *(float *)((int)pdVar16 + 0x1c) = fVar28;
              dVar24 = (double)CONCAT44(*(undefined4 *)pdVar2,fVar28);
            }
            else {
              *(double *)((int)pdVar16 + 0x1c) = in_xd2;
              dVar24 = *pdVar2;
            }
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar24,0),
                                        SUB84(dVar24,0));
            }
            else {
              dVar24 = dVar24 * dVar24;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar21 = pdVar21 + 7;
              *(uint *)pdVar21 = (uint)((ulonglong)dVar24 >> 0x20);
              dVar24 = (double)CONCAT44(*(undefined4 *)pdVar7,*(undefined4 *)((int)pdVar16 + 0x1c));
            }
            else {
              pdVar21 = (double *)((int)pdVar21 + 0x34);
              *pdVar21 = dVar24;
              dVar24 = *pdVar7;
              in_xd2 = *(double *)((int)pdVar16 + 0x1c);
            }
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar24,0),
                                        SUB84(dVar24,0));
            }
            else {
              dVar24 = dVar24 * dVar24;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar21 = (double *)((int)pdVar21 + -4);
              *(uint *)pdVar21 = (uint)((ulonglong)dVar24 >> 0x20);
              dVar24 = (double)CONCAT44(*(undefined4 *)DAT_8c618768,
                                        *(undefined4 *)((int)pdVar16 + 0x1c));
              uVar4 = DAT_8c61876c;
            }
            else {
              pdVar21 = pdVar21 + -1;
              *pdVar21 = dVar24;
              in_xd2 = *(double *)((int)pdVar16 + 0x1c);
              dVar24 = *DAT_8c618768;
              uVar4 = DAT_8c61876c;
            }
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar24,0),
                                        SUB84(dVar24,0));
            }
            else {
              dVar24 = dVar24 * dVar24;
            }
            DAT_8c61876c = uVar4;
            if (in_FPSCR_SZ == '\0') {
              *(uint *)((int)pdVar21 + -4) = (uint)((ulonglong)dVar24 >> 0x20);
              *(uint *)(pdVar21 + -1) = *(uint *)((int)pdVar2 + -0xc);
              *(uint *)((int)pdVar21 + -0xc) = *(uint *)(pdVar16 + 3);
              *(uint *)(pdVar21 + -2) = *(uint *)((int)pdVar16 + 0x14);
              pdVar21 = (double *)((int)pdVar21 + -0x14);
              *(uint *)pdVar21 = *(uint *)(pdVar16 + 2);
            }
            else {
              pdVar21[-1] = dVar24;
              pdVar21[-2] = *(double *)((int)pdVar2 + -0xc);
              pdVar21[-3] = pdVar16[3];
              pdVar21[-4] = *(double *)((int)pdVar16 + 0x14);
              in_xd2 = pdVar16[2];
              pdVar21 = pdVar21 + -5;
              *pdVar21 = in_xd2;
            }
            *(uint *)((int)pdVar21 + -4) = uVar4;
            pdVar7 = DAT_8c618764;
            pdVar2 = DAT_8c618760;
            if (in_FPSCR_SZ == '\0') {
              dVar24 = (double)CONCAT44(*(undefined4 *)DAT_8c618754,*(undefined4 *)pdVar18);
            }
            else {
              in_xd2 = *pdVar18;
              dVar24 = *DAT_8c618754;
            }
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar24,0),
                                        SUB84(dVar24,0));
            }
            else {
              dVar24 = dVar24 * dVar24;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),*(undefined4 *)DAT_8c618758
                                       );
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*DAT_8c61875c);
            }
            else {
              in_xd2 = *DAT_8c618758;
            }
            if (in_FPSCR_PR == '\0') {
              fVar28 = SUB84(dVar24,0) - (float)((ulonglong)dVar24 >> 0x20);
            }
            else {
              fVar28 = SUB84(dVar24 - dVar24,0);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar24 = (double)CONCAT44(*(undefined4 *)((int)pdVar18 + 4),fVar28);
            }
            else {
              dVar24 = *(double *)((int)pdVar18 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                        SUB84(dVar25,0) * (float)((ulonglong)dVar24 >> 0x20));
            }
            else {
              dVar25 = dVar25 * dVar24;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar24 = (double)CONCAT44(*(undefined4 *)(pdVar18 + 1),SUB84(dVar24,0));
            }
            else {
              dVar24 = pdVar18[1];
            }
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                        SUB84(dVar24,0) - SUB84(dVar25,0));
            }
            else {
              dVar24 = dVar24 - dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                        *(undefined4 *)(DAT_8c618754 + 1));
            }
            if (in_FPSCR_PR == '\0') {
              fVar28 = SUB84(dVar25,0) * (float)((ulonglong)dVar24 >> 0x20);
              in_dr0 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),fVar28);
              fVar28 = SUB84(dVar24,0) - fVar28;
            }
            else {
              in_dr0 = dVar25 * dVar24;
              fVar28 = SUB84(dVar24 - in_dr0,0);
            }
            if (in_FPSCR_SZ == '\0') {
              *(float *)((int)pdVar18 + 0x1c) = fVar28;
              dVar24 = (double)CONCAT44(*(undefined4 *)pdVar2,fVar28);
            }
            else {
              *(double *)((int)pdVar18 + 0x1c) = in_xd2;
              dVar24 = *pdVar2;
            }
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar24,0),
                                        SUB84(dVar24,0));
            }
            else {
              dVar24 = dVar24 * dVar24;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar21 = pdVar21 + 7;
              *(uint *)pdVar21 = (uint)((ulonglong)dVar24 >> 0x20);
              dVar24 = (double)CONCAT44(*(undefined4 *)pdVar7,*(undefined4 *)((int)pdVar18 + 0x1c));
            }
            else {
              pdVar21 = (double *)((int)pdVar21 + 0x34);
              *pdVar21 = dVar24;
              dVar24 = *pdVar7;
              in_xd2 = *(double *)((int)pdVar18 + 0x1c);
            }
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar24,0),
                                        SUB84(dVar24,0));
            }
            else {
              dVar24 = dVar24 * dVar24;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar21 = (double *)((int)pdVar21 + -4);
              *(uint *)pdVar21 = (uint)((ulonglong)dVar24 >> 0x20);
              dVar24 = (double)CONCAT44(*(undefined4 *)DAT_8c618768,
                                        *(undefined4 *)((int)pdVar18 + 0x1c));
              uVar4 = DAT_8c618770;
            }
            else {
              pdVar21 = pdVar21 + -1;
              *pdVar21 = dVar24;
              in_xd2 = *(double *)((int)pdVar18 + 0x1c);
              dVar24 = *DAT_8c618768;
              uVar4 = DAT_8c618770;
            }
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar24,0),
                                        SUB84(dVar24,0));
            }
            else {
              dVar24 = dVar24 * dVar24;
            }
            DAT_8c618770 = uVar4;
            if (in_FPSCR_SZ == '\0') {
              *(uint *)((int)pdVar21 + -4) = (uint)((ulonglong)dVar24 >> 0x20);
              *(uint *)(pdVar21 + -1) = *(uint *)((int)pdVar2 + -0xc);
              *(uint *)((int)pdVar21 + -0xc) = *(uint *)(pdVar18 + 3);
              *(uint *)(pdVar21 + -2) = *(uint *)((int)pdVar18 + 0x14);
              pdVar21 = (double *)((int)pdVar21 + -0x14);
              *(uint *)pdVar21 = *(uint *)(pdVar18 + 2);
            }
            else {
              pdVar21[-1] = dVar24;
              pdVar21[-2] = *(double *)((int)pdVar2 + -0xc);
              pdVar21[-3] = pdVar18[3];
              pdVar21[-4] = *(double *)((int)pdVar18 + 0x14);
              in_xd2 = pdVar18[2];
              pdVar21 = pdVar21 + -5;
              *pdVar21 = in_xd2;
            }
            *(uint *)((int)pdVar21 + -4) = uVar4;
            unaff_r8 = (uint *)((int)pdVar21 + 0x1c);
            *DAT_8c618774 = *DAT_8c618774 + 3;
          }
        }
        else {
          *DAT_8c618778 = 1;
          in_dr0 = (double)(*(code *)PTR_FUN_8c61877c)();
        }
      }
    }
  }
  else if (uVar4 == (int)DAT_8c618552) {
    *DAT_8c618780 = *DAT_8c618780 + (uint)param_3[1] * 4;
    for (iVar12 = 0; pdVar7 = DAT_8c618a24, pdVar2 = DAT_8c618a20, iVar12 < (int)(uint)param_3[1];
        iVar12 = iVar12 + 1) {
      uVar1 = *puVar13;
      puVar6 = puVar13 + 1;
      puVar15 = puVar13 + 2;
      iVar8 = *DAT_8c618a10;
      puVar14 = puVar13 + 3;
      puVar13 = puVar13 + 4;
      pdVar5 = (double *)((uint)uVar1 * 0x20 + iVar8);
      pdVar18 = (double *)((uint)*puVar6 * 0x20 + iVar8);
      pdVar21 = (double *)((uint)*puVar15 * 0x20 + iVar8);
      pdVar16 = (double *)((uint)*puVar14 * 0x20 + iVar8);
      if ((*(uint *)((int)pdVar5 + 0xc) & *(uint *)((int)pdVar18 + 0xc) &
           *(uint *)((int)pdVar21 + 0xc) & *(uint *)((int)pdVar16 + 0xc) & 0x3f) == 0) {
        if (((*(uint *)((int)pdVar5 + 0xc) | *(uint *)((int)pdVar18 + 0xc) |
              *(uint *)((int)pdVar21 + 0xc) | *(uint *)((int)pdVar16 + 0xc)) & 1) == 0) {
          if (in_FPSCR_SZ == '\0') {
            uVar27 = *(undefined4 *)(pdVar18 + 2);
            dVar25 = (double)CONCAT44(uVar27,*(undefined4 *)(pdVar5 + 2));
            dVar24 = (double)CONCAT44(uVar27,SUB84(param_1,0));
          }
          else {
            dVar25 = pdVar18[2];
            in_xd2 = pdVar5[2];
            uVar27 = (undefined4)((ulonglong)dVar25 >> 0x20);
            dVar24 = dVar25;
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) - SUB84(dVar25,0),
                                      SUB84(dVar25,0));
          }
          else {
            dVar25 = dVar25 - dVar25;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                      *(undefined4 *)((int)pdVar21 + 0x14));
            dVar24 = (double)CONCAT44(uVar27,*(undefined4 *)((int)pdVar18 + 0x14));
          }
          else {
            in_xd2 = *(double *)((int)pdVar21 + 0x14);
          }
          fVar28 = SUB84(dVar24,0);
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),SUB84(dVar25,0) - fVar28);
          }
          else {
            dVar25 = dVar25 - dVar24;
          }
          if (in_FPSCR_SZ == '\0') {
            in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar28);
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar25,0),
                                      SUB84(dVar25,0));
          }
          else {
            dVar25 = dVar25 * dVar25;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                      *(undefined4 *)((int)pdVar5 + 0x14));
          }
          else {
            in_xd2 = *(double *)((int)pdVar5 + 0x14);
          }
          if (in_FPSCR_PR == '\0') {
            dVar23 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                      SUB84(in_dr0,0) - SUB84(dVar25,0));
          }
          else {
            dVar23 = in_dr0 - dVar25;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)(pdVar21 + 2))
            ;
          }
          else {
            in_xd2 = pdVar21[2];
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                      SUB84(dVar25,0) - (float)((ulonglong)dVar24 >> 0x20));
          }
          else {
            dVar25 = dVar25 - dVar24;
          }
          uVar4 = (uint)((ulonglong)dVar25 >> 0x20);
          param_1 = dVar25;
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44(uVar4,fVar28);
          }
          if (in_FPSCR_PR == '\0') {
            in_dr0 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),
                                      SUB84(dVar23,0) * SUB84(dVar25,0));
          }
          else {
            in_dr0 = dVar23 * dVar25;
          }
          if (in_FPSCR_PR == '\0') {
            fVar28 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
            param_1 = (double)CONCAT44(fVar28,SUB84(param_1,0));
            bVar31 = 0.0 < fVar28;
          }
          else {
            param_1 = param_1 - in_dr0;
            bVar31 = (double)((ulonglong)uVar4 << 0x20) < param_1;
          }
          if (!bVar31) {
            if (in_FPSCR_SZ == '\0') {
              dVar24 = (double)CONCAT44(*(undefined4 *)DAT_8c618a14,*(undefined4 *)pdVar5);
            }
            else {
              in_xd2 = *pdVar5;
              dVar24 = *DAT_8c618a14;
            }
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar24,0),
                                        SUB84(dVar24,0));
            }
            else {
              dVar24 = dVar24 * dVar24;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),*(undefined4 *)DAT_8c618a18
                                       );
              in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),*DAT_8c618a1c);
            }
            else {
              in_xd2 = *DAT_8c618a18;
            }
            if (in_FPSCR_PR == '\0') {
              fVar28 = SUB84(dVar24,0) - (float)((ulonglong)dVar24 >> 0x20);
            }
            else {
              fVar28 = SUB84(dVar24 - dVar24,0);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar24 = (double)CONCAT44(*(undefined4 *)((int)pdVar5 + 4),fVar28);
            }
            else {
              dVar24 = *(double *)((int)pdVar5 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                        SUB84(in_dr0,0) * (float)((ulonglong)dVar24 >> 0x20));
            }
            else {
              dVar25 = in_dr0 * dVar24;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar24 = (double)CONCAT44(*(undefined4 *)(pdVar5 + 1),SUB84(dVar24,0));
            }
            else {
              dVar24 = pdVar5[1];
            }
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                        SUB84(dVar24,0) - SUB84(dVar25,0));
            }
            else {
              dVar24 = dVar24 - dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                        *(undefined4 *)(DAT_8c618a14 + 1));
            }
            if (in_FPSCR_PR == '\0') {
              fVar28 = SUB84(dVar25,0) * (float)((ulonglong)dVar24 >> 0x20);
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),fVar28);
              fVar28 = SUB84(dVar24,0) - fVar28;
            }
            else {
              dVar25 = dVar25 * dVar24;
              fVar28 = SUB84(dVar24 - dVar25,0);
            }
            if (in_FPSCR_SZ == '\0') {
              *(float *)((int)pdVar5 + 0x1c) = fVar28;
              dVar24 = (double)CONCAT44(*(undefined4 *)pdVar2,fVar28);
            }
            else {
              *(double *)((int)pdVar5 + 0x1c) = in_xd2;
              dVar24 = *pdVar2;
            }
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar24,0),
                                        SUB84(dVar24,0));
            }
            else {
              dVar24 = dVar24 * dVar24;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar11 = (double *)(unaff_r8 + 7);
              *(uint *)pdVar11 = (uint)((ulonglong)dVar24 >> 0x20);
              dVar24 = (double)CONCAT44(*(undefined4 *)pdVar7,*(undefined4 *)((int)pdVar5 + 0x1c));
            }
            else {
              pdVar11 = (double *)(unaff_r8 + 6);
              *pdVar11 = dVar24;
              dVar24 = *pdVar7;
              in_xd2 = *(double *)((int)pdVar5 + 0x1c);
            }
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar24,0),
                                        SUB84(dVar24,0));
            }
            else {
              dVar24 = dVar24 * dVar24;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar11 = (double *)((int)pdVar11 + -4);
              *(uint *)pdVar11 = (uint)((ulonglong)dVar24 >> 0x20);
              dVar24 = (double)CONCAT44(*(undefined4 *)DAT_8c618a28,
                                        *(undefined4 *)((int)pdVar5 + 0x1c));
              uVar4 = DAT_8c618a2c;
            }
            else {
              pdVar11 = pdVar11 + -1;
              *pdVar11 = dVar24;
              in_xd2 = *(double *)((int)pdVar5 + 0x1c);
              dVar24 = *DAT_8c618a28;
              uVar4 = DAT_8c618a2c;
            }
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar24,0),
                                        SUB84(dVar24,0));
            }
            else {
              dVar24 = dVar24 * dVar24;
            }
            DAT_8c618a2c = uVar4;
            if (in_FPSCR_SZ == '\0') {
              *(uint *)((int)pdVar11 + -4) = (uint)((ulonglong)dVar24 >> 0x20);
              *(uint *)(pdVar11 + -1) = *(uint *)((int)pdVar2 + -0xc);
              *(uint *)((int)pdVar11 + -0xc) = *(uint *)(pdVar5 + 3);
              *(uint *)(pdVar11 + -2) = *(uint *)((int)pdVar5 + 0x14);
              pdVar11 = (double *)((int)pdVar11 + -0x14);
              *(uint *)pdVar11 = *(uint *)(pdVar5 + 2);
            }
            else {
              pdVar11[-1] = dVar24;
              pdVar11[-2] = *(double *)((int)pdVar2 + -0xc);
              pdVar11[-3] = pdVar5[3];
              pdVar11[-4] = *(double *)((int)pdVar5 + 0x14);
              in_xd2 = pdVar5[2];
              pdVar11 = pdVar11 + -5;
              *pdVar11 = in_xd2;
            }
            *(uint *)((int)pdVar11 + -4) = uVar4;
            pdVar7 = DAT_8c618a24;
            pdVar2 = DAT_8c618a20;
            if (in_FPSCR_SZ == '\0') {
              dVar24 = (double)CONCAT44(*(undefined4 *)DAT_8c618a14,*(undefined4 *)pdVar18);
            }
            else {
              in_xd2 = *pdVar18;
              dVar24 = *DAT_8c618a14;
            }
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar24,0),
                                        SUB84(dVar24,0));
            }
            else {
              dVar24 = dVar24 * dVar24;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),*(undefined4 *)DAT_8c618a18
                                       );
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*DAT_8c618a1c);
            }
            else {
              in_xd2 = *DAT_8c618a18;
            }
            if (in_FPSCR_PR == '\0') {
              fVar28 = SUB84(dVar24,0) - (float)((ulonglong)dVar24 >> 0x20);
            }
            else {
              fVar28 = SUB84(dVar24 - dVar24,0);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar24 = (double)CONCAT44(*(undefined4 *)((int)pdVar18 + 4),fVar28);
            }
            else {
              dVar24 = *(double *)((int)pdVar18 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                        SUB84(dVar25,0) * (float)((ulonglong)dVar24 >> 0x20));
            }
            else {
              dVar25 = dVar25 * dVar24;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar24 = (double)CONCAT44(*(undefined4 *)(pdVar18 + 1),SUB84(dVar24,0));
            }
            else {
              dVar24 = pdVar18[1];
            }
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                        SUB84(dVar24,0) - SUB84(dVar25,0));
            }
            else {
              dVar24 = dVar24 - dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                        *(undefined4 *)(DAT_8c618a14 + 1));
            }
            if (in_FPSCR_PR == '\0') {
              fVar28 = SUB84(dVar25,0) * (float)((ulonglong)dVar24 >> 0x20);
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),fVar28);
              fVar28 = SUB84(dVar24,0) - fVar28;
            }
            else {
              dVar25 = dVar25 * dVar24;
              fVar28 = SUB84(dVar24 - dVar25,0);
            }
            if (in_FPSCR_SZ == '\0') {
              *(float *)((int)pdVar18 + 0x1c) = fVar28;
              dVar24 = (double)CONCAT44(*(undefined4 *)pdVar2,fVar28);
            }
            else {
              *(double *)((int)pdVar18 + 0x1c) = in_xd2;
              dVar24 = *pdVar2;
            }
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar24,0),
                                        SUB84(dVar24,0));
            }
            else {
              dVar24 = dVar24 * dVar24;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar11 = pdVar11 + 7;
              *(uint *)pdVar11 = (uint)((ulonglong)dVar24 >> 0x20);
              dVar24 = (double)CONCAT44(*(undefined4 *)pdVar7,*(undefined4 *)((int)pdVar18 + 0x1c));
            }
            else {
              pdVar11 = (double *)((int)pdVar11 + 0x34);
              *pdVar11 = dVar24;
              dVar24 = *pdVar7;
              in_xd2 = *(double *)((int)pdVar18 + 0x1c);
            }
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar24,0),
                                        SUB84(dVar24,0));
            }
            else {
              dVar24 = dVar24 * dVar24;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar11 = (double *)((int)pdVar11 + -4);
              *(uint *)pdVar11 = (uint)((ulonglong)dVar24 >> 0x20);
              dVar24 = (double)CONCAT44(*(undefined4 *)DAT_8c618a28,
                                        *(undefined4 *)((int)pdVar18 + 0x1c));
              uVar4 = DAT_8c618a2c;
            }
            else {
              pdVar11 = pdVar11 + -1;
              *pdVar11 = dVar24;
              in_xd2 = *(double *)((int)pdVar18 + 0x1c);
              dVar24 = *DAT_8c618a28;
              uVar4 = DAT_8c618a2c;
            }
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar24,0),
                                        SUB84(dVar24,0));
            }
            else {
              dVar24 = dVar24 * dVar24;
            }
            DAT_8c618a2c = uVar4;
            if (in_FPSCR_SZ == '\0') {
              *(uint *)((int)pdVar11 + -4) = (uint)((ulonglong)dVar24 >> 0x20);
              *(uint *)(pdVar11 + -1) = *(uint *)((int)pdVar2 + -0xc);
              *(uint *)((int)pdVar11 + -0xc) = *(uint *)(pdVar18 + 3);
              *(uint *)(pdVar11 + -2) = *(uint *)((int)pdVar18 + 0x14);
              pdVar11 = (double *)((int)pdVar11 + -0x14);
              *(uint *)pdVar11 = *(uint *)(pdVar18 + 2);
            }
            else {
              pdVar11[-1] = dVar24;
              pdVar11[-2] = *(double *)((int)pdVar2 + -0xc);
              pdVar11[-3] = pdVar18[3];
              pdVar11[-4] = *(double *)((int)pdVar18 + 0x14);
              in_xd2 = pdVar18[2];
              pdVar11 = pdVar11 + -5;
              *pdVar11 = in_xd2;
            }
            *(uint *)((int)pdVar11 + -4) = uVar4;
            pdVar7 = DAT_8c618a24;
            pdVar2 = DAT_8c618a20;
            if (in_FPSCR_SZ == '\0') {
              dVar24 = (double)CONCAT44(*(undefined4 *)DAT_8c618a14,*(undefined4 *)pdVar21);
            }
            else {
              in_xd2 = *pdVar21;
              dVar24 = *DAT_8c618a14;
            }
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar24,0),
                                        SUB84(dVar24,0));
            }
            else {
              dVar24 = dVar24 * dVar24;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),*(undefined4 *)DAT_8c618a18
                                       );
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*DAT_8c618a1c);
            }
            else {
              in_xd2 = *DAT_8c618a18;
            }
            if (in_FPSCR_PR == '\0') {
              fVar28 = SUB84(dVar24,0) - (float)((ulonglong)dVar24 >> 0x20);
            }
            else {
              fVar28 = SUB84(dVar24 - dVar24,0);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar24 = (double)CONCAT44(*(undefined4 *)((int)pdVar21 + 4),fVar28);
            }
            else {
              dVar24 = *(double *)((int)pdVar21 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                        SUB84(dVar25,0) * (float)((ulonglong)dVar24 >> 0x20));
            }
            else {
              dVar25 = dVar25 * dVar24;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar24 = (double)CONCAT44(*(undefined4 *)(pdVar21 + 1),SUB84(dVar24,0));
            }
            else {
              dVar24 = pdVar21[1];
            }
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                        SUB84(dVar24,0) - SUB84(dVar25,0));
            }
            else {
              dVar24 = dVar24 - dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                        *(undefined4 *)(DAT_8c618a14 + 1));
            }
            if (in_FPSCR_PR == '\0') {
              fVar28 = SUB84(dVar25,0) * (float)((ulonglong)dVar24 >> 0x20);
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),fVar28);
              fVar28 = SUB84(dVar24,0) - fVar28;
            }
            else {
              dVar25 = dVar25 * dVar24;
              fVar28 = SUB84(dVar24 - dVar25,0);
            }
            if (in_FPSCR_SZ == '\0') {
              *(float *)((int)pdVar21 + 0x1c) = fVar28;
              dVar24 = (double)CONCAT44(*(undefined4 *)pdVar2,fVar28);
            }
            else {
              *(double *)((int)pdVar21 + 0x1c) = in_xd2;
              dVar24 = *pdVar2;
            }
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar24,0),
                                        SUB84(dVar24,0));
            }
            else {
              dVar24 = dVar24 * dVar24;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar11 = pdVar11 + 7;
              *(uint *)pdVar11 = (uint)((ulonglong)dVar24 >> 0x20);
              dVar24 = (double)CONCAT44(*(undefined4 *)pdVar7,*(undefined4 *)((int)pdVar21 + 0x1c));
            }
            else {
              pdVar11 = (double *)((int)pdVar11 + 0x34);
              *pdVar11 = dVar24;
              dVar24 = *pdVar7;
              in_xd2 = *(double *)((int)pdVar21 + 0x1c);
            }
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar24,0),
                                        SUB84(dVar24,0));
            }
            else {
              dVar24 = dVar24 * dVar24;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar11 = (double *)((int)pdVar11 + -4);
              *(uint *)pdVar11 = (uint)((ulonglong)dVar24 >> 0x20);
              dVar24 = (double)CONCAT44(*(undefined4 *)DAT_8c618a28,
                                        *(undefined4 *)((int)pdVar21 + 0x1c));
              uVar4 = DAT_8c618a2c;
            }
            else {
              pdVar11 = pdVar11 + -1;
              *pdVar11 = dVar24;
              in_xd2 = *(double *)((int)pdVar21 + 0x1c);
              dVar24 = *DAT_8c618a28;
              uVar4 = DAT_8c618a2c;
            }
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar24,0),
                                        SUB84(dVar24,0));
            }
            else {
              dVar24 = dVar24 * dVar24;
            }
            DAT_8c618a2c = uVar4;
            if (in_FPSCR_SZ == '\0') {
              *(uint *)((int)pdVar11 + -4) = (uint)((ulonglong)dVar24 >> 0x20);
              *(uint *)(pdVar11 + -1) = *(uint *)((int)pdVar2 + -0xc);
              *(uint *)((int)pdVar11 + -0xc) = *(uint *)(pdVar21 + 3);
              *(uint *)(pdVar11 + -2) = *(uint *)((int)pdVar21 + 0x14);
              pdVar11 = (double *)((int)pdVar11 + -0x14);
              *(uint *)pdVar11 = *(uint *)(pdVar21 + 2);
            }
            else {
              pdVar11[-1] = dVar24;
              pdVar11[-2] = *(double *)((int)pdVar2 + -0xc);
              pdVar11[-3] = pdVar21[3];
              pdVar11[-4] = *(double *)((int)pdVar21 + 0x14);
              in_xd2 = pdVar21[2];
              pdVar11 = pdVar11 + -5;
              *pdVar11 = in_xd2;
            }
            *(uint *)((int)pdVar11 + -4) = uVar4;
            pdVar7 = DAT_8c618a24;
            pdVar2 = DAT_8c618a20;
            if (in_FPSCR_SZ == '\0') {
              dVar24 = (double)CONCAT44(*(undefined4 *)DAT_8c618a14,*(undefined4 *)pdVar16);
            }
            else {
              in_xd2 = *pdVar16;
              dVar24 = *DAT_8c618a14;
            }
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar24,0),
                                        SUB84(dVar24,0));
            }
            else {
              dVar24 = dVar24 * dVar24;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),*(undefined4 *)DAT_8c618a18
                                       );
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*DAT_8c618a1c);
            }
            else {
              in_xd2 = *DAT_8c618a18;
            }
            if (in_FPSCR_PR == '\0') {
              fVar28 = SUB84(dVar24,0) - (float)((ulonglong)dVar24 >> 0x20);
            }
            else {
              fVar28 = SUB84(dVar24 - dVar24,0);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar24 = (double)CONCAT44(*(undefined4 *)((int)pdVar16 + 4),fVar28);
            }
            else {
              dVar24 = *(double *)((int)pdVar16 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                        SUB84(dVar25,0) * (float)((ulonglong)dVar24 >> 0x20));
            }
            else {
              dVar25 = dVar25 * dVar24;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar24 = (double)CONCAT44(*(undefined4 *)(pdVar16 + 1),SUB84(dVar24,0));
            }
            else {
              dVar24 = pdVar16[1];
            }
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                        SUB84(dVar24,0) - SUB84(dVar25,0));
            }
            else {
              dVar24 = dVar24 - dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                        *(undefined4 *)(DAT_8c618a14 + 1));
            }
            if (in_FPSCR_PR == '\0') {
              fVar28 = SUB84(dVar25,0) * (float)((ulonglong)dVar24 >> 0x20);
              in_dr0 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),fVar28);
              fVar28 = SUB84(dVar24,0) - fVar28;
            }
            else {
              in_dr0 = dVar25 * dVar24;
              fVar28 = SUB84(dVar24 - in_dr0,0);
            }
            if (in_FPSCR_SZ == '\0') {
              *(float *)((int)pdVar16 + 0x1c) = fVar28;
              dVar24 = (double)CONCAT44(*(undefined4 *)pdVar2,fVar28);
            }
            else {
              *(double *)((int)pdVar16 + 0x1c) = in_xd2;
              dVar24 = *pdVar2;
            }
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar24,0),
                                        SUB84(dVar24,0));
            }
            else {
              dVar24 = dVar24 * dVar24;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar11 = pdVar11 + 7;
              *(uint *)pdVar11 = (uint)((ulonglong)dVar24 >> 0x20);
              dVar24 = (double)CONCAT44(*(undefined4 *)pdVar7,*(undefined4 *)((int)pdVar16 + 0x1c));
            }
            else {
              pdVar11 = (double *)((int)pdVar11 + 0x34);
              *pdVar11 = dVar24;
              dVar24 = *pdVar7;
              in_xd2 = *(double *)((int)pdVar16 + 0x1c);
            }
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar24,0),
                                        SUB84(dVar24,0));
            }
            else {
              dVar24 = dVar24 * dVar24;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar11 = (double *)((int)pdVar11 + -4);
              *(uint *)pdVar11 = (uint)((ulonglong)dVar24 >> 0x20);
              dVar24 = (double)CONCAT44(*(undefined4 *)DAT_8c618a28,
                                        *(undefined4 *)((int)pdVar16 + 0x1c));
              uVar4 = DAT_8c618a30;
            }
            else {
              pdVar11 = pdVar11 + -1;
              *pdVar11 = dVar24;
              in_xd2 = *(double *)((int)pdVar16 + 0x1c);
              dVar24 = *DAT_8c618a28;
              uVar4 = DAT_8c618a30;
            }
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar24,0),
                                        SUB84(dVar24,0));
            }
            else {
              dVar24 = dVar24 * dVar24;
            }
            DAT_8c618a30 = uVar4;
            if (in_FPSCR_SZ == '\0') {
              *(uint *)((int)pdVar11 + -4) = (uint)((ulonglong)dVar24 >> 0x20);
              *(uint *)(pdVar11 + -1) = *(uint *)((int)pdVar2 + -0xc);
              *(uint *)((int)pdVar11 + -0xc) = *(uint *)(pdVar16 + 3);
              *(uint *)(pdVar11 + -2) = *(uint *)((int)pdVar16 + 0x14);
              pdVar11 = (double *)((int)pdVar11 + -0x14);
              *(uint *)pdVar11 = *(uint *)(pdVar16 + 2);
            }
            else {
              pdVar11[-1] = dVar24;
              pdVar11[-2] = *(double *)((int)pdVar2 + -0xc);
              pdVar11[-3] = pdVar16[3];
              pdVar11[-4] = *(double *)((int)pdVar16 + 0x14);
              in_xd2 = pdVar16[2];
              pdVar11 = pdVar11 + -5;
              *pdVar11 = in_xd2;
            }
            *(uint *)((int)pdVar11 + -4) = uVar4;
            unaff_r8 = (uint *)((int)pdVar11 + 0x1c);
            *DAT_8c618a34 = *DAT_8c618a34 + 4;
          }
        }
        else {
          *DAT_8c618a38 = 1;
          if ((*(uint *)((int)pdVar5 + 0xc) & *(uint *)((int)pdVar18 + 0xc) &
               *(uint *)((int)pdVar21 + 0xc) & 1) == 0) {
            in_dr0 = (double)(*(code *)PTR_FUN_8c618a3c)(pdVar18,pdVar21);
          }
          if ((*(uint *)((int)pdVar18 + 0xc) & *(uint *)((int)pdVar21 + 0xc) &
               *(uint *)((int)pdVar16 + 0xc) & 1) == 0) {
            in_dr0 = (double)(*(code *)PTR_FUN_8c618a3c)(pdVar18,pdVar21,pdVar16);
          }
        }
      }
    }
  }
  else if (uVar4 == DAT_8c618558) {
    for (iVar12 = 0; uVar27 = SUB84(param_1,0), iVar12 < (int)(uint)param_3[1]; iVar12 = iVar12 + 1)
    {
      uVar4 = 0;
      puVar15 = puVar13 + 1;
      iVar8 = 0;
      uVar19 = (uint)*puVar13;
      *DAT_8c618cd8 = *DAT_8c618cd8 + uVar19;
      piVar3 = DAT_8c618cdc;
      uVar9 = 0xffffffff;
      puVar6 = puVar15;
      if (uVar19 != 0) {
        do {
          uVar1 = *puVar6;
          puVar6 = puVar6 + 1;
          iVar8 = iVar8 + 1;
          uVar22 = *(uint *)((uint)uVar1 * 0x20 + *DAT_8c618cdc + 0xc);
          uVar9 = uVar9 & uVar22;
          uVar4 = uVar4 | uVar22;
        } while (iVar8 < (int)uVar19);
      }
      if ((uVar9 & 0x3f) == 0) {
        if ((uVar4 & 1) == 0) {
          iVar10 = *DAT_8c618cdc;
          iVar8 = (uint)puVar13[2] * 0x20 + iVar10;
          if (in_FPSCR_SZ == '\0') {
            uVar27 = *(undefined4 *)(iVar8 + 0x10);
          }
          else {
            in_xd4 = *(double *)(iVar8 + 0x10);
          }
          iVar17 = (uint)*puVar15 * 0x20 + iVar10;
          if (in_FPSCR_SZ == '\0') {
            dVar24 = (double)CONCAT44(uVar27,*(undefined4 *)(iVar17 + 0x10));
          }
          else {
            in_xd2 = *(double *)(iVar17 + 0x10);
            dVar24 = in_xd4;
          }
          if (in_FPSCR_PR == '\0') {
            dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) - SUB84(dVar24,0),
                                      SUB84(dVar24,0));
          }
          else {
            dVar24 = dVar24 - dVar24;
          }
          iVar10 = (uint)puVar13[3] * 0x20 + iVar10;
          if (in_FPSCR_SZ == '\0') {
            fVar28 = *(float *)(iVar8 + 0x14);
            dVar25 = (double)CONCAT44(fVar28,uVar27);
            dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                      *(undefined4 *)(iVar10 + 0x14));
            in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar28);
          }
          else {
            dVar25 = *(double *)(iVar8 + 0x14);
            in_xd2 = *(double *)(iVar10 + 0x14);
            fVar28 = (float)((ulonglong)dVar25 >> 0x20);
          }
          if (in_FPSCR_PR == '\0') {
            fVar28 = SUB84(dVar24,0) - fVar28;
            param_1 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * fVar28,fVar28);
          }
          else {
            param_1 = (dVar24 - dVar25) * (dVar24 - dVar25);
          }
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),
                                       *(undefined4 *)(iVar17 + 0x14));
          }
          else {
            in_xd2 = *(double *)(iVar17 + 0x14);
          }
          if (in_FPSCR_PR == '\0') {
            dVar24 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                      SUB84(in_dr0,0) - SUB84(param_1,0));
          }
          else {
            dVar24 = in_dr0 - param_1;
          }
          uVar27 = (undefined4)((ulonglong)param_1 >> 0x20);
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44(uVar27,SUB84(dVar25,0));
            dVar25 = (double)CONCAT44(uVar27,*(undefined4 *)(iVar10 + 0x10));
          }
          else {
            in_xd2 = *(double *)(iVar10 + 0x10);
            dVar25 = param_1;
          }
          if (in_FPSCR_PR == '\0') {
            fVar28 = SUB84(dVar25,0) - SUB84(param_1,0);
            dVar25 = (double)CONCAT44(uVar27,fVar28);
            in_dr0 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),SUB84(dVar24,0) * fVar28);
          }
          else {
            dVar25 = dVar25 - param_1;
            in_dr0 = dVar24 * dVar25;
          }
          dVar25 = (double)((ulonglong)dVar25 & 0xffffffff00000000);
          if (in_FPSCR_PR == '\0') {
            fVar28 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
            param_1 = (double)CONCAT44(fVar28,SUB84(param_1,0));
            bVar31 = 0.0 < fVar28;
          }
          else {
            param_1 = param_1 - in_dr0;
            bVar31 = dVar25 < param_1;
          }
          if (bVar31) goto LAB_8c618adc;
          *DAT_8c618ce0 = *DAT_8c618ce0 + uVar19;
          iVar8 = 0;
          if (0 < (int)(uVar19 - 1)) {
            do {
              pdVar2 = DAT_8c618cf4;
              uVar1 = *puVar15;
              puVar15 = puVar15 + 1;
              if (in_FPSCR_SZ == '\0') {
                dVar24 = (double)CONCAT44(*(undefined4 *)DAT_8c618ce4,SUB84(dVar25,0));
              }
              else {
                dVar24 = *DAT_8c618ce4;
              }
              pdVar7 = (double *)((uint)uVar1 * 0x20 + *DAT_8c618cdc);
              if (in_FPSCR_SZ == '\0') {
                dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),*(undefined4 *)pdVar7);
                in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),*DAT_8c618cec);
              }
              else {
                in_xd2 = *pdVar7;
              }
              if (in_FPSCR_PR == '\0') {
                dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar24,0),
                                          SUB84(dVar24,0));
              }
              else {
                dVar24 = dVar24 * dVar24;
              }
              if (in_FPSCR_SZ == '\0') {
                dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                          *(undefined4 *)DAT_8c618ce8);
              }
              else {
                in_xd2 = *DAT_8c618ce8;
              }
              if (in_FPSCR_PR == '\0') {
                fVar28 = SUB84(dVar24,0) - (float)((ulonglong)dVar24 >> 0x20);
              }
              else {
                fVar28 = SUB84(dVar24 - dVar24,0);
              }
              if (in_FPSCR_SZ == '\0') {
                dVar24 = (double)CONCAT44(*(undefined4 *)((int)pdVar7 + 4),fVar28);
              }
              else {
                dVar24 = *(double *)((int)pdVar7 + 4);
              }
              if (in_FPSCR_PR == '\0') {
                dVar25 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                          SUB84(in_dr0,0) * (float)((ulonglong)dVar24 >> 0x20));
              }
              else {
                dVar25 = in_dr0 * dVar24;
              }
              if (in_FPSCR_SZ == '\0') {
                dVar24 = (double)CONCAT44(*(undefined4 *)(pdVar7 + 1),SUB84(dVar24,0));
              }
              else {
                dVar24 = pdVar7[1];
              }
              if (in_FPSCR_PR == '\0') {
                dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                          SUB84(dVar24,0) - SUB84(dVar25,0));
              }
              else {
                dVar24 = dVar24 - dVar25;
              }
              if (in_FPSCR_SZ == '\0') {
                dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*DAT_8c618cf0);
              }
              if (in_FPSCR_PR == '\0') {
                fVar28 = SUB84(dVar25,0) * (float)((ulonglong)dVar24 >> 0x20);
                in_dr0 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),fVar28);
                fVar28 = SUB84(dVar24,0) - fVar28;
              }
              else {
                in_dr0 = dVar25 * dVar24;
                fVar28 = SUB84(dVar24 - in_dr0,0);
              }
              if (in_FPSCR_SZ == '\0') {
                *(float *)((int)pdVar7 + 0x1c) = fVar28;
                dVar24 = (double)CONCAT44(*(undefined4 *)pdVar2,fVar28);
              }
              else {
                *(double *)((int)pdVar7 + 0x1c) = in_xd2;
                dVar24 = *pdVar2;
              }
              pdVar16 = DAT_8c618cfc;
              pdVar5 = DAT_8c618cf8;
              if (in_FPSCR_PR == '\0') {
                dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar24,0),
                                          SUB84(dVar24,0));
              }
              else {
                dVar24 = dVar24 * dVar24;
              }
              if (in_FPSCR_SZ == '\0') {
                pdVar18 = (double *)(unaff_r8 + 7);
                *(uint *)pdVar18 = (uint)((ulonglong)dVar24 >> 0x20);
                dVar24 = (double)CONCAT44(*(undefined4 *)pdVar5,*(undefined4 *)((int)pdVar7 + 0x1c))
                ;
              }
              else {
                pdVar18 = (double *)(unaff_r8 + 6);
                *pdVar18 = dVar24;
                dVar24 = *pdVar5;
                in_xd2 = *(double *)((int)pdVar7 + 0x1c);
              }
              uVar4 = DAT_8c618d00;
              if (in_FPSCR_PR == '\0') {
                dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar24,0),
                                          SUB84(dVar24,0));
              }
              else {
                dVar24 = dVar24 * dVar24;
              }
              if (in_FPSCR_SZ == '\0') {
                pdVar18 = (double *)((int)pdVar18 + -4);
                *(uint *)pdVar18 = (uint)((ulonglong)dVar24 >> 0x20);
                dVar25 = (double)CONCAT44(*(undefined4 *)pdVar16,*(undefined4 *)((int)pdVar7 + 0x1c)
                                         );
              }
              else {
                pdVar18 = pdVar18 + -1;
                *pdVar18 = dVar24;
                dVar25 = *pdVar16;
                in_xd2 = *(double *)((int)pdVar7 + 0x1c);
              }
              if (in_FPSCR_PR == '\0') {
                dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * SUB84(dVar25,0),
                                          SUB84(dVar25,0));
              }
              else {
                dVar25 = dVar25 * dVar25;
              }
              if (in_FPSCR_SZ == '\0') {
                *(uint *)((int)pdVar18 + -4) = (uint)((ulonglong)dVar25 >> 0x20);
                *(uint *)(pdVar18 + -1) = *(uint *)((int)pdVar2 + -0xc);
                *(uint *)((int)pdVar18 + -0xc) = *(uint *)(pdVar7 + 3);
                *(uint *)(pdVar18 + -2) = *(uint *)((int)pdVar7 + 0x14);
                dVar25 = (double)(ulonglong)*(uint *)(pdVar7 + 2);
                pdVar18 = (double *)((int)pdVar18 + -0x14);
                *(uint *)pdVar18 = *(uint *)(pdVar7 + 2);
              }
              else {
                pdVar18[-1] = dVar25;
                pdVar18[-2] = *(double *)((int)pdVar2 + -0xc);
                pdVar18[-3] = pdVar7[3];
                pdVar18[-4] = *(double *)((int)pdVar7 + 0x14);
                in_xd2 = pdVar7[2];
                pdVar18 = pdVar18 + -5;
                *pdVar18 = in_xd2;
              }
              *(uint *)((int)pdVar18 + -4) = uVar4;
              iVar8 = iVar8 + 1;
              unaff_r8 = (uint *)((int)pdVar18 + 0x1c);
            } while (iVar8 < (int)(uVar19 - 1));
          }
          pdVar2 = DAT_8c618cf4;
          puVar13 = puVar15 + 1;
          if (in_FPSCR_SZ == '\0') {
            dVar24 = (double)CONCAT44(*(undefined4 *)DAT_8c618ce4,SUB84(dVar25,0));
          }
          else {
            dVar24 = *DAT_8c618ce4;
          }
          pdVar7 = (double *)((uint)*puVar15 * 0x20 + *DAT_8c618cdc);
          if (in_FPSCR_SZ == '\0') {
            dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),*(undefined4 *)pdVar7);
            in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),*DAT_8c618cec);
          }
          else {
            in_xd2 = *pdVar7;
          }
          if (in_FPSCR_PR == '\0') {
            dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar24,0),
                                      SUB84(dVar24,0));
          }
          else {
            dVar24 = dVar24 * dVar24;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),*(undefined4 *)DAT_8c618ce8);
          }
          else {
            in_xd2 = *DAT_8c618ce8;
          }
          if (in_FPSCR_PR == '\0') {
            fVar28 = SUB84(dVar24,0) - (float)((ulonglong)dVar24 >> 0x20);
          }
          else {
            fVar28 = SUB84(dVar24 - dVar24,0);
          }
          if (in_FPSCR_SZ == '\0') {
            dVar24 = (double)CONCAT44(*(undefined4 *)((int)pdVar7 + 4),fVar28);
          }
          else {
            dVar24 = *(double *)((int)pdVar7 + 4);
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                      SUB84(in_dr0,0) * (float)((ulonglong)dVar24 >> 0x20));
          }
          else {
            dVar25 = in_dr0 * dVar24;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar24 = (double)CONCAT44(*(undefined4 *)(pdVar7 + 1),SUB84(dVar24,0));
          }
          else {
            dVar24 = pdVar7[1];
          }
          if (in_FPSCR_PR == '\0') {
            dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                      SUB84(dVar24,0) - SUB84(dVar25,0));
          }
          else {
            dVar24 = dVar24 - dVar25;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*DAT_8c618cf0);
          }
          if (in_FPSCR_PR == '\0') {
            fVar28 = SUB84(dVar25,0) * (float)((ulonglong)dVar24 >> 0x20);
            in_dr0 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),fVar28);
            fVar28 = SUB84(dVar24,0) - fVar28;
          }
          else {
            in_dr0 = dVar25 * dVar24;
            fVar28 = SUB84(dVar24 - in_dr0,0);
          }
          if (in_FPSCR_SZ == '\0') {
            *(float *)((int)pdVar7 + 0x1c) = fVar28;
            dVar24 = (double)CONCAT44(*(undefined4 *)pdVar2,fVar28);
          }
          else {
            *(double *)((int)pdVar7 + 0x1c) = in_xd2;
            dVar24 = *pdVar2;
          }
          pdVar16 = DAT_8c618cfc;
          pdVar5 = DAT_8c618cf8;
          if (in_FPSCR_PR == '\0') {
            dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar24,0),
                                      SUB84(dVar24,0));
          }
          else {
            dVar24 = dVar24 * dVar24;
          }
          if (in_FPSCR_SZ == '\0') {
            pdVar18 = (double *)(unaff_r8 + 7);
            *(uint *)pdVar18 = (uint)((ulonglong)dVar24 >> 0x20);
            dVar24 = (double)CONCAT44(*(undefined4 *)pdVar5,*(undefined4 *)((int)pdVar7 + 0x1c));
          }
          else {
            pdVar18 = (double *)(unaff_r8 + 6);
            *pdVar18 = dVar24;
            dVar24 = *pdVar5;
            in_xd2 = *(double *)((int)pdVar7 + 0x1c);
          }
          uVar4 = DAT_8c618d04;
          if (in_FPSCR_PR == '\0') {
            dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar24,0),
                                      SUB84(dVar24,0));
          }
          else {
            dVar24 = dVar24 * dVar24;
          }
          if (in_FPSCR_SZ == '\0') {
            pdVar18 = (double *)((int)pdVar18 + -4);
            *(uint *)pdVar18 = (uint)((ulonglong)dVar24 >> 0x20);
            dVar24 = (double)CONCAT44(*(undefined4 *)pdVar16,*(undefined4 *)((int)pdVar7 + 0x1c));
          }
          else {
            pdVar18 = pdVar18 + -1;
            *pdVar18 = dVar24;
            dVar24 = *pdVar16;
            in_xd2 = *(double *)((int)pdVar7 + 0x1c);
          }
          if (in_FPSCR_PR == '\0') {
            dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar24,0),
                                      SUB84(dVar24,0));
          }
          else {
            dVar24 = dVar24 * dVar24;
          }
          if (in_FPSCR_SZ == '\0') {
            *(uint *)((int)pdVar18 + -4) = (uint)((ulonglong)dVar24 >> 0x20);
            *(uint *)(pdVar18 + -1) = *(uint *)((int)pdVar2 + -0xc);
            *(uint *)((int)pdVar18 + -0xc) = *(uint *)(pdVar7 + 3);
            *(uint *)(pdVar18 + -2) = *(uint *)((int)pdVar7 + 0x14);
            pdVar18 = (double *)((int)pdVar18 + -0x14);
            *(uint *)pdVar18 = *(uint *)(pdVar7 + 2);
          }
          else {
            pdVar18[-1] = dVar24;
            pdVar18[-2] = *(double *)((int)pdVar2 + -0xc);
            pdVar18[-3] = pdVar7[3];
            pdVar18[-4] = *(double *)((int)pdVar7 + 0x14);
            in_xd2 = pdVar7[2];
            pdVar18 = pdVar18 + -5;
            *pdVar18 = in_xd2;
          }
          *(uint *)((int)pdVar18 + -4) = uVar4;
          unaff_r8 = (uint *)((int)pdVar18 + 0x1c);
        }
        else {
          *DAT_8c618d08 = 0;
          puVar6 = puVar13 + 2;
          puVar13 = puVar13 + 3;
          iVar17 = 0;
          iVar8 = (uint)*puVar15 * 0x20 + *piVar3;
          iVar10 = (uint)*puVar6 * 0x20 + *piVar3;
          if (0 < (int)(uVar19 - 2)) {
            do {
              uVar1 = *puVar13;
              puVar13 = puVar13 + 1;
              iVar20 = (uint)uVar1 * 0x20 + *DAT_8c618cdc;
              if ((*(uint *)(iVar8 + 0xc) & *(uint *)(iVar10 + 0xc) & *(uint *)(iVar20 + 0xc) & 1)
                  == 0) {
                in_dr0 = (double)(*(code *)PTR_FUN_8c618d0c)(iVar10,iVar20);
              }
              iVar17 = iVar17 + 1;
              iVar8 = iVar10;
              iVar10 = iVar20;
            } while (iVar17 < (int)(uVar19 - 2));
          }
        }
      }
      else {
LAB_8c618adc:
        puVar13 = puVar15 + uVar19;
      }
    }
  }
  else if (uVar4 == DAT_8c618554) {
    *DAT_8c618d10 = param_3;
    if (in_FPSCR_SZ == '\0') {
      uVar26 = CONCAT44(*(undefined4 *)(DAT_8c618ccc + 0x10),*(undefined4 *)(DAT_8c618ccc + 0x14));
      uVar29 = CONCAT44(*(undefined4 *)(DAT_8c618ccc + 0x18),*(undefined4 *)(DAT_8c618ccc + 0x1c));
      uVar30 = CONCAT44(*(undefined4 *)DAT_8c618cd0,param_2);
    }
    else {
      uVar26 = *(undefined8 *)(DAT_8c618ccc + 0x10);
      uVar29 = *(undefined8 *)(DAT_8c618ccc + 0x20);
      uVar30 = *DAT_8c618cd0;
    }
    (*(code *)PTR_FUN_8c618d14)(uVar26,uVar29,uVar30,*DAT_8c618d10);
  }
  return;
}

