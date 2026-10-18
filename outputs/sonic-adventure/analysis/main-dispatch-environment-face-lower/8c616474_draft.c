/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c616680) */
/* WARNING: Removing unreachable block (ram,0x8c6167d0) */
/* WARNING: Restarted to delay deadcode elimination for space: register */

void FUN_8c616474(double param_1,double param_2,double param_3,undefined4 param_4)

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
  double dVar22;
  float fVar23;
  double in_dr2;
  uint uVar24;
  float fVar25;
  double dVar26;
  undefined4 uVar28;
  undefined4 uVar29;
  double dVar30;
  double dVar31;
  undefined8 in_dr12;
  double dVar32;
  undefined4 uVar33;
  double in_dr14;
  undefined8 uVar34;
  double in_xd0;
  float fVar35;
  float fVar36;
  double in_xd2;
  double dVar37;
  float fVar38;
  float fVar39;
  double in_xd4;
  float fVar40;
  float fVar41;
  undefined8 in_xd6;
  float fVar42;
  float fVar43;
  undefined8 in_xd8;
  float fVar44;
  float in_xf10;
  float in_xf11;
  float fVar45;
  double in_xd12;
  float fVar46;
  float fVar47;
  double in_xd14;
  float fVar48;
  bool bVar49;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  int aiStack_48 [4];
  undefined1 local_38 [16];
  double dStack_28;
  undefined8 uStack_20;
  float fVar21;
  undefined4 uVar27;
  
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
  dVar32 = (double)CONCAT44(0x3f800000,(int)in_dr12);
  *(undefined4 *)((int)pdVar18 + -0x48) = param_4;
  dVar22 = (double)(*(code *)PTR_FUN_8c6164c8)();
  puVar15 = *(ushort **)(*(int *)((int)pdVar18 + -0x48) + 4);
  *(uint *)((int)pdVar18 + -0x38) = (uint)*(ushort *)(*(int *)((int)pdVar18 + -0x48) + 2);
  iVar14 = *DAT_8c6164cc;
  *(undefined4 *)((int)pdVar18 + -0x48) = 0;
  do {
    uVar33 = SUB84(in_dr14,0);
    uVar1 = *puVar15;
    puVar16 = puVar15 + 1;
    *(uint *)((int)pdVar18 + -0x44) = (uint)uVar1;
    uVar28 = (undefined4)((ulonglong)dVar32 >> 0x20);
    dVar26 = dVar32;
    if (in_FPSCR_SZ == '\0') {
      dVar26 = in_xd14;
      uVar33 = uVar28;
    }
    in_xd14 = dVar26;
    if ((uVar1 & DAT_8c6165ec) != 0) {
      if (in_FPSCR_PR == '\0') {
        param_1 = (double)CONCAT44(0xbf800000,SUB84(param_1,0));
      }
      else {
        param_1 = -(double)CONCAT44(0x3f800000,SUB84(param_1,0));
      }
      *(uint *)((int)pdVar18 + -0x44) = *(uint *)((int)pdVar18 + -0x44) & (int)DAT_8c6165e8;
      in_xd14 = param_1;
      if (in_FPSCR_SZ == '\0') {
        uVar33 = (undefined4)((ulonglong)param_1 >> 0x20);
        in_xd14 = dVar26;
      }
    }
    *(int *)((int)pdVar18 + -0x48) = *(int *)((int)pdVar18 + -0x48) + *(int *)((int)pdVar18 + -0x44)
    ;
    *(int *)((int)pdVar18 + -0x44) = *(int *)((int)pdVar18 + -0x44) + -2;
    puVar17 = puVar15 + 2;
    puVar15 = puVar15 + 3;
    pdVar12 = (double *)((uint)*puVar17 * 0x20 + iVar14);
    pdVar11 = (double *)((uint)*puVar16 * 0x20 + iVar14);
    pdVar9 = unaff_r8;
    if (in_FPSCR_SZ == '\0') {
      dVar32 = (double)CONCAT44(uVar28,*(undefined4 *)(pdVar12 + 2));
      param_2 = (double)CONCAT44(*(undefined4 *)(pdVar11 + 2),SUB84(param_2,0));
      in_dr14 = (double)CONCAT44(*(undefined4 *)((int)pdVar12 + 0x14),uVar33);
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
      uVar33 = (undefined4)((ulonglong)dVar32 >> 0x20);
      dVar26 = dVar32;
      if (in_FPSCR_SZ == '\0') {
        dVar26 = (double)CONCAT44(uVar33,SUB84(param_1,0));
      }
      if ((*(uint *)((int)pdVar11 + 0xc) & *(uint *)((int)pdVar12 + 0xc) &
           *(uint *)((int)pdVar13 + 0xc) & 0x3f) == 0) {
        if (((*(uint *)((int)pdVar11 + 0xc) | *(uint *)((int)pdVar12 + 0xc) |
             *(uint *)((int)pdVar13 + 0xc)) & 1) == 0) {
          if (in_FPSCR_SZ == '\0') {
            dVar30 = (double)CONCAT44(*(undefined4 *)((int)pdVar13 + 0x14),SUB84(dVar32,0));
          }
          else {
            dVar30 = *(double *)((int)pdVar13 + 0x14);
            in_xd2 = in_xd12;
          }
          fVar25 = (float)((ulonglong)in_dr14 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            dVar30 = (double)CONCAT44((float)((ulonglong)dVar30 >> 0x20) - fVar25,
                                      SUB84(dVar30,0) - (float)((ulonglong)param_2 >> 0x20));
          }
          else {
            dVar30 = (dVar30 - param_2) - in_dr14;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)(pdVar13 + 2))
            ;
          }
          else {
            in_xd0 = pdVar13[2];
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                      SUB84(dVar22,0) - SUB84(dVar32,0));
            fVar19 = SUB84(dVar30,0) * (float)((ulonglong)dVar30 >> 0x20);
          }
          else {
            dVar22 = dVar22 - dVar32;
            fVar19 = SUB84(dVar30 * dVar30,0);
          }
          dVar30 = in_dr14;
          if (in_FPSCR_SZ == '\0') {
            dVar30 = (double)CONCAT44(fVar25,fVar19);
          }
          if (in_FPSCR_PR == '\0') {
            fVar25 = ((float)((ulonglong)dVar30 >> 0x20) - SUB84(dVar26,0)) * SUB84(dVar22,0);
            in_dr2 = (double)CONCAT44(fVar25,SUB84(dVar30,0) - fVar25);
          }
          else {
            in_dr2 = (dVar30 - dVar26) * dVar22;
            in_dr2 = in_dr2 - in_dr2;
          }
          dVar30 = in_xd2;
          if (in_FPSCR_SZ == '\0') {
            dVar30 = (double)CONCAT44(SUB84(in_dr2,0),SUB84(dVar26,0));
          }
          if (in_FPSCR_PR == '\0') {
            dVar26 = (double)CONCAT44((float)((ulonglong)dVar30 >> 0x20) * SUB84(in_dr14,0),
                                      SUB84(dVar30,0));
          }
          else {
            dVar26 = dVar30 * in_dr14;
          }
        }
        else {
          puVar8 = (undefined4 *)((int)pdVar18 + -0x10);
          *puVar8 = pdVar11;
          *(double **)((int)pdVar18 + -0xc) = pdVar12;
          *(double **)((int)pdVar18 + -8) = pdVar13;
          iVar7 = 0;
          while( true ) {
            iVar3 = DAT_8c6165f0;
            pdVar10 = (double *)((int)pdVar18 + -0x34);
            uVar27 = (undefined4)((ulonglong)in_dr2 >> 0x20);
            uVar28 = SUB84(dVar26,0);
            if (2 < iVar7) break;
            piVar5 = puVar8 + iVar7;
            *(int **)((int)pdVar18 + -0x4c) = piVar5;
            *(int *)((int)pdVar18 + -0x50) = iVar7 * 0xc + (int)pdVar10;
            iVar7 = iVar7 + 1;
            if ((*(uint *)(*piVar5 + 0xc) & 1) == 0) {
              dVar26 = dVar32;
              if (in_FPSCR_SZ == '\0') {
                dVar26 = (double)CONCAT44(uVar33,uVar28);
              }
              iVar6 = **(int **)((int)pdVar18 + -0x4c);
              *(int *)((int)pdVar18 + -0x40) = iVar6;
              if (in_FPSCR_SZ == '\0') {
                in_dr2 = (double)CONCAT44(uVar27,*(undefined4 *)(iVar6 + 0x18));
              }
              else {
                in_xd2 = *(double *)(iVar6 + 0x18);
              }
              if (in_FPSCR_PR == '\0') {
                dVar26 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) / SUB84(in_dr2,0),
                                          SUB84(dVar26,0));
              }
              else {
                dVar26 = dVar26 / in_dr2;
              }
              if (in_FPSCR_SZ == '\0') {
                in_dr2 = (double)(ulonglong)*(uint *)(DAT_8c6165f0 + 0xc);
              }
              else {
                in_xd2 = *(double *)(DAT_8c6165f0 + 0xc);
              }
              if (in_FPSCR_SZ == '\0') {
                dVar30 = (double)CONCAT44(*(undefined4 *)(*(int *)((int)pdVar18 + -0x40) + 0x10),
                                          SUB84(in_dr2,0));
              }
              else {
                dVar30 = *(double *)(*(int *)((int)pdVar18 + -0x40) + 0x10);
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
                                          *(undefined4 *)DAT_8c6165f4);
              }
              else {
                in_xd2 = *DAT_8c6165f4;
              }
              fVar25 = (float)((ulonglong)dVar26 >> 0x20);
              if (in_FPSCR_PR == '\0') {
                dVar30 = (double)CONCAT44(((float)((ulonglong)dVar30 >> 0x20) * fVar25) /
                                          SUB84(dVar30,0),SUB84(dVar30,0));
              }
              else {
                dVar30 = (dVar30 * dVar26) / (dVar30 * dVar26);
              }
              uVar28 = (undefined4)((ulonglong)dVar30 >> 0x20);
              if (in_FPSCR_SZ == '\0') {
                *(undefined4 *)*(double **)((int)pdVar18 + -0x50) = uVar28;
              }
              else {
                **(double **)((int)pdVar18 + -0x50) = dVar30;
              }
              if (in_FPSCR_SZ == '\0') {
                dVar30 = (double)CONCAT44(uVar28,*(undefined4 *)(iVar3 + 0x10));
              }
              else {
                in_xd2 = *(double *)(iVar3 + 0x10);
              }
              if (in_FPSCR_SZ == '\0') {
                dVar30 = (double)CONCAT44(*(undefined4 *)(**(int **)((int)pdVar18 + -0x4c) + 0x14),
                                          SUB84(dVar30,0));
              }
              else {
                dVar30 = *(double *)(**(int **)((int)pdVar18 + -0x4c) + 0x14);
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
                                          *(undefined4 *)DAT_8c6165f8);
              }
              else {
                in_xd2 = *DAT_8c6165f8;
              }
              if (in_FPSCR_PR == '\0') {
                in_dr2 = (double)CONCAT44(((float)((ulonglong)dVar30 >> 0x20) * fVar25) /
                                          SUB84(dVar30,0),SUB84(dVar30,0));
              }
              else {
                in_dr2 = (dVar30 * dVar26) / (dVar30 * dVar26);
              }
              if (in_FPSCR_SZ == '\0') {
                *(int *)(*(int *)((int)pdVar18 + -0x50) + 4) = (int)((ulonglong)in_dr2 >> 0x20);
              }
              else {
                *(double *)(*(int *)((int)pdVar18 + -0x50) + 4) = in_dr2;
              }
              if (in_FPSCR_SZ == '\0') {
                *(float *)(*(int *)((int)pdVar18 + -0x50) + 8) = fVar25;
              }
              else {
                *(double *)(*(int *)((int)pdVar18 + -0x50) + 8) = dVar26;
              }
            }
            else {
              if (in_FPSCR_SZ == '\0') {
                uVar28 = *(undefined4 *)(**(int **)((int)pdVar18 + -0x4c) + 0x10);
                in_dr2 = (double)CONCAT44(uVar27,uVar28);
                *(undefined4 *)*(double **)((int)pdVar18 + -0x50) = uVar28;
              }
              else {
                in_xd2 = *(double *)(**(int **)((int)pdVar18 + -0x4c) + 0x10);
                **(double **)((int)pdVar18 + -0x50) = in_xd2;
              }
              if (in_FPSCR_SZ == '\0') {
                uVar28 = *(undefined4 *)(**(int **)((int)pdVar18 + -0x4c) + 0x14);
                in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),uVar28);
                *(undefined4 *)(*(int *)((int)pdVar18 + -0x50) + 4) = uVar28;
              }
              else {
                in_xd2 = *(double *)(**(int **)((int)pdVar18 + -0x4c) + 0x14);
                *(double *)(*(int *)((int)pdVar18 + -0x50) + 4) = in_xd2;
              }
              if (in_FPSCR_SZ == '\0') {
                uVar28 = *(undefined4 *)(**(int **)((int)pdVar18 + -0x4c) + 0x18);
                in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),uVar28);
                *(undefined4 *)(*(int *)((int)pdVar18 + -0x50) + 8) = uVar28;
              }
              else {
                in_xd2 = *(double *)(**(int **)((int)pdVar18 + -0x4c) + 0x18);
                *(double *)(*(int *)((int)pdVar18 + -0x50) + 8) = in_xd2;
              }
            }
          }
          if (in_FPSCR_SZ == '\0') {
            uVar29 = *(undefined4 *)((int)pdVar18 + -0x2c);
            dVar22 = (double)CONCAT44(uVar29,SUB84(dVar22,0));
            param_2 = (double)CONCAT44(uVar29,SUB84(param_2,0));
          }
          else {
            dVar22 = *(double *)((int)pdVar18 + -0x2c);
            uVar29 = (undefined4)((ulonglong)dVar22 >> 0x20);
            param_2 = dVar22;
          }
          puVar2 = (undefined8 *)((int)pdVar18 + -0x28);
          *(undefined8 **)((int)pdVar18 + -0x50) = puVar2;
          if (in_FPSCR_SZ == '\0') {
            uVar28 = *(undefined4 *)((int)pdVar18 + -0x30);
            param_2 = (double)CONCAT44(uVar29,*(undefined4 *)((int)pdVar18 + -0x24));
            param_3 = (double)CONCAT44(*(undefined4 *)((int)pdVar18 + -0x20),SUB84(param_3,0));
            in_dr2 = (double)CONCAT44(uVar27,uVar28);
          }
          else {
            in_xd2 = *(double *)((int)pdVar18 + -0x30);
            in_xd6 = *(undefined8 *)((int)pdVar18 + -0x24);
            param_3 = *(double *)((int)pdVar18 + -0x20);
            in_xd4 = in_xd2;
          }
          fVar25 = (float)((ulonglong)param_3 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) * SUB84(param_2,0),
                                      SUB84(dVar22,0));
            fVar19 = SUB84(in_dr2,0) * fVar25;
          }
          else {
            dVar22 = dVar22 * param_2;
            fVar19 = SUB84(in_dr2 * param_3,0);
          }
          if (in_FPSCR_SZ == '\0') {
            uVar27 = *(undefined4 *)pdVar10;
            param_3 = (double)CONCAT44(fVar25,*(undefined4 *)puVar2);
            dVar30 = (double)CONCAT44(uVar27,fVar19);
            dVar26 = (double)CONCAT44(uVar27,uVar28);
          }
          else {
            dVar30 = *pdVar10;
            in_xd8 = *puVar2;
            uVar27 = (undefined4)((ulonglong)dVar30 >> 0x20);
            dVar26 = dVar30;
          }
          if (in_FPSCR_PR == '\0') {
            dVar30 = (double)CONCAT44((float)((ulonglong)dVar30 >> 0x20) *
                                      (float)((ulonglong)param_3 >> 0x20),SUB84(dVar30,0));
          }
          else {
            dVar30 = dVar30 * param_3;
          }
          uVar28 = SUB84(dVar26,0);
          dVar31 = in_xd4;
          if (in_FPSCR_SZ == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),uVar28);
            dVar31 = in_xd0;
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) - SUB84(dVar30,0),
                                      SUB84(dVar22,0));
          }
          else {
            dVar22 = dVar22 - dVar30;
          }
          dVar37 = param_2;
          if (in_FPSCR_SZ == '\0') {
            dVar30 = (double)CONCAT44((int)((ulonglong)dVar30 >> 0x20),
                                      (int)((ulonglong)param_2 >> 0x20));
            dVar37 = in_xd2;
          }
          if (in_FPSCR_PR == '\0') {
            fVar25 = SUB84(dVar30,0) * SUB84(param_3,0);
            dVar30 = (double)CONCAT44((float)((ulonglong)dVar30 >> 0x20) - fVar25,fVar25);
          }
          else {
            dVar30 = dVar30 * param_3 - dVar30 * param_3;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar30 = (double)CONCAT44((int)((ulonglong)dVar30 >> 0x20),
                                      *(undefined4 *)((int)pdVar18 + -0x18));
          }
          else {
            dVar37 = *(double *)((int)pdVar18 + -0x18);
          }
          if (in_FPSCR_PR == '\0') {
            fVar25 = SUB84(dVar30,0) * (float)((ulonglong)dVar30 >> 0x20);
          }
          else {
            fVar25 = SUB84(dVar30 * dVar30,0);
          }
          if (in_FPSCR_SZ == '\0') {
            uVar34 = CONCAT44(*(undefined4 *)((int)pdVar18 + -0x1c),fVar25);
          }
          else {
            uVar34 = *(undefined8 *)((int)pdVar18 + -0x1c);
          }
          if (in_FPSCR_PR == '\0') {
            fVar25 = (float)((ulonglong)uVar34 >> 0x20);
            uVar34 = CONCAT44(fVar25,(float)((ulonglong)dVar22 >> 0x20) * fVar25 + (float)uVar34);
          }
          dVar30 = dVar37;
          if (in_FPSCR_SZ == '\0') {
            dVar30 = (double)CONCAT44((int)uVar34,uVar27);
            dVar26 = dVar37;
          }
          if (in_FPSCR_PR == '\0') {
            fVar25 = SUB84(dVar30,0) * SUB84(param_2,0);
            dVar30 = (double)CONCAT44((int)((ulonglong)dVar30 >> 0x20),fVar25);
            fVar25 = SUB84(dVar22,0) * SUB84(param_3,0) - fVar25;
          }
          else {
            dVar30 = dVar30 * param_2;
            fVar25 = SUB84(dVar22 * param_3 - dVar30,0);
          }
          if (in_FPSCR_SZ == '\0') {
            dVar30 = (double)CONCAT44((int)((ulonglong)dVar30 >> 0x20),
                                      *(undefined4 *)((int)pdVar18 + -0x14));
            dVar22 = (double)CONCAT44(fVar25,fVar25);
          }
          else {
            dVar26 = *(double *)((int)pdVar18 + -0x14);
            dVar22 = dVar31;
          }
          if (in_FPSCR_PR == '\0') {
            dVar30 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) * SUB84(dVar30,0) +
                                      (float)((ulonglong)dVar30 >> 0x20),SUB84(dVar30,0));
          }
          in_xd2 = in_xd14;
          if (in_FPSCR_SZ == '\0') {
            dVar30 = (double)CONCAT44((int)((ulonglong)dVar30 >> 0x20),SUB84(in_dr14,0));
            in_xd2 = dVar26;
          }
          uVar27 = (undefined4)((ulonglong)dVar30 >> 0x20);
          fVar25 = -SUB84(dVar30,0);
          dVar26 = (double)CONCAT44(uVar27,fVar25);
          dVar30 = dVar26;
          if (in_FPSCR_SZ == '\0') {
            dVar30 = (double)CONCAT44(uVar27,uVar28);
          }
          if (in_FPSCR_PR == '\0') {
            dVar30 = (double)CONCAT44((float)((ulonglong)dVar30 >> 0x20) * fVar25,SUB84(dVar30,0));
          }
          else {
            dVar30 = dVar30 * dVar26;
          }
          in_dr2 = (double)(ulonglong)(uint)fVar25;
          if (in_FPSCR_PR == '\0') {
            bVar49 = 0.0 < (float)((ulonglong)dVar30 >> 0x20);
          }
          else {
            bVar49 = in_dr2 < dVar30;
          }
          dVar26 = dVar30;
          in_xd0 = dVar31;
          if ((!bVar49) &&
             (*(undefined4 *)((int)pdVar18 + -0x3c) = 1, dVar26 = dVar32, in_FPSCR_SZ == '\0')) {
            dVar26 = (double)CONCAT44(uVar33,SUB84(dVar30,0));
          }
        }
      }
      uVar27 = DAT_8c616880;
      uVar28 = DAT_8c61687c;
      uVar24 = (uint)((ulonglong)in_dr2 >> 0x20);
      fVar25 = (float)((ulonglong)dVar26 >> 0x20);
      fVar19 = SUB84(in_xd0,0);
      fVar35 = (float)((ulonglong)in_xd0 >> 0x20);
      fVar36 = SUB84(in_xd2,0);
      fVar38 = (float)((ulonglong)in_xd2 >> 0x20);
      fVar39 = SUB84(in_xd4,0);
      fVar40 = (float)((ulonglong)in_xd4 >> 0x20);
      fVar41 = (float)in_xd6;
      fVar42 = (float)((ulonglong)in_xd6 >> 0x20);
      fVar43 = (float)in_xd8;
      fVar44 = (float)((ulonglong)in_xd8 >> 0x20);
      fVar45 = SUB84(in_xd12,0);
      fVar46 = (float)((ulonglong)in_xd12 >> 0x20);
      fVar47 = SUB84(in_xd14,0);
      fVar48 = (float)((ulonglong)in_xd14 >> 0x20);
      uVar33 = SUB84(param_2,0);
      uVar29 = SUB84(dVar26,0);
      if (pdVar9 < unaff_r8) {
        if (in_FPSCR_PR == '\0') {
          bVar49 = fVar25 < 0.0;
        }
        else {
          bVar49 = dVar26 < (double)((ulonglong)uVar24 << 0x20);
        }
        if (bVar49) {
LAB_8c616718:
          uVar28 = DAT_8c616880;
          uVar27 = SUB84(dVar26,0);
          if (in_FPSCR_SZ == '\0') {
            dVar22 = (double)CONCAT44(*(undefined4 *)pdVar12,SUB84(dVar22,0));
            fVar25 = *(float *)((int)pdVar12 + 4);
            pdVar10 = pdVar12 + 1;
          }
          else {
            dVar22 = *pdVar12;
            fVar25 = (float)((ulonglong)pdVar12[1] >> 0x20);
            pdVar10 = pdVar12 + 2;
          }
          in_dr2 = (double)CONCAT44(fVar25,0x3f800000);
                    /* WARNING: Ignoring partial resolution of indirect */
          if (in_FPSCR_PR == '\0') {
            fVar20 = SUB84(dVar22,0);
            fVar21 = (float)((ulonglong)dVar22 >> 0x20);
            fVar19 = fVar19 * fVar20 + fVar39 * fVar21 + fVar43 * 1.0 + fVar45 * fVar25;
            fVar19 = fVar35 * fVar20 + fVar40 * fVar21 + fVar44 * 1.0 + fVar46 * fVar25;
            fVar19 = fVar36 * fVar20 + fVar41 * fVar21 + in_xf10 * 1.0 + fVar47 * fVar25;
            fVar25 = fVar38 * fVar20 + fVar42 * fVar21 + in_xf11 * 1.0 + fVar48 * fVar25;
          }
          if (in_FPSCR_SZ == '\0') {
            uVar29 = *(undefined4 *)pdVar10;
            dVar26 = (double)CONCAT44(uVar29,uVar27);
            dVar30 = (double)CONCAT44(*(undefined4 *)((int)pdVar10 + 4),uVar33);
            *(undefined4 *)((int)unaff_r8 + 0xc) = *(undefined4 *)((int)pdVar10 + 4);
            *(undefined4 *)(unaff_r8 + 1) = uVar27;
            pdVar10 = (double *)((int)unaff_r8 + 4);
            *(undefined4 *)pdVar10 = uVar29;
          }
          else {
            dVar26 = *pdVar10;
            dVar30 = pdVar10[1];
            unaff_r8[1] = dVar30;
            *unaff_r8 = in_xd4;
            pdVar10 = unaff_r8 + -1;
            *pdVar10 = dVar26;
          }
          uVar33 = SUB84(dVar30,0);
          *(undefined4 *)((int)pdVar10 + -4) = uVar28;
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)pdVar10 + 0x14) = (int)((ulonglong)in_dr2 >> 0x20);
            *(int *)(pdVar10 + 2) = SUB84(dVar22,0);
            pdVar10 = (double *)((int)pdVar10 + 0xc);
            *(int *)pdVar10 = (int)((ulonglong)dVar22 >> 0x20);
          }
          else {
            pdVar10[2] = in_dr2;
            pdVar10[1] = in_xd0;
            *pdVar10 = dVar22;
          }
          unaff_r8 = pdVar10 + 2;
        }
        else {
          if (in_FPSCR_SZ == '\0') {
            dVar22 = (double)CONCAT44(*(undefined4 *)pdVar12,SUB84(dVar22,0));
            fVar25 = *(float *)((int)pdVar12 + 4);
            pdVar9 = pdVar12 + 1;
          }
          else {
            dVar22 = *pdVar12;
            fVar25 = (float)((ulonglong)pdVar12[1] >> 0x20);
            pdVar9 = pdVar12 + 2;
          }
          in_dr2 = (double)CONCAT44(fVar25,0x3f800000);
                    /* WARNING: Ignoring partial resolution of indirect */
          if (in_FPSCR_PR == '\0') {
            fVar20 = SUB84(dVar22,0);
            fVar21 = (float)((ulonglong)dVar22 >> 0x20);
            fVar19 = fVar19 * fVar20 + fVar39 * fVar21 + fVar43 * 1.0 + fVar45 * fVar25;
            fVar19 = fVar35 * fVar20 + fVar40 * fVar21 + fVar44 * 1.0 + fVar46 * fVar25;
            fVar19 = fVar36 * fVar20 + fVar41 * fVar21 + in_xf10 * 1.0 + fVar47 * fVar25;
            fVar25 = fVar38 * fVar20 + fVar42 * fVar21 + in_xf11 * 1.0 + fVar48 * fVar25;
          }
          if (in_FPSCR_SZ == '\0') {
            uVar27 = *(undefined4 *)pdVar9;
            dVar26 = (double)CONCAT44(uVar27,uVar29);
            dVar30 = (double)CONCAT44(*(undefined4 *)((int)pdVar9 + 4),uVar33);
            *(undefined4 *)((int)unaff_r8 + 0xc) = *(undefined4 *)((int)pdVar9 + 4);
            *(undefined4 *)(unaff_r8 + 1) = uVar29;
            pdVar9 = (double *)((int)unaff_r8 + 4);
            *(undefined4 *)pdVar9 = uVar27;
          }
          else {
            dVar26 = *pdVar9;
            dVar30 = pdVar9[1];
            unaff_r8[1] = dVar30;
            *unaff_r8 = in_xd4;
            pdVar9 = unaff_r8 + -1;
            *pdVar9 = dVar26;
          }
          uVar33 = SUB84(dVar30,0);
          *(undefined4 *)((int)pdVar9 + -4) = uVar28;
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)pdVar9 + 0x14) = (int)((ulonglong)in_dr2 >> 0x20);
            *(int *)(pdVar9 + 2) = SUB84(dVar22,0);
            pdVar9 = (double *)((int)pdVar9 + 0xc);
            *(int *)pdVar9 = (int)((ulonglong)dVar22 >> 0x20);
          }
          else {
            pdVar9[2] = in_dr2;
            pdVar9[1] = in_xd0;
            *pdVar9 = dVar22;
          }
          unaff_r8 = pdVar9 + 2;
          pdVar9 = unaff_r8;
        }
      }
      else {
        in_dr2 = (double)((ulonglong)uVar24 << 0x20);
        if (in_FPSCR_PR == '\0') {
          bVar49 = fVar25 < 0.0;
        }
        else {
          bVar49 = dVar26 < in_dr2;
        }
        if (bVar49) {
          if (in_FPSCR_SZ == '\0') {
            dVar22 = (double)CONCAT44(*(undefined4 *)pdVar11,SUB84(dVar22,0));
            fVar25 = *(float *)((int)pdVar11 + 4);
            pdVar10 = pdVar11 + 1;
          }
          else {
            dVar22 = *pdVar11;
            fVar25 = (float)((ulonglong)pdVar11[1] >> 0x20);
            pdVar10 = pdVar11 + 2;
          }
          dVar30 = (double)CONCAT44(fVar25,0x3f800000);
                    /* WARNING: Ignoring partial resolution of indirect */
          if (in_FPSCR_PR == '\0') {
            fVar21 = SUB84(dVar22,0);
            fVar23 = (float)((ulonglong)dVar22 >> 0x20);
            fVar20 = fVar19 * fVar21 + fVar39 * fVar23 + fVar43 * 1.0 + fVar45 * fVar25;
            fVar20 = fVar35 * fVar21 + fVar40 * fVar23 + fVar44 * 1.0 + fVar46 * fVar25;
            fVar20 = fVar36 * fVar21 + fVar41 * fVar23 + in_xf10 * 1.0 + fVar47 * fVar25;
            fVar25 = fVar38 * fVar21 + fVar42 * fVar23 + in_xf11 * 1.0 + fVar48 * fVar25;
          }
          if (in_FPSCR_SZ == '\0') {
            uVar28 = *(undefined4 *)pdVar10;
            dVar26 = (double)CONCAT44(uVar28,uVar29);
            dVar31 = (double)CONCAT44(*(undefined4 *)((int)pdVar10 + 4),uVar33);
            *(undefined4 *)((int)unaff_r8 + 0xc) = *(undefined4 *)((int)pdVar10 + 4);
            *(undefined4 *)(unaff_r8 + 1) = uVar29;
            pdVar10 = (double *)((int)unaff_r8 + 4);
            *(undefined4 *)pdVar10 = uVar28;
          }
          else {
            dVar26 = *pdVar10;
            dVar31 = pdVar10[1];
            unaff_r8[1] = dVar31;
            *unaff_r8 = in_xd4;
            pdVar10 = unaff_r8 + -1;
            *pdVar10 = dVar26;
          }
          uVar33 = SUB84(dVar31,0);
          *(undefined4 *)((int)pdVar10 + -4) = uVar27;
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)pdVar10 + 0x14) = (int)((ulonglong)dVar30 >> 0x20);
            *(int *)(pdVar10 + 2) = SUB84(dVar22,0);
            pdVar10 = (double *)((int)pdVar10 + 0xc);
            *(int *)pdVar10 = (int)((ulonglong)dVar22 >> 0x20);
          }
          else {
            pdVar10[2] = dVar30;
            pdVar10[1] = in_xd0;
            *pdVar10 = dVar22;
          }
          unaff_r8 = pdVar10 + 2;
          goto LAB_8c616718;
        }
      }
      puVar4 = PTR_FUN_8c616888;
      uVar28 = SUB84(dVar26,0);
      if (*(int *)((int)pdVar18 + -0x3c) != 0) {
        *DAT_8c616884 = 0;
        if (in_FPSCR_SZ == '\0') {
          dVar22 = (double)CONCAT44(*(undefined4 *)pdVar11,*(undefined4 *)pdVar12);
        }
        else {
          in_xd2 = *pdVar12;
          dVar22 = *pdVar11;
        }
        if (in_FPSCR_PR == '\0') {
          dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                    SUB84(dVar22,0));
        }
        else {
          dVar22 = dVar22 + dVar22;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)pdVar13);
        }
        else {
          in_xd2 = *pdVar13;
        }
        if (in_FPSCR_PR == '\0') {
          dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                    SUB84(dVar22,0));
        }
        else {
          dVar22 = dVar22 + dVar22;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)pdVar18 + -0x10) = (int)((ulonglong)dVar22 >> 0x20);
          dVar22 = (double)CONCAT44(*(undefined4 *)((int)pdVar11 + 4),
                                    *(undefined4 *)((int)pdVar12 + 4));
        }
        else {
          *(double *)((int)pdVar18 + -0x10) = dVar22;
          in_xd2 = *(double *)((int)pdVar12 + 4);
          dVar22 = *(double *)((int)pdVar11 + 4);
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
                                    *(undefined4 *)((int)pdVar13 + 4));
        }
        else {
          in_xd2 = *(double *)((int)pdVar13 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                    SUB84(dVar22,0));
        }
        else {
          dVar22 = dVar22 + dVar22;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)pdVar18 + -0xc) = (int)((ulonglong)dVar22 >> 0x20);
          dVar22 = (double)CONCAT44(*(undefined4 *)(pdVar11 + 1),*(undefined4 *)(pdVar12 + 1));
        }
        else {
          *(double *)((int)pdVar18 + -0xc) = dVar22;
          in_xd2 = pdVar12[1];
          dVar22 = pdVar11[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                    SUB84(dVar22,0));
        }
        else {
          dVar22 = dVar22 + dVar22;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)(pdVar13 + 1));
        }
        else {
          in_xd2 = pdVar13[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                    SUB84(dVar22,0));
        }
        else {
          dVar22 = dVar22 + dVar22;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)pdVar18 + -8) = (int)((ulonglong)dVar22 >> 0x20);
        }
        else {
          *(double *)((int)pdVar18 + -8) = dVar22;
        }
        dVar22 = (double)(*(code *)puVar4)((undefined1 *)((int)pdVar18 + -0x10));
        if (in_FPSCR_SZ == '\0') {
          dVar26 = (double)CONCAT44(*(undefined4 *)DAT_8c616890,
                                    *(undefined4 *)((int)pdVar18 + -0x10));
        }
        else {
          in_xd2 = *(double *)((int)pdVar18 + -0x10);
          dVar26 = *DAT_8c616890;
        }
        if (in_FPSCR_PR == '\0') {
          dVar26 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) * SUB84(dVar26,0),
                                    SUB84(dVar26,0));
        }
        else {
          dVar26 = dVar26 * dVar26;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),*(undefined4 *)DAT_8c61688c);
        }
        else {
          in_xd2 = *DAT_8c61688c;
        }
        if (in_FPSCR_PR == '\0') {
          fVar25 = SUB84(dVar26,0) - (float)((ulonglong)dVar26 >> 0x20);
        }
        else {
          fVar25 = SUB84(dVar26 - dVar26,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar26 = (double)CONCAT44(*(undefined4 *)((int)pdVar18 + -0xc),fVar25);
          dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                    *(undefined4 *)((int)DAT_8c616890 + 4));
        }
        else {
          dVar26 = *(double *)((int)pdVar18 + -0xc);
          in_xd0 = *(double *)((int)DAT_8c616890 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                    SUB84(dVar22,0) * (float)((ulonglong)dVar26 >> 0x20));
        }
        else {
          dVar22 = dVar22 * dVar26;
        }
        if (in_FPSCR_SZ == '\0') {
          in_dr2 = (double)CONCAT44(*(undefined4 *)((int)pdVar18 + -8),SUB84(dVar26,0));
        }
        else {
          in_dr2 = *(double *)((int)pdVar18 + -8);
        }
        if (in_FPSCR_PR == '\0') {
          in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),
                                    SUB84(in_dr2,0) - SUB84(dVar22,0));
        }
        else {
          in_dr2 = in_dr2 - dVar22;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                    *(undefined4 *)(DAT_8c616890 + 1));
        }
        else {
          in_xd0 = DAT_8c616890[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                    SUB84(dVar22,0) * (float)((ulonglong)in_dr2 >> 0x20));
        }
        else {
          dVar22 = dVar22 * in_dr2;
        }
        dVar26 = in_xd2;
        if (in_FPSCR_SZ == '\0') {
          dVar26 = (double)CONCAT44(SUB84(in_dr2,0),uVar28);
        }
        if (in_FPSCR_PR == '\0') {
          dVar26 = (double)((ulonglong)dVar26 & 0xffffffff);
        }
        else {
          dVar26 = dVar26 - dVar22;
        }
        dVar22 = (double)(*(code *)PTR_FUN_8c616894)(pdVar11,pdVar12,pdVar13);
        pdVar9 = unaff_r8;
      }
      uVar28 = DAT_8c61687c;
      param_2 = in_xd12;
      if (in_FPSCR_SZ == '\0') {
        param_2 = (double)CONCAT44(SUB84(dVar32,0),uVar33);
      }
      iVar7 = *(int *)((int)pdVar18 + -0x44);
      if (in_FPSCR_SZ == '\0') {
        dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),*(undefined4 *)(pdVar13 + 2));
        uVar34 = CONCAT44(*(undefined4 *)((int)pdVar13 + 0x14),SUB84(in_dr14,0));
        param_1 = (double)((ulonglong)in_dr14 >> 0x20);
      }
      else {
        in_xd12 = pdVar13[2];
        uVar34 = *(undefined8 *)((int)pdVar13 + 0x14);
        param_1 = dVar26;
        in_xd4 = in_dr14;
      }
      uVar33 = SUB84(param_1,0);
      in_dr14 = (double)CONCAT44((int)((ulonglong)uVar34 >> 0x20),-(float)uVar34);
      *(int *)((int)pdVar18 + -0x44) = iVar7 + -1;
      pdVar11 = pdVar12;
      pdVar12 = pdVar13;
    } while (iVar7 + -1 != 0);
    if (pdVar9 < unaff_r8) {
      if (in_FPSCR_SZ == '\0') {
        dVar22 = (double)CONCAT44(*(undefined4 *)pdVar13,SUB84(dVar22,0));
        fVar25 = *(float *)((int)pdVar13 + 4);
        pdVar13 = pdVar13 + 1;
      }
      else {
        dVar22 = *pdVar13;
        fVar25 = (float)((ulonglong)pdVar13[1] >> 0x20);
        pdVar13 = pdVar13 + 2;
      }
      in_dr2 = (double)CONCAT44(fVar25,0x3f800000);
                    /* WARNING: Ignoring partial resolution of indirect */
      if (in_FPSCR_PR == '\0') {
        fVar35 = SUB84(dVar22,0);
        fVar36 = (float)((ulonglong)dVar22 >> 0x20);
        fVar19 = SUB84(in_xd0,0) * fVar35 + SUB84(in_xd4,0) * fVar36 + (float)in_xd8 * 1.0 +
                 SUB84(in_xd12,0) * fVar25;
        fVar19 = (float)((ulonglong)in_xd0 >> 0x20) * fVar35 +
                 (float)((ulonglong)in_xd4 >> 0x20) * fVar36 +
                 (float)((ulonglong)in_xd8 >> 0x20) * 1.0 +
                 (float)((ulonglong)in_xd12 >> 0x20) * fVar25;
        fVar19 = SUB84(in_xd2,0) * fVar35 + (float)in_xd6 * fVar36 + in_xf10 * 1.0 +
                 SUB84(in_xd14,0) * fVar25;
        fVar25 = (float)((ulonglong)in_xd2 >> 0x20) * fVar35 +
                 (float)((ulonglong)in_xd6 >> 0x20) * fVar36 + in_xf11 * 1.0 +
                 (float)((ulonglong)in_xd14 >> 0x20) * fVar25;
      }
      if (in_FPSCR_SZ == '\0') {
        uVar27 = *(undefined4 *)pdVar13;
        param_1 = (double)CONCAT44(uVar27,uVar33);
        param_2 = (double)CONCAT44(*(undefined4 *)((int)pdVar13 + 4),SUB84(param_2,0));
        *(undefined4 *)((int)unaff_r8 + 0xc) = *(undefined4 *)((int)pdVar13 + 4);
        *(undefined4 *)(unaff_r8 + 1) = uVar33;
        pdVar9 = (double *)((int)unaff_r8 + 4);
        *(undefined4 *)pdVar9 = uVar27;
      }
      else {
        param_1 = *pdVar13;
        param_2 = pdVar13[1];
        unaff_r8[1] = param_2;
        *unaff_r8 = in_xd4;
        pdVar9 = unaff_r8 + -1;
        *pdVar9 = param_1;
      }
      *(undefined4 *)((int)pdVar9 + -4) = uVar28;
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar9 + 0x14) = (int)((ulonglong)in_dr2 >> 0x20);
        *(int *)(pdVar9 + 2) = SUB84(dVar22,0);
        pdVar9 = (double *)((int)pdVar9 + 0xc);
        *(int *)pdVar9 = (int)((ulonglong)dVar22 >> 0x20);
      }
      else {
        pdVar9[2] = in_dr2;
        pdVar9[1] = in_xd0;
        *pdVar9 = dVar22;
      }
      unaff_r8 = pdVar9 + 2;
    }
    iVar7 = *(int *)((int)pdVar18 + -0x38);
    *(int *)((int)pdVar18 + -0x38) = iVar7 + -1;
    if (iVar7 + -1 == 0) {
      *DAT_8c616898 = *DAT_8c616898 + *(int *)((int)pdVar18 + -0x48);
      *DAT_8c61689c = *DAT_8c61689c + *(int *)((int)pdVar18 + -0x48);
      return;
    }
  } while( true );
}

