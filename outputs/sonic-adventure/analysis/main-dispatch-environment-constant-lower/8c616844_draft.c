/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c616a58) */
/* WARNING: Removing unreachable block (ram,0x8c616b4e) */
/* WARNING: Restarted to delay deadcode elimination for space: register */

void FUN_8c616844(double param_1,double param_2,double param_3,undefined4 param_4)

{
  ushort uVar1;
  undefined8 *puVar2;
  int iVar3;
  undefined *puVar4;
  int *piVar5;
  int iVar6;
  int iVar7;
  undefined4 *puVar8;
  double *unaff_r8;
  double *pdVar9;
  double *pdVar10;
  double *pdVar11;
  double *pdVar12;
  double *pdVar13;
  int iVar14;
  ushort *puVar15;
  ushort *puVar16;
  ushort *puVar17;
  double *pdVar18;
  float fVar19;
  float fVar20;
  float fVar22;
  double dVar23;
  float fVar24;
  float fVar25;
  double in_dr2;
  uint uVar26;
  float fVar27;
  double dVar28;
  undefined4 uVar30;
  undefined4 uVar31;
  double dVar32;
  undefined8 in_dr12;
  double dVar33;
  undefined4 uVar34;
  double in_dr14;
  undefined8 uVar35;
  double in_xd0;
  double dVar36;
  float fVar37;
  double in_xd2;
  double dVar38;
  double dVar39;
  float fVar40;
  double in_xd4;
  float fVar41;
  float fVar42;
  undefined8 in_xd6;
  float fVar43;
  undefined8 in_xd8;
  float fVar44;
  float in_xf10;
  float fVar45;
  double in_xd12;
  float fVar46;
  float fVar47;
  double in_xd14;
  bool bVar48;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  int aiStack_48 [4];
  undefined1 local_38 [16];
  double dStack_28;
  undefined8 uStack_20;
  float fVar21;
  undefined4 uVar29;
  
  if (in_FPSCR_SZ == '\0') {
    uStack_20 = (double)CONCAT44(SUB84(in_dr14,0),(undefined4)uStack_20);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_20._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
    pdVar18 = &dStack_28;
    dStack_28 = (double)CONCAT44((int)in_dr12,(int)((ulonglong)in_dr12 >> 0x20));
  }
  else {
    pdVar18 = (double *)local_38;
    uStack_20 = in_xd14;
    dStack_28 = in_dr14;
  }
  *(undefined4 *)((int)pdVar18 + -4) = in_PR;
  dVar33 = (double)CONCAT44(0x3f800000,(int)in_dr12);
  *(undefined4 *)((int)pdVar18 + -0x44) = param_4;
  dVar23 = (double)(*(code *)PTR_FUN_8c6168a0)();
  puVar15 = *(ushort **)(*(int *)((int)pdVar18 + -0x44) + 4);
  *(uint *)((int)pdVar18 + -0x38) = (uint)*(ushort *)(*(int *)((int)pdVar18 + -0x44) + 2);
  iVar14 = *DAT_8c6168a4;
  *(undefined4 *)((int)pdVar18 + -0x44) = 0;
  do {
    uVar34 = SUB84(in_dr14,0);
    uVar1 = *puVar15;
    puVar16 = puVar15 + 1;
    *(uint *)((int)pdVar18 + -0x48) = (uint)uVar1;
    uVar30 = (undefined4)((ulonglong)dVar33 >> 0x20);
    dVar28 = dVar33;
    if (in_FPSCR_SZ == '\0') {
      dVar28 = in_xd14;
      uVar34 = uVar30;
    }
    in_xd14 = dVar28;
    if ((uVar1 & DAT_8c6169c4) != 0) {
      if (in_FPSCR_PR == '\0') {
        param_1 = (double)CONCAT44(0xbf800000,SUB84(param_1,0));
      }
      else {
        param_1 = -(double)CONCAT44(0x3f800000,SUB84(param_1,0));
      }
      *(uint *)((int)pdVar18 + -0x48) = *(uint *)((int)pdVar18 + -0x48) & (int)DAT_8c6169c0;
      in_xd14 = param_1;
      if (in_FPSCR_SZ == '\0') {
        uVar34 = (undefined4)((ulonglong)param_1 >> 0x20);
        in_xd14 = dVar28;
      }
    }
    *(int *)((int)pdVar18 + -0x44) = *(int *)((int)pdVar18 + -0x44) + *(int *)((int)pdVar18 + -0x48)
    ;
    *(int *)((int)pdVar18 + -0x48) = *(int *)((int)pdVar18 + -0x48) + -2;
    puVar17 = puVar15 + 2;
    puVar15 = puVar15 + 3;
    pdVar12 = (double *)((uint)*puVar17 * 0x20 + iVar14);
    pdVar11 = (double *)((uint)*puVar16 * 0x20 + iVar14);
    pdVar9 = unaff_r8;
    if (in_FPSCR_SZ == '\0') {
      dVar33 = (double)CONCAT44(uVar30,*(undefined4 *)(pdVar12 + 2));
      param_2 = (double)CONCAT44(*(undefined4 *)(pdVar11 + 2),SUB84(param_2,0));
      in_dr14 = (double)CONCAT44(*(undefined4 *)((int)pdVar12 + 0x14),uVar34);
      param_1 = (double)(ulonglong)*(uint *)((int)pdVar11 + 0x14);
    }
    else {
      in_xd12 = pdVar12[2];
      param_2 = pdVar11[2];
      in_dr14 = *(double *)((int)pdVar12 + 0x14);
      in_xd4 = *(double *)((int)pdVar11 + 0x14);
    }
    do {
      uVar1 = *puVar15;
      puVar15 = puVar15 + 1;
      pdVar13 = (double *)((uint)uVar1 * 0x20 + iVar14);
      *(undefined4 *)((int)pdVar18 + -0x3c) = 0;
      uVar34 = (undefined4)((ulonglong)dVar33 >> 0x20);
      dVar28 = dVar33;
      if (in_FPSCR_SZ == '\0') {
        dVar28 = (double)CONCAT44(uVar34,SUB84(param_1,0));
      }
      if ((*(uint *)((int)pdVar11 + 0xc) & *(uint *)((int)pdVar12 + 0xc) &
           *(uint *)((int)pdVar13 + 0xc) & 0x3f) == 0) {
        if (((*(uint *)((int)pdVar11 + 0xc) | *(uint *)((int)pdVar12 + 0xc) |
             *(uint *)((int)pdVar13 + 0xc)) & 1) == 0) {
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44(*(undefined4 *)((int)pdVar13 + 0x14),SUB84(dVar33,0));
          }
          else {
            dVar32 = *(double *)((int)pdVar13 + 0x14);
            in_xd2 = in_xd12;
          }
          fVar27 = (float)((ulonglong)in_dr14 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) - fVar27,
                                      SUB84(dVar32,0) - (float)((ulonglong)param_2 >> 0x20));
          }
          else {
            dVar32 = (dVar32 - param_2) - in_dr14;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(undefined4 *)(pdVar13 + 2))
            ;
          }
          else {
            in_xd0 = pdVar13[2];
          }
          if (in_FPSCR_PR == '\0') {
            dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),
                                      SUB84(dVar23,0) - SUB84(dVar33,0));
            fVar19 = SUB84(dVar32,0) * (float)((ulonglong)dVar32 >> 0x20);
          }
          else {
            dVar23 = dVar23 - dVar33;
            fVar19 = SUB84(dVar32 * dVar32,0);
          }
          dVar32 = in_dr14;
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44(fVar27,fVar19);
          }
          if (in_FPSCR_PR == '\0') {
            fVar27 = ((float)((ulonglong)dVar32 >> 0x20) - SUB84(dVar28,0)) * SUB84(dVar23,0);
            in_dr2 = (double)CONCAT44(fVar27,SUB84(dVar32,0) - fVar27);
          }
          else {
            in_dr2 = (dVar32 - dVar28) * dVar23;
            in_dr2 = in_dr2 - in_dr2;
          }
          dVar32 = in_xd2;
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44(SUB84(in_dr2,0),SUB84(dVar28,0));
          }
          if (in_FPSCR_PR == '\0') {
            dVar28 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) * SUB84(in_dr14,0),
                                      SUB84(dVar32,0));
          }
          else {
            dVar28 = dVar32 * in_dr14;
          }
        }
        else {
          puVar8 = (undefined4 *)((int)pdVar18 + -0x10);
          *puVar8 = pdVar11;
          *(double **)((int)pdVar18 + -0xc) = pdVar12;
          *(double **)((int)pdVar18 + -8) = pdVar13;
          iVar7 = 0;
          while( true ) {
            iVar3 = DAT_8c6169c8;
            pdVar10 = (double *)((int)pdVar18 + -0x34);
            uVar30 = (undefined4)((ulonglong)in_dr2 >> 0x20);
            if (2 < iVar7) break;
            piVar5 = puVar8 + iVar7;
            *(int **)((int)pdVar18 + -0x4c) = piVar5;
            *(int *)((int)pdVar18 + -0x50) = iVar7 * 0xc + (int)pdVar10;
            iVar7 = iVar7 + 1;
            if ((*(uint *)(*piVar5 + 0xc) & 1) == 0) {
              dVar32 = dVar33;
              if (in_FPSCR_SZ == '\0') {
                dVar32 = (double)CONCAT44(uVar34,SUB84(dVar28,0));
              }
              iVar6 = **(int **)((int)pdVar18 + -0x4c);
              *(int *)((int)pdVar18 + -0x40) = iVar6;
              if (in_FPSCR_SZ == '\0') {
                in_dr2 = (double)CONCAT44(uVar30,*(undefined4 *)(iVar6 + 0x18));
              }
              else {
                in_xd2 = *(double *)(iVar6 + 0x18);
              }
              if (in_FPSCR_PR == '\0') {
                dVar28 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) / SUB84(in_dr2,0),
                                          SUB84(dVar32,0));
              }
              else {
                dVar28 = dVar32 / in_dr2;
              }
              if (in_FPSCR_SZ == '\0') {
                in_dr2 = (double)(ulonglong)*(uint *)(DAT_8c6169c8 + 0xc);
              }
              else {
                in_xd2 = *(double *)(DAT_8c6169c8 + 0xc);
              }
              if (in_FPSCR_SZ == '\0') {
                dVar32 = (double)CONCAT44(*(undefined4 *)(*(int *)((int)pdVar18 + -0x40) + 0x10),
                                          SUB84(in_dr2,0));
              }
              else {
                dVar32 = *(double *)(*(int *)((int)pdVar18 + -0x40) + 0x10);
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
                                          *(undefined4 *)DAT_8c6169cc);
              }
              else {
                in_xd2 = *DAT_8c6169cc;
              }
              fVar27 = (float)((ulonglong)dVar28 >> 0x20);
              if (in_FPSCR_PR == '\0') {
                dVar32 = (double)CONCAT44(((float)((ulonglong)dVar32 >> 0x20) * fVar27) /
                                          SUB84(dVar32,0),SUB84(dVar32,0));
              }
              else {
                dVar32 = (dVar32 * dVar28) / (dVar32 * dVar28);
              }
              uVar30 = (undefined4)((ulonglong)dVar32 >> 0x20);
              if (in_FPSCR_SZ == '\0') {
                *(undefined4 *)*(double **)((int)pdVar18 + -0x50) = uVar30;
              }
              else {
                **(double **)((int)pdVar18 + -0x50) = dVar32;
              }
              if (in_FPSCR_SZ == '\0') {
                dVar32 = (double)CONCAT44(uVar30,*(undefined4 *)(iVar3 + 0x10));
              }
              else {
                in_xd2 = *(double *)(iVar3 + 0x10);
              }
              if (in_FPSCR_SZ == '\0') {
                dVar32 = (double)CONCAT44(*(undefined4 *)(**(int **)((int)pdVar18 + -0x4c) + 0x14),
                                          SUB84(dVar32,0));
              }
              else {
                dVar32 = *(double *)(**(int **)((int)pdVar18 + -0x4c) + 0x14);
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
                                          *(undefined4 *)DAT_8c6169d0);
              }
              else {
                in_xd2 = *DAT_8c6169d0;
              }
              if (in_FPSCR_PR == '\0') {
                in_dr2 = (double)CONCAT44(((float)((ulonglong)dVar32 >> 0x20) * fVar27) /
                                          SUB84(dVar32,0),SUB84(dVar32,0));
              }
              else {
                in_dr2 = (dVar32 * dVar28) / (dVar32 * dVar28);
              }
              if (in_FPSCR_SZ == '\0') {
                *(int *)(*(int *)((int)pdVar18 + -0x50) + 4) = (int)((ulonglong)in_dr2 >> 0x20);
              }
              else {
                *(double *)(*(int *)((int)pdVar18 + -0x50) + 4) = in_dr2;
              }
              if (in_FPSCR_SZ == '\0') {
                *(float *)(*(int *)((int)pdVar18 + -0x50) + 8) = fVar27;
              }
              else {
                *(double *)(*(int *)((int)pdVar18 + -0x50) + 8) = dVar28;
              }
            }
            else {
              if (in_FPSCR_SZ == '\0') {
                uVar29 = *(undefined4 *)(**(int **)((int)pdVar18 + -0x4c) + 0x10);
                in_dr2 = (double)CONCAT44(uVar30,uVar29);
                *(undefined4 *)*(double **)((int)pdVar18 + -0x50) = uVar29;
              }
              else {
                in_xd2 = *(double *)(**(int **)((int)pdVar18 + -0x4c) + 0x10);
                **(double **)((int)pdVar18 + -0x50) = in_xd2;
              }
              if (in_FPSCR_SZ == '\0') {
                uVar30 = *(undefined4 *)(**(int **)((int)pdVar18 + -0x4c) + 0x14);
                in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),uVar30);
                *(undefined4 *)(*(int *)((int)pdVar18 + -0x50) + 4) = uVar30;
              }
              else {
                in_xd2 = *(double *)(**(int **)((int)pdVar18 + -0x4c) + 0x14);
                *(double *)(*(int *)((int)pdVar18 + -0x50) + 4) = in_xd2;
              }
              if (in_FPSCR_SZ == '\0') {
                uVar30 = *(undefined4 *)(**(int **)((int)pdVar18 + -0x4c) + 0x18);
                in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),uVar30);
                *(undefined4 *)(*(int *)((int)pdVar18 + -0x50) + 8) = uVar30;
              }
              else {
                in_xd2 = *(double *)(**(int **)((int)pdVar18 + -0x4c) + 0x18);
                *(double *)(*(int *)((int)pdVar18 + -0x50) + 8) = in_xd2;
              }
            }
          }
          if (in_FPSCR_SZ == '\0') {
            uVar29 = *(undefined4 *)((int)pdVar18 + -0x2c);
            dVar23 = (double)CONCAT44(uVar29,SUB84(dVar23,0));
            param_2 = (double)CONCAT44(uVar29,SUB84(param_2,0));
          }
          else {
            dVar23 = *(double *)((int)pdVar18 + -0x2c);
            uVar29 = (undefined4)((ulonglong)dVar23 >> 0x20);
            param_2 = dVar23;
          }
          puVar2 = (undefined8 *)((int)pdVar18 + -0x28);
          *(undefined8 **)((int)pdVar18 + -0x50) = puVar2;
          if (in_FPSCR_SZ == '\0') {
            uVar31 = *(undefined4 *)((int)pdVar18 + -0x30);
            param_3 = (double)CONCAT44((int)((ulonglong)param_3 >> 0x20),
                                       *(undefined4 *)((int)pdVar18 + -0x20));
            in_dr2 = (double)CONCAT44(uVar30,uVar31);
            dVar28 = (double)CONCAT44(uVar31,*(undefined4 *)((int)pdVar18 + -0x24));
          }
          else {
            in_xd2 = *(double *)((int)pdVar18 + -0x30);
            in_xd4 = *(double *)((int)pdVar18 + -0x24);
            in_xd8 = *(undefined8 *)((int)pdVar18 + -0x20);
            uVar31 = (undefined4)((ulonglong)in_xd2 >> 0x20);
            dVar28 = in_xd2;
          }
          fVar27 = SUB84(dVar28,0);
          if (in_FPSCR_PR == '\0') {
            dVar23 = (double)CONCAT44((float)((ulonglong)dVar23 >> 0x20) * fVar27,SUB84(dVar23,0));
            fVar19 = SUB84(in_dr2,0) * SUB84(param_3,0);
          }
          else {
            dVar23 = dVar23 * dVar28;
            fVar19 = SUB84(in_dr2 * param_3,0);
          }
          if (in_FPSCR_SZ == '\0') {
            uVar30 = *(undefined4 *)pdVar10;
            param_2 = (double)CONCAT44(uVar29,*(undefined4 *)puVar2);
            dVar32 = (double)CONCAT44(uVar30,fVar19);
            param_3 = (double)CONCAT44(uVar30,SUB84(param_3,0));
          }
          else {
            dVar32 = *pdVar10;
            in_xd6 = *puVar2;
            uVar30 = (undefined4)((ulonglong)dVar32 >> 0x20);
            param_3 = dVar32;
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) * SUB84(param_3,0),
                                      SUB84(dVar32,0));
          }
          else {
            dVar32 = dVar32 * param_3;
          }
          dVar36 = dVar28;
          if (in_FPSCR_SZ == '\0') {
            dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),uVar31);
            dVar36 = in_xd0;
          }
          if (in_FPSCR_PR == '\0') {
            dVar23 = (double)CONCAT44((float)((ulonglong)dVar23 >> 0x20) - SUB84(dVar32,0),
                                      SUB84(dVar23,0));
          }
          else {
            dVar23 = dVar23 - dVar32;
          }
          dVar38 = param_2;
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                      (int)((ulonglong)param_2 >> 0x20));
            dVar38 = in_xd2;
          }
          if (in_FPSCR_PR == '\0') {
            fVar19 = SUB84(dVar32,0) * SUB84(param_2,0);
            dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) - fVar19,fVar19);
          }
          else {
            dVar32 = dVar32 * param_2 - dVar32 * param_2;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                      *(undefined4 *)((int)pdVar18 + -0x18));
          }
          else {
            dVar38 = *(double *)((int)pdVar18 + -0x18);
          }
          if (in_FPSCR_PR == '\0') {
            fVar19 = SUB84(dVar32,0) * (float)((ulonglong)dVar32 >> 0x20);
          }
          else {
            fVar19 = SUB84(dVar32 * dVar32,0);
          }
          if (in_FPSCR_SZ == '\0') {
            uVar35 = CONCAT44(*(undefined4 *)((int)pdVar18 + -0x1c),fVar19);
          }
          else {
            uVar35 = *(undefined8 *)((int)pdVar18 + -0x1c);
          }
          if (in_FPSCR_PR == '\0') {
            fVar19 = (float)((ulonglong)uVar35 >> 0x20);
            uVar35 = CONCAT44(fVar19,(float)((ulonglong)dVar23 >> 0x20) * fVar19 + (float)uVar35);
          }
          dVar32 = dVar38;
          dVar39 = param_3;
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44((int)uVar35,uVar30);
            dVar39 = dVar38;
          }
          if (in_FPSCR_PR == '\0') {
            fVar19 = SUB84(dVar32,0) * fVar27;
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),fVar19);
            fVar19 = SUB84(dVar23,0) * SUB84(param_2,0) - fVar19;
          }
          else {
            dVar32 = dVar32 * dVar28;
            fVar19 = SUB84(dVar23 * param_2 - dVar32,0);
          }
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                      *(undefined4 *)((int)pdVar18 + -0x14));
            dVar23 = (double)CONCAT44(fVar19,fVar19);
          }
          else {
            dVar39 = *(double *)((int)pdVar18 + -0x14);
            dVar23 = dVar36;
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((float)((ulonglong)dVar23 >> 0x20) * SUB84(dVar32,0) +
                                      (float)((ulonglong)dVar32 >> 0x20),SUB84(dVar32,0));
          }
          in_xd2 = in_xd14;
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),SUB84(in_dr14,0));
            in_xd2 = dVar39;
          }
          uVar30 = (undefined4)((ulonglong)dVar32 >> 0x20);
          fVar19 = -SUB84(dVar32,0);
          dVar28 = (double)CONCAT44(uVar30,fVar19);
          dVar32 = dVar28;
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44(uVar30,fVar27);
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) * fVar19,SUB84(dVar32,0));
          }
          else {
            dVar32 = dVar32 * dVar28;
          }
          in_dr2 = (double)(ulonglong)(uint)fVar19;
          if (in_FPSCR_PR == '\0') {
            bVar48 = 0.0 < (float)((ulonglong)dVar32 >> 0x20);
          }
          else {
            bVar48 = in_dr2 < dVar32;
          }
          dVar28 = dVar32;
          in_xd0 = dVar36;
          if ((!bVar48) &&
             (*(undefined4 *)((int)pdVar18 + -0x3c) = 1, dVar28 = dVar33, in_FPSCR_SZ == '\0')) {
            dVar28 = (double)CONCAT44(uVar34,SUB84(dVar32,0));
          }
        }
      }
      uVar29 = DAT_8c616c00;
      uVar30 = DAT_8c616bfc;
      uVar26 = (uint)((ulonglong)in_dr2 >> 0x20);
      fVar27 = (float)((ulonglong)dVar28 >> 0x20);
      fVar19 = SUB84(in_xd0,0);
      fVar37 = (float)((ulonglong)in_xd0 >> 0x20);
      fVar22 = SUB84(in_xd2,0);
      fVar40 = SUB84(in_xd4,0);
      fVar41 = (float)((ulonglong)in_xd4 >> 0x20);
      fVar42 = (float)in_xd6;
      fVar43 = (float)in_xd8;
      fVar44 = (float)((ulonglong)in_xd8 >> 0x20);
      fVar45 = SUB84(in_xd12,0);
      fVar46 = (float)((ulonglong)in_xd12 >> 0x20);
      fVar47 = SUB84(in_xd14,0);
      uVar34 = SUB84(param_2,0);
      uVar31 = SUB84(dVar28,0);
      if (pdVar9 < unaff_r8) {
        if (in_FPSCR_PR == '\0') {
          bVar48 = fVar27 < 0.0;
        }
        else {
          bVar48 = dVar28 < (double)((ulonglong)uVar26 << 0x20);
        }
        if (bVar48) {
LAB_8c616af4:
          uVar30 = DAT_8c616c00;
          uVar29 = SUB84(dVar28,0);
          if (in_FPSCR_SZ == '\0') {
            dVar23 = (double)CONCAT44(*(undefined4 *)pdVar12,SUB84(dVar23,0));
            fVar27 = *(float *)((int)pdVar12 + 4);
            pdVar10 = pdVar12 + 1;
          }
          else {
            dVar23 = *pdVar12;
            fVar27 = (float)((ulonglong)pdVar12[1] >> 0x20);
            pdVar10 = pdVar12 + 2;
          }
          fVar25 = 1.0;
                    /* WARNING: Ignoring partial resolution of indirect */
          if (in_FPSCR_PR == '\0') {
            fVar20 = SUB84(dVar23,0);
            fVar21 = (float)((ulonglong)dVar23 >> 0x20);
            fVar25 = fVar22 * fVar20 + fVar42 * fVar21 + in_xf10 * 1.0 + fVar47 * fVar27;
            fVar19 = fVar19 * fVar20 + fVar40 * fVar21 + fVar43 * 1.0 + fVar45 * fVar27;
            fVar27 = fVar37 * fVar20 + fVar41 * fVar21 + fVar44 * 1.0 + fVar46 * fVar27;
          }
          if (in_FPSCR_SZ == '\0') {
            uVar31 = *(undefined4 *)pdVar10;
            dVar28 = (double)CONCAT44(uVar31,uVar29);
            dVar32 = (double)CONCAT44(*(undefined4 *)((int)pdVar10 + 4),uVar34);
            *(undefined4 *)((int)unaff_r8 + 0xc) = *(undefined4 *)((int)pdVar10 + 4);
            *(undefined4 *)(unaff_r8 + 1) = uVar29;
            pdVar10 = (double *)((int)unaff_r8 + 4);
            *(undefined4 *)pdVar10 = uVar31;
          }
          else {
            dVar28 = *pdVar10;
            dVar32 = pdVar10[1];
            unaff_r8[1] = dVar32;
            *unaff_r8 = in_xd4;
            pdVar10 = unaff_r8 + -1;
            *pdVar10 = dVar28;
          }
          uVar34 = SUB84(dVar32,0);
          *(undefined4 *)((int)pdVar10 + -4) = uVar30;
          in_dr2 = (double)CONCAT44(0x3f800000,fVar25);
          if (in_FPSCR_SZ == '\0') {
            *(undefined4 *)((int)pdVar10 + 0x14) = 0x3f800000;
            *(int *)(pdVar10 + 2) = SUB84(dVar23,0);
            pdVar10 = (double *)((int)pdVar10 + 0xc);
            *(int *)pdVar10 = (int)((ulonglong)dVar23 >> 0x20);
          }
          else {
            pdVar10[2] = in_dr2;
            pdVar10[1] = in_xd0;
            *pdVar10 = dVar23;
          }
          unaff_r8 = pdVar10 + 2;
        }
        else {
          if (in_FPSCR_SZ == '\0') {
            dVar23 = (double)CONCAT44(*(undefined4 *)pdVar12,SUB84(dVar23,0));
            fVar27 = *(float *)((int)pdVar12 + 4);
            pdVar9 = pdVar12 + 1;
          }
          else {
            dVar23 = *pdVar12;
            fVar27 = (float)((ulonglong)pdVar12[1] >> 0x20);
            pdVar9 = pdVar12 + 2;
          }
          fVar25 = 1.0;
                    /* WARNING: Ignoring partial resolution of indirect */
          if (in_FPSCR_PR == '\0') {
            fVar20 = SUB84(dVar23,0);
            fVar21 = (float)((ulonglong)dVar23 >> 0x20);
            fVar25 = fVar22 * fVar20 + fVar42 * fVar21 + in_xf10 * 1.0 + fVar47 * fVar27;
            fVar19 = fVar19 * fVar20 + fVar40 * fVar21 + fVar43 * 1.0 + fVar45 * fVar27;
            fVar27 = fVar37 * fVar20 + fVar41 * fVar21 + fVar44 * 1.0 + fVar46 * fVar27;
          }
          if (in_FPSCR_SZ == '\0') {
            uVar29 = *(undefined4 *)pdVar9;
            dVar28 = (double)CONCAT44(uVar29,uVar31);
            dVar32 = (double)CONCAT44(*(undefined4 *)((int)pdVar9 + 4),uVar34);
            *(undefined4 *)((int)unaff_r8 + 0xc) = *(undefined4 *)((int)pdVar9 + 4);
            *(undefined4 *)(unaff_r8 + 1) = uVar31;
            pdVar9 = (double *)((int)unaff_r8 + 4);
            *(undefined4 *)pdVar9 = uVar29;
          }
          else {
            dVar28 = *pdVar9;
            dVar32 = pdVar9[1];
            unaff_r8[1] = dVar32;
            *unaff_r8 = in_xd4;
            pdVar9 = unaff_r8 + -1;
            *pdVar9 = dVar28;
          }
          uVar34 = SUB84(dVar32,0);
          *(undefined4 *)((int)pdVar9 + -4) = uVar30;
          in_dr2 = (double)CONCAT44(0x3f800000,fVar25);
          if (in_FPSCR_SZ == '\0') {
            *(undefined4 *)((int)pdVar9 + 0x14) = 0x3f800000;
            *(int *)(pdVar9 + 2) = SUB84(dVar23,0);
            pdVar9 = (double *)((int)pdVar9 + 0xc);
            *(int *)pdVar9 = (int)((ulonglong)dVar23 >> 0x20);
          }
          else {
            pdVar9[2] = in_dr2;
            pdVar9[1] = in_xd0;
            *pdVar9 = dVar23;
          }
          unaff_r8 = pdVar9 + 2;
          pdVar9 = unaff_r8;
        }
      }
      else {
        in_dr2 = (double)((ulonglong)uVar26 << 0x20);
        if (in_FPSCR_PR == '\0') {
          bVar48 = fVar27 < 0.0;
        }
        else {
          bVar48 = dVar28 < in_dr2;
        }
        if (bVar48) {
          if (in_FPSCR_SZ == '\0') {
            dVar23 = (double)CONCAT44(*(undefined4 *)pdVar11,SUB84(dVar23,0));
            fVar27 = *(float *)((int)pdVar11 + 4);
            pdVar10 = pdVar11 + 1;
          }
          else {
            dVar23 = *pdVar11;
            fVar27 = (float)((ulonglong)pdVar11[1] >> 0x20);
            pdVar10 = pdVar11 + 2;
          }
          fVar25 = 1.0;
                    /* WARNING: Ignoring partial resolution of indirect */
          if (in_FPSCR_PR == '\0') {
            fVar21 = SUB84(dVar23,0);
            fVar24 = (float)((ulonglong)dVar23 >> 0x20);
            fVar25 = fVar22 * fVar21 + fVar42 * fVar24 + in_xf10 * 1.0 + fVar47 * fVar27;
            fVar20 = fVar19 * fVar21 + fVar40 * fVar24 + fVar43 * 1.0 + fVar45 * fVar27;
            fVar27 = fVar37 * fVar21 + fVar41 * fVar24 + fVar44 * 1.0 + fVar46 * fVar27;
          }
          if (in_FPSCR_SZ == '\0') {
            uVar30 = *(undefined4 *)pdVar10;
            dVar28 = (double)CONCAT44(uVar30,uVar31);
            dVar32 = (double)CONCAT44(*(undefined4 *)((int)pdVar10 + 4),uVar34);
            *(undefined4 *)((int)unaff_r8 + 0xc) = *(undefined4 *)((int)pdVar10 + 4);
            *(undefined4 *)(unaff_r8 + 1) = uVar31;
            pdVar10 = (double *)((int)unaff_r8 + 4);
            *(undefined4 *)pdVar10 = uVar30;
          }
          else {
            dVar28 = *pdVar10;
            dVar32 = pdVar10[1];
            unaff_r8[1] = dVar32;
            *unaff_r8 = in_xd4;
            pdVar10 = unaff_r8 + -1;
            *pdVar10 = dVar28;
          }
          uVar34 = SUB84(dVar32,0);
          *(undefined4 *)((int)pdVar10 + -4) = uVar29;
          if (in_FPSCR_SZ == '\0') {
            *(undefined4 *)((int)pdVar10 + 0x14) = 0x3f800000;
            *(int *)(pdVar10 + 2) = SUB84(dVar23,0);
            pdVar10 = (double *)((int)pdVar10 + 0xc);
            *(int *)pdVar10 = (int)((ulonglong)dVar23 >> 0x20);
          }
          else {
            pdVar10[2] = (double)CONCAT44(0x3f800000,fVar25);
            pdVar10[1] = in_xd0;
            *pdVar10 = dVar23;
          }
          unaff_r8 = pdVar10 + 2;
          goto LAB_8c616af4;
        }
      }
      puVar4 = PTR_FUN_8c616c08;
      if (*(int *)((int)pdVar18 + -0x3c) != 0) {
        *DAT_8c616c04 = 0;
        dVar23 = (double)(*(code *)puVar4)(pdVar11,pdVar12,pdVar13);
        pdVar9 = unaff_r8;
      }
      uVar30 = DAT_8c616bfc;
      param_2 = in_xd12;
      if (in_FPSCR_SZ == '\0') {
        param_2 = (double)CONCAT44(SUB84(dVar33,0),uVar34);
      }
      iVar7 = *(int *)((int)pdVar18 + -0x48);
      if (in_FPSCR_SZ == '\0') {
        dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),*(undefined4 *)(pdVar13 + 2));
        uVar35 = CONCAT44(*(undefined4 *)((int)pdVar13 + 0x14),SUB84(in_dr14,0));
        param_1 = (double)((ulonglong)in_dr14 >> 0x20);
      }
      else {
        in_xd12 = pdVar13[2];
        uVar35 = *(undefined8 *)((int)pdVar13 + 0x14);
        param_1 = dVar28;
        in_xd4 = in_dr14;
      }
      uVar34 = SUB84(param_1,0);
      in_dr14 = (double)CONCAT44((int)((ulonglong)uVar35 >> 0x20),-(float)uVar35);
      *(int *)((int)pdVar18 + -0x48) = iVar7 + -1;
      pdVar11 = pdVar12;
      pdVar12 = pdVar13;
    } while (iVar7 + -1 != 0);
    if (pdVar9 < unaff_r8) {
      if (in_FPSCR_SZ == '\0') {
        dVar23 = (double)CONCAT44(*(undefined4 *)pdVar13,SUB84(dVar23,0));
        fVar27 = *(float *)((int)pdVar13 + 4);
        pdVar13 = pdVar13 + 1;
      }
      else {
        dVar23 = *pdVar13;
        fVar27 = (float)((ulonglong)pdVar13[1] >> 0x20);
        pdVar13 = pdVar13 + 2;
      }
      fVar19 = 1.0;
                    /* WARNING: Ignoring partial resolution of indirect */
      if (in_FPSCR_PR == '\0') {
        fVar37 = SUB84(dVar23,0);
        fVar40 = (float)((ulonglong)dVar23 >> 0x20);
        fVar19 = SUB84(in_xd2,0) * fVar37 + (float)in_xd6 * fVar40 + in_xf10 * 1.0 +
                 SUB84(in_xd14,0) * fVar27;
        fVar22 = SUB84(in_xd0,0) * fVar37 + SUB84(in_xd4,0) * fVar40 + (float)in_xd8 * 1.0 +
                 SUB84(in_xd12,0) * fVar27;
        fVar27 = (float)((ulonglong)in_xd0 >> 0x20) * fVar37 +
                 (float)((ulonglong)in_xd4 >> 0x20) * fVar40 +
                 (float)((ulonglong)in_xd8 >> 0x20) * 1.0 +
                 (float)((ulonglong)in_xd12 >> 0x20) * fVar27;
      }
      if (in_FPSCR_SZ == '\0') {
        uVar29 = *(undefined4 *)pdVar13;
        param_1 = (double)CONCAT44(uVar29,uVar34);
        param_2 = (double)CONCAT44(*(undefined4 *)((int)pdVar13 + 4),SUB84(param_2,0));
        *(undefined4 *)((int)unaff_r8 + 0xc) = *(undefined4 *)((int)pdVar13 + 4);
        *(undefined4 *)(unaff_r8 + 1) = uVar34;
        pdVar9 = (double *)((int)unaff_r8 + 4);
        *(undefined4 *)pdVar9 = uVar29;
      }
      else {
        param_1 = *pdVar13;
        param_2 = pdVar13[1];
        unaff_r8[1] = param_2;
        *unaff_r8 = in_xd4;
        pdVar9 = unaff_r8 + -1;
        *pdVar9 = param_1;
      }
      *(undefined4 *)((int)pdVar9 + -4) = uVar30;
      in_dr2 = (double)CONCAT44(0x3f800000,fVar19);
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)((int)pdVar9 + 0x14) = 0x3f800000;
        *(int *)(pdVar9 + 2) = SUB84(dVar23,0);
        pdVar9 = (double *)((int)pdVar9 + 0xc);
        *(int *)pdVar9 = (int)((ulonglong)dVar23 >> 0x20);
      }
      else {
        pdVar9[2] = in_dr2;
        pdVar9[1] = in_xd0;
        *pdVar9 = dVar23;
      }
      unaff_r8 = pdVar9 + 2;
    }
    iVar7 = *(int *)((int)pdVar18 + -0x38);
    *(int *)((int)pdVar18 + -0x38) = iVar7 + -1;
    if (iVar7 + -1 == 0) {
      *DAT_8c616c0c = *DAT_8c616c0c + *(int *)((int)pdVar18 + -0x44);
      *DAT_8c616c10 = *DAT_8c616c10 + *(int *)((int)pdVar18 + -0x44);
      return;
    }
  } while( true );
}

