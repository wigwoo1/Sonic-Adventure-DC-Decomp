/* Ghidra draft only; not buildable/verified source. */

void FUN_8c61f7c8(double param_1,undefined8 param_2,double param_3,undefined8 param_4,
                 ushort *param_5)

{
  ushort uVar1;
  float fVar2;
  int *piVar3;
  int *piVar4;
  undefined4 uVar5;
  uint uVar6;
  ulonglong *puVar7;
  undefined8 *puVar8;
  ushort *puVar9;
  int iVar10;
  double *pdVar11;
  uint uVar12;
  int unaff_r8;
  double *pdVar13;
  int iVar14;
  ushort *puVar15;
  ushort *puVar16;
  ushort *puVar17;
  undefined8 *puVar18;
  int iVar19;
  undefined8 *puVar20;
  uint uVar21;
  int iVar22;
  undefined8 *puVar23;
  int iVar24;
  uint uVar25;
  undefined8 *puVar26;
  double in_dr0;
  double dVar27;
  undefined8 uVar28;
  double dVar29;
  undefined8 in_dr2;
  undefined4 uVar31;
  ulonglong uVar30;
  double dVar32;
  float fVar34;
  float fVar36;
  double dVar35;
  undefined8 in_dr12;
  undefined4 uVar37;
  ulonglong in_dr14;
  undefined8 in_xd0;
  undefined8 in_xd2;
  double in_xd4;
  undefined8 in_xd6;
  undefined8 in_xd8;
  undefined8 in_xd10;
  undefined8 in_xd12;
  double in_xd14;
  bool bVar38;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 local_28 [8];
  undefined8 uStack_20;
  undefined4 uVar33;
  
  if (in_FPSCR_SZ == '\0') {
    uStack_20 = (double)CONCAT44((int)in_dr14,(undefined4)uStack_20);
    puVar26 = &uStack_20;
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_20._0_4_ = (int)(in_dr14 >> 0x20);
  }
  else {
    puVar26 = (undefined8 *)local_28;
    uStack_20 = in_xd14;
  }
  *(undefined4 *)((int)puVar26 + -4) = in_PR;
  if (in_FPSCR_SZ == '\0') {
    in_dr14 = (ulonglong)DAT_8c61f834;
  }
  else {
    in_xd14 = (double)CONCAT44(DAT_8c61f838,DAT_8c61f834);
  }
  *(ushort **)((int)puVar26 + -0xc) = param_5;
  dVar35 = (double)CONCAT44(0x3f800000,(int)in_dr14);
  uVar6 = *param_5 & DAT_8c61f838;
  puVar15 = *(ushort **)(param_5 + 2);
  if (uVar6 == 0) {
    *DAT_8c61f840 = *DAT_8c61f840 + (uint)*(ushort *)(*(int *)((int)puVar26 + -0xc) + 2) * 3;
    for (iVar14 = 0; piVar4 = DAT_8c61fa44, uVar33 = SUB84(param_1,0),
        iVar14 < (int)(uint)*(ushort *)(*(int *)((int)puVar26 + -0xc) + 2); iVar14 = iVar14 + 1) {
      uVar1 = *puVar15;
      puVar9 = puVar15 + 1;
      puVar17 = puVar15 + 2;
      iVar10 = *DAT_8c61fa40;
      puVar15 = puVar15 + 3;
      puVar8 = (undefined8 *)((uint)uVar1 * 0x20 + iVar10);
      puVar18 = (undefined8 *)((uint)*puVar9 * 0x20 + iVar10);
      puVar20 = (undefined8 *)((uint)*puVar17 * 0x20 + iVar10);
      if ((*(uint *)((int)puVar8 + 0xc) & *(uint *)((int)puVar18 + 0xc) &
           *(uint *)((int)puVar20 + 0xc) & 0x3f) == 0) {
        if (((*(uint *)((int)puVar8 + 0xc) | *(uint *)((int)puVar18 + 0xc) |
             *(uint *)((int)puVar20 + 0xc)) & 1) == 0) {
          if (in_FPSCR_SZ == '\0') {
            uVar33 = *(undefined4 *)(puVar18 + 2);
            dVar29 = (double)CONCAT44(uVar33,*(undefined4 *)(puVar8 + 2));
          }
          else {
            dVar29 = (double)puVar18[2];
          }
          if (in_FPSCR_PR == '\0') {
            dVar29 = (double)CONCAT44((float)((ulonglong)dVar29 >> 0x20) - SUB84(dVar29,0),
                                      SUB84(dVar29,0));
          }
          else {
            dVar29 = dVar29 - dVar29;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar29 = (double)CONCAT44((int)((ulonglong)dVar29 >> 0x20),
                                      *(undefined4 *)((int)puVar20 + 0x14));
            dVar32 = (double)CONCAT44(*(undefined4 *)((int)puVar18 + 0x14),uVar33);
          }
          else {
            dVar32 = *(double *)((int)puVar18 + 0x14);
          }
          fVar34 = (float)((ulonglong)dVar32 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            dVar29 = (double)CONCAT44((int)((ulonglong)dVar29 >> 0x20),SUB84(dVar29,0) - fVar34);
          }
          else {
            dVar29 = dVar29 - dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar34);
          }
          if (in_FPSCR_PR == '\0') {
            dVar29 = (double)CONCAT44((float)((ulonglong)dVar29 >> 0x20) * SUB84(dVar29,0),
                                      SUB84(dVar29,0));
          }
          else {
            dVar29 = dVar29 * dVar29;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar29 = (double)CONCAT44((int)((ulonglong)dVar29 >> 0x20),
                                      *(undefined4 *)((int)puVar8 + 0x14));
          }
          if (in_FPSCR_PR == '\0') {
            dVar27 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                      SUB84(in_dr0,0) - SUB84(dVar29,0));
          }
          else {
            dVar27 = in_dr0 - dVar29;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar29 = (double)CONCAT44((int)((ulonglong)dVar29 >> 0x20),*(undefined4 *)(puVar20 + 2))
            ;
          }
          if (in_FPSCR_PR == '\0') {
            dVar29 = (double)CONCAT44((int)((ulonglong)dVar29 >> 0x20),
                                      SUB84(dVar29,0) - SUB84(dVar32,0));
          }
          else {
            dVar29 = dVar29 - dVar32;
          }
          uVar6 = (uint)((ulonglong)dVar29 >> 0x20);
          param_1 = dVar29;
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44(uVar6,SUB84(dVar32,0));
          }
          if (in_FPSCR_PR == '\0') {
            in_dr0 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),
                                      SUB84(dVar27,0) * SUB84(dVar29,0));
          }
          else {
            in_dr0 = dVar27 * dVar29;
          }
          dVar29 = (double)((ulonglong)uVar6 << 0x20);
          if (in_FPSCR_PR == '\0') {
            fVar34 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
            param_1 = (double)CONCAT44(fVar34,SUB84(param_1,0));
            bVar38 = 0.0 < fVar34;
          }
          else {
            param_1 = param_1 - in_dr0;
            bVar38 = dVar29 < param_1;
          }
          if (!bVar38) {
            uVar33 = (undefined4)((ulonglong)dVar35 >> 0x20);
            if (in_FPSCR_SZ == '\0') {
              pdVar13 = (double *)(unaff_r8 + 0x18);
              *(undefined4 *)pdVar13 = uVar33;
            }
            else {
              pdVar13 = (double *)(unaff_r8 + 0x14);
              *pdVar13 = dVar35;
            }
            piVar3 = DAT_8c61fa44;
            iVar10 = *piVar4;
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)((int)puVar8 + 4),SUB84(in_dr0,0));
              dVar29 = (double)CONCAT44(uVar6,*(undefined4 *)(iVar10 + 0x14));
            }
            else {
              dVar32 = *(double *)((int)puVar8 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              fVar34 = SUB84(dVar29,0) * (float)((ulonglong)dVar32 >> 0x20);
            }
            else {
              fVar34 = SUB84(dVar29 * dVar32,0);
            }
            if (in_FPSCR_SZ == '\0') {
              uVar30 = CONCAT44(*(undefined4 *)(iVar10 + 4),fVar34);
              uVar28 = CONCAT44(*(undefined4 *)puVar8,SUB84(dVar32,0));
            }
            else {
              uVar30 = *(ulonglong *)(iVar10 + 4);
              uVar28 = *puVar8;
            }
            if (in_FPSCR_PR == '\0') {
              uVar30 = (ulonglong)
                       (uint)((float)((ulonglong)uVar28 >> 0x20) * (float)(uVar30 >> 0x20) +
                             (float)uVar30);
            }
            if (in_FPSCR_SZ == '\0') {
              uVar28 = CONCAT44(*(undefined4 *)(puVar8 + 1),(int)uVar28);
              uVar30 = CONCAT44(*(undefined4 *)(iVar10 + 0x24),(int)uVar30);
            }
            else {
              uVar28 = puVar8[1];
              uVar30 = *(ulonglong *)(iVar10 + 0x24);
            }
            if (in_FPSCR_PR == '\0') {
              uVar30 = (ulonglong)
                       (uint)((float)((ulonglong)uVar28 >> 0x20) * (float)(uVar30 >> 0x20) +
                             (float)uVar30);
            }
            uVar37 = SUB84(dVar35,0);
            dVar32 = in_xd14;
            dVar29 = in_xd14;
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(uVar37,(int)uVar28);
              dVar29 = (double)CONCAT44(uVar37,(int)uVar30);
            }
            if (in_FPSCR_PR == '\0') {
              dVar29 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) * SUB84(dVar29,0) +
                                        (float)((ulonglong)dVar29 >> 0x20),SUB84(dVar29,0));
            }
            uVar31 = (undefined4)((ulonglong)dVar29 >> 0x20);
            if (in_FPSCR_SZ == '\0') {
              pdVar13 = (double *)((int)pdVar13 + -4);
              *(undefined4 *)pdVar13 = uVar31;
            }
            else {
              pdVar13 = pdVar13 + -1;
              *pdVar13 = dVar29;
            }
            uVar5 = DAT_8c61fa48;
            puVar7 = (ulonglong *)*piVar3;
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)((int)puVar8 + 4),SUB84(dVar32,0));
              dVar29 = (double)CONCAT44(uVar31,(int)puVar7[2]);
            }
            else {
              dVar32 = *(double *)((int)puVar8 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              fVar34 = SUB84(dVar29,0) * (float)((ulonglong)dVar32 >> 0x20);
            }
            else {
              fVar34 = SUB84(dVar29 * dVar32,0);
            }
            if (in_FPSCR_SZ == '\0') {
              uVar28 = CONCAT44(*(undefined4 *)puVar8,SUB84(dVar32,0));
              uVar30 = CONCAT44((int)*puVar7,fVar34);
            }
            else {
              uVar28 = *puVar8;
              uVar30 = *puVar7;
            }
            if (in_FPSCR_PR == '\0') {
              uVar30 = (ulonglong)
                       (uint)((float)((ulonglong)uVar28 >> 0x20) * (float)(uVar30 >> 0x20) +
                             (float)uVar30);
            }
            if (in_FPSCR_SZ == '\0') {
              uVar28 = CONCAT44(*(undefined4 *)(puVar8 + 1),(int)uVar28);
              uVar30 = CONCAT44((int)puVar7[4],(int)uVar30);
            }
            else {
              uVar28 = puVar8[1];
              uVar30 = puVar7[4];
            }
            if (in_FPSCR_PR == '\0') {
              uVar30 = (ulonglong)
                       (uint)((float)((ulonglong)uVar28 >> 0x20) * (float)(uVar30 >> 0x20) +
                             (float)uVar30);
            }
            dVar32 = in_xd14;
            dVar29 = in_xd14;
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(uVar37,(int)uVar30);
              dVar29 = (double)CONCAT44(uVar37,(int)uVar28);
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((float)((ulonglong)dVar29 >> 0x20) * SUB84(dVar32,0) +
                                        (float)((ulonglong)dVar32 >> 0x20),SUB84(dVar32,0));
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)pdVar13 + -4) = (int)((ulonglong)dVar32 >> 0x20);
              *(undefined4 *)(pdVar13 + -1) = *(undefined4 *)(puVar8 + 3);
              *(undefined4 *)((int)pdVar13 + -0xc) = *(undefined4 *)((int)puVar8 + 0x14);
              pdVar13 = pdVar13 + -2;
              *(undefined4 *)pdVar13 = *(undefined4 *)(puVar8 + 2);
            }
            else {
              pdVar13[-1] = dVar32;
              pdVar13[-2] = (double)puVar8[3];
              pdVar13[-3] = *(double *)((int)puVar8 + 0x14);
              pdVar13 = pdVar13 + -4;
              *pdVar13 = (double)puVar8[2];
            }
            *(undefined4 *)((int)pdVar13 + -4) = uVar5;
            if (in_FPSCR_SZ == '\0') {
              pdVar13 = (double *)((int)pdVar13 + 0x34);
              *(undefined4 *)pdVar13 = uVar33;
              dVar29 = (double)CONCAT44(*(undefined4 *)((int)puVar18 + 4),SUB84(dVar29,0));
            }
            else {
              pdVar13 = pdVar13 + 6;
              *pdVar13 = dVar35;
              dVar29 = *(double *)((int)puVar18 + 4);
            }
            iVar10 = *piVar3;
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)(iVar10 + 4),*(undefined4 *)(iVar10 + 0x14));
            }
            else {
              dVar32 = *(double *)(iVar10 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                        SUB84(dVar32,0) * (float)((ulonglong)dVar29 >> 0x20));
            }
            else {
              dVar32 = dVar32 * dVar29;
            }
            if (in_FPSCR_SZ == '\0') {
              uVar28 = CONCAT44(*(undefined4 *)puVar18,SUB84(dVar29,0));
            }
            else {
              uVar28 = *puVar18;
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)(ulonglong)
                               (uint)((float)((ulonglong)uVar28 >> 0x20) *
                                      (float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0));
            }
            if (in_FPSCR_SZ == '\0') {
              uVar28 = CONCAT44(*(undefined4 *)(puVar18 + 1),(int)uVar28);
              uVar30 = CONCAT44(*(undefined4 *)(iVar10 + 0x24),SUB84(dVar32,0));
            }
            else {
              uVar28 = puVar18[1];
              uVar30 = *(ulonglong *)(iVar10 + 0x24);
            }
            if (in_FPSCR_PR == '\0') {
              uVar30 = (ulonglong)
                       (uint)((float)((ulonglong)uVar28 >> 0x20) * (float)(uVar30 >> 0x20) +
                             (float)uVar30);
            }
            dVar32 = in_xd14;
            dVar29 = in_xd14;
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(uVar37,(int)uVar28);
              dVar29 = (double)CONCAT44(uVar37,(int)uVar30);
            }
            if (in_FPSCR_PR == '\0') {
              dVar29 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) * SUB84(dVar29,0) +
                                        (float)((ulonglong)dVar29 >> 0x20),SUB84(dVar29,0));
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar13 = (double *)((int)pdVar13 + -4);
              *(int *)pdVar13 = (int)((ulonglong)dVar29 >> 0x20);
            }
            else {
              pdVar13 = pdVar13 + -1;
              *pdVar13 = dVar29;
            }
            pdVar11 = (double *)*piVar3;
            if (in_FPSCR_SZ == '\0') {
              dVar29 = (double)CONCAT44(*(undefined4 *)((int)puVar18 + 4),SUB84(dVar32,0));
              dVar32 = (double)CONCAT44(*(undefined4 *)pdVar11,*(undefined4 *)(pdVar11 + 2));
            }
            else {
              dVar29 = *(double *)((int)puVar18 + 4);
              dVar32 = *pdVar11;
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                        SUB84(dVar32,0) * (float)((ulonglong)dVar29 >> 0x20));
            }
            else {
              dVar32 = dVar32 * dVar29;
            }
            if (in_FPSCR_SZ == '\0') {
              uVar28 = CONCAT44(*(undefined4 *)puVar18,SUB84(dVar29,0));
            }
            else {
              uVar28 = *puVar18;
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)(ulonglong)
                               (uint)((float)((ulonglong)uVar28 >> 0x20) *
                                      (float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0));
            }
            if (in_FPSCR_SZ == '\0') {
              uVar28 = CONCAT44(*(undefined4 *)(puVar18 + 1),(int)uVar28);
              dVar29 = (double)CONCAT44(*(undefined4 *)(pdVar11 + 4),SUB84(dVar32,0));
            }
            else {
              uVar28 = puVar18[1];
              dVar29 = pdVar11[4];
            }
            if (in_FPSCR_PR == '\0') {
              dVar29 = (double)(ulonglong)
                               (uint)((float)((ulonglong)uVar28 >> 0x20) *
                                      (float)((ulonglong)dVar29 >> 0x20) + SUB84(dVar29,0));
            }
            dVar27 = in_xd14;
            dVar32 = in_xd14;
            if (in_FPSCR_SZ == '\0') {
              dVar27 = (double)CONCAT44(uVar37,SUB84(dVar29,0));
              dVar32 = (double)CONCAT44(uVar37,(int)uVar28);
            }
            if (in_FPSCR_PR == '\0') {
              dVar27 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) * SUB84(dVar27,0) +
                                        (float)((ulonglong)dVar27 >> 0x20),SUB84(dVar27,0));
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)pdVar13 + -4) = (int)((ulonglong)dVar27 >> 0x20);
              *(undefined4 *)(pdVar13 + -1) = *(undefined4 *)(puVar18 + 3);
              *(undefined4 *)((int)pdVar13 + -0xc) = *(undefined4 *)((int)puVar18 + 0x14);
              pdVar13 = pdVar13 + -2;
              *(undefined4 *)pdVar13 = *(undefined4 *)(puVar18 + 2);
            }
            else {
              pdVar13[-1] = dVar27;
              pdVar13[-2] = (double)puVar18[3];
              pdVar13[-3] = *(double *)((int)puVar18 + 0x14);
              pdVar13 = pdVar13 + -4;
              *pdVar13 = (double)puVar18[2];
            }
            *(undefined4 *)((int)pdVar13 + -4) = uVar5;
            if (in_FPSCR_SZ == '\0') {
              pdVar13 = (double *)((int)pdVar13 + 0x34);
              *(undefined4 *)pdVar13 = uVar33;
              dVar29 = (double)CONCAT44(*(undefined4 *)((int)puVar20 + 4),SUB84(dVar32,0));
            }
            else {
              pdVar13 = pdVar13 + 6;
              *pdVar13 = dVar35;
              dVar29 = *(double *)((int)puVar20 + 4);
            }
            iVar10 = *piVar3;
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)(iVar10 + 4),*(undefined4 *)(iVar10 + 0x14));
            }
            else {
              dVar32 = *(double *)(iVar10 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                        SUB84(dVar32,0) * (float)((ulonglong)dVar29 >> 0x20));
            }
            else {
              dVar32 = dVar32 * dVar29;
            }
            if (in_FPSCR_SZ == '\0') {
              uVar28 = CONCAT44(*(undefined4 *)puVar20,SUB84(dVar29,0));
            }
            else {
              uVar28 = *puVar20;
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)(ulonglong)
                               (uint)((float)((ulonglong)uVar28 >> 0x20) *
                                      (float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0));
            }
            if (in_FPSCR_SZ == '\0') {
              uVar28 = CONCAT44(*(undefined4 *)(puVar20 + 1),(int)uVar28);
              uVar30 = CONCAT44(*(undefined4 *)(iVar10 + 0x24),SUB84(dVar32,0));
            }
            else {
              uVar28 = puVar20[1];
              uVar30 = *(ulonglong *)(iVar10 + 0x24);
            }
            if (in_FPSCR_PR == '\0') {
              uVar30 = (ulonglong)
                       (uint)((float)((ulonglong)uVar28 >> 0x20) * (float)(uVar30 >> 0x20) +
                             (float)uVar30);
            }
            dVar32 = in_xd14;
            dVar29 = in_xd14;
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(uVar37,(int)uVar28);
              dVar29 = (double)CONCAT44(uVar37,(int)uVar30);
            }
            if (in_FPSCR_PR == '\0') {
              dVar29 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) * SUB84(dVar29,0) +
                                        (float)((ulonglong)dVar29 >> 0x20),SUB84(dVar29,0));
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar13 = (double *)((int)pdVar13 + -4);
              *(int *)pdVar13 = (int)((ulonglong)dVar29 >> 0x20);
            }
            else {
              pdVar13 = pdVar13 + -1;
              *pdVar13 = dVar29;
            }
            uVar33 = DAT_8c61fa4c;
            pdVar11 = (double *)*piVar3;
            if (in_FPSCR_SZ == '\0') {
              dVar29 = (double)CONCAT44(*(undefined4 *)((int)puVar20 + 4),SUB84(dVar32,0));
              dVar32 = (double)CONCAT44(*(undefined4 *)pdVar11,*(undefined4 *)(pdVar11 + 2));
            }
            else {
              dVar29 = *(double *)((int)puVar20 + 4);
              dVar32 = *pdVar11;
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                        SUB84(dVar32,0) * (float)((ulonglong)dVar29 >> 0x20));
            }
            else {
              dVar32 = dVar32 * dVar29;
            }
            if (in_FPSCR_SZ == '\0') {
              uVar28 = CONCAT44(*(undefined4 *)puVar20,SUB84(dVar29,0));
            }
            else {
              uVar28 = *puVar20;
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)(ulonglong)
                               (uint)((float)((ulonglong)uVar28 >> 0x20) *
                                      (float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0));
            }
            if (in_FPSCR_SZ == '\0') {
              uVar28 = CONCAT44(*(undefined4 *)(puVar20 + 1),(int)uVar28);
              dVar29 = (double)CONCAT44(*(undefined4 *)(pdVar11 + 4),SUB84(dVar32,0));
            }
            else {
              uVar28 = puVar20[1];
              dVar29 = pdVar11[4];
            }
            if (in_FPSCR_PR == '\0') {
              dVar29 = (double)(ulonglong)
                               (uint)((float)((ulonglong)uVar28 >> 0x20) *
                                      (float)((ulonglong)dVar29 >> 0x20) + SUB84(dVar29,0));
            }
            dVar32 = in_xd14;
            in_dr0 = in_xd14;
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(uVar37,SUB84(dVar29,0));
              in_dr0 = (double)CONCAT44(uVar37,(int)uVar28);
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((float)((ulonglong)in_dr0 >> 0x20) * SUB84(dVar32,0) +
                                        (float)((ulonglong)dVar32 >> 0x20),SUB84(dVar32,0));
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)pdVar13 + -4) = (int)((ulonglong)dVar32 >> 0x20);
              *(undefined4 *)(pdVar13 + -1) = *(undefined4 *)(puVar20 + 3);
              *(undefined4 *)((int)pdVar13 + -0xc) = *(undefined4 *)((int)puVar20 + 0x14);
              pdVar13 = pdVar13 + -2;
              *(undefined4 *)pdVar13 = *(undefined4 *)(puVar20 + 2);
            }
            else {
              pdVar13[-1] = dVar32;
              pdVar13[-2] = (double)puVar20[3];
              pdVar13[-3] = *(double *)((int)puVar20 + 0x14);
              pdVar13 = pdVar13 + -4;
              *pdVar13 = (double)puVar20[2];
            }
            *(undefined4 *)((int)pdVar13 + -4) = uVar33;
            *DAT_8c61fa50 = *DAT_8c61fa50 + 3;
            unaff_r8 = (int)pdVar13 + 0x1c;
          }
        }
        else {
          *DAT_8c61fa54 = 1;
          in_dr0 = (double)(*(code *)PTR_FUN_8c61fa58)();
        }
      }
    }
  }
  else if (uVar6 == (int)DAT_8c61f830) {
    *DAT_8c61fa5c = *DAT_8c61fa5c + (uint)*(ushort *)(*(int *)((int)puVar26 + -0xc) + 2) * 4;
    for (iVar14 = 0; piVar4 = DAT_8c61fd04,
        iVar14 < (int)(uint)*(ushort *)(*(int *)((int)puVar26 + -0xc) + 2); iVar14 = iVar14 + 1) {
      uVar1 = *puVar15;
      puVar9 = puVar15 + 1;
      puVar17 = puVar15 + 2;
      iVar10 = *DAT_8c61fd00;
      puVar16 = puVar15 + 3;
      puVar15 = puVar15 + 4;
      puVar8 = (undefined8 *)((uint)uVar1 * 0x20 + iVar10);
      puVar20 = (undefined8 *)((uint)*puVar9 * 0x20 + iVar10);
      puVar23 = (undefined8 *)((uint)*puVar17 * 0x20 + iVar10);
      puVar18 = (undefined8 *)((uint)*puVar16 * 0x20 + iVar10);
      if ((*(uint *)((int)puVar8 + 0xc) & *(uint *)((int)puVar20 + 0xc) &
           *(uint *)((int)puVar23 + 0xc) & *(uint *)((int)puVar18 + 0xc) & 0x3f) == 0) {
        if (((*(uint *)((int)puVar8 + 0xc) | *(uint *)((int)puVar20 + 0xc) |
              *(uint *)((int)puVar23 + 0xc) | *(uint *)((int)puVar18 + 0xc)) & 1) == 0) {
          if (in_FPSCR_SZ == '\0') {
            uVar33 = *(undefined4 *)(puVar20 + 2);
            dVar32 = (double)CONCAT44(uVar33,*(undefined4 *)(puVar8 + 2));
            dVar29 = (double)CONCAT44(uVar33,SUB84(param_1,0));
          }
          else {
            dVar32 = (double)puVar20[2];
            uVar33 = (undefined4)((ulonglong)dVar32 >> 0x20);
            dVar29 = dVar32;
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
                                      *(undefined4 *)((int)puVar23 + 0x14));
            dVar29 = (double)CONCAT44(uVar33,*(undefined4 *)((int)puVar20 + 0x14));
          }
          fVar34 = SUB84(dVar29,0);
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),SUB84(dVar32,0) - fVar34);
          }
          else {
            dVar32 = dVar32 - dVar29;
          }
          if (in_FPSCR_SZ == '\0') {
            in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar34);
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
                                      *(undefined4 *)((int)puVar8 + 0x14));
          }
          if (in_FPSCR_PR == '\0') {
            dVar27 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                      SUB84(in_dr0,0) - SUB84(dVar32,0));
          }
          else {
            dVar27 = in_dr0 - dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),*(undefined4 *)(puVar23 + 2))
            ;
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                      SUB84(dVar32,0) - (float)((ulonglong)dVar29 >> 0x20));
          }
          else {
            dVar32 = dVar32 - dVar29;
          }
          uVar6 = (uint)((ulonglong)dVar32 >> 0x20);
          param_1 = dVar32;
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44(uVar6,fVar34);
          }
          if (in_FPSCR_PR == '\0') {
            in_dr0 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),
                                      SUB84(dVar27,0) * SUB84(dVar32,0));
          }
          else {
            in_dr0 = dVar27 * dVar32;
          }
          dVar29 = (double)((ulonglong)uVar6 << 0x20);
          if (in_FPSCR_PR == '\0') {
            fVar34 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
            param_1 = (double)CONCAT44(fVar34,SUB84(param_1,0));
            bVar38 = 0.0 < fVar34;
          }
          else {
            param_1 = param_1 - in_dr0;
            bVar38 = dVar29 < param_1;
          }
          if (!bVar38) {
            uVar33 = (undefined4)((ulonglong)dVar35 >> 0x20);
            if (in_FPSCR_SZ == '\0') {
              pdVar13 = (double *)(unaff_r8 + 0x18);
              *(undefined4 *)pdVar13 = uVar33;
            }
            else {
              pdVar13 = (double *)(unaff_r8 + 0x14);
              *pdVar13 = dVar35;
            }
            piVar3 = DAT_8c61fd04;
            iVar10 = *piVar4;
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)((int)puVar8 + 4),SUB84(in_dr0,0));
              dVar29 = (double)CONCAT44(uVar6,*(undefined4 *)(iVar10 + 0x14));
            }
            else {
              dVar32 = *(double *)((int)puVar8 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              fVar34 = SUB84(dVar29,0) * (float)((ulonglong)dVar32 >> 0x20);
            }
            else {
              fVar34 = SUB84(dVar29 * dVar32,0);
            }
            if (in_FPSCR_SZ == '\0') {
              uVar30 = CONCAT44(*(undefined4 *)(iVar10 + 4),fVar34);
              uVar28 = CONCAT44(*(undefined4 *)puVar8,SUB84(dVar32,0));
            }
            else {
              uVar30 = *(ulonglong *)(iVar10 + 4);
              uVar28 = *puVar8;
            }
            if (in_FPSCR_PR == '\0') {
              uVar30 = (ulonglong)
                       (uint)((float)((ulonglong)uVar28 >> 0x20) * (float)(uVar30 >> 0x20) +
                             (float)uVar30);
            }
            if (in_FPSCR_SZ == '\0') {
              uVar28 = CONCAT44(*(undefined4 *)(puVar8 + 1),(int)uVar28);
              uVar30 = CONCAT44(*(undefined4 *)(iVar10 + 0x24),(int)uVar30);
            }
            else {
              uVar28 = puVar8[1];
              uVar30 = *(ulonglong *)(iVar10 + 0x24);
            }
            if (in_FPSCR_PR == '\0') {
              uVar30 = (ulonglong)
                       (uint)((float)((ulonglong)uVar28 >> 0x20) * (float)(uVar30 >> 0x20) +
                             (float)uVar30);
            }
            uVar37 = SUB84(dVar35,0);
            dVar32 = in_xd14;
            dVar29 = in_xd14;
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(uVar37,(int)uVar30);
              dVar29 = (double)CONCAT44(uVar37,(int)uVar28);
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((float)((ulonglong)dVar29 >> 0x20) * SUB84(dVar32,0) +
                                        (float)((ulonglong)dVar32 >> 0x20),SUB84(dVar32,0));
            }
            uVar31 = (undefined4)((ulonglong)dVar32 >> 0x20);
            if (in_FPSCR_SZ == '\0') {
              pdVar13 = (double *)((int)pdVar13 + -4);
              *(undefined4 *)pdVar13 = uVar31;
            }
            else {
              pdVar13 = pdVar13 + -1;
              *pdVar13 = dVar32;
            }
            uVar5 = DAT_8c61fd08;
            puVar7 = (ulonglong *)*piVar3;
            if (in_FPSCR_SZ == '\0') {
              dVar29 = (double)CONCAT44(*(undefined4 *)((int)puVar8 + 4),SUB84(dVar29,0));
              dVar32 = (double)CONCAT44(uVar31,(int)puVar7[2]);
            }
            else {
              dVar29 = *(double *)((int)puVar8 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              fVar34 = SUB84(dVar32,0) * (float)((ulonglong)dVar29 >> 0x20);
            }
            else {
              fVar34 = SUB84(dVar32 * dVar29,0);
            }
            if (in_FPSCR_SZ == '\0') {
              uVar30 = CONCAT44((int)*puVar7,fVar34);
              uVar28 = CONCAT44(*(undefined4 *)puVar8,SUB84(dVar29,0));
            }
            else {
              uVar30 = *puVar7;
              uVar28 = *puVar8;
            }
            if (in_FPSCR_PR == '\0') {
              uVar30 = (ulonglong)
                       (uint)((float)((ulonglong)uVar28 >> 0x20) * (float)(uVar30 >> 0x20) +
                             (float)uVar30);
            }
            if (in_FPSCR_SZ == '\0') {
              uVar28 = CONCAT44(*(undefined4 *)(puVar8 + 1),(int)uVar28);
              uVar30 = CONCAT44((int)puVar7[4],(int)uVar30);
            }
            else {
              uVar28 = puVar8[1];
              uVar30 = puVar7[4];
            }
            if (in_FPSCR_PR == '\0') {
              uVar30 = (ulonglong)
                       (uint)((float)((ulonglong)uVar28 >> 0x20) * (float)(uVar30 >> 0x20) +
                             (float)uVar30);
            }
            dVar32 = in_xd14;
            dVar29 = in_xd14;
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(uVar37,(int)uVar30);
              dVar29 = (double)CONCAT44(uVar37,(int)uVar28);
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((float)((ulonglong)dVar29 >> 0x20) * SUB84(dVar32,0) +
                                        (float)((ulonglong)dVar32 >> 0x20),SUB84(dVar32,0));
            }
            uVar31 = (undefined4)((ulonglong)dVar32 >> 0x20);
            if (in_FPSCR_SZ == '\0') {
              *(undefined4 *)((int)pdVar13 + -4) = uVar31;
              *(undefined4 *)(pdVar13 + -1) = *(undefined4 *)(puVar8 + 3);
              *(undefined4 *)((int)pdVar13 + -0xc) = *(undefined4 *)((int)puVar8 + 0x14);
              dVar32 = (double)CONCAT44(uVar31,*(undefined4 *)(puVar8 + 2));
              pdVar13 = pdVar13 + -2;
              *(undefined4 *)pdVar13 = *(undefined4 *)(puVar8 + 2);
            }
            else {
              pdVar13[-1] = dVar32;
              pdVar13[-2] = (double)puVar8[3];
              pdVar13[-3] = *(double *)((int)puVar8 + 0x14);
              pdVar13 = pdVar13 + -4;
              *pdVar13 = (double)puVar8[2];
            }
            *(undefined4 *)((int)pdVar13 + -4) = uVar5;
            piVar4 = DAT_8c61fd04;
            if (in_FPSCR_SZ == '\0') {
              pdVar13 = (double *)((int)pdVar13 + 0x34);
              *(undefined4 *)pdVar13 = uVar33;
            }
            else {
              pdVar13 = pdVar13 + 6;
              *pdVar13 = dVar35;
            }
            piVar3 = DAT_8c61fd04;
            iVar10 = *piVar4;
            if (in_FPSCR_SZ == '\0') {
              dVar29 = (double)CONCAT44(*(undefined4 *)((int)puVar20 + 4),SUB84(dVar29,0));
              dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                        *(undefined4 *)(iVar10 + 0x14));
            }
            else {
              dVar29 = *(double *)((int)puVar20 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              fVar34 = SUB84(dVar32,0) * (float)((ulonglong)dVar29 >> 0x20);
            }
            else {
              fVar34 = SUB84(dVar32 * dVar29,0);
            }
            if (in_FPSCR_SZ == '\0') {
              uVar30 = CONCAT44(*(undefined4 *)(iVar10 + 4),fVar34);
              uVar28 = CONCAT44(*(undefined4 *)puVar20,SUB84(dVar29,0));
            }
            else {
              uVar30 = *(ulonglong *)(iVar10 + 4);
              uVar28 = *puVar20;
            }
            if (in_FPSCR_PR == '\0') {
              uVar30 = (ulonglong)
                       (uint)((float)((ulonglong)uVar28 >> 0x20) * (float)(uVar30 >> 0x20) +
                             (float)uVar30);
            }
            if (in_FPSCR_SZ == '\0') {
              uVar28 = CONCAT44(*(undefined4 *)(puVar20 + 1),(int)uVar28);
              uVar30 = CONCAT44(*(undefined4 *)(iVar10 + 0x24),(int)uVar30);
            }
            else {
              uVar28 = puVar20[1];
              uVar30 = *(ulonglong *)(iVar10 + 0x24);
            }
            if (in_FPSCR_PR == '\0') {
              uVar30 = (ulonglong)
                       (uint)((float)((ulonglong)uVar28 >> 0x20) * (float)(uVar30 >> 0x20) +
                             (float)uVar30);
            }
            dVar32 = in_xd14;
            dVar29 = in_xd14;
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(uVar37,(int)uVar30);
              dVar29 = (double)CONCAT44(uVar37,(int)uVar28);
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((float)((ulonglong)dVar29 >> 0x20) * SUB84(dVar32,0) +
                                        (float)((ulonglong)dVar32 >> 0x20),SUB84(dVar32,0));
            }
            uVar31 = (undefined4)((ulonglong)dVar32 >> 0x20);
            if (in_FPSCR_SZ == '\0') {
              pdVar13 = (double *)((int)pdVar13 + -4);
              *(undefined4 *)pdVar13 = uVar31;
            }
            else {
              pdVar13 = pdVar13 + -1;
              *pdVar13 = dVar32;
            }
            uVar5 = DAT_8c61fd08;
            puVar7 = (ulonglong *)*piVar3;
            if (in_FPSCR_SZ == '\0') {
              dVar29 = (double)CONCAT44(*(undefined4 *)((int)puVar20 + 4),SUB84(dVar29,0));
              dVar32 = (double)CONCAT44(uVar31,(int)puVar7[2]);
            }
            else {
              dVar29 = *(double *)((int)puVar20 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              fVar34 = SUB84(dVar32,0) * (float)((ulonglong)dVar29 >> 0x20);
            }
            else {
              fVar34 = SUB84(dVar32 * dVar29,0);
            }
            if (in_FPSCR_SZ == '\0') {
              uVar30 = CONCAT44((int)*puVar7,fVar34);
              uVar28 = CONCAT44(*(undefined4 *)puVar20,SUB84(dVar29,0));
            }
            else {
              uVar30 = *puVar7;
              uVar28 = *puVar20;
            }
            if (in_FPSCR_PR == '\0') {
              uVar30 = (ulonglong)
                       (uint)((float)((ulonglong)uVar28 >> 0x20) * (float)(uVar30 >> 0x20) +
                             (float)uVar30);
            }
            if (in_FPSCR_SZ == '\0') {
              uVar28 = CONCAT44(*(undefined4 *)(puVar20 + 1),(int)uVar28);
              uVar30 = CONCAT44((int)puVar7[4],(int)uVar30);
            }
            else {
              uVar28 = puVar20[1];
              uVar30 = puVar7[4];
            }
            if (in_FPSCR_PR == '\0') {
              uVar30 = (ulonglong)
                       (uint)((float)((ulonglong)uVar28 >> 0x20) * (float)(uVar30 >> 0x20) +
                             (float)uVar30);
            }
            dVar32 = in_xd14;
            dVar29 = in_xd14;
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(uVar37,(int)uVar30);
              dVar29 = (double)CONCAT44(uVar37,(int)uVar28);
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((float)((ulonglong)dVar29 >> 0x20) * SUB84(dVar32,0) +
                                        (float)((ulonglong)dVar32 >> 0x20),SUB84(dVar32,0));
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)pdVar13 + -4) = (int)((ulonglong)dVar32 >> 0x20);
              *(undefined4 *)(pdVar13 + -1) = *(undefined4 *)(puVar20 + 3);
              *(undefined4 *)((int)pdVar13 + -0xc) = *(undefined4 *)((int)puVar20 + 0x14);
              pdVar13 = pdVar13 + -2;
              *(undefined4 *)pdVar13 = *(undefined4 *)(puVar20 + 2);
            }
            else {
              pdVar13[-1] = dVar32;
              pdVar13[-2] = (double)puVar20[3];
              pdVar13[-3] = *(double *)((int)puVar20 + 0x14);
              pdVar13 = pdVar13 + -4;
              *pdVar13 = (double)puVar20[2];
            }
            *(undefined4 *)((int)pdVar13 + -4) = uVar5;
            piVar4 = DAT_8c61fd04;
            if (in_FPSCR_SZ == '\0') {
              pdVar13 = (double *)((int)pdVar13 + 0x34);
              *(undefined4 *)pdVar13 = uVar33;
            }
            else {
              pdVar13 = pdVar13 + 6;
              *pdVar13 = dVar35;
            }
            iVar10 = *piVar4;
            if (in_FPSCR_SZ == '\0') {
              dVar29 = (double)CONCAT44(*(undefined4 *)((int)puVar23 + 4),SUB84(dVar29,0));
              dVar32 = (double)CONCAT44(*(undefined4 *)(iVar10 + 4),*(undefined4 *)(iVar10 + 0x14));
            }
            else {
              dVar29 = *(double *)((int)puVar23 + 4);
              dVar32 = *(double *)(iVar10 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                        SUB84(dVar32,0) * (float)((ulonglong)dVar29 >> 0x20));
            }
            else {
              dVar32 = dVar32 * dVar29;
            }
            if (in_FPSCR_SZ == '\0') {
              uVar28 = CONCAT44(*(undefined4 *)puVar23,SUB84(dVar29,0));
            }
            else {
              uVar28 = *puVar23;
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)(ulonglong)
                               (uint)((float)((ulonglong)uVar28 >> 0x20) *
                                      (float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0));
            }
            if (in_FPSCR_SZ == '\0') {
              uVar28 = CONCAT44(*(undefined4 *)(puVar23 + 1),(int)uVar28);
              uVar30 = CONCAT44(*(undefined4 *)(iVar10 + 0x24),SUB84(dVar32,0));
            }
            else {
              uVar28 = puVar23[1];
              uVar30 = *(ulonglong *)(iVar10 + 0x24);
            }
            if (in_FPSCR_PR == '\0') {
              uVar30 = (ulonglong)
                       (uint)((float)((ulonglong)uVar28 >> 0x20) * (float)(uVar30 >> 0x20) +
                             (float)uVar30);
            }
            dVar32 = in_xd14;
            dVar29 = in_xd14;
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(uVar37,(int)uVar28);
              dVar29 = (double)CONCAT44(uVar37,(int)uVar30);
            }
            if (in_FPSCR_PR == '\0') {
              dVar29 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) * SUB84(dVar29,0) +
                                        (float)((ulonglong)dVar29 >> 0x20),SUB84(dVar29,0));
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar13 = (double *)((int)pdVar13 + -4);
              *(int *)pdVar13 = (int)((ulonglong)dVar29 >> 0x20);
            }
            else {
              pdVar13 = pdVar13 + -1;
              *pdVar13 = dVar29;
            }
            uVar31 = DAT_8c61fd08;
            pdVar11 = (double *)*piVar4;
            if (in_FPSCR_SZ == '\0') {
              dVar29 = (double)CONCAT44(*(undefined4 *)((int)puVar23 + 4),SUB84(dVar32,0));
              dVar32 = (double)CONCAT44(*(undefined4 *)pdVar11,*(undefined4 *)(pdVar11 + 2));
            }
            else {
              dVar29 = *(double *)((int)puVar23 + 4);
              dVar32 = *pdVar11;
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                        SUB84(dVar32,0) * (float)((ulonglong)dVar29 >> 0x20));
            }
            else {
              dVar32 = dVar32 * dVar29;
            }
            if (in_FPSCR_SZ == '\0') {
              uVar28 = CONCAT44(*(undefined4 *)puVar23,SUB84(dVar29,0));
            }
            else {
              uVar28 = *puVar23;
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)(ulonglong)
                               (uint)((float)((ulonglong)uVar28 >> 0x20) *
                                      (float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0));
            }
            if (in_FPSCR_SZ == '\0') {
              uVar28 = CONCAT44(*(undefined4 *)(puVar23 + 1),(int)uVar28);
              dVar29 = (double)CONCAT44(*(undefined4 *)(pdVar11 + 4),SUB84(dVar32,0));
            }
            else {
              uVar28 = puVar23[1];
              dVar29 = pdVar11[4];
            }
            if (in_FPSCR_PR == '\0') {
              dVar29 = (double)(ulonglong)
                               (uint)((float)((ulonglong)uVar28 >> 0x20) *
                                      (float)((ulonglong)dVar29 >> 0x20) + SUB84(dVar29,0));
            }
            dVar27 = in_xd14;
            dVar32 = in_xd14;
            if (in_FPSCR_SZ == '\0') {
              dVar27 = (double)CONCAT44(uVar37,SUB84(dVar29,0));
              dVar32 = (double)CONCAT44(uVar37,(int)uVar28);
            }
            if (in_FPSCR_PR == '\0') {
              dVar27 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) * SUB84(dVar27,0) +
                                        (float)((ulonglong)dVar27 >> 0x20),SUB84(dVar27,0));
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)pdVar13 + -4) = (int)((ulonglong)dVar27 >> 0x20);
              *(undefined4 *)(pdVar13 + -1) = *(undefined4 *)(puVar23 + 3);
              *(undefined4 *)((int)pdVar13 + -0xc) = *(undefined4 *)((int)puVar23 + 0x14);
              pdVar13 = pdVar13 + -2;
              *(undefined4 *)pdVar13 = *(undefined4 *)(puVar23 + 2);
            }
            else {
              pdVar13[-1] = dVar27;
              pdVar13[-2] = (double)puVar23[3];
              pdVar13[-3] = *(double *)((int)puVar23 + 0x14);
              pdVar13 = pdVar13 + -4;
              *pdVar13 = (double)puVar23[2];
            }
            *(undefined4 *)((int)pdVar13 + -4) = uVar31;
            if (in_FPSCR_SZ == '\0') {
              pdVar13 = (double *)((int)pdVar13 + 0x34);
              *(undefined4 *)pdVar13 = uVar33;
              dVar29 = (double)CONCAT44(*(undefined4 *)((int)puVar18 + 4),SUB84(dVar32,0));
            }
            else {
              pdVar13 = pdVar13 + 6;
              *pdVar13 = dVar35;
              dVar29 = *(double *)((int)puVar18 + 4);
            }
            iVar10 = *piVar4;
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)(iVar10 + 4),*(undefined4 *)(iVar10 + 0x14));
            }
            else {
              dVar32 = *(double *)(iVar10 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                        SUB84(dVar32,0) * (float)((ulonglong)dVar29 >> 0x20));
            }
            else {
              dVar32 = dVar32 * dVar29;
            }
            if (in_FPSCR_SZ == '\0') {
              uVar28 = CONCAT44(*(undefined4 *)puVar18,SUB84(dVar29,0));
            }
            else {
              uVar28 = *puVar18;
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)(ulonglong)
                               (uint)((float)((ulonglong)uVar28 >> 0x20) *
                                      (float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0));
            }
            if (in_FPSCR_SZ == '\0') {
              uVar28 = CONCAT44(*(undefined4 *)(puVar18 + 1),(int)uVar28);
              uVar30 = CONCAT44(*(undefined4 *)(iVar10 + 0x24),SUB84(dVar32,0));
            }
            else {
              uVar28 = puVar18[1];
              uVar30 = *(ulonglong *)(iVar10 + 0x24);
            }
            if (in_FPSCR_PR == '\0') {
              uVar30 = (ulonglong)
                       (uint)((float)((ulonglong)uVar28 >> 0x20) * (float)(uVar30 >> 0x20) +
                             (float)uVar30);
            }
            dVar32 = in_xd14;
            dVar29 = in_xd14;
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(uVar37,(int)uVar28);
              dVar29 = (double)CONCAT44(uVar37,(int)uVar30);
            }
            if (in_FPSCR_PR == '\0') {
              dVar29 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) * SUB84(dVar29,0) +
                                        (float)((ulonglong)dVar29 >> 0x20),SUB84(dVar29,0));
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar13 = (double *)((int)pdVar13 + -4);
              *(int *)pdVar13 = (int)((ulonglong)dVar29 >> 0x20);
            }
            else {
              pdVar13 = pdVar13 + -1;
              *pdVar13 = dVar29;
            }
            uVar33 = DAT_8c61fd0c;
            pdVar11 = (double *)*piVar4;
            if (in_FPSCR_SZ == '\0') {
              dVar29 = (double)CONCAT44(*(undefined4 *)((int)puVar18 + 4),SUB84(dVar32,0));
              dVar32 = (double)CONCAT44(*(undefined4 *)pdVar11,*(undefined4 *)(pdVar11 + 2));
            }
            else {
              dVar29 = *(double *)((int)puVar18 + 4);
              dVar32 = *pdVar11;
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                        SUB84(dVar32,0) * (float)((ulonglong)dVar29 >> 0x20));
            }
            else {
              dVar32 = dVar32 * dVar29;
            }
            if (in_FPSCR_SZ == '\0') {
              uVar28 = CONCAT44(*(undefined4 *)puVar18,SUB84(dVar29,0));
            }
            else {
              uVar28 = *puVar18;
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)(ulonglong)
                               (uint)((float)((ulonglong)uVar28 >> 0x20) *
                                      (float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0));
            }
            if (in_FPSCR_SZ == '\0') {
              uVar28 = CONCAT44(*(undefined4 *)(puVar18 + 1),(int)uVar28);
              dVar29 = (double)CONCAT44(*(undefined4 *)(pdVar11 + 4),SUB84(dVar32,0));
            }
            else {
              uVar28 = puVar18[1];
              dVar29 = pdVar11[4];
            }
            if (in_FPSCR_PR == '\0') {
              dVar29 = (double)(ulonglong)
                               (uint)((float)((ulonglong)uVar28 >> 0x20) *
                                      (float)((ulonglong)dVar29 >> 0x20) + SUB84(dVar29,0));
            }
            dVar32 = in_xd14;
            in_dr0 = in_xd14;
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(uVar37,SUB84(dVar29,0));
              in_dr0 = (double)CONCAT44(uVar37,(int)uVar28);
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((float)((ulonglong)in_dr0 >> 0x20) * SUB84(dVar32,0) +
                                        (float)((ulonglong)dVar32 >> 0x20),SUB84(dVar32,0));
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)pdVar13 + -4) = (int)((ulonglong)dVar32 >> 0x20);
              *(undefined4 *)(pdVar13 + -1) = *(undefined4 *)(puVar18 + 3);
              *(undefined4 *)((int)pdVar13 + -0xc) = *(undefined4 *)((int)puVar18 + 0x14);
              pdVar13 = pdVar13 + -2;
              *(undefined4 *)pdVar13 = *(undefined4 *)(puVar18 + 2);
            }
            else {
              pdVar13[-1] = dVar32;
              pdVar13[-2] = (double)puVar18[3];
              pdVar13[-3] = *(double *)((int)puVar18 + 0x14);
              pdVar13 = pdVar13 + -4;
              *pdVar13 = (double)puVar18[2];
            }
            *(undefined4 *)((int)pdVar13 + -4) = uVar33;
            *DAT_8c61fd10 = *DAT_8c61fd10 + 4;
            unaff_r8 = (int)pdVar13 + 0x1c;
          }
        }
        else {
          *DAT_8c61fd14 = 1;
          if ((*(uint *)((int)puVar8 + 0xc) & *(uint *)((int)puVar20 + 0xc) &
               *(uint *)((int)puVar23 + 0xc) & 1) == 0) {
            in_dr0 = (double)(*(code *)PTR_FUN_8c61fd18)(puVar20,puVar23);
          }
          if ((*(uint *)((int)puVar20 + 0xc) & *(uint *)((int)puVar23 + 0xc) &
               *(uint *)((int)puVar18 + 0xc) & 1) == 0) {
            in_dr0 = (double)(*(code *)PTR_FUN_8c61fd18)(puVar20,puVar23,puVar18);
          }
        }
      }
    }
  }
  else if (uVar6 == DAT_8c61f83c) {
    for (iVar14 = 0; uVar33 = SUB84(param_1,0),
        iVar14 < (int)(uint)*(ushort *)(*(int *)((int)puVar26 + -0xc) + 2); iVar14 = iVar14 + 1) {
      uVar6 = 0;
      puVar17 = puVar15 + 1;
      iVar10 = 0;
      uVar21 = (uint)*puVar15;
      *DAT_8c61ffbc = *DAT_8c61ffbc + uVar21;
      piVar4 = DAT_8c61ffc0;
      uVar12 = 0xffffffff;
      puVar9 = puVar17;
      if (uVar21 != 0) {
        do {
          uVar1 = *puVar9;
          puVar9 = puVar9 + 1;
          iVar10 = iVar10 + 1;
          iVar24 = (uint)uVar1 * 0x20 + *DAT_8c61ffc0;
          *(int *)((int)puVar26 + -8) = iVar24;
          uVar25 = *(uint *)(iVar24 + 0xc);
          uVar12 = uVar12 & uVar25;
          uVar6 = uVar6 | uVar25;
        } while (iVar10 < (int)uVar21);
      }
      if ((uVar12 & 0x3f) == 0) {
        if ((uVar6 & 1) == 0) {
          iVar24 = *DAT_8c61ffc0;
          iVar10 = (uint)puVar15[2] * 0x20 + iVar24;
          if (in_FPSCR_SZ == '\0') {
            uVar33 = *(undefined4 *)(iVar10 + 0x10);
          }
          else {
            in_xd4 = *(double *)(iVar10 + 0x10);
          }
          iVar19 = (uint)*puVar17 * 0x20 + iVar24;
          dVar29 = in_xd4;
          if (in_FPSCR_SZ == '\0') {
            dVar29 = (double)CONCAT44(uVar33,*(undefined4 *)(iVar19 + 0x10));
          }
          if (in_FPSCR_PR == '\0') {
            dVar29 = (double)CONCAT44((float)((ulonglong)dVar29 >> 0x20) - SUB84(dVar29,0),
                                      SUB84(dVar29,0));
          }
          else {
            dVar29 = dVar29 - dVar29;
          }
          iVar24 = (uint)puVar15[3] * 0x20 + iVar24;
          if (in_FPSCR_SZ == '\0') {
            fVar34 = *(float *)(iVar10 + 0x14);
            dVar32 = (double)CONCAT44(fVar34,uVar33);
            dVar29 = (double)CONCAT44((int)((ulonglong)dVar29 >> 0x20),
                                      *(undefined4 *)(iVar24 + 0x14));
            in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar34);
          }
          else {
            dVar32 = *(double *)(iVar10 + 0x14);
            fVar34 = (float)((ulonglong)dVar32 >> 0x20);
          }
          if (in_FPSCR_PR == '\0') {
            fVar34 = SUB84(dVar29,0) - fVar34;
            param_1 = (double)CONCAT44((float)((ulonglong)dVar29 >> 0x20) * fVar34,fVar34);
          }
          else {
            param_1 = (dVar29 - dVar32) * (dVar29 - dVar32);
          }
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),
                                       *(undefined4 *)(iVar19 + 0x14));
          }
          if (in_FPSCR_PR == '\0') {
            dVar29 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                      SUB84(in_dr0,0) - SUB84(param_1,0));
          }
          else {
            dVar29 = in_dr0 - param_1;
          }
          uVar33 = (undefined4)((ulonglong)param_1 >> 0x20);
          dVar27 = param_1;
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44(uVar33,SUB84(dVar32,0));
            dVar27 = (double)CONCAT44(uVar33,*(undefined4 *)(iVar24 + 0x10));
          }
          if (in_FPSCR_PR == '\0') {
            fVar34 = SUB84(dVar27,0) - SUB84(param_1,0);
            dVar27 = (double)CONCAT44(uVar33,fVar34);
            in_dr0 = (double)CONCAT44((int)((ulonglong)dVar29 >> 0x20),SUB84(dVar29,0) * fVar34);
          }
          else {
            dVar27 = dVar27 - param_1;
            in_dr0 = dVar29 * dVar27;
          }
          if (in_FPSCR_PR == '\0') {
            fVar34 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
            param_1 = (double)CONCAT44(fVar34,SUB84(param_1,0));
            bVar38 = 0.0 < fVar34;
          }
          else {
            param_1 = param_1 - in_dr0;
            bVar38 = (double)((ulonglong)dVar27 & 0xffffffff00000000) < param_1;
          }
          if (bVar38) goto LAB_8c61fdb8;
          *DAT_8c61ffc4 = *DAT_8c61ffc4 + uVar21;
          iVar10 = 0;
          uVar37 = (undefined4)((ulonglong)dVar35 >> 0x20);
          uVar33 = SUB84(dVar35,0);
          if (0 < (int)(uVar21 - 1)) {
            do {
              uVar1 = *puVar17;
              puVar17 = puVar17 + 1;
              iVar24 = *DAT_8c61ffc0;
              if (in_FPSCR_SZ == '\0') {
                pdVar13 = (double *)(unaff_r8 + 0x18);
                *(undefined4 *)pdVar13 = uVar37;
              }
              else {
                pdVar13 = (double *)(unaff_r8 + 0x14);
                *pdVar13 = dVar35;
              }
              piVar4 = DAT_8c61ffc8;
              puVar8 = (undefined8 *)((uint)uVar1 * 0x20 + iVar24);
              if (in_FPSCR_SZ == '\0') {
                dVar29 = (double)CONCAT44(*(undefined4 *)((int)puVar8 + 4),SUB84(in_dr0,0));
              }
              else {
                dVar29 = *(double *)((int)puVar8 + 4);
              }
              iVar24 = *DAT_8c61ffc8;
              if (in_FPSCR_SZ == '\0') {
                dVar32 = (double)CONCAT44(*(undefined4 *)(iVar24 + 4),*(undefined4 *)(iVar24 + 0x14)
                                         );
              }
              else {
                dVar32 = *(double *)(iVar24 + 4);
              }
              if (in_FPSCR_PR == '\0') {
                dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                          SUB84(dVar32,0) * (float)((ulonglong)dVar29 >> 0x20));
              }
              else {
                dVar32 = dVar32 * dVar29;
              }
              if (in_FPSCR_SZ == '\0') {
                uVar28 = CONCAT44(*(undefined4 *)puVar8,SUB84(dVar29,0));
              }
              else {
                uVar28 = *puVar8;
              }
              if (in_FPSCR_PR == '\0') {
                dVar32 = (double)(ulonglong)
                                 (uint)((float)((ulonglong)uVar28 >> 0x20) *
                                        (float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0));
              }
              if (in_FPSCR_SZ == '\0') {
                uVar28 = CONCAT44(*(undefined4 *)(puVar8 + 1),(int)uVar28);
                uVar30 = CONCAT44(*(undefined4 *)(iVar24 + 0x24),SUB84(dVar32,0));
              }
              else {
                uVar28 = puVar8[1];
                uVar30 = *(ulonglong *)(iVar24 + 0x24);
              }
              if (in_FPSCR_PR == '\0') {
                uVar30 = (ulonglong)
                         (uint)((float)((ulonglong)uVar28 >> 0x20) * (float)(uVar30 >> 0x20) +
                               (float)uVar30);
              }
              dVar32 = in_xd14;
              dVar29 = in_xd14;
              if (in_FPSCR_SZ == '\0') {
                dVar32 = (double)CONCAT44(uVar33,(int)uVar28);
                dVar29 = (double)CONCAT44(uVar33,(int)uVar30);
              }
              if (in_FPSCR_PR == '\0') {
                dVar29 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) * SUB84(dVar29,0) +
                                          (float)((ulonglong)dVar29 >> 0x20),SUB84(dVar29,0));
              }
              if (in_FPSCR_SZ == '\0') {
                pdVar13 = (double *)((int)pdVar13 + -4);
                *(int *)pdVar13 = (int)((ulonglong)dVar29 >> 0x20);
              }
              else {
                pdVar13 = pdVar13 + -1;
                *pdVar13 = dVar29;
              }
              uVar31 = DAT_8c61ffcc;
              pdVar11 = (double *)*piVar4;
              if (in_FPSCR_SZ == '\0') {
                dVar29 = (double)CONCAT44(*(undefined4 *)((int)puVar8 + 4),SUB84(dVar32,0));
                dVar32 = (double)CONCAT44(*(undefined4 *)pdVar11,*(undefined4 *)(pdVar11 + 2));
              }
              else {
                dVar29 = *(double *)((int)puVar8 + 4);
                dVar32 = *pdVar11;
              }
              if (in_FPSCR_PR == '\0') {
                dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                          SUB84(dVar32,0) * (float)((ulonglong)dVar29 >> 0x20));
              }
              else {
                dVar32 = dVar32 * dVar29;
              }
              if (in_FPSCR_SZ == '\0') {
                uVar28 = CONCAT44(*(undefined4 *)puVar8,SUB84(dVar29,0));
              }
              else {
                uVar28 = *puVar8;
              }
              if (in_FPSCR_PR == '\0') {
                dVar32 = (double)(ulonglong)
                                 (uint)((float)((ulonglong)uVar28 >> 0x20) *
                                        (float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0));
              }
              if (in_FPSCR_SZ == '\0') {
                uVar28 = CONCAT44(*(undefined4 *)(puVar8 + 1),(int)uVar28);
                dVar29 = (double)CONCAT44(*(undefined4 *)(pdVar11 + 4),SUB84(dVar32,0));
              }
              else {
                uVar28 = puVar8[1];
                dVar29 = pdVar11[4];
              }
              if (in_FPSCR_PR == '\0') {
                dVar29 = (double)(ulonglong)
                                 (uint)((float)((ulonglong)uVar28 >> 0x20) *
                                        (float)((ulonglong)dVar29 >> 0x20) + SUB84(dVar29,0));
              }
              dVar32 = in_xd14;
              in_dr0 = in_xd14;
              if (in_FPSCR_SZ == '\0') {
                dVar32 = (double)CONCAT44(uVar33,SUB84(dVar29,0));
                in_dr0 = (double)CONCAT44(uVar33,(int)uVar28);
              }
              if (in_FPSCR_PR == '\0') {
                dVar32 = (double)CONCAT44((float)((ulonglong)in_dr0 >> 0x20) * SUB84(dVar32,0) +
                                          (float)((ulonglong)dVar32 >> 0x20),SUB84(dVar32,0));
              }
              if (in_FPSCR_SZ == '\0') {
                *(int *)((int)pdVar13 + -4) = (int)((ulonglong)dVar32 >> 0x20);
                *(undefined4 *)(pdVar13 + -1) = *(undefined4 *)(puVar8 + 3);
                *(undefined4 *)((int)pdVar13 + -0xc) = *(undefined4 *)((int)puVar8 + 0x14);
                pdVar13 = pdVar13 + -2;
                *(undefined4 *)pdVar13 = *(undefined4 *)(puVar8 + 2);
              }
              else {
                pdVar13[-1] = dVar32;
                pdVar13[-2] = (double)puVar8[3];
                pdVar13[-3] = *(double *)((int)puVar8 + 0x14);
                pdVar13 = pdVar13 + -4;
                *pdVar13 = (double)puVar8[2];
              }
              *(undefined4 *)((int)pdVar13 + -4) = uVar31;
              iVar10 = iVar10 + 1;
              unaff_r8 = (int)pdVar13 + 0x1c;
            } while (iVar10 < (int)(uVar21 - 1));
          }
          uVar1 = *puVar17;
          puVar15 = puVar17 + 1;
          iVar10 = *DAT_8c61ffc0;
          if (in_FPSCR_SZ == '\0') {
            pdVar13 = (double *)(unaff_r8 + 0x18);
            *(undefined4 *)pdVar13 = uVar37;
          }
          else {
            pdVar13 = (double *)(unaff_r8 + 0x14);
            *pdVar13 = dVar35;
          }
          piVar4 = DAT_8c61ffc8;
          puVar8 = (undefined8 *)((uint)uVar1 * 0x20 + iVar10);
          if (in_FPSCR_SZ == '\0') {
            dVar29 = (double)CONCAT44(*(undefined4 *)((int)puVar8 + 4),SUB84(in_dr0,0));
          }
          else {
            dVar29 = *(double *)((int)puVar8 + 4);
          }
          iVar10 = *DAT_8c61ffc8;
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44(*(undefined4 *)(iVar10 + 4),*(undefined4 *)(iVar10 + 0x14));
          }
          else {
            dVar32 = *(double *)(iVar10 + 4);
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                      SUB84(dVar32,0) * (float)((ulonglong)dVar29 >> 0x20));
          }
          else {
            dVar32 = dVar32 * dVar29;
          }
          if (in_FPSCR_SZ == '\0') {
            uVar28 = CONCAT44(*(undefined4 *)puVar8,SUB84(dVar29,0));
          }
          else {
            uVar28 = *puVar8;
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)(ulonglong)
                             (uint)((float)((ulonglong)uVar28 >> 0x20) *
                                    (float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0));
          }
          if (in_FPSCR_SZ == '\0') {
            uVar28 = CONCAT44(*(undefined4 *)(puVar8 + 1),(int)uVar28);
            uVar30 = CONCAT44(*(undefined4 *)(iVar10 + 0x24),SUB84(dVar32,0));
          }
          else {
            uVar28 = puVar8[1];
            uVar30 = *(ulonglong *)(iVar10 + 0x24);
          }
          if (in_FPSCR_PR == '\0') {
            uVar30 = (ulonglong)
                     (uint)((float)((ulonglong)uVar28 >> 0x20) * (float)(uVar30 >> 0x20) +
                           (float)uVar30);
          }
          dVar32 = in_xd14;
          dVar29 = in_xd14;
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44(uVar33,(int)uVar28);
            dVar29 = (double)CONCAT44(uVar33,(int)uVar30);
          }
          if (in_FPSCR_PR == '\0') {
            dVar29 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) * SUB84(dVar29,0) +
                                      (float)((ulonglong)dVar29 >> 0x20),SUB84(dVar29,0));
          }
          if (in_FPSCR_SZ == '\0') {
            pdVar13 = (double *)((int)pdVar13 + -4);
            *(int *)pdVar13 = (int)((ulonglong)dVar29 >> 0x20);
          }
          else {
            pdVar13 = pdVar13 + -1;
            *pdVar13 = dVar29;
          }
          uVar37 = DAT_8c61ffd0;
          pdVar11 = (double *)*piVar4;
          if (in_FPSCR_SZ == '\0') {
            dVar29 = (double)CONCAT44(*(undefined4 *)((int)puVar8 + 4),SUB84(dVar32,0));
            dVar32 = (double)CONCAT44(*(undefined4 *)pdVar11,*(undefined4 *)(pdVar11 + 2));
          }
          else {
            dVar29 = *(double *)((int)puVar8 + 4);
            dVar32 = *pdVar11;
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                      SUB84(dVar32,0) * (float)((ulonglong)dVar29 >> 0x20));
          }
          else {
            dVar32 = dVar32 * dVar29;
          }
          if (in_FPSCR_SZ == '\0') {
            uVar28 = CONCAT44(*(undefined4 *)puVar8,SUB84(dVar29,0));
          }
          else {
            uVar28 = *puVar8;
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)(ulonglong)
                             (uint)((float)((ulonglong)uVar28 >> 0x20) *
                                    (float)((ulonglong)dVar32 >> 0x20) + SUB84(dVar32,0));
          }
          if (in_FPSCR_SZ == '\0') {
            uVar28 = CONCAT44(*(undefined4 *)(puVar8 + 1),(int)uVar28);
            dVar29 = (double)CONCAT44(*(undefined4 *)(pdVar11 + 4),SUB84(dVar32,0));
          }
          else {
            uVar28 = puVar8[1];
            dVar29 = pdVar11[4];
          }
          if (in_FPSCR_PR == '\0') {
            dVar29 = (double)(ulonglong)
                             (uint)((float)((ulonglong)uVar28 >> 0x20) *
                                    (float)((ulonglong)dVar29 >> 0x20) + SUB84(dVar29,0));
          }
          dVar32 = in_xd14;
          in_dr0 = in_xd14;
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44(uVar33,SUB84(dVar29,0));
            in_dr0 = (double)CONCAT44(uVar33,(int)uVar28);
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((float)((ulonglong)in_dr0 >> 0x20) * SUB84(dVar32,0) +
                                      (float)((ulonglong)dVar32 >> 0x20),SUB84(dVar32,0));
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)pdVar13 + -4) = (int)((ulonglong)dVar32 >> 0x20);
            *(undefined4 *)(pdVar13 + -1) = *(undefined4 *)(puVar8 + 3);
            *(undefined4 *)((int)pdVar13 + -0xc) = *(undefined4 *)((int)puVar8 + 0x14);
            pdVar13 = pdVar13 + -2;
            *(undefined4 *)pdVar13 = *(undefined4 *)(puVar8 + 2);
          }
          else {
            pdVar13[-1] = dVar32;
            pdVar13[-2] = (double)puVar8[3];
            pdVar13[-3] = *(double *)((int)puVar8 + 0x14);
            pdVar13 = pdVar13 + -4;
            *pdVar13 = (double)puVar8[2];
          }
          *(undefined4 *)((int)pdVar13 + -4) = uVar37;
          unaff_r8 = (int)pdVar13 + 0x1c;
        }
        else {
          *DAT_8c61ffd4 = 0;
          puVar9 = puVar15 + 2;
          puVar15 = puVar15 + 3;
          iVar19 = 0;
          iVar10 = (uint)*puVar17 * 0x20 + *piVar4;
          iVar24 = (uint)*puVar9 * 0x20 + *piVar4;
          if (0 < (int)(uVar21 - 2)) {
            do {
              uVar1 = *puVar15;
              puVar15 = puVar15 + 1;
              iVar22 = (uint)uVar1 * 0x20 + *DAT_8c61ffc0;
              if ((*(uint *)(iVar10 + 0xc) & *(uint *)(iVar24 + 0xc) & *(uint *)(iVar22 + 0xc) & 1)
                  == 0) {
                in_dr0 = (double)(*(code *)PTR_FUN_8c61ffd8)(iVar24,iVar22);
              }
              iVar19 = iVar19 + 1;
              iVar10 = iVar24;
              iVar24 = iVar22;
            } while (iVar19 < (int)(uVar21 - 2));
          }
        }
      }
      else {
LAB_8c61fdb8:
        puVar15 = puVar17 + uVar21;
      }
    }
  }
  else if (uVar6 == DAT_8c61f838) {
    *DAT_8c61ffdc = *(undefined4 *)((int)puVar26 + -0xc);
    fVar34 = (float)((ulonglong)param_1 >> 0x20);
    fVar36 = (float)((ulonglong)param_3 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)puVar26 + -0x10) = (int)((ulonglong)in_dr0 >> 0x20);
      *(int *)((int)puVar26 + -0x14) = SUB84(in_dr0,0);
      *(int *)((int)puVar26 + -0x18) = (int)((ulonglong)in_dr2 >> 0x20);
      *(int *)((int)puVar26 + -0x1c) = (int)in_dr2;
      *(float *)((int)puVar26 + -0x20) = fVar34;
      *(float *)((int)puVar26 + -0x24) = SUB84(param_1,0);
      *(int *)((int)puVar26 + -0x28) = (int)((ulonglong)param_2 >> 0x20);
      *(int *)((int)puVar26 + -0x2c) = (int)param_2;
      *(float *)((int)puVar26 + -0x30) = fVar36;
      *(float *)((int)puVar26 + -0x34) = SUB84(param_3,0);
      *(int *)((int)puVar26 + -0x38) = (int)((ulonglong)param_4 >> 0x20);
      *(int *)((int)puVar26 + -0x3c) = (int)param_4;
      *(int *)((int)puVar26 + -0x40) = (int)((ulonglong)in_dr12 >> 0x20);
      *(int *)((int)puVar26 + -0x44) = (int)in_dr12;
      fVar2 = DAT_8c61ffb4;
    }
    else {
      *(double *)((int)puVar26 + -0x14) = in_dr0;
      *(undefined8 *)((int)puVar26 + -0x1c) = in_xd0;
      *(undefined8 *)((int)puVar26 + -0x24) = in_dr2;
      *(undefined8 *)((int)puVar26 + -0x2c) = in_xd2;
      *(double *)((int)puVar26 + -0x34) = param_1;
      *(double *)((int)puVar26 + -0x3c) = in_xd4;
      *(undefined8 *)((int)puVar26 + -0x44) = param_2;
      *(undefined8 *)((int)puVar26 + -0x4c) = in_xd6;
      *(double *)((int)puVar26 + -0x54) = param_3;
      *(undefined8 *)((int)puVar26 + -0x5c) = in_xd8;
      *(undefined8 *)((int)puVar26 + -100) = param_4;
      *(undefined8 *)((int)puVar26 + -0x6c) = in_xd10;
      *(undefined8 *)((int)puVar26 + -0x74) = in_dr12;
      *(undefined8 *)((int)puVar26 + -0x7c) = in_xd12;
      fVar2 = fRam8c61ffb8;
    }
    dVar35 = (double)CONCAT44(fVar2,DAT_8c61ffb4);
    if (in_FPSCR_PR == '\0') {
      dVar29 = (double)CONCAT44(fVar34 * fVar2,SUB84(param_1,0) * fVar2);
      dVar35 = (double)CONCAT44(fVar36 * fVar2,SUB84(param_3,0) * fVar2);
    }
    else {
      dVar29 = param_1 * dVar35 * dVar35;
      dVar35 = param_3 * dVar35 * dVar35;
    }
    (*(code *)PTR_FUN_8c61ffe0)(dVar29,dVar35,*DAT_8c61ffdc);
  }
  return;
}

