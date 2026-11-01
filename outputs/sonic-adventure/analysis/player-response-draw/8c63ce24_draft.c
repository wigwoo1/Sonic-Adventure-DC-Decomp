/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c63d10a) */
/* WARNING: Removing unreachable block (ram,0x8c63d0ca) */
/* WARNING: Removing unreachable block (ram,0x8c63d320) */
/* WARNING: Removing unreachable block (ram,0x8c63d0e8) */
/* WARNING: Removing unreachable block (ram,0x8c63d0fa) */
/* WARNING: Removing unreachable block (ram,0x8c63d116) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c63ce24(undefined4 param_1,double param_2,int *param_3,int param_4,uint param_5)

{
  double *pdVar1;
  int iVar2;
  double *pdVar3;
  int iVar4;
  uint uVar5;
  short *psVar6;
  int iVar7;
  double *pdVar8;
  undefined4 uVar9;
  int iVar10;
  undefined8 *puVar11;
  double *pdVar12;
  undefined4 extraout_fr1;
  undefined4 extraout_fr1_00;
  double dVar13;
  float fVar14;
  float fVar15;
  double in_dr2;
  double dVar16;
  float fVar17;
  double dVar18;
  undefined4 uVar21;
  double dVar19;
  float fVar22;
  undefined4 uVar23;
  float fVar25;
  double dVar24;
  float fVar26;
  double in_dr12;
  undefined4 uVar28;
  double dVar27;
  float fVar29;
  double in_dr14;
  double dVar30;
  double in_xd0;
  double in_xd2;
  double dVar31;
  double in_xd4;
  double in_xd6;
  double in_xd8;
  double dVar32;
  double in_xd10;
  double dVar33;
  undefined8 in_xd14;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_3c [16];
  double dStack_2c;
  undefined8 uStack_24;
  undefined4 uVar20;
  
  pdVar1 = DAT_8c63cf14;
  uVar23 = (undefined4)((ulonglong)in_dr14 >> 0x20);
  uVar28 = (undefined4)((ulonglong)in_dr12 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    uStack_24 = CONCAT44(SUB84(in_dr14,0),(undefined4)uStack_24);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_24._0_4_ = uVar23;
    pdVar12 = &dStack_2c;
    dStack_2c = (double)CONCAT44(SUB84(in_dr12,0),uVar28);
  }
  else {
    pdVar12 = (double *)auStack_3c;
    uStack_24 = in_xd14;
    dStack_2c = in_dr14;
  }
  *(undefined4 *)((int)pdVar12 + -4) = in_PR;
  iVar2 = (int)DAT_8c63cefe;
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),DAT_8c63cf10);
  }
  else {
    in_xd2 = (double)CONCAT44(DAT_8c63cf14,DAT_8c63cf10);
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)pdVar12 + iVar2 + 0x10) = SUB84(in_dr2,0);
  }
  else {
    *(double *)((int)pdVar12 + iVar2 + 0x10) = in_xd2;
  }
  if (in_FPSCR_SZ == '\0') {
    uVar20 = *(undefined4 *)DAT_8c63cf14;
    dVar18 = (double)CONCAT44(uVar20,param_1);
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)DAT_8c63cf18);
    in_dr14 = (double)CONCAT44(uVar23,uVar20);
  }
  else {
    dVar18 = *DAT_8c63cf14;
    in_xd2 = *DAT_8c63cf18;
    uVar20 = (undefined4)((ulonglong)dVar18 >> 0x20);
  }
  if (in_FPSCR_PR == '\0') {
    fVar29 = SUB84(in_dr14,0) * SUB84(in_dr2,0);
  }
  else {
    fVar29 = SUB84(in_dr14 * in_dr2,0);
  }
  if (in_FPSCR_SZ == '\0') {
    in_dr12 = (double)CONCAT44(uVar28,uVar20);
    dVar16 = (double)CONCAT44(*(undefined4 *)DAT_8c63cf1c,SUB84(in_dr2,0));
  }
  else {
    dVar16 = *DAT_8c63cf1c;
  }
  *(undefined1 **)((int)pdVar12 + (int)DAT_8c63cf02 + iVar2 + -4) =
       (undefined1 *)((int)pdVar12 + (int)DAT_8c63cf00 + iVar2 + -4);
  *(undefined1 **)((int)pdVar12 + (int)DAT_8c63cf04 + iVar2 + -4) =
       (undefined1 *)((int)pdVar12 + iVar2 + 0x5c);
  *(undefined1 **)((int)pdVar12 + (int)DAT_8c63cf06 + iVar2 + -4) =
       (undefined1 *)((int)pdVar12 + iVar2 + 0x54);
  *(undefined1 **)((int)pdVar12 + (int)DAT_8c63cf08 + iVar2 + -4) =
       (undefined1 *)((int)pdVar12 + iVar2 + 100);
  *(undefined1 **)((int)pdVar12 + (int)DAT_8c63cf0a + iVar2 + -4) =
       (undefined1 *)((int)pdVar12 + iVar2 + 0x4c);
  *(undefined1 **)((int)pdVar12 + (int)DAT_8c63cf0c + iVar2 + -4) =
       (undefined1 *)((int)pdVar12 + iVar2 + 0x44);
  if (in_FPSCR_PR == '\0') {
    fVar26 = SUB84(in_dr12,0) * (float)((ulonglong)dVar16 >> 0x20);
  }
  else {
    fVar26 = SUB84(in_dr12 * dVar16,0);
  }
  (*(code *)PTR_FUN_8c63cf20)();
  *(uint *)((int)pdVar12 + iVar2 + -4) = DAT_8c63cf28 & param_5;
  if (((DAT_8c63cf28 & param_5) == 0) || ((param_5 & 0x40) == 0)) {
    if (*(int *)((int)pdVar12 + iVar2 + -4) == 0) {
      if ((param_5 & 0x40) == 0) {
        (*(code *)PTR_FUN_8c63cf24)(DAT_8c63cf44);
        pdVar3 = (double *)(*(code *)PTR_FUN_8c63cf48)();
      }
      else {
        (*(code *)PTR_FUN_8c63cf24)(DAT_8c63cf3c);
        pdVar3 = (double *)(*(code *)PTR_FUN_8c63cf40)();
      }
    }
    else {
      (*(code *)PTR_FUN_8c63cf24)(DAT_8c63cf34);
      pdVar3 = (double *)(*(code *)PTR_FUN_8c63cf38)();
    }
  }
  else {
    (*(code *)PTR_FUN_8c63cf24)(DAT_8c63cf2c);
    pdVar3 = (double *)(*(code *)PTR_FUN_8c63cf30)();
  }
  uVar28 = _DAT_8c63cf90;
  if ((param_5 & 0xf) == 0) {
    iVar10 = 2;
  }
  else {
    iVar10 = 1;
  }
  uVar5 = param_5 & 1;
  if (in_FPSCR_SZ == '\0') {
    dVar27 = (double)CONCAT44(DAT_8c63cf8c,fVar26);
  }
  else {
    dVar27 = (double)CONCAT44(_DAT_8c63cf90,DAT_8c63cf8c);
  }
  *(uint *)((int)pdVar12 + iVar2 + 0x14) = uVar5;
  dVar30 = (double)CONCAT44(0x3f800000,fVar29);
  if (*(int *)((int)pdVar12 + iVar2 + -4) == 0) {
    *(undefined4 *)((int)pdVar12 + iVar2 + 4) = 0;
    *(undefined4 *)((int)pdVar12 + iVar2 + -4) = 1;
    *(undefined4 *)((int)pdVar12 + iVar2 + 0xc) = 1;
    *(undefined4 *)((int)pdVar12 + iVar2 + 8) = 0;
    *(uint *)((int)pdVar12 + iVar2 + 0x14) = uVar5;
    if (0 < param_4) {
      do {
        if (*(int *)((int)pdVar12 + iVar2 + 0x14) == 0) {
          iVar4 = *(int *)((int)pdVar12 + iVar2 + 8);
        }
        else {
          iVar4 = 0;
        }
        *(int *)((int)pdVar12 + iVar2) = iVar4;
        (*(code *)PTR_FUN_8c63d3d0)
                  (0,iVar4 * 0xc + *param_3,
                   *(undefined4 *)((int)pdVar12 + (int)DAT_8c63d3c6 + iVar2 + -4));
        *(int *)((int)pdVar12 + iVar2 + 0x18) = *(int *)((int)pdVar12 + iVar2 + 0xc);
        (*(code *)PTR_FUN_8c63d3d0)
                  (0,*(int *)((int)pdVar12 + iVar2 + 0xc) * 0xc + *param_3,
                   *(int *)((int)pdVar12 + (int)DAT_8c63d3c6 + iVar2 + -4) + 0xc);
        iVar4 = (*(code *)PTR_FUN_8c63d3d4)
                          ((undefined1 *)((int)pdVar12 + (int)DAT_8c63d3c6 + iVar2 + -4),2);
        if (iVar4 != 0) {
          iVar7 = (int)DAT_8c63d3ca;
          **(undefined4 **)((int)pdVar12 + (int)DAT_8c63d3c8 + iVar2 + -4) =
               *(undefined4 *)(*(int *)((int)pdVar12 + iVar2) * 4 + param_3[1]);
          iVar4 = (int)DAT_8c63d3c6;
          *(undefined4 *)(*(int *)((int)pdVar12 + (int)DAT_8c63d3c8 + iVar2 + -4) + 4) =
               *(undefined4 *)(*(int *)((int)pdVar12 + iVar2 + -4) * 4 + param_3[1]);
          iVar4 = (*(code *)PTR_FUN_8c63d3d8)
                            ((undefined1 *)((int)pdVar12 + iVar4 + iVar2 + -4),
                             (undefined1 *)((int)pdVar12 + iVar7 + iVar2 + -4),param_5);
          uVar23 = SUB84(param_2,0);
          if (iVar4 != 0) {
            uVar20 = (undefined4)((ulonglong)dVar30 >> 0x20);
            dVar19 = dVar30;
            if (in_FPSCR_SZ == '\0') {
              dVar19 = (double)CONCAT44(uVar20,SUB84(dVar18,0));
            }
            pdVar8 = *(double **)((int)pdVar12 + (int)DAT_8c63d3ca + iVar2 + -4);
            if (in_FPSCR_SZ == '\0') {
              dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),uVar20);
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),*(undefined4 *)(pdVar8 + 1)
                                       );
            }
            else {
              in_xd2 = pdVar8[1];
              in_xd4 = dVar30;
            }
            if (in_FPSCR_PR == '\0') {
              dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) / SUB84(dVar16,0),
                                        SUB84(dVar19,0));
            }
            else {
              dVar19 = dVar19 / dVar16;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),
                                        *(undefined4 *)((int)pdVar8 + 0x14));
            }
            else {
              in_xd2 = *(double *)((int)pdVar8 + 0x14);
            }
            if (in_FPSCR_PR == '\0') {
              dVar18 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),
                                        SUB84(dVar19,0) / SUB84(dVar16,0));
            }
            else {
              dVar18 = dVar19 / dVar16;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),*(undefined4 *)pdVar8);
              uVar23 = *(undefined4 *)((int)pdVar1 + 0xc);
            }
            else {
              in_xd2 = *pdVar8;
              in_xd6 = *(double *)((int)pdVar1 + 0xc);
            }
            if (in_FPSCR_PR == '\0') {
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),
                                        SUB84(dVar16,0) * SUB84(dVar30,0));
            }
            else {
              dVar16 = dVar16 * dVar30;
            }
            uVar21 = (undefined4)((ulonglong)dVar18 >> 0x20);
            dVar19 = dVar18;
            param_2 = in_xd6;
            if (in_FPSCR_SZ == '\0') {
              dVar19 = (double)CONCAT44(uVar21,extraout_fr1_00);
              param_2 = (double)CONCAT44(uVar23,uVar23);
            }
            if (in_FPSCR_PR == '\0') {
              param_2 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) * SUB84(dVar16,0) +
                                         (float)((ulonglong)param_2 >> 0x20),SUB84(param_2,0));
            }
            uVar23 = SUB84(dVar18,0);
            if (in_FPSCR_SZ == '\0') {
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),
                                        *(undefined4 *)((int)pdVar8 + 0xc));
              dVar19 = (double)CONCAT44(uVar23,SUB84(dVar19,0));
            }
            else {
              in_xd2 = *(double *)((int)pdVar8 + 0xc);
              dVar19 = in_xd4;
            }
            if (in_FPSCR_PR == '\0') {
              fVar29 = SUB84(dVar16,0) * SUB84(dVar30,0);
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),fVar29);
              param_2 = (double)CONCAT44((int)((ulonglong)param_2 >> 0x20),
                                         (float)((ulonglong)dVar19 >> 0x20) * fVar29 +
                                         SUB84(param_2,0));
            }
            else {
              dVar16 = dVar16 * dVar30;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),
                                        *(undefined4 *)((int)pdVar8 + 4));
              dVar19 = (double)CONCAT44(uVar21,SUB84(dVar19,0));
            }
            else {
              in_xd2 = *(double *)((int)pdVar8 + 4);
              dVar19 = dVar18;
            }
            if (in_FPSCR_PR == '\0') {
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),
                                        SUB84(dVar16,0) * SUB84(dVar27,0));
            }
            else {
              dVar16 = dVar16 * dVar27;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)(pdVar1 + 2),*(undefined4 *)(pdVar1 + 2));
            }
            else {
              dVar32 = pdVar1[2];
              in_xd8 = dVar32;
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) * SUB84(dVar16,0) +
                                        (float)((ulonglong)dVar32 >> 0x20),SUB84(dVar32,0));
            }
            if (in_FPSCR_SZ == '\0') {
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),*(undefined4 *)(pdVar8 + 2)
                                       );
            }
            else {
              in_xd2 = pdVar8[2];
            }
            if (in_FPSCR_PR == '\0') {
              fVar29 = SUB84(dVar16,0) * SUB84(dVar27,0);
            }
            else {
              fVar29 = SUB84(dVar16 * dVar27,0);
            }
            uVar9 = (undefined4)((ulonglong)dVar32 >> 0x20);
            fVar26 = (float)((ulonglong)param_2 >> 0x20);
            dVar24 = dVar32;
            dVar33 = param_2;
            dVar16 = in_xd4;
            if (in_FPSCR_SZ == '\0') {
              dVar24 = (double)CONCAT44(uVar9,fVar26);
              dVar33 = in_xd10;
              dVar16 = (double)CONCAT44(uVar23,SUB84(dVar19,0));
            }
            fVar22 = SUB84(param_2,0);
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),SUB84(dVar24,0) - fVar22);
            }
            else {
              dVar24 = dVar24 - param_2;
            }
            dVar19 = dVar30;
            if (in_FPSCR_SZ == '\0') {
              dVar19 = (double)CONCAT44(uVar20,fVar29);
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44(uVar9,(float)((ulonglong)dVar16 >> 0x20) * SUB84(dVar19,0) +
                                              SUB84(dVar32,0));
            }
            dVar13 = dVar33;
            if (in_FPSCR_SZ == '\0') {
              dVar13 = (double)CONCAT44(SUB84(dVar24,0),SUB84(dVar16,0));
            }
            fVar29 = SUB84(dVar32,0);
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) - fVar29,SUB84(dVar24,0))
              ;
            }
            else {
              dVar24 = dVar24 - dVar32;
            }
            fVar25 = (float)((ulonglong)dVar24 >> 0x20);
            dVar16 = dVar24;
            if (in_FPSCR_SZ == '\0') {
              dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),fVar25);
              dVar16 = in_xd2;
            }
            fVar14 = (float)((ulonglong)dVar13 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              fVar15 = SQRT(fVar14 * SUB84(dVar24,0) + SUB84(dVar19,0) * fVar25);
              fVar17 = (float)((ulonglong)dVar19 >> 0x20) / fVar15;
              dVar19 = (double)CONCAT44(fVar17,fVar15);
              dVar24 = (double)CONCAT44(fVar25 * fVar17,SUB84(dVar24,0));
            }
            else {
              dVar19 = SQRT(dVar19 * dVar24) / SQRT(dVar19 * dVar24);
              fVar17 = (float)((ulonglong)dVar19 >> 0x20);
              dVar24 = dVar24 * dVar19;
            }
            dVar31 = dVar19;
            if (in_FPSCR_SZ == '\0') {
              dVar19 = (double)CONCAT44(fVar17,fVar17);
              dVar31 = dVar16;
            }
            fVar25 = SUB84(dVar19,0);
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),SUB84(dVar24,0) * fVar25);
            }
            else {
              dVar24 = dVar24 * dVar19;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)pdVar12 + iVar2 + 0x1c) = (int)((ulonglong)dVar19 >> 0x20);
            }
            else {
              *(double *)((int)pdVar12 + iVar2 + 0x1c) = dVar19;
            }
            fVar17 = (float)((ulonglong)dVar27 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar16 = (double)CONCAT44(0x40000000,fVar25);
              dVar19 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * fVar17,
                                        SUB84(dVar24,0) * fVar17);
            }
            else {
              dVar16 = (double)CONCAT44(0x3f800000,fVar25) + (double)CONCAT44(0x3f800000,fVar25);
              dVar19 = dVar24 * dVar27 * dVar27;
            }
            fVar25 = (float)((ulonglong)dVar19 >> 0x20);
            dVar24 = dVar19;
            if (in_FPSCR_SZ == '\0') {
              dVar13 = (double)CONCAT44(fVar14,fVar25);
              dVar24 = in_xd0;
            }
            fVar14 = (float)((ulonglong)dVar16 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              fVar17 = SUB84(dVar13,0) * fVar14;
            }
            else {
              fVar17 = SUB84(dVar13 * dVar16,0);
            }
            fVar15 = -SUB84(dVar19,0);
            dVar13 = (double)CONCAT44(fVar25,fVar15);
            dVar19 = dVar33;
            if (in_FPSCR_SZ == '\0') {
              dVar16 = (double)CONCAT44(fVar14,fVar15);
              dVar19 = dVar31;
            }
            if (in_FPSCR_PR == '\0') {
              fVar14 = (float)((ulonglong)dVar16 >> 0x20);
              dVar16 = (double)CONCAT44(fVar14,SUB84(dVar16,0) * fVar14);
            }
            else {
              dVar16 = dVar16 * dVar16;
            }
            if (in_FPSCR_SZ == '\0') {
              *(float *)((int)pdVar12 + iVar2 + 0x28) = fVar17;
              *(int *)((int)pdVar12 + iVar2 + 0x24) = SUB84(dVar16,0);
            }
            else {
              *(double *)((int)pdVar12 + iVar2 + 0x28) = dVar24;
              *(double *)((int)pdVar12 + iVar2 + 0x24) = dVar19;
            }
            fVar14 = (float)((ulonglong)dVar32 >> 0x20);
            dVar31 = dVar32;
            if (in_FPSCR_SZ == '\0') {
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),fVar14);
              dVar31 = dVar19;
            }
            if (in_FPSCR_PR == '\0') {
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),SUB84(dVar16,0) - fVar15);
            }
            else {
              dVar16 = dVar16 - dVar13;
            }
            uVar20 = **(undefined4 **)((int)pdVar12 + (int)DAT_8c63d3cc + iVar2 + -4);
            uVar9 = (*(undefined4 **)((int)pdVar12 + (int)DAT_8c63d3cc + iVar2 + -4))[1];
            *(undefined4 *)(pdVar3 + 3) = uVar20;
            if (in_FPSCR_SZ == '\0') {
              *(undefined4 *)((int)pdVar3 + 0xc) = uVar21;
              pdVar3 = pdVar3 + 1;
              *(int *)pdVar3 = SUB84(dVar16,0);
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),fVar26);
            }
            else {
              pdVar3[1] = dVar18;
              *pdVar3 = dVar31;
              dVar31 = param_2;
            }
            if (in_FPSCR_PR == '\0') {
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),SUB84(dVar16,0) - fVar25);
            }
            else {
              dVar16 = dVar16 - dVar13;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar3 = (double *)((int)pdVar3 + -4);
              *(int *)pdVar3 = SUB84(dVar16,0);
            }
            else {
              pdVar3 = pdVar3 + -1;
              *pdVar3 = dVar31;
            }
            *(undefined4 *)((int)pdVar3 + -4) = uVar28;
            in_xd2 = in_xd8;
            if (in_FPSCR_SZ == '\0') {
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),fVar29);
              in_xd2 = dVar31;
            }
            if (in_FPSCR_PR == '\0') {
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),SUB84(dVar16,0) - fVar15);
            }
            else {
              dVar16 = dVar16 - dVar13;
            }
            *(undefined4 *)((int)pdVar3 + 0x34) = uVar9;
            if (in_FPSCR_SZ == '\0') {
              *(undefined4 *)(pdVar3 + 5) = uVar23;
              pdVar3 = (double *)((int)pdVar3 + 0x24);
              *(int *)pdVar3 = SUB84(dVar16,0);
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),fVar22);
            }
            else {
              *(double *)((int)pdVar3 + 0x24) = in_xd4;
              pdVar3 = (double *)((int)pdVar3 + 0x1c);
              *pdVar3 = in_xd2;
              in_xd2 = in_xd6;
            }
            if (in_FPSCR_PR == '\0') {
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),SUB84(dVar16,0) - fVar25);
            }
            else {
              dVar16 = dVar16 - dVar13;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar3 = (double *)((int)pdVar3 + -4);
              *(int *)pdVar3 = SUB84(dVar16,0);
            }
            else {
              pdVar3 = pdVar3 + -1;
              *pdVar3 = in_xd2;
            }
            *(undefined4 *)((int)pdVar3 + -4) = uVar28;
            *(undefined4 *)((int)pdVar3 + 0x34) = uVar20;
            if (in_FPSCR_SZ == '\0') {
              pdVar3 = pdVar3 + 5;
              *(undefined4 *)pdVar3 = uVar21;
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),
                                        *(undefined4 *)((int)pdVar12 + iVar2 + 0x24));
            }
            else {
              pdVar3 = (double *)((int)pdVar3 + 0x24);
              *pdVar3 = dVar18;
              in_xd2 = *(double *)((int)pdVar12 + iVar2 + 0x24);
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44(fVar14 + SUB84(dVar16,0),fVar29);
            }
            else {
              dVar32 = dVar32 + dVar16;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar3 = (double *)((int)pdVar3 + -4);
              *(int *)pdVar3 = (int)((ulonglong)dVar32 >> 0x20);
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),
                                        *(undefined4 *)((int)pdVar12 + iVar2 + 0x28));
            }
            else {
              pdVar3 = pdVar3 + -1;
              *pdVar3 = dVar32;
              in_xd2 = *(double *)((int)pdVar12 + iVar2 + 0x28);
            }
            if (in_FPSCR_PR == '\0') {
              param_2 = (double)CONCAT44(fVar26 + SUB84(dVar16,0),fVar22);
            }
            else {
              param_2 = param_2 + dVar16;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar3 = (double *)((int)pdVar3 + -4);
              *(int *)pdVar3 = (int)((ulonglong)param_2 >> 0x20);
            }
            else {
              pdVar3 = pdVar3 + -1;
              *pdVar3 = param_2;
            }
            *(undefined4 *)((int)pdVar3 + -4) = uVar28;
            uVar20 = DAT_8c63d3dc;
            *(undefined4 *)((int)pdVar3 + 0x34) = uVar9;
            if (in_FPSCR_SZ == '\0') {
              pdVar3 = pdVar3 + 5;
              *(undefined4 *)pdVar3 = uVar23;
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),
                                        *(undefined4 *)((int)pdVar12 + iVar2 + 0x24));
            }
            else {
              pdVar3 = (double *)((int)pdVar3 + 0x24);
              *pdVar3 = in_xd4;
              in_xd2 = *(double *)((int)pdVar12 + iVar2 + 0x24);
            }
            if (in_FPSCR_PR == '\0') {
              fVar29 = SUB84(dVar32,0) + SUB84(dVar16,0);
            }
            else {
              fVar29 = SUB84(dVar32 + dVar16,0);
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar3 = (double *)((int)pdVar3 + -4);
              *(float *)pdVar3 = fVar29;
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),
                                        *(undefined4 *)((int)pdVar12 + iVar2 + 0x28));
            }
            else {
              pdVar3 = pdVar3 + -1;
              *pdVar3 = in_xd8;
              in_xd2 = *(double *)((int)pdVar12 + iVar2 + 0x28);
            }
            if (in_FPSCR_PR == '\0') {
              param_2 = (double)(ulonglong)(uint)(SUB84(param_2,0) + SUB84(dVar16,0));
            }
            else {
              param_2 = param_2 + dVar16;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar3 = (double *)((int)pdVar3 + -4);
              *(int *)pdVar3 = SUB84(param_2,0);
            }
            else {
              pdVar3 = pdVar3 + -1;
              *pdVar3 = in_xd6;
            }
            *(undefined4 *)((int)pdVar3 + -4) = uVar20;
            pdVar3 = (double *)((int)pdVar3 + 0x1c);
            in_xd0 = dVar24;
            in_xd10 = dVar33;
          }
        }
        *(int *)((int)pdVar12 + iVar2 + 4) = *(int *)((int)pdVar12 + iVar2 + 4) + 1;
        *(int *)((int)pdVar12 + iVar2 + -4) = *(int *)((int)pdVar12 + iVar2 + -4) + iVar10;
        *(int *)((int)pdVar12 + iVar2 + 0xc) = *(int *)((int)pdVar12 + iVar2 + 0xc) + iVar10;
        *(int *)((int)pdVar12 + iVar2 + 8) = *(int *)((int)pdVar12 + iVar2 + 8) + iVar10;
      } while (*(int *)((int)pdVar12 + iVar2 + 4) < param_4);
    }
  }
  else {
    *(undefined4 *)((int)pdVar12 + iVar2 + 4) = 0;
    *(undefined4 *)((int)pdVar12 + iVar2 + -4) = 1;
    *(undefined4 *)((int)pdVar12 + iVar2 + 0xc) = 1;
    *(undefined4 *)((int)pdVar12 + iVar2 + 8) = 0;
    *(uint *)((int)pdVar12 + iVar2 + 0x14) = uVar5;
    if (0 < param_4) {
      do {
        if (*(int *)((int)pdVar12 + iVar2 + 0x14) == 0) {
          iVar4 = *(int *)((int)pdVar12 + iVar2 + 8);
        }
        else {
          iVar4 = 0;
        }
        *(int *)((int)pdVar12 + iVar2) = iVar4;
        (*(code *)PTR_FUN_8c63d150)
                  (0,iVar4 * 0xc + *param_3,
                   *(undefined4 *)((int)pdVar12 + (int)DAT_8c63d142 + iVar2 + -4));
        *(int *)((int)pdVar12 + iVar2 + 0x18) = *(int *)((int)pdVar12 + iVar2 + 0xc);
        (*(code *)PTR_FUN_8c63d150)
                  (0,*(int *)((int)pdVar12 + iVar2 + 0xc) * 0xc + *param_3,
                   *(int *)((int)pdVar12 + (int)DAT_8c63d142 + iVar2 + -4) + 0xc);
        iVar4 = (*(code *)PTR_FUN_8c63d154)
                          ((undefined1 *)((int)pdVar12 + (int)DAT_8c63d142 + iVar2 + -4),2);
        if (iVar4 != 0) {
          iVar7 = *(int *)((int)pdVar12 + iVar2);
          iVar4 = (int)DAT_8c63d144;
          **(undefined2 **)((int)pdVar12 + (int)DAT_8c63d144 + iVar2 + -4) =
               *(undefined2 *)(param_3[2] + iVar7 * 4);
          *(undefined2 *)(*(int *)((int)pdVar12 + iVar4 + iVar2 + -4) + 2) =
               *(undefined2 *)(param_3[2] + iVar7 * 4 + 2);
          iVar7 = *(int *)((int)pdVar12 + iVar2 + -4);
          iVar4 = (int)DAT_8c63d144;
          *(undefined2 *)(*(int *)((int)pdVar12 + (int)DAT_8c63d144 + iVar2 + -4) + 4) =
               *(undefined2 *)(param_3[2] + iVar7 * 4);
          *(undefined2 *)(*(int *)((int)pdVar12 + iVar4 + iVar2 + -4) + 6) =
               *(undefined2 *)(param_3[2] + iVar7 * 4 + 2);
          iVar7 = (int)DAT_8c63d148;
          **(undefined4 **)((int)pdVar12 + (int)DAT_8c63d146 + iVar2 + -4) =
               *(undefined4 *)(*(int *)((int)pdVar12 + iVar2) * 4 + param_3[1]);
          iVar4 = (int)DAT_8c63d142;
          *(undefined4 *)(*(int *)((int)pdVar12 + (int)DAT_8c63d146 + iVar2 + -4) + 4) =
               *(undefined4 *)(*(int *)((int)pdVar12 + iVar2 + -4) * 4 + param_3[1]);
          iVar4 = (*(code *)PTR_FUN_8c63d158)
                            ((undefined1 *)((int)pdVar12 + iVar4 + iVar2 + -4),
                             (undefined1 *)((int)pdVar12 + iVar7 + iVar2 + -4),param_5);
          uVar23 = SUB84(param_2,0);
          if (iVar4 != 0) {
            uVar20 = (undefined4)((ulonglong)dVar30 >> 0x20);
            dVar19 = dVar30;
            if (in_FPSCR_SZ == '\0') {
              dVar19 = (double)CONCAT44(uVar20,SUB84(dVar18,0));
            }
            pdVar8 = *(double **)((int)pdVar12 + (int)DAT_8c63d148 + iVar2 + -4);
            if (in_FPSCR_SZ == '\0') {
              dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),uVar20);
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),*(undefined4 *)(pdVar8 + 1)
                                       );
            }
            else {
              in_xd2 = pdVar8[1];
              in_xd4 = dVar30;
            }
            if (in_FPSCR_PR == '\0') {
              dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) / SUB84(dVar16,0),
                                        SUB84(dVar19,0));
            }
            else {
              dVar19 = dVar19 / dVar16;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),
                                        *(undefined4 *)((int)pdVar8 + 0x14));
            }
            else {
              in_xd2 = *(double *)((int)pdVar8 + 0x14);
            }
            if (in_FPSCR_PR == '\0') {
              dVar18 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),
                                        SUB84(dVar19,0) / SUB84(dVar16,0));
            }
            else {
              dVar18 = dVar19 / dVar16;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),*(undefined4 *)pdVar8);
              uVar23 = *(undefined4 *)((int)pdVar1 + 0xc);
            }
            else {
              in_xd2 = *pdVar8;
              in_xd6 = *(double *)((int)pdVar1 + 0xc);
            }
            if (in_FPSCR_PR == '\0') {
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),
                                        SUB84(dVar16,0) * SUB84(dVar30,0));
            }
            else {
              dVar16 = dVar16 * dVar30;
            }
            uVar21 = (undefined4)((ulonglong)dVar18 >> 0x20);
            dVar19 = dVar18;
            param_2 = in_xd6;
            if (in_FPSCR_SZ == '\0') {
              dVar19 = (double)CONCAT44(uVar21,extraout_fr1);
              param_2 = (double)CONCAT44(uVar23,uVar23);
            }
            if (in_FPSCR_PR == '\0') {
              param_2 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) * SUB84(dVar16,0) +
                                         (float)((ulonglong)param_2 >> 0x20),SUB84(param_2,0));
            }
            uVar23 = SUB84(dVar18,0);
            if (in_FPSCR_SZ == '\0') {
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),
                                        *(undefined4 *)((int)pdVar8 + 0xc));
              dVar19 = (double)CONCAT44(uVar23,SUB84(dVar19,0));
            }
            else {
              in_xd2 = *(double *)((int)pdVar8 + 0xc);
              dVar19 = in_xd4;
            }
            if (in_FPSCR_PR == '\0') {
              fVar29 = SUB84(dVar16,0) * SUB84(dVar30,0);
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),fVar29);
              param_2 = (double)CONCAT44((int)((ulonglong)param_2 >> 0x20),
                                         (float)((ulonglong)dVar19 >> 0x20) * fVar29 +
                                         SUB84(param_2,0));
            }
            else {
              dVar16 = dVar16 * dVar30;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),
                                        *(undefined4 *)((int)pdVar8 + 4));
              dVar19 = (double)CONCAT44(uVar21,SUB84(dVar19,0));
            }
            else {
              in_xd2 = *(double *)((int)pdVar8 + 4);
              dVar19 = dVar18;
            }
            if (in_FPSCR_PR == '\0') {
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),
                                        SUB84(dVar16,0) * SUB84(dVar27,0));
            }
            else {
              dVar16 = dVar16 * dVar27;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar32 = (double)CONCAT44(*(undefined4 *)(pdVar1 + 2),*(undefined4 *)(pdVar1 + 2));
            }
            else {
              dVar32 = pdVar1[2];
              in_xd8 = dVar32;
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) * SUB84(dVar16,0) +
                                        (float)((ulonglong)dVar32 >> 0x20),SUB84(dVar32,0));
            }
            if (in_FPSCR_SZ == '\0') {
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),*(undefined4 *)(pdVar8 + 2)
                                       );
            }
            else {
              in_xd2 = pdVar8[2];
            }
            if (in_FPSCR_PR == '\0') {
              fVar29 = SUB84(dVar16,0) * SUB84(dVar27,0);
            }
            else {
              fVar29 = SUB84(dVar16 * dVar27,0);
            }
            uVar9 = (undefined4)((ulonglong)dVar32 >> 0x20);
            fVar26 = (float)((ulonglong)param_2 >> 0x20);
            dVar24 = dVar32;
            dVar33 = param_2;
            dVar16 = in_xd4;
            if (in_FPSCR_SZ == '\0') {
              dVar24 = (double)CONCAT44(uVar9,fVar26);
              dVar33 = in_xd10;
              dVar16 = (double)CONCAT44(uVar23,SUB84(dVar19,0));
            }
            fVar22 = SUB84(param_2,0);
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),SUB84(dVar24,0) - fVar22);
            }
            else {
              dVar24 = dVar24 - param_2;
            }
            dVar19 = dVar30;
            if (in_FPSCR_SZ == '\0') {
              dVar19 = (double)CONCAT44(uVar20,fVar29);
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44(uVar9,(float)((ulonglong)dVar16 >> 0x20) * SUB84(dVar19,0) +
                                              SUB84(dVar32,0));
            }
            dVar13 = dVar33;
            if (in_FPSCR_SZ == '\0') {
              dVar13 = (double)CONCAT44(SUB84(dVar24,0),SUB84(dVar16,0));
            }
            fVar29 = SUB84(dVar32,0);
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) - fVar29,SUB84(dVar24,0))
              ;
            }
            else {
              dVar24 = dVar24 - dVar32;
            }
            fVar25 = (float)((ulonglong)dVar24 >> 0x20);
            dVar16 = dVar24;
            if (in_FPSCR_SZ == '\0') {
              dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),fVar25);
              dVar16 = in_xd2;
            }
            fVar14 = (float)((ulonglong)dVar13 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              fVar15 = SQRT(fVar14 * SUB84(dVar24,0) + SUB84(dVar19,0) * fVar25);
              fVar17 = (float)((ulonglong)dVar19 >> 0x20) / fVar15;
              dVar19 = (double)CONCAT44(fVar17,fVar15);
              dVar24 = (double)CONCAT44(fVar25 * fVar17,SUB84(dVar24,0));
            }
            else {
              dVar19 = SQRT(dVar19 * dVar24) / SQRT(dVar19 * dVar24);
              fVar17 = (float)((ulonglong)dVar19 >> 0x20);
              dVar24 = dVar24 * dVar19;
            }
            dVar31 = dVar19;
            if (in_FPSCR_SZ == '\0') {
              dVar19 = (double)CONCAT44(fVar17,fVar17);
              dVar31 = dVar16;
            }
            fVar25 = SUB84(dVar19,0);
            if (in_FPSCR_PR == '\0') {
              dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),SUB84(dVar24,0) * fVar25);
            }
            else {
              dVar24 = dVar24 * dVar19;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)pdVar12 + iVar2 + 0x20) = (int)((ulonglong)dVar19 >> 0x20);
            }
            else {
              *(double *)((int)pdVar12 + iVar2 + 0x20) = dVar19;
            }
            fVar17 = (float)((ulonglong)dVar27 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              dVar16 = (double)CONCAT44(0x40000000,fVar25);
              dVar19 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * fVar17,
                                        SUB84(dVar24,0) * fVar17);
            }
            else {
              dVar16 = (double)CONCAT44(0x3f800000,fVar25) + (double)CONCAT44(0x3f800000,fVar25);
              dVar19 = dVar24 * dVar27 * dVar27;
            }
            fVar25 = (float)((ulonglong)dVar19 >> 0x20);
            dVar24 = dVar19;
            if (in_FPSCR_SZ == '\0') {
              dVar13 = (double)CONCAT44(fVar14,fVar25);
              dVar24 = in_xd0;
            }
            fVar14 = (float)((ulonglong)dVar16 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              fVar17 = SUB84(dVar13,0) * fVar14;
            }
            else {
              fVar17 = SUB84(dVar13 * dVar16,0);
            }
            fVar15 = -SUB84(dVar19,0);
            dVar19 = (double)CONCAT44(fVar25,fVar15);
            in_xd2 = dVar33;
            if (in_FPSCR_SZ == '\0') {
              dVar16 = (double)CONCAT44(fVar14,fVar15);
              in_xd2 = dVar31;
            }
            if (in_FPSCR_PR == '\0') {
              fVar14 = SUB84(dVar16,0) * (float)((ulonglong)dVar16 >> 0x20);
            }
            else {
              fVar14 = SUB84(dVar16 * dVar16,0);
            }
            if (in_FPSCR_SZ == '\0') {
              *(float *)((int)pdVar12 + iVar2 + 0x34) = fVar17;
              *(float *)((int)pdVar12 + iVar2 + 0x40) = fVar14;
            }
            else {
              *(double *)((int)pdVar12 + iVar2 + 0x34) = dVar24;
              *(double *)((int)pdVar12 + iVar2 + 0x40) = in_xd2;
            }
            psVar6 = *(short **)((int)pdVar12 + (int)DAT_8c63d14a + iVar2 + -4);
            if (in_FPSCR_SZ == '\0') {
              fVar14 = *(float *)((int)pdVar12 + iVar2 + 0x10);
            }
            else {
              fVar14 = (float)((ulonglong)*(undefined8 *)((int)pdVar12 + iVar2 + 0x10) >> 0x20);
            }
            dVar16 = (double)CONCAT44(fVar14,(float)(int)*psVar6);
            if (in_FPSCR_PR == '\0') {
              fVar14 = (float)(int)*psVar6 * fVar14;
            }
            else {
              fVar14 = SUB84(dVar16 * dVar16,0);
            }
            if (in_FPSCR_SZ == '\0') {
              *(float *)((int)pdVar12 + iVar2 + 0x38) = fVar14;
            }
            else {
              *(double *)((int)pdVar12 + iVar2 + 0x38) = in_xd2;
            }
            if (in_FPSCR_SZ == '\0') {
              fVar14 = *(float *)((int)pdVar12 + iVar2 + 0x10);
            }
            else {
              fVar14 = (float)((ulonglong)*(undefined8 *)((int)pdVar12 + iVar2 + 0x10) >> 0x20);
            }
            dVar16 = (double)CONCAT44(fVar14,(float)(int)psVar6[1]);
            if (in_FPSCR_PR == '\0') {
              fVar14 = (float)(int)psVar6[1] * fVar14;
            }
            else {
              fVar14 = SUB84(dVar16 * dVar16,0);
            }
            if (in_FPSCR_SZ == '\0') {
              *(float *)((int)pdVar12 + iVar2 + 0x3c) = fVar14;
            }
            else {
              *(double *)((int)pdVar12 + iVar2 + 0x3c) = in_xd2;
            }
            if (in_FPSCR_SZ == '\0') {
              fVar14 = *(float *)((int)pdVar12 + iVar2 + 0x10);
            }
            else {
              fVar14 = (float)((ulonglong)*(undefined8 *)((int)pdVar12 + iVar2 + 0x10) >> 0x20);
            }
            dVar16 = (double)CONCAT44(fVar14,(float)(int)psVar6[2]);
            if (in_FPSCR_PR == '\0') {
              fVar14 = (float)(int)psVar6[2] * fVar14;
            }
            else {
              fVar14 = SUB84(dVar16 * dVar16,0);
            }
            if (in_FPSCR_SZ == '\0') {
              *(float *)((int)pdVar12 + iVar2 + 0x30) = fVar14;
            }
            else {
              *(double *)((int)pdVar12 + iVar2 + 0x30) = in_xd2;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar16 = (double)CONCAT44(*(undefined4 *)((int)pdVar12 + iVar2 + 0x10),
                                        (float)(int)psVar6[3]);
            }
            else {
              dVar16 = *(double *)((int)pdVar12 + iVar2 + 0x10);
            }
            if (in_FPSCR_PR == '\0') {
              fVar14 = (float)((ulonglong)dVar16 >> 0x20);
              dVar16 = (double)CONCAT44(fVar14,SUB84(dVar16,0) * fVar14);
            }
            else {
              dVar16 = dVar16 * dVar16;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)pdVar12 + iVar2 + 0x2c) = SUB84(dVar16,0);
            }
            else {
              *(double *)((int)pdVar12 + iVar2 + 0x2c) = in_xd2;
            }
            uVar20 = **(undefined4 **)((int)pdVar12 + (int)DAT_8c63d14c + iVar2 + -4);
            uVar9 = (*(undefined4 **)((int)pdVar12 + (int)DAT_8c63d14c + iVar2 + -4))[1];
            *(undefined4 *)(pdVar3 + 3) = uVar20;
            fVar14 = (float)((ulonglong)dVar32 >> 0x20);
            if (in_FPSCR_SZ == '\0') {
              *(undefined4 *)((int)pdVar3 + 0x14) = *(undefined4 *)((int)pdVar12 + iVar2 + 0x3c);
              *(undefined4 *)(pdVar3 + 2) = *(undefined4 *)((int)pdVar12 + iVar2 + 0x38);
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),fVar14);
              pdVar3 = (double *)((int)pdVar3 + 0xc);
              *(undefined4 *)pdVar3 = uVar21;
            }
            else {
              pdVar3[2] = *(double *)((int)pdVar12 + iVar2 + 0x3c);
              pdVar3[1] = *(double *)((int)pdVar12 + iVar2 + 0x38);
              *pdVar3 = dVar18;
              in_xd2 = dVar32;
            }
            if (in_FPSCR_PR == '\0') {
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),SUB84(dVar16,0) - fVar15);
            }
            else {
              dVar16 = dVar16 - dVar19;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar3 = (double *)((int)pdVar3 + -4);
              *(int *)pdVar3 = SUB84(dVar16,0);
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),fVar26);
            }
            else {
              pdVar3 = pdVar3 + -1;
              *pdVar3 = in_xd2;
              in_xd2 = param_2;
            }
            if (in_FPSCR_PR == '\0') {
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),SUB84(dVar16,0) - fVar25);
            }
            else {
              dVar16 = dVar16 - dVar19;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar3 = (double *)((int)pdVar3 + -4);
              *(int *)pdVar3 = SUB84(dVar16,0);
            }
            else {
              pdVar3 = pdVar3 + -1;
              *pdVar3 = in_xd2;
            }
            *(undefined4 *)((int)pdVar3 + -4) = uVar28;
            *(undefined4 *)((int)pdVar3 + 0x34) = uVar9;
            if (in_FPSCR_SZ == '\0') {
              *(undefined4 *)(pdVar3 + 6) = *(undefined4 *)((int)pdVar12 + iVar2 + 0x2c);
              puVar11 = (undefined8 *)((int)pdVar3 + 0x2c);
              *(undefined4 *)puVar11 = *(undefined4 *)((int)pdVar12 + iVar2 + 0x30);
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),fVar29);
            }
            else {
              *(undefined8 *)((int)pdVar3 + 0x2c) = *(undefined8 *)((int)pdVar12 + iVar2 + 0x2c);
              puVar11 = (undefined8 *)((int)pdVar3 + 0x24);
              *puVar11 = *(undefined8 *)((int)pdVar12 + iVar2 + 0x30);
              in_xd2 = in_xd8;
            }
            if (in_FPSCR_PR == '\0') {
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),SUB84(dVar16,0) - fVar15);
            }
            else {
              dVar16 = dVar16 - dVar19;
            }
            if (in_FPSCR_SZ == '\0') {
              *(undefined4 *)((int)puVar11 + -4) = uVar23;
              pdVar3 = (double *)(puVar11 + -1);
              *(int *)pdVar3 = SUB84(dVar16,0);
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),fVar22);
            }
            else {
              puVar11[-1] = in_xd4;
              pdVar3 = (double *)(puVar11 + -2);
              *pdVar3 = in_xd2;
              in_xd2 = in_xd6;
            }
            if (in_FPSCR_PR == '\0') {
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),SUB84(dVar16,0) - fVar25);
            }
            else {
              dVar16 = dVar16 - dVar19;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar3 = (double *)((int)pdVar3 + -4);
              *(int *)pdVar3 = SUB84(dVar16,0);
            }
            else {
              pdVar3 = pdVar3 + -1;
              *pdVar3 = in_xd2;
            }
            *(undefined4 *)((int)pdVar3 + -4) = uVar28;
            *(undefined4 *)((int)pdVar3 + 0x34) = uVar20;
            if (in_FPSCR_SZ == '\0') {
              *(undefined4 *)(pdVar3 + 6) = *(undefined4 *)((int)pdVar12 + iVar2 + 0x3c);
              *(undefined4 *)((int)pdVar3 + 0x2c) = *(undefined4 *)((int)pdVar12 + iVar2 + 0x38);
              pdVar3 = pdVar3 + 5;
              *(undefined4 *)pdVar3 = uVar21;
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),
                                        *(undefined4 *)((int)pdVar12 + iVar2 + 0x40));
            }
            else {
              *(undefined8 *)((int)pdVar3 + 0x2c) = *(undefined8 *)((int)pdVar12 + iVar2 + 0x3c);
              *(undefined8 *)((int)pdVar3 + 0x24) = *(undefined8 *)((int)pdVar12 + iVar2 + 0x38);
              pdVar3 = (double *)((int)pdVar3 + 0x1c);
              *pdVar3 = dVar18;
              in_xd2 = *(double *)((int)pdVar12 + iVar2 + 0x40);
            }
            if (in_FPSCR_PR == '\0') {
              dVar32 = (double)CONCAT44(fVar14 + SUB84(dVar16,0),fVar29);
            }
            else {
              dVar32 = dVar32 + dVar16;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar3 = (double *)((int)pdVar3 + -4);
              *(int *)pdVar3 = (int)((ulonglong)dVar32 >> 0x20);
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),
                                        *(undefined4 *)((int)pdVar12 + iVar2 + 0x34));
            }
            else {
              pdVar3 = pdVar3 + -1;
              *pdVar3 = dVar32;
              in_xd2 = *(double *)((int)pdVar12 + iVar2 + 0x34);
            }
            if (in_FPSCR_PR == '\0') {
              param_2 = (double)CONCAT44(fVar26 + SUB84(dVar16,0),fVar22);
            }
            else {
              param_2 = param_2 + dVar16;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar3 = (double *)((int)pdVar3 + -4);
              *(int *)pdVar3 = (int)((ulonglong)param_2 >> 0x20);
            }
            else {
              pdVar3 = pdVar3 + -1;
              *pdVar3 = param_2;
            }
            *(undefined4 *)((int)pdVar3 + -4) = uVar28;
            uVar20 = DAT_8c63d228;
            *(undefined4 *)((int)pdVar3 + 0x34) = uVar9;
            if (in_FPSCR_SZ == '\0') {
              *(undefined4 *)(pdVar3 + 6) = *(undefined4 *)((int)pdVar12 + iVar2 + 0x2c);
              *(undefined4 *)((int)pdVar3 + 0x2c) = *(undefined4 *)((int)pdVar12 + iVar2 + 0x30);
              pdVar3 = pdVar3 + 5;
              *(undefined4 *)pdVar3 = uVar23;
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),
                                        *(undefined4 *)((int)pdVar12 + iVar2 + 0x40));
            }
            else {
              *(undefined8 *)((int)pdVar3 + 0x2c) = *(undefined8 *)((int)pdVar12 + iVar2 + 0x2c);
              *(undefined8 *)((int)pdVar3 + 0x24) = *(undefined8 *)((int)pdVar12 + iVar2 + 0x30);
              pdVar3 = (double *)((int)pdVar3 + 0x1c);
              *pdVar3 = in_xd4;
              in_xd2 = *(double *)((int)pdVar12 + iVar2 + 0x40);
            }
            if (in_FPSCR_PR == '\0') {
              fVar29 = SUB84(dVar32,0) + SUB84(dVar16,0);
            }
            else {
              fVar29 = SUB84(dVar32 + dVar16,0);
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar3 = (double *)((int)pdVar3 + -4);
              *(float *)pdVar3 = fVar29;
              dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),
                                        *(undefined4 *)((int)pdVar12 + iVar2 + 0x34));
            }
            else {
              pdVar3 = pdVar3 + -1;
              *pdVar3 = in_xd8;
              in_xd2 = *(double *)((int)pdVar12 + iVar2 + 0x34);
            }
            if (in_FPSCR_PR == '\0') {
              param_2 = (double)(ulonglong)(uint)(SUB84(param_2,0) + SUB84(dVar16,0));
            }
            else {
              param_2 = param_2 + dVar16;
            }
            if (in_FPSCR_SZ == '\0') {
              pdVar3 = (double *)((int)pdVar3 + -4);
              *(int *)pdVar3 = SUB84(param_2,0);
            }
            else {
              pdVar3 = pdVar3 + -1;
              *pdVar3 = in_xd6;
            }
            *(undefined4 *)((int)pdVar3 + -4) = uVar20;
            pdVar3 = (double *)((int)pdVar3 + 0x1c);
            in_xd0 = dVar24;
            in_xd10 = dVar33;
          }
        }
        *(int *)((int)pdVar12 + iVar2 + 4) = *(int *)((int)pdVar12 + iVar2 + 4) + 1;
        *(int *)((int)pdVar12 + iVar2 + -4) = *(int *)((int)pdVar12 + iVar2 + -4) + iVar10;
        *(int *)((int)pdVar12 + iVar2 + 0xc) = *(int *)((int)pdVar12 + iVar2 + 0xc) + iVar10;
        *(int *)((int)pdVar12 + iVar2 + 8) = *(int *)((int)pdVar12 + iVar2 + 8) + iVar10;
      } while (*(int *)((int)pdVar12 + iVar2 + 4) < param_4);
    }
  }
  (*(code *)PTR_FUN_8c63d40c)(pdVar3);
  (*(code *)PTR_FUN_8c63d410)();
  return;
}

