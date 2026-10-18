/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c61d5e8) */
/* WARNING: Removing unreachable block (ram,0x8c61d5cc) */
/* WARNING: Removing unreachable block (ram,0x8c61d590) */
/* WARNING: Removing unreachable block (ram,0x8c61d536) */
/* WARNING: Removing unreachable block (ram,0x8c61d828) */
/* WARNING: Removing unreachable block (ram,0x8c61d73a) */
/* WARNING: Removing unreachable block (ram,0x8c61d7a2) */
/* WARNING: Removing unreachable block (ram,0x8c61d760) */
/* WARNING: Removing unreachable block (ram,0x8c61d72c) */
/* WARNING: Removing unreachable block (ram,0x8c61d706) */
/* WARNING: Removing unreachable block (ram,0x8c61d6f8) */
/* WARNING: Removing unreachable block (ram,0x8c61d7f8) */
/* WARNING: Removing unreachable block (ram,0x8c61d794) */
/* WARNING: Removing unreachable block (ram,0x8c61d836) */
/* WARNING: Removing unreachable block (ram,0x8c61da74) */
/* WARNING: Removing unreachable block (ram,0x8c61d9b4) */
/* WARNING: Removing unreachable block (ram,0x8c61da84) */
/* WARNING: Removing unreachable block (ram,0x8c61d528) */
/* WARNING: Removing unreachable block (ram,0x8c61d55c) */
/* WARNING: Removing unreachable block (ram,0x8c61d56a) */
/* WARNING: Removing unreachable block (ram,0x8c61d59e) */
/* WARNING: Removing unreachable block (ram,0x8c61d5dc) */
/* WARNING: Removing unreachable block (ram,0x8c61d604) */
/* WARNING: Removing unreachable block (ram,0x8c61da9a) */
/* WARNING: Removing unreachable block (ram,0x8c61d856) */
/* WARNING: Removing unreachable block (ram,0x8c61d76e) */
/* WARNING: Removing unreachable block (ram,0x8c61d808) */
/* WARNING: Removing unreachable block (ram,0x8c61d816) */
/* WARNING: Removing unreachable block (ram,0x8c61d848) */
/* WARNING: Removing unreachable block (ram,0x8c61d5f8) */
/* WARNING: Removing unreachable block (ram,0x8c61d616) */
/* WARNING: Removing unreachable block (ram,0x8c61da00) */
/* WARNING: Removing unreachable block (ram,0x8c61d866) */
/* WARNING: Removing unreachable block (ram,0x8c61d9c2) */
/* WARNING: Removing unreachable block (ram,0x8c61daac) */
/* WARNING: Removing unreachable block (ram,0x8c61da0e) */
/* WARNING: Removing unreachable block (ram,0x8c61dace) */
/* WARNING: Removing unreachable block (ram,0x8c61dade) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c61d408(double param_1,ushort *param_2)

{
  short sVar1;
  ushort uVar2;
  ushort uVar3;
  short *psVar4;
  undefined2 *puVar5;
  undefined2 *puVar6;
  undefined2 *puVar7;
  undefined *puVar8;
  int *piVar9;
  uint uVar10;
  int iVar11;
  uint uVar12;
  int iVar13;
  ushort *puVar14;
  undefined4 *unaff_r8;
  double *pdVar15;
  ushort *puVar16;
  ushort *puVar17;
  ushort *puVar18;
  short *psVar19;
  int iVar20;
  uint uVar21;
  int iVar22;
  uint uVar23;
  undefined8 *puVar24;
  double in_dr0;
  double dVar25;
  double in_dr2;
  undefined4 uVar26;
  undefined4 uVar27;
  undefined4 uVar28;
  double dVar29;
  float fVar30;
  ulonglong in_dr14;
  double dVar31;
  undefined8 in_xd0;
  double in_xd2;
  double in_xd4;
  double dVar32;
  undefined8 in_xd14;
  bool bVar33;
  undefined4 in_PR;
  undefined2 uVar34;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 local_28 [8];
  undefined8 uStack_20;
  
  uVar28 = DAT_8c61d7e4;
  puVar7 = DAT_8c61d7d8;
  puVar6 = DAT_8c61d514;
  puVar5 = DAT_8c61d510;
  if (in_FPSCR_SZ == '\0') {
    uStack_20 = CONCAT44((int)in_dr14,(undefined4)uStack_20);
    puVar24 = &uStack_20;
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_20._0_4_ = (int)(in_dr14 >> 0x20);
  }
  else {
    puVar24 = (undefined8 *)local_28;
    uStack_20 = in_xd14;
  }
  *(undefined4 *)((int)puVar24 + -4) = in_PR;
  if (in_FPSCR_SZ == '\0') {
    in_dr14 = (ulonglong)DAT_8c61d500;
  }
  dVar31 = (double)CONCAT44(0x3f800000,(int)in_dr14);
  *(ushort **)((int)puVar24 + -0x14) = param_2;
  psVar19 = *(short **)(param_2 + 10);
  uVar10 = *param_2 & DAT_8c61d504;
  puVar16 = *(ushort **)(param_2 + 2);
  if (uVar10 == 0) {
    *DAT_8c61d50c = *DAT_8c61d50c + (uint)*(ushort *)(*(int *)((int)puVar24 + -0x14) + 2) * 3;
    for (iVar11 = 0; iVar11 < (int)(uint)*(ushort *)(*(int *)((int)puVar24 + -0x14) + 2);
        iVar11 = iVar11 + 1) {
      uVar2 = *puVar16;
      puVar14 = puVar16 + 1;
      puVar18 = puVar16 + 2;
      iVar13 = *DAT_8c61d518;
      puVar16 = puVar16 + 3;
      iVar22 = (uint)*puVar18 * 0x20 + iVar13;
      iVar20 = (uint)uVar2 * 0x20 + iVar13;
      iVar13 = (uint)*puVar14 * 0x20 + iVar13;
      if ((*(uint *)(iVar20 + 0xc) & *(uint *)(iVar13 + 0xc) & *(uint *)(iVar22 + 0xc) & 0x3f) == 0)
      {
        if (((*(uint *)(iVar20 + 0xc) | *(uint *)(iVar13 + 0xc) | *(uint *)(iVar22 + 0xc)) & 1) == 0
           ) {
          if (in_FPSCR_SZ == '\0') {
            uVar28 = *(undefined4 *)(iVar13 + 0x10);
            dVar29 = (double)CONCAT44(uVar28,*(undefined4 *)(iVar20 + 0x10));
            dVar32 = (double)CONCAT44(uVar28,SUB84(param_1,0));
          }
          else {
            dVar29 = *(double *)(iVar13 + 0x10);
            in_xd2 = *(double *)(iVar20 + 0x10);
            uVar28 = (undefined4)((ulonglong)dVar29 >> 0x20);
            dVar32 = dVar29;
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
                                      *(undefined4 *)(iVar22 + 0x14));
            dVar32 = (double)CONCAT44(uVar28,*(undefined4 *)(iVar13 + 0x14));
          }
          else {
            in_xd2 = *(double *)(iVar22 + 0x14);
          }
          fVar30 = SUB84(dVar32,0);
          if (in_FPSCR_PR == '\0') {
            dVar29 = (double)CONCAT44((int)((ulonglong)dVar29 >> 0x20),SUB84(dVar29,0) - fVar30);
          }
          else {
            dVar29 = dVar29 - dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar30);
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
                                      *(undefined4 *)(iVar20 + 0x14));
          }
          else {
            in_xd2 = *(double *)(iVar20 + 0x14);
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                      SUB84(in_dr0,0) - SUB84(dVar29,0));
          }
          else {
            dVar25 = in_dr0 - dVar29;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar29 = (double)CONCAT44((int)((ulonglong)dVar29 >> 0x20),
                                      *(undefined4 *)(iVar22 + 0x10));
          }
          else {
            in_xd2 = *(double *)(iVar22 + 0x10);
          }
          if (in_FPSCR_PR == '\0') {
            dVar29 = (double)CONCAT44((int)((ulonglong)dVar29 >> 0x20),
                                      SUB84(dVar29,0) - (float)((ulonglong)dVar32 >> 0x20));
          }
          else {
            dVar29 = dVar29 - dVar32;
          }
          uVar10 = (uint)((ulonglong)dVar29 >> 0x20);
          param_1 = dVar29;
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44(uVar10,fVar30);
          }
          if (in_FPSCR_PR == '\0') {
            in_dr0 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                      SUB84(dVar25,0) * SUB84(dVar29,0));
          }
          else {
            in_dr0 = dVar25 * dVar29;
          }
          in_dr2 = (double)((ulonglong)uVar10 << 0x20);
          if (in_FPSCR_PR == '\0') {
            fVar30 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
            param_1 = (double)CONCAT44(fVar30,SUB84(param_1,0));
            bVar33 = 0.0 < fVar30;
          }
          else {
            param_1 = param_1 - in_dr0;
            bVar33 = in_dr2 < param_1;
          }
          if (!bVar33) {
            uVar28 = (undefined4)((ulonglong)dVar31 >> 0x20);
            if (in_FPSCR_SZ == '\0') {
              pdVar15 = (double *)(unaff_r8 + 6);
              *(undefined4 *)pdVar15 = uVar28;
            }
            else {
              pdVar15 = (double *)(unaff_r8 + 5);
              *pdVar15 = dVar31;
            }
            fVar30 = SUB84(dVar31,0);
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44(uVar10,(float)(int)psVar19[1] * fVar30);
            }
            else {
              dVar32 = (double)CONCAT44(uVar10,(float)(int)psVar19[1]) * dVar31;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar15 = (double *)((int)pdVar15 + -4);
              *(int *)pdVar15 = SUB84(dVar32,0);
            }
            else {
              pdVar15 = pdVar15 + -1;
              *pdVar15 = in_xd2;
            }
            uVar27 = DAT_8c61d7c8;
            uVar26 = (undefined4)((ulonglong)dVar32 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44(uVar26,(float)(int)*psVar19 * fVar30);
            }
            else {
              dVar32 = (double)CONCAT44(uVar26,(float)(int)*psVar19) * dVar31;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)pdVar15 + -4) = SUB84(dVar32,0);
              *(undefined4 *)(pdVar15 + -1) = *(undefined4 *)(iVar20 + 0x18);
              *(undefined4 *)((int)pdVar15 + -0xc) = *(undefined4 *)(iVar20 + 0x14);
              dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                        *(undefined4 *)(iVar20 + 0x10));
              pdVar15 = pdVar15 + -2;
              *(undefined4 *)pdVar15 = *(undefined4 *)(iVar20 + 0x10);
            }
            else {
              pdVar15[-1] = in_xd2;
              pdVar15[-2] = *(double *)(iVar20 + 0x18);
              pdVar15[-3] = *(double *)(iVar20 + 0x14);
              in_xd2 = *(double *)(iVar20 + 0x10);
              pdVar15 = pdVar15 + -4;
              *pdVar15 = in_xd2;
            }
            *(undefined4 *)((int)pdVar15 + -4) = uVar27;
            if (in_FPSCR_SZ == '\0') {
              pdVar15 = (double *)((int)pdVar15 + 0x34);
              *(undefined4 *)pdVar15 = uVar28;
            }
            else {
              pdVar15 = pdVar15 + 6;
              *pdVar15 = dVar31;
            }
            uVar27 = (undefined4)((ulonglong)dVar32 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44(uVar27,(float)(int)psVar19[3] * fVar30);
            }
            else {
              dVar32 = (double)CONCAT44(uVar27,(float)(int)psVar19[3]) * dVar31;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar15 = (double *)((int)pdVar15 + -4);
              *(int *)pdVar15 = SUB84(dVar32,0);
            }
            else {
              pdVar15 = pdVar15 + -1;
              *pdVar15 = in_xd2;
            }
            uVar27 = DAT_8c61d7c8;
            uVar26 = (undefined4)((ulonglong)dVar32 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44(uVar26,(float)(int)psVar19[2] * fVar30);
            }
            else {
              dVar32 = (double)CONCAT44(uVar26,(float)(int)psVar19[2]) * dVar31;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)pdVar15 + -4) = SUB84(dVar32,0);
              *(undefined4 *)(pdVar15 + -1) = *(undefined4 *)(iVar13 + 0x18);
              *(undefined4 *)((int)pdVar15 + -0xc) = *(undefined4 *)(iVar13 + 0x14);
              dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                        *(undefined4 *)(iVar13 + 0x10));
              pdVar15 = pdVar15 + -2;
              *(undefined4 *)pdVar15 = *(undefined4 *)(iVar13 + 0x10);
            }
            else {
              pdVar15[-1] = in_xd2;
              pdVar15[-2] = *(double *)(iVar13 + 0x18);
              pdVar15[-3] = *(double *)(iVar13 + 0x14);
              in_xd2 = *(double *)(iVar13 + 0x10);
              pdVar15 = pdVar15 + -4;
              *pdVar15 = in_xd2;
            }
            *(undefined4 *)((int)pdVar15 + -4) = uVar27;
            if (in_FPSCR_SZ == '\0') {
              pdVar15 = (double *)((int)pdVar15 + 0x34);
              *(undefined4 *)pdVar15 = uVar28;
            }
            else {
              pdVar15 = pdVar15 + 6;
              *pdVar15 = dVar31;
            }
            uVar28 = (undefined4)((ulonglong)dVar32 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44(uVar28,(float)(int)psVar19[5] * fVar30);
            }
            else {
              dVar32 = (double)CONCAT44(uVar28,(float)(int)psVar19[5]) * dVar31;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar15 = (double *)((int)pdVar15 + -4);
              *(int *)pdVar15 = SUB84(dVar32,0);
            }
            else {
              pdVar15 = pdVar15 + -1;
              *pdVar15 = in_xd2;
            }
            uVar28 = DAT_8c61d7cc;
            uVar27 = (undefined4)((ulonglong)dVar32 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              in_dr2 = (double)CONCAT44(uVar27,(float)(int)psVar19[4] * fVar30);
            }
            else {
              in_dr2 = (double)CONCAT44(uVar27,(float)(int)psVar19[4]) * dVar31;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)pdVar15 + -4) = SUB84(in_dr2,0);
              *(undefined4 *)(pdVar15 + -1) = *(undefined4 *)(iVar22 + 0x18);
              *(undefined4 *)((int)pdVar15 + -0xc) = *(undefined4 *)(iVar22 + 0x14);
              in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),
                                        *(undefined4 *)(iVar22 + 0x10));
              pdVar15 = pdVar15 + -2;
              *(undefined4 *)pdVar15 = *(undefined4 *)(iVar22 + 0x10);
            }
            else {
              pdVar15[-1] = in_xd2;
              pdVar15[-2] = *(double *)(iVar22 + 0x18);
              pdVar15[-3] = *(double *)(iVar22 + 0x14);
              in_xd2 = *(double *)(iVar22 + 0x10);
              pdVar15 = pdVar15 + -4;
              *pdVar15 = in_xd2;
            }
            *(undefined4 *)((int)pdVar15 + -4) = uVar28;
            *DAT_8c61d7d0 = *DAT_8c61d7d0 + 3;
            unaff_r8 = (undefined4 *)((int)pdVar15 + 0x1c);
          }
        }
        else {
          *DAT_8c61d7d4 = 1;
          uVar28 = (undefined4)((ulonglong)in_dr2 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            uVar34 = (undefined2)(int)(float)(int)*psVar19;
          }
          else {
            uVar34 = (undefined2)(int)(double)CONCAT44(uVar28,(float)(int)*psVar19);
          }
          *puVar5 = uVar34;
          if (in_FPSCR_PR == '\0') {
            uVar34 = (undefined2)(int)(float)(int)psVar19[1];
          }
          else {
            uVar34 = (undefined2)(int)(double)CONCAT44(uVar28,(float)(int)psVar19[1]);
          }
          puVar5[1] = uVar34;
          if (in_FPSCR_PR == '\0') {
            uVar34 = (undefined2)(int)(float)(int)psVar19[2];
          }
          else {
            uVar34 = (undefined2)(int)(double)CONCAT44(uVar28,(float)(int)psVar19[2]);
          }
          puVar5[2] = uVar34;
          if (in_FPSCR_PR == '\0') {
            uVar34 = (undefined2)(int)(float)(int)psVar19[3];
          }
          else {
            uVar34 = (undefined2)(int)(double)CONCAT44(uVar28,(float)(int)psVar19[3]);
          }
          puVar5[3] = uVar34;
          puVar7 = DAT_8c61d7d8;
          if (in_FPSCR_PR == '\0') {
            uVar34 = (undefined2)(int)(float)(int)psVar19[4];
          }
          else {
            uVar34 = (undefined2)(int)(double)CONCAT44(uVar28,(float)(int)psVar19[4]);
          }
          *puVar6 = uVar34;
          puVar8 = PTR_FUN_8c61d7dc;
          in_dr2 = (double)CONCAT44(uVar28,(float)(int)psVar19[5]);
          if (in_FPSCR_PR == '\0') {
            uVar34 = (undefined2)(int)(float)(int)psVar19[5];
          }
          else {
            uVar34 = (undefined2)(int)in_dr2;
          }
          puVar6[1] = uVar34;
          in_dr0 = (double)(*(code *)puVar8)(puVar7);
        }
      }
      psVar19 = psVar19 + 6;
    }
  }
  else if (uVar10 == (int)DAT_8c61d4f6) {
    *DAT_8c61d7e0 = *DAT_8c61d7e0 + (uint)*(ushort *)(*(int *)((int)puVar24 + -0x14) + 2) * 4;
    *(undefined4 *)((int)puVar24 + -0x10) = 0;
    *(undefined2 **)((int)puVar24 + -0x1c) = puVar7 + 2;
    *(undefined4 *)((int)puVar24 + -0xc) = uVar28;
    *(undefined4 *)((int)puVar24 + -0x18) = DAT_8c61d7e8;
    while (uVar28 = SUB84(param_1,0),
          *(int *)((int)puVar24 + -0x10) <
          (int)(uint)*(ushort *)(*(int *)((int)puVar24 + -0x14) + 2)) {
      uVar2 = *puVar16;
      puVar14 = puVar16 + 1;
      puVar18 = puVar16 + 2;
      iVar13 = *DAT_8c61d7ec;
      puVar17 = puVar16 + 3;
      puVar16 = puVar16 + 4;
      iVar11 = (uint)uVar2 * 0x20 + iVar13;
      iVar22 = (uint)*puVar14 * 0x20 + iVar13;
      iVar20 = (uint)*puVar18 * 0x20 + iVar13;
      iVar13 = (uint)*puVar17 * 0x20 + iVar13;
      if ((*(uint *)(iVar11 + 0xc) & *(uint *)(iVar22 + 0xc) & *(uint *)(iVar20 + 0xc) &
           *(uint *)(iVar13 + 0xc) & 0x3f) == 0) {
        if (((*(uint *)(iVar11 + 0xc) | *(uint *)(iVar22 + 0xc) | *(uint *)(iVar20 + 0xc) |
             *(uint *)(iVar13 + 0xc)) & 1) == 0) {
          if (in_FPSCR_SZ == '\0') {
            uVar28 = *(undefined4 *)(iVar22 + 0x10);
            dVar32 = (double)CONCAT44(uVar28,*(undefined4 *)(iVar11 + 0x10));
          }
          else {
            dVar32 = *(double *)(iVar22 + 0x10);
            in_xd2 = *(double *)(iVar11 + 0x10);
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
                                      *(undefined4 *)(iVar20 + 0x14));
            dVar29 = (double)CONCAT44(*(undefined4 *)(iVar22 + 0x14),uVar28);
          }
          else {
            in_xd2 = *(double *)(iVar20 + 0x14);
            dVar29 = *(double *)(iVar22 + 0x14);
          }
          fVar30 = (float)((ulonglong)dVar29 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),SUB84(dVar32,0) - fVar30);
          }
          else {
            dVar32 = dVar32 - dVar29;
          }
          if (in_FPSCR_SZ == '\0') {
            in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar30);
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
                                      *(undefined4 *)(iVar11 + 0x14));
          }
          else {
            in_xd2 = *(double *)(iVar11 + 0x14);
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                      SUB84(in_dr0,0) - SUB84(dVar32,0));
          }
          else {
            dVar25 = in_dr0 - dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                      *(undefined4 *)(iVar20 + 0x10));
          }
          else {
            in_xd2 = *(double *)(iVar20 + 0x10);
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                      SUB84(dVar32,0) - SUB84(dVar29,0));
          }
          else {
            dVar32 = dVar32 - dVar29;
          }
          uVar10 = (uint)((ulonglong)dVar32 >> 0x20);
          param_1 = dVar32;
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44(uVar10,SUB84(dVar29,0));
          }
          if (in_FPSCR_PR == '\0') {
            in_dr0 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                      SUB84(dVar25,0) * SUB84(dVar32,0));
          }
          else {
            in_dr0 = dVar25 * dVar32;
          }
          in_dr2 = (double)((ulonglong)uVar10 << 0x20);
          if (in_FPSCR_PR == '\0') {
            fVar30 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
            param_1 = (double)CONCAT44(fVar30,SUB84(param_1,0));
            bVar33 = 0.0 < fVar30;
          }
          else {
            param_1 = param_1 - in_dr0;
            bVar33 = in_dr2 < param_1;
          }
          if (!bVar33) {
            uVar28 = (undefined4)((ulonglong)dVar31 >> 0x20);
            if (in_FPSCR_SZ == '\0') {
              pdVar15 = (double *)(unaff_r8 + 6);
              *(undefined4 *)pdVar15 = uVar28;
            }
            else {
              pdVar15 = (double *)(unaff_r8 + 5);
              *pdVar15 = dVar31;
            }
            fVar30 = SUB84(dVar31,0);
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44(uVar10,(float)(int)psVar19[1] * fVar30);
            }
            else {
              dVar32 = (double)CONCAT44(uVar10,(float)(int)psVar19[1]) * dVar31;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar15 = (double *)((int)pdVar15 + -4);
              *(int *)pdVar15 = SUB84(dVar32,0);
            }
            else {
              pdVar15 = pdVar15 + -1;
              *pdVar15 = in_xd2;
            }
            uVar27 = DAT_8c61d7c8;
            uVar26 = (undefined4)((ulonglong)dVar32 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44(uVar26,(float)(int)*psVar19 * fVar30);
            }
            else {
              dVar32 = (double)CONCAT44(uVar26,(float)(int)*psVar19) * dVar31;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)pdVar15 + -4) = SUB84(dVar32,0);
              *(undefined4 *)(pdVar15 + -1) = *(undefined4 *)(iVar11 + 0x18);
              *(undefined4 *)((int)pdVar15 + -0xc) = *(undefined4 *)(iVar11 + 0x14);
              dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                        *(undefined4 *)(iVar11 + 0x10));
              pdVar15 = pdVar15 + -2;
              *(undefined4 *)pdVar15 = *(undefined4 *)(iVar11 + 0x10);
            }
            else {
              pdVar15[-1] = in_xd2;
              pdVar15[-2] = *(double *)(iVar11 + 0x18);
              pdVar15[-3] = *(double *)(iVar11 + 0x14);
              in_xd2 = *(double *)(iVar11 + 0x10);
              pdVar15 = pdVar15 + -4;
              *pdVar15 = in_xd2;
            }
            *(undefined4 *)((int)pdVar15 + -4) = uVar27;
            if (in_FPSCR_SZ == '\0') {
              pdVar15 = (double *)((int)pdVar15 + 0x34);
              *(undefined4 *)pdVar15 = uVar28;
            }
            else {
              pdVar15 = pdVar15 + 6;
              *pdVar15 = dVar31;
            }
            uVar27 = (undefined4)((ulonglong)dVar32 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44(uVar27,(float)(int)psVar19[3] * fVar30);
            }
            else {
              dVar32 = (double)CONCAT44(uVar27,(float)(int)psVar19[3]) * dVar31;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar15 = (double *)((int)pdVar15 + -4);
              *(int *)pdVar15 = SUB84(dVar32,0);
            }
            else {
              pdVar15 = pdVar15 + -1;
              *pdVar15 = in_xd2;
            }
            uVar27 = DAT_8c61d7c8;
            uVar26 = (undefined4)((ulonglong)dVar32 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44(uVar26,(float)(int)psVar19[2] * fVar30);
            }
            else {
              dVar32 = (double)CONCAT44(uVar26,(float)(int)psVar19[2]) * dVar31;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)pdVar15 + -4) = SUB84(dVar32,0);
              *(undefined4 *)(pdVar15 + -1) = *(undefined4 *)(iVar22 + 0x18);
              *(undefined4 *)((int)pdVar15 + -0xc) = *(undefined4 *)(iVar22 + 0x14);
              dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                        *(undefined4 *)(iVar22 + 0x10));
              pdVar15 = pdVar15 + -2;
              *(undefined4 *)pdVar15 = *(undefined4 *)(iVar22 + 0x10);
            }
            else {
              pdVar15[-1] = in_xd2;
              pdVar15[-2] = *(double *)(iVar22 + 0x18);
              pdVar15[-3] = *(double *)(iVar22 + 0x14);
              in_xd2 = *(double *)(iVar22 + 0x10);
              pdVar15 = pdVar15 + -4;
              *pdVar15 = in_xd2;
            }
            *(undefined4 *)((int)pdVar15 + -4) = uVar27;
            if (in_FPSCR_SZ == '\0') {
              pdVar15 = (double *)((int)pdVar15 + 0x34);
              *(undefined4 *)pdVar15 = uVar28;
            }
            else {
              pdVar15 = pdVar15 + 6;
              *pdVar15 = dVar31;
            }
            uVar27 = (undefined4)((ulonglong)dVar32 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44(uVar27,(float)(int)psVar19[5] * fVar30);
            }
            else {
              dVar32 = (double)CONCAT44(uVar27,(float)(int)psVar19[5]) * dVar31;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar15 = (double *)((int)pdVar15 + -4);
              *(int *)pdVar15 = SUB84(dVar32,0);
            }
            else {
              pdVar15 = pdVar15 + -1;
              *pdVar15 = in_xd2;
            }
            uVar27 = DAT_8c61d7c8;
            uVar26 = (undefined4)((ulonglong)dVar32 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44(uVar26,(float)(int)psVar19[4] * fVar30);
            }
            else {
              dVar32 = (double)CONCAT44(uVar26,(float)(int)psVar19[4]) * dVar31;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)pdVar15 + -4) = SUB84(dVar32,0);
              *(undefined4 *)(pdVar15 + -1) = *(undefined4 *)(iVar20 + 0x18);
              *(undefined4 *)((int)pdVar15 + -0xc) = *(undefined4 *)(iVar20 + 0x14);
              dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                        *(undefined4 *)(iVar20 + 0x10));
              pdVar15 = pdVar15 + -2;
              *(undefined4 *)pdVar15 = *(undefined4 *)(iVar20 + 0x10);
            }
            else {
              pdVar15[-1] = in_xd2;
              pdVar15[-2] = *(double *)(iVar20 + 0x18);
              pdVar15[-3] = *(double *)(iVar20 + 0x14);
              in_xd2 = *(double *)(iVar20 + 0x10);
              pdVar15 = pdVar15 + -4;
              *pdVar15 = in_xd2;
            }
            *(undefined4 *)((int)pdVar15 + -4) = uVar27;
            if (in_FPSCR_SZ == '\0') {
              pdVar15 = (double *)((int)pdVar15 + 0x34);
              *(undefined4 *)pdVar15 = uVar28;
            }
            else {
              pdVar15 = pdVar15 + 6;
              *pdVar15 = dVar31;
            }
            uVar28 = (undefined4)((ulonglong)dVar32 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44(uVar28,(float)(int)psVar19[7] * fVar30);
            }
            else {
              dVar32 = (double)CONCAT44(uVar28,(float)(int)psVar19[7]) * dVar31;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar15 = (double *)((int)pdVar15 + -4);
              *(int *)pdVar15 = SUB84(dVar32,0);
            }
            else {
              pdVar15 = pdVar15 + -1;
              *pdVar15 = in_xd2;
            }
            uVar28 = DAT_8c61d7cc;
            uVar27 = (undefined4)((ulonglong)dVar32 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              in_dr2 = (double)CONCAT44(uVar27,(float)(int)psVar19[6] * fVar30);
            }
            else {
              in_dr2 = (double)CONCAT44(uVar27,(float)(int)psVar19[6]) * dVar31;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)pdVar15 + -4) = SUB84(in_dr2,0);
              *(undefined4 *)(pdVar15 + -1) = *(undefined4 *)(iVar13 + 0x18);
              *(undefined4 *)((int)pdVar15 + -0xc) = *(undefined4 *)(iVar13 + 0x14);
              in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),
                                        *(undefined4 *)(iVar13 + 0x10));
              pdVar15 = pdVar15 + -2;
              *(undefined4 *)pdVar15 = *(undefined4 *)(iVar13 + 0x10);
            }
            else {
              pdVar15[-1] = in_xd2;
              pdVar15[-2] = *(double *)(iVar13 + 0x18);
              pdVar15[-3] = *(double *)(iVar13 + 0x14);
              in_xd2 = *(double *)(iVar13 + 0x10);
              pdVar15 = pdVar15 + -4;
              *pdVar15 = in_xd2;
            }
            *(undefined4 *)((int)pdVar15 + -4) = uVar28;
            *DAT_8c61d7d0 = *DAT_8c61d7d0 + 4;
            unaff_r8 = (undefined4 *)((int)pdVar15 + 0x1c);
          }
        }
        else {
          *DAT_8c61da2c = 1;
          uVar28 = (undefined4)((ulonglong)in_dr2 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            uVar34 = (undefined2)(int)(float)(int)*psVar19;
          }
          else {
            uVar34 = (undefined2)(int)(double)CONCAT44(uVar28,(float)(int)*psVar19);
          }
          *puVar7 = uVar34;
          if (in_FPSCR_PR == '\0') {
            uVar34 = (undefined2)(int)(float)(int)psVar19[1];
          }
          else {
            uVar34 = (undefined2)(int)(double)CONCAT44(uVar28,(float)(int)psVar19[1]);
          }
          puVar7[1] = uVar34;
          if (in_FPSCR_PR == '\0') {
            uVar34 = (undefined2)(int)(float)(int)psVar19[2];
          }
          else {
            uVar34 = (undefined2)(int)(double)CONCAT44(uVar28,(float)(int)psVar19[2]);
          }
          **(undefined2 **)((int)puVar24 + -0x1c) = uVar34;
          if (in_FPSCR_PR == '\0') {
            uVar34 = (undefined2)(int)(float)(int)psVar19[3];
          }
          else {
            uVar34 = (undefined2)(int)(double)CONCAT44(uVar28,(float)(int)psVar19[3]);
          }
          *(undefined2 *)(*(int *)((int)puVar24 + -0x1c) + 2) = uVar34;
          if (in_FPSCR_PR == '\0') {
            uVar34 = (undefined2)(int)(float)(int)psVar19[4];
          }
          else {
            uVar34 = (undefined2)(int)(double)CONCAT44(uVar28,(float)(int)psVar19[4]);
          }
          **(undefined2 **)((int)puVar24 + -0xc) = uVar34;
          if (in_FPSCR_PR == '\0') {
            uVar34 = (undefined2)(int)(float)(int)psVar19[5];
          }
          else {
            uVar34 = (undefined2)(int)(double)CONCAT44(uVar28,(float)(int)psVar19[5]);
          }
          *(undefined2 *)(*(int *)((int)puVar24 + -0xc) + 2) = uVar34;
          if (in_FPSCR_PR == '\0') {
            uVar34 = (undefined2)(int)(float)(int)psVar19[6];
          }
          else {
            uVar34 = (undefined2)(int)(double)CONCAT44(uVar28,(float)(int)psVar19[6]);
          }
          **(undefined2 **)((int)puVar24 + -0x18) = uVar34;
          in_dr2 = (double)CONCAT44(uVar28,(float)(int)psVar19[7]);
          if (in_FPSCR_PR == '\0') {
            uVar34 = (undefined2)(int)(float)(int)psVar19[7];
          }
          else {
            uVar34 = (undefined2)(int)in_dr2;
          }
          *(undefined2 *)(*(int *)((int)puVar24 + -0x18) + 2) = uVar34;
          if ((*(uint *)(iVar11 + 0xc) & *(uint *)(iVar22 + 0xc) & *(uint *)(iVar20 + 0xc) & 1) == 0
             ) {
            in_dr0 = (double)(*(code *)PTR_FUN_8c61da30)(iVar22,iVar20,puVar7);
          }
          if ((*(uint *)(iVar22 + 0xc) & *(uint *)(iVar20 + 0xc) & *(uint *)(iVar13 + 0xc) & 1) == 0
             ) {
            in_dr0 = (double)(*(code *)PTR_FUN_8c61da30)
                                       (iVar22,iVar20,iVar13,*(undefined4 *)((int)puVar24 + -0x1c));
          }
        }
      }
      psVar19 = psVar19 + 8;
      *(int *)((int)puVar24 + -0x10) = *(int *)((int)puVar24 + -0x10) + 1;
    }
  }
  else if (uVar10 == DAT_8c61d508) {
    *(undefined4 *)((int)puVar24 + -0xc) = 0;
    *(int *)((int)puVar24 + -0x18) = DAT_8c61da34 + 8;
    *(int *)((int)puVar24 + -0x10) = DAT_8c61da34;
    *(int *)((int)puVar24 + -0x1c) = DAT_8c61da34 + 4;
    while (uVar28 = SUB84(param_1,0),
          *(int *)((int)puVar24 + -0xc) < (int)(uint)*(ushort *)(*(int *)((int)puVar24 + -0x14) + 2)
          ) {
      uVar12 = 0;
      puVar18 = puVar16 + 1;
      iVar11 = 0;
      uVar21 = (uint)*puVar16;
      *DAT_8c61da38 = *DAT_8c61da38 + uVar21;
      piVar9 = DAT_8c61db40;
      uVar10 = 0xffffffff;
      puVar14 = puVar18;
      if (uVar21 != 0) {
        do {
          uVar2 = *puVar14;
          puVar14 = puVar14 + 1;
          iVar11 = iVar11 + 1;
          iVar13 = (uint)uVar2 * 0x20 + *DAT_8c61da3c;
          *(int *)((int)puVar24 + -8) = iVar13;
          uVar23 = *(uint *)(iVar13 + 0xc);
          uVar10 = uVar10 & uVar23;
          uVar12 = uVar12 | uVar23;
        } while (iVar11 < (int)uVar21);
      }
      if ((uVar10 & 0x3f) == 0) {
        if ((uVar12 & 1) == 0) {
          iVar13 = *DAT_8c61da3c;
          iVar11 = (uint)puVar16[2] * 0x20 + iVar13;
          if (in_FPSCR_SZ == '\0') {
            uVar28 = *(undefined4 *)(iVar11 + 0x10);
          }
          else {
            in_xd4 = *(double *)(iVar11 + 0x10);
          }
          iVar20 = (uint)*puVar18 * 0x20 + iVar13;
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44(uVar28,*(undefined4 *)(iVar20 + 0x10));
          }
          else {
            in_xd2 = *(double *)(iVar20 + 0x10);
            dVar32 = in_xd4;
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) - SUB84(dVar32,0),
                                      SUB84(dVar32,0));
          }
          else {
            dVar32 = dVar32 - dVar32;
          }
          iVar13 = (uint)puVar16[3] * 0x20 + iVar13;
          if (in_FPSCR_SZ == '\0') {
            fVar30 = *(float *)(iVar11 + 0x14);
            dVar29 = (double)CONCAT44(fVar30,uVar28);
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                      *(undefined4 *)(iVar13 + 0x14));
            in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),fVar30);
          }
          else {
            dVar29 = *(double *)(iVar11 + 0x14);
            in_xd2 = *(double *)(iVar13 + 0x14);
            fVar30 = (float)((ulonglong)dVar29 >> 0x20);
          }
          if (in_FPSCR_PR == '\0') {
            fVar30 = SUB84(dVar32,0) - fVar30;
            param_1 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) * fVar30,fVar30);
          }
          else {
            param_1 = (dVar32 - dVar29) * (dVar32 - dVar29);
          }
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),
                                       *(undefined4 *)(iVar20 + 0x14));
          }
          else {
            in_xd2 = *(double *)(iVar20 + 0x14);
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),
                                      SUB84(in_dr0,0) - SUB84(param_1,0));
          }
          else {
            dVar32 = in_dr0 - param_1;
          }
          uVar28 = (undefined4)((ulonglong)param_1 >> 0x20);
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44(uVar28,SUB84(dVar29,0));
            dVar29 = (double)CONCAT44(uVar28,*(undefined4 *)(iVar13 + 0x10));
          }
          else {
            in_xd2 = *(double *)(iVar13 + 0x10);
            dVar29 = param_1;
          }
          if (in_FPSCR_PR == '\0') {
            fVar30 = SUB84(dVar29,0) - SUB84(param_1,0);
            dVar29 = (double)CONCAT44(uVar28,fVar30);
            in_dr0 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),SUB84(dVar32,0) * fVar30);
          }
          else {
            dVar29 = dVar29 - param_1;
            in_dr0 = dVar32 * dVar29;
          }
          in_dr2 = (double)((ulonglong)dVar29 & 0xffffffff00000000);
          if (in_FPSCR_PR == '\0') {
            fVar30 = (float)((ulonglong)param_1 >> 0x20) - SUB84(in_dr0,0);
            param_1 = (double)CONCAT44(fVar30,SUB84(param_1,0));
            bVar33 = 0.0 < fVar30;
          }
          else {
            param_1 = param_1 - in_dr0;
            bVar33 = in_dr2 < param_1;
          }
          if (bVar33) {
            puVar16 = puVar18 + uVar21;
            psVar19 = psVar19 + uVar21 * 2;
          }
          else {
            *DAT_8c61da40 = *DAT_8c61da40 + uVar21;
            iVar11 = 0;
            uVar28 = (undefined4)((ulonglong)dVar31 >> 0x20);
            fVar30 = SUB84(dVar31,0);
            if (0 < (int)(uVar21 - 1)) {
              do {
                uVar2 = *puVar18;
                puVar18 = puVar18 + 1;
                iVar13 = *DAT_8c61da3c;
                if (in_FPSCR_SZ == '\0') {
                  pdVar15 = (double *)(unaff_r8 + 6);
                  *(undefined4 *)pdVar15 = uVar28;
                }
                else {
                  pdVar15 = (double *)(unaff_r8 + 5);
                  *pdVar15 = dVar31;
                }
                iVar13 = (uint)uVar2 * 0x20 + iVar13;
                uVar27 = (undefined4)((ulonglong)in_dr2 >> 0x20);
                if (in_FPSCR_PR == '\0') {
                  dVar32 = (double)CONCAT44(uVar27,(float)(int)psVar19[1] * fVar30);
                }
                else {
                  dVar32 = (double)CONCAT44(uVar27,(float)(int)psVar19[1]) * dVar31;
                }
                if (in_FPSCR_SZ == '\0') {
                  pdVar15 = (double *)((int)pdVar15 + -4);
                  *(int *)pdVar15 = SUB84(dVar32,0);
                }
                else {
                  pdVar15 = pdVar15 + -1;
                  *pdVar15 = in_xd2;
                }
                uVar27 = DAT_8c61da44;
                sVar1 = *psVar19;
                psVar19 = psVar19 + 2;
                uVar26 = (undefined4)((ulonglong)dVar32 >> 0x20);
                if (in_FPSCR_PR == '\0') {
                  in_dr2 = (double)CONCAT44(uVar26,(float)(int)sVar1 * fVar30);
                }
                else {
                  in_dr2 = (double)CONCAT44(uVar26,(float)(int)sVar1) * dVar31;
                }
                if (in_FPSCR_SZ == '\0') {
                  *(int *)((int)pdVar15 + -4) = SUB84(in_dr2,0);
                  *(undefined4 *)(pdVar15 + -1) = *(undefined4 *)(iVar13 + 0x18);
                  *(undefined4 *)((int)pdVar15 + -0xc) = *(undefined4 *)(iVar13 + 0x14);
                  in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),
                                            *(undefined4 *)(iVar13 + 0x10));
                  pdVar15 = pdVar15 + -2;
                  *(undefined4 *)pdVar15 = *(undefined4 *)(iVar13 + 0x10);
                }
                else {
                  pdVar15[-1] = in_xd2;
                  pdVar15[-2] = *(double *)(iVar13 + 0x18);
                  pdVar15[-3] = *(double *)(iVar13 + 0x14);
                  in_xd2 = *(double *)(iVar13 + 0x10);
                  pdVar15 = pdVar15 + -4;
                  *pdVar15 = in_xd2;
                }
                *(undefined4 *)((int)pdVar15 + -4) = uVar27;
                iVar11 = iVar11 + 1;
                unaff_r8 = (undefined4 *)((int)pdVar15 + 0x1c);
              } while (iVar11 < (int)(uVar21 - 1));
            }
            uVar2 = *puVar18;
            puVar16 = puVar18 + 1;
            iVar11 = *DAT_8c61da3c;
            if (in_FPSCR_SZ == '\0') {
              pdVar15 = (double *)(unaff_r8 + 6);
              *(undefined4 *)pdVar15 = uVar28;
            }
            else {
              pdVar15 = (double *)(unaff_r8 + 5);
              *pdVar15 = dVar31;
            }
            iVar11 = (uint)uVar2 * 0x20 + iVar11;
            uVar28 = (undefined4)((ulonglong)in_dr2 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44(uVar28,(float)(int)psVar19[1] * fVar30);
            }
            else {
              dVar32 = (double)CONCAT44(uVar28,(float)(int)psVar19[1]) * dVar31;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar15 = (double *)((int)pdVar15 + -4);
              *(int *)pdVar15 = SUB84(dVar32,0);
            }
            else {
              pdVar15 = pdVar15 + -1;
              *pdVar15 = in_xd2;
            }
            uVar28 = DAT_8c61da48;
            sVar1 = *psVar19;
            psVar19 = psVar19 + 2;
            uVar27 = (undefined4)((ulonglong)dVar32 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              in_dr2 = (double)CONCAT44(uVar27,(float)(int)sVar1 * fVar30);
            }
            else {
              in_dr2 = (double)CONCAT44(uVar27,(float)(int)sVar1) * dVar31;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)pdVar15 + -4) = SUB84(in_dr2,0);
              *(undefined4 *)(pdVar15 + -1) = *(undefined4 *)(iVar11 + 0x18);
              *(undefined4 *)((int)pdVar15 + -0xc) = *(undefined4 *)(iVar11 + 0x14);
              in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),
                                        *(undefined4 *)(iVar11 + 0x10));
              pdVar15 = pdVar15 + -2;
              *(undefined4 *)pdVar15 = *(undefined4 *)(iVar11 + 0x10);
            }
            else {
              pdVar15[-1] = in_xd2;
              pdVar15[-2] = *(double *)(iVar11 + 0x18);
              pdVar15[-3] = *(double *)(iVar11 + 0x14);
              in_xd2 = *(double *)(iVar11 + 0x10);
              pdVar15 = pdVar15 + -4;
              *pdVar15 = in_xd2;
            }
            *(undefined4 *)((int)pdVar15 + -4) = uVar28;
            unaff_r8 = (undefined4 *)((int)pdVar15 + 0x1c);
          }
        }
        else {
          *DAT_8c61db3c = 0;
          uVar2 = *puVar18;
          iVar11 = *piVar9;
          uVar3 = puVar16[2];
          puVar16 = puVar16 + 3;
          uVar28 = (undefined4)((ulonglong)in_dr2 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            uVar34 = (undefined2)(int)(float)(int)*psVar19;
          }
          else {
            uVar34 = (undefined2)(int)(double)CONCAT44(uVar28,(float)(int)*psVar19);
          }
          **(undefined2 **)((int)puVar24 + -0x10) = uVar34;
          if (in_FPSCR_PR == '\0') {
            uVar34 = (undefined2)(int)(float)(int)psVar19[1];
          }
          else {
            uVar34 = (undefined2)(int)(double)CONCAT44(uVar28,(float)(int)psVar19[1]);
          }
          *(undefined2 *)(*(int *)((int)puVar24 + -0x10) + 2) = uVar34;
          if (in_FPSCR_PR == '\0') {
            uVar34 = (undefined2)(int)(float)(int)psVar19[2];
          }
          else {
            uVar34 = (undefined2)(int)(double)CONCAT44(uVar28,(float)(int)psVar19[2]);
          }
          **(undefined2 **)((int)puVar24 + -0x1c) = uVar34;
          psVar4 = psVar19 + 3;
          psVar19 = psVar19 + 4;
          in_dr2 = (double)CONCAT44(uVar28,(float)(int)*psVar4);
          if (in_FPSCR_PR == '\0') {
            uVar34 = (undefined2)(int)(float)(int)*psVar4;
          }
          else {
            uVar34 = (undefined2)(int)in_dr2;
          }
          *(undefined2 *)(*(int *)((int)puVar24 + -0x1c) + 2) = uVar34;
          iVar20 = 0;
          iVar13 = (uint)uVar2 * 0x20 + iVar11;
          iVar11 = (uint)uVar3 * 0x20 + iVar11;
          if (0 < (int)(uVar21 - 2)) {
            do {
              uVar2 = *puVar16;
              puVar16 = puVar16 + 1;
              iVar22 = (uint)uVar2 * 0x20 + *DAT_8c61db40;
              uVar28 = (undefined4)((ulonglong)in_dr2 >> 0x20);
              if (in_FPSCR_PR == '\0') {
                uVar34 = (undefined2)(int)(float)(int)*psVar19;
              }
              else {
                uVar34 = (undefined2)(int)(double)CONCAT44(uVar28,(float)(int)*psVar19);
              }
              **(undefined2 **)((int)puVar24 + -0x18) = uVar34;
              in_dr2 = (double)CONCAT44(uVar28,(float)(int)psVar19[1]);
              if (in_FPSCR_PR == '\0') {
                uVar34 = (undefined2)(int)(float)(int)psVar19[1];
              }
              else {
                uVar34 = (undefined2)(int)in_dr2;
              }
              *(undefined2 *)(*(int *)((int)puVar24 + -0x18) + 2) = uVar34;
              psVar19 = psVar19 + 2;
              if ((*(uint *)(iVar13 + 0xc) & *(uint *)(iVar11 + 0xc) & *(uint *)(iVar22 + 0xc) & 1)
                  == 0) {
                in_dr0 = (double)(*(code *)PTR_FUN_8c61db48)(iVar11,iVar22,DAT_8c61db44);
              }
              iVar20 = iVar20 + 1;
              **(undefined4 **)((int)puVar24 + -0x10) = **(undefined4 **)((int)puVar24 + -0x1c);
              **(undefined4 **)((int)puVar24 + -0x1c) = **(undefined4 **)((int)puVar24 + -0x18);
              iVar13 = iVar11;
              iVar11 = iVar22;
            } while (iVar20 < (int)(uVar21 - 2));
          }
        }
      }
      else {
        psVar19 = psVar19 + uVar21 * 2;
        puVar16 = puVar18 + uVar21;
      }
      *(int *)((int)puVar24 + -0xc) = *(int *)((int)puVar24 + -0xc) + 1;
    }
  }
  else if (uVar10 == DAT_8c61d504) {
    *DAT_8c61db5c = *(undefined4 *)((int)puVar24 + -0x14);
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)puVar24 + -0x20) = (int)((ulonglong)in_dr0 >> 0x20);
      *(int *)((int)puVar24 + -0x24) = SUB84(in_dr0,0);
    }
    else {
      *(double *)((int)puVar24 + -0x24) = in_dr0;
      *(undefined8 *)((int)puVar24 + -0x2c) = in_xd0;
    }
    (*(code *)PTR_FUN_8c61dc08)(*DAT_8c61dc04);
  }
  return;
}

