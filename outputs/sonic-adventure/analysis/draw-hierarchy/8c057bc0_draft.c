/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c057dd4) */
/* WARNING: Removing unreachable block (ram,0x8c057d1c) */
/* WARNING: Removing unreachable block (ram,0x8c058028) */
/* WARNING: Removing unreachable block (ram,0x8c057f40) */
/* WARNING: Removing unreachable block (ram,0x8c058036) */
/* WARNING: Removing unreachable block (ram,0x8c057d36) */
/* WARNING: Removing unreachable block (ram,0x8c057d02) */
/* WARNING: Removing unreachable block (ram,0x8c057de2) */
/* WARNING: Removing unreachable block (ram,0x8c058042) */
/* WARNING: Removing unreachable block (ram,0x8c057dee) */
/* WARNING: Removing unreachable block (ram,0x8c057f5a) */
/* WARNING: Removing unreachable block (ram,0x8c057f26) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint FUN_8c057bc0(double param_1,double param_2,uint *param_3,int *param_4)

{
  undefined *puVar1;
  undefined *puVar2;
  undefined4 uVar3;
  uint uVar4;
  uint uVar5;
  uint *puVar6;
  double *pdVar7;
  uint uVar8;
  uint *puVar9;
  undefined4 *puVar10;
  double *pdVar11;
  int iVar12;
  int iVar13;
  int iVar14;
  undefined8 *puVar15;
  double dVar16;
  float fVar17;
  float fVar18;
  float fVar19;
  double in_dr2;
  undefined4 uVar21;
  double dVar20;
  double dVar22;
  undefined8 in_dr14;
  double dVar23;
  undefined8 in_xd0;
  double in_xd2;
  double in_xd4;
  undefined8 in_xd14;
  undefined8 uVar24;
  bool bVar25;
  undefined4 in_PR;
  int iVar26;
  int iVar27;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 local_2c [8];
  undefined8 uStack_24;
  
  puVar9 = DAT_8c057ca4;
  puVar6 = DAT_8c057ca0;
  if (in_FPSCR_SZ == '\0') {
    uStack_24 = CONCAT44((int)in_dr14,(undefined4)uStack_24);
    puVar15 = &uStack_24;
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_24._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
  }
  else {
    puVar15 = (undefined8 *)local_2c;
    uStack_24 = in_xd14;
  }
  *(undefined4 *)((int)puVar15 + -4) = in_PR;
  uVar5 = *DAT_8c057ca4;
  *DAT_8c057ca4 = uVar5 + 1;
  if (*puVar6 < uVar5 + 1) {
    *puVar9 = *puVar6;
    uVar5 = 0;
  }
  else {
    uVar5 = param_4[2];
    uVar8 = 1;
    iVar13 = 0;
    do {
      if (uVar5 == 0) break;
      uVar4 = ~uVar8;
      iVar13 = iVar13 + 1;
      uVar8 = uVar8 << 1;
      uVar5 = uVar5 & uVar4;
    } while (iVar13 < 5);
    dVar16 = (double)(*(code *)PTR_FUN_8c057ca8)(0);
    puVar2 = PTR_FUN_8c057cb4;
    puVar1 = PTR_FUN_8c057cb0;
    if (in_FPSCR_SZ == '\0') {
      dVar23 = (double)CONCAT44(_DAT_8c057cac,(int)in_dr14);
    }
    else {
      dVar23 = (double)CONCAT44(PTR_FUN_8c057cb0,_DAT_8c057cac);
    }
    if (iVar13 == 2) {
      iVar13 = *param_4;
      for (iVar14 = 0; iVar14 < 2; iVar14 = iVar14 + 1) {
        iVar12 = iVar14 * 4;
        if (*(int *)(iVar13 + iVar12) == 0) {
          if (iVar14 == 0) {
            dVar16 = (double)(*(code *)PTR_FUN_8c057cb8)(0,param_3 + 2);
          }
          else if ((iVar14 == 1) && ((*param_3 & 2) == 0)) {
            *(uint **)((int)puVar15 + -0x1c) = param_3 + 5;
            dVar16 = (double)(*(code *)puVar1)(0,**(undefined4 **)((int)puVar15 + -0x1c),
                                               (*(undefined4 **)((int)puVar15 + -0x1c))[1],
                                               param_3[7]);
          }
        }
        else {
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44(param_4[4],SUB84(param_1,0));
          }
          else {
            param_1 = *(double *)(param_4 + 4);
          }
          fVar17 = (float)((ulonglong)param_1 >> 0x20);
          fVar18 = (float)((ulonglong)dVar23 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            bVar25 = fVar18 < fVar17;
          }
          else {
            bVar25 = dVar23 < param_1;
          }
          if (bVar25) {
            if (in_FPSCR_SZ == '\0') {
              in_dr2 = (double)CONCAT44(fVar17,DAT_8c057cbc);
            }
            else {
              in_xd2 = (double)CONCAT44(uRam8c057cc0,DAT_8c057cbc);
              in_dr2 = param_1;
            }
            if (in_FPSCR_PR == '\0') {
              in_dr2 = (double)CONCAT44((float)((ulonglong)in_dr2 >> 0x20) + SUB84(in_dr2,0),
                                        SUB84(in_dr2,0));
            }
            else {
              in_dr2 = in_dr2 + in_dr2;
            }
            dVar22 = in_dr2;
            if (in_FPSCR_SZ == '\0') {
              uVar3 = (undefined4)((ulonglong)in_dr2 >> 0x20);
              in_dr2 = (double)CONCAT44(uVar3,uVar3);
              dVar22 = in_xd2;
            }
          }
          else {
            dVar22 = param_1;
            if (in_FPSCR_SZ == '\0') {
              in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),fVar17);
              dVar22 = in_xd2;
            }
          }
          in_xd2 = dVar22;
          if (in_FPSCR_PR == '\0') {
            uVar5 = (uint)SUB84(in_dr2,0);
          }
          else {
            uVar5 = (uint)in_dr2;
          }
          puVar6 = *(uint **)(iVar13 + iVar12);
          puVar9 = puVar6 + 4;
          if (*puVar6 < uVar5) {
            uVar8 = *puVar9;
            while (uVar8 <= uVar5) {
              puVar6 = puVar6 + 4;
              puVar9 = puVar9 + 4;
              if (uVar5 <= *puVar6) break;
              uVar8 = *puVar9;
            }
          }
          uVar3 = (undefined4)((ulonglong)in_dr2 >> 0x20);
          if (*puVar6 == param_4[3] - 1U) {
            puVar9 = *(uint **)(iVar13 + iVar12);
            fVar17 = (float)(int)*puVar6;
            in_dr2 = (double)CONCAT44(uVar3,fVar17);
            if ((int)*puVar6 < 0) {
              if (in_FPSCR_PR == '\0') {
                in_dr2 = (double)CONCAT44(DAT_8c057e60,fVar17 + DAT_8c057e60);
              }
              else {
                in_dr2 = (double)CONCAT44(DAT_8c057e60,fVar17) +
                         (double)CONCAT44(DAT_8c057e60,fVar17);
              }
            }
            if (in_FPSCR_SZ == '\0') {
              dVar23 = (double)CONCAT44(fVar18,param_4[4]);
            }
            else {
              in_xd14 = *(undefined8 *)(param_4 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),
                                        SUB84(dVar23,0) - SUB84(in_dr2,0));
            }
            else {
              dVar23 = dVar23 - in_dr2;
            }
          }
          else {
            uVar5 = *puVar6;
            fVar17 = (float)(int)uVar5;
            dVar22 = (double)CONCAT44(uVar3,fVar17);
            if ((int)uVar5 < 0) {
              if (in_FPSCR_PR == '\0') {
                dVar22 = (double)CONCAT44(DAT_8c057e60,fVar17 + DAT_8c057e60);
              }
              else {
                dVar22 = (double)CONCAT44(DAT_8c057e60,fVar17) +
                         (double)CONCAT44(DAT_8c057e60,fVar17);
              }
            }
            if (in_FPSCR_SZ == '\0') {
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),param_4[4]);
            }
            else {
              in_xd0 = *(undefined8 *)(param_4 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),
                                        SUB84(dVar16,0) - SUB84(dVar22,0));
            }
            else {
              dVar16 = dVar16 - dVar22;
            }
            fVar17 = (float)(int)(*puVar9 - uVar5);
            in_dr2 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),fVar17);
            if ((int)(*puVar9 - uVar5) < 0) {
              if (in_FPSCR_PR == '\0') {
                in_dr2 = (double)CONCAT44(DAT_8c057e60,fVar17 + DAT_8c057e60);
              }
              else {
                in_dr2 = (double)CONCAT44(DAT_8c057e60,fVar17) +
                         (double)CONCAT44(DAT_8c057e60,fVar17);
              }
            }
            uVar24 = in_xd0;
            if (in_FPSCR_SZ == '\0') {
              dVar23 = (double)CONCAT44(fVar18,SUB84(dVar16,0));
              uVar24 = in_xd14;
            }
            in_xd14 = uVar24;
            if (in_FPSCR_PR == '\0') {
              dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),
                                        SUB84(dVar23,0) / SUB84(in_dr2,0));
            }
            else {
              dVar23 = dVar23 / in_dr2;
            }
          }
          fVar17 = (float)((ulonglong)in_xd14 >> 0x20);
          if (iVar14 == 0) {
            if (in_FPSCR_SZ == '\0') {
              fVar17 = SUB84(dVar23,0);
              in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),puVar9[1]);
              pdVar7 = (double *)(puVar9 + 2);
              dVar16 = (double)CONCAT44(puVar6[1],SUB84(param_1,0));
              pdVar11 = (double *)(puVar6 + 2);
            }
            else {
              in_xd2 = *(double *)(puVar9 + 1);
              pdVar7 = (double *)(puVar9 + 3);
              dVar16 = *(double *)(puVar6 + 1);
              pdVar11 = (double *)(puVar6 + 3);
            }
            fVar18 = (float)((ulonglong)dVar16 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              fVar19 = SUB84(in_dr2,0) - fVar18;
            }
            else {
              fVar19 = SUB84(in_dr2 - dVar16,0);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar16 = (double)CONCAT44(fVar18,*(uint *)pdVar11);
              dVar22 = (double)CONCAT44(*(uint *)((int)pdVar11 + 4),SUB84(param_2,0));
              param_1 = (double)CONCAT44(fVar18,fVar19);
            }
            else {
              in_xd4 = *pdVar11;
              dVar22 = pdVar11[1];
              param_1 = dVar16;
            }
            if (in_FPSCR_PR == '\0') {
              param_1 = (double)CONCAT44(fVar17 * SUB84(param_1,0) +
                                         (float)((ulonglong)param_1 >> 0x20),SUB84(param_1,0));
            }
            if (in_FPSCR_SZ == '\0') {
              param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),*(uint *)pdVar7);
              pdVar7 = (double *)((int)pdVar7 + 4);
            }
            else {
              in_xd2 = *pdVar7;
              pdVar7 = pdVar7 + 1;
            }
            if (in_FPSCR_PR == '\0') {
              param_1 = (double)(ulonglong)(uint)(SUB84(param_1,0) - SUB84(dVar16,0));
            }
            else {
              param_1 = param_1 - dVar16;
            }
            dVar20 = in_xd4;
            if (in_FPSCR_SZ == '\0') {
              dVar20 = (double)CONCAT44(SUB84(dVar16,0),SUB84(param_1,0));
              param_1 = (double)((ulonglong)dVar16 & 0xffffffff);
            }
            if (in_FPSCR_PR == '\0') {
              dVar20 = (double)CONCAT44(fVar17 * SUB84(dVar20,0) +
                                        (float)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0));
            }
            if (in_FPSCR_SZ == '\0') {
              dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),*(uint *)pdVar7);
            }
            else {
              in_xd2 = *pdVar7;
            }
            fVar18 = (float)((ulonglong)dVar22 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0) - fVar18);
            }
            else {
              dVar20 = dVar20 - dVar22;
            }
            in_dr2 = dVar22;
            if (in_FPSCR_SZ == '\0') {
              param_1 = (double)((ulonglong)dVar20 >> 0x20);
              in_dr2 = (double)CONCAT44(fVar18,SUB84(dVar20,0));
              dVar20 = in_xd4;
            }
            in_xd4 = dVar20;
            if (in_FPSCR_PR == '\0') {
              in_dr2 = (double)CONCAT44(fVar17 * SUB84(in_dr2,0) +
                                        (float)((ulonglong)in_dr2 >> 0x20),SUB84(in_dr2,0));
            }
            param_2 = in_dr2;
            if (in_FPSCR_SZ == '\0') {
              param_2 = (double)((ulonglong)dVar22 & 0xffffffff);
            }
            dVar16 = (double)(*(code *)PTR_FUN_8c057e64)(0);
          }
          else if (iVar14 == 1) {
            *(uint **)((int)puVar15 + -0x1c) = puVar9 + 1;
            *(uint *)((int)puVar15 + -0x14) = puVar6[1];
            *(uint *)((int)puVar15 + -0x18) = puVar6[2];
            *(uint *)((int)puVar15 + -0x10) = puVar6[3];
            puVar10 = *(undefined4 **)((int)puVar15 + -0x1c);
            *(undefined4 **)((int)puVar15 + -0x1c) = puVar10 + 1;
            uVar3 = (*(code *)puVar2)(*(undefined4 *)((int)puVar15 + -0x14),*puVar10);
            uVar21 = (undefined4)((ulonglong)in_dr2 >> 0x20);
            *(undefined4 *)((int)puVar15 + -0xc) = uVar3;
            puVar10 = *(undefined4 **)((int)puVar15 + -0x1c);
            *(undefined4 **)((int)puVar15 + -0x1c) = puVar10 + 1;
            uVar3 = (*(code *)puVar2)(*(undefined4 *)((int)puVar15 + -0x18),*puVar10);
            *(undefined4 *)((int)puVar15 + -8) = uVar3;
            iVar12 = (*(code *)puVar2)(*(undefined4 *)((int)puVar15 + -0x10),
                                       **(undefined4 **)((int)puVar15 + -0x1c));
            fVar17 = SUB84(dVar23,0);
            if (in_FPSCR_PR == '\0') {
              fVar18 = (float)*(int *)((int)puVar15 + -0xc) * fVar17;
              dVar16 = (double)CONCAT44(uVar21,fVar18);
              iVar26 = (int)fVar18;
            }
            else {
              dVar16 = (double)CONCAT44(uVar21,(float)*(int *)((int)puVar15 + -0xc)) * dVar23;
              iVar26 = (int)dVar16;
            }
            if (in_FPSCR_PR == '\0') {
              iVar27 = (int)((float)*(int *)((int)puVar15 + -8) * fVar17);
            }
            else {
              iVar27 = (int)((double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),
                                              (float)*(int *)((int)puVar15 + -8)) * dVar23);
            }
            in_dr2 = in_xd2;
            if (in_FPSCR_SZ == '\0') {
              in_dr2 = (double)CONCAT44((float)iVar12,(float)iVar12);
            }
            if (in_FPSCR_PR == '\0') {
              fVar17 = (float)((ulonglong)in_dr2 >> 0x20) * fVar17;
              in_dr2 = (double)CONCAT44(fVar17,SUB84(in_dr2,0));
              iVar12 = (int)fVar17;
            }
            else {
              in_dr2 = in_dr2 * dVar23;
              iVar12 = (int)in_dr2;
            }
            dVar16 = (double)(*(code *)puVar1)(0,iVar26 + *(int *)((int)puVar15 + -0x14),
                                               iVar27 + *(int *)((int)puVar15 + -0x18),
                                               iVar12 + *(int *)((int)puVar15 + -0x10));
          }
        }
      }
      if ((*param_3 & 4) == 0) {
        (*(code *)PTR_FUN_8c057e68)(0,param_3 + 8);
      }
      iVar13 = iVar13 + 0x10;
    }
    else {
      if (iVar13 != 3) {
        do {
                    /* WARNING: Do nothing block with infinite loop */
        } while( true );
      }
      iVar13 = *param_4;
      for (iVar14 = 0; iVar14 < 3; iVar14 = iVar14 + 1) {
        iVar12 = iVar14 * 4;
        if (*(int *)(iVar13 + iVar12) == 0) {
          if (iVar14 == 0) {
            if ((*param_3 & 1) == 0) {
              dVar16 = (double)(*(code *)PTR_FUN_8c057fc0)(0,param_3 + 2);
            }
          }
          else if (iVar14 == 1) {
            if ((*param_3 & 2) == 0) {
              *(uint **)((int)puVar15 + -0x1c) = param_3 + 5;
              dVar16 = (double)(*(code *)puVar1)(0,**(undefined4 **)((int)puVar15 + -0x1c),
                                                 (*(undefined4 **)((int)puVar15 + -0x1c))[1],
                                                 param_3[7]);
            }
          }
          else if ((iVar14 == 2) && ((*param_3 & 4) == 0)) {
            dVar16 = (double)(*(code *)PTR_FUN_8c057fc4)(0,param_3 + 8);
          }
        }
        else {
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44(param_4[4],SUB84(param_1,0));
          }
          else {
            param_1 = *(double *)(param_4 + 4);
          }
          fVar17 = (float)((ulonglong)param_1 >> 0x20);
          fVar18 = (float)((ulonglong)dVar23 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            bVar25 = fVar18 < fVar17;
          }
          else {
            bVar25 = dVar23 < param_1;
          }
          if (bVar25) {
            if (in_FPSCR_SZ == '\0') {
              in_dr2 = (double)CONCAT44(fVar17,DAT_8c057fc8);
            }
            else {
              in_xd2 = (double)CONCAT44(_DAT_8c057fcc,DAT_8c057fc8);
              in_dr2 = param_1;
            }
            if (in_FPSCR_PR == '\0') {
              in_dr2 = (double)CONCAT44((float)((ulonglong)in_dr2 >> 0x20) + SUB84(in_dr2,0),
                                        SUB84(in_dr2,0));
            }
            else {
              in_dr2 = in_dr2 + in_dr2;
            }
            dVar22 = in_dr2;
            if (in_FPSCR_SZ == '\0') {
              uVar3 = (undefined4)((ulonglong)in_dr2 >> 0x20);
              in_dr2 = (double)CONCAT44(uVar3,uVar3);
              dVar22 = in_xd2;
            }
          }
          else {
            dVar22 = param_1;
            if (in_FPSCR_SZ == '\0') {
              in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),fVar17);
              dVar22 = in_xd2;
            }
          }
          in_xd2 = dVar22;
          if (in_FPSCR_PR == '\0') {
            uVar5 = (uint)SUB84(in_dr2,0);
          }
          else {
            uVar5 = (uint)in_dr2;
          }
          puVar6 = *(uint **)(iVar13 + iVar12);
          puVar9 = puVar6 + 4;
          if (*puVar6 < uVar5) {
            uVar8 = *puVar9;
            while (uVar8 <= uVar5) {
              puVar6 = puVar6 + 4;
              puVar9 = puVar9 + 4;
              if (uVar5 <= *puVar6) break;
              uVar8 = *puVar9;
            }
          }
          uVar3 = (undefined4)((ulonglong)in_dr2 >> 0x20);
          if (*puVar6 == param_4[3] - 1U) {
            puVar9 = *(uint **)(iVar13 + iVar12);
            fVar17 = (float)(int)*puVar6;
            in_dr2 = (double)CONCAT44(uVar3,fVar17);
            if ((int)*puVar6 < 0) {
              if (in_FPSCR_PR == '\0') {
                in_dr2 = (double)CONCAT44(_DAT_8c057fcc,fVar17 + _DAT_8c057fcc);
              }
              else {
                in_dr2 = (double)CONCAT44(_DAT_8c057fcc,fVar17) +
                         (double)CONCAT44(_DAT_8c057fcc,fVar17);
              }
            }
            if (in_FPSCR_SZ == '\0') {
              dVar23 = (double)CONCAT44(fVar18,param_4[4]);
            }
            else {
              in_xd14 = *(undefined8 *)(param_4 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),
                                        SUB84(dVar23,0) - SUB84(in_dr2,0));
            }
            else {
              dVar23 = dVar23 - in_dr2;
            }
          }
          else {
            uVar5 = *puVar6;
            fVar17 = (float)(int)uVar5;
            dVar22 = (double)CONCAT44(uVar3,fVar17);
            if ((int)uVar5 < 0) {
              if (in_FPSCR_PR == '\0') {
                dVar22 = (double)CONCAT44(_DAT_8c057fcc,fVar17 + _DAT_8c057fcc);
              }
              else {
                dVar22 = (double)CONCAT44(_DAT_8c057fcc,fVar17) +
                         (double)CONCAT44(_DAT_8c057fcc,fVar17);
              }
            }
            if (in_FPSCR_SZ == '\0') {
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),param_4[4]);
            }
            else {
              in_xd0 = *(undefined8 *)(param_4 + 4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),
                                        SUB84(dVar16,0) - SUB84(dVar22,0));
            }
            else {
              dVar16 = dVar16 - dVar22;
            }
            fVar17 = (float)(int)(*puVar9 - uVar5);
            in_dr2 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),fVar17);
            if ((int)(*puVar9 - uVar5) < 0) {
              if (in_FPSCR_PR == '\0') {
                in_dr2 = (double)CONCAT44(_DAT_8c057fcc,fVar17 + _DAT_8c057fcc);
              }
              else {
                in_dr2 = (double)CONCAT44(_DAT_8c057fcc,fVar17) +
                         (double)CONCAT44(_DAT_8c057fcc,fVar17);
              }
            }
            uVar24 = in_xd0;
            if (in_FPSCR_SZ == '\0') {
              dVar23 = (double)CONCAT44(fVar18,SUB84(dVar16,0));
              uVar24 = in_xd14;
            }
            in_xd14 = uVar24;
            if (in_FPSCR_PR == '\0') {
              dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),
                                        SUB84(dVar23,0) / SUB84(in_dr2,0));
            }
            else {
              dVar23 = dVar23 / in_dr2;
            }
          }
          fVar17 = (float)((ulonglong)in_xd14 >> 0x20);
          uVar3 = (undefined4)((ulonglong)in_dr2 >> 0x20);
          if (iVar14 == 0) {
            if (in_FPSCR_SZ == '\0') {
              in_dr2 = (double)CONCAT44(uVar3,puVar9[1]);
              pdVar7 = (double *)(puVar9 + 2);
              dVar16 = (double)CONCAT44(puVar6[1],SUB84(param_1,0));
              pdVar11 = (double *)(puVar6 + 2);
              fVar17 = SUB84(dVar23,0);
            }
            else {
              in_xd2 = *(double *)(puVar9 + 1);
              pdVar7 = (double *)(puVar9 + 3);
              dVar16 = *(double *)(puVar6 + 1);
              pdVar11 = (double *)(puVar6 + 3);
            }
            fVar18 = (float)((ulonglong)dVar16 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              fVar19 = SUB84(in_dr2,0) - fVar18;
            }
            else {
              fVar19 = SUB84(in_dr2 - dVar16,0);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar16 = (double)CONCAT44(fVar18,*(uint *)pdVar11);
              dVar22 = (double)CONCAT44(*(uint *)((int)pdVar11 + 4),SUB84(param_2,0));
              param_1 = (double)CONCAT44(fVar18,fVar19);
            }
            else {
              in_xd4 = *pdVar11;
              dVar22 = pdVar11[1];
              param_1 = dVar16;
            }
            if (in_FPSCR_PR == '\0') {
              param_1 = (double)CONCAT44(fVar17 * SUB84(param_1,0) +
                                         (float)((ulonglong)param_1 >> 0x20),SUB84(param_1,0));
            }
            if (in_FPSCR_SZ == '\0') {
              param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),*(uint *)pdVar7);
              pdVar7 = (double *)((int)pdVar7 + 4);
            }
            else {
              in_xd2 = *pdVar7;
              pdVar7 = pdVar7 + 1;
            }
            if (in_FPSCR_PR == '\0') {
              param_1 = (double)(ulonglong)(uint)(SUB84(param_1,0) - SUB84(dVar16,0));
            }
            else {
              param_1 = param_1 - dVar16;
            }
            dVar20 = in_xd4;
            if (in_FPSCR_SZ == '\0') {
              dVar20 = (double)CONCAT44(SUB84(dVar16,0),SUB84(param_1,0));
              param_1 = (double)((ulonglong)dVar16 & 0xffffffff);
            }
            if (in_FPSCR_PR == '\0') {
              dVar20 = (double)CONCAT44(fVar17 * SUB84(dVar20,0) +
                                        (float)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0));
            }
            if (in_FPSCR_SZ == '\0') {
              dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),*(uint *)pdVar7);
            }
            else {
              in_xd2 = *pdVar7;
            }
            fVar18 = (float)((ulonglong)dVar22 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0) - fVar18);
            }
            else {
              dVar20 = dVar20 - dVar22;
            }
            in_dr2 = dVar22;
            if (in_FPSCR_SZ == '\0') {
              param_1 = (double)((ulonglong)dVar20 >> 0x20);
              in_dr2 = (double)CONCAT44(fVar18,SUB84(dVar20,0));
              dVar20 = in_xd4;
            }
            in_xd4 = dVar20;
            if (in_FPSCR_PR == '\0') {
              in_dr2 = (double)CONCAT44(fVar17 * SUB84(in_dr2,0) +
                                        (float)((ulonglong)in_dr2 >> 0x20),SUB84(in_dr2,0));
            }
            param_2 = in_dr2;
            if (in_FPSCR_SZ == '\0') {
              param_2 = (double)((ulonglong)dVar22 & 0xffffffff);
            }
            dVar16 = (double)(*(code *)PTR_FUN_8c057fd0)(0);
          }
          else if (iVar14 == 1) {
            *(uint **)((int)puVar15 + -0x1c) = puVar9 + 1;
            *(uint *)((int)puVar15 + -0x14) = puVar6[1];
            *(uint *)((int)puVar15 + -0x18) = puVar6[2];
            *(uint *)((int)puVar15 + -0x10) = puVar6[3];
            puVar10 = *(undefined4 **)((int)puVar15 + -0x1c);
            *(undefined4 **)((int)puVar15 + -0x1c) = puVar10 + 1;
            uVar3 = (*(code *)puVar2)(*(undefined4 *)((int)puVar15 + -0x14),*puVar10);
            uVar21 = (undefined4)((ulonglong)in_dr2 >> 0x20);
            *(undefined4 *)((int)puVar15 + -8) = uVar3;
            puVar10 = *(undefined4 **)((int)puVar15 + -0x1c);
            *(undefined4 **)((int)puVar15 + -0x1c) = puVar10 + 1;
            uVar3 = (*(code *)puVar2)(*(undefined4 *)((int)puVar15 + -0x18),*puVar10);
            *(undefined4 *)((int)puVar15 + -0xc) = uVar3;
            iVar12 = (*(code *)puVar2)(*(undefined4 *)((int)puVar15 + -0x10),
                                       **(undefined4 **)((int)puVar15 + -0x1c));
            fVar17 = SUB84(dVar23,0);
            if (in_FPSCR_PR == '\0') {
              fVar18 = (float)*(int *)((int)puVar15 + -8) * fVar17;
              dVar16 = (double)CONCAT44(uVar21,fVar18);
              iVar26 = (int)fVar18;
            }
            else {
              dVar16 = (double)CONCAT44(uVar21,(float)*(int *)((int)puVar15 + -8)) * dVar23;
              iVar26 = (int)dVar16;
            }
            if (in_FPSCR_PR == '\0') {
              iVar27 = (int)((float)*(int *)((int)puVar15 + -0xc) * fVar17);
            }
            else {
              iVar27 = (int)((double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),
                                              (float)*(int *)((int)puVar15 + -0xc)) * dVar23);
            }
            in_dr2 = in_xd2;
            if (in_FPSCR_SZ == '\0') {
              in_dr2 = (double)CONCAT44((float)iVar12,(float)iVar12);
            }
            if (in_FPSCR_PR == '\0') {
              fVar17 = (float)((ulonglong)in_dr2 >> 0x20) * fVar17;
              in_dr2 = (double)CONCAT44(fVar17,SUB84(in_dr2,0));
              iVar12 = (int)fVar17;
            }
            else {
              in_dr2 = in_dr2 * dVar23;
              iVar12 = (int)in_dr2;
            }
            dVar16 = (double)(*(code *)puVar1)(0,iVar26 + *(int *)((int)puVar15 + -0x14),
                                               iVar27 + *(int *)((int)puVar15 + -0x18),
                                               iVar12 + *(int *)((int)puVar15 + -0x10));
          }
          else if (iVar14 == 2) {
            if (in_FPSCR_SZ == '\0') {
              in_dr2 = (double)CONCAT44(uVar3,puVar9[1]);
              pdVar7 = (double *)(puVar9 + 2);
              dVar16 = (double)CONCAT44(puVar6[1],SUB84(param_1,0));
              pdVar11 = (double *)(puVar6 + 2);
              fVar17 = SUB84(dVar23,0);
            }
            else {
              in_xd2 = *(double *)(puVar9 + 1);
              pdVar7 = (double *)(puVar9 + 3);
              dVar16 = *(double *)(puVar6 + 1);
              pdVar11 = (double *)(puVar6 + 3);
            }
            fVar18 = (float)((ulonglong)dVar16 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              fVar19 = SUB84(in_dr2,0) - fVar18;
            }
            else {
              fVar19 = SUB84(in_dr2 - dVar16,0);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar16 = (double)CONCAT44(fVar18,*(uint *)pdVar11);
              dVar22 = (double)CONCAT44(*(uint *)((int)pdVar11 + 4),SUB84(param_2,0));
              param_1 = (double)CONCAT44(fVar18,fVar19);
            }
            else {
              in_xd4 = *pdVar11;
              dVar22 = pdVar11[1];
              param_1 = dVar16;
            }
            if (in_FPSCR_PR == '\0') {
              param_1 = (double)CONCAT44(fVar17 * SUB84(param_1,0) +
                                         (float)((ulonglong)param_1 >> 0x20),SUB84(param_1,0));
            }
            if (in_FPSCR_SZ == '\0') {
              param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),*(uint *)pdVar7);
              pdVar7 = (double *)((int)pdVar7 + 4);
            }
            else {
              in_xd2 = *pdVar7;
              pdVar7 = pdVar7 + 1;
            }
            if (in_FPSCR_PR == '\0') {
              param_1 = (double)(ulonglong)(uint)(SUB84(param_1,0) - SUB84(dVar16,0));
            }
            else {
              param_1 = param_1 - dVar16;
            }
            dVar20 = in_xd4;
            if (in_FPSCR_SZ == '\0') {
              dVar20 = (double)CONCAT44(SUB84(dVar16,0),SUB84(param_1,0));
              param_1 = (double)((ulonglong)dVar16 & 0xffffffff);
            }
            if (in_FPSCR_PR == '\0') {
              dVar20 = (double)CONCAT44(fVar17 * SUB84(dVar20,0) +
                                        (float)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0));
            }
            if (in_FPSCR_SZ == '\0') {
              dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),*(uint *)pdVar7);
            }
            else {
              in_xd2 = *pdVar7;
            }
            fVar18 = (float)((ulonglong)dVar22 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),SUB84(dVar20,0) - fVar18);
            }
            else {
              dVar20 = dVar20 - dVar22;
            }
            in_dr2 = dVar22;
            if (in_FPSCR_SZ == '\0') {
              param_1 = (double)((ulonglong)dVar20 >> 0x20);
              in_dr2 = (double)CONCAT44(fVar18,SUB84(dVar20,0));
              dVar20 = in_xd4;
            }
            in_xd4 = dVar20;
            if (in_FPSCR_PR == '\0') {
              in_dr2 = (double)CONCAT44(fVar17 * SUB84(in_dr2,0) +
                                        (float)((ulonglong)in_dr2 >> 0x20),SUB84(in_dr2,0));
            }
            param_2 = in_dr2;
            if (in_FPSCR_SZ == '\0') {
              param_2 = (double)((ulonglong)dVar22 & 0xffffffff);
            }
            dVar16 = (double)(*(code *)PTR_FUN_8c0580e0)(0);
          }
        }
      }
      iVar13 = iVar13 + 0x18;
    }
    *param_4 = iVar13;
    (*(code *)PTR_FUN_8c0580e4)(param_4[1]);
    param_4[1] = param_4[1] + 0x40;
    for (uVar5 = param_3[0xb]; uVar5 != 0; uVar5 = FUN_8c057bc0(uVar5,param_4)) {
    }
    (*(code *)PTR_FUN_8c0580e8)(1);
    uVar5 = param_3[0xc];
  }
  return uVar5;
}

