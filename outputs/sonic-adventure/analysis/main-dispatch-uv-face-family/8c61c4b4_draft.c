/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c61c66a) */
/* WARNING: Removing unreachable block (ram,0x8c61c65c) */
/* WARNING: Removing unreachable block (ram,0x8c61c626) */
/* WARNING: Removing unreachable block (ram,0x8c61ca3c) */
/* WARNING: Removing unreachable block (ram,0x8c61c902) */
/* WARNING: Removing unreachable block (ram,0x8c61c97a) */
/* WARNING: Removing unreachable block (ram,0x8c61c938) */
/* WARNING: Removing unreachable block (ram,0x8c61c90e) */
/* WARNING: Removing unreachable block (ram,0x8c61ca4e) */
/* WARNING: Removing unreachable block (ram,0x8c61ce44) */
/* WARNING: Removing unreachable block (ram,0x8c61cc8c) */
/* WARNING: Removing unreachable block (ram,0x8c61ccce) */
/* WARNING: Removing unreachable block (ram,0x8c61cd1e) */
/* WARNING: Removing unreachable block (ram,0x8c61cc9a) */
/* WARNING: Removing unreachable block (ram,0x8c61ce56) */
/* WARNING: Removing unreachable block (ram,0x8c61c632) */
/* WARNING: Removing unreachable block (ram,0x8c61c690) */
/* WARNING: Removing unreachable block (ram,0x8c61c72c) */
/* WARNING: Removing unreachable block (ram,0x8c61c69e) */
/* WARNING: Removing unreachable block (ram,0x8c61ca70) */
/* WARNING: Removing unreachable block (ram,0x8c61c9a0) */
/* WARNING: Removing unreachable block (ram,0x8c61c96c) */
/* WARNING: Removing unreachable block (ram,0x8c61c946) */
/* WARNING: Removing unreachable block (ram,0x8c61ca5e) */
/* WARNING: Removing unreachable block (ram,0x8c61ce64) */
/* WARNING: Removing unreachable block (ram,0x8c61ccdc) */
/* WARNING: Removing unreachable block (ram,0x8c61cd2c) */
/* WARNING: Removing unreachable block (ram,0x8c61cd6a) */
/* WARNING: Removing unreachable block (ram,0x8c61ce74) */
/* WARNING: Removing unreachable block (ram,0x8c61c73e) */
/* WARNING: Removing unreachable block (ram,0x8c61c76e) */
/* WARNING: Removing unreachable block (ram,0x8c61c74e) */
/* WARNING: Removing unreachable block (ram,0x8c61ceb0) */
/* WARNING: Removing unreachable block (ram,0x8c61cea0) */
/* WARNING: Removing unreachable block (ram,0x8c61ca8e) */
/* WARNING: Removing unreachable block (ram,0x8c61ca9e) */
/* WARNING: Removing unreachable block (ram,0x8c61c9ae) */
/* WARNING: Removing unreachable block (ram,0x8c61c760) */
/* WARNING: Removing unreachable block (ram,0x8c61c77e) */
/* WARNING: Removing unreachable block (ram,0x8c61ca7e) */
/* WARNING: Removing unreachable block (ram,0x8c61cd78) */
/* WARNING: Removing unreachable block (ram,0x8c61caae) */

void FUN_8c61c4b4(double param_1,ushort *param_2)

