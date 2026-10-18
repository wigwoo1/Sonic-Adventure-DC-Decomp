/* Ghidra draft only; not buildable/verified source. */

void FUN_8c61e4f4(double param_1,undefined8 param_2,double param_3,undefined8 param_4,
                 ushort *param_5)

{
  ushort uVar1;
  float fVar2;
  int *piVar3;
  int *piVar4;
  undefined4 uVar5;
  undefined *puVar6;
  uint uVar7;
  double *pdVar8;
  ulonglong *puVar9;
  int iVar10;
  ushort *puVar11;
  int iVar12;
  uint uVar13;
  int unaff_r8;
  double *pdVar14;
  int iVar15;
  uint uVar16;
  ushort *puVar17;
  ushort *puVar18;
  ushort *puVar19;
  double *pdVar20;
  uint uVar21;
  double *unaff_r12;
  undefined8 *puVar22;
  double *unaff_r13;
  double *pdVar23;
  double *unaff_r14;
  double *pdVar24;
  undefined8 *puVar25;
  double in_dr0;
  double dVar26;
  double dVar27;
  undefined8 uVar28;
  float fVar29;
  undefined8 in_dr2;
  double dVar30;
  ulonglong uVar31;
  undefined4 uVar34;
  double dVar32;
  double dVar33;
  undefined4 uVar35;
  undefined4 uVar36;
  float fVar37;
  undefined8 in_dr12;
  undefined8 in_dr14;
  double dVar38;
  undefined8 in_xd0;
  double in_xd2;
  double in_xd4;
  undefined8 in_xd6;
  undefined8 in_xd8;
  undefined8 in_xd10;
  undefined8 in_xd12;
  double in_xd14;
  bool bVar39;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_28 [8];
  undefined8 uStack_20;
  
  if (in_FPSCR_SZ == '\0') {
    uStack_20 = (double)CONCAT44((int)in_dr14,(undefined4)uStack_20);
    puVar25 = &uStack_20;
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_20._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
  }
  else {
    puVar25 = (undefined8 *)auStack_28;
    uStack_20 = in_xd14;
  }
  *(undefined4 *)((int)puVar25 + -4) = in_PR;
  if (in_FPSCR_SZ == '\0') {
    dVar38 = (double)CONCAT44(DAT_8c61e560,(int)in_dr14);
  }
  else {
    dVar38 = (double)CONCAT44(DAT_8c61e564,DAT_8c61e560);
  }
  *(ushort **)((int)puVar25 + -0x1c) = param_5;
  uVar7 = *param_5 & DAT_8c61e564;
  puVar17 = *(ushort **)(param_5 + 2);
  if (uVar7 == 0) {
    *DAT_8c61e56c = *DAT_8c61e56c + (uint)*(ushort *)(*(int *)((int)puVar25 + -0x1c) + 2) * 3;
    for (iVar15 = 0; uVar35 = SUB84(param_1,0),
        iVar15 < (int)(uint)*(ushort *)(*(int *)((int)puVar25 + -0x1c) + 2); iVar15 = iVar15 + 1) {
      uVar1 = *puVar17;
      puVar11 = puVar17 + 1;
      puVar18 = puVar17 + 2;
      iVar12 = *DAT_8c61e840;
      puVar17 = puVar17 + 3;
      pdVar24 = (double *)((uint)uVar1 * 0x20 + iVar12);
      pdVar20 = (double *)((uint)*puVar11 * 0x20 + iVar12);
      pdVar23 = (double *)((uint)*puVar18 * 0x20 + iVar12);
      if ((*(uint *)((int)pdVar24 + 0xc) & *(uint *)((int)pdVar20 + 0xc) &
           *(uint *)((int)pdVar23 + 0xc) & 0x3f) == 0) {
        if (((*(uint *)((int)pdVar24 + 0xc) | *(uint *)((int)pdVar20 + 0xc) |
             *(uint *)((int)pdVar23 + 0xc)) & 1) == 0) {
          if (in_FPSCR_SZ == '\0') {
            uVar36 = *(undefined4 *)(pdVar20 + 2);
            dVar33 = (double)CONCAT44(uVar36,*(undefined4 *)(pdVar24 + 2));
            dVar32 = (double)CONCAT44(uVar36,uVar35);
          }
          else {
            dVar33 = pdVar20[2];
            in_xd2 = pdVar24[2];
            uVar36 = (undefined4)((ulonglong)dVar33 >> 0x20);
            dVar32 = dVar33;
          }
          if (in_FPSCR_PR == '\0') {
            dVar33 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) - SUB84(dVar33,0),
                                      SUB84(dVar33,0));
          }
          else {
            dVar33 = dVar33 - dVar33;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),
                                      *(undefined4 *)((int)pdVar23 + 0x14));
            dVar32 = (double)CONCAT44(uVar36,*(undefined4 *)((int)pdVar20 + 0x14));
          }
          else {
            in_xd2 = *(double *)((int)pdVar23 + 0x14);
          }
          fVar29 = SUB84(dVar32,0);
          if (in_FPSCR_PR == '\0') {
            dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),SUB84(dVar33,0) - fVar29);
          }
          else {
            dVar33 = dVar33 - dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar29);
          }
          if (in_FPSCR_PR == '\0') {
            dVar33 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) * SUB84(dVar33,0),
                                      SUB84(dVar33,0));
          }
          else {
            dVar33 = dVar33 * dVar33;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),
                                      *(undefined4 *)((int)pdVar24 + 0x14));
          }
          else {
            in_xd2 = *(double *)((int)pdVar24 + 0x14);
          }
          if (in_FPSCR_PR == '\0') {
            dVar27 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                      SUB84(in_dr0,0) - SUB84(dVar33,0));
          }
          else {
            dVar27 = in_dr0 - dVar33;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),*(undefined4 *)(pdVar23 + 2))
            ;
          }
          else {
            in_xd2 = pdVar23[2];
          }
          if (in_FPSCR_PR == '\0') {
            dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),
                                      SUB84(dVar33,0) - (float)((ulonglong)dVar32 >> 0x20));
          }
          else {
            dVar33 = dVar33 - dVar32;
          }
          uVar7 = (uint)((ulonglong)dVar33 >> 0x20);
          param_1 = dVar33;
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44(uVar7,fVar29);
          }
          if (in_FPSCR_PR == '\0') {
            in_dr0 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),
                                      SUB84(dVar27,0) * SUB84(dVar33,0));
          }
          else {
            in_dr0 = dVar27 * dVar33;
          }
          if (in_FPSCR_PR == '\0') {
            fVar29 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
            param_1 = (double)CONCAT44(fVar29,SUB84(param_1,0));
            bVar39 = 0.0 < fVar29;
          }
          else {
            param_1 = param_1 - in_dr0;
            bVar39 = (double)((ulonglong)uVar7 << 0x20) < param_1;
          }
          uVar35 = SUB84(param_1,0);
          if (!bVar39) {
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)pdVar24,*(undefined4 *)pdVar20);
            }
            else {
              dVar32 = *pdVar24;
              in_xd2 = *pdVar20;
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0),
                                        SUB84(dVar32,0));
            }
            else {
              dVar32 = dVar32 + dVar32;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),*(undefined4 *)pdVar23);
            }
            else {
              in_xd2 = *pdVar23;
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0),
                                        SUB84(dVar32,0));
            }
            else {
              dVar32 = dVar32 + dVar32;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)puVar25 + -0x10) = (int)((ulonglong)dVar32 >> 0x20);
              dVar32 = (double)CONCAT44(*(undefined4 *)((int)pdVar24 + 4),
                                        *(undefined4 *)((int)pdVar20 + 4));
            }
            else {
              *(double *)((int)puVar25 + -0x10) = dVar32;
              dVar32 = *(double *)((int)pdVar24 + 4);
              in_xd2 = *(double *)((int)pdVar20 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0),
                                        SUB84(dVar32,0));
            }
            else {
              dVar32 = dVar32 + dVar32;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                        *(undefined4 *)((int)pdVar23 + 4));
            }
            else {
              in_xd2 = *(double *)((int)pdVar23 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0),
                                        SUB84(dVar32,0));
            }
            else {
              dVar32 = dVar32 + dVar32;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)puVar25 + -0xc) = (int)((ulonglong)dVar32 >> 0x20);
              dVar32 = (double)CONCAT44(*(undefined4 *)(pdVar24 + 1),*(undefined4 *)(pdVar20 + 1));
            }
            else {
              *(double *)((int)puVar25 + -0xc) = dVar32;
              dVar32 = pdVar24[1];
              in_xd2 = pdVar20[1];
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0),
                                        SUB84(dVar32,0));
            }
            else {
              dVar32 = dVar32 + dVar32;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                        *(undefined4 *)(pdVar23 + 1));
            }
            else {
              in_xd2 = pdVar23[1];
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0),
                                        SUB84(dVar32,0));
            }
            else {
              dVar32 = dVar32 + dVar32;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)puVar25 + -8) = (int)((ulonglong)dVar32 >> 0x20);
            }
            else {
              *(double *)((int)puVar25 + -8) = dVar32;
            }
            dVar32 = (double)(*(code *)PTR_FUN_8c61e844)((undefined1 *)((int)puVar25 + -0x10));
            piVar3 = DAT_8c61e854;
            if (in_FPSCR_SZ == '\0') {
              dVar33 = (double)CONCAT44(*(undefined4 *)DAT_8c61e848,
                                        *(undefined4 *)((int)puVar25 + -0x10));
            }
            else {
              in_xd2 = *(double *)((int)puVar25 + -0x10);
              dVar33 = *DAT_8c61e848;
            }
            if (in_FPSCR_PR == '\0') {
              dVar33 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) * SUB84(dVar33,0),
                                        SUB84(dVar33,0));
            }
            else {
              dVar33 = dVar33 * dVar33;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),*(undefined4 *)DAT_8c61e84c
                                       );
              dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),*DAT_8c61e850);
            }
            else {
              in_xd2 = *DAT_8c61e84c;
            }
            if (in_FPSCR_PR == '\0') {
              fVar29 = SUB84(dVar33,0) - (float)((ulonglong)dVar33 >> 0x20);
            }
            else {
              fVar29 = SUB84(dVar33 - dVar33,0);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar33 = (double)CONCAT44(*(undefined4 *)((int)puVar25 + -0xc),fVar29);
            }
            else {
              dVar33 = *(double *)((int)puVar25 + -0xc);
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                        SUB84(dVar32,0) * (float)((ulonglong)dVar33 >> 0x20));
            }
            else {
              dVar32 = dVar32 * dVar33;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar33 = (double)CONCAT44(*(undefined4 *)((int)puVar25 + -8),SUB84(dVar33,0));
            }
            else {
              dVar33 = *(double *)((int)puVar25 + -8);
            }
            iVar12 = *DAT_8c61e854;
            if (in_FPSCR_PR == '\0') {
              dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),
                                        SUB84(dVar33,0) - SUB84(dVar32,0));
            }
            else {
              dVar33 = dVar33 - dVar32;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)((int)pdVar24 + 4),
                                        *(undefined4 *)(DAT_8c61e848 + 1));
            }
            else {
              dVar32 = *(double *)((int)pdVar24 + 4);
            }
            fVar29 = (float)((ulonglong)dVar33 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),SUB84(dVar32,0) * fVar29);
            }
            else {
              dVar32 = dVar32 * dVar33;
            }
            if (in_FPSCR_SZ == '\0') {
              uVar36 = SUB84(dVar33,0);
              dVar33 = (double)CONCAT44(fVar29,*(undefined4 *)(iVar12 + 0x14));
              dVar27 = in_xd2;
              in_xd2 = (double)CONCAT44(uVar36,uVar35);
            }
            else {
              dVar27 = *(double *)(iVar12 + 0x14);
            }
            if (in_FPSCR_PR == '\0') {
              param_1 = (double)CONCAT44((float)((ulonglong)in_xd2 >> 0x20) - SUB84(dVar32,0),
                                         SUB84(in_xd2,0));
              fVar29 = SUB84(dVar33,0) * (float)((ulonglong)dVar32 >> 0x20);
            }
            else {
              param_1 = in_xd2 - dVar32;
              fVar29 = SUB84(dVar33 * dVar32,0);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)pdVar24,SUB84(dVar32,0));
              uVar31 = CONCAT44(*(undefined4 *)(iVar12 + 4),fVar29);
            }
            else {
              dVar32 = *pdVar24;
              uVar31 = *(ulonglong *)(iVar12 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              uVar31 = (ulonglong)
                       (uint)((float)((ulonglong)dVar32 >> 0x20) * (float)(uVar31 >> 0x20) +
                             (float)uVar31);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)(pdVar24 + 1),SUB84(dVar32,0));
              uVar31 = CONCAT44(*(undefined4 *)(iVar12 + 0x24),(int)uVar31);
            }
            else {
              dVar32 = pdVar24[1];
              uVar31 = *(ulonglong *)(iVar12 + 0x24);
            }
            if (in_FPSCR_PR == '\0') {
              uVar31 = (ulonglong)
                       (uint)((float)((ulonglong)dVar32 >> 0x20) * (float)(uVar31 >> 0x20) +
                             (float)uVar31);
            }
            uVar35 = (undefined4)((ulonglong)dVar38 >> 0x20);
            dVar26 = dVar38;
            dVar33 = dVar38;
            if (in_FPSCR_SZ == '\0') {
              dVar26 = (double)CONCAT44(uVar35,(int)uVar31);
              dVar33 = (double)CONCAT44(uVar35,SUB84(dVar32,0));
            }
            if (in_FPSCR_PR == '\0') {
              dVar26 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) * SUB84(dVar26,0) +
                                        (float)((ulonglong)dVar26 >> 0x20),SUB84(dVar26,0));
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar14 = (double *)(unaff_r8 + 0x14);
              *(int *)pdVar14 = (int)((ulonglong)dVar26 >> 0x20);
            }
            else {
              pdVar14 = (double *)(unaff_r8 + 0x10);
              *pdVar14 = dVar26;
            }
            pdVar8 = (double *)*piVar3;
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)((int)pdVar24 + 4),SUB84(dVar33,0));
              dVar33 = (double)CONCAT44(*(undefined4 *)pdVar8,*(undefined4 *)(pdVar8 + 2));
            }
            else {
              dVar32 = *(double *)((int)pdVar24 + 4);
              dVar27 = pdVar8[2];
              dVar33 = *pdVar8;
            }
            if (in_FPSCR_PR == '\0') {
              dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),
                                        SUB84(dVar33,0) * (float)((ulonglong)dVar32 >> 0x20));
            }
            else {
              dVar33 = dVar33 * dVar32;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)pdVar24,SUB84(dVar32,0));
            }
            else {
              dVar32 = *pdVar24;
            }
            if (in_FPSCR_PR == '\0') {
              dVar33 = (double)(ulonglong)
                               (uint)((float)((ulonglong)dVar32 >> 0x20) *
                                      (float)((ulonglong)dVar33 >> 0x20) + SUB84(dVar33,0));
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)(pdVar24 + 1),SUB84(dVar32,0));
              dVar33 = (double)CONCAT44(*(undefined4 *)(pdVar8 + 4),SUB84(dVar33,0));
            }
            else {
              dVar32 = pdVar24[1];
              dVar33 = pdVar8[4];
            }
            if (in_FPSCR_PR == '\0') {
              dVar33 = (double)(ulonglong)
                               (uint)((float)((ulonglong)dVar32 >> 0x20) *
                                      (float)((ulonglong)dVar33 >> 0x20) + SUB84(dVar33,0));
            }
            dVar30 = dVar38;
            dVar26 = dVar38;
            if (in_FPSCR_SZ == '\0') {
              dVar30 = (double)CONCAT44(uVar35,SUB84(dVar33,0));
              dVar26 = (double)CONCAT44(uVar35,SUB84(dVar32,0));
            }
            if (in_FPSCR_PR == '\0') {
              dVar30 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) * SUB84(dVar30,0) +
                                        (float)((ulonglong)dVar30 >> 0x20),SUB84(dVar30,0));
            }
            uVar36 = (undefined4)((ulonglong)dVar30 >> 0x20);
            if (in_FPSCR_SZ == '\0') {
              *(undefined4 *)((int)pdVar14 + -4) = uVar36;
              uVar34 = DAT_8c61e858;
              *(undefined4 *)(pdVar14 + -1) = *(undefined4 *)(pdVar24 + 3);
              *(undefined4 *)((int)pdVar14 + -0xc) = *(undefined4 *)((int)pdVar24 + 0x14);
              dVar30 = (double)CONCAT44(uVar36,*(undefined4 *)(pdVar24 + 2));
              pdVar14 = pdVar14 + -2;
              *(undefined4 *)pdVar14 = *(undefined4 *)(pdVar24 + 2);
            }
            else {
              pdVar14[-1] = dVar30;
              uVar34 = DAT_8c61e858;
              pdVar14[-2] = pdVar24[3];
              pdVar14[-3] = *(double *)((int)pdVar24 + 0x14);
              dVar27 = pdVar24[2];
              pdVar14 = pdVar14 + -4;
              *pdVar14 = dVar27;
            }
            *(undefined4 *)((int)pdVar14 + -4) = uVar34;
            piVar3 = DAT_8c61e854;
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)((int)pdVar20 + 4),SUB84(dVar26,0));
            }
            else {
              dVar32 = *(double *)((int)pdVar20 + 4);
            }
            iVar12 = *DAT_8c61e854;
            if (in_FPSCR_SZ == '\0') {
              dVar30 = (double)CONCAT44((int)((ulonglong)dVar30 >> 0x20),
                                        *(undefined4 *)(iVar12 + 0x14));
            }
            else {
              dVar27 = *(double *)(iVar12 + 0x14);
            }
            if (in_FPSCR_PR == '\0') {
              fVar29 = SUB84(dVar30,0) * (float)((ulonglong)dVar32 >> 0x20);
            }
            else {
              fVar29 = SUB84(dVar30 * dVar32,0);
            }
            if (in_FPSCR_SZ == '\0') {
              uVar31 = CONCAT44(*(undefined4 *)(iVar12 + 4),fVar29);
              dVar32 = (double)CONCAT44(*(undefined4 *)pdVar20,SUB84(dVar32,0));
            }
            else {
              uVar31 = *(ulonglong *)(iVar12 + 4);
              dVar32 = *pdVar20;
            }
            if (in_FPSCR_PR == '\0') {
              uVar31 = (ulonglong)
                       (uint)((float)((ulonglong)dVar32 >> 0x20) * (float)(uVar31 >> 0x20) +
                             (float)uVar31);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)(pdVar20 + 1),SUB84(dVar32,0));
              uVar31 = CONCAT44(*(undefined4 *)(iVar12 + 0x24),(int)uVar31);
            }
            else {
              dVar32 = pdVar20[1];
              uVar31 = *(ulonglong *)(iVar12 + 0x24);
            }
            if (in_FPSCR_PR == '\0') {
              uVar31 = (ulonglong)
                       (uint)((float)((ulonglong)dVar32 >> 0x20) * (float)(uVar31 >> 0x20) +
                             (float)uVar31);
            }
            dVar26 = dVar38;
            dVar33 = dVar38;
            if (in_FPSCR_SZ == '\0') {
              dVar26 = (double)CONCAT44(uVar35,(int)uVar31);
              dVar33 = (double)CONCAT44(uVar35,SUB84(dVar32,0));
            }
            if (in_FPSCR_PR == '\0') {
              dVar26 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) * SUB84(dVar26,0) +
                                        (float)((ulonglong)dVar26 >> 0x20),SUB84(dVar26,0));
            }
            uVar36 = (undefined4)((ulonglong)dVar26 >> 0x20);
            if (in_FPSCR_SZ == '\0') {
              pdVar14 = pdVar14 + 6;
              *(undefined4 *)pdVar14 = uVar36;
            }
            else {
              pdVar14 = (double *)((int)pdVar14 + 0x2c);
              *pdVar14 = dVar26;
            }
            uVar34 = DAT_8c61e858;
            puVar9 = (ulonglong *)*piVar3;
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)((int)pdVar20 + 4),SUB84(dVar33,0));
              dVar26 = (double)CONCAT44(uVar36,(int)puVar9[2]);
            }
            else {
              dVar32 = *(double *)((int)pdVar20 + 4);
              dVar27 = (double)puVar9[2];
            }
            if (in_FPSCR_PR == '\0') {
              fVar29 = SUB84(dVar26,0) * (float)((ulonglong)dVar32 >> 0x20);
            }
            else {
              fVar29 = SUB84(dVar26 * dVar32,0);
            }
            if (in_FPSCR_SZ == '\0') {
              uVar31 = CONCAT44((int)*puVar9,fVar29);
              dVar32 = (double)CONCAT44(*(undefined4 *)pdVar20,SUB84(dVar32,0));
            }
            else {
              uVar31 = *puVar9;
              dVar32 = *pdVar20;
            }
            if (in_FPSCR_PR == '\0') {
              uVar31 = (ulonglong)
                       (uint)((float)((ulonglong)dVar32 >> 0x20) * (float)(uVar31 >> 0x20) +
                             (float)uVar31);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)(pdVar20 + 1),SUB84(dVar32,0));
              uVar31 = CONCAT44((int)puVar9[4],(int)uVar31);
            }
            else {
              dVar32 = pdVar20[1];
              uVar31 = puVar9[4];
            }
            if (in_FPSCR_PR == '\0') {
              uVar31 = (ulonglong)
                       (uint)((float)((ulonglong)dVar32 >> 0x20) * (float)(uVar31 >> 0x20) +
                             (float)uVar31);
            }
            dVar26 = dVar38;
            dVar33 = dVar38;
            if (in_FPSCR_SZ == '\0') {
              dVar26 = (double)CONCAT44(uVar35,(int)uVar31);
              dVar33 = (double)CONCAT44(uVar35,SUB84(dVar32,0));
            }
            if (in_FPSCR_PR == '\0') {
              dVar26 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) * SUB84(dVar26,0) +
                                        (float)((ulonglong)dVar26 >> 0x20),SUB84(dVar26,0));
            }
            uVar36 = (undefined4)((ulonglong)dVar26 >> 0x20);
            if (in_FPSCR_SZ == '\0') {
              *(undefined4 *)((int)pdVar14 + -4) = uVar36;
              *(undefined4 *)(pdVar14 + -1) = *(undefined4 *)(pdVar20 + 3);
              *(undefined4 *)((int)pdVar14 + -0xc) = *(undefined4 *)((int)pdVar20 + 0x14);
              dVar26 = (double)CONCAT44(uVar36,*(undefined4 *)(pdVar20 + 2));
              pdVar14 = pdVar14 + -2;
              *(undefined4 *)pdVar14 = *(undefined4 *)(pdVar20 + 2);
            }
            else {
              pdVar14[-1] = dVar26;
              pdVar14[-2] = pdVar20[3];
              pdVar14[-3] = *(double *)((int)pdVar20 + 0x14);
              dVar27 = pdVar20[2];
              pdVar14 = pdVar14 + -4;
              *pdVar14 = dVar27;
            }
            *(undefined4 *)((int)pdVar14 + -4) = uVar34;
            piVar3 = DAT_8c61e854;
            if (in_FPSCR_SZ == '\0') {
              pdVar14 = (double *)((int)pdVar14 + 0x34);
              *(int *)pdVar14 = (int)((ulonglong)param_1 >> 0x20);
            }
            else {
              pdVar14 = pdVar14 + 6;
              *pdVar14 = param_1;
            }
            piVar4 = DAT_8c61e854;
            iVar12 = *piVar3;
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)((int)pdVar23 + 4),SUB84(dVar33,0));
              dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),
                                        *(undefined4 *)(iVar12 + 0x14));
            }
            else {
              dVar32 = *(double *)((int)pdVar23 + 4);
              dVar27 = *(double *)(iVar12 + 0x14);
            }
            if (in_FPSCR_PR == '\0') {
              fVar29 = SUB84(dVar26,0) * (float)((ulonglong)dVar32 >> 0x20);
            }
            else {
              fVar29 = SUB84(dVar26 * dVar32,0);
            }
            if (in_FPSCR_SZ == '\0') {
              uVar31 = CONCAT44(*(undefined4 *)(iVar12 + 4),fVar29);
              dVar32 = (double)CONCAT44(*(undefined4 *)pdVar23,SUB84(dVar32,0));
            }
            else {
              uVar31 = *(ulonglong *)(iVar12 + 4);
              dVar32 = *pdVar23;
            }
            if (in_FPSCR_PR == '\0') {
              uVar31 = (ulonglong)
                       (uint)((float)((ulonglong)dVar32 >> 0x20) * (float)(uVar31 >> 0x20) +
                             (float)uVar31);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)(pdVar23 + 1),SUB84(dVar32,0));
              uVar31 = CONCAT44(*(undefined4 *)(iVar12 + 0x24),(int)uVar31);
            }
            else {
              dVar32 = pdVar23[1];
              uVar31 = *(ulonglong *)(iVar12 + 0x24);
            }
            if (in_FPSCR_PR == '\0') {
              uVar31 = (ulonglong)
                       (uint)((float)((ulonglong)dVar32 >> 0x20) * (float)(uVar31 >> 0x20) +
                             (float)uVar31);
            }
            dVar26 = dVar38;
            dVar33 = dVar38;
            if (in_FPSCR_SZ == '\0') {
              dVar26 = (double)CONCAT44(uVar35,SUB84(dVar32,0));
              dVar33 = (double)CONCAT44(uVar35,(int)uVar31);
            }
            if (in_FPSCR_PR == '\0') {
              dVar33 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) * SUB84(dVar33,0) +
                                        (float)((ulonglong)dVar33 >> 0x20),SUB84(dVar33,0));
            }
            uVar36 = (undefined4)((ulonglong)dVar33 >> 0x20);
            if (in_FPSCR_SZ == '\0') {
              pdVar14 = (double *)((int)pdVar14 + -4);
              *(undefined4 *)pdVar14 = uVar36;
            }
            else {
              pdVar14 = pdVar14 + -1;
              *pdVar14 = dVar33;
            }
            uVar34 = DAT_8c61e85c;
            puVar9 = (ulonglong *)*piVar4;
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)((int)pdVar23 + 4),SUB84(dVar26,0));
              dVar33 = (double)CONCAT44(uVar36,(int)puVar9[2]);
            }
            else {
              dVar32 = *(double *)((int)pdVar23 + 4);
              dVar27 = (double)puVar9[2];
            }
            if (in_FPSCR_PR == '\0') {
              fVar29 = SUB84(dVar33,0) * (float)((ulonglong)dVar32 >> 0x20);
            }
            else {
              fVar29 = SUB84(dVar33 * dVar32,0);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)pdVar23,SUB84(dVar32,0));
              uVar31 = CONCAT44((int)*puVar9,fVar29);
            }
            else {
              dVar32 = *pdVar23;
              uVar31 = *puVar9;
            }
            if (in_FPSCR_PR == '\0') {
              uVar31 = (ulonglong)
                       (uint)((float)((ulonglong)dVar32 >> 0x20) * (float)(uVar31 >> 0x20) +
                             (float)uVar31);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)(pdVar23 + 1),SUB84(dVar32,0));
              uVar31 = CONCAT44((int)puVar9[4],(int)uVar31);
            }
            else {
              dVar32 = pdVar23[1];
              uVar31 = puVar9[4];
            }
            if (in_FPSCR_PR == '\0') {
              uVar31 = (ulonglong)
                       (uint)((float)((ulonglong)dVar32 >> 0x20) * (float)(uVar31 >> 0x20) +
                             (float)uVar31);
            }
            dVar33 = dVar38;
            in_dr0 = dVar38;
            if (in_FPSCR_SZ == '\0') {
              dVar33 = (double)CONCAT44(uVar35,(int)uVar31);
              in_dr0 = (double)CONCAT44(uVar35,SUB84(dVar32,0));
            }
            if (in_FPSCR_PR == '\0') {
              dVar33 = (double)CONCAT44((float)((ulonglong)in_dr0 >> 0x20) * SUB84(dVar33,0) +
                                        (float)((ulonglong)dVar33 >> 0x20),SUB84(dVar33,0));
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)pdVar14 + -4) = (int)((ulonglong)dVar33 >> 0x20);
              *(undefined4 *)(pdVar14 + -1) = *(undefined4 *)(pdVar23 + 3);
              *(undefined4 *)((int)pdVar14 + -0xc) = *(undefined4 *)((int)pdVar23 + 0x14);
              pdVar14 = pdVar14 + -2;
              *(undefined4 *)pdVar14 = *(undefined4 *)(pdVar23 + 2);
              in_xd2 = dVar27;
            }
            else {
              pdVar14[-1] = dVar33;
              pdVar14[-2] = pdVar23[3];
              pdVar14[-3] = *(double *)((int)pdVar23 + 0x14);
              in_xd2 = pdVar23[2];
              pdVar14 = pdVar14 + -4;
              *pdVar14 = in_xd2;
            }
            *(undefined4 *)((int)pdVar14 + -4) = uVar34;
            *DAT_8c61e860 = *DAT_8c61e860 + 3;
            unaff_r8 = (int)pdVar14 + 0x1c;
          }
        }
        else {
          *DAT_8c61e864 = 1;
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44(*(undefined4 *)pdVar24,*(undefined4 *)pdVar20);
          }
          else {
            in_xd2 = *pdVar20;
            dVar32 = *pdVar24;
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0),
                                      SUB84(dVar32,0));
          }
          else {
            dVar32 = dVar32 + dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),*(undefined4 *)pdVar23);
          }
          else {
            in_xd2 = *pdVar23;
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0),
                                      SUB84(dVar32,0));
          }
          else {
            dVar32 = dVar32 + dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)puVar25 + -0x10) = (int)((ulonglong)dVar32 >> 0x20);
            dVar32 = (double)CONCAT44(*(undefined4 *)((int)pdVar24 + 4),
                                      *(undefined4 *)((int)pdVar20 + 4));
          }
          else {
            *(double *)((int)puVar25 + -0x10) = dVar32;
            dVar32 = *(double *)((int)pdVar24 + 4);
            in_xd2 = *(double *)((int)pdVar20 + 4);
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0),
                                      SUB84(dVar32,0));
          }
          else {
            dVar32 = dVar32 + dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                      *(undefined4 *)((int)pdVar23 + 4));
          }
          else {
            in_xd2 = *(double *)((int)pdVar23 + 4);
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0),
                                      SUB84(dVar32,0));
          }
          else {
            dVar32 = dVar32 + dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)puVar25 + -0xc) = (int)((ulonglong)dVar32 >> 0x20);
            dVar32 = (double)CONCAT44(*(undefined4 *)(pdVar24 + 1),*(undefined4 *)(pdVar20 + 1));
          }
          else {
            *(double *)((int)puVar25 + -0xc) = dVar32;
            dVar32 = pdVar24[1];
            in_xd2 = pdVar20[1];
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0),
                                      SUB84(dVar32,0));
          }
          else {
            dVar32 = dVar32 + dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),*(undefined4 *)(pdVar23 + 1))
            ;
          }
          else {
            in_xd2 = pdVar23[1];
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0),
                                      SUB84(dVar32,0));
          }
          else {
            dVar32 = dVar32 + dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)puVar25 + -8) = (int)((ulonglong)dVar32 >> 0x20);
          }
          else {
            *(double *)((int)puVar25 + -8) = dVar32;
          }
          dVar32 = (double)(*(code *)PTR_FUN_8c61e844)((undefined1 *)((int)puVar25 + -0x10));
          if (in_FPSCR_SZ == '\0') {
            dVar33 = (double)CONCAT44(*(undefined4 *)DAT_8c61e848,
                                      *(undefined4 *)((int)puVar25 + -0x10));
          }
          else {
            in_xd2 = *(double *)((int)puVar25 + -0x10);
            dVar33 = *DAT_8c61e848;
          }
          if (in_FPSCR_PR == '\0') {
            dVar33 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) * SUB84(dVar33,0),
                                      SUB84(dVar33,0));
          }
          else {
            dVar33 = dVar33 * dVar33;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),*(undefined4 *)DAT_8c61e84c);
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),*DAT_8c61e850);
          }
          else {
            in_xd2 = *DAT_8c61e84c;
          }
          if (in_FPSCR_PR == '\0') {
            fVar29 = SUB84(dVar33,0) - (float)((ulonglong)dVar33 >> 0x20);
          }
          else {
            fVar29 = SUB84(dVar33 - dVar33,0);
          }
          if (in_FPSCR_SZ == '\0') {
            dVar33 = (double)CONCAT44(*(undefined4 *)((int)puVar25 + -0xc),fVar29);
          }
          else {
            dVar33 = *(double *)((int)puVar25 + -0xc);
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                      SUB84(dVar32,0) * (float)((ulonglong)dVar33 >> 0x20));
          }
          else {
            dVar32 = dVar32 * dVar33;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar33 = (double)CONCAT44(*(undefined4 *)((int)puVar25 + -8),SUB84(dVar33,0));
          }
          else {
            dVar33 = *(double *)((int)puVar25 + -8);
          }
          if (in_FPSCR_PR == '\0') {
            dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),
                                      SUB84(dVar33,0) - SUB84(dVar32,0));
          }
          else {
            dVar33 = dVar33 - dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                      *(undefined4 *)(DAT_8c61e848 + 1));
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                      SUB84(dVar32,0) * (float)((ulonglong)dVar33 >> 0x20));
          }
          else {
            dVar32 = dVar32 * dVar33;
          }
          param_1 = in_xd2;
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44(SUB84(dVar33,0),uVar35);
          }
          if (in_FPSCR_PR == '\0') {
            param_1 = (double)((ulonglong)param_1 & 0xffffffff);
          }
          else {
            param_1 = param_1 - dVar32;
          }
          in_dr0 = (double)(*(code *)PTR_FUN_8c61e868)(pdVar24,pdVar20,pdVar23);
        }
      }
    }
  }
  else if (uVar7 == (int)DAT_8c61e55e) {
    *DAT_8c61e86c = *DAT_8c61e86c + (uint)*(ushort *)(*(int *)((int)puVar25 + -0x1c) + 2) * 4;
    for (iVar15 = 0; puVar6 = PTR_FUN_8c61ecb4, uVar35 = SUB84(param_1,0),
        iVar15 < (int)(uint)*(ushort *)(*(int *)((int)puVar25 + -0x1c) + 2); iVar15 = iVar15 + 1) {
      uVar1 = *puVar17;
      puVar11 = puVar17 + 1;
      puVar18 = puVar17 + 2;
      iVar12 = *DAT_8c61eb34;
      puVar19 = puVar17 + 3;
      puVar17 = puVar17 + 4;
      pdVar20 = (double *)((uint)uVar1 * 0x20 + iVar12);
      pdVar24 = (double *)((uint)*puVar11 * 0x20 + iVar12);
      pdVar23 = (double *)((uint)*puVar18 * 0x20 + iVar12);
      puVar22 = (undefined8 *)((uint)*puVar19 * 0x20 + iVar12);
      if ((*(uint *)((int)pdVar20 + 0xc) & *(uint *)((int)pdVar24 + 0xc) &
           *(uint *)((int)pdVar23 + 0xc) & *(uint *)((int)puVar22 + 0xc) & 0x3f) == 0) {
        if (((*(uint *)((int)pdVar20 + 0xc) | *(uint *)((int)pdVar24 + 0xc) |
              *(uint *)((int)pdVar23 + 0xc) | *(uint *)((int)puVar22 + 0xc)) & 1) == 0) {
          if (in_FPSCR_SZ == '\0') {
            uVar35 = *(undefined4 *)(pdVar24 + 2);
            dVar32 = (double)CONCAT44(uVar35,*(undefined4 *)(pdVar20 + 2));
          }
          else {
            dVar32 = pdVar24[2];
            in_xd2 = pdVar20[2];
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) - SUB84(dVar32,0),
                                      SUB84(dVar32,0));
          }
          else {
            dVar32 = dVar32 - dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                      *(undefined4 *)((int)pdVar23 + 0x14));
            dVar33 = (double)CONCAT44(*(undefined4 *)((int)pdVar24 + 0x14),uVar35);
          }
          else {
            in_xd2 = *(double *)((int)pdVar23 + 0x14);
            dVar33 = *(double *)((int)pdVar24 + 0x14);
          }
          fVar29 = (float)((ulonglong)dVar33 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),SUB84(dVar32,0) - fVar29);
          }
          else {
            dVar32 = dVar32 - dVar33;
          }
          if (in_FPSCR_SZ == '\0') {
            in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar29);
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) * SUB84(dVar32,0),
                                      SUB84(dVar32,0));
          }
          else {
            dVar32 = dVar32 * dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                      *(undefined4 *)((int)pdVar20 + 0x14));
          }
          else {
            in_xd2 = *(double *)((int)pdVar20 + 0x14);
          }
          if (in_FPSCR_PR == '\0') {
            dVar27 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                      SUB84(in_dr0,0) - SUB84(dVar32,0));
          }
          else {
            dVar27 = in_dr0 - dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),*(undefined4 *)(pdVar23 + 2))
            ;
          }
          else {
            in_xd2 = pdVar23[2];
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                      SUB84(dVar32,0) - SUB84(dVar33,0));
          }
          else {
            dVar32 = dVar32 - dVar33;
          }
          uVar7 = (uint)((ulonglong)dVar32 >> 0x20);
          param_1 = dVar32;
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44(uVar7,SUB84(dVar33,0));
          }
          if (in_FPSCR_PR == '\0') {
            in_dr0 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),
                                      SUB84(dVar27,0) * SUB84(dVar32,0));
          }
          else {
            in_dr0 = dVar27 * dVar32;
          }
          if (in_FPSCR_PR == '\0') {
            fVar29 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
            param_1 = (double)CONCAT44(fVar29,SUB84(param_1,0));
            bVar39 = 0.0 < fVar29;
          }
          else {
            param_1 = param_1 - in_dr0;
            bVar39 = (double)((ulonglong)uVar7 << 0x20) < param_1;
          }
          uVar35 = SUB84(param_1,0);
          if (!bVar39) {
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)pdVar20,*(undefined4 *)pdVar24);
            }
            else {
              dVar32 = *pdVar20;
              in_xd2 = *pdVar24;
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0),
                                        SUB84(dVar32,0));
            }
            else {
              dVar32 = dVar32 + dVar32;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),*(undefined4 *)pdVar23);
            }
            else {
              in_xd2 = *pdVar23;
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0),
                                        SUB84(dVar32,0));
            }
            else {
              dVar32 = dVar32 + dVar32;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)puVar25 + -0x10) = (int)((ulonglong)dVar32 >> 0x20);
              dVar32 = (double)CONCAT44(*(undefined4 *)((int)pdVar20 + 4),
                                        *(undefined4 *)((int)pdVar24 + 4));
            }
            else {
              *(double *)((int)puVar25 + -0x10) = dVar32;
              dVar32 = *(double *)((int)pdVar20 + 4);
              in_xd2 = *(double *)((int)pdVar24 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0),
                                        SUB84(dVar32,0));
            }
            else {
              dVar32 = dVar32 + dVar32;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                        *(undefined4 *)((int)pdVar23 + 4));
            }
            else {
              in_xd2 = *(double *)((int)pdVar23 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0),
                                        SUB84(dVar32,0));
            }
            else {
              dVar32 = dVar32 + dVar32;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)puVar25 + -0xc) = (int)((ulonglong)dVar32 >> 0x20);
              dVar32 = (double)CONCAT44(*(undefined4 *)(pdVar20 + 1),*(undefined4 *)(pdVar24 + 1));
            }
            else {
              *(double *)((int)puVar25 + -0xc) = dVar32;
              dVar32 = pdVar20[1];
              in_xd2 = pdVar24[1];
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0),
                                        SUB84(dVar32,0));
            }
            else {
              dVar32 = dVar32 + dVar32;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                        *(undefined4 *)(pdVar23 + 1));
            }
            else {
              in_xd2 = pdVar23[1];
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0),
                                        SUB84(dVar32,0));
            }
            else {
              dVar32 = dVar32 + dVar32;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)puVar25 + -8) = (int)((ulonglong)dVar32 >> 0x20);
            }
            else {
              *(double *)((int)puVar25 + -8) = dVar32;
            }
            dVar32 = (double)(*(code *)PTR_FUN_8c61eb38)((undefined1 *)((int)puVar25 + -0x10));
            piVar3 = DAT_8c61eb48;
            if (in_FPSCR_SZ == '\0') {
              dVar33 = (double)CONCAT44(*(undefined4 *)DAT_8c61eb3c,
                                        *(undefined4 *)((int)puVar25 + -0x10));
            }
            else {
              in_xd2 = *(double *)((int)puVar25 + -0x10);
              dVar33 = *DAT_8c61eb3c;
            }
            if (in_FPSCR_PR == '\0') {
              dVar33 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) * SUB84(dVar33,0),
                                        SUB84(dVar33,0));
            }
            else {
              dVar33 = dVar33 * dVar33;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),*(undefined4 *)DAT_8c61eb40
                                       );
              dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),*DAT_8c61eb44);
            }
            else {
              in_xd2 = *DAT_8c61eb40;
            }
            if (in_FPSCR_PR == '\0') {
              fVar29 = SUB84(dVar33,0) - (float)((ulonglong)dVar33 >> 0x20);
            }
            else {
              fVar29 = SUB84(dVar33 - dVar33,0);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar33 = (double)CONCAT44(*(undefined4 *)((int)puVar25 + -0xc),fVar29);
            }
            else {
              dVar33 = *(double *)((int)puVar25 + -0xc);
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                        SUB84(dVar32,0) * (float)((ulonglong)dVar33 >> 0x20));
            }
            else {
              dVar32 = dVar32 * dVar33;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar33 = (double)CONCAT44(*(undefined4 *)((int)puVar25 + -8),SUB84(dVar33,0));
            }
            else {
              dVar33 = *(double *)((int)puVar25 + -8);
            }
            iVar12 = *DAT_8c61eb48;
            if (in_FPSCR_PR == '\0') {
              dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),
                                        SUB84(dVar33,0) - SUB84(dVar32,0));
            }
            else {
              dVar33 = dVar33 - dVar32;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)((int)pdVar20 + 4),
                                        *(undefined4 *)(DAT_8c61eb3c + 1));
            }
            else {
              dVar32 = *(double *)((int)pdVar20 + 4);
            }
            fVar29 = (float)((ulonglong)dVar33 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),SUB84(dVar32,0) * fVar29);
            }
            else {
              dVar32 = dVar32 * dVar33;
            }
            if (in_FPSCR_SZ == '\0') {
              uVar36 = SUB84(dVar33,0);
              dVar33 = (double)CONCAT44(fVar29,*(undefined4 *)(iVar12 + 0x14));
              dVar27 = in_xd2;
              in_xd2 = (double)CONCAT44(uVar36,uVar35);
            }
            else {
              dVar27 = *(double *)(iVar12 + 0x14);
            }
            if (in_FPSCR_PR == '\0') {
              param_1 = (double)CONCAT44((float)((ulonglong)in_xd2 >> 0x20) - SUB84(dVar32,0),
                                         SUB84(in_xd2,0));
              fVar29 = SUB84(dVar33,0) * (float)((ulonglong)dVar32 >> 0x20);
            }
            else {
              param_1 = in_xd2 - dVar32;
              fVar29 = SUB84(dVar33 * dVar32,0);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)pdVar20,SUB84(dVar32,0));
              uVar31 = CONCAT44(*(undefined4 *)(iVar12 + 4),fVar29);
            }
            else {
              dVar32 = *pdVar20;
              uVar31 = *(ulonglong *)(iVar12 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              uVar31 = (ulonglong)
                       (uint)((float)((ulonglong)dVar32 >> 0x20) * (float)(uVar31 >> 0x20) +
                             (float)uVar31);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)(pdVar20 + 1),SUB84(dVar32,0));
              uVar31 = CONCAT44(*(undefined4 *)(iVar12 + 0x24),(int)uVar31);
            }
            else {
              dVar32 = pdVar20[1];
              uVar31 = *(ulonglong *)(iVar12 + 0x24);
            }
            if (in_FPSCR_PR == '\0') {
              uVar31 = (ulonglong)
                       (uint)((float)((ulonglong)dVar32 >> 0x20) * (float)(uVar31 >> 0x20) +
                             (float)uVar31);
            }
            uVar35 = (undefined4)((ulonglong)dVar38 >> 0x20);
            dVar26 = dVar38;
            dVar33 = dVar38;
            if (in_FPSCR_SZ == '\0') {
              dVar26 = (double)CONCAT44(uVar35,(int)uVar31);
              dVar33 = (double)CONCAT44(uVar35,SUB84(dVar32,0));
            }
            if (in_FPSCR_PR == '\0') {
              dVar26 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) * SUB84(dVar26,0) +
                                        (float)((ulonglong)dVar26 >> 0x20),SUB84(dVar26,0));
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar14 = (double *)(unaff_r8 + 0x14);
              *(int *)pdVar14 = (int)((ulonglong)dVar26 >> 0x20);
            }
            else {
              pdVar14 = (double *)(unaff_r8 + 0x10);
              *pdVar14 = dVar26;
            }
            pdVar8 = (double *)*piVar3;
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)((int)pdVar20 + 4),SUB84(dVar33,0));
              dVar33 = (double)CONCAT44(*(undefined4 *)pdVar8,*(undefined4 *)(pdVar8 + 2));
            }
            else {
              dVar32 = *(double *)((int)pdVar20 + 4);
              dVar27 = pdVar8[2];
              dVar33 = *pdVar8;
            }
            if (in_FPSCR_PR == '\0') {
              dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),
                                        SUB84(dVar33,0) * (float)((ulonglong)dVar32 >> 0x20));
            }
            else {
              dVar33 = dVar33 * dVar32;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)pdVar20,SUB84(dVar32,0));
            }
            else {
              dVar32 = *pdVar20;
            }
            if (in_FPSCR_PR == '\0') {
              dVar33 = (double)(ulonglong)
                               (uint)((float)((ulonglong)dVar32 >> 0x20) *
                                      (float)((ulonglong)dVar33 >> 0x20) + SUB84(dVar33,0));
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)(pdVar20 + 1),SUB84(dVar32,0));
              dVar33 = (double)CONCAT44(*(undefined4 *)(pdVar8 + 4),SUB84(dVar33,0));
            }
            else {
              dVar32 = pdVar20[1];
              dVar33 = pdVar8[4];
            }
            if (in_FPSCR_PR == '\0') {
              dVar33 = (double)(ulonglong)
                               (uint)((float)((ulonglong)dVar32 >> 0x20) *
                                      (float)((ulonglong)dVar33 >> 0x20) + SUB84(dVar33,0));
            }
            dVar30 = dVar38;
            dVar26 = dVar38;
            if (in_FPSCR_SZ == '\0') {
              dVar30 = (double)CONCAT44(uVar35,SUB84(dVar33,0));
              dVar26 = (double)CONCAT44(uVar35,SUB84(dVar32,0));
            }
            if (in_FPSCR_PR == '\0') {
              dVar30 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) * SUB84(dVar30,0) +
                                        (float)((ulonglong)dVar30 >> 0x20),SUB84(dVar30,0));
            }
            uVar36 = (undefined4)((ulonglong)dVar30 >> 0x20);
            if (in_FPSCR_SZ == '\0') {
              *(undefined4 *)((int)pdVar14 + -4) = uVar36;
              uVar34 = DAT_8c61eb4c;
              *(undefined4 *)(pdVar14 + -1) = *(undefined4 *)(pdVar20 + 3);
              *(undefined4 *)((int)pdVar14 + -0xc) = *(undefined4 *)((int)pdVar20 + 0x14);
              dVar30 = (double)CONCAT44(uVar36,*(undefined4 *)(pdVar20 + 2));
              pdVar14 = pdVar14 + -2;
              *(undefined4 *)pdVar14 = *(undefined4 *)(pdVar20 + 2);
            }
            else {
              pdVar14[-1] = dVar30;
              uVar34 = DAT_8c61eb4c;
              pdVar14[-2] = pdVar20[3];
              pdVar14[-3] = *(double *)((int)pdVar20 + 0x14);
              dVar27 = pdVar20[2];
              pdVar14 = pdVar14 + -4;
              *pdVar14 = dVar27;
            }
            *(undefined4 *)((int)pdVar14 + -4) = uVar34;
            piVar3 = DAT_8c61eb48;
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)((int)pdVar24 + 4),SUB84(dVar26,0));
            }
            else {
              dVar32 = *(double *)((int)pdVar24 + 4);
            }
            iVar12 = *DAT_8c61eb48;
            if (in_FPSCR_SZ == '\0') {
              dVar30 = (double)CONCAT44((int)((ulonglong)dVar30 >> 0x20),
                                        *(undefined4 *)(iVar12 + 0x14));
            }
            else {
              dVar27 = *(double *)(iVar12 + 0x14);
            }
            if (in_FPSCR_PR == '\0') {
              fVar29 = SUB84(dVar30,0) * (float)((ulonglong)dVar32 >> 0x20);
            }
            else {
              fVar29 = SUB84(dVar30 * dVar32,0);
            }
            if (in_FPSCR_SZ == '\0') {
              uVar31 = CONCAT44(*(undefined4 *)(iVar12 + 4),fVar29);
              dVar32 = (double)CONCAT44(*(undefined4 *)pdVar24,SUB84(dVar32,0));
            }
            else {
              uVar31 = *(ulonglong *)(iVar12 + 4);
              dVar32 = *pdVar24;
            }
            if (in_FPSCR_PR == '\0') {
              uVar31 = (ulonglong)
                       (uint)((float)((ulonglong)dVar32 >> 0x20) * (float)(uVar31 >> 0x20) +
                             (float)uVar31);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)(pdVar24 + 1),SUB84(dVar32,0));
              uVar31 = CONCAT44(*(undefined4 *)(iVar12 + 0x24),(int)uVar31);
            }
            else {
              dVar32 = pdVar24[1];
              uVar31 = *(ulonglong *)(iVar12 + 0x24);
            }
            if (in_FPSCR_PR == '\0') {
              uVar31 = (ulonglong)
                       (uint)((float)((ulonglong)dVar32 >> 0x20) * (float)(uVar31 >> 0x20) +
                             (float)uVar31);
            }
            dVar26 = dVar38;
            dVar33 = dVar38;
            if (in_FPSCR_SZ == '\0') {
              dVar26 = (double)CONCAT44(uVar35,(int)uVar31);
              dVar33 = (double)CONCAT44(uVar35,SUB84(dVar32,0));
            }
            if (in_FPSCR_PR == '\0') {
              dVar26 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) * SUB84(dVar26,0) +
                                        (float)((ulonglong)dVar26 >> 0x20),SUB84(dVar26,0));
            }
            uVar36 = (undefined4)((ulonglong)dVar26 >> 0x20);
            if (in_FPSCR_SZ == '\0') {
              pdVar14 = pdVar14 + 6;
              *(undefined4 *)pdVar14 = uVar36;
            }
            else {
              pdVar14 = (double *)((int)pdVar14 + 0x2c);
              *pdVar14 = dVar26;
            }
            uVar34 = DAT_8c61eb4c;
            puVar9 = (ulonglong *)*piVar3;
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)((int)pdVar24 + 4),SUB84(dVar33,0));
              dVar26 = (double)CONCAT44(uVar36,(int)puVar9[2]);
            }
            else {
              dVar32 = *(double *)((int)pdVar24 + 4);
              dVar27 = (double)puVar9[2];
            }
            if (in_FPSCR_PR == '\0') {
              fVar29 = SUB84(dVar26,0) * (float)((ulonglong)dVar32 >> 0x20);
            }
            else {
              fVar29 = SUB84(dVar26 * dVar32,0);
            }
            if (in_FPSCR_SZ == '\0') {
              uVar31 = CONCAT44((int)*puVar9,fVar29);
              dVar32 = (double)CONCAT44(*(undefined4 *)pdVar24,SUB84(dVar32,0));
            }
            else {
              uVar31 = *puVar9;
              dVar32 = *pdVar24;
            }
            if (in_FPSCR_PR == '\0') {
              uVar31 = (ulonglong)
                       (uint)((float)((ulonglong)dVar32 >> 0x20) * (float)(uVar31 >> 0x20) +
                             (float)uVar31);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)(pdVar24 + 1),SUB84(dVar32,0));
              uVar31 = CONCAT44((int)puVar9[4],(int)uVar31);
            }
            else {
              dVar32 = pdVar24[1];
              uVar31 = puVar9[4];
            }
            if (in_FPSCR_PR == '\0') {
              uVar31 = (ulonglong)
                       (uint)((float)((ulonglong)dVar32 >> 0x20) * (float)(uVar31 >> 0x20) +
                             (float)uVar31);
            }
            dVar26 = dVar38;
            dVar33 = dVar38;
            if (in_FPSCR_SZ == '\0') {
              dVar26 = (double)CONCAT44(uVar35,(int)uVar31);
              dVar33 = (double)CONCAT44(uVar35,SUB84(dVar32,0));
            }
            if (in_FPSCR_PR == '\0') {
              dVar26 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) * SUB84(dVar26,0) +
                                        (float)((ulonglong)dVar26 >> 0x20),SUB84(dVar26,0));
            }
            uVar36 = (undefined4)((ulonglong)dVar26 >> 0x20);
            if (in_FPSCR_SZ == '\0') {
              *(undefined4 *)((int)pdVar14 + -4) = uVar36;
              *(undefined4 *)(pdVar14 + -1) = *(undefined4 *)(pdVar24 + 3);
              *(undefined4 *)((int)pdVar14 + -0xc) = *(undefined4 *)((int)pdVar24 + 0x14);
              dVar26 = (double)CONCAT44(uVar36,*(undefined4 *)(pdVar24 + 2));
              pdVar14 = pdVar14 + -2;
              *(undefined4 *)pdVar14 = *(undefined4 *)(pdVar24 + 2);
            }
            else {
              pdVar14[-1] = dVar26;
              pdVar14[-2] = pdVar24[3];
              pdVar14[-3] = *(double *)((int)pdVar24 + 0x14);
              dVar27 = pdVar24[2];
              pdVar14 = pdVar14 + -4;
              *pdVar14 = dVar27;
            }
            *(undefined4 *)((int)pdVar14 + -4) = uVar34;
            piVar3 = DAT_8c61eb48;
            uVar36 = (undefined4)((ulonglong)param_1 >> 0x20);
            if (in_FPSCR_SZ == '\0') {
              pdVar14 = (double *)((int)pdVar14 + 0x34);
              *(undefined4 *)pdVar14 = uVar36;
            }
            else {
              pdVar14 = pdVar14 + 6;
              *pdVar14 = param_1;
            }
            piVar4 = DAT_8c61eb48;
            iVar12 = *piVar3;
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)((int)pdVar23 + 4),SUB84(dVar33,0));
              dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),
                                        *(undefined4 *)(iVar12 + 0x14));
            }
            else {
              dVar32 = *(double *)((int)pdVar23 + 4);
              dVar27 = *(double *)(iVar12 + 0x14);
            }
            if (in_FPSCR_PR == '\0') {
              fVar29 = SUB84(dVar26,0) * (float)((ulonglong)dVar32 >> 0x20);
            }
            else {
              fVar29 = SUB84(dVar26 * dVar32,0);
            }
            if (in_FPSCR_SZ == '\0') {
              uVar31 = CONCAT44(*(undefined4 *)(iVar12 + 4),fVar29);
              dVar32 = (double)CONCAT44(*(undefined4 *)pdVar23,SUB84(dVar32,0));
            }
            else {
              uVar31 = *(ulonglong *)(iVar12 + 4);
              dVar32 = *pdVar23;
            }
            if (in_FPSCR_PR == '\0') {
              uVar31 = (ulonglong)
                       (uint)((float)((ulonglong)dVar32 >> 0x20) * (float)(uVar31 >> 0x20) +
                             (float)uVar31);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)(pdVar23 + 1),SUB84(dVar32,0));
              uVar31 = CONCAT44(*(undefined4 *)(iVar12 + 0x24),(int)uVar31);
            }
            else {
              dVar32 = pdVar23[1];
              uVar31 = *(ulonglong *)(iVar12 + 0x24);
            }
            if (in_FPSCR_PR == '\0') {
              uVar31 = (ulonglong)
                       (uint)((float)((ulonglong)dVar32 >> 0x20) * (float)(uVar31 >> 0x20) +
                             (float)uVar31);
            }
            dVar26 = dVar38;
            dVar33 = dVar38;
            if (in_FPSCR_SZ == '\0') {
              dVar26 = (double)CONCAT44(uVar35,(int)uVar31);
              dVar33 = (double)CONCAT44(uVar35,SUB84(dVar32,0));
            }
            if (in_FPSCR_PR == '\0') {
              dVar26 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) * SUB84(dVar26,0) +
                                        (float)((ulonglong)dVar26 >> 0x20),SUB84(dVar26,0));
            }
            uVar34 = (undefined4)((ulonglong)dVar26 >> 0x20);
            if (in_FPSCR_SZ == '\0') {
              pdVar14 = (double *)((int)pdVar14 + -4);
              *(undefined4 *)pdVar14 = uVar34;
            }
            else {
              pdVar14 = pdVar14 + -1;
              *pdVar14 = dVar26;
            }
            uVar5 = DAT_8c61eb4c;
            puVar9 = (ulonglong *)*piVar4;
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)((int)pdVar23 + 4),SUB84(dVar33,0));
              dVar26 = (double)CONCAT44(uVar34,(int)puVar9[2]);
            }
            else {
              dVar32 = *(double *)((int)pdVar23 + 4);
              dVar27 = (double)puVar9[2];
            }
            if (in_FPSCR_PR == '\0') {
              fVar29 = SUB84(dVar26,0) * (float)((ulonglong)dVar32 >> 0x20);
            }
            else {
              fVar29 = SUB84(dVar26 * dVar32,0);
            }
            if (in_FPSCR_SZ == '\0') {
              uVar31 = CONCAT44((int)*puVar9,fVar29);
              dVar32 = (double)CONCAT44(*(undefined4 *)pdVar23,SUB84(dVar32,0));
            }
            else {
              uVar31 = *puVar9;
              dVar32 = *pdVar23;
            }
            if (in_FPSCR_PR == '\0') {
              uVar31 = (ulonglong)
                       (uint)((float)((ulonglong)dVar32 >> 0x20) * (float)(uVar31 >> 0x20) +
                             (float)uVar31);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)(pdVar23 + 1),SUB84(dVar32,0));
              uVar31 = CONCAT44((int)puVar9[4],(int)uVar31);
            }
            else {
              dVar32 = pdVar23[1];
              uVar31 = puVar9[4];
            }
            if (in_FPSCR_PR == '\0') {
              uVar31 = (ulonglong)
                       (uint)((float)((ulonglong)dVar32 >> 0x20) * (float)(uVar31 >> 0x20) +
                             (float)uVar31);
            }
            dVar26 = dVar38;
            dVar33 = dVar38;
            if (in_FPSCR_SZ == '\0') {
              dVar26 = (double)CONCAT44(uVar35,(int)uVar31);
              dVar33 = (double)CONCAT44(uVar35,SUB84(dVar32,0));
            }
            if (in_FPSCR_PR == '\0') {
              dVar26 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) * SUB84(dVar26,0) +
                                        (float)((ulonglong)dVar26 >> 0x20),SUB84(dVar26,0));
            }
            uVar34 = (undefined4)((ulonglong)dVar26 >> 0x20);
            if (in_FPSCR_SZ == '\0') {
              *(undefined4 *)((int)pdVar14 + -4) = uVar34;
              *(undefined4 *)(pdVar14 + -1) = *(undefined4 *)(pdVar23 + 3);
              *(undefined4 *)((int)pdVar14 + -0xc) = *(undefined4 *)((int)pdVar23 + 0x14);
              dVar26 = (double)CONCAT44(uVar34,*(undefined4 *)(pdVar23 + 2));
              pdVar14 = pdVar14 + -2;
              *(undefined4 *)pdVar14 = *(undefined4 *)(pdVar23 + 2);
            }
            else {
              pdVar14[-1] = dVar26;
              pdVar14[-2] = pdVar23[3];
              pdVar14[-3] = *(double *)((int)pdVar23 + 0x14);
              dVar27 = pdVar23[2];
              pdVar14 = pdVar14 + -4;
              *pdVar14 = dVar27;
            }
            *(undefined4 *)((int)pdVar14 + -4) = uVar5;
            piVar3 = DAT_8c61eb48;
            if (in_FPSCR_SZ == '\0') {
              pdVar14 = (double *)((int)pdVar14 + 0x34);
              *(undefined4 *)pdVar14 = uVar36;
            }
            else {
              pdVar14 = pdVar14 + 6;
              *pdVar14 = param_1;
            }
            piVar4 = DAT_8c61eb48;
            iVar12 = *piVar3;
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)((int)puVar22 + 4),SUB84(dVar33,0));
              dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),
                                        *(undefined4 *)(iVar12 + 0x14));
            }
            else {
              dVar32 = *(double *)((int)puVar22 + 4);
              dVar27 = *(double *)(iVar12 + 0x14);
            }
            if (in_FPSCR_PR == '\0') {
              fVar29 = SUB84(dVar26,0) * (float)((ulonglong)dVar32 >> 0x20);
            }
            else {
              fVar29 = SUB84(dVar26 * dVar32,0);
            }
            if (in_FPSCR_SZ == '\0') {
              uVar31 = CONCAT44(*(undefined4 *)(iVar12 + 4),fVar29);
              uVar28 = CONCAT44(*(undefined4 *)puVar22,SUB84(dVar32,0));
            }
            else {
              uVar31 = *(ulonglong *)(iVar12 + 4);
              uVar28 = *puVar22;
            }
            if (in_FPSCR_PR == '\0') {
              uVar31 = (ulonglong)
                       (uint)((float)((ulonglong)uVar28 >> 0x20) * (float)(uVar31 >> 0x20) +
                             (float)uVar31);
            }
            if (in_FPSCR_SZ == '\0') {
              uVar28 = CONCAT44(*(undefined4 *)(puVar22 + 1),(int)uVar28);
              uVar31 = CONCAT44(*(undefined4 *)(iVar12 + 0x24),(int)uVar31);
            }
            else {
              uVar28 = puVar22[1];
              uVar31 = *(ulonglong *)(iVar12 + 0x24);
            }
            if (in_FPSCR_PR == '\0') {
              uVar31 = (ulonglong)
                       (uint)((float)((ulonglong)uVar28 >> 0x20) * (float)(uVar31 >> 0x20) +
                             (float)uVar31);
            }
            dVar33 = dVar38;
            dVar32 = dVar38;
            if (in_FPSCR_SZ == '\0') {
              dVar33 = (double)CONCAT44(uVar35,(int)uVar31);
              dVar32 = (double)CONCAT44(uVar35,(int)uVar28);
            }
            if (in_FPSCR_PR == '\0') {
              dVar33 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) * SUB84(dVar33,0) +
                                        (float)((ulonglong)dVar33 >> 0x20),SUB84(dVar33,0));
            }
            uVar36 = (undefined4)((ulonglong)dVar33 >> 0x20);
            if (in_FPSCR_SZ == '\0') {
              pdVar14 = (double *)((int)pdVar14 + -4);
              *(undefined4 *)pdVar14 = uVar36;
            }
            else {
              pdVar14 = pdVar14 + -1;
              *pdVar14 = dVar33;
            }
            uVar34 = DAT_8c61eb50;
            puVar9 = (ulonglong *)*piVar4;
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)((int)puVar22 + 4),SUB84(dVar32,0));
              dVar33 = (double)CONCAT44(uVar36,(int)puVar9[2]);
            }
            else {
              dVar32 = *(double *)((int)puVar22 + 4);
              dVar27 = (double)puVar9[2];
            }
            if (in_FPSCR_PR == '\0') {
              fVar29 = SUB84(dVar33,0) * (float)((ulonglong)dVar32 >> 0x20);
            }
            else {
              fVar29 = SUB84(dVar33 * dVar32,0);
            }
            if (in_FPSCR_SZ == '\0') {
              uVar31 = CONCAT44((int)*puVar9,fVar29);
              uVar28 = CONCAT44(*(undefined4 *)puVar22,SUB84(dVar32,0));
            }
            else {
              uVar31 = *puVar9;
              uVar28 = *puVar22;
            }
            if (in_FPSCR_PR == '\0') {
              uVar31 = (ulonglong)
                       (uint)((float)((ulonglong)uVar28 >> 0x20) * (float)(uVar31 >> 0x20) +
                             (float)uVar31);
            }
            if (in_FPSCR_SZ == '\0') {
              uVar28 = CONCAT44(*(undefined4 *)(puVar22 + 1),(int)uVar28);
              uVar31 = CONCAT44((int)puVar9[4],(int)uVar31);
            }
            else {
              uVar28 = puVar22[1];
              uVar31 = puVar9[4];
            }
            if (in_FPSCR_PR == '\0') {
              uVar31 = (ulonglong)
                       (uint)((float)((ulonglong)uVar28 >> 0x20) * (float)(uVar31 >> 0x20) +
                             (float)uVar31);
            }
            dVar32 = dVar38;
            in_dr0 = dVar38;
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(uVar35,(int)uVar31);
              in_dr0 = (double)CONCAT44(uVar35,(int)uVar28);
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((float)((ulonglong)in_dr0 >> 0x20) * SUB84(dVar32,0) +
                                        (float)((ulonglong)dVar32 >> 0x20),SUB84(dVar32,0));
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)pdVar14 + -4) = (int)((ulonglong)dVar32 >> 0x20);
              *(undefined4 *)(pdVar14 + -1) = *(undefined4 *)(puVar22 + 3);
              *(undefined4 *)((int)pdVar14 + -0xc) = *(undefined4 *)((int)puVar22 + 0x14);
              pdVar14 = pdVar14 + -2;
              *(undefined4 *)pdVar14 = *(undefined4 *)(puVar22 + 2);
              in_xd2 = dVar27;
            }
            else {
              pdVar14[-1] = dVar32;
              pdVar14[-2] = (double)puVar22[3];
              pdVar14[-3] = *(double *)((int)puVar22 + 0x14);
              in_xd2 = (double)puVar22[2];
              pdVar14 = pdVar14 + -4;
              *pdVar14 = in_xd2;
            }
            *(undefined4 *)((int)pdVar14 + -4) = uVar34;
            *DAT_8c61eb54 = *DAT_8c61eb54 + 4;
            unaff_r8 = (int)pdVar14 + 0x1c;
          }
        }
        else {
          *DAT_8c61ecb0 = 1;
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44(*(undefined4 *)pdVar20,*(undefined4 *)pdVar24);
          }
          else {
            in_xd2 = *pdVar24;
            dVar32 = *pdVar20;
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0),
                                      SUB84(dVar32,0));
          }
          else {
            dVar32 = dVar32 + dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),*(undefined4 *)pdVar23);
          }
          else {
            in_xd2 = *pdVar23;
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0),
                                      SUB84(dVar32,0));
          }
          else {
            dVar32 = dVar32 + dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)puVar25 + -0x10) = (int)((ulonglong)dVar32 >> 0x20);
            dVar32 = (double)CONCAT44(*(undefined4 *)((int)pdVar20 + 4),
                                      *(undefined4 *)((int)pdVar24 + 4));
          }
          else {
            *(double *)((int)puVar25 + -0x10) = dVar32;
            in_xd2 = *(double *)((int)pdVar24 + 4);
            dVar32 = *(double *)((int)pdVar20 + 4);
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0),
                                      SUB84(dVar32,0));
          }
          else {
            dVar32 = dVar32 + dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                      *(undefined4 *)((int)pdVar23 + 4));
          }
          else {
            in_xd2 = *(double *)((int)pdVar23 + 4);
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0),
                                      SUB84(dVar32,0));
          }
          else {
            dVar32 = dVar32 + dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)puVar25 + -0xc) = (int)((ulonglong)dVar32 >> 0x20);
            dVar32 = (double)CONCAT44(*(undefined4 *)(pdVar20 + 1),*(undefined4 *)(pdVar24 + 1));
          }
          else {
            *(double *)((int)puVar25 + -0xc) = dVar32;
            dVar32 = pdVar20[1];
            in_xd2 = pdVar24[1];
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0),
                                      SUB84(dVar32,0));
          }
          else {
            dVar32 = dVar32 + dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),*(undefined4 *)(pdVar23 + 1))
            ;
          }
          else {
            in_xd2 = pdVar23[1];
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0),
                                      SUB84(dVar32,0));
          }
          else {
            dVar32 = dVar32 + dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)puVar25 + -8) = (int)((ulonglong)dVar32 >> 0x20);
          }
          else {
            *(double *)((int)puVar25 + -8) = dVar32;
          }
          dVar32 = (double)(*(code *)puVar6)((undefined1 *)((int)puVar25 + -0x10));
          if (in_FPSCR_SZ == '\0') {
            dVar33 = (double)CONCAT44(*(undefined4 *)DAT_8c61ecb8,
                                      *(undefined4 *)((int)puVar25 + -0x10));
          }
          else {
            in_xd2 = *(double *)((int)puVar25 + -0x10);
            dVar33 = *DAT_8c61ecb8;
          }
          if (in_FPSCR_PR == '\0') {
            dVar33 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) * SUB84(dVar33,0),
                                      SUB84(dVar33,0));
          }
          else {
            dVar33 = dVar33 * dVar33;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),*(undefined4 *)DAT_8c61ecbc);
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),*DAT_8c61ecc0);
          }
          else {
            in_xd2 = *DAT_8c61ecbc;
          }
          if (in_FPSCR_PR == '\0') {
            fVar29 = SUB84(dVar33,0) - (float)((ulonglong)dVar33 >> 0x20);
          }
          else {
            fVar29 = SUB84(dVar33 - dVar33,0);
          }
          if (in_FPSCR_SZ == '\0') {
            dVar33 = (double)CONCAT44(*(undefined4 *)((int)puVar25 + -0xc),fVar29);
          }
          else {
            dVar33 = *(double *)((int)puVar25 + -0xc);
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                      SUB84(dVar32,0) * (float)((ulonglong)dVar33 >> 0x20));
          }
          else {
            dVar32 = dVar32 * dVar33;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar33 = (double)CONCAT44(*(undefined4 *)((int)puVar25 + -8),SUB84(dVar33,0));
          }
          else {
            dVar33 = *(double *)((int)puVar25 + -8);
          }
          if (in_FPSCR_PR == '\0') {
            dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),
                                      SUB84(dVar33,0) - SUB84(dVar32,0));
          }
          else {
            dVar33 = dVar33 - dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                      *(undefined4 *)(DAT_8c61ecb8 + 1));
          }
          if (in_FPSCR_PR == '\0') {
            in_dr0 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                      SUB84(dVar32,0) * (float)((ulonglong)dVar33 >> 0x20));
          }
          else {
            in_dr0 = dVar32 * dVar33;
          }
          dVar32 = in_xd2;
          if (in_FPSCR_SZ == '\0') {
            dVar38 = (double)CONCAT44((int)((ulonglong)dVar38 >> 0x20),SUB84(dVar33,0));
            dVar32 = in_xd14;
          }
          in_xd14 = dVar32;
          if (in_FPSCR_PR == '\0') {
            dVar38 = (double)CONCAT44((int)((ulonglong)dVar38 >> 0x20),
                                      SUB84(dVar38,0) - SUB84(in_dr0,0));
          }
          else {
            dVar38 = dVar38 - in_dr0;
          }
          if ((*(uint *)((int)pdVar20 + 0xc) & *(uint *)((int)pdVar24 + 0xc) &
               *(uint *)((int)pdVar23 + 0xc) & 1) == 0) {
            dVar32 = in_xd14;
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)((ulonglong)param_1 & 0xffffffff);
            }
            param_1 = dVar32;
            in_dr0 = (double)(*(code *)PTR_FUN_8c61ecc4)(pdVar20,pdVar24,pdVar23);
          }
          if ((*(uint *)((int)pdVar24 + 0xc) & *(uint *)((int)pdVar23 + 0xc) &
               *(uint *)((int)puVar22 + 0xc) & 1) == 0) {
            dVar32 = in_xd14;
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)((ulonglong)param_1 & 0xffffffff);
            }
            param_1 = dVar32;
            in_dr0 = (double)(*(code *)PTR_FUN_8c61ecc4)(pdVar24,pdVar23,puVar22);
          }
        }
      }
    }
  }
  else if (uVar7 == DAT_8c61e568) {
    *(undefined4 *)((int)puVar25 + -0x18) = 0;
    while (uVar35 = SUB84(param_1,0),
          *(int *)((int)puVar25 + -0x18) <
          (int)(uint)*(ushort *)(*(int *)((int)puVar25 + -0x1c) + 2)) {
      uVar13 = 0;
      puVar18 = puVar17 + 1;
      iVar15 = 0;
      uVar16 = (uint)*puVar17;
      *DAT_8c61ecc8 = *DAT_8c61ecc8 + uVar16;
      uVar7 = 0xffffffff;
      puVar11 = puVar18;
      if (uVar16 != 0) {
        do {
          uVar1 = *puVar11;
          puVar11 = puVar11 + 1;
          iVar15 = iVar15 + 1;
          iVar12 = (uint)uVar1 * 0x20 + *DAT_8c61eccc;
          *(int *)((int)puVar25 + -0x14) = iVar12;
          uVar21 = *(uint *)(iVar12 + 0xc);
          uVar7 = uVar7 & uVar21;
          uVar13 = uVar13 | uVar21;
        } while (iVar15 < (int)uVar16);
      }
      if ((uVar7 & 0x3f) == 0) {
        if ((uVar13 & 1) == 0) {
          iVar15 = *DAT_8c61eccc;
          unaff_r14 = (double *)((uint)puVar17[2] * 0x20 + iVar15);
          if (in_FPSCR_SZ == '\0') {
            uVar35 = *(undefined4 *)(unaff_r14 + 2);
          }
          else {
            in_xd4 = unaff_r14[2];
          }
          unaff_r12 = (double *)((uint)*puVar18 * 0x20 + iVar15);
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44(uVar35,*(undefined4 *)(unaff_r12 + 2));
          }
          else {
            in_xd2 = unaff_r12[2];
            dVar32 = in_xd4;
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) - SUB84(dVar32,0),
                                      SUB84(dVar32,0));
          }
          else {
            dVar32 = dVar32 - dVar32;
          }
          unaff_r13 = (double *)((uint)puVar17[3] * 0x20 + iVar15);
          if (in_FPSCR_SZ == '\0') {
            fVar29 = *(float *)((int)unaff_r14 + 0x14);
            dVar33 = (double)CONCAT44(fVar29,uVar35);
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                      *(undefined4 *)((int)unaff_r13 + 0x14));
            in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar29);
          }
          else {
            dVar33 = *(double *)((int)unaff_r14 + 0x14);
            in_xd2 = *(double *)((int)unaff_r13 + 0x14);
            fVar29 = (float)((ulonglong)dVar33 >> 0x20);
          }
          if (in_FPSCR_PR == '\0') {
            fVar29 = SUB84(dVar32,0) - fVar29;
            param_1 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) * fVar29,fVar29);
          }
          else {
            param_1 = (dVar32 - dVar33) * (dVar32 - dVar33);
          }
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),
                                       *(undefined4 *)((int)unaff_r12 + 0x14));
          }
          else {
            in_xd2 = *(double *)((int)unaff_r12 + 0x14);
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                      SUB84(in_dr0,0) - SUB84(param_1,0));
          }
          else {
            dVar32 = in_dr0 - param_1;
          }
          uVar35 = (undefined4)((ulonglong)param_1 >> 0x20);
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44(uVar35,SUB84(dVar33,0));
            dVar33 = (double)CONCAT44(uVar35,*(undefined4 *)(unaff_r13 + 2));
          }
          else {
            in_xd2 = unaff_r13[2];
            dVar33 = param_1;
          }
          if (in_FPSCR_PR == '\0') {
            fVar29 = SUB84(dVar33,0) - SUB84(param_1,0);
            dVar33 = (double)CONCAT44(uVar35,fVar29);
            in_dr0 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),SUB84(dVar32,0) * fVar29);
          }
          else {
            dVar33 = dVar33 - param_1;
            in_dr0 = dVar32 * dVar33;
          }
          if (in_FPSCR_PR == '\0') {
            fVar29 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
            param_1 = (double)CONCAT44(fVar29,SUB84(param_1,0));
            bVar39 = 0.0 < fVar29;
          }
          else {
            param_1 = param_1 - in_dr0;
            bVar39 = (double)((ulonglong)dVar33 & 0xffffffff00000000) < param_1;
          }
          uVar35 = SUB84(param_1,0);
          if (bVar39) goto LAB_8c61ecaa;
          *DAT_8c61ef44 = *DAT_8c61ef44 + uVar16;
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44(*(undefined4 *)unaff_r12,*(undefined4 *)unaff_r14);
          }
          else {
            in_xd2 = *unaff_r14;
            dVar32 = *unaff_r12;
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0),
                                      SUB84(dVar32,0));
          }
          else {
            dVar32 = dVar32 + dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),*(undefined4 *)unaff_r13);
          }
          else {
            in_xd2 = *unaff_r13;
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0),
                                      SUB84(dVar32,0));
          }
          else {
            dVar32 = dVar32 + dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)puVar25 + -0x10) = (int)((ulonglong)dVar32 >> 0x20);
            dVar32 = (double)CONCAT44(*(undefined4 *)((int)unaff_r12 + 4),
                                      *(undefined4 *)((int)unaff_r14 + 4));
          }
          else {
            *(double *)((int)puVar25 + -0x10) = dVar32;
            dVar32 = *(double *)((int)unaff_r12 + 4);
            in_xd2 = *(double *)((int)unaff_r14 + 4);
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0),
                                      SUB84(dVar32,0));
          }
          else {
            dVar32 = dVar32 + dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                      *(undefined4 *)((int)unaff_r13 + 4));
          }
          else {
            in_xd2 = *(double *)((int)unaff_r13 + 4);
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0),
                                      SUB84(dVar32,0));
          }
          else {
            dVar32 = dVar32 + dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)puVar25 + -0xc) = (int)((ulonglong)dVar32 >> 0x20);
            dVar32 = (double)CONCAT44(*(undefined4 *)(unaff_r12 + 1),*(undefined4 *)(unaff_r14 + 1))
            ;
          }
          else {
            *(double *)((int)puVar25 + -0xc) = dVar32;
            in_xd2 = unaff_r14[1];
            dVar32 = unaff_r12[1];
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0),
                                      SUB84(dVar32,0));
          }
          else {
            dVar32 = dVar32 + dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                      *(undefined4 *)(unaff_r13 + 1));
          }
          else {
            in_xd2 = unaff_r13[1];
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0),
                                      SUB84(dVar32,0));
          }
          else {
            dVar32 = dVar32 + dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)puVar25 + -8) = (int)((ulonglong)dVar32 >> 0x20);
          }
          else {
            *(double *)((int)puVar25 + -8) = dVar32;
          }
          dVar32 = (double)(*(code *)PTR_FUN_8c61ef48)((undefined1 *)((int)puVar25 + -0x10));
          piVar3 = DAT_8c61ef5c;
          if (in_FPSCR_SZ == '\0') {
            dVar33 = (double)CONCAT44(*(undefined4 *)DAT_8c61ef4c,
                                      *(undefined4 *)((int)puVar25 + -0x10));
          }
          else {
            in_xd2 = *(double *)((int)puVar25 + -0x10);
            dVar33 = *DAT_8c61ef4c;
          }
          if (in_FPSCR_PR == '\0') {
            dVar33 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) * SUB84(dVar33,0),
                                      SUB84(dVar33,0));
          }
          else {
            dVar33 = dVar33 * dVar33;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),*(undefined4 *)DAT_8c61ef50);
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),*DAT_8c61ef54);
          }
          else {
            in_xd2 = *DAT_8c61ef50;
          }
          if (in_FPSCR_PR == '\0') {
            fVar29 = SUB84(dVar33,0) - (float)((ulonglong)dVar33 >> 0x20);
          }
          else {
            fVar29 = SUB84(dVar33 - dVar33,0);
          }
          if (in_FPSCR_SZ == '\0') {
            dVar33 = (double)CONCAT44(*(undefined4 *)((int)puVar25 + -0xc),fVar29);
          }
          else {
            dVar33 = *(double *)((int)puVar25 + -0xc);
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                      SUB84(dVar32,0) * (float)((ulonglong)dVar33 >> 0x20));
          }
          else {
            dVar32 = dVar32 * dVar33;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar33 = (double)CONCAT44(*(undefined4 *)((int)puVar25 + -8),SUB84(dVar33,0));
          }
          else {
            dVar33 = *(double *)((int)puVar25 + -8);
          }
          if (in_FPSCR_PR == '\0') {
            dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),
                                      SUB84(dVar33,0) - SUB84(dVar32,0));
          }
          else {
            dVar33 = dVar33 - dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                      *(undefined4 *)(DAT_8c61ef4c + 1));
          }
          iVar15 = *DAT_8c61ef5c;
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                      SUB84(dVar32,0) * (float)((ulonglong)dVar33 >> 0x20));
          }
          else {
            dVar32 = dVar32 * dVar33;
          }
          puVar22 = (undefined8 *)((uint)*puVar18 * 0x20 + *DAT_8c61ef58);
          param_1 = in_xd2;
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44(SUB84(dVar33,0),uVar35);
          }
          if (in_FPSCR_PR == '\0') {
            param_1 = (double)CONCAT44((float)((ulonglong)param_1 >> 0x20) - SUB84(dVar32,0),
                                       SUB84(param_1,0));
          }
          else {
            param_1 = param_1 - dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44(*(undefined4 *)((int)puVar22 + 4),SUB84(dVar32,0));
            dVar33 = (double)CONCAT44(*(undefined4 *)(iVar15 + 4),*(undefined4 *)(iVar15 + 0x14));
          }
          else {
            dVar32 = *(double *)((int)puVar22 + 4);
            in_xd2 = *(double *)(iVar15 + 0x14);
            dVar33 = *(double *)(iVar15 + 4);
          }
          if (in_FPSCR_PR == '\0') {
            dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),
                                      SUB84(dVar33,0) * (float)((ulonglong)dVar32 >> 0x20));
          }
          else {
            dVar33 = dVar33 * dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            uVar28 = CONCAT44(*(undefined4 *)puVar22,SUB84(dVar32,0));
          }
          else {
            uVar28 = *puVar22;
          }
          if (in_FPSCR_PR == '\0') {
            dVar33 = (double)(ulonglong)
                             (uint)((float)((ulonglong)uVar28 >> 0x20) *
                                    (float)((ulonglong)dVar33 >> 0x20) + SUB84(dVar33,0));
          }
          if (in_FPSCR_SZ == '\0') {
            uVar28 = CONCAT44(*(undefined4 *)(puVar22 + 1),(int)uVar28);
            uVar31 = CONCAT44(*(undefined4 *)(iVar15 + 0x24),SUB84(dVar33,0));
          }
          else {
            uVar28 = puVar22[1];
            uVar31 = *(ulonglong *)(iVar15 + 0x24);
          }
          if (in_FPSCR_PR == '\0') {
            uVar31 = (ulonglong)
                     (uint)((float)((ulonglong)uVar28 >> 0x20) * (float)(uVar31 >> 0x20) +
                           (float)uVar31);
          }
          uVar35 = (undefined4)((ulonglong)dVar38 >> 0x20);
          dVar33 = dVar38;
          dVar32 = dVar38;
          if (in_FPSCR_SZ == '\0') {
            dVar33 = (double)CONCAT44(uVar35,(int)uVar31);
            dVar32 = (double)CONCAT44(uVar35,(int)uVar28);
          }
          if (in_FPSCR_PR == '\0') {
            dVar33 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) * SUB84(dVar33,0) +
                                      (float)((ulonglong)dVar33 >> 0x20),SUB84(dVar33,0));
          }
          if (in_FPSCR_SZ == '\0') {
            pdVar20 = (double *)(unaff_r8 + 0x14);
            *(int *)pdVar20 = (int)((ulonglong)dVar33 >> 0x20);
          }
          else {
            pdVar20 = (double *)(unaff_r8 + 0x10);
            *pdVar20 = dVar33;
          }
          uVar36 = DAT_8c61ef60;
          pdVar23 = (double *)*piVar3;
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44(*(undefined4 *)((int)puVar22 + 4),SUB84(dVar32,0));
            dVar33 = (double)CONCAT44(*(undefined4 *)pdVar23,*(undefined4 *)(pdVar23 + 2));
          }
          else {
            dVar32 = *(double *)((int)puVar22 + 4);
            in_xd2 = pdVar23[2];
            dVar33 = *pdVar23;
          }
          if (in_FPSCR_PR == '\0') {
            dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),
                                      SUB84(dVar33,0) * (float)((ulonglong)dVar32 >> 0x20));
          }
          else {
            dVar33 = dVar33 * dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            uVar28 = CONCAT44(*(undefined4 *)puVar22,SUB84(dVar32,0));
          }
          else {
            uVar28 = *puVar22;
          }
          if (in_FPSCR_PR == '\0') {
            dVar33 = (double)(ulonglong)
                             (uint)((float)((ulonglong)uVar28 >> 0x20) *
                                    (float)((ulonglong)dVar33 >> 0x20) + SUB84(dVar33,0));
          }
          if (in_FPSCR_SZ == '\0') {
            uVar28 = CONCAT44(*(undefined4 *)(puVar22 + 1),(int)uVar28);
            dVar32 = (double)CONCAT44(*(undefined4 *)(pdVar23 + 4),SUB84(dVar33,0));
          }
          else {
            uVar28 = puVar22[1];
            dVar32 = pdVar23[4];
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)(ulonglong)
                             (uint)((float)((ulonglong)uVar28 >> 0x20) *
                                    (float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0));
          }
          dVar27 = dVar38;
          dVar33 = dVar38;
          if (in_FPSCR_SZ == '\0') {
            dVar27 = (double)CONCAT44(uVar35,SUB84(dVar32,0));
            dVar33 = (double)CONCAT44(uVar35,(int)uVar28);
          }
          if (in_FPSCR_PR == '\0') {
            dVar27 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) * SUB84(dVar27,0) +
                                      (float)((ulonglong)dVar27 >> 0x20),SUB84(dVar27,0));
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)pdVar20 + -4) = (int)((ulonglong)dVar27 >> 0x20);
            *(undefined4 *)(pdVar20 + -1) = *(undefined4 *)(puVar22 + 3);
            *(undefined4 *)((int)pdVar20 + -0xc) = *(undefined4 *)((int)puVar22 + 0x14);
            pdVar20 = pdVar20 + -2;
            *(undefined4 *)pdVar20 = *(undefined4 *)(puVar22 + 2);
          }
          else {
            pdVar20[-1] = dVar27;
            pdVar20[-2] = (double)puVar22[3];
            pdVar20[-3] = *(double *)((int)puVar22 + 0x14);
            in_xd2 = (double)puVar22[2];
            pdVar20 = pdVar20 + -4;
            *pdVar20 = in_xd2;
          }
          *(undefined4 *)((int)pdVar20 + -4) = uVar36;
          piVar3 = DAT_8c61ef5c;
          puVar11 = puVar17 + 3;
          iVar15 = *DAT_8c61ef5c;
          puVar22 = (undefined8 *)((uint)puVar17[2] * 0x20 + *DAT_8c61ef58);
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44(*(undefined4 *)((int)puVar22 + 4),SUB84(dVar33,0));
            dVar33 = (double)CONCAT44(*(undefined4 *)(iVar15 + 4),*(undefined4 *)(iVar15 + 0x14));
          }
          else {
            dVar32 = *(double *)((int)puVar22 + 4);
            in_xd2 = *(double *)(iVar15 + 0x14);
            dVar33 = *(double *)(iVar15 + 4);
          }
          if (in_FPSCR_PR == '\0') {
            dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),
                                      SUB84(dVar33,0) * (float)((ulonglong)dVar32 >> 0x20));
          }
          else {
            dVar33 = dVar33 * dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            uVar28 = CONCAT44(*(undefined4 *)puVar22,SUB84(dVar32,0));
          }
          else {
            uVar28 = *puVar22;
          }
          if (in_FPSCR_PR == '\0') {
            dVar33 = (double)(ulonglong)
                             (uint)((float)((ulonglong)uVar28 >> 0x20) *
                                    (float)((ulonglong)dVar33 >> 0x20) + SUB84(dVar33,0));
          }
          if (in_FPSCR_SZ == '\0') {
            uVar28 = CONCAT44(*(undefined4 *)(puVar22 + 1),(int)uVar28);
            uVar31 = CONCAT44(*(undefined4 *)(iVar15 + 0x24),SUB84(dVar33,0));
          }
          else {
            uVar28 = puVar22[1];
            uVar31 = *(ulonglong *)(iVar15 + 0x24);
          }
          if (in_FPSCR_PR == '\0') {
            uVar31 = (ulonglong)
                     (uint)((float)((ulonglong)uVar28 >> 0x20) * (float)(uVar31 >> 0x20) +
                           (float)uVar31);
          }
          dVar33 = dVar38;
          dVar32 = dVar38;
          if (in_FPSCR_SZ == '\0') {
            dVar33 = (double)CONCAT44(uVar35,(int)uVar31);
            dVar32 = (double)CONCAT44(uVar35,(int)uVar28);
          }
          if (in_FPSCR_PR == '\0') {
            dVar33 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) * SUB84(dVar33,0) +
                                      (float)((ulonglong)dVar33 >> 0x20),SUB84(dVar33,0));
          }
          if (in_FPSCR_SZ == '\0') {
            pdVar20 = pdVar20 + 6;
            *(int *)pdVar20 = (int)((ulonglong)dVar33 >> 0x20);
          }
          else {
            pdVar20 = (double *)((int)pdVar20 + 0x2c);
            *pdVar20 = dVar33;
          }
          uVar36 = DAT_8c61ef60;
          pdVar23 = (double *)*piVar3;
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44(*(undefined4 *)((int)puVar22 + 4),SUB84(dVar32,0));
            dVar33 = (double)CONCAT44(*(undefined4 *)pdVar23,*(undefined4 *)(pdVar23 + 2));
          }
          else {
            dVar32 = *(double *)((int)puVar22 + 4);
            in_xd2 = pdVar23[2];
            dVar33 = *pdVar23;
          }
          if (in_FPSCR_PR == '\0') {
            dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),
                                      SUB84(dVar33,0) * (float)((ulonglong)dVar32 >> 0x20));
          }
          else {
            dVar33 = dVar33 * dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            uVar28 = CONCAT44(*(undefined4 *)puVar22,SUB84(dVar32,0));
          }
          else {
            uVar28 = *puVar22;
          }
          if (in_FPSCR_PR == '\0') {
            dVar33 = (double)(ulonglong)
                             (uint)((float)((ulonglong)uVar28 >> 0x20) *
                                    (float)((ulonglong)dVar33 >> 0x20) + SUB84(dVar33,0));
          }
          if (in_FPSCR_SZ == '\0') {
            uVar28 = CONCAT44(*(undefined4 *)(puVar22 + 1),(int)uVar28);
            dVar32 = (double)CONCAT44(*(undefined4 *)(pdVar23 + 4),SUB84(dVar33,0));
          }
          else {
            uVar28 = puVar22[1];
            dVar32 = pdVar23[4];
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)(ulonglong)
                             (uint)((float)((ulonglong)uVar28 >> 0x20) *
                                    (float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0));
          }
          dVar27 = dVar38;
          dVar33 = dVar38;
          if (in_FPSCR_SZ == '\0') {
            dVar27 = (double)CONCAT44(uVar35,SUB84(dVar32,0));
            dVar33 = (double)CONCAT44(uVar35,(int)uVar28);
          }
          if (in_FPSCR_PR == '\0') {
            dVar27 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) * SUB84(dVar27,0) +
                                      (float)((ulonglong)dVar27 >> 0x20),SUB84(dVar27,0));
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)pdVar20 + -4) = (int)((ulonglong)dVar27 >> 0x20);
            *(undefined4 *)(pdVar20 + -1) = *(undefined4 *)(puVar22 + 3);
            *(undefined4 *)((int)pdVar20 + -0xc) = *(undefined4 *)((int)puVar22 + 0x14);
            pdVar20 = pdVar20 + -2;
            *(undefined4 *)pdVar20 = *(undefined4 *)(puVar22 + 2);
          }
          else {
            pdVar20[-1] = dVar27;
            pdVar20[-2] = (double)puVar22[3];
            pdVar20[-3] = *(double *)((int)puVar22 + 0x14);
            in_xd2 = (double)puVar22[2];
            pdVar20 = pdVar20 + -4;
            *pdVar20 = in_xd2;
          }
          *(undefined4 *)((int)pdVar20 + -4) = uVar36;
          iVar15 = 2;
          iVar12 = (int)pdVar20 + 0x1c;
          uVar36 = (undefined4)((ulonglong)param_1 >> 0x20);
          if (2 < (int)(uVar16 - 1)) {
            do {
              uVar1 = *puVar11;
              puVar11 = puVar11 + 1;
              iVar10 = *DAT_8c61ef58;
              if (in_FPSCR_SZ == '\0') {
                pdVar20 = (double *)(iVar12 + 0x18);
                *(undefined4 *)pdVar20 = uVar36;
              }
              else {
                pdVar20 = (double *)(iVar12 + 0x14);
                *pdVar20 = param_1;
              }
              piVar3 = DAT_8c61ef5c;
              puVar22 = (undefined8 *)((uint)uVar1 * 0x20 + iVar10);
              if (in_FPSCR_SZ == '\0') {
                dVar32 = (double)CONCAT44(*(undefined4 *)((int)puVar22 + 4),SUB84(dVar33,0));
              }
              else {
                dVar32 = *(double *)((int)puVar22 + 4);
              }
              iVar12 = *DAT_8c61ef5c;
              if (in_FPSCR_SZ == '\0') {
                dVar33 = (double)CONCAT44(*(undefined4 *)(iVar12 + 4),*(undefined4 *)(iVar12 + 0x14)
                                         );
              }
              else {
                in_xd2 = *(double *)(iVar12 + 0x14);
                dVar33 = *(double *)(iVar12 + 4);
              }
              if (in_FPSCR_PR == '\0') {
                dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),
                                          SUB84(dVar33,0) * (float)((ulonglong)dVar32 >> 0x20));
              }
              else {
                dVar33 = dVar33 * dVar32;
              }
              if (in_FPSCR_SZ == '\0') {
                uVar28 = CONCAT44(*(undefined4 *)puVar22,SUB84(dVar32,0));
              }
              else {
                uVar28 = *puVar22;
              }
              if (in_FPSCR_PR == '\0') {
                dVar33 = (double)(ulonglong)
                                 (uint)((float)((ulonglong)uVar28 >> 0x20) *
                                        (float)((ulonglong)dVar33 >> 0x20) + SUB84(dVar33,0));
              }
              if (in_FPSCR_SZ == '\0') {
                uVar28 = CONCAT44(*(undefined4 *)(puVar22 + 1),(int)uVar28);
                uVar31 = CONCAT44(*(undefined4 *)(iVar12 + 0x24),SUB84(dVar33,0));
              }
              else {
                uVar28 = puVar22[1];
                uVar31 = *(ulonglong *)(iVar12 + 0x24);
              }
              if (in_FPSCR_PR == '\0') {
                uVar31 = (ulonglong)
                         (uint)((float)((ulonglong)uVar28 >> 0x20) * (float)(uVar31 >> 0x20) +
                               (float)uVar31);
              }
              dVar33 = dVar38;
              dVar32 = dVar38;
              if (in_FPSCR_SZ == '\0') {
                dVar33 = (double)CONCAT44(uVar35,(int)uVar28);
                dVar32 = (double)CONCAT44(uVar35,(int)uVar31);
              }
              if (in_FPSCR_PR == '\0') {
                dVar32 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) * SUB84(dVar32,0) +
                                          (float)((ulonglong)dVar32 >> 0x20),SUB84(dVar32,0));
              }
              if (in_FPSCR_SZ == '\0') {
                pdVar20 = (double *)((int)pdVar20 + -4);
                *(int *)pdVar20 = (int)((ulonglong)dVar32 >> 0x20);
              }
              else {
                pdVar20 = pdVar20 + -1;
                *pdVar20 = dVar32;
              }
              uVar34 = DAT_8c61ef60;
              pdVar23 = (double *)*piVar3;
              if (in_FPSCR_SZ == '\0') {
                dVar32 = (double)CONCAT44(*(undefined4 *)((int)puVar22 + 4),SUB84(dVar33,0));
                dVar33 = (double)CONCAT44(*(undefined4 *)pdVar23,*(undefined4 *)(pdVar23 + 2));
              }
              else {
                dVar32 = *(double *)((int)puVar22 + 4);
                in_xd2 = pdVar23[2];
                dVar33 = *pdVar23;
              }
              if (in_FPSCR_PR == '\0') {
                dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),
                                          SUB84(dVar33,0) * (float)((ulonglong)dVar32 >> 0x20));
              }
              else {
                dVar33 = dVar33 * dVar32;
              }
              if (in_FPSCR_SZ == '\0') {
                uVar28 = CONCAT44(*(undefined4 *)puVar22,SUB84(dVar32,0));
              }
              else {
                uVar28 = *puVar22;
              }
              if (in_FPSCR_PR == '\0') {
                dVar33 = (double)(ulonglong)
                                 (uint)((float)((ulonglong)uVar28 >> 0x20) *
                                        (float)((ulonglong)dVar33 >> 0x20) + SUB84(dVar33,0));
              }
              if (in_FPSCR_SZ == '\0') {
                uVar28 = CONCAT44(*(undefined4 *)(puVar22 + 1),(int)uVar28);
                dVar32 = (double)CONCAT44(*(undefined4 *)(pdVar23 + 4),SUB84(dVar33,0));
              }
              else {
                uVar28 = puVar22[1];
                dVar32 = pdVar23[4];
              }
              if (in_FPSCR_PR == '\0') {
                dVar32 = (double)(ulonglong)
                                 (uint)((float)((ulonglong)uVar28 >> 0x20) *
                                        (float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0));
              }
              dVar27 = dVar38;
              dVar33 = dVar38;
              if (in_FPSCR_SZ == '\0') {
                dVar27 = (double)CONCAT44(uVar35,SUB84(dVar32,0));
                dVar33 = (double)CONCAT44(uVar35,(int)uVar28);
              }
              if (in_FPSCR_PR == '\0') {
                dVar27 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) * SUB84(dVar27,0) +
                                          (float)((ulonglong)dVar27 >> 0x20),SUB84(dVar27,0));
              }
              if (in_FPSCR_SZ == '\0') {
                *(int *)((int)pdVar20 + -4) = (int)((ulonglong)dVar27 >> 0x20);
                *(undefined4 *)(pdVar20 + -1) = *(undefined4 *)(puVar22 + 3);
                *(undefined4 *)((int)pdVar20 + -0xc) = *(undefined4 *)((int)puVar22 + 0x14);
                pdVar20 = pdVar20 + -2;
                *(undefined4 *)pdVar20 = *(undefined4 *)(puVar22 + 2);
              }
              else {
                pdVar20[-1] = dVar27;
                pdVar20[-2] = (double)puVar22[3];
                pdVar20[-3] = *(double *)((int)puVar22 + 0x14);
                in_xd2 = (double)puVar22[2];
                pdVar20 = pdVar20 + -4;
                *pdVar20 = in_xd2;
              }
              *(undefined4 *)((int)pdVar20 + -4) = uVar34;
              iVar15 = iVar15 + 1;
              iVar12 = (int)pdVar20 + 0x1c;
            } while (iVar15 < (int)(uVar16 - 1));
          }
          uVar1 = *puVar11;
          puVar17 = puVar11 + 1;
          iVar15 = *DAT_8c61ef58;
          if (in_FPSCR_SZ == '\0') {
            pdVar20 = (double *)(iVar12 + 0x18);
            *(undefined4 *)pdVar20 = uVar36;
          }
          else {
            pdVar20 = (double *)(iVar12 + 0x14);
            *pdVar20 = param_1;
          }
          piVar3 = DAT_8c61ef5c;
          puVar22 = (undefined8 *)((uint)uVar1 * 0x20 + iVar15);
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44(*(undefined4 *)((int)puVar22 + 4),SUB84(dVar33,0));
          }
          else {
            dVar32 = *(double *)((int)puVar22 + 4);
          }
          iVar15 = *DAT_8c61ef5c;
          if (in_FPSCR_SZ == '\0') {
            dVar33 = (double)CONCAT44(*(undefined4 *)(iVar15 + 4),*(undefined4 *)(iVar15 + 0x14));
          }
          else {
            in_xd2 = *(double *)(iVar15 + 0x14);
            dVar33 = *(double *)(iVar15 + 4);
          }
          if (in_FPSCR_PR == '\0') {
            dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),
                                      SUB84(dVar33,0) * (float)((ulonglong)dVar32 >> 0x20));
          }
          else {
            dVar33 = dVar33 * dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            uVar28 = CONCAT44(*(undefined4 *)puVar22,SUB84(dVar32,0));
          }
          else {
            uVar28 = *puVar22;
          }
          if (in_FPSCR_PR == '\0') {
            dVar33 = (double)(ulonglong)
                             (uint)((float)((ulonglong)uVar28 >> 0x20) *
                                    (float)((ulonglong)dVar33 >> 0x20) + SUB84(dVar33,0));
          }
          if (in_FPSCR_SZ == '\0') {
            uVar28 = CONCAT44(*(undefined4 *)(puVar22 + 1),(int)uVar28);
            uVar31 = CONCAT44(*(undefined4 *)(iVar15 + 0x24),SUB84(dVar33,0));
          }
          else {
            uVar28 = puVar22[1];
            uVar31 = *(ulonglong *)(iVar15 + 0x24);
          }
          if (in_FPSCR_PR == '\0') {
            uVar31 = (ulonglong)
                     (uint)((float)((ulonglong)uVar28 >> 0x20) * (float)(uVar31 >> 0x20) +
                           (float)uVar31);
          }
          dVar33 = dVar38;
          dVar32 = dVar38;
          if (in_FPSCR_SZ == '\0') {
            dVar33 = (double)CONCAT44(uVar35,(int)uVar28);
            dVar32 = (double)CONCAT44(uVar35,(int)uVar31);
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) * SUB84(dVar32,0) +
                                      (float)((ulonglong)dVar32 >> 0x20),SUB84(dVar32,0));
          }
          if (in_FPSCR_SZ == '\0') {
            pdVar20 = (double *)((int)pdVar20 + -4);
            *(int *)pdVar20 = (int)((ulonglong)dVar32 >> 0x20);
          }
          else {
            pdVar20 = pdVar20 + -1;
            *pdVar20 = dVar32;
          }
          uVar36 = DAT_8c61ef64;
          pdVar23 = (double *)*piVar3;
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44(*(undefined4 *)((int)puVar22 + 4),SUB84(dVar33,0));
            dVar33 = (double)CONCAT44(*(undefined4 *)pdVar23,*(undefined4 *)(pdVar23 + 2));
          }
          else {
            dVar32 = *(double *)((int)puVar22 + 4);
            in_xd2 = pdVar23[2];
            dVar33 = *pdVar23;
          }
          if (in_FPSCR_PR == '\0') {
            dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),
                                      SUB84(dVar33,0) * (float)((ulonglong)dVar32 >> 0x20));
          }
          else {
            dVar33 = dVar33 * dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            uVar28 = CONCAT44(*(undefined4 *)puVar22,SUB84(dVar32,0));
          }
          else {
            uVar28 = *puVar22;
          }
          if (in_FPSCR_PR == '\0') {
            dVar33 = (double)(ulonglong)
                             (uint)((float)((ulonglong)uVar28 >> 0x20) *
                                    (float)((ulonglong)dVar33 >> 0x20) + SUB84(dVar33,0));
          }
          if (in_FPSCR_SZ == '\0') {
            uVar28 = CONCAT44(*(undefined4 *)(puVar22 + 1),(int)uVar28);
            dVar32 = (double)CONCAT44(*(undefined4 *)(pdVar23 + 4),SUB84(dVar33,0));
          }
          else {
            uVar28 = puVar22[1];
            dVar32 = pdVar23[4];
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)(ulonglong)
                             (uint)((float)((ulonglong)uVar28 >> 0x20) *
                                    (float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0));
          }
          dVar33 = dVar38;
          in_dr0 = dVar38;
          if (in_FPSCR_SZ == '\0') {
            dVar33 = (double)CONCAT44(uVar35,SUB84(dVar32,0));
            in_dr0 = (double)CONCAT44(uVar35,(int)uVar28);
          }
          if (in_FPSCR_PR == '\0') {
            dVar33 = (double)CONCAT44((float)((ulonglong)in_dr0 >> 0x20) * SUB84(dVar33,0) +
                                      (float)((ulonglong)dVar33 >> 0x20),SUB84(dVar33,0));
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)pdVar20 + -4) = (int)((ulonglong)dVar33 >> 0x20);
            *(undefined4 *)(pdVar20 + -1) = *(undefined4 *)(puVar22 + 3);
            *(undefined4 *)((int)pdVar20 + -0xc) = *(undefined4 *)((int)puVar22 + 0x14);
            pdVar20 = pdVar20 + -2;
            *(undefined4 *)pdVar20 = *(undefined4 *)(puVar22 + 2);
          }
          else {
            pdVar20[-1] = dVar33;
            pdVar20[-2] = (double)puVar22[3];
            pdVar20[-3] = *(double *)((int)puVar22 + 0x14);
            in_xd2 = (double)puVar22[2];
            pdVar20 = pdVar20 + -4;
            *pdVar20 = in_xd2;
          }
          *(undefined4 *)((int)pdVar20 + -4) = uVar36;
          unaff_r8 = (int)pdVar20 + 0x1c;
        }
        else {
          *DAT_8c61f0e4 = 0;
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44(*(undefined4 *)unaff_r12,*(undefined4 *)unaff_r14);
          }
          else {
            in_xd2 = *unaff_r14;
            dVar32 = *unaff_r12;
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0),
                                      SUB84(dVar32,0));
          }
          else {
            dVar32 = dVar32 + dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),*(undefined4 *)unaff_r13);
          }
          else {
            in_xd2 = *unaff_r13;
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0),
                                      SUB84(dVar32,0));
          }
          else {
            dVar32 = dVar32 + dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)puVar25 + -0x10) = (int)((ulonglong)dVar32 >> 0x20);
            dVar32 = (double)CONCAT44(*(undefined4 *)((int)unaff_r12 + 4),
                                      *(undefined4 *)((int)unaff_r14 + 4));
          }
          else {
            *(double *)((int)puVar25 + -0x10) = dVar32;
            in_xd2 = *(double *)((int)unaff_r14 + 4);
            dVar32 = *(double *)((int)unaff_r12 + 4);
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0),
                                      SUB84(dVar32,0));
          }
          else {
            dVar32 = dVar32 + dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                      *(undefined4 *)((int)unaff_r13 + 4));
          }
          else {
            in_xd2 = *(double *)((int)unaff_r13 + 4);
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0),
                                      SUB84(dVar32,0));
          }
          else {
            dVar32 = dVar32 + dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)puVar25 + -0xc) = (int)((ulonglong)dVar32 >> 0x20);
            dVar32 = (double)CONCAT44(*(undefined4 *)(unaff_r12 + 1),*(undefined4 *)(unaff_r14 + 1))
            ;
          }
          else {
            *(double *)((int)puVar25 + -0xc) = dVar32;
            in_xd2 = unaff_r14[1];
            dVar32 = unaff_r12[1];
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0),
                                      SUB84(dVar32,0));
          }
          else {
            dVar32 = dVar32 + dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                      *(undefined4 *)(unaff_r13 + 1));
          }
          else {
            in_xd2 = unaff_r13[1];
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0),
                                      SUB84(dVar32,0));
          }
          else {
            dVar32 = dVar32 + dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)puVar25 + -8) = (int)((ulonglong)dVar32 >> 0x20);
          }
          else {
            *(double *)((int)puVar25 + -8) = dVar32;
          }
          dVar32 = (double)(*(code *)PTR_FUN_8c61f0e8)((undefined1 *)((int)puVar25 + -0x10));
          if (in_FPSCR_SZ == '\0') {
            dVar33 = (double)CONCAT44(*(undefined4 *)DAT_8c61f0ec,
                                      *(undefined4 *)((int)puVar25 + -0x10));
          }
          else {
            in_xd2 = *(double *)((int)puVar25 + -0x10);
            dVar33 = *DAT_8c61f0ec;
          }
          if (in_FPSCR_PR == '\0') {
            dVar33 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) * SUB84(dVar33,0),
                                      SUB84(dVar33,0));
          }
          else {
            dVar33 = dVar33 * dVar33;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),*(undefined4 *)DAT_8c61f0f0);
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),*DAT_8c61f0f4);
          }
          else {
            in_xd2 = *DAT_8c61f0f0;
          }
          puVar11 = puVar17 + 2;
          if (in_FPSCR_PR == '\0') {
            fVar29 = SUB84(dVar33,0) - (float)((ulonglong)dVar33 >> 0x20);
          }
          else {
            fVar29 = SUB84(dVar33 - dVar33,0);
          }
          if (in_FPSCR_SZ == '\0') {
            dVar33 = (double)CONCAT44(*(undefined4 *)((int)puVar25 + -0xc),fVar29);
          }
          else {
            dVar33 = *(double *)((int)puVar25 + -0xc);
          }
          puVar17 = puVar17 + 3;
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                      SUB84(dVar32,0) * (float)((ulonglong)dVar33 >> 0x20));
          }
          else {
            dVar32 = dVar32 * dVar33;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar33 = (double)CONCAT44(*(undefined4 *)((int)puVar25 + -8),SUB84(dVar33,0));
          }
          else {
            dVar33 = *(double *)((int)puVar25 + -8);
          }
          if (in_FPSCR_PR == '\0') {
            dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),
                                      SUB84(dVar33,0) - SUB84(dVar32,0));
          }
          else {
            dVar33 = dVar33 - dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                      *(undefined4 *)(DAT_8c61f0ec + 1));
          }
          if (in_FPSCR_PR == '\0') {
            in_dr0 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                      SUB84(dVar32,0) * (float)((ulonglong)dVar33 >> 0x20));
          }
          else {
            in_dr0 = dVar32 * dVar33;
          }
          dVar32 = in_xd2;
          if (in_FPSCR_SZ == '\0') {
            dVar38 = (double)CONCAT44((int)((ulonglong)dVar38 >> 0x20),SUB84(dVar33,0));
            dVar32 = in_xd14;
          }
          in_xd14 = dVar32;
          unaff_r12 = (double *)((uint)*puVar18 * 0x20 + *DAT_8c61f0f8);
          if (in_FPSCR_PR == '\0') {
            dVar38 = (double)CONCAT44((int)((ulonglong)dVar38 >> 0x20),
                                      SUB84(dVar38,0) - SUB84(in_dr0,0));
          }
          else {
            dVar38 = dVar38 - in_dr0;
          }
          unaff_r14 = (double *)((uint)*puVar11 * 0x20 + *DAT_8c61f0f8);
          iVar15 = 0;
          pdVar20 = unaff_r12;
          if (0 < (int)(uVar16 - 2)) {
            do {
              unaff_r12 = unaff_r14;
              uVar1 = *puVar17;
              puVar17 = puVar17 + 1;
              unaff_r13 = (double *)((uint)uVar1 * 0x20 + *DAT_8c61f0f8);
              if ((*(uint *)((int)pdVar20 + 0xc) & *(uint *)((int)unaff_r12 + 0xc) &
                   *(uint *)((int)unaff_r13 + 0xc) & 1) == 0) {
                dVar32 = in_xd14;
                if (in_FPSCR_SZ == '\0') {
                  dVar32 = (double)((ulonglong)param_1 & 0xffffffff);
                }
                param_1 = dVar32;
                in_dr0 = (double)(*(code *)PTR_FUN_8c61f0fc)(pdVar20,unaff_r12,unaff_r13);
              }
              iVar15 = iVar15 + 1;
              pdVar20 = unaff_r12;
              unaff_r14 = unaff_r13;
            } while (iVar15 < (int)(uVar16 - 2));
          }
        }
      }
      else {
LAB_8c61ecaa:
        puVar17 = puVar18 + uVar16;
      }
      *(int *)((int)puVar25 + -0x18) = *(int *)((int)puVar25 + -0x18) + 1;
    }
  }
  else if (uVar7 == DAT_8c61e564) {
    *DAT_8c61f100 = *(undefined4 *)((int)puVar25 + -0x1c);
    fVar29 = (float)((ulonglong)param_1 >> 0x20);
    fVar37 = (float)((ulonglong)param_3 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)puVar25 + -0x20) = (int)((ulonglong)in_dr0 >> 0x20);
      *(int *)((int)puVar25 + -0x24) = SUB84(in_dr0,0);
      *(int *)((int)puVar25 + -0x28) = (int)((ulonglong)in_dr2 >> 0x20);
      *(int *)((int)puVar25 + -0x2c) = (int)in_dr2;
      *(float *)((int)puVar25 + -0x30) = fVar29;
      *(float *)((int)puVar25 + -0x34) = SUB84(param_1,0);
      *(int *)((int)puVar25 + -0x38) = (int)((ulonglong)param_2 >> 0x20);
      *(int *)((int)puVar25 + -0x3c) = (int)param_2;
      *(float *)((int)puVar25 + -0x40) = fVar37;
      *(float *)((int)puVar25 + -0x44) = SUB84(param_3,0);
      *(int *)((int)puVar25 + -0x48) = (int)((ulonglong)param_4 >> 0x20);
      *(int *)((int)puVar25 + -0x4c) = (int)param_4;
      *(int *)((int)puVar25 + -0x50) = (int)((ulonglong)in_dr12 >> 0x20);
      *(int *)((int)puVar25 + -0x54) = (int)in_dr12;
      *(int *)((int)puVar25 + -0x58) = (int)((ulonglong)dVar38 >> 0x20);
      *(int *)((int)puVar25 + -0x5c) = SUB84(dVar38,0);
      fVar2 = DAT_8c61f0d4;
    }
    else {
      *(double *)((int)puVar25 + -0x24) = in_dr0;
      *(undefined8 *)((int)puVar25 + -0x2c) = in_xd0;
      *(undefined8 *)((int)puVar25 + -0x34) = in_dr2;
      *(double *)((int)puVar25 + -0x3c) = in_xd2;
      *(double *)((int)puVar25 + -0x44) = param_1;
      *(double *)((int)puVar25 + -0x4c) = in_xd4;
      *(undefined8 *)((int)puVar25 + -0x54) = param_2;
      *(undefined8 *)((int)puVar25 + -0x5c) = in_xd6;
      *(double *)((int)puVar25 + -100) = param_3;
      *(undefined8 *)((int)puVar25 + -0x6c) = in_xd8;
      *(undefined8 *)((int)puVar25 + -0x74) = param_4;
      *(undefined8 *)((int)puVar25 + -0x7c) = in_xd10;
      *(undefined8 *)((int)puVar25 + -0x84) = in_dr12;
      *(undefined8 *)((int)puVar25 + -0x8c) = in_xd12;
      *(double *)((int)puVar25 + -0x94) = dVar38;
      *(double *)((int)puVar25 + -0x9c) = in_xd14;
      fVar2 = DAT_8c61f0d8;
    }
    dVar38 = (double)CONCAT44(fVar2,DAT_8c61f0d4);
    if (in_FPSCR_PR == '\0') {
      dVar32 = (double)CONCAT44(fVar29 * fVar2,SUB84(param_1,0) * fVar2);
      dVar38 = (double)CONCAT44(fVar37 * fVar2,SUB84(param_3,0) * fVar2);
    }
    else {
      dVar32 = param_1 * dVar38 * dVar38;
      dVar38 = param_3 * dVar38 * dVar38;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar33 = (double)CONCAT44(*(undefined4 *)((int)DAT_8c61f0d8 + 4),(int)param_2);
      dVar27 = (double)CONCAT44(*(undefined4 *)((int)DAT_8c61f0d8 + 8),(int)param_4);
    }
    else {
      dVar33 = *(double *)((int)DAT_8c61f0d8 + 8);
      dVar27 = *(double *)((int)DAT_8c61f0d8 + 0x10);
    }
    if (in_FPSCR_PR == '\0') {
      dVar33 = (double)CONCAT44(-(float)((ulonglong)dVar33 >> 0x20),SUB84(dVar33,0));
      dVar27 = (double)CONCAT44(-(float)((ulonglong)dVar27 >> 0x20),SUB84(dVar27,0));
    }
    else {
      dVar33 = -dVar33;
      dVar27 = -dVar27;
    }
    (*(code *)PTR_FUN_8c61f104)(dVar32,dVar33,dVar38,dVar27,*DAT_8c61f100);
  }
  return;
}

