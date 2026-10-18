/* Ghidra draft only; not buildable/verified source. */

void FUN_8c61b488(double param_1,undefined4 param_2,ushort *param_3)

{
  ushort uVar1;
  undefined *puVar2;
  uint uVar3;
  int iVar4;
  ushort *puVar5;
  int iVar6;
  uint uVar7;
  undefined8 *unaff_r8;
  double *pdVar8;
  int iVar9;
  uint uVar10;
  ushort *puVar11;
  ushort *puVar12;
  ushort *puVar13;
  double *pdVar14;
  uint uVar15;
  double *unaff_r12;
  double *unaff_r13;
  double *pdVar16;
  double *unaff_r14;
  double *pdVar17;
  double in_dr0;
  double dVar18;
  undefined4 uVar19;
  float fVar20;
  float fVar21;
  double dVar22;
  double dVar23;
  undefined4 uVar24;
  undefined4 uVar26;
  undefined8 uVar25;
  undefined8 uVar27;
  undefined8 uVar28;
  double in_xd2;
  double in_xd4;
  bool bVar29;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  int local_40;
  undefined4 uStack_38;
  undefined4 uStack_34;
  float fStack_30;
  float fStack_2c;
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined4 local_1c;
  
  uVar3 = *param_3 & DAT_8c61b4ec;
  puVar11 = *(ushort **)(param_3 + 2);
  local_1c = in_PR;
  if (uVar3 == 0) {
    *DAT_8c61b4f4 = *DAT_8c61b4f4 + (uint)param_3[1] * 3;
    for (iVar9 = 0; uVar24 = SUB84(param_1,0), iVar9 < (int)(uint)param_3[1]; iVar9 = iVar9 + 1) {
      uVar1 = *puVar11;
      puVar5 = puVar11 + 1;
      puVar12 = puVar11 + 2;
      iVar6 = *DAT_8c61b734;
      puVar11 = puVar11 + 3;
      pdVar16 = (double *)((uint)uVar1 * 0x20 + iVar6);
      pdVar14 = (double *)((uint)*puVar5 * 0x20 + iVar6);
      pdVar17 = (double *)((uint)*puVar12 * 0x20 + iVar6);
      if ((*(uint *)((int)pdVar16 + 0xc) & *(uint *)((int)pdVar14 + 0xc) &
           *(uint *)((int)pdVar17 + 0xc) & 0x3f) == 0) {
        if (((*(uint *)((int)pdVar16 + 0xc) | *(uint *)((int)pdVar14 + 0xc) |
             *(uint *)((int)pdVar17 + 0xc)) & 1) == 0) {
          if (in_FPSCR_SZ == '\0') {
            uVar26 = *(undefined4 *)(pdVar14 + 2);
            dVar23 = (double)CONCAT44(uVar26,*(undefined4 *)(pdVar16 + 2));
            dVar22 = (double)CONCAT44(uVar26,uVar24);
          }
          else {
            dVar23 = pdVar14[2];
            in_xd2 = pdVar16[2];
            uVar26 = (undefined4)((ulonglong)dVar23 >> 0x20);
            dVar22 = dVar23;
          }
          if (in_FPSCR_PR == '\0') {
            dVar23 = (double)CONCAT44((float)((ulonglong)dVar23 >> 0x20) - SUB84(dVar23,0),
                                      SUB84(dVar23,0));
          }
          else {
            dVar23 = dVar23 - dVar23;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),
                                      *(undefined4 *)((int)pdVar17 + 0x14));
            dVar22 = (double)CONCAT44(uVar26,*(undefined4 *)((int)pdVar14 + 0x14));
          }
          else {
            in_xd2 = *(double *)((int)pdVar17 + 0x14);
          }
          fVar20 = SUB84(dVar22,0);
          if (in_FPSCR_PR == '\0') {
            dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),SUB84(dVar23,0) - fVar20);
          }
          else {
            dVar23 = dVar23 - dVar22;
          }
          if (in_FPSCR_SZ == '\0') {
            in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar20);
          }
          if (in_FPSCR_PR == '\0') {
            dVar23 = (double)CONCAT44((float)((ulonglong)dVar23 >> 0x20) * SUB84(dVar23,0),
                                      SUB84(dVar23,0));
          }
          else {
            dVar23 = dVar23 * dVar23;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),
                                      *(undefined4 *)((int)pdVar16 + 0x14));
          }
          else {
            in_xd2 = *(double *)((int)pdVar16 + 0x14);
          }
          if (in_FPSCR_PR == '\0') {
            dVar18 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                      SUB84(in_dr0,0) - SUB84(dVar23,0));
          }
          else {
            dVar18 = in_dr0 - dVar23;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(undefined4 *)(pdVar17 + 2))
            ;
          }
          else {
            in_xd2 = pdVar17[2];
          }
          if (in_FPSCR_PR == '\0') {
            dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),
                                      SUB84(dVar23,0) - (float)((ulonglong)dVar22 >> 0x20));
          }
          else {
            dVar23 = dVar23 - dVar22;
          }
          uVar3 = (uint)((ulonglong)dVar23 >> 0x20);
          param_1 = dVar23;
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44(uVar3,fVar20);
          }
          if (in_FPSCR_PR == '\0') {
            in_dr0 = (double)CONCAT44((int)((ulonglong)dVar18 >> 0x20),
                                      SUB84(dVar18,0) * SUB84(dVar23,0));
          }
          else {
            in_dr0 = dVar18 * dVar23;
          }
          if (in_FPSCR_PR == '\0') {
            fVar20 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
            param_1 = (double)CONCAT44(fVar20,SUB84(param_1,0));
            bVar29 = 0.0 < fVar20;
          }
          else {
            param_1 = param_1 - in_dr0;
            bVar29 = (double)((ulonglong)uVar3 << 0x20) < param_1;
          }
          uVar24 = SUB84(param_1,0);
          if (!bVar29) {
            if (in_FPSCR_SZ == '\0') {
              dVar22 = (double)CONCAT44(*(undefined4 *)pdVar16,*(undefined4 *)pdVar14);
            }
            else {
              dVar22 = *pdVar16;
              in_xd2 = *pdVar14;
            }
            if (in_FPSCR_PR == '\0') {
              dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                        SUB84(dVar22,0));
            }
            else {
              dVar22 = dVar22 + dVar22;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)pdVar17);
            }
            else {
              in_xd2 = *pdVar17;
            }
            if (in_FPSCR_PR == '\0') {
              dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                        SUB84(dVar22,0));
            }
            else {
              dVar22 = dVar22 + dVar22;
            }
            if (in_FPSCR_SZ == '\0') {
              uStack_28 = (undefined4)((ulonglong)dVar22 >> 0x20);
              dVar22 = (double)CONCAT44(*(undefined4 *)((int)pdVar16 + 4),
                                        *(undefined4 *)((int)pdVar14 + 4));
            }
            else {
              uStack_28 = SUB84(dVar22,0);
              dVar22 = *(double *)((int)pdVar16 + 4);
              in_xd2 = *(double *)((int)pdVar14 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                        SUB84(dVar22,0));
            }
            else {
              dVar22 = dVar22 + dVar22;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                        *(undefined4 *)((int)pdVar17 + 4));
            }
            else {
              in_xd2 = *(double *)((int)pdVar17 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                        SUB84(dVar22,0));
            }
            else {
              dVar22 = dVar22 + dVar22;
            }
            if (in_FPSCR_SZ == '\0') {
              uStack_24 = (undefined4)((ulonglong)dVar22 >> 0x20);
              dVar22 = (double)CONCAT44(*(undefined4 *)(pdVar16 + 1),*(undefined4 *)(pdVar14 + 1));
            }
            else {
              uStack_24 = SUB84(dVar22,0);
              dVar22 = pdVar16[1];
              in_xd2 = pdVar14[1];
            }
            if (in_FPSCR_PR == '\0') {
              dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                        SUB84(dVar22,0));
            }
            else {
              dVar22 = dVar22 + dVar22;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                        *(undefined4 *)(pdVar17 + 1));
            }
            else {
              in_xd2 = pdVar17[1];
            }
            if (in_FPSCR_PR == '\0') {
              dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                        SUB84(dVar22,0));
            }
            else {
              dVar22 = dVar22 + dVar22;
            }
            uVar26 = (undefined4)((ulonglong)dVar22 >> 0x20);
            uStack_20 = uVar26;
            if (in_FPSCR_SZ != '\0') {
              uStack_20 = SUB84(dVar22,0);
              local_1c = uVar26;
            }
            dVar22 = (double)(*(code *)PTR_FUN_8c61b738)(&uStack_28);
            uVar26 = DAT_8c61b754;
            if (in_FPSCR_SZ == '\0') {
              dVar23 = (double)CONCAT44(*(undefined4 *)DAT_8c61b73c,uStack_28);
            }
            else {
              in_xd2 = (double)CONCAT44(uStack_24,uStack_28);
              dVar23 = *DAT_8c61b73c;
            }
            if (in_FPSCR_PR == '\0') {
              dVar23 = (double)CONCAT44((float)((ulonglong)dVar23 >> 0x20) * SUB84(dVar23,0),
                                        SUB84(dVar23,0));
            }
            else {
              dVar23 = dVar23 * dVar23;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(undefined4 *)DAT_8c61b740
                                       );
              dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*DAT_8c61b744);
            }
            else {
              in_xd2 = *DAT_8c61b740;
            }
            if (in_FPSCR_PR == '\0') {
              fVar20 = SUB84(dVar23,0) - (float)((ulonglong)dVar23 >> 0x20);
            }
            else {
              fVar20 = SUB84(dVar23 - dVar23,0);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar23 = (double)CONCAT44(uStack_24,fVar20);
            }
            else {
              dVar23 = (double)CONCAT44(uStack_20,uStack_24);
            }
            if (in_FPSCR_PR == '\0') {
              dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                        SUB84(dVar22,0) * (float)((ulonglong)dVar23 >> 0x20));
            }
            else {
              dVar22 = dVar22 * dVar23;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar23 = (double)CONCAT44(uStack_20,SUB84(dVar23,0));
            }
            else {
              dVar23 = (double)CONCAT44(local_1c,uStack_20);
            }
            if (in_FPSCR_PR == '\0') {
              dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),
                                        SUB84(dVar23,0) - SUB84(dVar22,0));
            }
            else {
              dVar23 = dVar23 - dVar22;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                        *(undefined4 *)(DAT_8c61b73c + 1));
            }
            fVar20 = (float)((ulonglong)dVar23 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              in_dr0 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),SUB84(dVar22,0) * fVar20);
            }
            else {
              in_dr0 = dVar22 * dVar23;
            }
            if (in_FPSCR_SZ == '\0') {
              uVar19 = SUB84(dVar23,0);
              dVar23 = (double)CONCAT44(fVar20,*(undefined4 *)DAT_8c61b748);
              dVar22 = in_xd2;
              in_xd2 = (double)CONCAT44(uVar19,uVar24);
            }
            else {
              dVar22 = *DAT_8c61b748;
            }
            if (in_FPSCR_PR == '\0') {
              param_1 = (double)CONCAT44((float)((ulonglong)in_xd2 >> 0x20) - SUB84(in_dr0,0),
                                         SUB84(in_xd2,0));
            }
            else {
              param_1 = in_xd2 - in_dr0;
            }
            if (in_FPSCR_SZ == '\0') {
              uStack_38 = SUB84(dVar23,0);
              dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(undefined4 *)DAT_8c61b74c
                                       );
            }
            else {
              uStack_38 = SUB84(dVar22,0);
              dVar22 = *DAT_8c61b74c;
            }
            fStack_2c = (float)((ulonglong)param_1 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),SUB84(dVar23,0) * fStack_2c
                                       );
            }
            else {
              dVar23 = dVar23 * param_1;
            }
            if (in_FPSCR_SZ == '\0') {
              uStack_34 = SUB84(dVar23,0);
              dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(undefined4 *)DAT_8c61b750
                                       );
            }
            else {
              uStack_34 = SUB84(dVar22,0);
              dVar22 = *DAT_8c61b750;
            }
            if (in_FPSCR_PR == '\0') {
              dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),SUB84(dVar23,0) * fStack_2c
                                       );
            }
            else {
              dVar23 = dVar23 * param_1;
            }
            if (in_FPSCR_SZ == '\0') {
              fStack_30 = SUB84(dVar23,0);
              dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),
                                        *(undefined4 *)((int)DAT_8c61b748 + 0xc));
            }
            else {
              fStack_30 = SUB84(dVar22,0);
              dVar22 = *(double *)((int)DAT_8c61b748 + 0xc);
            }
            if (in_FPSCR_PR == '\0') {
              fStack_2c = SUB84(dVar23,0) * fStack_2c;
            }
            else {
              fStack_2c = SUB84(dVar23 * param_1,0);
            }
            if (in_FPSCR_SZ == '\0') {
              *(undefined4 *)((int)unaff_r8 + 0xc) = *(undefined4 *)(pdVar16 + 3);
              *(undefined4 *)(unaff_r8 + 1) = *(undefined4 *)((int)pdVar16 + 0x14);
              pdVar8 = (double *)((int)unaff_r8 + 4);
              *(undefined4 *)pdVar8 = *(undefined4 *)(pdVar16 + 2);
            }
            else {
              fStack_2c = SUB84(dVar22,0);
              uStack_28 = (undefined4)((ulonglong)dVar22 >> 0x20);
              unaff_r8[1] = pdVar16[3];
              *unaff_r8 = *(undefined8 *)((int)pdVar16 + 0x14);
              dVar22 = pdVar16[2];
              pdVar8 = (double *)(unaff_r8 + -1);
              *pdVar8 = dVar22;
            }
            *(undefined4 *)((int)pdVar8 + -4) = uVar26;
            if (in_FPSCR_SZ == '\0') {
              *(undefined4 *)(pdVar8 + 5) = *(undefined4 *)(pdVar14 + 3);
              *(undefined4 *)((int)pdVar8 + 0x24) = *(undefined4 *)((int)pdVar14 + 0x14);
              pdVar8 = pdVar8 + 4;
              *(undefined4 *)pdVar8 = *(undefined4 *)(pdVar14 + 2);
            }
            else {
              *(double *)((int)pdVar8 + 0x24) = pdVar14[3];
              *(undefined8 *)((int)pdVar8 + 0x1c) = *(undefined8 *)((int)pdVar14 + 0x14);
              dVar22 = pdVar14[2];
              pdVar8 = (double *)((int)pdVar8 + 0x14);
              *pdVar8 = dVar22;
            }
            *(undefined4 *)((int)pdVar8 + -4) = uVar26;
            uVar24 = DAT_8c61b758;
            if (in_FPSCR_SZ == '\0') {
              *(float *)(pdVar8 + 7) = fStack_2c;
              *(float *)((int)pdVar8 + 0x34) = fStack_30;
              *(undefined4 *)(pdVar8 + 6) = uStack_34;
              *(undefined4 *)((int)pdVar8 + 0x2c) = uStack_38;
              *(undefined4 *)(pdVar8 + 5) = *(undefined4 *)(pdVar17 + 3);
              *(undefined4 *)((int)pdVar8 + 0x24) = *(undefined4 *)((int)pdVar17 + 0x14);
              pdVar8 = pdVar8 + 4;
              *(undefined4 *)pdVar8 = *(undefined4 *)(pdVar17 + 2);
              in_xd2 = dVar22;
            }
            else {
              *(ulonglong *)((int)pdVar8 + 0x34) = CONCAT44(uStack_28,fStack_2c);
              *(ulonglong *)((int)pdVar8 + 0x2c) = CONCAT44(fStack_2c,fStack_30);
              *(ulonglong *)((int)pdVar8 + 0x24) = CONCAT44(fStack_30,uStack_34);
              *(ulonglong *)((int)pdVar8 + 0x1c) = CONCAT44(uStack_34,uStack_38);
              *(double *)((int)pdVar8 + 0x14) = pdVar17[3];
              *(undefined8 *)((int)pdVar8 + 0xc) = *(undefined8 *)((int)pdVar17 + 0x14);
              in_xd2 = pdVar17[2];
              pdVar8 = (double *)((int)pdVar8 + 4);
              *pdVar8 = in_xd2;
            }
            *(undefined4 *)((int)pdVar8 + -4) = uVar24;
            unaff_r8 = (undefined8 *)((int)pdVar8 + 0x1c);
            *DAT_8c61b75c = *DAT_8c61b75c + 3;
          }
        }
        else {
          *DAT_8c61b760 = 1;
          if (in_FPSCR_SZ == '\0') {
            dVar22 = (double)CONCAT44(*(undefined4 *)pdVar16,*(undefined4 *)pdVar14);
          }
          else {
            in_xd2 = *pdVar14;
            dVar22 = *pdVar16;
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                      SUB84(dVar22,0));
          }
          else {
            dVar22 = dVar22 + dVar22;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)pdVar17);
          }
          else {
            in_xd2 = *pdVar17;
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                      SUB84(dVar22,0));
          }
          else {
            dVar22 = dVar22 + dVar22;
          }
          if (in_FPSCR_SZ == '\0') {
            uStack_28 = (undefined4)((ulonglong)dVar22 >> 0x20);
            dVar22 = (double)CONCAT44(*(undefined4 *)((int)pdVar16 + 4),
                                      *(undefined4 *)((int)pdVar14 + 4));
          }
          else {
            uStack_28 = SUB84(dVar22,0);
            in_xd2 = *(double *)((int)pdVar14 + 4);
            dVar22 = *(double *)((int)pdVar16 + 4);
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                      SUB84(dVar22,0));
          }
          else {
            dVar22 = dVar22 + dVar22;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                      *(undefined4 *)((int)pdVar17 + 4));
          }
          else {
            in_xd2 = *(double *)((int)pdVar17 + 4);
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                      SUB84(dVar22,0));
          }
          else {
            dVar22 = dVar22 + dVar22;
          }
          if (in_FPSCR_SZ == '\0') {
            uStack_24 = (undefined4)((ulonglong)dVar22 >> 0x20);
            dVar22 = (double)CONCAT44(*(undefined4 *)(pdVar16 + 1),*(undefined4 *)(pdVar14 + 1));
          }
          else {
            uStack_24 = SUB84(dVar22,0);
            dVar22 = pdVar16[1];
            in_xd2 = pdVar14[1];
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                      SUB84(dVar22,0));
          }
          else {
            dVar22 = dVar22 + dVar22;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)(pdVar17 + 1))
            ;
          }
          else {
            in_xd2 = pdVar17[1];
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                      SUB84(dVar22,0));
          }
          else {
            dVar22 = dVar22 + dVar22;
          }
          uVar26 = (undefined4)((ulonglong)dVar22 >> 0x20);
          uStack_20 = uVar26;
          if (in_FPSCR_SZ != '\0') {
            uStack_20 = SUB84(dVar22,0);
            local_1c = uVar26;
          }
          dVar22 = (double)(*(code *)PTR_FUN_8c61b738)(&uStack_28);
          if (in_FPSCR_SZ == '\0') {
            dVar23 = (double)CONCAT44(*(undefined4 *)DAT_8c61b73c,uStack_28);
          }
          else {
            in_xd2 = (double)CONCAT44(uStack_24,uStack_28);
            dVar23 = *DAT_8c61b73c;
          }
          if (in_FPSCR_PR == '\0') {
            dVar23 = (double)CONCAT44((float)((ulonglong)dVar23 >> 0x20) * SUB84(dVar23,0),
                                      SUB84(dVar23,0));
          }
          else {
            dVar23 = dVar23 * dVar23;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(undefined4 *)DAT_8c61b740);
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*DAT_8c61b744);
            param_1 = in_xd2;
          }
          else {
            param_1 = *DAT_8c61b740;
          }
          if (in_FPSCR_PR == '\0') {
            fVar20 = SUB84(dVar23,0) - (float)((ulonglong)dVar23 >> 0x20);
          }
          else {
            fVar20 = SUB84(dVar23 - dVar23,0);
          }
          if (in_FPSCR_SZ == '\0') {
            dVar23 = (double)CONCAT44(uStack_24,fVar20);
          }
          else {
            dVar23 = (double)CONCAT44(uStack_20,uStack_24);
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                      SUB84(dVar22,0) * (float)((ulonglong)dVar23 >> 0x20));
          }
          else {
            dVar22 = dVar22 * dVar23;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar23 = (double)CONCAT44(uStack_20,SUB84(dVar23,0));
          }
          else {
            dVar23 = (double)CONCAT44(local_1c,uStack_20);
          }
          if (in_FPSCR_PR == '\0') {
            dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),
                                      SUB84(dVar23,0) - SUB84(dVar22,0));
          }
          else {
            dVar23 = dVar23 - dVar22;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                      *(undefined4 *)(DAT_8c61b73c + 1));
          }
          fVar20 = (float)((ulonglong)dVar23 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),SUB84(dVar22,0) * fVar20);
          }
          else {
            dVar22 = dVar22 * dVar23;
          }
          if (in_FPSCR_SZ == '\0') {
            uVar26 = SUB84(dVar23,0);
            dVar23 = (double)CONCAT44(fVar20,*(undefined4 *)DAT_8c61b748);
            in_xd2 = param_1;
            param_1 = (double)CONCAT44(uVar26,uVar24);
          }
          else {
            in_xd2 = *DAT_8c61b748;
          }
          if (in_FPSCR_PR == '\0') {
            param_1 = (double)CONCAT44((float)((ulonglong)param_1 >> 0x20) - SUB84(dVar22,0),
                                       SUB84(param_1,0));
          }
          else {
            param_1 = param_1 - dVar22;
          }
          if (in_FPSCR_SZ == '\0') {
            uStack_38 = SUB84(dVar23,0);
            dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(undefined4 *)DAT_8c61b74c);
          }
          else {
            uStack_38 = SUB84(in_xd2,0);
            in_xd2 = *DAT_8c61b74c;
          }
          fStack_2c = (float)((ulonglong)param_1 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),SUB84(dVar23,0) * fStack_2c);
          }
          else {
            dVar23 = dVar23 * param_1;
          }
          if (in_FPSCR_SZ == '\0') {
            uStack_34 = SUB84(dVar23,0);
            dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(undefined4 *)DAT_8c61b750);
          }
          else {
            uStack_34 = SUB84(in_xd2,0);
            in_xd2 = *DAT_8c61b750;
          }
          if (in_FPSCR_PR == '\0') {
            dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),SUB84(dVar23,0) * fStack_2c);
          }
          else {
            dVar23 = dVar23 * param_1;
          }
          if (in_FPSCR_SZ == '\0') {
            fStack_30 = SUB84(dVar23,0);
            dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),
                                      *(undefined4 *)((int)DAT_8c61b748 + 0xc));
          }
          else {
            fStack_30 = SUB84(in_xd2,0);
            in_xd2 = *(double *)((int)DAT_8c61b748 + 0xc);
          }
          if (in_FPSCR_PR == '\0') {
            fStack_2c = SUB84(dVar23,0) * fStack_2c;
          }
          else {
            fStack_2c = SUB84(dVar23 * param_1,0);
          }
          if (in_FPSCR_SZ != '\0') {
            fStack_2c = SUB84(in_xd2,0);
            uStack_28 = (undefined4)((ulonglong)in_xd2 >> 0x20);
          }
          in_dr0 = (double)(*(code *)PTR_FUN_8c61b764)(pdVar16,pdVar14,pdVar17,&uStack_38);
        }
      }
    }
  }
  else if (uVar3 == (int)DAT_8c61b4ea) {
    *DAT_8c61b768 = *DAT_8c61b768 + (uint)param_3[1] * 4;
    for (iVar9 = 0; puVar2 = PTR_FUN_8c61ba10, uVar24 = SUB84(param_1,0),
        iVar9 < (int)(uint)param_3[1]; iVar9 = iVar9 + 1) {
      uVar1 = *puVar11;
      puVar5 = puVar11 + 1;
      puVar12 = puVar11 + 2;
      iVar6 = *DAT_8c61ba0c;
      puVar13 = puVar11 + 3;
      puVar11 = puVar11 + 4;
      pdVar14 = (double *)((uint)uVar1 * 0x20 + iVar6);
      pdVar16 = (double *)((uint)*puVar5 * 0x20 + iVar6);
      pdVar17 = (double *)((uint)*puVar12 * 0x20 + iVar6);
      iVar6 = (uint)*puVar13 * 0x20 + iVar6;
      if ((*(uint *)((int)pdVar14 + 0xc) & *(uint *)((int)pdVar16 + 0xc) &
           *(uint *)((int)pdVar17 + 0xc) & *(uint *)(iVar6 + 0xc) & 0x3f) == 0) {
        if (((*(uint *)((int)pdVar14 + 0xc) | *(uint *)((int)pdVar16 + 0xc) |
              *(uint *)((int)pdVar17 + 0xc) | *(uint *)(iVar6 + 0xc)) & 1) == 0) {
          if (in_FPSCR_SZ == '\0') {
            uVar24 = *(undefined4 *)(pdVar16 + 2);
            dVar22 = (double)CONCAT44(uVar24,*(undefined4 *)(pdVar14 + 2));
          }
          else {
            dVar22 = pdVar16[2];
            in_xd2 = pdVar14[2];
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) - SUB84(dVar22,0),
                                      SUB84(dVar22,0));
          }
          else {
            dVar22 = dVar22 - dVar22;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                      *(undefined4 *)((int)pdVar17 + 0x14));
            dVar23 = (double)CONCAT44(*(undefined4 *)((int)pdVar16 + 0x14),uVar24);
          }
          else {
            in_xd2 = *(double *)((int)pdVar17 + 0x14);
            dVar23 = *(double *)((int)pdVar16 + 0x14);
          }
          fVar20 = (float)((ulonglong)dVar23 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),SUB84(dVar22,0) - fVar20);
          }
          else {
            dVar22 = dVar22 - dVar23;
          }
          if (in_FPSCR_SZ == '\0') {
            in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar20);
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) * SUB84(dVar22,0),
                                      SUB84(dVar22,0));
          }
          else {
            dVar22 = dVar22 * dVar22;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                      *(undefined4 *)((int)pdVar14 + 0x14));
          }
          else {
            in_xd2 = *(double *)((int)pdVar14 + 0x14);
          }
          if (in_FPSCR_PR == '\0') {
            dVar18 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                      SUB84(in_dr0,0) - SUB84(dVar22,0));
          }
          else {
            dVar18 = in_dr0 - dVar22;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)(pdVar17 + 2))
            ;
          }
          else {
            in_xd2 = pdVar17[2];
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                      SUB84(dVar22,0) - SUB84(dVar23,0));
          }
          else {
            dVar22 = dVar22 - dVar23;
          }
          uVar3 = (uint)((ulonglong)dVar22 >> 0x20);
          param_1 = dVar22;
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44(uVar3,SUB84(dVar23,0));
          }
          if (in_FPSCR_PR == '\0') {
            in_dr0 = (double)CONCAT44((int)((ulonglong)dVar18 >> 0x20),
                                      SUB84(dVar18,0) * SUB84(dVar22,0));
          }
          else {
            in_dr0 = dVar18 * dVar22;
          }
          if (in_FPSCR_PR == '\0') {
            fVar20 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
            param_1 = (double)CONCAT44(fVar20,SUB84(param_1,0));
            bVar29 = 0.0 < fVar20;
          }
          else {
            param_1 = param_1 - in_dr0;
            bVar29 = (double)((ulonglong)uVar3 << 0x20) < param_1;
          }
          uVar24 = SUB84(param_1,0);
          if (!bVar29) {
            if (in_FPSCR_SZ == '\0') {
              dVar22 = (double)CONCAT44(*(undefined4 *)pdVar14,*(undefined4 *)pdVar16);
            }
            else {
              dVar22 = *pdVar14;
              in_xd2 = *pdVar16;
            }
            if (in_FPSCR_PR == '\0') {
              dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                        SUB84(dVar22,0));
            }
            else {
              dVar22 = dVar22 + dVar22;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)pdVar17);
            }
            else {
              in_xd2 = *pdVar17;
            }
            if (in_FPSCR_PR == '\0') {
              dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                        SUB84(dVar22,0));
            }
            else {
              dVar22 = dVar22 + dVar22;
            }
            if (in_FPSCR_SZ == '\0') {
              uStack_28 = (undefined4)((ulonglong)dVar22 >> 0x20);
              dVar22 = (double)CONCAT44(*(undefined4 *)((int)pdVar14 + 4),
                                        *(undefined4 *)((int)pdVar16 + 4));
            }
            else {
              uStack_28 = SUB84(dVar22,0);
              dVar22 = *(double *)((int)pdVar14 + 4);
              in_xd2 = *(double *)((int)pdVar16 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                        SUB84(dVar22,0));
            }
            else {
              dVar22 = dVar22 + dVar22;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                        *(undefined4 *)((int)pdVar17 + 4));
            }
            else {
              in_xd2 = *(double *)((int)pdVar17 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                        SUB84(dVar22,0));
            }
            else {
              dVar22 = dVar22 + dVar22;
            }
            if (in_FPSCR_SZ == '\0') {
              uStack_24 = (undefined4)((ulonglong)dVar22 >> 0x20);
              dVar22 = (double)CONCAT44(*(undefined4 *)(pdVar14 + 1),*(undefined4 *)(pdVar16 + 1));
            }
            else {
              uStack_24 = SUB84(dVar22,0);
              dVar22 = pdVar14[1];
              in_xd2 = pdVar16[1];
            }
            if (in_FPSCR_PR == '\0') {
              dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                        SUB84(dVar22,0));
            }
            else {
              dVar22 = dVar22 + dVar22;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                        *(undefined4 *)(pdVar17 + 1));
            }
            else {
              in_xd2 = pdVar17[1];
            }
            if (in_FPSCR_PR == '\0') {
              dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                        SUB84(dVar22,0));
            }
            else {
              dVar22 = dVar22 + dVar22;
            }
            uVar26 = (undefined4)((ulonglong)dVar22 >> 0x20);
            uStack_20 = uVar26;
            if (in_FPSCR_SZ != '\0') {
              uStack_20 = SUB84(dVar22,0);
              local_1c = uVar26;
            }
            dVar22 = (double)(*(code *)PTR_FUN_8c61ba10)(&uStack_28);
            uVar26 = DAT_8c61ba2c;
            if (in_FPSCR_SZ == '\0') {
              dVar23 = (double)CONCAT44(*(undefined4 *)DAT_8c61ba14,uStack_28);
            }
            else {
              in_xd2 = (double)CONCAT44(uStack_24,uStack_28);
              dVar23 = *DAT_8c61ba14;
            }
            if (in_FPSCR_PR == '\0') {
              dVar23 = (double)CONCAT44((float)((ulonglong)dVar23 >> 0x20) * SUB84(dVar23,0),
                                        SUB84(dVar23,0));
            }
            else {
              dVar23 = dVar23 * dVar23;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(undefined4 *)DAT_8c61ba18
                                       );
              dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*DAT_8c61ba1c);
            }
            else {
              in_xd2 = *DAT_8c61ba18;
            }
            if (in_FPSCR_PR == '\0') {
              fVar20 = SUB84(dVar23,0) - (float)((ulonglong)dVar23 >> 0x20);
            }
            else {
              fVar20 = SUB84(dVar23 - dVar23,0);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar23 = (double)CONCAT44(uStack_24,fVar20);
            }
            else {
              dVar23 = (double)CONCAT44(uStack_20,uStack_24);
            }
            if (in_FPSCR_PR == '\0') {
              dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                        SUB84(dVar22,0) * (float)((ulonglong)dVar23 >> 0x20));
            }
            else {
              dVar22 = dVar22 * dVar23;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar23 = (double)CONCAT44(uStack_20,SUB84(dVar23,0));
            }
            else {
              dVar23 = (double)CONCAT44(local_1c,uStack_20);
            }
            if (in_FPSCR_PR == '\0') {
              dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),
                                        SUB84(dVar23,0) - SUB84(dVar22,0));
            }
            else {
              dVar23 = dVar23 - dVar22;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                        *(undefined4 *)(DAT_8c61ba14 + 1));
            }
            fVar20 = (float)((ulonglong)dVar23 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              in_dr0 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),SUB84(dVar22,0) * fVar20);
            }
            else {
              in_dr0 = dVar22 * dVar23;
            }
            if (in_FPSCR_SZ == '\0') {
              uVar19 = SUB84(dVar23,0);
              dVar23 = (double)CONCAT44(fVar20,*(undefined4 *)DAT_8c61ba20);
              dVar22 = in_xd2;
              in_xd2 = (double)CONCAT44(uVar19,uVar24);
            }
            else {
              dVar22 = *DAT_8c61ba20;
            }
            if (in_FPSCR_PR == '\0') {
              param_1 = (double)CONCAT44((float)((ulonglong)in_xd2 >> 0x20) - SUB84(in_dr0,0),
                                         SUB84(in_xd2,0));
            }
            else {
              param_1 = in_xd2 - in_dr0;
            }
            if (in_FPSCR_SZ == '\0') {
              uStack_38 = SUB84(dVar23,0);
              dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(undefined4 *)DAT_8c61ba24
                                       );
            }
            else {
              uStack_38 = SUB84(dVar22,0);
              dVar22 = *DAT_8c61ba24;
            }
            fStack_2c = (float)((ulonglong)param_1 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),SUB84(dVar23,0) * fStack_2c
                                       );
            }
            else {
              dVar23 = dVar23 * param_1;
            }
            if (in_FPSCR_SZ == '\0') {
              uStack_34 = SUB84(dVar23,0);
              dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(undefined4 *)DAT_8c61ba28
                                       );
            }
            else {
              uStack_34 = SUB84(dVar22,0);
              dVar22 = *DAT_8c61ba28;
            }
            if (in_FPSCR_PR == '\0') {
              dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),SUB84(dVar23,0) * fStack_2c
                                       );
            }
            else {
              dVar23 = dVar23 * param_1;
            }
            if (in_FPSCR_SZ == '\0') {
              fStack_30 = SUB84(dVar23,0);
              dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),
                                        *(undefined4 *)((int)DAT_8c61ba20 + 0xc));
            }
            else {
              fStack_30 = SUB84(dVar22,0);
              dVar22 = *(double *)((int)DAT_8c61ba20 + 0xc);
            }
            if (in_FPSCR_PR == '\0') {
              fStack_2c = SUB84(dVar23,0) * fStack_2c;
            }
            else {
              fStack_2c = SUB84(dVar23 * param_1,0);
            }
            if (in_FPSCR_SZ == '\0') {
              *(undefined4 *)((int)unaff_r8 + 0xc) = *(undefined4 *)(pdVar14 + 3);
              *(undefined4 *)(unaff_r8 + 1) = *(undefined4 *)((int)pdVar14 + 0x14);
              pdVar8 = (double *)((int)unaff_r8 + 4);
              *(undefined4 *)pdVar8 = *(undefined4 *)(pdVar14 + 2);
            }
            else {
              fStack_2c = SUB84(dVar22,0);
              uStack_28 = (undefined4)((ulonglong)dVar22 >> 0x20);
              unaff_r8[1] = pdVar14[3];
              *unaff_r8 = *(undefined8 *)((int)pdVar14 + 0x14);
              dVar22 = pdVar14[2];
              pdVar8 = (double *)(unaff_r8 + -1);
              *pdVar8 = dVar22;
            }
            *(undefined4 *)((int)pdVar8 + -4) = uVar26;
            if (in_FPSCR_SZ == '\0') {
              *(undefined4 *)(pdVar8 + 5) = *(undefined4 *)(pdVar16 + 3);
              *(undefined4 *)((int)pdVar8 + 0x24) = *(undefined4 *)((int)pdVar16 + 0x14);
              pdVar8 = pdVar8 + 4;
              *(undefined4 *)pdVar8 = *(undefined4 *)(pdVar16 + 2);
            }
            else {
              *(double *)((int)pdVar8 + 0x24) = pdVar16[3];
              *(undefined8 *)((int)pdVar8 + 0x1c) = *(undefined8 *)((int)pdVar16 + 0x14);
              dVar22 = pdVar16[2];
              pdVar8 = (double *)((int)pdVar8 + 0x14);
              *pdVar8 = dVar22;
            }
            *(undefined4 *)((int)pdVar8 + -4) = uVar26;
            if (in_FPSCR_SZ == '\0') {
              *(float *)(pdVar8 + 7) = fStack_2c;
              *(float *)((int)pdVar8 + 0x34) = fStack_30;
              *(undefined4 *)(pdVar8 + 6) = uStack_34;
              *(undefined4 *)((int)pdVar8 + 0x2c) = uStack_38;
              *(undefined4 *)(pdVar8 + 5) = *(undefined4 *)(pdVar17 + 3);
              *(undefined4 *)((int)pdVar8 + 0x24) = *(undefined4 *)((int)pdVar17 + 0x14);
              pdVar8 = pdVar8 + 4;
              *(undefined4 *)pdVar8 = *(undefined4 *)(pdVar17 + 2);
            }
            else {
              *(ulonglong *)((int)pdVar8 + 0x34) = CONCAT44(uStack_28,fStack_2c);
              *(ulonglong *)((int)pdVar8 + 0x2c) = CONCAT44(fStack_2c,fStack_30);
              *(ulonglong *)((int)pdVar8 + 0x24) = CONCAT44(fStack_30,uStack_34);
              *(ulonglong *)((int)pdVar8 + 0x1c) = CONCAT44(uStack_34,uStack_38);
              *(double *)((int)pdVar8 + 0x14) = pdVar17[3];
              *(undefined8 *)((int)pdVar8 + 0xc) = *(undefined8 *)((int)pdVar17 + 0x14);
              dVar22 = pdVar17[2];
              pdVar8 = (double *)((int)pdVar8 + 4);
              *pdVar8 = dVar22;
            }
            *(undefined4 *)((int)pdVar8 + -4) = uVar26;
            uVar24 = DAT_8c61ba30;
            if (in_FPSCR_SZ == '\0') {
              *(float *)(pdVar8 + 7) = fStack_2c;
              *(float *)((int)pdVar8 + 0x34) = fStack_30;
              *(undefined4 *)(pdVar8 + 6) = uStack_34;
              *(undefined4 *)((int)pdVar8 + 0x2c) = uStack_38;
              *(undefined4 *)(pdVar8 + 5) = *(undefined4 *)(iVar6 + 0x18);
              *(undefined4 *)((int)pdVar8 + 0x24) = *(undefined4 *)(iVar6 + 0x14);
              pdVar8 = pdVar8 + 4;
              *(undefined4 *)pdVar8 = *(undefined4 *)(iVar6 + 0x10);
              in_xd2 = dVar22;
            }
            else {
              *(ulonglong *)((int)pdVar8 + 0x34) = CONCAT44(uStack_28,fStack_2c);
              *(ulonglong *)((int)pdVar8 + 0x2c) = CONCAT44(fStack_2c,fStack_30);
              *(ulonglong *)((int)pdVar8 + 0x24) = CONCAT44(fStack_30,uStack_34);
              *(ulonglong *)((int)pdVar8 + 0x1c) = CONCAT44(uStack_34,uStack_38);
              *(undefined8 *)((int)pdVar8 + 0x14) = *(undefined8 *)(iVar6 + 0x18);
              *(undefined8 *)((int)pdVar8 + 0xc) = *(undefined8 *)(iVar6 + 0x14);
              in_xd2 = *(double *)(iVar6 + 0x10);
              pdVar8 = (double *)((int)pdVar8 + 4);
              *pdVar8 = in_xd2;
            }
            *(undefined4 *)((int)pdVar8 + -4) = uVar24;
            unaff_r8 = (undefined8 *)((int)pdVar8 + 0x1c);
            *DAT_8c61ba34 = *DAT_8c61ba34 + 4;
          }
        }
        else {
          *DAT_8c61ba38 = 1;
          if (in_FPSCR_SZ == '\0') {
            dVar22 = (double)CONCAT44(*(undefined4 *)pdVar14,*(undefined4 *)pdVar16);
          }
          else {
            in_xd2 = *pdVar16;
            dVar22 = *pdVar14;
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                      SUB84(dVar22,0));
          }
          else {
            dVar22 = dVar22 + dVar22;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)pdVar17);
          }
          else {
            in_xd2 = *pdVar17;
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                      SUB84(dVar22,0));
          }
          else {
            dVar22 = dVar22 + dVar22;
          }
          if (in_FPSCR_SZ == '\0') {
            uStack_28 = (undefined4)((ulonglong)dVar22 >> 0x20);
            dVar22 = (double)CONCAT44(*(undefined4 *)((int)pdVar14 + 4),
                                      *(undefined4 *)((int)pdVar16 + 4));
          }
          else {
            uStack_28 = SUB84(dVar22,0);
            dVar22 = *(double *)((int)pdVar14 + 4);
            in_xd2 = *(double *)((int)pdVar16 + 4);
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                      SUB84(dVar22,0));
          }
          else {
            dVar22 = dVar22 + dVar22;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                      *(undefined4 *)((int)pdVar17 + 4));
          }
          else {
            in_xd2 = *(double *)((int)pdVar17 + 4);
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                      SUB84(dVar22,0));
          }
          else {
            dVar22 = dVar22 + dVar22;
          }
          if (in_FPSCR_SZ == '\0') {
            uStack_24 = (undefined4)((ulonglong)dVar22 >> 0x20);
            dVar22 = (double)CONCAT44(*(undefined4 *)(pdVar14 + 1),*(undefined4 *)(pdVar16 + 1));
          }
          else {
            uStack_24 = SUB84(dVar22,0);
            dVar22 = pdVar14[1];
            in_xd2 = pdVar16[1];
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                      SUB84(dVar22,0));
          }
          else {
            dVar22 = dVar22 + dVar22;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)(pdVar17 + 1))
            ;
          }
          else {
            in_xd2 = pdVar17[1];
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                      SUB84(dVar22,0));
          }
          else {
            dVar22 = dVar22 + dVar22;
          }
          uVar26 = (undefined4)((ulonglong)dVar22 >> 0x20);
          uStack_20 = uVar26;
          if (in_FPSCR_SZ != '\0') {
            uStack_20 = SUB84(dVar22,0);
            local_1c = uVar26;
          }
          dVar22 = (double)(*(code *)puVar2)(&uStack_28);
          if (in_FPSCR_SZ == '\0') {
            dVar23 = (double)CONCAT44(*(undefined4 *)DAT_8c61ba14,uStack_28);
          }
          else {
            in_xd2 = (double)CONCAT44(uStack_24,uStack_28);
            dVar23 = *DAT_8c61ba14;
          }
          if (in_FPSCR_PR == '\0') {
            dVar23 = (double)CONCAT44((float)((ulonglong)dVar23 >> 0x20) * SUB84(dVar23,0),
                                      SUB84(dVar23,0));
          }
          else {
            dVar23 = dVar23 * dVar23;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(undefined4 *)DAT_8c61ba18);
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*DAT_8c61ba1c);
            param_1 = in_xd2;
          }
          else {
            param_1 = *DAT_8c61ba18;
          }
          if (in_FPSCR_PR == '\0') {
            fVar20 = SUB84(dVar23,0) - (float)((ulonglong)dVar23 >> 0x20);
          }
          else {
            fVar20 = SUB84(dVar23 - dVar23,0);
          }
          if (in_FPSCR_SZ == '\0') {
            dVar23 = (double)CONCAT44(uStack_24,fVar20);
          }
          else {
            dVar23 = (double)CONCAT44(uStack_20,uStack_24);
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                      SUB84(dVar22,0) * (float)((ulonglong)dVar23 >> 0x20));
          }
          else {
            dVar22 = dVar22 * dVar23;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar23 = (double)CONCAT44(uStack_20,SUB84(dVar23,0));
          }
          else {
            dVar23 = (double)CONCAT44(local_1c,uStack_20);
          }
          if (in_FPSCR_PR == '\0') {
            dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),
                                      SUB84(dVar23,0) - SUB84(dVar22,0));
          }
          else {
            dVar23 = dVar23 - dVar22;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                      *(undefined4 *)(DAT_8c61ba14 + 1));
          }
          fVar20 = (float)((ulonglong)dVar23 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            in_dr0 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),SUB84(dVar22,0) * fVar20);
          }
          else {
            in_dr0 = dVar22 * dVar23;
          }
          if (in_FPSCR_SZ == '\0') {
            uVar26 = SUB84(dVar23,0);
            dVar23 = (double)CONCAT44(fVar20,*(undefined4 *)DAT_8c61ba20);
            in_xd2 = param_1;
            param_1 = (double)CONCAT44(uVar26,uVar24);
          }
          else {
            in_xd2 = *DAT_8c61ba20;
          }
          if (in_FPSCR_PR == '\0') {
            param_1 = (double)CONCAT44((float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0),
                                       SUB84(param_1,0));
          }
          else {
            param_1 = param_1 - in_dr0;
          }
          if (in_FPSCR_SZ == '\0') {
            uStack_38 = SUB84(dVar23,0);
            dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(undefined4 *)DAT_8c61ba24);
          }
          else {
            uStack_38 = SUB84(in_xd2,0);
            in_xd2 = *DAT_8c61ba24;
          }
          fStack_2c = (float)((ulonglong)param_1 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),SUB84(dVar23,0) * fStack_2c);
          }
          else {
            dVar23 = dVar23 * param_1;
          }
          if (in_FPSCR_SZ == '\0') {
            uStack_34 = SUB84(dVar23,0);
            dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(undefined4 *)DAT_8c61ba28);
          }
          else {
            uStack_34 = SUB84(in_xd2,0);
            in_xd2 = *DAT_8c61ba28;
          }
          if (in_FPSCR_PR == '\0') {
            dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),SUB84(dVar23,0) * fStack_2c);
          }
          else {
            dVar23 = dVar23 * param_1;
          }
          if (in_FPSCR_SZ == '\0') {
            fStack_30 = SUB84(dVar23,0);
            dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),
                                      *(undefined4 *)((int)DAT_8c61ba20 + 0xc));
          }
          else {
            fStack_30 = SUB84(in_xd2,0);
            in_xd2 = *(double *)((int)DAT_8c61ba20 + 0xc);
          }
          if (in_FPSCR_PR == '\0') {
            fStack_2c = SUB84(dVar23,0) * fStack_2c;
          }
          else {
            fStack_2c = SUB84(dVar23 * param_1,0);
          }
          if (in_FPSCR_SZ != '\0') {
            fStack_2c = SUB84(in_xd2,0);
            uStack_28 = (undefined4)((ulonglong)in_xd2 >> 0x20);
          }
          if ((*(uint *)((int)pdVar14 + 0xc) & *(uint *)((int)pdVar16 + 0xc) &
               *(uint *)((int)pdVar17 + 0xc) & 1) == 0) {
            in_dr0 = (double)(*(code *)PTR_FUN_8c61ba3c)(pdVar14,pdVar16,pdVar17,&uStack_38);
          }
          if ((*(uint *)((int)pdVar16 + 0xc) & *(uint *)((int)pdVar17 + 0xc) &
               *(uint *)(iVar6 + 0xc) & 1) == 0) {
            in_dr0 = (double)(*(code *)PTR_FUN_8c61ba3c)(pdVar16,pdVar17,iVar6,&uStack_38);
          }
        }
      }
    }
  }
  else if (uVar3 == DAT_8c61b4f0) {
    for (local_40 = 0; uVar24 = SUB84(param_1,0), local_40 < (int)(uint)param_3[1];
        local_40 = local_40 + 1) {
      uVar7 = 0;
      puVar12 = puVar11 + 1;
      iVar9 = 0;
      uVar10 = (uint)*puVar11;
      *DAT_8c61bc68 = *DAT_8c61bc68 + uVar10;
      uVar3 = 0xffffffff;
      puVar5 = puVar12;
      if (uVar10 != 0) {
        do {
          uVar1 = *puVar5;
          puVar5 = puVar5 + 1;
          iVar9 = iVar9 + 1;
          uVar15 = *(uint *)((uint)uVar1 * 0x20 + *DAT_8c61bc6c + 0xc);
          uVar3 = uVar3 & uVar15;
          uVar7 = uVar7 | uVar15;
        } while (iVar9 < (int)uVar10);
      }
      if ((uVar3 & 0x3f) == 0) {
        if ((uVar7 & 1) == 0) {
          iVar9 = *DAT_8c61bc6c;
          unaff_r14 = (double *)((uint)puVar11[2] * 0x20 + iVar9);
          if (in_FPSCR_SZ == '\0') {
            uVar24 = *(undefined4 *)(unaff_r14 + 2);
          }
          else {
            in_xd4 = unaff_r14[2];
          }
          unaff_r12 = (double *)((uint)*puVar12 * 0x20 + iVar9);
          if (in_FPSCR_SZ == '\0') {
            dVar22 = (double)CONCAT44(uVar24,*(undefined4 *)(unaff_r12 + 2));
          }
          else {
            in_xd2 = unaff_r12[2];
            dVar22 = in_xd4;
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) - SUB84(dVar22,0),
                                      SUB84(dVar22,0));
          }
          else {
            dVar22 = dVar22 - dVar22;
          }
          unaff_r13 = (double *)((uint)puVar11[3] * 0x20 + iVar9);
          if (in_FPSCR_SZ == '\0') {
            fVar20 = *(float *)((int)unaff_r14 + 0x14);
            dVar23 = (double)CONCAT44(fVar20,uVar24);
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                      *(undefined4 *)((int)unaff_r13 + 0x14));
            in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar20);
          }
          else {
            dVar23 = *(double *)((int)unaff_r14 + 0x14);
            in_xd2 = *(double *)((int)unaff_r13 + 0x14);
            fVar20 = (float)((ulonglong)dVar23 >> 0x20);
          }
          if (in_FPSCR_PR == '\0') {
            fVar20 = SUB84(dVar22,0) - fVar20;
            param_1 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) * fVar20,fVar20);
          }
          else {
            param_1 = (dVar22 - dVar23) * (dVar22 - dVar23);
          }
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),
                                       *(undefined4 *)((int)unaff_r12 + 0x14));
          }
          else {
            in_xd2 = *(double *)((int)unaff_r12 + 0x14);
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                      SUB84(in_dr0,0) - SUB84(param_1,0));
          }
          else {
            dVar22 = in_dr0 - param_1;
          }
          uVar24 = (undefined4)((ulonglong)param_1 >> 0x20);
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44(uVar24,SUB84(dVar23,0));
            dVar23 = (double)CONCAT44(uVar24,*(undefined4 *)(unaff_r13 + 2));
          }
          else {
            in_xd2 = unaff_r13[2];
            dVar23 = param_1;
          }
          if (in_FPSCR_PR == '\0') {
            fVar20 = SUB84(dVar23,0) - SUB84(param_1,0);
            dVar23 = (double)CONCAT44(uVar24,fVar20);
            in_dr0 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),SUB84(dVar22,0) * fVar20);
          }
          else {
            dVar23 = dVar23 - param_1;
            in_dr0 = dVar22 * dVar23;
          }
          if (in_FPSCR_PR == '\0') {
            fVar20 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
            param_1 = (double)CONCAT44(fVar20,SUB84(param_1,0));
            bVar29 = 0.0 < fVar20;
          }
          else {
            param_1 = param_1 - in_dr0;
            bVar29 = (double)((ulonglong)dVar23 & 0xffffffff00000000) < param_1;
          }
          uVar24 = SUB84(param_1,0);
          if (bVar29) goto LAB_8c61badc;
          *DAT_8c61bc70 = *DAT_8c61bc70 + uVar10;
          if (in_FPSCR_SZ == '\0') {
            dVar22 = (double)CONCAT44(*(undefined4 *)unaff_r12,*(undefined4 *)unaff_r14);
          }
          else {
            in_xd2 = *unaff_r14;
            dVar22 = *unaff_r12;
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                      SUB84(dVar22,0));
          }
          else {
            dVar22 = dVar22 + dVar22;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)unaff_r13);
          }
          else {
            in_xd2 = *unaff_r13;
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                      SUB84(dVar22,0));
          }
          else {
            dVar22 = dVar22 + dVar22;
          }
          if (in_FPSCR_SZ == '\0') {
            uStack_28 = (undefined4)((ulonglong)dVar22 >> 0x20);
            dVar22 = (double)CONCAT44(*(undefined4 *)((int)unaff_r12 + 4),
                                      *(undefined4 *)((int)unaff_r14 + 4));
          }
          else {
            uStack_28 = SUB84(dVar22,0);
            dVar22 = *(double *)((int)unaff_r12 + 4);
            in_xd2 = *(double *)((int)unaff_r14 + 4);
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                      SUB84(dVar22,0));
          }
          else {
            dVar22 = dVar22 + dVar22;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                      *(undefined4 *)((int)unaff_r13 + 4));
          }
          else {
            in_xd2 = *(double *)((int)unaff_r13 + 4);
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                      SUB84(dVar22,0));
          }
          else {
            dVar22 = dVar22 + dVar22;
          }
          if (in_FPSCR_SZ == '\0') {
            uStack_24 = (undefined4)((ulonglong)dVar22 >> 0x20);
            dVar22 = (double)CONCAT44(*(undefined4 *)(unaff_r12 + 1),*(undefined4 *)(unaff_r14 + 1))
            ;
          }
          else {
            uStack_24 = SUB84(dVar22,0);
            in_xd2 = unaff_r14[1];
            dVar22 = unaff_r12[1];
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                      SUB84(dVar22,0));
          }
          else {
            dVar22 = dVar22 + dVar22;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                      *(undefined4 *)(unaff_r13 + 1));
          }
          else {
            in_xd2 = unaff_r13[1];
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                      SUB84(dVar22,0));
          }
          else {
            dVar22 = dVar22 + dVar22;
          }
          uVar26 = (undefined4)((ulonglong)dVar22 >> 0x20);
          uStack_20 = uVar26;
          if (in_FPSCR_SZ != '\0') {
            uStack_20 = SUB84(dVar22,0);
            local_1c = uVar26;
          }
          dVar22 = (double)(*(code *)PTR_FUN_8c61bc74)(&uStack_28);
          uVar26 = DAT_8c61bc90;
          if (in_FPSCR_SZ == '\0') {
            dVar23 = (double)CONCAT44(*(undefined4 *)DAT_8c61bc78,uStack_28);
          }
          else {
            in_xd2 = (double)CONCAT44(uStack_24,uStack_28);
            dVar23 = *DAT_8c61bc78;
          }
          if (in_FPSCR_PR == '\0') {
            dVar23 = (double)CONCAT44((float)((ulonglong)dVar23 >> 0x20) * SUB84(dVar23,0),
                                      SUB84(dVar23,0));
          }
          else {
            dVar23 = dVar23 * dVar23;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(undefined4 *)DAT_8c61bc7c);
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*DAT_8c61bc80);
          }
          else {
            in_xd2 = *DAT_8c61bc7c;
          }
          if (in_FPSCR_PR == '\0') {
            fVar20 = SUB84(dVar23,0) - (float)((ulonglong)dVar23 >> 0x20);
          }
          else {
            fVar20 = SUB84(dVar23 - dVar23,0);
          }
          if (in_FPSCR_SZ == '\0') {
            dVar23 = (double)CONCAT44(uStack_24,fVar20);
          }
          else {
            dVar23 = (double)CONCAT44(uStack_20,uStack_24);
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                      SUB84(dVar22,0) * (float)((ulonglong)dVar23 >> 0x20));
          }
          else {
            dVar22 = dVar22 * dVar23;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar23 = (double)CONCAT44(uStack_20,SUB84(dVar23,0));
          }
          else {
            dVar23 = (double)CONCAT44(local_1c,uStack_20);
          }
          if (in_FPSCR_PR == '\0') {
            dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),
                                      SUB84(dVar23,0) - SUB84(dVar22,0));
          }
          else {
            dVar23 = dVar23 - dVar22;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                      *(undefined4 *)(DAT_8c61bc78 + 1));
          }
          fVar20 = (float)((ulonglong)dVar23 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            in_dr0 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),SUB84(dVar22,0) * fVar20);
          }
          else {
            in_dr0 = dVar22 * dVar23;
          }
          if (in_FPSCR_SZ == '\0') {
            uVar19 = SUB84(dVar23,0);
            dVar23 = (double)CONCAT44(fVar20,*(undefined4 *)DAT_8c61bc84);
            dVar22 = in_xd2;
            in_xd2 = (double)CONCAT44(uVar19,uVar24);
          }
          else {
            dVar22 = *DAT_8c61bc84;
          }
          if (in_FPSCR_PR == '\0') {
            param_1 = (double)CONCAT44((float)((ulonglong)in_xd2 >> 0x20) - SUB84(in_dr0,0),
                                       SUB84(in_xd2,0));
          }
          else {
            param_1 = in_xd2 - in_dr0;
          }
          if (in_FPSCR_SZ == '\0') {
            uStack_38 = SUB84(dVar23,0);
            dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(undefined4 *)DAT_8c61bc88);
          }
          else {
            uStack_38 = SUB84(dVar22,0);
            dVar22 = *DAT_8c61bc88;
          }
          fVar20 = (float)((ulonglong)param_1 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),SUB84(dVar23,0) * fVar20);
          }
          else {
            dVar23 = dVar23 * param_1;
          }
          if (in_FPSCR_SZ == '\0') {
            uStack_34 = SUB84(dVar23,0);
            dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(undefined4 *)DAT_8c61bc8c);
          }
          else {
            uStack_34 = SUB84(dVar22,0);
            dVar22 = *DAT_8c61bc8c;
          }
          if (in_FPSCR_PR == '\0') {
            dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),SUB84(dVar23,0) * fVar20);
          }
          else {
            dVar23 = dVar23 * param_1;
          }
          if (in_FPSCR_SZ == '\0') {
            fStack_30 = SUB84(dVar23,0);
            dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),
                                      *(undefined4 *)((int)DAT_8c61bc84 + 0xc));
          }
          else {
            fStack_30 = SUB84(dVar22,0);
            dVar22 = *(double *)((int)DAT_8c61bc84 + 0xc);
          }
          if (in_FPSCR_PR == '\0') {
            dVar23 = (double)(ulonglong)(uint)(SUB84(dVar23,0) * fVar20);
          }
          else {
            dVar23 = dVar23 * param_1;
          }
          if (in_FPSCR_SZ == '\0') {
            fStack_2c = SUB84(dVar23,0);
          }
          else {
            fStack_2c = SUB84(dVar22,0);
            uStack_28 = (undefined4)((ulonglong)dVar22 >> 0x20);
          }
          iVar9 = (uint)*puVar12 * 0x20 + *DAT_8c61bc6c;
          if (in_FPSCR_SZ == '\0') {
            *(undefined4 *)((int)unaff_r8 + 0xc) = *(undefined4 *)(iVar9 + 0x18);
            *(undefined4 *)(unaff_r8 + 1) = *(undefined4 *)(iVar9 + 0x14);
            dVar23 = (double)(ulonglong)*(uint *)(iVar9 + 0x10);
            pdVar14 = (double *)((int)unaff_r8 + 4);
            *(uint *)pdVar14 = *(uint *)(iVar9 + 0x10);
          }
          else {
            unaff_r8[1] = *(undefined8 *)(iVar9 + 0x18);
            *unaff_r8 = *(undefined8 *)(iVar9 + 0x14);
            dVar22 = *(double *)(iVar9 + 0x10);
            pdVar14 = (double *)(unaff_r8 + -1);
            *pdVar14 = dVar22;
          }
          *(undefined4 *)((int)pdVar14 + -4) = uVar26;
          uVar24 = DAT_8c61bc90;
          puVar5 = puVar11 + 3;
          iVar9 = (uint)puVar11[2] * 0x20 + *DAT_8c61bc6c;
          if (in_FPSCR_SZ == '\0') {
            *(undefined4 *)(pdVar14 + 5) = *(undefined4 *)(iVar9 + 0x18);
            *(undefined4 *)((int)pdVar14 + 0x24) = *(undefined4 *)(iVar9 + 0x14);
            dVar23 = (double)(ulonglong)*(uint *)(iVar9 + 0x10);
            pdVar14 = pdVar14 + 4;
            *(uint *)pdVar14 = *(uint *)(iVar9 + 0x10);
          }
          else {
            *(undefined8 *)((int)pdVar14 + 0x24) = *(undefined8 *)(iVar9 + 0x18);
            *(undefined8 *)((int)pdVar14 + 0x1c) = *(undefined8 *)(iVar9 + 0x14);
            dVar22 = *(double *)(iVar9 + 0x10);
            pdVar14 = (double *)((int)pdVar14 + 0x14);
            *pdVar14 = dVar22;
          }
          fVar20 = SUB84(dVar23,0);
          *(undefined4 *)((int)pdVar14 + -4) = uVar24;
          iVar9 = 2;
          iVar6 = (int)pdVar14 + 0x1c;
          if (2 < (int)(uVar10 - 1)) {
            do {
              uVar1 = *puVar5;
              puVar5 = puVar5 + 1;
              if (in_FPSCR_SZ == '\0') {
                dVar23 = (double)(ulonglong)(uint)fStack_2c;
              }
              else {
                dVar22 = (double)CONCAT44(uStack_28,fStack_2c);
              }
              iVar4 = *DAT_8c61bc6c;
              if (in_FPSCR_SZ == '\0') {
                pdVar14 = (double *)(iVar6 + 0x1c);
                *(int *)pdVar14 = SUB84(dVar23,0);
                dVar23 = (double)(ulonglong)(uint)fStack_30;
              }
              else {
                pdVar14 = (double *)(iVar6 + 0x18);
                *pdVar14 = dVar22;
                dVar22 = (double)CONCAT44(fStack_2c,fStack_30);
              }
              if (in_FPSCR_SZ == '\0') {
                pdVar14 = (double *)((int)pdVar14 + -4);
                *(int *)pdVar14 = SUB84(dVar23,0);
              }
              else {
                pdVar14 = pdVar14 + -1;
                *pdVar14 = dVar22;
              }
              uVar24 = DAT_8c61bc90;
              iVar4 = (uint)uVar1 * 0x20 + iVar4;
              if (in_FPSCR_SZ == '\0') {
                *(undefined4 *)((int)pdVar14 + -4) = uStack_34;
                *(undefined4 *)(pdVar14 + -1) = uStack_38;
                *(undefined4 *)((int)pdVar14 + -0xc) = *(undefined4 *)(iVar4 + 0x18);
                *(undefined4 *)(pdVar14 + -2) = *(undefined4 *)(iVar4 + 0x14);
                dVar23 = (double)(ulonglong)*(uint *)(iVar4 + 0x10);
                pdVar14 = (double *)((int)pdVar14 + -0x14);
                *(uint *)pdVar14 = *(uint *)(iVar4 + 0x10);
              }
              else {
                pdVar14[-1] = (double)CONCAT44(fStack_30,uStack_34);
                pdVar14[-2] = (double)CONCAT44(uStack_34,uStack_38);
                pdVar14[-3] = *(double *)(iVar4 + 0x18);
                pdVar14[-4] = *(double *)(iVar4 + 0x14);
                dVar22 = *(double *)(iVar4 + 0x10);
                pdVar14 = pdVar14 + -5;
                *pdVar14 = dVar22;
              }
              fVar20 = SUB84(dVar23,0);
              *(undefined4 *)((int)pdVar14 + -4) = uVar24;
              iVar9 = iVar9 + 1;
              iVar6 = (int)pdVar14 + 0x1c;
            } while (iVar9 < (int)(uVar10 - 1));
          }
          uVar1 = *puVar5;
          puVar11 = puVar5 + 1;
          fVar21 = fStack_2c;
          if (in_FPSCR_SZ != '\0') {
            dVar22 = (double)CONCAT44(uStack_28,fStack_2c);
            fVar21 = fVar20;
          }
          iVar9 = *DAT_8c61bc6c;
          if (in_FPSCR_SZ == '\0') {
            pdVar14 = (double *)(iVar6 + 0x1c);
            *(float *)pdVar14 = fVar21;
            fVar21 = fStack_30;
          }
          else {
            pdVar14 = (double *)(iVar6 + 0x18);
            *pdVar14 = dVar22;
            dVar22 = (double)CONCAT44(fStack_2c,fStack_30);
          }
          if (in_FPSCR_SZ == '\0') {
            pdVar14 = (double *)((int)pdVar14 + -4);
            *(float *)pdVar14 = fVar21;
          }
          else {
            pdVar14 = pdVar14 + -1;
            *pdVar14 = dVar22;
          }
          uVar24 = DAT_8c61bc94;
          iVar9 = (uint)uVar1 * 0x20 + iVar9;
          if (in_FPSCR_SZ == '\0') {
            *(undefined4 *)((int)pdVar14 + -4) = uStack_34;
            *(undefined4 *)(pdVar14 + -1) = uStack_38;
            *(undefined4 *)((int)pdVar14 + -0xc) = *(undefined4 *)(iVar9 + 0x18);
            *(undefined4 *)(pdVar14 + -2) = *(undefined4 *)(iVar9 + 0x14);
            pdVar14 = (double *)((int)pdVar14 + -0x14);
            *(undefined4 *)pdVar14 = *(undefined4 *)(iVar9 + 0x10);
            in_xd2 = dVar22;
          }
          else {
            pdVar14[-1] = (double)CONCAT44(fStack_30,uStack_34);
            pdVar14[-2] = (double)CONCAT44(uStack_34,uStack_38);
            pdVar14[-3] = *(double *)(iVar9 + 0x18);
            pdVar14[-4] = *(double *)(iVar9 + 0x14);
            in_xd2 = *(double *)(iVar9 + 0x10);
            pdVar14 = pdVar14 + -5;
            *pdVar14 = in_xd2;
          }
          *(undefined4 *)((int)pdVar14 + -4) = uVar24;
          unaff_r8 = (undefined8 *)((int)pdVar14 + 0x1c);
        }
        else {
          *DAT_8c61be14 = 0;
          if (in_FPSCR_SZ == '\0') {
            dVar22 = (double)CONCAT44(*(undefined4 *)unaff_r12,*(undefined4 *)unaff_r14);
          }
          else {
            in_xd2 = *unaff_r14;
            dVar22 = *unaff_r12;
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                      SUB84(dVar22,0));
          }
          else {
            dVar22 = dVar22 + dVar22;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)unaff_r13);
          }
          else {
            in_xd2 = *unaff_r13;
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                      SUB84(dVar22,0));
          }
          else {
            dVar22 = dVar22 + dVar22;
          }
          if (in_FPSCR_SZ == '\0') {
            uStack_28 = (undefined4)((ulonglong)dVar22 >> 0x20);
            dVar22 = (double)CONCAT44(*(undefined4 *)((int)unaff_r12 + 4),
                                      *(undefined4 *)((int)unaff_r14 + 4));
          }
          else {
            uStack_28 = SUB84(dVar22,0);
            in_xd2 = *(double *)((int)unaff_r14 + 4);
            dVar22 = *(double *)((int)unaff_r12 + 4);
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                      SUB84(dVar22,0));
          }
          else {
            dVar22 = dVar22 + dVar22;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                      *(undefined4 *)((int)unaff_r13 + 4));
          }
          else {
            in_xd2 = *(double *)((int)unaff_r13 + 4);
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                      SUB84(dVar22,0));
          }
          else {
            dVar22 = dVar22 + dVar22;
          }
          if (in_FPSCR_SZ == '\0') {
            uStack_24 = (undefined4)((ulonglong)dVar22 >> 0x20);
            dVar22 = (double)CONCAT44(*(undefined4 *)(unaff_r12 + 1),*(undefined4 *)(unaff_r14 + 1))
            ;
          }
          else {
            uStack_24 = SUB84(dVar22,0);
            in_xd2 = unaff_r14[1];
            dVar22 = unaff_r12[1];
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                      SUB84(dVar22,0));
          }
          else {
            dVar22 = dVar22 + dVar22;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                      *(undefined4 *)(unaff_r13 + 1));
          }
          else {
            in_xd2 = unaff_r13[1];
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                      SUB84(dVar22,0));
          }
          else {
            dVar22 = dVar22 + dVar22;
          }
          uVar26 = (undefined4)((ulonglong)dVar22 >> 0x20);
          uStack_20 = uVar26;
          if (in_FPSCR_SZ != '\0') {
            uStack_20 = SUB84(dVar22,0);
            local_1c = uVar26;
          }
          dVar22 = (double)(*(code *)PTR_FUN_8c61be18)(&uStack_28);
          if (in_FPSCR_SZ == '\0') {
            dVar23 = (double)CONCAT44(*(undefined4 *)DAT_8c61be1c,uStack_28);
          }
          else {
            in_xd2 = (double)CONCAT44(uStack_24,uStack_28);
            dVar23 = *DAT_8c61be1c;
          }
          if (in_FPSCR_PR == '\0') {
            dVar23 = (double)CONCAT44((float)((ulonglong)dVar23 >> 0x20) * SUB84(dVar23,0),
                                      SUB84(dVar23,0));
          }
          else {
            dVar23 = dVar23 * dVar23;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(undefined4 *)DAT_8c61be20);
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*DAT_8c61be24);
            param_1 = in_xd2;
          }
          else {
            param_1 = *DAT_8c61be20;
          }
          if (in_FPSCR_PR == '\0') {
            fVar20 = SUB84(dVar23,0) - (float)((ulonglong)dVar23 >> 0x20);
          }
          else {
            fVar20 = SUB84(dVar23 - dVar23,0);
          }
          if (in_FPSCR_SZ == '\0') {
            dVar23 = (double)CONCAT44(uStack_24,fVar20);
          }
          else {
            dVar23 = (double)CONCAT44(uStack_20,uStack_24);
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                      SUB84(dVar22,0) * (float)((ulonglong)dVar23 >> 0x20));
          }
          else {
            dVar22 = dVar22 * dVar23;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar23 = (double)CONCAT44(uStack_20,SUB84(dVar23,0));
          }
          else {
            dVar23 = (double)CONCAT44(local_1c,uStack_20);
          }
          if (in_FPSCR_PR == '\0') {
            dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),
                                      SUB84(dVar23,0) - SUB84(dVar22,0));
          }
          else {
            dVar23 = dVar23 - dVar22;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                      *(undefined4 *)(DAT_8c61be1c + 1));
          }
          fVar20 = (float)((ulonglong)dVar23 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            in_dr0 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),SUB84(dVar22,0) * fVar20);
          }
          else {
            in_dr0 = dVar22 * dVar23;
          }
          if (in_FPSCR_SZ == '\0') {
            uVar26 = SUB84(dVar23,0);
            dVar23 = (double)CONCAT44(fVar20,*(undefined4 *)DAT_8c61be28);
            in_xd2 = param_1;
            param_1 = (double)CONCAT44(uVar26,uVar24);
          }
          else {
            in_xd2 = *DAT_8c61be28;
          }
          if (in_FPSCR_PR == '\0') {
            param_1 = (double)CONCAT44((float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0),
                                       SUB84(param_1,0));
          }
          else {
            param_1 = param_1 - in_dr0;
          }
          if (in_FPSCR_SZ == '\0') {
            uStack_38 = SUB84(dVar23,0);
            dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(undefined4 *)DAT_8c61be2c);
          }
          else {
            uStack_38 = SUB84(in_xd2,0);
            in_xd2 = *DAT_8c61be2c;
          }
          fStack_2c = (float)((ulonglong)param_1 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),SUB84(dVar23,0) * fStack_2c);
          }
          else {
            dVar23 = dVar23 * param_1;
          }
          if (in_FPSCR_SZ == '\0') {
            uStack_34 = SUB84(dVar23,0);
            dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(undefined4 *)DAT_8c61be30);
          }
          else {
            uStack_34 = SUB84(in_xd2,0);
            in_xd2 = *DAT_8c61be30;
          }
          if (in_FPSCR_PR == '\0') {
            dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),SUB84(dVar23,0) * fStack_2c);
          }
          else {
            dVar23 = dVar23 * param_1;
          }
          if (in_FPSCR_SZ == '\0') {
            fStack_30 = SUB84(dVar23,0);
            dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),
                                      *(undefined4 *)((int)DAT_8c61be28 + 0xc));
          }
          else {
            fStack_30 = SUB84(in_xd2,0);
            in_xd2 = *(double *)((int)DAT_8c61be28 + 0xc);
          }
          if (in_FPSCR_PR == '\0') {
            fStack_2c = SUB84(dVar23,0) * fStack_2c;
          }
          else {
            fStack_2c = SUB84(dVar23 * param_1,0);
          }
          if (in_FPSCR_SZ != '\0') {
            fStack_2c = SUB84(in_xd2,0);
            uStack_28 = (undefined4)((ulonglong)in_xd2 >> 0x20);
          }
          puVar5 = puVar11 + 2;
          puVar11 = puVar11 + 3;
          unaff_r12 = (double *)((uint)*puVar12 * 0x20 + *DAT_8c61be34);
          unaff_r14 = (double *)((uint)*puVar5 * 0x20 + *DAT_8c61be34);
          iVar9 = 0;
          pdVar14 = unaff_r12;
          if (0 < (int)(uVar10 - 2)) {
            do {
              unaff_r12 = unaff_r14;
              uVar1 = *puVar11;
              puVar11 = puVar11 + 1;
              unaff_r13 = (double *)((uint)uVar1 * 0x20 + *DAT_8c61be34);
              if ((*(uint *)((int)pdVar14 + 0xc) & *(uint *)((int)unaff_r12 + 0xc) &
                   *(uint *)((int)unaff_r13 + 0xc) & 1) == 0) {
                in_dr0 = (double)(*(code *)PTR_FUN_8c61be38)(pdVar14,unaff_r12,unaff_r13,&uStack_38)
                ;
              }
              iVar9 = iVar9 + 1;
              pdVar14 = unaff_r12;
              unaff_r14 = unaff_r13;
            } while (iVar9 < (int)(uVar10 - 2));
          }
        }
      }
      else {
LAB_8c61badc:
        puVar11 = puVar12 + uVar10;
      }
    }
  }
  else if (uVar3 == DAT_8c61b4ec) {
    *DAT_8c61be3c = param_3;
    if (in_FPSCR_SZ == '\0') {
      uVar25 = CONCAT44(*(undefined4 *)(DAT_8c61be08 + 0x10),*(undefined4 *)(DAT_8c61be08 + 0x14));
      uVar27 = CONCAT44(*(undefined4 *)(DAT_8c61be08 + 0x18),*(undefined4 *)(DAT_8c61be08 + 0x1c));
      uVar28 = CONCAT44(*(undefined4 *)DAT_8c61be0c,param_2);
    }
    else {
      uVar25 = *(undefined8 *)(DAT_8c61be08 + 0x10);
      uVar27 = *(undefined8 *)(DAT_8c61be08 + 0x20);
      uVar28 = *DAT_8c61be0c;
    }
    (*(code *)PTR_FUN_8c61be40)(uVar25,uVar27,uVar28,*DAT_8c61be3c);
  }
  return;
}