{
  short sVar1;
  ushort uVar2;
  undefined *puVar3;
  uint uVar4;
  int iVar5;
  uint uVar6;
  int iVar7;
  uint uVar8;
  ushort *puVar9;
  undefined4 *unaff_r8;
  double *pdVar10;
  undefined4 *puVar11;
  ushort *puVar12;
  ushort *puVar13;
  ushort *puVar14;
  short *psVar15;
  short *psVar16;
  double *pdVar17;
  int iVar18;
  double *unaff_r12;
  double *unaff_r13;
  double *pdVar19;
  double *unaff_r14;
  double *pdVar20;
  undefined8 *puVar21;
  double in_dr0;
  double dVar22;
  float fVar23;
  float fVar24;
  undefined8 in_dr2;
  undefined4 uVar27;
  double dVar25;
  double dVar26;
  undefined4 uVar29;
  undefined4 uVar30;
  undefined8 uVar28;
  undefined8 in_dr14;
  double dVar31;
  undefined8 in_xd0;
  double in_xd2;
  double in_xd4;
  double in_xd14;
  bool bVar32;
  undefined4 in_PR;
  undefined2 uVar33;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_28 [8];
  undefined8 uStack_20;
  
  uVar29 = DAT_8c61c890;
  uVar30 = DAT_8c61c5b8;
  if (in_FPSCR_SZ == '\0') {
    uStack_20 = (double)CONCAT44((int)in_dr14,(undefined4)uStack_20);
    puVar21 = &uStack_20;
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_20._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
  }
  else {
    puVar21 = (undefined8 *)auStack_28;
    uStack_20 = in_xd14;
  }
  *(undefined4 *)((int)puVar21 + -4) = in_PR;
  if (in_FPSCR_SZ == '\0') {
    dVar31 = (double)CONCAT44(DAT_8c61c5a4,(int)in_dr14);
  }
  else {
    dVar31 = (double)CONCAT44(DAT_8c61c5a8,DAT_8c61c5a4);
  }
  *(ushort **)((int)puVar21 + -0x20) = param_2;
  psVar15 = *(short **)(param_2 + 10);
  uVar4 = *param_2 & DAT_8c61c5a8;
  puVar12 = *(ushort **)(param_2 + 2);
  if (uVar4 == 0) {
    *DAT_8c61c5b0 = *DAT_8c61c5b0 + (uint)*(ushort *)(*(int *)((int)puVar21 + -0x20) + 2) * 3;
    *(int *)((int)puVar21 + -0x2c) = DAT_8c61c5b4;
    *(undefined4 *)((int)puVar21 + -0x28) = uVar30;
    *(int *)((int)puVar21 + -0x24) = DAT_8c61c5b4 + 8;
    for (iVar7 = 0; uVar30 = SUB84(param_1,0),
        iVar7 < (int)(uint)*(ushort *)(*(int *)((int)puVar21 + -0x20) + 2); iVar7 = iVar7 + 1) {
      uVar2 = *puVar12;
      puVar9 = puVar12 + 1;
      puVar13 = puVar12 + 2;
      iVar18 = *DAT_8c61c5bc;
      puVar12 = puVar12 + 3;
      pdVar20 = (double *)((uint)uVar2 * 0x20 + iVar18);
      pdVar17 = (double *)((uint)*puVar9 * 0x20 + iVar18);
      pdVar19 = (double *)((uint)*puVar13 * 0x20 + iVar18);
      if ((*(uint *)((int)pdVar20 + 0xc) & *(uint *)((int)pdVar17 + 0xc) &
           *(uint *)((int)pdVar19 + 0xc) & 0x3f) == 0) {
        if (((*(uint *)((int)pdVar20 + 0xc) | *(uint *)((int)pdVar17 + 0xc) |
             *(uint *)((int)pdVar19 + 0xc)) & 1) == 0) {
          if (in_FPSCR_SZ == '\0') {
            uVar29 = *(undefined4 *)(pdVar17 + 2);
            dVar26 = (double)CONCAT44(uVar29,*(undefined4 *)(pdVar20 + 2));
            dVar25 = (double)CONCAT44(uVar29,uVar30);
          }
          else {
            dVar26 = pdVar17[2];
            in_xd2 = pdVar20[2];
            uVar29 = (undefined4)((ulonglong)dVar26 >> 0x20);
            dVar25 = dVar26;
          }
          if (in_FPSCR_PR == '\0') {
            dVar26 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) - SUB84(dVar26,0),
                                      SUB84(dVar26,0));
          }
          else {
            dVar26 = dVar26 - dVar26;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),
                                      *(undefined4 *)((int)pdVar19 + 0x14));
            dVar25 = (double)CONCAT44(uVar29,*(undefined4 *)((int)pdVar17 + 0x14));
          }
          else {
            in_xd2 = *(double *)((int)pdVar19 + 0x14);
          }
          fVar24 = SUB84(dVar25,0);
          if (in_FPSCR_PR == '\0') {
            dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),SUB84(dVar26,0) - fVar24);
          }
          else {
            dVar26 = dVar26 - dVar25;
          }
          if (in_FPSCR_SZ == '\0') {
            in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar24);
          }
          if (in_FPSCR_PR == '\0') {
            dVar26 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) * SUB84(dVar26,0),
                                      SUB84(dVar26,0));
          }
          else {
            dVar26 = dVar26 * dVar26;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),
                                      *(undefined4 *)((int)pdVar20 + 0x14));
          }
          else {
            in_xd2 = *(double *)((int)pdVar20 + 0x14);
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                      SUB84(in_dr0,0) - SUB84(dVar26,0));
          }
          else {
            dVar22 = in_dr0 - dVar26;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),*(undefined4 *)(pdVar19 + 2))
            ;
          }
          else {
            in_xd2 = pdVar19[2];
          }
          if (in_FPSCR_PR == '\0') {
            dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),
                                      SUB84(dVar26,0) - (float)((ulonglong)dVar25 >> 0x20));
          }
          else {
            dVar26 = dVar26 - dVar25;
          }
          uVar4 = (uint)((ulonglong)dVar26 >> 0x20);
          param_1 = dVar26;
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44(uVar4,fVar24);
          }
          if (in_FPSCR_PR == '\0') {
            in_dr0 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                      SUB84(dVar22,0) * SUB84(dVar26,0));
          }
          else {
            in_dr0 = dVar22 * dVar26;
          }
          if (in_FPSCR_PR == '\0') {
            fVar24 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
            param_1 = (double)CONCAT44(fVar24,SUB84(param_1,0));
            bVar32 = 0.0 < fVar24;
          }
          else {
            param_1 = param_1 - in_dr0;
            bVar32 = (double)((ulonglong)uVar4 << 0x20) < param_1;
          }
          uVar30 = SUB84(param_1,0);
          if (!bVar32) {
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44(*(undefined4 *)pdVar20,*(undefined4 *)pdVar17);
            }
            else {
              dVar25 = *pdVar20;
              in_xd2 = *pdVar17;
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = dVar25 + dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)pdVar19);
            }
            else {
              in_xd2 = *pdVar19;
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = dVar25 + dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)puVar21 + -0x10) = (int)((ulonglong)dVar25 >> 0x20);
              dVar25 = (double)CONCAT44(*(undefined4 *)((int)pdVar20 + 4),
                                        *(undefined4 *)((int)pdVar17 + 4));
            }
            else {
              *(double *)((int)puVar21 + -0x10) = dVar25;
              dVar25 = *(double *)((int)pdVar20 + 4);
              in_xd2 = *(double *)((int)pdVar17 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = dVar25 + dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                        *(undefined4 *)((int)pdVar19 + 4));
            }
            else {
              in_xd2 = *(double *)((int)pdVar19 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = dVar25 + dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)puVar21 + -0xc) = (int)((ulonglong)dVar25 >> 0x20);
              dVar25 = (double)CONCAT44(*(undefined4 *)(pdVar20 + 1),*(undefined4 *)(pdVar17 + 1));
            }
            else {
              *(double *)((int)puVar21 + -0xc) = dVar25;
              dVar25 = pdVar20[1];
              in_xd2 = pdVar17[1];
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = dVar25 + dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                        *(undefined4 *)(pdVar19 + 1));
            }
            else {
              in_xd2 = pdVar19[1];
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = dVar25 + dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)puVar21 + -8) = (int)((ulonglong)dVar25 >> 0x20);
            }
            else {
              *(double *)((int)puVar21 + -8) = dVar25;
            }
            dVar25 = (double)(*(code *)PTR_FUN_8c61c864)((undefined1 *)((int)puVar21 + -0x10));
            if (in_FPSCR_SZ == '\0') {
              dVar26 = (double)CONCAT44(*(undefined4 *)DAT_8c61c868,
                                        *(undefined4 *)((int)puVar21 + -0x10));
            }
            else {
              in_xd2 = *(double *)((int)puVar21 + -0x10);
              dVar26 = *DAT_8c61c868;
            }
            if (in_FPSCR_PR == '\0') {
              dVar26 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) * SUB84(dVar26,0),
                                        SUB84(dVar26,0));
            }
            else {
              dVar26 = dVar26 * dVar26;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),*(undefined4 *)DAT_8c61c86c
                                       );
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*DAT_8c61c870);
            }
            else {
              in_xd2 = *DAT_8c61c86c;
            }
            if (in_FPSCR_PR == '\0') {
              fVar24 = SUB84(dVar26,0) - (float)((ulonglong)dVar26 >> 0x20);
            }
            else {
              fVar24 = SUB84(dVar26 - dVar26,0);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar26 = (double)CONCAT44(*(undefined4 *)((int)puVar21 + -0xc),fVar24);
            }
            else {
              dVar26 = *(double *)((int)puVar21 + -0xc);
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                        SUB84(dVar25,0) * (float)((ulonglong)dVar26 >> 0x20));
            }
            else {
              dVar25 = dVar25 * dVar26;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar26 = (double)CONCAT44(*(undefined4 *)((int)puVar21 + -8),SUB84(dVar26,0));
            }
            else {
              dVar26 = *(double *)((int)puVar21 + -8);
            }
            if (in_FPSCR_PR == '\0') {
              dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),
                                        SUB84(dVar26,0) - SUB84(dVar25,0));
            }
            else {
              dVar26 = dVar26 - dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                        *(undefined4 *)(DAT_8c61c868 + 1));
            }
            fVar24 = (float)((ulonglong)dVar26 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              in_dr0 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),SUB84(dVar25,0) * fVar24);
            }
            else {
              in_dr0 = dVar25 * dVar26;
            }
            param_1 = in_xd2;
            if (in_FPSCR_SZ == '\0') {
              param_1 = (double)CONCAT44(SUB84(dVar26,0),uVar30);
            }
            fVar23 = (float)((ulonglong)dVar31 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              param_1 = (double)CONCAT44((float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0),
                                         SUB84(param_1,0));
              dVar25 = (double)CONCAT44(fVar24,(float)(int)psVar15[1] * fVar23);
            }
            else {
              param_1 = param_1 - in_dr0;
              dVar25 = (double)CONCAT44(fVar24,(float)(int)psVar15[1]) * dVar31;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar10 = (double *)(unaff_r8 + 5);
              *(int *)pdVar10 = SUB84(dVar25,0);
            }
            else {
              pdVar10 = (double *)(unaff_r8 + 4);
              *pdVar10 = in_xd2;
            }
            uVar30 = (undefined4)((ulonglong)dVar25 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44(uVar30,(float)(int)*psVar15 * fVar23);
            }
            else {
              dVar25 = (double)CONCAT44(uVar30,(float)(int)*psVar15) * dVar31;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)pdVar10 + -4) = SUB84(dVar25,0);
              uVar30 = DAT_8c61c874;
              *(undefined4 *)(pdVar10 + -1) = *(undefined4 *)(pdVar20 + 3);
              *(undefined4 *)((int)pdVar10 + -0xc) = *(undefined4 *)((int)pdVar20 + 0x14);
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                        *(undefined4 *)(pdVar20 + 2));
              pdVar10 = pdVar10 + -2;
              *(undefined4 *)pdVar10 = *(undefined4 *)(pdVar20 + 2);
            }
            else {
              pdVar10[-1] = in_xd2;
              uVar30 = DAT_8c61c874;
              pdVar10[-2] = pdVar20[3];
              pdVar10[-3] = *(double *)((int)pdVar20 + 0x14);
              in_xd2 = pdVar20[2];
              pdVar10 = pdVar10 + -4;
              *pdVar10 = in_xd2;
            }
            *(undefined4 *)((int)pdVar10 + -4) = uVar30;
            uVar30 = (undefined4)((ulonglong)dVar25 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44(uVar30,(float)(int)psVar15[3] * fVar23);
            }
            else {
              dVar25 = (double)CONCAT44(uVar30,(float)(int)psVar15[3]) * dVar31;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar10 = pdVar10 + 6;
              *(int *)pdVar10 = SUB84(dVar25,0);
            }
            else {
              pdVar10 = (double *)((int)pdVar10 + 0x2c);
              *pdVar10 = in_xd2;
            }
            uVar30 = DAT_8c61c874;
            uVar29 = (undefined4)((ulonglong)dVar25 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44(uVar29,(float)(int)psVar15[2] * fVar23);
            }
            else {
              dVar25 = (double)CONCAT44(uVar29,(float)(int)psVar15[2]) * dVar31;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)pdVar10 + -4) = SUB84(dVar25,0);
              *(undefined4 *)(pdVar10 + -1) = *(undefined4 *)(pdVar17 + 3);
              *(undefined4 *)((int)pdVar10 + -0xc) = *(undefined4 *)((int)pdVar17 + 0x14);
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                        *(undefined4 *)(pdVar17 + 2));
              pdVar10 = pdVar10 + -2;
              *(undefined4 *)pdVar10 = *(undefined4 *)(pdVar17 + 2);
            }
            else {
              pdVar10[-1] = in_xd2;
              pdVar10[-2] = pdVar17[3];
              pdVar10[-3] = *(double *)((int)pdVar17 + 0x14);
              in_xd2 = pdVar17[2];
              pdVar10 = pdVar10 + -4;
              *pdVar10 = in_xd2;
            }
            *(undefined4 *)((int)pdVar10 + -4) = uVar30;
            if (in_FPSCR_SZ == '\0') {
              pdVar10 = (double *)((int)pdVar10 + 0x34);
              *(int *)pdVar10 = (int)((ulonglong)param_1 >> 0x20);
            }
            else {
              pdVar10 = pdVar10 + 6;
              *pdVar10 = param_1;
            }
            uVar30 = (undefined4)((ulonglong)dVar25 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44(uVar30,(float)(int)psVar15[5] * fVar23);
            }
            else {
              dVar25 = (double)CONCAT44(uVar30,(float)(int)psVar15[5]) * dVar31;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar10 = (double *)((int)pdVar10 + -4);
              *(int *)pdVar10 = SUB84(dVar25,0);
            }
            else {
              pdVar10 = pdVar10 + -1;
              *pdVar10 = in_xd2;
            }
            uVar30 = DAT_8c61c878;
            if (in_FPSCR_PR == '\0') {
              fVar23 = (float)(int)psVar15[4] * fVar23;
            }
            else {
              fVar23 = SUB84((double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                              (float)(int)psVar15[4]) * dVar31,0);
            }
            if (in_FPSCR_SZ == '\0') {
              *(float *)((int)pdVar10 + -4) = fVar23;
              *(undefined4 *)(pdVar10 + -1) = *(undefined4 *)(pdVar19 + 3);
              *(undefined4 *)((int)pdVar10 + -0xc) = *(undefined4 *)((int)pdVar19 + 0x14);
              pdVar10 = pdVar10 + -2;
              *(undefined4 *)pdVar10 = *(undefined4 *)(pdVar19 + 2);
            }
            else {
              pdVar10[-1] = in_xd2;
              pdVar10[-2] = pdVar19[3];
              pdVar10[-3] = *(double *)((int)pdVar19 + 0x14);
              in_xd2 = pdVar19[2];
              pdVar10 = pdVar10 + -4;
              *pdVar10 = in_xd2;
            }
            *(undefined4 *)((int)pdVar10 + -4) = uVar30;
            unaff_r8 = (undefined4 *)((int)pdVar10 + 0x1c);
            *DAT_8c61c87c = *DAT_8c61c87c + 3;
          }
        }
        else {
          *DAT_8c61c880 = 1;
          if (in_FPSCR_SZ == '\0') {
            dVar25 = (double)CONCAT44(*(undefined4 *)pdVar20,*(undefined4 *)pdVar17);
          }
          else {
            in_xd2 = *pdVar17;
            dVar25 = *pdVar20;
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                      SUB84(dVar25,0));
          }
          else {
            dVar25 = dVar25 + dVar25;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)pdVar19);
          }
          else {
            in_xd2 = *pdVar19;
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                      SUB84(dVar25,0));
          }
          else {
            dVar25 = dVar25 + dVar25;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)puVar21 + -0x10) = (int)((ulonglong)dVar25 >> 0x20);
            dVar25 = (double)CONCAT44(*(undefined4 *)((int)pdVar20 + 4),
                                      *(undefined4 *)((int)pdVar17 + 4));
          }
          else {
            *(double *)((int)puVar21 + -0x10) = dVar25;
            in_xd2 = *(double *)((int)pdVar17 + 4);
            dVar25 = *(double *)((int)pdVar20 + 4);
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                      SUB84(dVar25,0));
          }
          else {
            dVar25 = dVar25 + dVar25;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                      *(undefined4 *)((int)pdVar19 + 4));
          }
          else {
            in_xd2 = *(double *)((int)pdVar19 + 4);
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                      SUB84(dVar25,0));
          }
          else {
            dVar25 = dVar25 + dVar25;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)puVar21 + -0xc) = (int)((ulonglong)dVar25 >> 0x20);
            dVar25 = (double)CONCAT44(*(undefined4 *)(pdVar20 + 1),*(undefined4 *)(pdVar17 + 1));
          }
          else {
            *(double *)((int)puVar21 + -0xc) = dVar25;
            dVar25 = pdVar20[1];
            in_xd2 = pdVar17[1];
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                      SUB84(dVar25,0));
          }
          else {
            dVar25 = dVar25 + dVar25;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)(pdVar19 + 1))
            ;
          }
          else {
            in_xd2 = pdVar19[1];
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                      SUB84(dVar25,0));
          }
          else {
            dVar25 = dVar25 + dVar25;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)puVar21 + -8) = (int)((ulonglong)dVar25 >> 0x20);
          }
          else {
            *(double *)((int)puVar21 + -8) = dVar25;
          }
          dVar25 = (double)(*(code *)PTR_FUN_8c61c864)((undefined1 *)((int)puVar21 + -0x10));
          if (in_FPSCR_SZ == '\0') {
            dVar26 = (double)CONCAT44(*(undefined4 *)DAT_8c61c868,
                                      *(undefined4 *)((int)puVar21 + -0x10));
          }
          else {
            in_xd2 = *(double *)((int)puVar21 + -0x10);
            dVar26 = *DAT_8c61c868;
          }
          if (in_FPSCR_PR == '\0') {
            dVar26 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) * SUB84(dVar26,0),
                                      SUB84(dVar26,0));
          }
          else {
            dVar26 = dVar26 * dVar26;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),*(undefined4 *)DAT_8c61c86c);
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*DAT_8c61c870);
          }
          else {
            in_xd2 = *DAT_8c61c86c;
          }
          if (in_FPSCR_PR == '\0') {
            fVar24 = SUB84(dVar26,0) - (float)((ulonglong)dVar26 >> 0x20);
          }
          else {
            fVar24 = SUB84(dVar26 - dVar26,0);
          }
          if (in_FPSCR_SZ == '\0') {
            dVar26 = (double)CONCAT44(*(undefined4 *)((int)puVar21 + -0xc),fVar24);
          }
          else {
            dVar26 = *(double *)((int)puVar21 + -0xc);
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                      SUB84(dVar25,0) * (float)((ulonglong)dVar26 >> 0x20));
          }
          else {
            dVar25 = dVar25 * dVar26;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar26 = (double)CONCAT44(*(undefined4 *)((int)puVar21 + -8),SUB84(dVar26,0));
          }
          else {
            dVar26 = *(double *)((int)puVar21 + -8);
          }
          if (in_FPSCR_PR == '\0') {
            dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),
                                      SUB84(dVar26,0) - SUB84(dVar25,0));
          }
          else {
            dVar26 = dVar26 - dVar25;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                      *(undefined4 *)(DAT_8c61c868 + 1));
          }
          fVar24 = (float)((ulonglong)dVar26 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),SUB84(dVar25,0) * fVar24);
          }
          else {
            dVar25 = dVar25 * dVar26;
          }
          param_1 = in_xd2;
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44(SUB84(dVar26,0),uVar30);
          }
          if (in_FPSCR_PR == '\0') {
            param_1 = (double)((ulonglong)param_1 & 0xffffffff);
            uVar33 = (undefined2)(int)(float)(int)*psVar15;
          }
          else {
            param_1 = param_1 - dVar25;
            uVar33 = (undefined2)(int)(double)CONCAT44(fVar24,(float)(int)*psVar15);
          }
          **(undefined2 **)((int)puVar21 + -0x2c) = uVar33;
          if (in_FPSCR_PR == '\0') {
            uVar33 = (undefined2)(int)(float)(int)psVar15[1];
          }
          else {
            uVar33 = (undefined2)(int)(double)CONCAT44(fVar24,(float)(int)psVar15[1]);
          }
          *(undefined2 *)(*(int *)((int)puVar21 + -0x2c) + 2) = uVar33;
          if (in_FPSCR_PR == '\0') {
            uVar33 = (undefined2)(int)(float)(int)psVar15[2];
          }
          else {
            uVar33 = (undefined2)(int)(double)CONCAT44(fVar24,(float)(int)psVar15[2]);
          }
          **(undefined2 **)((int)puVar21 + -0x28) = uVar33;
          if (in_FPSCR_PR == '\0') {
            uVar33 = (undefined2)(int)(float)(int)psVar15[3];
          }
          else {
            uVar33 = (undefined2)(int)(double)CONCAT44(fVar24,(float)(int)psVar15[3]);
          }
          *(undefined2 *)(*(int *)((int)puVar21 + -0x28) + 2) = uVar33;
          if (in_FPSCR_PR == '\0') {
            uVar33 = (undefined2)(int)(float)(int)psVar15[4];
          }
          else {
            uVar33 = (undefined2)(int)(double)CONCAT44(fVar24,(float)(int)psVar15[4]);
          }
          **(undefined2 **)((int)puVar21 + -0x24) = uVar33;
          iVar18 = DAT_8c61c884;
          if (in_FPSCR_PR == '\0') {
            uVar33 = (undefined2)(int)(float)(int)psVar15[5];
          }
          else {
            uVar33 = (undefined2)(int)(double)CONCAT44(fVar24,(float)(int)psVar15[5]);
          }
          *(undefined2 *)(*(int *)((int)puVar21 + -0x24) + 2) = uVar33;
          in_dr0 = (double)(*(code *)PTR_FUN_8c61c888)(pdVar20,pdVar17,pdVar19,iVar18);
        }
      }
      psVar15 = psVar15 + 6;
    }
  }
  else if (uVar4 == (int)DAT_8c61c5a0) {
    *DAT_8c61c88c = *DAT_8c61c88c + (uint)*(ushort *)(*(int *)((int)puVar21 + -0x20) + 2) * 4;
    *(undefined4 *)((int)puVar21 + -0x24) = 0;
    *(int *)((int)puVar21 + -0x2c) = DAT_8c61c884;
    *(undefined4 *)((int)puVar21 + -0x28) = uVar29;
    *(undefined4 *)((int)puVar21 + -0x1c) = DAT_8c61c894;
    *(int *)((int)puVar21 + -0x18) = DAT_8c61c884 + 0xc;
    while (puVar3 = PTR_FUN_8c61cb74,
          *(int *)((int)puVar21 + -0x24) <
          (int)(uint)*(ushort *)(*(int *)((int)puVar21 + -0x20) + 2)) {
      uVar2 = *puVar12;
      puVar9 = puVar12 + 1;
      puVar13 = puVar12 + 2;
      iVar7 = *DAT_8c61c898;
      puVar14 = puVar12 + 3;
      puVar12 = puVar12 + 4;
      pdVar17 = (double *)((uint)uVar2 * 0x20 + iVar7);
      pdVar19 = (double *)((uint)*puVar9 * 0x20 + iVar7);
      pdVar20 = (double *)((uint)*puVar13 * 0x20 + iVar7);
      iVar7 = (uint)*puVar14 * 0x20 + iVar7;
      if ((*(uint *)((int)pdVar17 + 0xc) & *(uint *)((int)pdVar19 + 0xc) &
           *(uint *)((int)pdVar20 + 0xc) & *(uint *)(iVar7 + 0xc) & 0x3f) == 0) {
        if (((*(uint *)((int)pdVar17 + 0xc) | *(uint *)((int)pdVar19 + 0xc) |
              *(uint *)((int)pdVar20 + 0xc) | *(uint *)(iVar7 + 0xc)) & 1) == 0) {
          if (in_FPSCR_SZ == '\0') {
            uVar30 = *(undefined4 *)(pdVar19 + 2);
            dVar26 = (double)CONCAT44(uVar30,*(undefined4 *)(pdVar17 + 2));
            dVar25 = (double)CONCAT44(uVar30,SUB84(param_1,0));
          }
          else {
            dVar26 = pdVar19[2];
            in_xd2 = pdVar17[2];
            uVar30 = (undefined4)((ulonglong)dVar26 >> 0x20);
            dVar25 = dVar26;
          }
          if (in_FPSCR_PR == '\0') {
            dVar26 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) - SUB84(dVar26,0),
                                      SUB84(dVar26,0));
          }
          else {
            dVar26 = dVar26 - dVar26;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),
                                      *(undefined4 *)((int)pdVar20 + 0x14));
            dVar25 = (double)CONCAT44(uVar30,*(undefined4 *)((int)pdVar19 + 0x14));
          }
          else {
            in_xd2 = *(double *)((int)pdVar20 + 0x14);
          }
          fVar24 = SUB84(dVar25,0);
          if (in_FPSCR_PR == '\0') {
            dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),SUB84(dVar26,0) - fVar24);
          }
          else {
            dVar26 = dVar26 - dVar25;
          }
          if (in_FPSCR_SZ == '\0') {
            in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar24);
          }
          if (in_FPSCR_PR == '\0') {
            dVar26 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) * SUB84(dVar26,0),
                                      SUB84(dVar26,0));
          }
          else {
            dVar26 = dVar26 * dVar26;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),
                                      *(undefined4 *)((int)pdVar17 + 0x14));
          }
          else {
            in_xd2 = *(double *)((int)pdVar17 + 0x14);
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                      SUB84(in_dr0,0) - SUB84(dVar26,0));
          }
          else {
            dVar22 = in_dr0 - dVar26;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),*(undefined4 *)(pdVar20 + 2))
            ;
          }
          else {
            in_xd2 = pdVar20[2];
          }
          if (in_FPSCR_PR == '\0') {
            dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),
                                      SUB84(dVar26,0) - (float)((ulonglong)dVar25 >> 0x20));
          }
          else {
            dVar26 = dVar26 - dVar25;
          }
          uVar4 = (uint)((ulonglong)dVar26 >> 0x20);
          param_1 = dVar26;
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44(uVar4,fVar24);
          }
          if (in_FPSCR_PR == '\0') {
            in_dr0 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                      SUB84(dVar22,0) * SUB84(dVar26,0));
          }
          else {
            in_dr0 = dVar22 * dVar26;
          }
          if (in_FPSCR_PR == '\0') {
            fVar24 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
            param_1 = (double)CONCAT44(fVar24,SUB84(param_1,0));
            bVar32 = 0.0 < fVar24;
          }
          else {
            param_1 = param_1 - in_dr0;
            bVar32 = (double)((ulonglong)uVar4 << 0x20) < param_1;
          }
          uVar30 = SUB84(param_1,0);
          if (!bVar32) {
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44(*(undefined4 *)pdVar17,*(undefined4 *)pdVar19);
            }
            else {
              dVar25 = *pdVar17;
              in_xd2 = *pdVar19;
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = dVar25 + dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)pdVar20);
            }
            else {
              in_xd2 = *pdVar20;
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = dVar25 + dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)puVar21 + -0x10) = (int)((ulonglong)dVar25 >> 0x20);
              dVar25 = (double)CONCAT44(*(undefined4 *)((int)pdVar17 + 4),
                                        *(undefined4 *)((int)pdVar19 + 4));
            }
            else {
              *(double *)((int)puVar21 + -0x10) = dVar25;
              dVar25 = *(double *)((int)pdVar17 + 4);
              in_xd2 = *(double *)((int)pdVar19 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = dVar25 + dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                        *(undefined4 *)((int)pdVar20 + 4));
            }
            else {
              in_xd2 = *(double *)((int)pdVar20 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = dVar25 + dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)puVar21 + -0xc) = (int)((ulonglong)dVar25 >> 0x20);
              dVar25 = (double)CONCAT44(*(undefined4 *)(pdVar17 + 1),*(undefined4 *)(pdVar19 + 1));
            }
            else {
              *(double *)((int)puVar21 + -0xc) = dVar25;
              dVar25 = pdVar17[1];
              in_xd2 = pdVar19[1];
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = dVar25 + dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                        *(undefined4 *)(pdVar20 + 1));
            }
            else {
              in_xd2 = pdVar20[1];
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = dVar25 + dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)puVar21 + -8) = (int)((ulonglong)dVar25 >> 0x20);
            }
            else {
              *(double *)((int)puVar21 + -8) = dVar25;
            }
            dVar25 = (double)(*(code *)PTR_FUN_8c61cb74)((undefined1 *)((int)puVar21 + -0x10));
            if (in_FPSCR_SZ == '\0') {
              dVar26 = (double)CONCAT44(*(undefined4 *)DAT_8c61cb78,
                                        *(undefined4 *)((int)puVar21 + -0x10));
            }
            else {
              in_xd2 = *(double *)((int)puVar21 + -0x10);
              dVar26 = *DAT_8c61cb78;
            }
            if (in_FPSCR_PR == '\0') {
              dVar26 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) * SUB84(dVar26,0),
                                        SUB84(dVar26,0));
            }
            else {
              dVar26 = dVar26 * dVar26;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),*(undefined4 *)DAT_8c61cb7c
                                       );
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*DAT_8c61cb80);
            }
            else {
              in_xd2 = *DAT_8c61cb7c;
            }
            if (in_FPSCR_PR == '\0') {
              fVar24 = SUB84(dVar26,0) - (float)((ulonglong)dVar26 >> 0x20);
            }
            else {
              fVar24 = SUB84(dVar26 - dVar26,0);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar26 = (double)CONCAT44(*(undefined4 *)((int)puVar21 + -0xc),fVar24);
            }
            else {
              dVar26 = *(double *)((int)puVar21 + -0xc);
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                        SUB84(dVar25,0) * (float)((ulonglong)dVar26 >> 0x20));
            }
            else {
              dVar25 = dVar25 * dVar26;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar26 = (double)CONCAT44(*(undefined4 *)((int)puVar21 + -8),SUB84(dVar26,0));
            }
            else {
              dVar26 = *(double *)((int)puVar21 + -8);
            }
            if (in_FPSCR_PR == '\0') {
              dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),
                                        SUB84(dVar26,0) - SUB84(dVar25,0));
            }
            else {
              dVar26 = dVar26 - dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                        *(undefined4 *)(DAT_8c61cb78 + 1));
            }
            fVar24 = (float)((ulonglong)dVar26 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              in_dr0 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),SUB84(dVar25,0) * fVar24);
            }
            else {
              in_dr0 = dVar25 * dVar26;
            }
            param_1 = in_xd2;
            if (in_FPSCR_SZ == '\0') {
              param_1 = (double)CONCAT44(SUB84(dVar26,0),uVar30);
            }
            fVar23 = (float)((ulonglong)dVar31 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              param_1 = (double)CONCAT44((float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0),
                                         SUB84(param_1,0));
              dVar25 = (double)CONCAT44(fVar24,(float)(int)psVar15[1] * fVar23);
            }
            else {
              param_1 = param_1 - in_dr0;
              dVar25 = (double)CONCAT44(fVar24,(float)(int)psVar15[1]) * dVar31;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar10 = (double *)(unaff_r8 + 5);
              *(int *)pdVar10 = SUB84(dVar25,0);
            }
            else {
              pdVar10 = (double *)(unaff_r8 + 4);
              *pdVar10 = in_xd2;
            }
            uVar30 = (undefined4)((ulonglong)dVar25 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44(uVar30,(float)(int)*psVar15 * fVar23);
            }
            else {
              dVar25 = (double)CONCAT44(uVar30,(float)(int)*psVar15) * dVar31;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)pdVar10 + -4) = SUB84(dVar25,0);
              uVar30 = DAT_8c61cb84;
              *(undefined4 *)(pdVar10 + -1) = *(undefined4 *)(pdVar17 + 3);
              *(undefined4 *)((int)pdVar10 + -0xc) = *(undefined4 *)((int)pdVar17 + 0x14);
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                        *(undefined4 *)(pdVar17 + 2));
              pdVar10 = pdVar10 + -2;
              *(undefined4 *)pdVar10 = *(undefined4 *)(pdVar17 + 2);
            }
            else {
              pdVar10[-1] = in_xd2;
              uVar30 = DAT_8c61cb84;
              pdVar10[-2] = pdVar17[3];
              pdVar10[-3] = *(double *)((int)pdVar17 + 0x14);
              in_xd2 = pdVar17[2];
              pdVar10 = pdVar10 + -4;
              *pdVar10 = in_xd2;
            }
            *(undefined4 *)((int)pdVar10 + -4) = uVar30;
            uVar30 = (undefined4)((ulonglong)dVar25 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44(uVar30,(float)(int)psVar15[3] * fVar23);
            }
            else {
              dVar25 = (double)CONCAT44(uVar30,(float)(int)psVar15[3]) * dVar31;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar10 = pdVar10 + 6;
              *(int *)pdVar10 = SUB84(dVar25,0);
            }
            else {
              pdVar10 = (double *)((int)pdVar10 + 0x2c);
              *pdVar10 = in_xd2;
            }
            uVar30 = DAT_8c61cb84;
            uVar29 = (undefined4)((ulonglong)dVar25 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44(uVar29,(float)(int)psVar15[2] * fVar23);
            }
            else {
              dVar25 = (double)CONCAT44(uVar29,(float)(int)psVar15[2]) * dVar31;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)pdVar10 + -4) = SUB84(dVar25,0);
              *(undefined4 *)(pdVar10 + -1) = *(undefined4 *)(pdVar19 + 3);
              *(undefined4 *)((int)pdVar10 + -0xc) = *(undefined4 *)((int)pdVar19 + 0x14);
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                        *(undefined4 *)(pdVar19 + 2));
              pdVar10 = pdVar10 + -2;
              *(undefined4 *)pdVar10 = *(undefined4 *)(pdVar19 + 2);
            }
            else {
              pdVar10[-1] = in_xd2;
              pdVar10[-2] = pdVar19[3];
              pdVar10[-3] = *(double *)((int)pdVar19 + 0x14);
              in_xd2 = pdVar19[2];
              pdVar10 = pdVar10 + -4;
              *pdVar10 = in_xd2;
            }
            *(undefined4 *)((int)pdVar10 + -4) = uVar30;
            uVar30 = (undefined4)((ulonglong)param_1 >> 0x20);
            if (in_FPSCR_SZ == '\0') {
              pdVar10 = (double *)((int)pdVar10 + 0x34);
              *(undefined4 *)pdVar10 = uVar30;
            }
            else {
              pdVar10 = pdVar10 + 6;
              *pdVar10 = param_1;
            }
            uVar29 = (undefined4)((ulonglong)dVar25 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44(uVar29,(float)(int)psVar15[5] * fVar23);
            }
            else {
              dVar25 = (double)CONCAT44(uVar29,(float)(int)psVar15[5]) * dVar31;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar10 = (double *)((int)pdVar10 + -4);
              *(int *)pdVar10 = SUB84(dVar25,0);
            }
            else {
              pdVar10 = pdVar10 + -1;
              *pdVar10 = in_xd2;
            }
            uVar29 = DAT_8c61cb84;
            uVar27 = (undefined4)((ulonglong)dVar25 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44(uVar27,(float)(int)psVar15[4] * fVar23);
            }
            else {
              dVar25 = (double)CONCAT44(uVar27,(float)(int)psVar15[4]) * dVar31;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)pdVar10 + -4) = SUB84(dVar25,0);
              *(undefined4 *)(pdVar10 + -1) = *(undefined4 *)(pdVar20 + 3);
              *(undefined4 *)((int)pdVar10 + -0xc) = *(undefined4 *)((int)pdVar20 + 0x14);
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                        *(undefined4 *)(pdVar20 + 2));
              pdVar10 = pdVar10 + -2;
              *(undefined4 *)pdVar10 = *(undefined4 *)(pdVar20 + 2);
            }
            else {
              pdVar10[-1] = in_xd2;
              pdVar10[-2] = pdVar20[3];
              pdVar10[-3] = *(double *)((int)pdVar20 + 0x14);
              in_xd2 = pdVar20[2];
              pdVar10 = pdVar10 + -4;
              *pdVar10 = in_xd2;
            }
            *(undefined4 *)((int)pdVar10 + -4) = uVar29;
            if (in_FPSCR_SZ == '\0') {
              pdVar10 = (double *)((int)pdVar10 + 0x34);
              *(undefined4 *)pdVar10 = uVar30;
            }
            else {
              pdVar10 = pdVar10 + 6;
              *pdVar10 = param_1;
            }
            uVar30 = (undefined4)((ulonglong)dVar25 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44(uVar30,(float)(int)psVar15[7] * fVar23);
            }
            else {
              dVar25 = (double)CONCAT44(uVar30,(float)(int)psVar15[7]) * dVar31;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar10 = (double *)((int)pdVar10 + -4);
              *(int *)pdVar10 = SUB84(dVar25,0);
            }
            else {
              pdVar10 = pdVar10 + -1;
              *pdVar10 = in_xd2;
            }
            uVar30 = DAT_8c61cb88;
            if (in_FPSCR_PR == '\0') {
              fVar23 = (float)(int)psVar15[6] * fVar23;
            }
            else {
              fVar23 = SUB84((double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                              (float)(int)psVar15[6]) * dVar31,0);
            }
            if (in_FPSCR_SZ == '\0') {
              *(float *)((int)pdVar10 + -4) = fVar23;
              *(undefined4 *)(pdVar10 + -1) = *(undefined4 *)(iVar7 + 0x18);
              *(undefined4 *)((int)pdVar10 + -0xc) = *(undefined4 *)(iVar7 + 0x14);
              pdVar10 = pdVar10 + -2;
              *(undefined4 *)pdVar10 = *(undefined4 *)(iVar7 + 0x10);
            }
            else {
              pdVar10[-1] = in_xd2;
              pdVar10[-2] = *(double *)(iVar7 + 0x18);
              pdVar10[-3] = *(double *)(iVar7 + 0x14);
              in_xd2 = *(double *)(iVar7 + 0x10);
              pdVar10 = pdVar10 + -4;
              *pdVar10 = in_xd2;
            }
            *(undefined4 *)((int)pdVar10 + -4) = uVar30;
            unaff_r8 = (undefined4 *)((int)pdVar10 + 0x1c);
            *DAT_8c61cb8c = *DAT_8c61cb8c + 4;
          }
        }
        else {
          *DAT_8c61cb90 = 1;
          if (in_FPSCR_SZ == '\0') {
            dVar25 = (double)CONCAT44(*(undefined4 *)pdVar17,*(undefined4 *)pdVar19);
          }
          else {
            in_xd2 = *pdVar19;
            dVar25 = *pdVar17;
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                      SUB84(dVar25,0));
          }
          else {
            dVar25 = dVar25 + dVar25;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)pdVar20);
          }
          else {
            in_xd2 = *pdVar20;
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                      SUB84(dVar25,0));
          }
          else {
            dVar25 = dVar25 + dVar25;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)puVar21 + -0x10) = (int)((ulonglong)dVar25 >> 0x20);
            dVar25 = (double)CONCAT44(*(undefined4 *)((int)pdVar17 + 4),
                                      *(undefined4 *)((int)pdVar19 + 4));
          }
          else {
            *(double *)((int)puVar21 + -0x10) = dVar25;
            in_xd2 = *(double *)((int)pdVar19 + 4);
            dVar25 = *(double *)((int)pdVar17 + 4);
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                      SUB84(dVar25,0));
          }
          else {
            dVar25 = dVar25 + dVar25;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                      *(undefined4 *)((int)pdVar20 + 4));
          }
          else {
            in_xd2 = *(double *)((int)pdVar20 + 4);
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                      SUB84(dVar25,0));
          }
          else {
            dVar25 = dVar25 + dVar25;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)puVar21 + -0xc) = (int)((ulonglong)dVar25 >> 0x20);
            dVar25 = (double)CONCAT44(*(undefined4 *)(pdVar17 + 1),*(undefined4 *)(pdVar19 + 1));
          }
          else {
            *(double *)((int)puVar21 + -0xc) = dVar25;
            dVar25 = pdVar17[1];
            in_xd2 = pdVar19[1];
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                      SUB84(dVar25,0));
          }
          else {
            dVar25 = dVar25 + dVar25;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)(pdVar20 + 1))
            ;
          }
          else {
            in_xd2 = pdVar20[1];
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                      SUB84(dVar25,0));
          }
          else {
            dVar25 = dVar25 + dVar25;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)puVar21 + -8) = (int)((ulonglong)dVar25 >> 0x20);
          }
          else {
            *(double *)((int)puVar21 + -8) = dVar25;
          }
          dVar25 = (double)(*(code *)puVar3)((undefined1 *)((int)puVar21 + -0x10));
          if (in_FPSCR_SZ == '\0') {
            dVar26 = (double)CONCAT44(*(undefined4 *)DAT_8c61cb78,
                                      *(undefined4 *)((int)puVar21 + -0x10));
          }
          else {
            in_xd2 = *(double *)((int)puVar21 + -0x10);
            dVar26 = *DAT_8c61cb78;
          }
          if (in_FPSCR_PR == '\0') {
            dVar26 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) * SUB84(dVar26,0),
                                      SUB84(dVar26,0));
          }
          else {
            dVar26 = dVar26 * dVar26;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),*(undefined4 *)DAT_8c61cb7c);
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*DAT_8c61cb80);
          }
          else {
            in_xd2 = *DAT_8c61cb7c;
          }
          if (in_FPSCR_PR == '\0') {
            fVar24 = SUB84(dVar26,0) - (float)((ulonglong)dVar26 >> 0x20);
          }
          else {
            fVar24 = SUB84(dVar26 - dVar26,0);
          }
          if (in_FPSCR_SZ == '\0') {
            dVar26 = (double)CONCAT44(*(undefined4 *)((int)puVar21 + -0xc),fVar24);
          }
          else {
            dVar26 = *(double *)((int)puVar21 + -0xc);
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                      SUB84(dVar25,0) * (float)((ulonglong)dVar26 >> 0x20));
          }
          else {
            dVar25 = dVar25 * dVar26;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar26 = (double)CONCAT44(*(undefined4 *)((int)puVar21 + -8),SUB84(dVar26,0));
          }
          else {
            dVar26 = *(double *)((int)puVar21 + -8);
          }
          if (in_FPSCR_PR == '\0') {
            dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),
                                      SUB84(dVar26,0) - SUB84(dVar25,0));
          }
          else {
            dVar26 = dVar26 - dVar25;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                      *(undefined4 *)(DAT_8c61cb78 + 1));
          }
          fVar24 = (float)((ulonglong)dVar26 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            in_dr0 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),SUB84(dVar25,0) * fVar24);
          }
          else {
            in_dr0 = dVar25 * dVar26;
          }
          dVar25 = in_xd2;
          if (in_FPSCR_SZ == '\0') {
            dVar31 = (double)CONCAT44((int)((ulonglong)dVar31 >> 0x20),SUB84(dVar26,0));
            dVar25 = in_xd14;
          }
          in_xd14 = dVar25;
          if (in_FPSCR_PR == '\0') {
            dVar31 = (double)CONCAT44((int)((ulonglong)dVar31 >> 0x20),
                                      SUB84(dVar31,0) - SUB84(in_dr0,0));
            uVar33 = (undefined2)(int)(float)(int)*psVar15;
          }
          else {
            dVar31 = dVar31 - in_dr0;
            uVar33 = (undefined2)(int)(double)CONCAT44(fVar24,(float)(int)*psVar15);
          }
          **(undefined2 **)((int)puVar21 + -0x2c) = uVar33;
          if (in_FPSCR_PR == '\0') {
            uVar33 = (undefined2)(int)(float)(int)psVar15[1];
          }
          else {
            uVar33 = (undefined2)(int)(double)CONCAT44(fVar24,(float)(int)psVar15[1]);
          }
          *(undefined2 *)(*(int *)((int)puVar21 + -0x2c) + 2) = uVar33;
          if (in_FPSCR_PR == '\0') {
            uVar33 = (undefined2)(int)(float)(int)psVar15[2];
          }
          else {
            uVar33 = (undefined2)(int)(double)CONCAT44(fVar24,(float)(int)psVar15[2]);
          }
          **(undefined2 **)((int)puVar21 + -0x28) = uVar33;
          if (in_FPSCR_PR == '\0') {
            uVar33 = (undefined2)(int)(float)(int)psVar15[3];
          }
          else {
            uVar33 = (undefined2)(int)(double)CONCAT44(fVar24,(float)(int)psVar15[3]);
          }
          *(undefined2 *)(*(int *)((int)puVar21 + -0x28) + 2) = uVar33;
          if (in_FPSCR_PR == '\0') {
            uVar33 = (undefined2)(int)(float)(int)psVar15[4];
          }
          else {
            uVar33 = (undefined2)(int)(double)CONCAT44(fVar24,(float)(int)psVar15[4]);
          }
          **(undefined2 **)((int)puVar21 + -0x1c) = uVar33;
          if (in_FPSCR_PR == '\0') {
            uVar33 = (undefined2)(int)(float)(int)psVar15[5];
          }
          else {
            uVar33 = (undefined2)(int)(double)CONCAT44(fVar24,(float)(int)psVar15[5]);
          }
          *(undefined2 *)(*(int *)((int)puVar21 + -0x1c) + 2) = uVar33;
          if (in_FPSCR_PR == '\0') {
            uVar33 = (undefined2)(int)(float)(int)psVar15[6];
          }
          else {
            uVar33 = (undefined2)(int)(double)CONCAT44(fVar24,(float)(int)psVar15[6]);
          }
          **(undefined2 **)((int)puVar21 + -0x18) = uVar33;
          if (in_FPSCR_PR == '\0') {
            uVar33 = (undefined2)(int)(float)(int)psVar15[7];
          }
          else {
            uVar33 = (undefined2)(int)(double)CONCAT44(fVar24,(float)(int)psVar15[7]);
          }
          *(undefined2 *)(*(int *)((int)puVar21 + -0x18) + 2) = uVar33;
          if ((*(uint *)((int)pdVar17 + 0xc) & *(uint *)((int)pdVar19 + 0xc) &
               *(uint *)((int)pdVar20 + 0xc) & 1) == 0) {
            dVar25 = in_xd14;
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)((ulonglong)param_1 & 0xffffffff);
            }
            param_1 = dVar25;
            in_dr0 = (double)(*(code *)PTR_FUN_8c61cb94)
                                       (pdVar17,pdVar19,pdVar20,
                                        *(undefined4 *)((int)puVar21 + -0x2c));
          }
          if ((*(uint *)((int)pdVar19 + 0xc) & *(uint *)((int)pdVar20 + 0xc) &
               *(uint *)(iVar7 + 0xc) & 1) == 0) {
            dVar25 = in_xd14;
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)((ulonglong)param_1 & 0xffffffff);
            }
            param_1 = dVar25;
            in_dr0 = (double)(*(code *)PTR_FUN_8c61cb94)
                                       (pdVar19,pdVar20,iVar7,*(undefined4 *)((int)puVar21 + -0x28))
            ;
          }
        }
      }
      psVar15 = psVar15 + 8;
      *(int *)((int)puVar21 + -0x24) = *(int *)((int)puVar21 + -0x24) + 1;
    }
  }
  else if (uVar4 == DAT_8c61c5ac) {
    *(undefined4 *)((int)puVar21 + -0x18) = 0;
    *(int *)((int)puVar21 + -0x24) = DAT_8c61cb98 + 8;
    *(int *)((int)puVar21 + -0x1c) = DAT_8c61cb98;
    *(int *)((int)puVar21 + -0x28) = DAT_8c61cb98 + 4;
    while (uVar30 = SUB84(param_1,0),
          *(int *)((int)puVar21 + -0x18) <
          (int)(uint)*(ushort *)(*(int *)((int)puVar21 + -0x20) + 2)) {
      uVar8 = 0;
      puVar13 = puVar12 + 1;
      iVar7 = 0;
      uVar4 = (uint)*puVar12;
      uVar6 = 0xffffffff;
      *(uint *)((int)puVar21 + -0x2c) = uVar4;
      *DAT_8c61cb9c = *DAT_8c61cb9c + uVar4;
      puVar9 = puVar13;
      if (uVar4 != 0) {
        do {
          uVar2 = *puVar9;
          puVar9 = puVar9 + 1;
          iVar7 = iVar7 + 1;
          iVar18 = (uint)uVar2 * 0x20 + *DAT_8c61cba0;
          *(int *)((int)puVar21 + -0x14) = iVar18;
          uVar4 = *(uint *)(iVar18 + 0xc);
          uVar6 = uVar6 & uVar4;
          uVar8 = uVar8 | uVar4;
        } while (iVar7 < *(int *)((int)puVar21 + -0x2c));
      }
      if ((uVar6 & 0x3f) == 0) {
        if ((uVar8 & 1) == 0) {
          iVar7 = *DAT_8c61cd9c;
          unaff_r14 = (double *)((uint)puVar12[2] * 0x20 + iVar7);
          if (in_FPSCR_SZ == '\0') {
            uVar30 = *(undefined4 *)(unaff_r14 + 2);
          }
          else {
            in_xd4 = unaff_r14[2];
          }
          unaff_r12 = (double *)((uint)*puVar13 * 0x20 + iVar7);
          if (in_FPSCR_SZ == '\0') {
            dVar25 = (double)CONCAT44(uVar30,*(undefined4 *)(unaff_r12 + 2));
          }
          else {
            in_xd2 = unaff_r12[2];
            dVar25 = in_xd4;
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) - SUB84(dVar25,0),
                                      SUB84(dVar25,0));
          }
          else {
            dVar25 = dVar25 - dVar25;
          }
          unaff_r13 = (double *)((uint)puVar12[3] * 0x20 + iVar7);
          if (in_FPSCR_SZ == '\0') {
            fVar24 = *(float *)((int)unaff_r14 + 0x14);
            dVar26 = (double)CONCAT44(fVar24,uVar30);
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                      *(undefined4 *)((int)unaff_r13 + 0x14));
            in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar24);
          }
          else {
            dVar26 = *(double *)((int)unaff_r14 + 0x14);
            in_xd2 = *(double *)((int)unaff_r13 + 0x14);
            fVar24 = (float)((ulonglong)dVar26 >> 0x20);
          }
          if (in_FPSCR_PR == '\0') {
            fVar24 = SUB84(dVar25,0) - fVar24;
            param_1 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * fVar24,fVar24);
          }
          else {
            param_1 = (dVar25 - dVar26) * (dVar25 - dVar26);
          }
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),
                                       *(undefined4 *)((int)unaff_r12 + 0x14));
          }
          else {
            in_xd2 = *(double *)((int)unaff_r12 + 0x14);
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                      SUB84(in_dr0,0) - SUB84(param_1,0));
          }
          else {
            dVar25 = in_dr0 - param_1;
          }
          uVar30 = (undefined4)((ulonglong)param_1 >> 0x20);
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44(uVar30,SUB84(dVar26,0));
            dVar26 = (double)CONCAT44(uVar30,*(undefined4 *)(unaff_r13 + 2));
          }
          else {
            in_xd2 = unaff_r13[2];
            dVar26 = param_1;
          }
          if (in_FPSCR_PR == '\0') {
            fVar24 = SUB84(dVar26,0) - SUB84(param_1,0);
            dVar26 = (double)CONCAT44(uVar30,fVar24);
            in_dr0 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),SUB84(dVar25,0) * fVar24);
          }
          else {
            dVar26 = dVar26 - param_1;
            in_dr0 = dVar25 * dVar26;
          }
          if (in_FPSCR_PR == '\0') {
            fVar24 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
            param_1 = (double)CONCAT44(fVar24,SUB84(param_1,0));
            bVar32 = 0.0 < fVar24;
          }
          else {
            param_1 = param_1 - in_dr0;
            bVar32 = (double)((ulonglong)dVar26 & 0xffffffff00000000) < param_1;
          }
          uVar30 = SUB84(param_1,0);
          if (bVar32) {
            puVar12 = puVar13 + *(int *)((int)puVar21 + -0x2c);
            psVar15 = psVar15 + *(int *)((int)puVar21 + -0x2c) * 2;
          }
          else {
            *DAT_8c61cda0 = *DAT_8c61cda0 + *(int *)((int)puVar21 + -0x2c);
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44(*(undefined4 *)unaff_r12,*(undefined4 *)unaff_r14);
            }
            else {
              dVar25 = *unaff_r12;
              in_xd2 = *unaff_r14;
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = dVar25 + dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)unaff_r13);
            }
            else {
              in_xd2 = *unaff_r13;
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = dVar25 + dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)puVar21 + -0x10) = (int)((ulonglong)dVar25 >> 0x20);
              dVar25 = (double)CONCAT44(*(undefined4 *)((int)unaff_r12 + 4),
                                        *(undefined4 *)((int)unaff_r14 + 4));
            }
            else {
              *(double *)((int)puVar21 + -0x10) = dVar25;
              dVar25 = *(double *)((int)unaff_r12 + 4);
              in_xd2 = *(double *)((int)unaff_r14 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = dVar25 + dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                        *(undefined4 *)((int)unaff_r13 + 4));
            }
            else {
              in_xd2 = *(double *)((int)unaff_r13 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = dVar25 + dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)puVar21 + -0xc) = (int)((ulonglong)dVar25 >> 0x20);
              dVar25 = (double)CONCAT44(*(undefined4 *)(unaff_r12 + 1),
                                        *(undefined4 *)(unaff_r14 + 1));
            }
            else {
              *(double *)((int)puVar21 + -0xc) = dVar25;
              in_xd2 = unaff_r14[1];
              dVar25 = unaff_r12[1];
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = dVar25 + dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                        *(undefined4 *)(unaff_r13 + 1));
            }
            else {
              in_xd2 = unaff_r13[1];
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = dVar25 + dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)puVar21 + -8) = (int)((ulonglong)dVar25 >> 0x20);
            }
            else {
              *(double *)((int)puVar21 + -8) = dVar25;
            }
            dVar25 = (double)(*(code *)PTR_FUN_8c61cda4)((undefined1 *)((int)puVar21 + -0x10));
            if (in_FPSCR_SZ == '\0') {
              dVar26 = (double)CONCAT44(*(undefined4 *)DAT_8c61cda8,
                                        *(undefined4 *)((int)puVar21 + -0x10));
            }
            else {
              in_xd2 = *(double *)((int)puVar21 + -0x10);
              dVar26 = *DAT_8c61cda8;
            }
            if (in_FPSCR_PR == '\0') {
              dVar26 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) * SUB84(dVar26,0),
                                        SUB84(dVar26,0));
            }
            else {
              dVar26 = dVar26 * dVar26;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),*(undefined4 *)DAT_8c61cdac
                                       );
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*DAT_8c61cdb0);
            }
            else {
              in_xd2 = *DAT_8c61cdac;
            }
            if (in_FPSCR_PR == '\0') {
              fVar24 = SUB84(dVar26,0) - (float)((ulonglong)dVar26 >> 0x20);
            }
            else {
              fVar24 = SUB84(dVar26 - dVar26,0);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar26 = (double)CONCAT44(*(undefined4 *)((int)puVar21 + -0xc),fVar24);
            }
            else {
              dVar26 = *(double *)((int)puVar21 + -0xc);
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                        SUB84(dVar25,0) * (float)((ulonglong)dVar26 >> 0x20));
            }
            else {
              dVar25 = dVar25 * dVar26;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar26 = (double)CONCAT44(*(undefined4 *)((int)puVar21 + -8),SUB84(dVar26,0));
            }
            else {
              dVar26 = *(double *)((int)puVar21 + -8);
            }
            if (in_FPSCR_PR == '\0') {
              dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),
                                        SUB84(dVar26,0) - SUB84(dVar25,0));
            }
            else {
              dVar26 = dVar26 - dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                        *(undefined4 *)(DAT_8c61cda8 + 1));
            }
            fVar24 = (float)((ulonglong)dVar26 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              in_dr0 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),SUB84(dVar25,0) * fVar24);
            }
            else {
              in_dr0 = dVar25 * dVar26;
            }
            iVar7 = (uint)*puVar13 * 0x20 + *DAT_8c61cd9c;
            param_1 = in_xd2;
            if (in_FPSCR_SZ == '\0') {
              param_1 = (double)CONCAT44(SUB84(dVar26,0),uVar30);
            }
            if (in_FPSCR_PR == '\0') {
              param_1 = (double)CONCAT44((float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0),
                                         SUB84(param_1,0));
            }
            else {
              param_1 = param_1 - in_dr0;
            }
            fVar23 = (float)((ulonglong)dVar31 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44(fVar24,(float)(int)psVar15[1] * fVar23);
            }
            else {
              dVar25 = (double)CONCAT44(fVar24,(float)(int)psVar15[1]) * dVar31;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar17 = (double *)(unaff_r8 + 5);
              *(int *)pdVar17 = SUB84(dVar25,0);
            }
            else {
              pdVar17 = (double *)(unaff_r8 + 4);
              *pdVar17 = in_xd2;
            }
            uVar30 = DAT_8c61cdb4;
            uVar29 = (undefined4)((ulonglong)dVar25 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44(uVar29,(float)(int)*psVar15 * fVar23);
            }
            else {
              dVar25 = (double)CONCAT44(uVar29,(float)(int)*psVar15) * dVar31;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)pdVar17 + -4) = SUB84(dVar25,0);
              *(undefined4 *)(pdVar17 + -1) = *(undefined4 *)(iVar7 + 0x18);
              *(undefined4 *)((int)pdVar17 + -0xc) = *(undefined4 *)(iVar7 + 0x14);
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                        *(undefined4 *)(iVar7 + 0x10));
              pdVar17 = pdVar17 + -2;
              *(undefined4 *)pdVar17 = *(undefined4 *)(iVar7 + 0x10);
            }
            else {
              pdVar17[-1] = in_xd2;
              pdVar17[-2] = *(double *)(iVar7 + 0x18);
              pdVar17[-3] = *(double *)(iVar7 + 0x14);
              in_xd2 = *(double *)(iVar7 + 0x10);
              pdVar17 = pdVar17 + -4;
              *pdVar17 = in_xd2;
            }
            *(undefined4 *)((int)pdVar17 + -4) = uVar30;
            puVar9 = puVar12 + 3;
            iVar7 = (uint)puVar12[2] * 0x20 + *DAT_8c61cd9c;
            uVar30 = (undefined4)((ulonglong)dVar25 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44(uVar30,(float)(int)psVar15[3] * fVar23);
            }
            else {
              dVar25 = (double)CONCAT44(uVar30,(float)(int)psVar15[3]) * dVar31;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar17 = pdVar17 + 6;
              *(int *)pdVar17 = SUB84(dVar25,0);
            }
            else {
              pdVar17 = (double *)((int)pdVar17 + 0x2c);
              *pdVar17 = in_xd2;
            }
            uVar30 = DAT_8c61cdb4;
            psVar16 = psVar15 + 4;
            uVar29 = (undefined4)((ulonglong)dVar25 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44(uVar29,(float)(int)psVar15[2] * fVar23);
            }
            else {
              dVar25 = (double)CONCAT44(uVar29,(float)(int)psVar15[2]) * dVar31;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)pdVar17 + -4) = SUB84(dVar25,0);
              *(undefined4 *)(pdVar17 + -1) = *(undefined4 *)(iVar7 + 0x18);
              *(undefined4 *)((int)pdVar17 + -0xc) = *(undefined4 *)(iVar7 + 0x14);
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                        *(undefined4 *)(iVar7 + 0x10));
              pdVar17 = pdVar17 + -2;
              *(undefined4 *)pdVar17 = *(undefined4 *)(iVar7 + 0x10);
            }
            else {
              pdVar17[-1] = in_xd2;
              pdVar17[-2] = *(double *)(iVar7 + 0x18);
              pdVar17[-3] = *(double *)(iVar7 + 0x14);
              in_xd2 = *(double *)(iVar7 + 0x10);
              pdVar17 = pdVar17 + -4;
              *pdVar17 = in_xd2;
            }
            uVar29 = (undefined4)((ulonglong)dVar25 >> 0x20);
            *(undefined4 *)((int)pdVar17 + -4) = uVar30;
            iVar18 = *(int *)((int)puVar21 + -0x2c);
            iVar7 = 2;
            puVar11 = (undefined4 *)((int)pdVar17 + 0x1c);
            uVar30 = (undefined4)((ulonglong)param_1 >> 0x20);
            if (2 < iVar18 + -1) {
              do {
                uVar2 = *puVar9;
                puVar9 = puVar9 + 1;
                iVar5 = *DAT_8c61cd9c;
                if (in_FPSCR_SZ == '\0') {
                  pdVar17 = (double *)(puVar11 + 6);
                  *(undefined4 *)pdVar17 = uVar30;
                }
                else {
                  pdVar17 = (double *)(puVar11 + 5);
                  *pdVar17 = param_1;
                }
                iVar5 = (uint)uVar2 * 0x20 + iVar5;
                uVar29 = (undefined4)((ulonglong)dVar25 >> 0x20);
                if (in_FPSCR_PR == '\0') {
                  dVar25 = (double)CONCAT44(uVar29,(float)(int)psVar16[1] * fVar23);
                }
                else {
                  dVar25 = (double)CONCAT44(uVar29,(float)(int)psVar16[1]) * dVar31;
                }
                if (in_FPSCR_SZ == '\0') {
                  pdVar17 = (double *)((int)pdVar17 + -4);
                  *(int *)pdVar17 = SUB84(dVar25,0);
                }
                else {
                  pdVar17 = pdVar17 + -1;
                  *pdVar17 = in_xd2;
                }
                uVar27 = DAT_8c61cdb4;
                sVar1 = *psVar16;
                psVar16 = psVar16 + 2;
                uVar29 = (undefined4)((ulonglong)dVar25 >> 0x20);
                if (in_FPSCR_PR == '\0') {
                  dVar25 = (double)CONCAT44(uVar29,(float)(int)sVar1 * fVar23);
                }
                else {
                  dVar25 = (double)CONCAT44(uVar29,(float)(int)sVar1) * dVar31;
                }
                if (in_FPSCR_SZ == '\0') {
                  *(int *)((int)pdVar17 + -4) = SUB84(dVar25,0);
                  *(undefined4 *)(pdVar17 + -1) = *(undefined4 *)(iVar5 + 0x18);
                  *(undefined4 *)((int)pdVar17 + -0xc) = *(undefined4 *)(iVar5 + 0x14);
                  dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                            *(undefined4 *)(iVar5 + 0x10));
                  pdVar17 = pdVar17 + -2;
                  *(undefined4 *)pdVar17 = *(undefined4 *)(iVar5 + 0x10);
                }
                else {
                  pdVar17[-1] = in_xd2;
                  pdVar17[-2] = *(double *)(iVar5 + 0x18);
                  pdVar17[-3] = *(double *)(iVar5 + 0x14);
                  in_xd2 = *(double *)(iVar5 + 0x10);
                  pdVar17 = pdVar17 + -4;
                  *pdVar17 = in_xd2;
                }
                uVar29 = (undefined4)((ulonglong)dVar25 >> 0x20);
                *(undefined4 *)((int)pdVar17 + -4) = uVar27;
                iVar7 = iVar7 + 1;
                puVar11 = (undefined4 *)((int)pdVar17 + 0x1c);
              } while (iVar7 < iVar18 + -1);
            }
            uVar2 = *puVar9;
            puVar12 = puVar9 + 1;
            iVar7 = *DAT_8c61cd9c;
            if (in_FPSCR_SZ == '\0') {
              pdVar17 = (double *)(puVar11 + 6);
              *(undefined4 *)pdVar17 = uVar30;
            }
            else {
              pdVar17 = (double *)(puVar11 + 5);
              *pdVar17 = param_1;
            }
            iVar7 = (uint)uVar2 * 0x20 + iVar7;
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44(uVar29,(float)(int)psVar16[1] * fVar23);
            }
            else {
              dVar25 = (double)CONCAT44(uVar29,(float)(int)psVar16[1]) * dVar31;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar17 = (double *)((int)pdVar17 + -4);
              *(int *)pdVar17 = SUB84(dVar25,0);
            }
            else {
              pdVar17 = pdVar17 + -1;
              *pdVar17 = in_xd2;
            }
            uVar30 = DAT_8c61cdb8;
            psVar15 = psVar16 + 2;
            if (in_FPSCR_PR == '\0') {
              fVar23 = (float)(int)*psVar16 * fVar23;
            }
            else {
              fVar23 = SUB84((double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),(float)(int)*psVar16)
                             * dVar31,0);
            }
            if (in_FPSCR_SZ == '\0') {
              *(float *)((int)pdVar17 + -4) = fVar23;
              *(undefined4 *)(pdVar17 + -1) = *(undefined4 *)(iVar7 + 0x18);
              *(undefined4 *)((int)pdVar17 + -0xc) = *(undefined4 *)(iVar7 + 0x14);
              pdVar17 = pdVar17 + -2;
              *(undefined4 *)pdVar17 = *(undefined4 *)(iVar7 + 0x10);
            }
            else {
              pdVar17[-1] = in_xd2;
              pdVar17[-2] = *(double *)(iVar7 + 0x18);
              pdVar17[-3] = *(double *)(iVar7 + 0x14);
              in_xd2 = *(double *)(iVar7 + 0x10);
              pdVar17 = pdVar17 + -4;
              *pdVar17 = in_xd2;
            }
            *(undefined4 *)((int)pdVar17 + -4) = uVar30;
            unaff_r8 = (undefined4 *)((int)pdVar17 + 0x1c);
          }
        }
        else {
          *DAT_8c61cf74 = 0;
          if (in_FPSCR_SZ == '\0') {
            dVar25 = (double)CONCAT44(*(undefined4 *)unaff_r12,*(undefined4 *)unaff_r14);
          }
          else {
            in_xd2 = *unaff_r14;
            dVar25 = *unaff_r12;
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                      SUB84(dVar25,0));
          }
          else {
            dVar25 = dVar25 + dVar25;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)unaff_r13);
          }
          else {
            in_xd2 = *unaff_r13;
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                      SUB84(dVar25,0));
          }
          else {
            dVar25 = dVar25 + dVar25;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)puVar21 + -0x10) = (int)((ulonglong)dVar25 >> 0x20);
            dVar25 = (double)CONCAT44(*(undefined4 *)((int)unaff_r12 + 4),
                                      *(undefined4 *)((int)unaff_r14 + 4));
          }
          else {
            *(double *)((int)puVar21 + -0x10) = dVar25;
            in_xd2 = *(double *)((int)unaff_r14 + 4);
            dVar25 = *(double *)((int)unaff_r12 + 4);
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                      SUB84(dVar25,0));
          }
          else {
            dVar25 = dVar25 + dVar25;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                      *(undefined4 *)((int)unaff_r13 + 4));
          }
          else {
            in_xd2 = *(double *)((int)unaff_r13 + 4);
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                      SUB84(dVar25,0));
          }
          else {
            dVar25 = dVar25 + dVar25;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)puVar21 + -0xc) = (int)((ulonglong)dVar25 >> 0x20);
            dVar25 = (double)CONCAT44(*(undefined4 *)(unaff_r12 + 1),*(undefined4 *)(unaff_r14 + 1))
            ;
          }
          else {
            *(double *)((int)puVar21 + -0xc) = dVar25;
            in_xd2 = unaff_r14[1];
            dVar25 = unaff_r12[1];
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                      SUB84(dVar25,0));
          }
          else {
            dVar25 = dVar25 + dVar25;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                      *(undefined4 *)(unaff_r13 + 1));
          }
          else {
            in_xd2 = unaff_r13[1];
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                      SUB84(dVar25,0));
          }
          else {
            dVar25 = dVar25 + dVar25;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)puVar21 + -8) = (int)((ulonglong)dVar25 >> 0x20);
          }
          else {
            *(double *)((int)puVar21 + -8) = dVar25;
          }
          dVar25 = (double)(*(code *)PTR_FUN_8c61cf78)((undefined1 *)((int)puVar21 + -0x10));
          if (in_FPSCR_SZ == '\0') {
            dVar26 = (double)CONCAT44(*(undefined4 *)DAT_8c61cf7c,
                                      *(undefined4 *)((int)puVar21 + -0x10));
          }
          else {
            in_xd2 = *(double *)((int)puVar21 + -0x10);
            dVar26 = *DAT_8c61cf7c;
          }
          if (in_FPSCR_PR == '\0') {
            dVar26 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) * SUB84(dVar26,0),
                                      SUB84(dVar26,0));
          }
          else {
            dVar26 = dVar26 * dVar26;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),*(undefined4 *)DAT_8c61cf80);
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*DAT_8c61cf84);
          }
          else {
            in_xd2 = *DAT_8c61cf80;
          }
          puVar9 = puVar12 + 2;
          if (in_FPSCR_PR == '\0') {
            fVar24 = SUB84(dVar26,0) - (float)((ulonglong)dVar26 >> 0x20);
          }
          else {
            fVar24 = SUB84(dVar26 - dVar26,0);
          }
          if (in_FPSCR_SZ == '\0') {
            dVar26 = (double)CONCAT44(*(undefined4 *)((int)puVar21 + -0xc),fVar24);
          }
          else {
            dVar26 = *(double *)((int)puVar21 + -0xc);
          }
          puVar12 = puVar12 + 3;
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                      SUB84(dVar25,0) * (float)((ulonglong)dVar26 >> 0x20));
          }
          else {
            dVar25 = dVar25 * dVar26;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar26 = (double)CONCAT44(*(undefined4 *)((int)puVar21 + -8),SUB84(dVar26,0));
          }
          else {
            dVar26 = *(double *)((int)puVar21 + -8);
          }
          if (in_FPSCR_PR == '\0') {
            dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),
                                      SUB84(dVar26,0) - SUB84(dVar25,0));
          }
          else {
            dVar26 = dVar26 - dVar25;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                      *(undefined4 *)(DAT_8c61cf7c + 1));
          }
          fVar24 = (float)((ulonglong)dVar26 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            in_dr0 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),SUB84(dVar25,0) * fVar24);
          }
          else {
            in_dr0 = dVar25 * dVar26;
          }
          dVar25 = in_xd2;
          if (in_FPSCR_SZ == '\0') {
            dVar31 = (double)CONCAT44((int)((ulonglong)dVar31 >> 0x20),SUB84(dVar26,0));
            dVar25 = in_xd14;
          }
          in_xd14 = dVar25;
          unaff_r12 = (double *)((uint)*puVar13 * 0x20 + *DAT_8c61cf88);
          if (in_FPSCR_PR == '\0') {
            dVar31 = (double)CONCAT44((int)((ulonglong)dVar31 >> 0x20),
                                      SUB84(dVar31,0) - SUB84(in_dr0,0));
          }
          else {
            dVar31 = dVar31 - in_dr0;
          }
          unaff_r14 = (double *)((uint)*puVar9 * 0x20 + *DAT_8c61cf88);
          if (in_FPSCR_PR == '\0') {
            uVar33 = (undefined2)(int)(float)(int)*psVar15;
          }
          else {
            uVar33 = (undefined2)(int)(double)CONCAT44(fVar24,(float)(int)*psVar15);
          }
          **(undefined2 **)((int)puVar21 + -0x1c) = uVar33;
          if (in_FPSCR_PR == '\0') {
            uVar33 = (undefined2)(int)(float)(int)psVar15[1];
          }
          else {
            uVar33 = (undefined2)(int)(double)CONCAT44(fVar24,(float)(int)psVar15[1]);
          }
          *(undefined2 *)(*(int *)((int)puVar21 + -0x1c) + 2) = uVar33;
          if (in_FPSCR_PR == '\0') {
            uVar33 = (undefined2)(int)(float)(int)psVar15[2];
          }
          else {
            uVar33 = (undefined2)(int)(double)CONCAT44(fVar24,(float)(int)psVar15[2]);
          }
          **(undefined2 **)((int)puVar21 + -0x28) = uVar33;
          dVar25 = (double)CONCAT44(fVar24,(float)(int)psVar15[3]);
          if (in_FPSCR_PR == '\0') {
            uVar33 = (undefined2)(int)(float)(int)psVar15[3];
          }
          else {
            uVar33 = (undefined2)(int)dVar25;
          }
          psVar15 = psVar15 + 4;
          iVar18 = 0;
          *(undefined2 *)(*(int *)((int)puVar21 + -0x28) + 2) = uVar33;
          iVar7 = *(int *)((int)puVar21 + -0x2c);
          *(int *)((int)puVar21 + -0x2c) = iVar7 + -2;
          pdVar17 = unaff_r12;
          if (0 < iVar7 + -2) {
            do {
              unaff_r12 = unaff_r14;
              uVar2 = *puVar12;
              puVar12 = puVar12 + 1;
              unaff_r13 = (double *)((uint)uVar2 * 0x20 + *DAT_8c61cf88);
              uVar30 = (undefined4)((ulonglong)dVar25 >> 0x20);
              if (in_FPSCR_PR == '\0') {
                uVar33 = (undefined2)(int)(float)(int)*psVar15;
              }
              else {
                uVar33 = (undefined2)(int)(double)CONCAT44(uVar30,(float)(int)*psVar15);
              }
              **(undefined2 **)((int)puVar21 + -0x24) = uVar33;
              dVar25 = (double)CONCAT44(uVar30,(float)(int)psVar15[1]);
              if (in_FPSCR_PR == '\0') {
                uVar33 = (undefined2)(int)(float)(int)psVar15[1];
              }
              else {
                uVar33 = (undefined2)(int)dVar25;
              }
              *(undefined2 *)(*(int *)((int)puVar21 + -0x24) + 2) = uVar33;
              psVar15 = psVar15 + 2;
              if ((*(uint *)((int)pdVar17 + 0xc) & *(uint *)((int)unaff_r12 + 0xc) &
                   *(uint *)((int)unaff_r13 + 0xc) & 1) == 0) {
                dVar26 = in_xd14;
                if (in_FPSCR_SZ == '\0') {
                  dVar26 = (double)((ulonglong)param_1 & 0xffffffff);
                }
                param_1 = dVar26;
                in_dr0 = (double)(*(code *)PTR_FUN_8c61cf90)
                                           (pdVar17,unaff_r12,unaff_r13,DAT_8c61cf8c);
              }
              iVar18 = iVar18 + 1;
              **(undefined4 **)((int)puVar21 + -0x1c) = **(undefined4 **)((int)puVar21 + -0x28);
              **(undefined4 **)((int)puVar21 + -0x28) = **(undefined4 **)((int)puVar21 + -0x24);
              pdVar17 = unaff_r12;
              unaff_r14 = unaff_r13;
            } while (iVar18 < *(int *)((int)puVar21 + -0x2c));
          }
        }
      }
      else {
        psVar15 = psVar15 + *(int *)((int)puVar21 + -0x2c) * 2;
        puVar12 = puVar13 + *(int *)((int)puVar21 + -0x2c);
      }
      *(int *)((int)puVar21 + -0x18) = *(int *)((int)puVar21 + -0x18) + 1;
    }
  }
  else if (uVar4 == DAT_8c61c5a8) {
    *DAT_8c61cf94 = *(undefined4 *)((int)puVar21 + -0x20);
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)puVar21 + -0x30) = (int)((ulonglong)in_dr0 >> 0x20);
      *(int *)((int)puVar21 + -0x34) = SUB84(in_dr0,0);
      *(int *)((int)puVar21 + -0x38) = (int)((ulonglong)in_dr2 >> 0x20);
      *(int *)((int)puVar21 + -0x3c) = (int)in_dr2;
      *(int *)((int)puVar21 + -0x40) = (int)((ulonglong)param_1 >> 0x20);
      *(int *)((int)puVar21 + -0x44) = SUB84(param_1,0);
      uVar28 = CONCAT44(DAT_8c61cf6c,SUB84(param_1,0));
    }
    else {
      *(double *)((int)puVar21 + -0x34) = in_dr0;
      *(undefined8 *)((int)puVar21 + -0x3c) = in_xd0;
      *(undefined8 *)((int)puVar21 + -0x44) = in_dr2;
      *(double *)((int)puVar21 + -0x4c) = in_xd2;
      *(double *)((int)puVar21 + -0x54) = param_1;
      *(double *)((int)puVar21 + -0x5c) = in_xd4;
      uVar28 = CONCAT44(uRam8c61cf70,DAT_8c61cf6c);
    }
    (*(code *)PTR_FUN_8c61cf98)(uVar28,*DAT_8c61cf94);
  }
  return;
}

