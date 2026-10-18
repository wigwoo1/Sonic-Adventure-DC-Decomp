/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c61a488(double param_1,undefined8 param_2,double param_3,undefined8 param_4,
                 ushort *param_5)

{
  ushort uVar1;
  float fVar2;
  undefined4 uVar3;
  int *piVar4;
  int *piVar5;
  uint uVar6;
  double *pdVar7;
  double *pdVar8;
  ushort *puVar9;
  int iVar10;
  uint uVar11;
  int unaff_r8;
  double *pdVar12;
  int iVar13;
  ushort *puVar14;
  ushort *puVar15;
  ushort *puVar16;
  double *pdVar17;
  int iVar18;
  double *pdVar19;
  uint uVar20;
  int iVar21;
  double *pdVar22;
  int iVar23;
  uint uVar24;
  undefined8 *puVar25;
  float fVar26;
  double in_dr0;
  double dVar27;
  undefined8 in_dr2;
  double dVar28;
  ulonglong uVar29;
  double dVar30;
  float fVar32;
  double dVar33;
  undefined8 in_dr12;
  undefined8 in_dr14;
  undefined8 in_xd0;
  double in_xd2;
  double in_xd4;
  undefined8 in_xd6;
  undefined8 in_xd8;
  undefined8 in_xd10;
  undefined8 in_xd12;
  double in_xd14;
  bool bVar34;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined8 local_20;
  undefined4 uVar31;
  
  if (in_FPSCR_SZ == '\0') {
    puVar25 = (undefined8 *)((int)&local_20 + 4);
    local_20 = (double)CONCAT44((int)in_dr14,(undefined4)local_20);
  }
  else {
    puVar25 = &local_20;
    local_20 = in_xd14;
  }
  *(undefined4 *)((int)puVar25 + -4) = in_PR;
  if (in_FPSCR_SZ == '\0') {
    in_dr14 = CONCAT44((int)((ulonglong)in_dr14 >> 0x20),DAT_8c61a4f0);
  }
  else {
    in_xd14 = (double)CONCAT44(_DAT_8c61a4f4,DAT_8c61a4f0);
  }
  *(ushort **)((int)puVar25 + -0xc) = param_5;
  uVar6 = *param_5 & _DAT_8c61a4f4;
  puVar14 = *(ushort **)(param_5 + 2);
  if (uVar6 == 0) {
    *DAT_8c61a4fc = *DAT_8c61a4fc + (uint)*(ushort *)(*(int *)((int)puVar25 + -0xc) + 2) * 3;
    for (iVar13 = 0; piVar5 = DAT_8c61a77c, uVar31 = SUB84(param_1,0),
        iVar13 < (int)(uint)*(ushort *)(*(int *)((int)puVar25 + -0xc) + 2); iVar13 = iVar13 + 1) {
      uVar1 = *puVar14;
      puVar9 = puVar14 + 1;
      puVar16 = puVar14 + 2;
      iVar10 = *DAT_8c61a76c;
      puVar14 = puVar14 + 3;
      pdVar8 = (double *)((uint)uVar1 * 0x20 + iVar10);
      pdVar17 = (double *)((uint)*puVar9 * 0x20 + iVar10);
      pdVar19 = (double *)((uint)*puVar16 * 0x20 + iVar10);
      if ((*(uint *)((int)pdVar8 + 0xc) & *(uint *)((int)pdVar17 + 0xc) &
           *(uint *)((int)pdVar19 + 0xc) & 0x3f) == 0) {
        if (((*(uint *)((int)pdVar8 + 0xc) | *(uint *)((int)pdVar17 + 0xc) |
             *(uint *)((int)pdVar19 + 0xc)) & 1) == 0) {
          if (in_FPSCR_SZ == '\0') {
            uVar31 = *(undefined4 *)(pdVar17 + 2);
            dVar28 = (double)CONCAT44(uVar31,*(undefined4 *)(pdVar8 + 2));
          }
          else {
            dVar28 = pdVar17[2];
            in_xd2 = pdVar8[2];
          }
          if (in_FPSCR_PR == '\0') {
            dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) - SUB84(dVar28,0),
                                      SUB84(dVar28,0));
          }
          else {
            dVar28 = dVar28 - dVar28;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),
                                      *(undefined4 *)((int)pdVar19 + 0x14));
            dVar30 = (double)CONCAT44(*(undefined4 *)((int)pdVar17 + 0x14),uVar31);
          }
          else {
            in_xd2 = *(double *)((int)pdVar19 + 0x14);
            dVar30 = *(double *)((int)pdVar17 + 0x14);
          }
          fVar32 = (float)((ulonglong)dVar30 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),SUB84(dVar28,0) - fVar32);
          }
          else {
            dVar28 = dVar28 - dVar30;
          }
          if (in_FPSCR_SZ == '\0') {
            in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar32);
          }
          if (in_FPSCR_PR == '\0') {
            dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) * SUB84(dVar28,0),
                                      SUB84(dVar28,0));
          }
          else {
            dVar28 = dVar28 * dVar28;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),
                                      *(undefined4 *)((int)pdVar8 + 0x14));
          }
          else {
            in_xd2 = *(double *)((int)pdVar8 + 0x14);
          }
          if (in_FPSCR_PR == '\0') {
            dVar27 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                      SUB84(in_dr0,0) - SUB84(dVar28,0));
          }
          else {
            dVar27 = in_dr0 - dVar28;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),*(undefined4 *)(pdVar19 + 2))
            ;
          }
          else {
            in_xd2 = pdVar19[2];
          }
          if (in_FPSCR_PR == '\0') {
            dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),
                                      SUB84(dVar28,0) - SUB84(dVar30,0));
          }
          else {
            dVar28 = dVar28 - dVar30;
          }
          uVar6 = (uint)((ulonglong)dVar28 >> 0x20);
          param_1 = dVar28;
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44(uVar6,SUB84(dVar30,0));
          }
          if (in_FPSCR_PR == '\0') {
            in_dr0 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),
                                      SUB84(dVar27,0) * SUB84(dVar28,0));
          }
          else {
            in_dr0 = dVar27 * dVar28;
          }
          if (in_FPSCR_PR == '\0') {
            fVar32 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
            param_1 = (double)CONCAT44(fVar32,SUB84(param_1,0));
            bVar34 = 0.0 < fVar32;
          }
          else {
            param_1 = param_1 - in_dr0;
            bVar34 = (double)((ulonglong)uVar6 << 0x20) < param_1;
          }
          if (!bVar34) {
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)DAT_8c61a770,*(undefined4 *)pdVar8);
            }
            else {
              in_xd2 = *pdVar8;
              dVar28 = *DAT_8c61a770;
            }
            if (in_FPSCR_PR == '\0') {
              dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) * SUB84(dVar28,0),
                                        SUB84(dVar28,0));
            }
            else {
              dVar28 = dVar28 * dVar28;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),*(undefined4 *)DAT_8c61a774
                                       );
              in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),*DAT_8c61a778);
            }
            else {
              in_xd2 = *DAT_8c61a774;
            }
            if (in_FPSCR_PR == '\0') {
              fVar32 = SUB84(dVar28,0) - (float)((ulonglong)dVar28 >> 0x20);
            }
            else {
              fVar32 = SUB84(dVar28 - dVar28,0);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)((int)pdVar8 + 4),fVar32);
            }
            else {
              dVar28 = *(double *)((int)pdVar8 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar30 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                        SUB84(in_dr0,0) * (float)((ulonglong)dVar28 >> 0x20));
            }
            else {
              dVar30 = in_dr0 * dVar28;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar8 + 1),SUB84(dVar28,0));
            }
            else {
              dVar28 = pdVar8[1];
            }
            if (in_FPSCR_PR == '\0') {
              dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),
                                        SUB84(dVar28,0) - SUB84(dVar30,0));
            }
            else {
              dVar28 = dVar28 - dVar30;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar30 = (double)CONCAT44((int)((ulonglong)dVar30 >> 0x20),
                                        *(undefined4 *)(DAT_8c61a770 + 1));
            }
            fVar32 = (float)((ulonglong)dVar28 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              fVar26 = SUB84(dVar30,0) * fVar32;
              dVar28 = (double)CONCAT44(fVar32,SUB84(dVar28,0) - fVar26);
            }
            else {
              fVar26 = SUB84(dVar30 * dVar28,0);
              dVar28 = dVar28 - dVar30 * dVar28;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar22 = (double *)(unaff_r8 + 0x18);
              *(int *)pdVar22 = SUB84(dVar28,0);
            }
            else {
              pdVar22 = (double *)(unaff_r8 + 0x14);
              *pdVar22 = in_xd2;
            }
            piVar4 = DAT_8c61a77c;
            iVar10 = *piVar5;
            if (in_FPSCR_SZ == '\0') {
              dVar30 = (double)CONCAT44(*(undefined4 *)((int)pdVar8 + 4),fVar26);
              dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),
                                        *(undefined4 *)(iVar10 + 0x14));
            }
            else {
              dVar30 = *(double *)((int)pdVar8 + 4);
              in_xd2 = *(double *)(iVar10 + 0x14);
            }
            if (in_FPSCR_PR == '\0') {
              fVar32 = SUB84(dVar28,0) * (float)((ulonglong)dVar30 >> 0x20);
            }
            else {
              fVar32 = SUB84(dVar28 * dVar30,0);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)pdVar8,SUB84(dVar30,0));
              uVar29 = CONCAT44(*(undefined4 *)(iVar10 + 4),fVar32);
            }
            else {
              dVar28 = *pdVar8;
              uVar29 = *(ulonglong *)(iVar10 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              uVar29 = (ulonglong)
                       (uint)((float)((ulonglong)dVar28 >> 0x20) * (float)(uVar29 >> 0x20) +
                             (float)uVar29);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar8 + 1),SUB84(dVar28,0));
              uVar29 = CONCAT44(*(undefined4 *)(iVar10 + 0x24),(int)uVar29);
            }
            else {
              dVar28 = pdVar8[1];
              uVar29 = *(ulonglong *)(iVar10 + 0x24);
            }
            if (in_FPSCR_PR == '\0') {
              uVar29 = (ulonglong)
                       (uint)((float)((ulonglong)dVar28 >> 0x20) * (float)(uVar29 >> 0x20) +
                             (float)uVar29);
            }
            uVar31 = (undefined4)in_dr14;
            dVar27 = in_xd14;
            dVar30 = in_xd14;
            if (in_FPSCR_SZ == '\0') {
              dVar27 = (double)CONCAT44(uVar31,(int)uVar29);
              dVar30 = (double)CONCAT44(uVar31,SUB84(dVar28,0));
            }
            if (in_FPSCR_PR == '\0') {
              dVar27 = (double)CONCAT44((float)((ulonglong)dVar30 >> 0x20) * SUB84(dVar27,0) +
                                        (float)((ulonglong)dVar27 >> 0x20),SUB84(dVar27,0));
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar22 = (double *)((int)pdVar22 + -4);
              *(int *)pdVar22 = (int)((ulonglong)dVar27 >> 0x20);
              dVar28 = (double)CONCAT44(*(undefined4 *)((int)pdVar8 + 4),SUB84(dVar30,0));
            }
            else {
              pdVar22 = pdVar22 + -1;
              *pdVar22 = dVar27;
              dVar28 = *(double *)((int)pdVar8 + 4);
            }
            pdVar12 = (double *)*piVar4;
            if (in_FPSCR_SZ == '\0') {
              dVar30 = (double)CONCAT44(*(undefined4 *)pdVar12,*(undefined4 *)(pdVar12 + 2));
            }
            else {
              in_xd2 = pdVar12[2];
              dVar30 = *pdVar12;
            }
            if (in_FPSCR_PR == '\0') {
              dVar30 = (double)CONCAT44((int)((ulonglong)dVar30 >> 0x20),
                                        SUB84(dVar30,0) * (float)((ulonglong)dVar28 >> 0x20));
            }
            else {
              dVar30 = dVar30 * dVar28;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)pdVar8,SUB84(dVar28,0));
            }
            else {
              dVar28 = *pdVar8;
            }
            if (in_FPSCR_PR == '\0') {
              dVar30 = (double)(ulonglong)
                               (uint)((float)((ulonglong)dVar28 >> 0x20) *
                                      (float)((ulonglong)dVar30 >> 0x20) + SUB84(dVar30,0));
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar8 + 1),SUB84(dVar28,0));
              dVar30 = (double)CONCAT44(*(undefined4 *)(pdVar12 + 4),SUB84(dVar30,0));
            }
            else {
              dVar28 = pdVar8[1];
              dVar30 = pdVar12[4];
            }
            if (in_FPSCR_PR == '\0') {
              dVar30 = (double)(ulonglong)
                               (uint)((float)((ulonglong)dVar28 >> 0x20) *
                                      (float)((ulonglong)dVar30 >> 0x20) + SUB84(dVar30,0));
            }
            dVar33 = in_xd14;
            dVar27 = in_xd14;
            if (in_FPSCR_SZ == '\0') {
              dVar33 = (double)CONCAT44(uVar31,SUB84(dVar30,0));
              dVar27 = (double)CONCAT44(uVar31,SUB84(dVar28,0));
            }
            fVar32 = (float)((ulonglong)dVar27 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar33 = (double)CONCAT44(fVar32 * SUB84(dVar33,0) +
                                        (float)((ulonglong)dVar33 >> 0x20),SUB84(dVar33,0));
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)pdVar22 + -4) = (int)((ulonglong)dVar33 >> 0x20);
              *(undefined4 *)(pdVar22 + -1) = *(undefined4 *)(pdVar8 + 3);
              uVar3 = DAT_8c61a780;
              *(undefined4 *)((int)pdVar22 + -0xc) = *(undefined4 *)((int)pdVar8 + 0x14);
              pdVar22 = pdVar22 + -2;
              *(undefined4 *)pdVar22 = *(undefined4 *)(pdVar8 + 2);
            }
            else {
              pdVar22[-1] = dVar33;
              pdVar22[-2] = pdVar8[3];
              uVar3 = DAT_8c61a780;
              pdVar22[-3] = *(double *)((int)pdVar8 + 0x14);
              in_xd2 = pdVar8[2];
              pdVar22 = pdVar22 + -4;
              *pdVar22 = in_xd2;
            }
            *(undefined4 *)((int)pdVar22 + -4) = uVar3;
            piVar5 = DAT_8c61a77c;
            pdVar8 = DAT_8c61a774;
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)DAT_8c61a770,*(undefined4 *)pdVar17);
            }
            else {
              in_xd2 = *pdVar17;
              dVar28 = *DAT_8c61a770;
            }
            if (in_FPSCR_PR == '\0') {
              dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) * SUB84(dVar28,0),
                                        SUB84(dVar28,0));
            }
            else {
              dVar28 = dVar28 * dVar28;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),*(undefined4 *)DAT_8c61a774
                                       );
              dVar27 = (double)CONCAT44(fVar32,*DAT_8c61a778);
            }
            else {
              in_xd2 = *DAT_8c61a774;
            }
            if (in_FPSCR_PR == '\0') {
              fVar32 = SUB84(dVar28,0) - (float)((ulonglong)dVar28 >> 0x20);
            }
            else {
              fVar32 = SUB84(dVar28 - dVar28,0);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)((int)pdVar17 + 4),fVar32);
            }
            else {
              dVar28 = *(double *)((int)pdVar17 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),
                                        SUB84(dVar27,0) * (float)((ulonglong)dVar28 >> 0x20));
            }
            else {
              dVar27 = dVar27 * dVar28;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar17 + 1),SUB84(dVar28,0));
            }
            else {
              dVar28 = pdVar17[1];
            }
            if (in_FPSCR_PR == '\0') {
              dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),
                                        SUB84(dVar28,0) - SUB84(dVar27,0));
            }
            else {
              dVar28 = dVar28 - dVar27;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),
                                        *(undefined4 *)(DAT_8c61a770 + 1));
            }
            if (in_FPSCR_PR == '\0') {
              fVar32 = SUB84(dVar27,0) * (float)((ulonglong)dVar28 >> 0x20);
              fVar26 = SUB84(dVar28,0) - fVar32;
            }
            else {
              fVar32 = SUB84(dVar27 * dVar28,0);
              fVar26 = SUB84(dVar28 - dVar27 * dVar28,0);
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar22 = (double *)((int)pdVar22 + 0x34);
              *(float *)pdVar22 = fVar26;
              dVar28 = (double)CONCAT44(*(undefined4 *)((int)pdVar17 + 4),fVar32);
            }
            else {
              pdVar22 = pdVar22 + 6;
              *pdVar22 = in_xd2;
              dVar28 = *(double *)((int)pdVar17 + 4);
            }
            iVar10 = *piVar5;
            if (in_FPSCR_SZ == '\0') {
              dVar30 = (double)CONCAT44(*(undefined4 *)(iVar10 + 4),*(undefined4 *)(iVar10 + 0x14));
            }
            else {
              in_xd2 = *(double *)(iVar10 + 0x14);
              dVar30 = *(double *)(iVar10 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar30 = (double)CONCAT44((int)((ulonglong)dVar30 >> 0x20),
                                        SUB84(dVar30,0) * (float)((ulonglong)dVar28 >> 0x20));
            }
            else {
              dVar30 = dVar30 * dVar28;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)pdVar17,SUB84(dVar28,0));
            }
            else {
              dVar28 = *pdVar17;
            }
            if (in_FPSCR_PR == '\0') {
              dVar30 = (double)(ulonglong)
                               (uint)((float)((ulonglong)dVar28 >> 0x20) *
                                      (float)((ulonglong)dVar30 >> 0x20) + SUB84(dVar30,0));
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar17 + 1),SUB84(dVar28,0));
              uVar29 = CONCAT44(*(undefined4 *)(iVar10 + 0x24),SUB84(dVar30,0));
            }
            else {
              dVar28 = pdVar17[1];
              uVar29 = *(ulonglong *)(iVar10 + 0x24);
            }
            if (in_FPSCR_PR == '\0') {
              uVar29 = (ulonglong)
                       (uint)((float)((ulonglong)dVar28 >> 0x20) * (float)(uVar29 >> 0x20) +
                             (float)uVar29);
            }
            dVar27 = in_xd14;
            dVar30 = in_xd14;
            if (in_FPSCR_SZ == '\0') {
              dVar27 = (double)CONCAT44(uVar31,(int)uVar29);
              dVar30 = (double)CONCAT44(uVar31,SUB84(dVar28,0));
            }
            if (in_FPSCR_PR == '\0') {
              dVar27 = (double)CONCAT44((float)((ulonglong)dVar30 >> 0x20) * SUB84(dVar27,0) +
                                        (float)((ulonglong)dVar27 >> 0x20),SUB84(dVar27,0));
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar22 = (double *)((int)pdVar22 + -4);
              *(int *)pdVar22 = (int)((ulonglong)dVar27 >> 0x20);
              dVar28 = (double)CONCAT44(*(undefined4 *)((int)pdVar17 + 4),SUB84(dVar30,0));
            }
            else {
              pdVar22 = pdVar22 + -1;
              *pdVar22 = dVar27;
              dVar28 = *(double *)((int)pdVar17 + 4);
            }
            pdVar12 = (double *)*piVar5;
            if (in_FPSCR_SZ == '\0') {
              dVar30 = (double)CONCAT44(*(undefined4 *)pdVar12,*(undefined4 *)(pdVar12 + 2));
            }
            else {
              in_xd2 = pdVar12[2];
              dVar30 = *pdVar12;
            }
            if (in_FPSCR_PR == '\0') {
              dVar30 = (double)CONCAT44((int)((ulonglong)dVar30 >> 0x20),
                                        SUB84(dVar30,0) * (float)((ulonglong)dVar28 >> 0x20));
            }
            else {
              dVar30 = dVar30 * dVar28;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)pdVar17,SUB84(dVar28,0));
            }
            else {
              dVar28 = *pdVar17;
            }
            if (in_FPSCR_PR == '\0') {
              dVar30 = (double)(ulonglong)
                               (uint)((float)((ulonglong)dVar28 >> 0x20) *
                                      (float)((ulonglong)dVar30 >> 0x20) + SUB84(dVar30,0));
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar17 + 1),SUB84(dVar28,0));
              dVar30 = (double)CONCAT44(*(undefined4 *)(pdVar12 + 4),SUB84(dVar30,0));
            }
            else {
              dVar28 = pdVar17[1];
              dVar30 = pdVar12[4];
            }
            if (in_FPSCR_PR == '\0') {
              dVar30 = (double)(ulonglong)
                               (uint)((float)((ulonglong)dVar28 >> 0x20) *
                                      (float)((ulonglong)dVar30 >> 0x20) + SUB84(dVar30,0));
            }
            dVar33 = in_xd14;
            dVar27 = in_xd14;
            if (in_FPSCR_SZ == '\0') {
              dVar33 = (double)CONCAT44(uVar31,SUB84(dVar30,0));
              dVar27 = (double)CONCAT44(uVar31,SUB84(dVar28,0));
            }
            fVar32 = (float)((ulonglong)dVar27 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar33 = (double)CONCAT44(fVar32 * SUB84(dVar33,0) +
                                        (float)((ulonglong)dVar33 >> 0x20),SUB84(dVar33,0));
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)pdVar22 + -4) = (int)((ulonglong)dVar33 >> 0x20);
              *(undefined4 *)(pdVar22 + -1) = *(undefined4 *)(pdVar17 + 3);
              *(undefined4 *)((int)pdVar22 + -0xc) = *(undefined4 *)((int)pdVar17 + 0x14);
              uVar3 = DAT_8c61a780;
              pdVar22 = pdVar22 + -2;
              *(undefined4 *)pdVar22 = *(undefined4 *)(pdVar17 + 2);
            }
            else {
              pdVar22[-1] = dVar33;
              pdVar22[-2] = pdVar17[3];
              pdVar22[-3] = *(double *)((int)pdVar17 + 0x14);
              uVar3 = DAT_8c61a780;
              in_xd2 = pdVar17[2];
              pdVar22 = pdVar22 + -4;
              *pdVar22 = in_xd2;
            }
            *(undefined4 *)((int)pdVar22 + -4) = uVar3;
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)DAT_8c61a770,*(undefined4 *)pdVar19);
              pdVar17 = (double *)((int)DAT_8c61a770 + 4);
            }
            else {
              in_xd2 = *pdVar19;
              dVar28 = *DAT_8c61a770;
              pdVar17 = DAT_8c61a770 + 1;
            }
            if (in_FPSCR_PR == '\0') {
              dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) * SUB84(dVar28,0),
                                        SUB84(dVar28,0));
            }
            else {
              dVar28 = dVar28 * dVar28;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),*(undefined4 *)pdVar8);
              dVar27 = (double)CONCAT44(fVar32,*(undefined4 *)pdVar17);
              pdVar17 = (double *)((int)pdVar17 + 4);
            }
            else {
              in_xd2 = *pdVar8;
              pdVar17 = pdVar17 + 1;
            }
            if (in_FPSCR_PR == '\0') {
              fVar32 = SUB84(dVar28,0) - (float)((ulonglong)dVar28 >> 0x20);
            }
            else {
              fVar32 = SUB84(dVar28 - dVar28,0);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)((int)pdVar19 + 4),fVar32);
            }
            else {
              dVar28 = *(double *)((int)pdVar19 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),
                                        SUB84(dVar27,0) * (float)((ulonglong)dVar28 >> 0x20));
            }
            else {
              dVar27 = dVar27 * dVar28;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar19 + 1),SUB84(dVar28,0));
            }
            else {
              dVar28 = pdVar19[1];
            }
            if (in_FPSCR_PR == '\0') {
              dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),
                                        SUB84(dVar28,0) - SUB84(dVar27,0));
            }
            else {
              dVar28 = dVar28 - dVar27;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),*(undefined4 *)pdVar17);
            }
            if (in_FPSCR_PR == '\0') {
              fVar32 = SUB84(dVar27,0) * (float)((ulonglong)dVar28 >> 0x20);
              fVar26 = SUB84(dVar28,0) - fVar32;
            }
            else {
              fVar32 = SUB84(dVar27 * dVar28,0);
              fVar26 = SUB84(dVar28 - dVar27 * dVar28,0);
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar22 = (double *)((int)pdVar22 + 0x34);
              *(float *)pdVar22 = fVar26;
              dVar28 = (double)CONCAT44(*(undefined4 *)((int)pdVar19 + 4),fVar32);
            }
            else {
              pdVar22 = pdVar22 + 6;
              *pdVar22 = in_xd2;
              dVar28 = *(double *)((int)pdVar19 + 4);
            }
            iVar10 = *piVar5;
            if (in_FPSCR_SZ == '\0') {
              dVar30 = (double)CONCAT44(*(undefined4 *)(iVar10 + 4),*(undefined4 *)(iVar10 + 0x14));
            }
            else {
              in_xd2 = *(double *)(iVar10 + 0x14);
              dVar30 = *(double *)(iVar10 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar30 = (double)CONCAT44((int)((ulonglong)dVar30 >> 0x20),
                                        SUB84(dVar30,0) * (float)((ulonglong)dVar28 >> 0x20));
            }
            else {
              dVar30 = dVar30 * dVar28;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)pdVar19,SUB84(dVar28,0));
            }
            else {
              dVar28 = *pdVar19;
            }
            if (in_FPSCR_PR == '\0') {
              dVar30 = (double)(ulonglong)
                               (uint)((float)((ulonglong)dVar28 >> 0x20) *
                                      (float)((ulonglong)dVar30 >> 0x20) + SUB84(dVar30,0));
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar19 + 1),SUB84(dVar28,0));
              uVar29 = CONCAT44(*(undefined4 *)(iVar10 + 0x24),SUB84(dVar30,0));
            }
            else {
              dVar28 = pdVar19[1];
              uVar29 = *(ulonglong *)(iVar10 + 0x24);
            }
            if (in_FPSCR_PR == '\0') {
              uVar29 = (ulonglong)
                       (uint)((float)((ulonglong)dVar28 >> 0x20) * (float)(uVar29 >> 0x20) +
                             (float)uVar29);
            }
            dVar27 = in_xd14;
            dVar30 = in_xd14;
            if (in_FPSCR_SZ == '\0') {
              dVar27 = (double)CONCAT44(uVar31,(int)uVar29);
              dVar30 = (double)CONCAT44(uVar31,SUB84(dVar28,0));
            }
            if (in_FPSCR_PR == '\0') {
              dVar27 = (double)CONCAT44((float)((ulonglong)dVar30 >> 0x20) * SUB84(dVar27,0) +
                                        (float)((ulonglong)dVar27 >> 0x20),SUB84(dVar27,0));
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar22 = (double *)((int)pdVar22 + -4);
              *(int *)pdVar22 = (int)((ulonglong)dVar27 >> 0x20);
              dVar28 = (double)CONCAT44(*(undefined4 *)((int)pdVar19 + 4),SUB84(dVar30,0));
            }
            else {
              pdVar22 = pdVar22 + -1;
              *pdVar22 = dVar27;
              dVar28 = *(double *)((int)pdVar19 + 4);
            }
            pdVar8 = (double *)*piVar5;
            if (in_FPSCR_SZ == '\0') {
              dVar30 = (double)CONCAT44(*(undefined4 *)pdVar8,*(undefined4 *)(pdVar8 + 2));
            }
            else {
              in_xd2 = pdVar8[2];
              dVar30 = *pdVar8;
            }
            if (in_FPSCR_PR == '\0') {
              dVar30 = (double)CONCAT44((int)((ulonglong)dVar30 >> 0x20),
                                        SUB84(dVar30,0) * (float)((ulonglong)dVar28 >> 0x20));
            }
            else {
              dVar30 = dVar30 * dVar28;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)pdVar19,SUB84(dVar28,0));
            }
            else {
              dVar28 = *pdVar19;
            }
            if (in_FPSCR_PR == '\0') {
              dVar30 = (double)(ulonglong)
                               (uint)((float)((ulonglong)dVar28 >> 0x20) *
                                      (float)((ulonglong)dVar30 >> 0x20) + SUB84(dVar30,0));
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar19 + 1),SUB84(dVar28,0));
              dVar30 = (double)CONCAT44(*(undefined4 *)(pdVar8 + 4),SUB84(dVar30,0));
            }
            else {
              dVar28 = pdVar19[1];
              dVar30 = pdVar8[4];
            }
            if (in_FPSCR_PR == '\0') {
              dVar30 = (double)(ulonglong)
                               (uint)((float)((ulonglong)dVar28 >> 0x20) *
                                      (float)((ulonglong)dVar30 >> 0x20) + SUB84(dVar30,0));
            }
            dVar27 = in_xd14;
            in_dr0 = in_xd14;
            if (in_FPSCR_SZ == '\0') {
              dVar27 = (double)CONCAT44(uVar31,SUB84(dVar30,0));
              in_dr0 = (double)CONCAT44(uVar31,SUB84(dVar28,0));
            }
            if (in_FPSCR_PR == '\0') {
              dVar27 = (double)CONCAT44((float)((ulonglong)in_dr0 >> 0x20) * SUB84(dVar27,0) +
                                        (float)((ulonglong)dVar27 >> 0x20),SUB84(dVar27,0));
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)pdVar22 + -4) = (int)((ulonglong)dVar27 >> 0x20);
              *(undefined4 *)(pdVar22 + -1) = *(undefined4 *)(pdVar19 + 3);
              *(undefined4 *)((int)pdVar22 + -0xc) = *(undefined4 *)((int)pdVar19 + 0x14);
              pdVar22 = pdVar22 + -2;
              *(undefined4 *)pdVar22 = *(undefined4 *)(pdVar19 + 2);
            }
            else {
              pdVar22[-1] = dVar27;
              pdVar22[-2] = pdVar19[3];
              pdVar22[-3] = *(double *)((int)pdVar19 + 0x14);
              in_xd2 = pdVar19[2];
              pdVar22 = pdVar22 + -4;
              *pdVar22 = in_xd2;
            }
            *(undefined4 *)((int)pdVar22 + -4) = DAT_8c61a784;
            unaff_r8 = (int)pdVar22 + 0x1c;
            *DAT_8c61a788 = *DAT_8c61a788 + 3;
          }
        }
        else {
          *DAT_8c61a78c = 1;
          in_dr0 = (double)(*(code *)PTR_FUN_8c61a790)();
        }
      }
    }
  }
  else if (uVar6 == (int)DAT_8c61a4ec) {
    *DAT_8c61a794 = *DAT_8c61a794 + (uint)*(ushort *)(*(int *)((int)puVar25 + -0xc) + 2) * 4;
    for (iVar13 = 0; piVar5 = DAT_8c61aae8,
        iVar13 < (int)(uint)*(ushort *)(*(int *)((int)puVar25 + -0xc) + 2); iVar13 = iVar13 + 1) {
      uVar1 = *puVar14;
      puVar9 = puVar14 + 1;
      puVar16 = puVar14 + 2;
      iVar10 = *DAT_8c61aad8;
      puVar15 = puVar14 + 3;
      puVar14 = puVar14 + 4;
      pdVar8 = (double *)((uint)uVar1 * 0x20 + iVar10);
      pdVar19 = (double *)((uint)*puVar9 * 0x20 + iVar10);
      pdVar22 = (double *)((uint)*puVar16 * 0x20 + iVar10);
      pdVar17 = (double *)((uint)*puVar15 * 0x20 + iVar10);
      if ((*(uint *)((int)pdVar8 + 0xc) & *(uint *)((int)pdVar19 + 0xc) &
           *(uint *)((int)pdVar22 + 0xc) & *(uint *)((int)pdVar17 + 0xc) & 0x3f) == 0) {
        if (((*(uint *)((int)pdVar8 + 0xc) | *(uint *)((int)pdVar19 + 0xc) |
              *(uint *)((int)pdVar22 + 0xc) | *(uint *)((int)pdVar17 + 0xc)) & 1) == 0) {
          if (in_FPSCR_SZ == '\0') {
            uVar31 = *(undefined4 *)(pdVar19 + 2);
            dVar30 = (double)CONCAT44(uVar31,*(undefined4 *)(pdVar8 + 2));
            dVar28 = (double)CONCAT44(uVar31,SUB84(param_1,0));
          }
          else {
            dVar30 = pdVar19[2];
            in_xd2 = pdVar8[2];
            uVar31 = (undefined4)((ulonglong)dVar30 >> 0x20);
            dVar28 = dVar30;
          }
          if (in_FPSCR_PR == '\0') {
            dVar30 = (double)CONCAT44((float)((ulonglong)dVar30 >> 0x20) - SUB84(dVar30,0),
                                      SUB84(dVar30,0));
          }
          else {
            dVar30 = dVar30 - dVar30;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar30 = (double)CONCAT44((int)((ulonglong)dVar30 >> 0x20),
                                      *(undefined4 *)((int)pdVar22 + 0x14));
            dVar28 = (double)CONCAT44(uVar31,*(undefined4 *)((int)pdVar19 + 0x14));
          }
          else {
            in_xd2 = *(double *)((int)pdVar22 + 0x14);
          }
          fVar32 = SUB84(dVar28,0);
          if (in_FPSCR_PR == '\0') {
            dVar30 = (double)CONCAT44((int)((ulonglong)dVar30 >> 0x20),SUB84(dVar30,0) - fVar32);
          }
          else {
            dVar30 = dVar30 - dVar28;
          }
          if (in_FPSCR_SZ == '\0') {
            in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar32);
          }
          if (in_FPSCR_PR == '\0') {
            dVar30 = (double)CONCAT44((float)((ulonglong)dVar30 >> 0x20) * SUB84(dVar30,0),
                                      SUB84(dVar30,0));
          }
          else {
            dVar30 = dVar30 * dVar30;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar30 = (double)CONCAT44((int)((ulonglong)dVar30 >> 0x20),
                                      *(undefined4 *)((int)pdVar8 + 0x14));
          }
          else {
            in_xd2 = *(double *)((int)pdVar8 + 0x14);
          }
          if (in_FPSCR_PR == '\0') {
            dVar27 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                      SUB84(in_dr0,0) - SUB84(dVar30,0));
          }
          else {
            dVar27 = in_dr0 - dVar30;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar30 = (double)CONCAT44((int)((ulonglong)dVar30 >> 0x20),*(undefined4 *)(pdVar22 + 2))
            ;
          }
          else {
            in_xd2 = pdVar22[2];
          }
          if (in_FPSCR_PR == '\0') {
            dVar30 = (double)CONCAT44((int)((ulonglong)dVar30 >> 0x20),
                                      SUB84(dVar30,0) - (float)((ulonglong)dVar28 >> 0x20));
          }
          else {
            dVar30 = dVar30 - dVar28;
          }
          uVar6 = (uint)((ulonglong)dVar30 >> 0x20);
          param_1 = dVar30;
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44(uVar6,fVar32);
          }
          if (in_FPSCR_PR == '\0') {
            in_dr0 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),
                                      SUB84(dVar27,0) * SUB84(dVar30,0));
          }
          else {
            in_dr0 = dVar27 * dVar30;
          }
          if (in_FPSCR_PR == '\0') {
            fVar32 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
            param_1 = (double)CONCAT44(fVar32,SUB84(param_1,0));
            bVar34 = 0.0 < fVar32;
          }
          else {
            param_1 = param_1 - in_dr0;
            bVar34 = (double)((ulonglong)uVar6 << 0x20) < param_1;
          }
          if (!bVar34) {
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)DAT_8c61aadc,*(undefined4 *)pdVar8);
            }
            else {
              in_xd2 = *pdVar8;
              dVar28 = *DAT_8c61aadc;
            }
            if (in_FPSCR_PR == '\0') {
              dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) * SUB84(dVar28,0),
                                        SUB84(dVar28,0));
            }
            else {
              dVar28 = dVar28 * dVar28;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),*(undefined4 *)DAT_8c61aae0
                                       );
              in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),*DAT_8c61aae4);
            }
            else {
              in_xd2 = *DAT_8c61aae0;
            }
            if (in_FPSCR_PR == '\0') {
              fVar32 = SUB84(dVar28,0) - (float)((ulonglong)dVar28 >> 0x20);
            }
            else {
              fVar32 = SUB84(dVar28 - dVar28,0);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)((int)pdVar8 + 4),fVar32);
            }
            else {
              dVar28 = *(double *)((int)pdVar8 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar30 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                        SUB84(in_dr0,0) * (float)((ulonglong)dVar28 >> 0x20));
            }
            else {
              dVar30 = in_dr0 * dVar28;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar8 + 1),SUB84(dVar28,0));
            }
            else {
              dVar28 = pdVar8[1];
            }
            if (in_FPSCR_PR == '\0') {
              dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),
                                        SUB84(dVar28,0) - SUB84(dVar30,0));
            }
            else {
              dVar28 = dVar28 - dVar30;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar30 = (double)CONCAT44((int)((ulonglong)dVar30 >> 0x20),
                                        *(undefined4 *)(DAT_8c61aadc + 1));
            }
            fVar32 = (float)((ulonglong)dVar28 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              fVar26 = SUB84(dVar30,0) * fVar32;
              dVar28 = (double)CONCAT44(fVar32,SUB84(dVar28,0) - fVar26);
            }
            else {
              fVar26 = SUB84(dVar30 * dVar28,0);
              dVar28 = dVar28 - dVar30 * dVar28;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar12 = (double *)(unaff_r8 + 0x18);
              *(int *)pdVar12 = SUB84(dVar28,0);
            }
            else {
              pdVar12 = (double *)(unaff_r8 + 0x14);
              *pdVar12 = in_xd2;
            }
            piVar4 = DAT_8c61aae8;
            iVar10 = *piVar5;
            if (in_FPSCR_SZ == '\0') {
              dVar30 = (double)CONCAT44(*(undefined4 *)((int)pdVar8 + 4),fVar26);
              dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),
                                        *(undefined4 *)(iVar10 + 0x14));
            }
            else {
              dVar30 = *(double *)((int)pdVar8 + 4);
              in_xd2 = *(double *)(iVar10 + 0x14);
            }
            if (in_FPSCR_PR == '\0') {
              fVar32 = SUB84(dVar28,0) * (float)((ulonglong)dVar30 >> 0x20);
            }
            else {
              fVar32 = SUB84(dVar28 * dVar30,0);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)pdVar8,SUB84(dVar30,0));
              uVar29 = CONCAT44(*(undefined4 *)(iVar10 + 4),fVar32);
            }
            else {
              dVar28 = *pdVar8;
              uVar29 = *(ulonglong *)(iVar10 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              uVar29 = (ulonglong)
                       (uint)((float)((ulonglong)dVar28 >> 0x20) * (float)(uVar29 >> 0x20) +
                             (float)uVar29);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar8 + 1),SUB84(dVar28,0));
              uVar29 = CONCAT44(*(undefined4 *)(iVar10 + 0x24),(int)uVar29);
            }
            else {
              dVar28 = pdVar8[1];
              uVar29 = *(ulonglong *)(iVar10 + 0x24);
            }
            if (in_FPSCR_PR == '\0') {
              uVar29 = (ulonglong)
                       (uint)((float)((ulonglong)dVar28 >> 0x20) * (float)(uVar29 >> 0x20) +
                             (float)uVar29);
            }
            uVar31 = (undefined4)in_dr14;
            dVar27 = in_xd14;
            dVar30 = in_xd14;
            if (in_FPSCR_SZ == '\0') {
              dVar27 = (double)CONCAT44(uVar31,(int)uVar29);
              dVar30 = (double)CONCAT44(uVar31,SUB84(dVar28,0));
            }
            if (in_FPSCR_PR == '\0') {
              dVar27 = (double)CONCAT44((float)((ulonglong)dVar30 >> 0x20) * SUB84(dVar27,0) +
                                        (float)((ulonglong)dVar27 >> 0x20),SUB84(dVar27,0));
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar12 = (double *)((int)pdVar12 + -4);
              *(int *)pdVar12 = (int)((ulonglong)dVar27 >> 0x20);
            }
            else {
              pdVar12 = pdVar12 + -1;
              *pdVar12 = dVar27;
            }
            pdVar7 = (double *)*piVar4;
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)((int)pdVar8 + 4),SUB84(dVar30,0));
              dVar30 = (double)CONCAT44(*(undefined4 *)pdVar7,*(undefined4 *)(pdVar7 + 2));
            }
            else {
              dVar28 = *(double *)((int)pdVar8 + 4);
              in_xd2 = pdVar7[2];
              dVar30 = *pdVar7;
            }
            if (in_FPSCR_PR == '\0') {
              dVar30 = (double)CONCAT44((int)((ulonglong)dVar30 >> 0x20),
                                        SUB84(dVar30,0) * (float)((ulonglong)dVar28 >> 0x20));
            }
            else {
              dVar30 = dVar30 * dVar28;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)pdVar8,SUB84(dVar28,0));
            }
            else {
              dVar28 = *pdVar8;
            }
            if (in_FPSCR_PR == '\0') {
              dVar30 = (double)(ulonglong)
                               (uint)((float)((ulonglong)dVar28 >> 0x20) *
                                      (float)((ulonglong)dVar30 >> 0x20) + SUB84(dVar30,0));
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar8 + 1),SUB84(dVar28,0));
              dVar30 = (double)CONCAT44(*(undefined4 *)(pdVar7 + 4),SUB84(dVar30,0));
            }
            else {
              dVar28 = pdVar8[1];
              dVar30 = pdVar7[4];
            }
            if (in_FPSCR_PR == '\0') {
              dVar30 = (double)(ulonglong)
                               (uint)((float)((ulonglong)dVar28 >> 0x20) *
                                      (float)((ulonglong)dVar30 >> 0x20) + SUB84(dVar30,0));
            }
            dVar33 = in_xd14;
            dVar27 = in_xd14;
            if (in_FPSCR_SZ == '\0') {
              dVar33 = (double)CONCAT44(uVar31,SUB84(dVar30,0));
              dVar27 = (double)CONCAT44(uVar31,SUB84(dVar28,0));
            }
            fVar32 = (float)((ulonglong)dVar27 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar33 = (double)CONCAT44(fVar32 * SUB84(dVar33,0) +
                                        (float)((ulonglong)dVar33 >> 0x20),SUB84(dVar33,0));
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)pdVar12 + -4) = (int)((ulonglong)dVar33 >> 0x20);
              uVar3 = DAT_8c61aaec;
              *(undefined4 *)(pdVar12 + -1) = *(undefined4 *)(pdVar8 + 3);
              *(undefined4 *)((int)pdVar12 + -0xc) = *(undefined4 *)((int)pdVar8 + 0x14);
              pdVar12 = pdVar12 + -2;
              *(undefined4 *)pdVar12 = *(undefined4 *)(pdVar8 + 2);
            }
            else {
              pdVar12[-1] = dVar33;
              uVar3 = DAT_8c61aaec;
              pdVar12[-2] = pdVar8[3];
              pdVar12[-3] = *(double *)((int)pdVar8 + 0x14);
              in_xd2 = pdVar8[2];
              pdVar12 = pdVar12 + -4;
              *pdVar12 = in_xd2;
            }
            *(undefined4 *)((int)pdVar12 + -4) = uVar3;
            piVar5 = DAT_8c61aae8;
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)DAT_8c61aadc,*(undefined4 *)pdVar19);
            }
            else {
              in_xd2 = *pdVar19;
              dVar28 = *DAT_8c61aadc;
            }
            if (in_FPSCR_PR == '\0') {
              dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) * SUB84(dVar28,0),
                                        SUB84(dVar28,0));
            }
            else {
              dVar28 = dVar28 * dVar28;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),*(undefined4 *)DAT_8c61aae0
                                       );
              dVar27 = (double)CONCAT44(fVar32,*DAT_8c61aae4);
            }
            else {
              in_xd2 = *DAT_8c61aae0;
            }
            if (in_FPSCR_PR == '\0') {
              fVar32 = SUB84(dVar28,0) - (float)((ulonglong)dVar28 >> 0x20);
            }
            else {
              fVar32 = SUB84(dVar28 - dVar28,0);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)((int)pdVar19 + 4),fVar32);
            }
            else {
              dVar28 = *(double *)((int)pdVar19 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),
                                        SUB84(dVar27,0) * (float)((ulonglong)dVar28 >> 0x20));
            }
            else {
              dVar27 = dVar27 * dVar28;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar19 + 1),SUB84(dVar28,0));
            }
            else {
              dVar28 = pdVar19[1];
            }
            if (in_FPSCR_PR == '\0') {
              dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),
                                        SUB84(dVar28,0) - SUB84(dVar27,0));
            }
            else {
              dVar28 = dVar28 - dVar27;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),
                                        *(undefined4 *)(DAT_8c61aadc + 1));
            }
            fVar32 = (float)((ulonglong)dVar28 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              fVar26 = SUB84(dVar27,0) * fVar32;
              dVar28 = (double)CONCAT44(fVar32,SUB84(dVar28,0) - fVar26);
            }
            else {
              fVar26 = SUB84(dVar27 * dVar28,0);
              dVar28 = dVar28 - dVar27 * dVar28;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar12 = (double *)((int)pdVar12 + 0x34);
              *(int *)pdVar12 = SUB84(dVar28,0);
            }
            else {
              pdVar12 = pdVar12 + 6;
              *pdVar12 = in_xd2;
            }
            piVar4 = DAT_8c61aae8;
            iVar10 = *piVar5;
            if (in_FPSCR_SZ == '\0') {
              dVar30 = (double)CONCAT44(*(undefined4 *)((int)pdVar19 + 4),fVar26);
              dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),
                                        *(undefined4 *)(iVar10 + 0x14));
            }
            else {
              dVar30 = *(double *)((int)pdVar19 + 4);
              in_xd2 = *(double *)(iVar10 + 0x14);
            }
            if (in_FPSCR_PR == '\0') {
              fVar32 = SUB84(dVar28,0) * (float)((ulonglong)dVar30 >> 0x20);
            }
            else {
              fVar32 = SUB84(dVar28 * dVar30,0);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)pdVar19,SUB84(dVar30,0));
              uVar29 = CONCAT44(*(undefined4 *)(iVar10 + 4),fVar32);
            }
            else {
              dVar28 = *pdVar19;
              uVar29 = *(ulonglong *)(iVar10 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              uVar29 = (ulonglong)
                       (uint)((float)((ulonglong)dVar28 >> 0x20) * (float)(uVar29 >> 0x20) +
                             (float)uVar29);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar19 + 1),SUB84(dVar28,0));
              uVar29 = CONCAT44(*(undefined4 *)(iVar10 + 0x24),(int)uVar29);
            }
            else {
              dVar28 = pdVar19[1];
              uVar29 = *(ulonglong *)(iVar10 + 0x24);
            }
            if (in_FPSCR_PR == '\0') {
              uVar29 = (ulonglong)
                       (uint)((float)((ulonglong)dVar28 >> 0x20) * (float)(uVar29 >> 0x20) +
                             (float)uVar29);
            }
            dVar27 = in_xd14;
            dVar30 = in_xd14;
            if (in_FPSCR_SZ == '\0') {
              dVar27 = (double)CONCAT44(uVar31,(int)uVar29);
              dVar30 = (double)CONCAT44(uVar31,SUB84(dVar28,0));
            }
            if (in_FPSCR_PR == '\0') {
              dVar27 = (double)CONCAT44((float)((ulonglong)dVar30 >> 0x20) * SUB84(dVar27,0) +
                                        (float)((ulonglong)dVar27 >> 0x20),SUB84(dVar27,0));
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar12 = (double *)((int)pdVar12 + -4);
              *(int *)pdVar12 = (int)((ulonglong)dVar27 >> 0x20);
            }
            else {
              pdVar12 = pdVar12 + -1;
              *pdVar12 = dVar27;
            }
            pdVar8 = (double *)*piVar4;
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)((int)pdVar19 + 4),SUB84(dVar30,0));
              dVar30 = (double)CONCAT44(*(undefined4 *)pdVar8,*(undefined4 *)(pdVar8 + 2));
            }
            else {
              dVar28 = *(double *)((int)pdVar19 + 4);
              in_xd2 = pdVar8[2];
              dVar30 = *pdVar8;
            }
            if (in_FPSCR_PR == '\0') {
              dVar30 = (double)CONCAT44((int)((ulonglong)dVar30 >> 0x20),
                                        SUB84(dVar30,0) * (float)((ulonglong)dVar28 >> 0x20));
            }
            else {
              dVar30 = dVar30 * dVar28;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)pdVar19,SUB84(dVar28,0));
            }
            else {
              dVar28 = *pdVar19;
            }
            if (in_FPSCR_PR == '\0') {
              dVar30 = (double)(ulonglong)
                               (uint)((float)((ulonglong)dVar28 >> 0x20) *
                                      (float)((ulonglong)dVar30 >> 0x20) + SUB84(dVar30,0));
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar19 + 1),SUB84(dVar28,0));
              dVar30 = (double)CONCAT44(*(undefined4 *)(pdVar8 + 4),SUB84(dVar30,0));
            }
            else {
              dVar28 = pdVar19[1];
              dVar30 = pdVar8[4];
            }
            if (in_FPSCR_PR == '\0') {
              dVar30 = (double)(ulonglong)
                               (uint)((float)((ulonglong)dVar28 >> 0x20) *
                                      (float)((ulonglong)dVar30 >> 0x20) + SUB84(dVar30,0));
            }
            dVar33 = in_xd14;
            dVar27 = in_xd14;
            if (in_FPSCR_SZ == '\0') {
              dVar33 = (double)CONCAT44(uVar31,SUB84(dVar30,0));
              dVar27 = (double)CONCAT44(uVar31,SUB84(dVar28,0));
            }
            fVar32 = (float)((ulonglong)dVar27 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar33 = (double)CONCAT44(fVar32 * SUB84(dVar33,0) +
                                        (float)((ulonglong)dVar33 >> 0x20),SUB84(dVar33,0));
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)pdVar12 + -4) = (int)((ulonglong)dVar33 >> 0x20);
              uVar3 = DAT_8c61aaec;
              *(undefined4 *)(pdVar12 + -1) = *(undefined4 *)(pdVar19 + 3);
              *(undefined4 *)((int)pdVar12 + -0xc) = *(undefined4 *)((int)pdVar19 + 0x14);
              pdVar12 = pdVar12 + -2;
              *(undefined4 *)pdVar12 = *(undefined4 *)(pdVar19 + 2);
            }
            else {
              pdVar12[-1] = dVar33;
              uVar3 = DAT_8c61aaec;
              pdVar12[-2] = pdVar19[3];
              pdVar12[-3] = *(double *)((int)pdVar19 + 0x14);
              in_xd2 = pdVar19[2];
              pdVar12 = pdVar12 + -4;
              *pdVar12 = in_xd2;
            }
            *(undefined4 *)((int)pdVar12 + -4) = uVar3;
            piVar5 = DAT_8c61aae8;
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)DAT_8c61aadc,*(undefined4 *)pdVar22);
            }
            else {
              in_xd2 = *pdVar22;
              dVar28 = *DAT_8c61aadc;
            }
            if (in_FPSCR_PR == '\0') {
              dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) * SUB84(dVar28,0),
                                        SUB84(dVar28,0));
            }
            else {
              dVar28 = dVar28 * dVar28;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),*(undefined4 *)DAT_8c61aae0
                                       );
              dVar27 = (double)CONCAT44(fVar32,*DAT_8c61aae4);
            }
            else {
              in_xd2 = *DAT_8c61aae0;
            }
            if (in_FPSCR_PR == '\0') {
              fVar32 = SUB84(dVar28,0) - (float)((ulonglong)dVar28 >> 0x20);
            }
            else {
              fVar32 = SUB84(dVar28 - dVar28,0);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)((int)pdVar22 + 4),fVar32);
            }
            else {
              dVar28 = *(double *)((int)pdVar22 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),
                                        SUB84(dVar27,0) * (float)((ulonglong)dVar28 >> 0x20));
            }
            else {
              dVar27 = dVar27 * dVar28;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar22 + 1),SUB84(dVar28,0));
            }
            else {
              dVar28 = pdVar22[1];
            }
            if (in_FPSCR_PR == '\0') {
              dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),
                                        SUB84(dVar28,0) - SUB84(dVar27,0));
            }
            else {
              dVar28 = dVar28 - dVar27;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),
                                        *(undefined4 *)(DAT_8c61aadc + 1));
            }
            if (in_FPSCR_PR == '\0') {
              fVar32 = SUB84(dVar27,0) * (float)((ulonglong)dVar28 >> 0x20);
              fVar26 = SUB84(dVar28,0) - fVar32;
            }
            else {
              fVar32 = SUB84(dVar27 * dVar28,0);
              fVar26 = SUB84(dVar28 - dVar27 * dVar28,0);
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar12 = (double *)((int)pdVar12 + 0x34);
              *(float *)pdVar12 = fVar26;
              dVar28 = (double)CONCAT44(*(undefined4 *)((int)pdVar22 + 4),fVar32);
            }
            else {
              pdVar12 = pdVar12 + 6;
              *pdVar12 = in_xd2;
              dVar28 = *(double *)((int)pdVar22 + 4);
            }
            iVar10 = *piVar5;
            if (in_FPSCR_SZ == '\0') {
              dVar30 = (double)CONCAT44(*(undefined4 *)(iVar10 + 4),*(undefined4 *)(iVar10 + 0x14));
            }
            else {
              in_xd2 = *(double *)(iVar10 + 0x14);
              dVar30 = *(double *)(iVar10 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar30 = (double)CONCAT44((int)((ulonglong)dVar30 >> 0x20),
                                        SUB84(dVar30,0) * (float)((ulonglong)dVar28 >> 0x20));
            }
            else {
              dVar30 = dVar30 * dVar28;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)pdVar22,SUB84(dVar28,0));
            }
            else {
              dVar28 = *pdVar22;
            }
            if (in_FPSCR_PR == '\0') {
              dVar30 = (double)(ulonglong)
                               (uint)((float)((ulonglong)dVar28 >> 0x20) *
                                      (float)((ulonglong)dVar30 >> 0x20) + SUB84(dVar30,0));
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar22 + 1),SUB84(dVar28,0));
              uVar29 = CONCAT44(*(undefined4 *)(iVar10 + 0x24),SUB84(dVar30,0));
            }
            else {
              dVar28 = pdVar22[1];
              uVar29 = *(ulonglong *)(iVar10 + 0x24);
            }
            if (in_FPSCR_PR == '\0') {
              uVar29 = (ulonglong)
                       (uint)((float)((ulonglong)dVar28 >> 0x20) * (float)(uVar29 >> 0x20) +
                             (float)uVar29);
            }
            dVar27 = in_xd14;
            dVar30 = in_xd14;
            if (in_FPSCR_SZ == '\0') {
              dVar27 = (double)CONCAT44(uVar31,(int)uVar29);
              dVar30 = (double)CONCAT44(uVar31,SUB84(dVar28,0));
            }
            if (in_FPSCR_PR == '\0') {
              dVar27 = (double)CONCAT44((float)((ulonglong)dVar30 >> 0x20) * SUB84(dVar27,0) +
                                        (float)((ulonglong)dVar27 >> 0x20),SUB84(dVar27,0));
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar12 = (double *)((int)pdVar12 + -4);
              *(int *)pdVar12 = (int)((ulonglong)dVar27 >> 0x20);
              dVar28 = (double)CONCAT44(*(undefined4 *)((int)pdVar22 + 4),SUB84(dVar30,0));
            }
            else {
              pdVar12 = pdVar12 + -1;
              *pdVar12 = dVar27;
              dVar28 = *(double *)((int)pdVar22 + 4);
            }
            pdVar8 = (double *)*piVar5;
            if (in_FPSCR_SZ == '\0') {
              dVar30 = (double)CONCAT44(*(undefined4 *)pdVar8,*(undefined4 *)(pdVar8 + 2));
            }
            else {
              in_xd2 = pdVar8[2];
              dVar30 = *pdVar8;
            }
            if (in_FPSCR_PR == '\0') {
              dVar30 = (double)CONCAT44((int)((ulonglong)dVar30 >> 0x20),
                                        SUB84(dVar30,0) * (float)((ulonglong)dVar28 >> 0x20));
            }
            else {
              dVar30 = dVar30 * dVar28;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)pdVar22,SUB84(dVar28,0));
            }
            else {
              dVar28 = *pdVar22;
            }
            if (in_FPSCR_PR == '\0') {
              dVar30 = (double)(ulonglong)
                               (uint)((float)((ulonglong)dVar28 >> 0x20) *
                                      (float)((ulonglong)dVar30 >> 0x20) + SUB84(dVar30,0));
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar22 + 1),SUB84(dVar28,0));
              dVar30 = (double)CONCAT44(*(undefined4 *)(pdVar8 + 4),SUB84(dVar30,0));
            }
            else {
              dVar28 = pdVar22[1];
              dVar30 = pdVar8[4];
            }
            if (in_FPSCR_PR == '\0') {
              dVar30 = (double)(ulonglong)
                               (uint)((float)((ulonglong)dVar28 >> 0x20) *
                                      (float)((ulonglong)dVar30 >> 0x20) + SUB84(dVar30,0));
            }
            dVar33 = in_xd14;
            dVar27 = in_xd14;
            if (in_FPSCR_SZ == '\0') {
              dVar33 = (double)CONCAT44(uVar31,SUB84(dVar30,0));
              dVar27 = (double)CONCAT44(uVar31,SUB84(dVar28,0));
            }
            fVar32 = (float)((ulonglong)dVar27 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar33 = (double)CONCAT44(fVar32 * SUB84(dVar33,0) +
                                        (float)((ulonglong)dVar33 >> 0x20),SUB84(dVar33,0));
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)pdVar12 + -4) = (int)((ulonglong)dVar33 >> 0x20);
              *(undefined4 *)(pdVar12 + -1) = *(undefined4 *)(pdVar22 + 3);
              *(undefined4 *)((int)pdVar12 + -0xc) = *(undefined4 *)((int)pdVar22 + 0x14);
              uVar3 = DAT_8c61aaec;
              pdVar12 = pdVar12 + -2;
              *(undefined4 *)pdVar12 = *(undefined4 *)(pdVar22 + 2);
            }
            else {
              pdVar12[-1] = dVar33;
              pdVar12[-2] = pdVar22[3];
              pdVar12[-3] = *(double *)((int)pdVar22 + 0x14);
              uVar3 = DAT_8c61aaec;
              in_xd2 = pdVar22[2];
              pdVar12 = pdVar12 + -4;
              *pdVar12 = in_xd2;
            }
            *(undefined4 *)((int)pdVar12 + -4) = uVar3;
            piVar5 = DAT_8c61aae8;
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)DAT_8c61aadc,*(undefined4 *)pdVar17);
            }
            else {
              in_xd2 = *pdVar17;
              dVar28 = *DAT_8c61aadc;
            }
            if (in_FPSCR_PR == '\0') {
              dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) * SUB84(dVar28,0),
                                        SUB84(dVar28,0));
            }
            else {
              dVar28 = dVar28 * dVar28;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),*(undefined4 *)DAT_8c61aae0
                                       );
              dVar27 = (double)CONCAT44(fVar32,*DAT_8c61aae4);
            }
            else {
              in_xd2 = *DAT_8c61aae0;
            }
            if (in_FPSCR_PR == '\0') {
              fVar32 = SUB84(dVar28,0) - (float)((ulonglong)dVar28 >> 0x20);
            }
            else {
              fVar32 = SUB84(dVar28 - dVar28,0);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)((int)pdVar17 + 4),fVar32);
            }
            else {
              dVar28 = *(double *)((int)pdVar17 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),
                                        SUB84(dVar27,0) * (float)((ulonglong)dVar28 >> 0x20));
            }
            else {
              dVar27 = dVar27 * dVar28;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar17 + 1),SUB84(dVar28,0));
            }
            else {
              dVar28 = pdVar17[1];
            }
            if (in_FPSCR_PR == '\0') {
              dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),
                                        SUB84(dVar28,0) - SUB84(dVar27,0));
            }
            else {
              dVar28 = dVar28 - dVar27;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),
                                        *(undefined4 *)(DAT_8c61aadc + 1));
            }
            if (in_FPSCR_PR == '\0') {
              fVar32 = SUB84(dVar27,0) * (float)((ulonglong)dVar28 >> 0x20);
              fVar26 = SUB84(dVar28,0) - fVar32;
            }
            else {
              fVar32 = SUB84(dVar27 * dVar28,0);
              fVar26 = SUB84(dVar28 - dVar27 * dVar28,0);
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar12 = (double *)((int)pdVar12 + 0x34);
              *(float *)pdVar12 = fVar26;
              dVar28 = (double)CONCAT44(*(undefined4 *)((int)pdVar17 + 4),fVar32);
            }
            else {
              pdVar12 = pdVar12 + 6;
              *pdVar12 = in_xd2;
              dVar28 = *(double *)((int)pdVar17 + 4);
            }
            iVar10 = *piVar5;
            if (in_FPSCR_SZ == '\0') {
              dVar30 = (double)CONCAT44(*(undefined4 *)(iVar10 + 4),*(undefined4 *)(iVar10 + 0x14));
            }
            else {
              in_xd2 = *(double *)(iVar10 + 0x14);
              dVar30 = *(double *)(iVar10 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar30 = (double)CONCAT44((int)((ulonglong)dVar30 >> 0x20),
                                        SUB84(dVar30,0) * (float)((ulonglong)dVar28 >> 0x20));
            }
            else {
              dVar30 = dVar30 * dVar28;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)pdVar17,SUB84(dVar28,0));
            }
            else {
              dVar28 = *pdVar17;
            }
            if (in_FPSCR_PR == '\0') {
              dVar30 = (double)(ulonglong)
                               (uint)((float)((ulonglong)dVar28 >> 0x20) *
                                      (float)((ulonglong)dVar30 >> 0x20) + SUB84(dVar30,0));
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar17 + 1),SUB84(dVar28,0));
              uVar29 = CONCAT44(*(undefined4 *)(iVar10 + 0x24),SUB84(dVar30,0));
            }
            else {
              dVar28 = pdVar17[1];
              uVar29 = *(ulonglong *)(iVar10 + 0x24);
            }
            if (in_FPSCR_PR == '\0') {
              uVar29 = (ulonglong)
                       (uint)((float)((ulonglong)dVar28 >> 0x20) * (float)(uVar29 >> 0x20) +
                             (float)uVar29);
            }
            dVar27 = in_xd14;
            dVar30 = in_xd14;
            if (in_FPSCR_SZ == '\0') {
              dVar27 = (double)CONCAT44(uVar31,(int)uVar29);
              dVar30 = (double)CONCAT44(uVar31,SUB84(dVar28,0));
            }
            if (in_FPSCR_PR == '\0') {
              dVar27 = (double)CONCAT44((float)((ulonglong)dVar30 >> 0x20) * SUB84(dVar27,0) +
                                        (float)((ulonglong)dVar27 >> 0x20),SUB84(dVar27,0));
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar12 = (double *)((int)pdVar12 + -4);
              *(int *)pdVar12 = (int)((ulonglong)dVar27 >> 0x20);
              dVar28 = (double)CONCAT44(*(undefined4 *)((int)pdVar17 + 4),SUB84(dVar30,0));
            }
            else {
              pdVar12 = pdVar12 + -1;
              *pdVar12 = dVar27;
              dVar28 = *(double *)((int)pdVar17 + 4);
            }
            pdVar8 = (double *)*piVar5;
            if (in_FPSCR_SZ == '\0') {
              dVar30 = (double)CONCAT44(*(undefined4 *)pdVar8,*(undefined4 *)(pdVar8 + 2));
            }
            else {
              in_xd2 = pdVar8[2];
              dVar30 = *pdVar8;
            }
            if (in_FPSCR_PR == '\0') {
              dVar30 = (double)CONCAT44((int)((ulonglong)dVar30 >> 0x20),
                                        SUB84(dVar30,0) * (float)((ulonglong)dVar28 >> 0x20));
            }
            else {
              dVar30 = dVar30 * dVar28;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)pdVar17,SUB84(dVar28,0));
            }
            else {
              dVar28 = *pdVar17;
            }
            if (in_FPSCR_PR == '\0') {
              dVar30 = (double)(ulonglong)
                               (uint)((float)((ulonglong)dVar28 >> 0x20) *
                                      (float)((ulonglong)dVar30 >> 0x20) + SUB84(dVar30,0));
            }
            if (in_FPSCR_SZ == '\0') {
              dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar17 + 1),SUB84(dVar28,0));
              dVar30 = (double)CONCAT44(*(undefined4 *)(pdVar8 + 4),SUB84(dVar30,0));
            }
            else {
              dVar28 = pdVar17[1];
              dVar30 = pdVar8[4];
            }
            if (in_FPSCR_PR == '\0') {
              dVar30 = (double)(ulonglong)
                               (uint)((float)((ulonglong)dVar28 >> 0x20) *
                                      (float)((ulonglong)dVar30 >> 0x20) + SUB84(dVar30,0));
            }
            dVar27 = in_xd14;
            in_dr0 = in_xd14;
            if (in_FPSCR_SZ == '\0') {
              dVar27 = (double)CONCAT44(uVar31,SUB84(dVar30,0));
              in_dr0 = (double)CONCAT44(uVar31,SUB84(dVar28,0));
            }
            if (in_FPSCR_PR == '\0') {
              dVar27 = (double)CONCAT44((float)((ulonglong)in_dr0 >> 0x20) * SUB84(dVar27,0) +
                                        (float)((ulonglong)dVar27 >> 0x20),SUB84(dVar27,0));
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)pdVar12 + -4) = (int)((ulonglong)dVar27 >> 0x20);
              *(undefined4 *)(pdVar12 + -1) = *(undefined4 *)(pdVar17 + 3);
              *(undefined4 *)((int)pdVar12 + -0xc) = *(undefined4 *)((int)pdVar17 + 0x14);
              uVar31 = DAT_8c61aaf0;
              pdVar12 = pdVar12 + -2;
              *(undefined4 *)pdVar12 = *(undefined4 *)(pdVar17 + 2);
            }
            else {
              pdVar12[-1] = dVar27;
              pdVar12[-2] = pdVar17[3];
              pdVar12[-3] = *(double *)((int)pdVar17 + 0x14);
              uVar31 = DAT_8c61aaf0;
              in_xd2 = pdVar17[2];
              pdVar12 = pdVar12 + -4;
              *pdVar12 = in_xd2;
            }
            *(undefined4 *)((int)pdVar12 + -4) = uVar31;
            unaff_r8 = (int)pdVar12 + 0x1c;
            *DAT_8c61aaf4 = *DAT_8c61aaf4 + 4;
          }
        }
        else {
          *DAT_8c61aaf8 = 1;
          if ((*(uint *)((int)pdVar8 + 0xc) & *(uint *)((int)pdVar19 + 0xc) &
               *(uint *)((int)pdVar22 + 0xc) & 1) == 0) {
            in_dr0 = (double)(*(code *)PTR_FUN_8c61aafc)(pdVar19,pdVar22);
          }
          if ((*(uint *)((int)pdVar19 + 0xc) & *(uint *)((int)pdVar22 + 0xc) &
               *(uint *)((int)pdVar17 + 0xc) & 1) == 0) {
            in_dr0 = (double)(*(code *)PTR_FUN_8c61aafc)(pdVar19,pdVar22,pdVar17);
          }
        }
      }
    }
  }
  else if (uVar6 == DAT_8c61a4f8) {
    for (iVar13 = 0; uVar31 = SUB84(param_1,0),
        iVar13 < (int)(uint)*(ushort *)(*(int *)((int)puVar25 + -0xc) + 2); iVar13 = iVar13 + 1) {
      uVar6 = 0;
      puVar16 = puVar14 + 1;
      iVar10 = 0;
      uVar20 = (uint)*puVar14;
      *DAT_8c61ae08 = *DAT_8c61ae08 + uVar20;
      piVar5 = DAT_8c61ae0c;
      uVar11 = 0xffffffff;
      puVar9 = puVar16;
      if (uVar20 != 0) {
        do {
          uVar1 = *puVar9;
          puVar9 = puVar9 + 1;
          iVar10 = iVar10 + 1;
          iVar23 = (uint)uVar1 * 0x20 + *DAT_8c61ae0c;
          *(int *)((int)puVar25 + -8) = iVar23;
          uVar24 = *(uint *)(iVar23 + 0xc);
          uVar11 = uVar11 & uVar24;
          uVar6 = uVar6 | uVar24;
        } while (iVar10 < (int)uVar20);
      }
      if ((uVar11 & 0x3f) == 0) {
        if ((uVar6 & 1) == 0) {
          iVar23 = *DAT_8c61ae0c;
          iVar10 = (uint)puVar14[2] * 0x20 + iVar23;
          if (in_FPSCR_SZ == '\0') {
            uVar31 = *(undefined4 *)(iVar10 + 0x10);
          }
          else {
            in_xd4 = *(double *)(iVar10 + 0x10);
          }
          iVar18 = (uint)*puVar16 * 0x20 + iVar23;
          if (in_FPSCR_SZ == '\0') {
            dVar28 = (double)CONCAT44(uVar31,*(undefined4 *)(iVar18 + 0x10));
          }
          else {
            in_xd2 = *(double *)(iVar18 + 0x10);
            dVar28 = in_xd4;
          }
          if (in_FPSCR_PR == '\0') {
            dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) - SUB84(dVar28,0),
                                      SUB84(dVar28,0));
          }
          else {
            dVar28 = dVar28 - dVar28;
          }
          iVar23 = (uint)puVar14[3] * 0x20 + iVar23;
          if (in_FPSCR_SZ == '\0') {
            fVar32 = *(float *)(iVar10 + 0x14);
            dVar30 = (double)CONCAT44(fVar32,uVar31);
            dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),
                                      *(undefined4 *)(iVar23 + 0x14));
            in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar32);
          }
          else {
            dVar30 = *(double *)(iVar10 + 0x14);
            in_xd2 = *(double *)(iVar23 + 0x14);
            fVar32 = (float)((ulonglong)dVar30 >> 0x20);
          }
          if (in_FPSCR_PR == '\0') {
            fVar32 = SUB84(dVar28,0) - fVar32;
            param_1 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) * fVar32,fVar32);
          }
          else {
            param_1 = (dVar28 - dVar30) * (dVar28 - dVar30);
          }
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),
                                       *(undefined4 *)(iVar18 + 0x14));
          }
          else {
            in_xd2 = *(double *)(iVar18 + 0x14);
          }
          if (in_FPSCR_PR == '\0') {
            dVar28 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                      SUB84(in_dr0,0) - SUB84(param_1,0));
          }
          else {
            dVar28 = in_dr0 - param_1;
          }
          uVar31 = (undefined4)((ulonglong)param_1 >> 0x20);
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44(uVar31,SUB84(dVar30,0));
            dVar30 = (double)CONCAT44(uVar31,*(undefined4 *)(iVar23 + 0x10));
          }
          else {
            in_xd2 = *(double *)(iVar23 + 0x10);
            dVar30 = param_1;
          }
          if (in_FPSCR_PR == '\0') {
            fVar32 = SUB84(dVar30,0) - SUB84(param_1,0);
            dVar30 = (double)CONCAT44(uVar31,fVar32);
            in_dr0 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),SUB84(dVar28,0) * fVar32);
          }
          else {
            dVar30 = dVar30 - param_1;
            in_dr0 = dVar28 * dVar30;
          }
          if (in_FPSCR_PR == '\0') {
            fVar32 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
            param_1 = (double)CONCAT44(fVar32,SUB84(param_1,0));
            bVar34 = 0.0 < fVar32;
          }
          else {
            param_1 = param_1 - in_dr0;
            bVar34 = (double)((ulonglong)dVar30 & 0xffffffff00000000) < param_1;
          }
          if (bVar34) goto LAB_8c61ab9c;
          *DAT_8c61ae10 = *DAT_8c61ae10 + uVar20;
          iVar10 = 0;
          uVar31 = (undefined4)in_dr14;
          if (0 < (int)(uVar20 - 1)) {
            do {
              piVar5 = DAT_8c61ae20;
              uVar1 = *puVar16;
              puVar16 = puVar16 + 1;
              pdVar8 = (double *)((uint)uVar1 * 0x20 + *DAT_8c61ae0c);
              if (in_FPSCR_SZ == '\0') {
                dVar28 = (double)CONCAT44(*(undefined4 *)DAT_8c61ae14,*(undefined4 *)pdVar8);
                in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),*DAT_8c61ae1c);
              }
              else {
                in_xd2 = *pdVar8;
                dVar28 = *DAT_8c61ae14;
              }
              if (in_FPSCR_PR == '\0') {
                dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) * SUB84(dVar28,0),
                                          SUB84(dVar28,0));
              }
              else {
                dVar28 = dVar28 * dVar28;
              }
              if (in_FPSCR_SZ == '\0') {
                dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),
                                          *(undefined4 *)DAT_8c61ae18);
              }
              else {
                in_xd2 = *DAT_8c61ae18;
              }
              if (in_FPSCR_PR == '\0') {
                fVar32 = SUB84(dVar28,0) - (float)((ulonglong)dVar28 >> 0x20);
              }
              else {
                fVar32 = SUB84(dVar28 - dVar28,0);
              }
              if (in_FPSCR_SZ == '\0') {
                dVar28 = (double)CONCAT44(*(undefined4 *)((int)pdVar8 + 4),fVar32);
              }
              else {
                dVar28 = *(double *)((int)pdVar8 + 4);
              }
              if (in_FPSCR_PR == '\0') {
                dVar30 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                          SUB84(in_dr0,0) * (float)((ulonglong)dVar28 >> 0x20));
              }
              else {
                dVar30 = in_dr0 * dVar28;
              }
              if (in_FPSCR_SZ == '\0') {
                dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar8 + 1),SUB84(dVar28,0));
              }
              else {
                dVar28 = pdVar8[1];
              }
              if (in_FPSCR_PR == '\0') {
                dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),
                                          SUB84(dVar28,0) - SUB84(dVar30,0));
              }
              else {
                dVar28 = dVar28 - dVar30;
              }
              if (in_FPSCR_SZ == '\0') {
                dVar30 = (double)CONCAT44((int)((ulonglong)dVar30 >> 0x20),
                                          *(undefined4 *)(DAT_8c61ae14 + 1));
              }
              if (in_FPSCR_PR == '\0') {
                fVar32 = SUB84(dVar30,0) * (float)((ulonglong)dVar28 >> 0x20);
                fVar26 = SUB84(dVar28,0) - fVar32;
              }
              else {
                fVar32 = SUB84(dVar30 * dVar28,0);
                fVar26 = SUB84(dVar28 - dVar30 * dVar28,0);
              }
              if (in_FPSCR_SZ == '\0') {
                pdVar17 = (double *)(unaff_r8 + 0x18);
                *(float *)pdVar17 = fVar26;
              }
              else {
                pdVar17 = (double *)(unaff_r8 + 0x14);
                *pdVar17 = in_xd2;
              }
              iVar23 = *piVar5;
              if (in_FPSCR_SZ == '\0') {
                dVar28 = (double)CONCAT44(*(undefined4 *)((int)pdVar8 + 4),fVar32);
                dVar30 = (double)CONCAT44(*(undefined4 *)(iVar23 + 4),*(undefined4 *)(iVar23 + 0x14)
                                         );
              }
              else {
                dVar28 = *(double *)((int)pdVar8 + 4);
                in_xd2 = *(double *)(iVar23 + 0x14);
                dVar30 = *(double *)(iVar23 + 4);
              }
              if (in_FPSCR_PR == '\0') {
                dVar30 = (double)CONCAT44((int)((ulonglong)dVar30 >> 0x20),
                                          SUB84(dVar30,0) * (float)((ulonglong)dVar28 >> 0x20));
              }
              else {
                dVar30 = dVar30 * dVar28;
              }
              if (in_FPSCR_SZ == '\0') {
                dVar28 = (double)CONCAT44(*(undefined4 *)pdVar8,SUB84(dVar28,0));
              }
              else {
                dVar28 = *pdVar8;
              }
              if (in_FPSCR_PR == '\0') {
                dVar30 = (double)(ulonglong)
                                 (uint)((float)((ulonglong)dVar28 >> 0x20) *
                                        (float)((ulonglong)dVar30 >> 0x20) + SUB84(dVar30,0));
              }
              if (in_FPSCR_SZ == '\0') {
                dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar8 + 1),SUB84(dVar28,0));
                uVar29 = CONCAT44(*(undefined4 *)(iVar23 + 0x24),SUB84(dVar30,0));
              }
              else {
                dVar28 = pdVar8[1];
                uVar29 = *(ulonglong *)(iVar23 + 0x24);
              }
              if (in_FPSCR_PR == '\0') {
                uVar29 = (ulonglong)
                         (uint)((float)((ulonglong)dVar28 >> 0x20) * (float)(uVar29 >> 0x20) +
                               (float)uVar29);
              }
              dVar27 = in_xd14;
              dVar30 = in_xd14;
              if (in_FPSCR_SZ == '\0') {
                dVar27 = (double)CONCAT44(uVar31,(int)uVar29);
                dVar30 = (double)CONCAT44(uVar31,SUB84(dVar28,0));
              }
              if (in_FPSCR_PR == '\0') {
                dVar27 = (double)CONCAT44((float)((ulonglong)dVar30 >> 0x20) * SUB84(dVar27,0) +
                                          (float)((ulonglong)dVar27 >> 0x20),SUB84(dVar27,0));
              }
              if (in_FPSCR_SZ == '\0') {
                pdVar17 = (double *)((int)pdVar17 + -4);
                *(int *)pdVar17 = (int)((ulonglong)dVar27 >> 0x20);
                dVar28 = (double)CONCAT44(*(undefined4 *)((int)pdVar8 + 4),SUB84(dVar30,0));
              }
              else {
                pdVar17 = pdVar17 + -1;
                *pdVar17 = dVar27;
                dVar28 = *(double *)((int)pdVar8 + 4);
              }
              uVar3 = DAT_8c61ae24;
              pdVar19 = (double *)*piVar5;
              if (in_FPSCR_SZ == '\0') {
                dVar30 = (double)CONCAT44(*(undefined4 *)pdVar19,*(undefined4 *)(pdVar19 + 2));
              }
              else {
                in_xd2 = pdVar19[2];
                dVar30 = *pdVar19;
              }
              if (in_FPSCR_PR == '\0') {
                dVar30 = (double)CONCAT44((int)((ulonglong)dVar30 >> 0x20),
                                          SUB84(dVar30,0) * (float)((ulonglong)dVar28 >> 0x20));
              }
              else {
                dVar30 = dVar30 * dVar28;
              }
              if (in_FPSCR_SZ == '\0') {
                dVar28 = (double)CONCAT44(*(undefined4 *)pdVar8,SUB84(dVar28,0));
              }
              else {
                dVar28 = *pdVar8;
              }
              if (in_FPSCR_PR == '\0') {
                dVar30 = (double)(ulonglong)
                                 (uint)((float)((ulonglong)dVar28 >> 0x20) *
                                        (float)((ulonglong)dVar30 >> 0x20) + SUB84(dVar30,0));
              }
              if (in_FPSCR_SZ == '\0') {
                dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar8 + 1),SUB84(dVar28,0));
                dVar30 = (double)CONCAT44(*(undefined4 *)(pdVar19 + 4),SUB84(dVar30,0));
              }
              else {
                dVar28 = pdVar8[1];
                dVar30 = pdVar19[4];
              }
              if (in_FPSCR_PR == '\0') {
                dVar30 = (double)(ulonglong)
                                 (uint)((float)((ulonglong)dVar28 >> 0x20) *
                                        (float)((ulonglong)dVar30 >> 0x20) + SUB84(dVar30,0));
              }
              in_dr0 = in_xd14;
              dVar27 = in_xd14;
              if (in_FPSCR_SZ == '\0') {
                in_dr0 = (double)CONCAT44(uVar31,SUB84(dVar28,0));
                dVar27 = (double)CONCAT44(uVar31,SUB84(dVar30,0));
              }
              if (in_FPSCR_PR == '\0') {
                dVar27 = (double)CONCAT44((float)((ulonglong)in_dr0 >> 0x20) * SUB84(dVar27,0) +
                                          (float)((ulonglong)dVar27 >> 0x20),SUB84(dVar27,0));
              }
              if (in_FPSCR_SZ == '\0') {
                *(int *)((int)pdVar17 + -4) = (int)((ulonglong)dVar27 >> 0x20);
                *(undefined4 *)(pdVar17 + -1) = *(undefined4 *)(pdVar8 + 3);
                *(undefined4 *)((int)pdVar17 + -0xc) = *(undefined4 *)((int)pdVar8 + 0x14);
                pdVar17 = pdVar17 + -2;
                *(undefined4 *)pdVar17 = *(undefined4 *)(pdVar8 + 2);
              }
              else {
                pdVar17[-1] = dVar27;
                pdVar17[-2] = pdVar8[3];
                pdVar17[-3] = *(double *)((int)pdVar8 + 0x14);
                in_xd2 = pdVar8[2];
                pdVar17 = pdVar17 + -4;
                *pdVar17 = in_xd2;
              }
              *(undefined4 *)((int)pdVar17 + -4) = uVar3;
              iVar10 = iVar10 + 1;
              unaff_r8 = (int)pdVar17 + 0x1c;
            } while (iVar10 < (int)(uVar20 - 1));
          }
          piVar5 = DAT_8c61ae20;
          puVar14 = puVar16 + 1;
          pdVar8 = (double *)((uint)*puVar16 * 0x20 + *DAT_8c61ae0c);
          if (in_FPSCR_SZ == '\0') {
            dVar28 = (double)CONCAT44(*(undefined4 *)DAT_8c61ae14,*(undefined4 *)pdVar8);
            in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),*DAT_8c61ae1c);
          }
          else {
            in_xd2 = *pdVar8;
            dVar28 = *DAT_8c61ae14;
          }
          if (in_FPSCR_PR == '\0') {
            dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) * SUB84(dVar28,0),
                                      SUB84(dVar28,0));
          }
          else {
            dVar28 = dVar28 * dVar28;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),*(undefined4 *)DAT_8c61ae18);
          }
          else {
            in_xd2 = *DAT_8c61ae18;
          }
          if (in_FPSCR_PR == '\0') {
            fVar32 = SUB84(dVar28,0) - (float)((ulonglong)dVar28 >> 0x20);
          }
          else {
            fVar32 = SUB84(dVar28 - dVar28,0);
          }
          if (in_FPSCR_SZ == '\0') {
            dVar28 = (double)CONCAT44(*(undefined4 *)((int)pdVar8 + 4),fVar32);
          }
          else {
            dVar28 = *(double *)((int)pdVar8 + 4);
          }
          if (in_FPSCR_PR == '\0') {
            dVar30 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                      SUB84(in_dr0,0) * (float)((ulonglong)dVar28 >> 0x20));
          }
          else {
            dVar30 = in_dr0 * dVar28;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar8 + 1),SUB84(dVar28,0));
          }
          else {
            dVar28 = pdVar8[1];
          }
          if (in_FPSCR_PR == '\0') {
            dVar28 = (double)CONCAT44((int)((ulonglong)dVar28 >> 0x20),
                                      SUB84(dVar28,0) - SUB84(dVar30,0));
          }
          else {
            dVar28 = dVar28 - dVar30;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar30 = (double)CONCAT44((int)((ulonglong)dVar30 >> 0x20),
                                      *(undefined4 *)(DAT_8c61ae14 + 1));
          }
          if (in_FPSCR_PR == '\0') {
            fVar32 = SUB84(dVar30,0) * (float)((ulonglong)dVar28 >> 0x20);
            fVar26 = SUB84(dVar28,0) - fVar32;
          }
          else {
            fVar32 = SUB84(dVar30 * dVar28,0);
            fVar26 = SUB84(dVar28 - dVar30 * dVar28,0);
          }
          if (in_FPSCR_SZ == '\0') {
            pdVar17 = (double *)(unaff_r8 + 0x18);
            *(float *)pdVar17 = fVar26;
          }
          else {
            pdVar17 = (double *)(unaff_r8 + 0x14);
            *pdVar17 = in_xd2;
          }
          iVar10 = *piVar5;
          if (in_FPSCR_SZ == '\0') {
            dVar28 = (double)CONCAT44(*(undefined4 *)((int)pdVar8 + 4),fVar32);
            dVar30 = (double)CONCAT44(*(undefined4 *)(iVar10 + 4),*(undefined4 *)(iVar10 + 0x14));
          }
          else {
            dVar28 = *(double *)((int)pdVar8 + 4);
            in_xd2 = *(double *)(iVar10 + 0x14);
            dVar30 = *(double *)(iVar10 + 4);
          }
          if (in_FPSCR_PR == '\0') {
            dVar30 = (double)CONCAT44((int)((ulonglong)dVar30 >> 0x20),
                                      SUB84(dVar30,0) * (float)((ulonglong)dVar28 >> 0x20));
          }
          else {
            dVar30 = dVar30 * dVar28;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar28 = (double)CONCAT44(*(undefined4 *)pdVar8,SUB84(dVar28,0));
          }
          else {
            dVar28 = *pdVar8;
          }
          if (in_FPSCR_PR == '\0') {
            dVar30 = (double)(ulonglong)
                             (uint)((float)((ulonglong)dVar28 >> 0x20) *
                                    (float)((ulonglong)dVar30 >> 0x20) + SUB84(dVar30,0));
          }
          if (in_FPSCR_SZ == '\0') {
            dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar8 + 1),SUB84(dVar28,0));
            uVar29 = CONCAT44(*(undefined4 *)(iVar10 + 0x24),SUB84(dVar30,0));
          }
          else {
            dVar28 = pdVar8[1];
            uVar29 = *(ulonglong *)(iVar10 + 0x24);
          }
          if (in_FPSCR_PR == '\0') {
            uVar29 = (ulonglong)
                     (uint)((float)((ulonglong)dVar28 >> 0x20) * (float)(uVar29 >> 0x20) +
                           (float)uVar29);
          }
          dVar27 = in_xd14;
          dVar30 = in_xd14;
          if (in_FPSCR_SZ == '\0') {
            dVar27 = (double)CONCAT44(uVar31,(int)uVar29);
            dVar30 = (double)CONCAT44(uVar31,SUB84(dVar28,0));
          }
          if (in_FPSCR_PR == '\0') {
            dVar27 = (double)CONCAT44((float)((ulonglong)dVar30 >> 0x20) * SUB84(dVar27,0) +
                                      (float)((ulonglong)dVar27 >> 0x20),SUB84(dVar27,0));
          }
          if (in_FPSCR_SZ == '\0') {
            pdVar17 = (double *)((int)pdVar17 + -4);
            *(int *)pdVar17 = (int)((ulonglong)dVar27 >> 0x20);
            dVar28 = (double)CONCAT44(*(undefined4 *)((int)pdVar8 + 4),SUB84(dVar30,0));
          }
          else {
            pdVar17 = pdVar17 + -1;
            *pdVar17 = dVar27;
            dVar28 = *(double *)((int)pdVar8 + 4);
          }
          uVar3 = DAT_8c61ae28;
          pdVar19 = (double *)*piVar5;
          if (in_FPSCR_SZ == '\0') {
            dVar30 = (double)CONCAT44(*(undefined4 *)pdVar19,*(undefined4 *)(pdVar19 + 2));
          }
          else {
            in_xd2 = pdVar19[2];
            dVar30 = *pdVar19;
          }
          if (in_FPSCR_PR == '\0') {
            dVar30 = (double)CONCAT44((int)((ulonglong)dVar30 >> 0x20),
                                      SUB84(dVar30,0) * (float)((ulonglong)dVar28 >> 0x20));
          }
          else {
            dVar30 = dVar30 * dVar28;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar28 = (double)CONCAT44(*(undefined4 *)pdVar8,SUB84(dVar28,0));
          }
          else {
            dVar28 = *pdVar8;
          }
          if (in_FPSCR_PR == '\0') {
            dVar30 = (double)(ulonglong)
                             (uint)((float)((ulonglong)dVar28 >> 0x20) *
                                    (float)((ulonglong)dVar30 >> 0x20) + SUB84(dVar30,0));
          }
          if (in_FPSCR_SZ == '\0') {
            dVar28 = (double)CONCAT44(*(undefined4 *)(pdVar8 + 1),SUB84(dVar28,0));
            dVar30 = (double)CONCAT44(*(undefined4 *)(pdVar19 + 4),SUB84(dVar30,0));
          }
          else {
            dVar28 = pdVar8[1];
            dVar30 = pdVar19[4];
          }
          if (in_FPSCR_PR == '\0') {
            dVar30 = (double)(ulonglong)
                             (uint)((float)((ulonglong)dVar28 >> 0x20) *
                                    (float)((ulonglong)dVar30 >> 0x20) + SUB84(dVar30,0));
          }
          in_dr0 = in_xd14;
          dVar27 = in_xd14;
          if (in_FPSCR_SZ == '\0') {
            in_dr0 = (double)CONCAT44(uVar31,SUB84(dVar28,0));
            dVar27 = (double)CONCAT44(uVar31,SUB84(dVar30,0));
          }
          if (in_FPSCR_PR == '\0') {
            dVar27 = (double)CONCAT44((float)((ulonglong)in_dr0 >> 0x20) * SUB84(dVar27,0) +
                                      (float)((ulonglong)dVar27 >> 0x20),SUB84(dVar27,0));
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)pdVar17 + -4) = (int)((ulonglong)dVar27 >> 0x20);
            *(undefined4 *)(pdVar17 + -1) = *(undefined4 *)(pdVar8 + 3);
            *(undefined4 *)((int)pdVar17 + -0xc) = *(undefined4 *)((int)pdVar8 + 0x14);
            pdVar17 = pdVar17 + -2;
            *(undefined4 *)pdVar17 = *(undefined4 *)(pdVar8 + 2);
          }
          else {
            pdVar17[-1] = dVar27;
            pdVar17[-2] = pdVar8[3];
            pdVar17[-3] = *(double *)((int)pdVar8 + 0x14);
            in_xd2 = pdVar8[2];
            pdVar17 = pdVar17 + -4;
            *pdVar17 = in_xd2;
          }
          *(undefined4 *)((int)pdVar17 + -4) = uVar3;
          unaff_r8 = (int)pdVar17 + 0x1c;
        }
        else {
          *DAT_8c61ae2c = 0;
          puVar9 = puVar14 + 2;
          puVar14 = puVar14 + 3;
          iVar18 = 0;
          iVar10 = (uint)*puVar16 * 0x20 + *piVar5;
          iVar23 = (uint)*puVar9 * 0x20 + *piVar5;
          if (0 < (int)(uVar20 - 2)) {
            do {
              uVar1 = *puVar14;
              puVar14 = puVar14 + 1;
              iVar21 = (uint)uVar1 * 0x20 + *DAT_8c61ae0c;
              if ((*(uint *)(iVar10 + 0xc) & *(uint *)(iVar23 + 0xc) & *(uint *)(iVar21 + 0xc) & 1)
                  == 0) {
                in_dr0 = (double)(*(code *)PTR_FUN_8c61ae30)(iVar23,iVar21);
              }
              iVar18 = iVar18 + 1;
              iVar10 = iVar23;
              iVar23 = iVar21;
            } while (iVar18 < (int)(uVar20 - 2));
          }
        }
      }
      else {
LAB_8c61ab9c:
        puVar14 = puVar16 + uVar20;
      }
    }
  }
  else if (uVar6 == _DAT_8c61a4f4) {
    *DAT_8c61ae34 = *(undefined4 *)((int)puVar25 + -0xc);
    fVar32 = (float)((ulonglong)param_1 >> 0x20);
    fVar26 = (float)((ulonglong)param_3 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)puVar25 + -0x10) = (int)((ulonglong)in_dr0 >> 0x20);
      *(int *)((int)puVar25 + -0x14) = SUB84(in_dr0,0);
      *(int *)((int)puVar25 + -0x18) = (int)((ulonglong)in_dr2 >> 0x20);
      *(int *)((int)puVar25 + -0x1c) = (int)in_dr2;
      *(float *)((int)puVar25 + -0x20) = fVar32;
      *(float *)((int)puVar25 + -0x24) = SUB84(param_1,0);
      *(int *)((int)puVar25 + -0x28) = (int)((ulonglong)param_2 >> 0x20);
      *(int *)((int)puVar25 + -0x2c) = (int)param_2;
      *(float *)((int)puVar25 + -0x30) = fVar26;
      *(float *)((int)puVar25 + -0x34) = SUB84(param_3,0);
      *(int *)((int)puVar25 + -0x38) = (int)((ulonglong)param_4 >> 0x20);
      *(int *)((int)puVar25 + -0x3c) = (int)param_4;
      *(int *)((int)puVar25 + -0x40) = (int)((ulonglong)in_dr12 >> 0x20);
      *(int *)((int)puVar25 + -0x44) = (int)in_dr12;
      *(int *)((int)puVar25 + -0x48) = (int)((ulonglong)in_dr14 >> 0x20);
      *(int *)((int)puVar25 + -0x4c) = (int)in_dr14;
      fVar2 = DAT_8c61adfc;
    }
    else {
      *(double *)((int)puVar25 + -0x14) = in_dr0;
      *(undefined8 *)((int)puVar25 + -0x1c) = in_xd0;
      *(undefined8 *)((int)puVar25 + -0x24) = in_dr2;
      *(double *)((int)puVar25 + -0x2c) = in_xd2;
      *(double *)((int)puVar25 + -0x34) = param_1;
      *(double *)((int)puVar25 + -0x3c) = in_xd4;
      *(undefined8 *)((int)puVar25 + -0x44) = param_2;
      *(undefined8 *)((int)puVar25 + -0x4c) = in_xd6;
      *(double *)((int)puVar25 + -0x54) = param_3;
      *(undefined8 *)((int)puVar25 + -0x5c) = in_xd8;
      *(undefined8 *)((int)puVar25 + -100) = param_4;
      *(undefined8 *)((int)puVar25 + -0x6c) = in_xd10;
      *(undefined8 *)((int)puVar25 + -0x74) = in_dr12;
      *(undefined8 *)((int)puVar25 + -0x7c) = in_xd12;
      *(undefined8 *)((int)puVar25 + -0x84) = in_dr14;
      *(double *)((int)puVar25 + -0x8c) = in_xd14;
      fVar2 = DAT_8c61ae00;
    }
    dVar28 = (double)CONCAT44(fVar2,DAT_8c61adfc);
    if (in_FPSCR_PR == '\0') {
      dVar30 = (double)CONCAT44(fVar32 * fVar2,SUB84(param_1,0) * fVar2);
      dVar28 = (double)CONCAT44(fVar26 * fVar2,SUB84(param_3,0) * fVar2);
    }
    else {
      dVar30 = param_1 * dVar28 * dVar28;
      dVar28 = param_3 * dVar28 * dVar28;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar27 = (double)CONCAT44(*(undefined4 *)((int)DAT_8c61ae00 + 4),(int)param_2);
      dVar33 = (double)CONCAT44(*(undefined4 *)((int)DAT_8c61ae00 + 8),(int)param_4);
    }
    else {
      dVar27 = *(double *)((int)DAT_8c61ae00 + 8);
      dVar33 = *(double *)((int)DAT_8c61ae00 + 0x10);
    }
    if (in_FPSCR_PR == '\0') {
      dVar27 = (double)CONCAT44(-(float)((ulonglong)dVar27 >> 0x20),SUB84(dVar27,0));
      dVar33 = (double)CONCAT44(-(float)((ulonglong)dVar33 >> 0x20),SUB84(dVar33,0));
    }
    else {
      dVar27 = -dVar27;
      dVar33 = -dVar33;
    }
    (*(code *)PTR_FUN_8c61ae38)(dVar30,dVar27,dVar28,dVar33,*DAT_8c61ae34);
  }
  return;
}

