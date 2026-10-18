/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c615934) */
/* WARNING: Removing unreachable block (ram,0x8c615af8) */
/* WARNING: Restarted to delay deadcode elimination for space: register */

void FUN_8c61572c(double param_1,double param_2,double param_3,double param_4,undefined4 param_5)

{
  ushort uVar1;
  int iVar2;
  undefined *puVar3;
  int *piVar4;
  int iVar5;
  int iVar6;
  double *pdVar7;
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
  double dVar19;
  float fVar20;
  float fVar21;
  double in_dr2;
  uint uVar22;
  float fVar23;
  undefined4 uVar24;
  undefined4 uVar26;
  double dVar25;
  undefined4 uVar29;
  double dVar27;
  double dVar28;
  undefined8 in_dr12;
  double dVar30;
  undefined4 uVar31;
  double in_dr14;
  undefined8 uVar32;
  double in_xd0;
  double in_xd2;
  double dVar33;
  double in_xd4;
  double in_xd6;
  double in_xd8;
  double in_xd12;
  double in_xd14;
  bool bVar34;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  int aiStack_48 [4];
  undefined1 local_38 [16];
  double dStack_28;
  undefined8 uStack_20;
  
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
  dVar30 = (double)CONCAT44(0x3f800000,(int)in_dr12);
  *(undefined4 *)((int)pdVar18 + -0x48) = param_5;
  dVar19 = (double)(*(code *)PTR_FUN_8c61577c)();
  puVar15 = *(ushort **)(*(int *)((int)pdVar18 + -0x48) + 4);
  *(uint *)((int)pdVar18 + -0x38) = (uint)*(ushort *)(*(int *)((int)pdVar18 + -0x48) + 2);
  iVar14 = *DAT_8c615780;
  *(undefined4 *)((int)pdVar18 + -0x48) = 0;
  do {
    uVar31 = SUB84(in_dr14,0);
    uVar1 = *puVar15;
    puVar16 = puVar15 + 1;
    *(uint *)((int)pdVar18 + -0x44) = (uint)uVar1;
    uVar24 = (undefined4)((ulonglong)dVar30 >> 0x20);
    dVar25 = dVar30;
    if (in_FPSCR_SZ == '\0') {
      dVar25 = in_xd14;
      uVar31 = uVar24;
    }
    in_xd14 = dVar25;
    if ((uVar1 & DAT_8c6158a0) != 0) {
      if (in_FPSCR_PR == '\0') {
        param_1 = (double)CONCAT44(0xbf800000,SUB84(param_1,0));
      }
      else {
        param_1 = -(double)CONCAT44(0x3f800000,SUB84(param_1,0));
      }
      *(uint *)((int)pdVar18 + -0x44) = *(uint *)((int)pdVar18 + -0x44) & (int)DAT_8c61589c;
      in_xd14 = param_1;
      if (in_FPSCR_SZ == '\0') {
        uVar31 = (undefined4)((ulonglong)param_1 >> 0x20);
        in_xd14 = dVar25;
      }
    }
    *(int *)((int)pdVar18 + -0x48) = *(int *)((int)pdVar18 + -0x48) + *(int *)((int)pdVar18 + -0x44)
    ;
    *(int *)((int)pdVar18 + -0x44) = *(int *)((int)pdVar18 + -0x44) + -2;
    puVar17 = puVar15 + 2;
    puVar15 = puVar15 + 3;
    pdVar12 = (double *)((uint)*puVar17 * 0x20 + iVar14);
    pdVar11 = (double *)((uint)*puVar16 * 0x20 + iVar14);
    pdVar7 = unaff_r8;
    if (in_FPSCR_SZ == '\0') {
      dVar30 = (double)CONCAT44(uVar24,*(undefined4 *)(pdVar12 + 2));
      param_2 = (double)CONCAT44(*(undefined4 *)(pdVar11 + 2),SUB84(param_2,0));
      in_dr14 = (double)CONCAT44(*(undefined4 *)((int)pdVar12 + 0x14),uVar31);
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
      uVar31 = (undefined4)((ulonglong)dVar30 >> 0x20);
      dVar25 = dVar30;
      if (in_FPSCR_SZ == '\0') {
        dVar25 = (double)CONCAT44(uVar31,SUB84(param_1,0));
      }
      if ((*(uint *)((int)pdVar11 + 0xc) & *(uint *)((int)pdVar12 + 0xc) &
           *(uint *)((int)pdVar13 + 0xc) & 0x3f) == 0) {
        if (((*(uint *)((int)pdVar11 + 0xc) | *(uint *)((int)pdVar12 + 0xc) |
             *(uint *)((int)pdVar13 + 0xc)) & 1) == 0) {
          if (in_FPSCR_SZ == '\0') {
            dVar27 = (double)CONCAT44(*(undefined4 *)((int)pdVar13 + 0x14),SUB84(dVar30,0));
          }
          else {
            dVar27 = *(double *)((int)pdVar13 + 0x14);
            in_xd2 = in_xd12;
          }
          fVar21 = (float)((ulonglong)in_dr14 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) - fVar21,
                                      SUB84(dVar27,0) - (float)((ulonglong)param_2 >> 0x20));
          }
          else {
            dVar27 = (dVar27 - param_2) - in_dr14;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),*(undefined4 *)(pdVar13 + 2))
            ;
          }
          else {
            in_xd0 = pdVar13[2];
          }
          if (in_FPSCR_PR == '\0') {
            dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),
                                      SUB84(dVar19,0) - SUB84(dVar30,0));
            fVar20 = SUB84(dVar27,0) * (float)((ulonglong)dVar27 >> 0x20);
          }
          else {
            dVar19 = dVar19 - dVar30;
            fVar20 = SUB84(dVar27 * dVar27,0);
          }
          dVar27 = in_dr14;
          if (in_FPSCR_SZ == '\0') {
            dVar27 = (double)CONCAT44(fVar21,fVar20);
          }
          if (in_FPSCR_PR == '\0') {
            fVar21 = ((float)((ulonglong)dVar27 >> 0x20) - SUB84(dVar25,0)) * SUB84(dVar19,0);
            in_dr2 = (double)CONCAT44(fVar21,SUB84(dVar27,0) - fVar21);
          }
          else {
            in_dr2 = (dVar27 - dVar25) * dVar19;
            in_dr2 = in_dr2 - in_dr2;
          }
          dVar27 = in_xd2;
          if (in_FPSCR_SZ == '\0') {
            dVar27 = (double)CONCAT44(SUB84(in_dr2,0),SUB84(dVar25,0));
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) * SUB84(in_dr14,0),
                                      SUB84(dVar27,0));
          }
          else {
            dVar25 = dVar27 * in_dr14;
          }
        }
        else {
          puVar8 = (undefined4 *)((int)pdVar18 + -0x10);
          *puVar8 = pdVar11;
          *(double **)((int)pdVar18 + -0xc) = pdVar12;
          *(double **)((int)pdVar18 + -8) = pdVar13;
          iVar6 = 0;
          while( true ) {
            iVar2 = DAT_8c6158a4;
            pdVar9 = (double *)((int)pdVar18 + -0x34);
            uVar26 = (undefined4)((ulonglong)in_dr2 >> 0x20);
            uVar24 = SUB84(dVar25,0);
            if (2 < iVar6) break;
            piVar4 = puVar8 + iVar6;
            *(int **)((int)pdVar18 + -0x4c) = piVar4;
            *(int *)((int)pdVar18 + -0x50) = iVar6 * 0xc + (int)pdVar9;
            iVar6 = iVar6 + 1;
            if ((*(uint *)(*piVar4 + 0xc) & 1) == 0) {
              dVar25 = dVar30;
              if (in_FPSCR_SZ == '\0') {
                dVar25 = (double)CONCAT44(uVar31,uVar24);
              }
              iVar5 = **(int **)((int)pdVar18 + -0x4c);
              *(int *)((int)pdVar18 + -0x40) = iVar5;
              if (in_FPSCR_SZ == '\0') {
                in_dr2 = (double)CONCAT44(uVar26,*(undefined4 *)(iVar5 + 0x18));
              }
              else {
                in_xd2 = *(double *)(iVar5 + 0x18);
              }
              if (in_FPSCR_PR == '\0') {
                dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) / SUB84(in_dr2,0),
                                          SUB84(dVar25,0));
              }
              else {
                dVar25 = dVar25 / in_dr2;
              }
              if (in_FPSCR_SZ == '\0') {
                in_dr2 = (double)(ulonglong)*(uint *)(DAT_8c6158a4 + 0xc);
              }
              else {
                in_xd2 = *(double *)(DAT_8c6158a4 + 0xc);
              }
              if (in_FPSCR_SZ == '\0') {
                dVar27 = (double)CONCAT44(*(undefined4 *)(*(int *)((int)pdVar18 + -0x40) + 0x10),
                                          SUB84(in_dr2,0));
              }
              else {
                dVar27 = *(double *)(*(int *)((int)pdVar18 + -0x40) + 0x10);
              }
              if (in_FPSCR_PR == '\0') {
                dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) - SUB84(dVar27,0),
                                          SUB84(dVar27,0));
              }
              else {
                dVar27 = dVar27 - dVar27;
              }
              if (in_FPSCR_SZ == '\0') {
                dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),
                                          *(undefined4 *)DAT_8c6158a8);
              }
              else {
                in_xd2 = *DAT_8c6158a8;
              }
              fVar21 = (float)((ulonglong)dVar25 >> 0x20);
              if (in_FPSCR_PR == '\0') {
                dVar27 = (double)CONCAT44(((float)((ulonglong)dVar27 >> 0x20) * fVar21) /
                                          SUB84(dVar27,0),SUB84(dVar27,0));
              }
              else {
                dVar27 = (dVar27 * dVar25) / (dVar27 * dVar25);
              }
              uVar24 = (undefined4)((ulonglong)dVar27 >> 0x20);
              if (in_FPSCR_SZ == '\0') {
                *(undefined4 *)*(double **)((int)pdVar18 + -0x50) = uVar24;
              }
              else {
                **(double **)((int)pdVar18 + -0x50) = dVar27;
              }
              if (in_FPSCR_SZ == '\0') {
                dVar27 = (double)CONCAT44(uVar24,*(undefined4 *)(iVar2 + 0x10));
              }
              else {
                in_xd2 = *(double *)(iVar2 + 0x10);
              }
              if (in_FPSCR_SZ == '\0') {
                dVar27 = (double)CONCAT44(*(undefined4 *)(**(int **)((int)pdVar18 + -0x4c) + 0x14),
                                          SUB84(dVar27,0));
              }
              else {
                dVar27 = *(double *)(**(int **)((int)pdVar18 + -0x4c) + 0x14);
              }
              if (in_FPSCR_PR == '\0') {
                dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) - SUB84(dVar27,0),
                                          SUB84(dVar27,0));
              }
              else {
                dVar27 = dVar27 - dVar27;
              }
              if (in_FPSCR_SZ == '\0') {
                dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),
                                          *(undefined4 *)DAT_8c6158ac);
              }
              else {
                in_xd2 = *DAT_8c6158ac;
              }
              if (in_FPSCR_PR == '\0') {
                in_dr2 = (double)CONCAT44(((float)((ulonglong)dVar27 >> 0x20) * fVar21) /
                                          SUB84(dVar27,0),SUB84(dVar27,0));
              }
              else {
                in_dr2 = (dVar27 * dVar25) / (dVar27 * dVar25);
              }
              if (in_FPSCR_SZ == '\0') {
                *(int *)(*(int *)((int)pdVar18 + -0x50) + 4) = (int)((ulonglong)in_dr2 >> 0x20);
              }
              else {
                *(double *)(*(int *)((int)pdVar18 + -0x50) + 4) = in_dr2;
              }
              if (in_FPSCR_SZ == '\0') {
                *(float *)(*(int *)((int)pdVar18 + -0x50) + 8) = fVar21;
              }
              else {
                *(double *)(*(int *)((int)pdVar18 + -0x50) + 8) = dVar25;
              }
            }
            else {
              if (in_FPSCR_SZ == '\0') {
                uVar24 = *(undefined4 *)(**(int **)((int)pdVar18 + -0x4c) + 0x10);
                in_dr2 = (double)CONCAT44(uVar26,uVar24);
                *(undefined4 *)*(double **)((int)pdVar18 + -0x50) = uVar24;
              }
              else {
                in_xd2 = *(double *)(**(int **)((int)pdVar18 + -0x4c) + 0x10);
                **(double **)((int)pdVar18 + -0x50) = in_xd2;
              }
              if (in_FPSCR_SZ == '\0') {
                uVar24 = *(undefined4 *)(**(int **)((int)pdVar18 + -0x4c) + 0x14);
                in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),uVar24);
                *(undefined4 *)(*(int *)((int)pdVar18 + -0x50) + 4) = uVar24;
              }
              else {
                in_xd2 = *(double *)(**(int **)((int)pdVar18 + -0x4c) + 0x14);
                *(double *)(*(int *)((int)pdVar18 + -0x50) + 4) = in_xd2;
              }
              if (in_FPSCR_SZ == '\0') {
                uVar24 = *(undefined4 *)(**(int **)((int)pdVar18 + -0x4c) + 0x18);
                in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),uVar24);
                *(undefined4 *)(*(int *)((int)pdVar18 + -0x50) + 8) = uVar24;
              }
              else {
                in_xd2 = *(double *)(**(int **)((int)pdVar18 + -0x4c) + 0x18);
                *(double *)(*(int *)((int)pdVar18 + -0x50) + 8) = in_xd2;
              }
            }
          }
          if (in_FPSCR_SZ == '\0') {
            uVar29 = *(undefined4 *)((int)pdVar18 + -0x2c);
            dVar19 = (double)CONCAT44(uVar29,SUB84(dVar19,0));
            param_2 = (double)CONCAT44(uVar29,SUB84(param_2,0));
          }
          else {
            dVar19 = *(double *)((int)pdVar18 + -0x2c);
            uVar29 = (undefined4)((ulonglong)dVar19 >> 0x20);
            param_2 = dVar19;
          }
          pdVar10 = (double *)((int)pdVar18 + -0x28);
          *(double **)((int)pdVar18 + -0x50) = pdVar10;
          if (in_FPSCR_SZ == '\0') {
            uVar24 = *(undefined4 *)((int)pdVar18 + -0x30);
            param_2 = (double)CONCAT44(uVar29,*(undefined4 *)((int)pdVar18 + -0x24));
            param_3 = (double)CONCAT44(*(undefined4 *)((int)pdVar18 + -0x20),SUB84(param_3,0));
            in_dr2 = (double)CONCAT44(uVar26,uVar24);
          }
          else {
            in_xd2 = *(double *)((int)pdVar18 + -0x30);
            in_xd6 = *(double *)((int)pdVar18 + -0x24);
            param_3 = *(double *)((int)pdVar18 + -0x20);
            in_xd4 = in_xd2;
          }
          fVar21 = (float)((ulonglong)param_3 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) * SUB84(param_2,0),
                                      SUB84(dVar19,0));
            fVar20 = SUB84(in_dr2,0) * fVar21;
          }
          else {
            dVar19 = dVar19 * param_2;
            fVar20 = SUB84(in_dr2 * param_3,0);
          }
          if (in_FPSCR_SZ == '\0') {
            uVar26 = *(undefined4 *)pdVar9;
            param_3 = (double)CONCAT44(fVar21,*(undefined4 *)pdVar10);
            dVar27 = (double)CONCAT44(uVar26,fVar20);
            dVar25 = (double)CONCAT44(uVar26,uVar24);
          }
          else {
            dVar27 = *pdVar9;
            in_xd8 = *pdVar10;
            uVar26 = (undefined4)((ulonglong)dVar27 >> 0x20);
            dVar25 = dVar27;
          }
          if (in_FPSCR_PR == '\0') {
            dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) *
                                      (float)((ulonglong)param_3 >> 0x20),SUB84(dVar27,0));
          }
          else {
            dVar27 = dVar27 * param_3;
          }
          uVar24 = SUB84(dVar25,0);
          dVar28 = in_xd4;
          if (in_FPSCR_SZ == '\0') {
            dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),uVar24);
            dVar28 = in_xd0;
          }
          if (in_FPSCR_PR == '\0') {
            dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) - SUB84(dVar27,0),
                                      SUB84(dVar19,0));
          }
          else {
            dVar19 = dVar19 - dVar27;
          }
          dVar33 = param_2;
          if (in_FPSCR_SZ == '\0') {
            dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),
                                      (int)((ulonglong)param_2 >> 0x20));
            dVar33 = in_xd2;
          }
          if (in_FPSCR_PR == '\0') {
            fVar21 = SUB84(dVar27,0) * SUB84(param_3,0);
            dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) - fVar21,fVar21);
          }
          else {
            dVar27 = dVar27 * param_3 - dVar27 * param_3;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),
                                      *(undefined4 *)((int)pdVar18 + -0x18));
          }
          else {
            dVar33 = *(double *)((int)pdVar18 + -0x18);
          }
          if (in_FPSCR_PR == '\0') {
            fVar21 = SUB84(dVar27,0) * (float)((ulonglong)dVar27 >> 0x20);
          }
          else {
            fVar21 = SUB84(dVar27 * dVar27,0);
          }
          if (in_FPSCR_SZ == '\0') {
            uVar32 = CONCAT44(*(undefined4 *)((int)pdVar18 + -0x1c),fVar21);
          }
          else {
            uVar32 = *(undefined8 *)((int)pdVar18 + -0x1c);
          }
          if (in_FPSCR_PR == '\0') {
            fVar21 = (float)((ulonglong)uVar32 >> 0x20);
            uVar32 = CONCAT44(fVar21,(float)((ulonglong)dVar19 >> 0x20) * fVar21 + (float)uVar32);
          }
          dVar27 = dVar33;
          if (in_FPSCR_SZ == '\0') {
            dVar27 = (double)CONCAT44((int)uVar32,uVar26);
            dVar25 = dVar33;
          }
          if (in_FPSCR_PR == '\0') {
            fVar21 = SUB84(dVar27,0) * SUB84(param_2,0);
            dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),fVar21);
            fVar21 = SUB84(dVar19,0) * SUB84(param_3,0) - fVar21;
          }
          else {
            dVar27 = dVar27 * param_2;
            fVar21 = SUB84(dVar19 * param_3 - dVar27,0);
          }
          if (in_FPSCR_SZ == '\0') {
            dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),
                                      *(undefined4 *)((int)pdVar18 + -0x14));
            dVar19 = (double)CONCAT44(fVar21,fVar21);
          }
          else {
            dVar25 = *(double *)((int)pdVar18 + -0x14);
            dVar19 = dVar28;
          }
          if (in_FPSCR_PR == '\0') {
            dVar27 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) * SUB84(dVar27,0) +
                                      (float)((ulonglong)dVar27 >> 0x20),SUB84(dVar27,0));
          }
          in_xd2 = in_xd14;
          if (in_FPSCR_SZ == '\0') {
            dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),SUB84(in_dr14,0));
            in_xd2 = dVar25;
          }
          uVar26 = (undefined4)((ulonglong)dVar27 >> 0x20);
          fVar21 = -SUB84(dVar27,0);
          dVar25 = (double)CONCAT44(uVar26,fVar21);
          dVar27 = dVar25;
          if (in_FPSCR_SZ == '\0') {
            dVar27 = (double)CONCAT44(uVar26,uVar24);
          }
          if (in_FPSCR_PR == '\0') {
            dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) * fVar21,SUB84(dVar27,0));
          }
          else {
            dVar27 = dVar27 * dVar25;
          }
          in_dr2 = (double)(ulonglong)(uint)fVar21;
          if (in_FPSCR_PR == '\0') {
            bVar34 = 0.0 < (float)((ulonglong)dVar27 >> 0x20);
          }
          else {
            bVar34 = in_dr2 < dVar27;
          }
          dVar25 = dVar27;
          in_xd0 = dVar28;
          if ((!bVar34) &&
             (*(undefined4 *)((int)pdVar18 + -0x3c) = 1, dVar25 = dVar30, in_FPSCR_SZ == '\0')) {
            dVar25 = (double)CONCAT44(uVar31,SUB84(dVar27,0));
          }
        }
      }
      uVar26 = DAT_8c615bc0;
      uVar24 = DAT_8c615bbc;
      uVar31 = SUB84(param_3,0);
      uVar22 = (uint)((ulonglong)in_dr2 >> 0x20);
      fVar21 = (float)((ulonglong)dVar25 >> 0x20);
      uVar29 = SUB84(dVar19,0);
      if (pdVar7 < unaff_r8) {
        dVar27 = (double)((ulonglong)uVar22 << 0x20);
        if (in_FPSCR_PR == '\0') {
          bVar34 = fVar21 < 0.0;
        }
        else {
          bVar34 = dVar25 < dVar27;
        }
        if (bVar34) {
LAB_8c6159fc:
          uVar24 = DAT_8c615bc0;
          uVar31 = SUB84(param_3,0);
          if (in_FPSCR_SZ == '\0') {
            dVar25 = (double)CONCAT44(*(undefined4 *)pdVar12,SUB84(dVar25,0));
            dVar28 = (double)CONCAT44(*(undefined4 *)((int)pdVar12 + 4),SUB84(param_2,0));
            pdVar9 = pdVar12 + 1;
            in_dr2 = (double)CONCAT44(SUB84(dVar27,0),SUB84(dVar27,0));
            dVar19 = (double)CONCAT44(SUB84(dVar19,0),SUB84(dVar19,0));
          }
          else {
            dVar25 = *pdVar12;
            dVar28 = pdVar12[1];
            pdVar9 = pdVar12 + 2;
            in_dr2 = in_xd2;
            dVar19 = in_xd0;
          }
          in_dr2 = (double)((ulonglong)in_dr2 & 0xffffffff00000000);
                    /* WARNING: Ignoring partial resolution of indirect */
          fVar20 = SUB84(dVar28,0);
          fVar21 = SUB84(dVar25,0);
          if (in_FPSCR_PR == '\0') {
            fVar23 = SUB84(dVar19,0) * fVar21 +
                     (float)((ulonglong)dVar19 >> 0x20) * (float)((ulonglong)dVar25 >> 0x20) +
                     fVar20 * 0.0 +
                     (float)((ulonglong)in_dr2 >> 0x20) * (float)((ulonglong)dVar28 >> 0x20);
          }
          if (in_FPSCR_SZ == '\0') {
            param_3 = (double)CONCAT44(*(undefined4 *)pdVar9,uVar31);
            param_4 = (double)CONCAT44(*(undefined4 *)((int)pdVar9 + 4),SUB84(param_4,0));
            pdVar9 = pdVar9 + 1;
            dVar25 = (double)CONCAT44(fVar21,fVar21);
            dVar19 = (double)CONCAT44(uVar31,SUB84(dVar19,0));
          }
          else {
            param_3 = *pdVar9;
            param_4 = pdVar9[1];
            pdVar9 = pdVar9 + 2;
            uVar31 = SUB84(param_3,0);
            dVar25 = in_xd4;
            dVar19 = in_xd8;
          }
          if (in_FPSCR_PR == '\0') {
            dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) - SUB84(in_dr2,0),
                                      SUB84(dVar19,0));
          }
          else {
            dVar19 = dVar19 - in_dr2;
          }
          fVar21 = (float)((ulonglong)dVar19 >> 0x20);
          if (in_FPSCR_SZ == '\0') {
            pdVar10 = (double *)((int)unaff_r8 + 0xc);
            *(int *)pdVar10 = (int)((ulonglong)param_4 >> 0x20);
            pdVar9 = (double *)((int)pdVar9 + -4);
            *(float *)pdVar9 = fVar21;
            param_2 = (double)CONCAT44(fVar20,fVar20);
          }
          else {
            pdVar10 = unaff_r8 + 1;
            *pdVar10 = param_4;
            pdVar9 = pdVar9 + -1;
            *pdVar9 = dVar19;
            param_2 = in_xd6;
          }
          CacheBlockWriteBack(pdVar9);
          if (in_FPSCR_PR == '\0') {
            param_2 = (double)CONCAT44((int)((ulonglong)param_2 >> 0x20),SUB84(param_2,0) * fVar21);
          }
          else {
            param_2 = param_2 * dVar19;
          }
          if (in_FPSCR_SZ == '\0') {
            pdVar10 = (double *)((int)pdVar10 + -4);
            *(undefined4 *)pdVar10 = uVar31;
          }
          else {
            pdVar10 = pdVar10 + -1;
            *pdVar10 = in_xd8;
          }
          if (in_FPSCR_PR == '\0') {
            param_2 = (double)CONCAT44((float)((ulonglong)param_2 >> 0x20) * fVar21,SUB84(param_2,0)
                                      );
          }
          else {
            param_2 = param_2 * dVar19;
          }
          if (in_FPSCR_SZ == '\0') {
            pdVar10 = (double *)((int)pdVar10 + -4);
            *(int *)pdVar10 = (int)((ulonglong)param_3 >> 0x20);
          }
          else {
            pdVar10 = pdVar10 + -1;
            *pdVar10 = param_3;
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),SUB84(dVar25,0) * fVar21);
          }
          else {
            dVar25 = dVar25 * dVar19;
          }
          *(undefined4 *)((int)pdVar10 + -4) = uVar24;
          if (in_FPSCR_SZ == '\0') {
            *(int *)(pdVar10 + 3) = SUB84(param_2,0);
            *(int *)((int)pdVar10 + 0x14) = (int)((ulonglong)param_2 >> 0x20);
            *(int *)(pdVar10 + 2) = SUB84(dVar25,0);
            pdVar10 = (double *)((int)pdVar10 + 0xc);
            *(int *)pdVar10 = (int)((ulonglong)dVar25 >> 0x20);
          }
          else {
            *(double *)((int)pdVar10 + 0x14) = in_xd6;
            *(double *)((int)pdVar10 + 0xc) = param_2;
            *(double *)((int)pdVar10 + 4) = in_xd4;
            pdVar10 = (double *)((int)pdVar10 + -4);
            *pdVar10 = dVar25;
          }
          unaff_r8 = pdVar10 + 2;
        }
        else {
          if (in_FPSCR_SZ == '\0') {
            dVar25 = (double)CONCAT44(*(undefined4 *)pdVar12,SUB84(dVar25,0));
            dVar27 = (double)CONCAT44(*(undefined4 *)((int)pdVar12 + 4),SUB84(param_2,0));
            pdVar7 = pdVar12 + 1;
            in_dr2 = 0.0;
            dVar19 = (double)CONCAT44(uVar29,uVar29);
          }
          else {
            dVar25 = *pdVar12;
            dVar27 = pdVar12[1];
            pdVar7 = pdVar12 + 2;
            in_dr2 = in_xd2;
            dVar19 = in_xd0;
          }
          in_dr2 = (double)((ulonglong)in_dr2 & 0xffffffff00000000);
                    /* WARNING: Ignoring partial resolution of indirect */
          fVar20 = SUB84(dVar27,0);
          fVar21 = SUB84(dVar25,0);
          if (in_FPSCR_PR == '\0') {
            fVar23 = SUB84(dVar19,0) * fVar21 +
                     (float)((ulonglong)dVar19 >> 0x20) * (float)((ulonglong)dVar25 >> 0x20) +
                     fVar20 * 0.0 +
                     (float)((ulonglong)in_dr2 >> 0x20) * (float)((ulonglong)dVar27 >> 0x20);
          }
          if (in_FPSCR_SZ == '\0') {
            param_3 = (double)CONCAT44(*(undefined4 *)pdVar7,uVar31);
            param_4 = (double)CONCAT44(*(undefined4 *)((int)pdVar7 + 4),SUB84(param_4,0));
            pdVar7 = pdVar7 + 1;
            dVar25 = (double)CONCAT44(fVar21,fVar21);
            dVar19 = (double)CONCAT44(uVar31,SUB84(dVar19,0));
          }
          else {
            param_3 = *pdVar7;
            param_4 = pdVar7[1];
            pdVar7 = pdVar7 + 2;
            uVar31 = SUB84(param_3,0);
            dVar25 = in_xd4;
            dVar19 = in_xd8;
          }
          if (in_FPSCR_PR == '\0') {
            dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) - SUB84(in_dr2,0),
                                      SUB84(dVar19,0));
          }
          else {
            dVar19 = dVar19 - in_dr2;
          }
          fVar21 = (float)((ulonglong)dVar19 >> 0x20);
          if (in_FPSCR_SZ == '\0') {
            pdVar9 = (double *)((int)unaff_r8 + 0xc);
            *(int *)pdVar9 = (int)((ulonglong)param_4 >> 0x20);
            pdVar7 = (double *)((int)pdVar7 + -4);
            *(float *)pdVar7 = fVar21;
            param_2 = (double)CONCAT44(fVar20,fVar20);
          }
          else {
            pdVar9 = unaff_r8 + 1;
            *pdVar9 = param_4;
            pdVar7 = pdVar7 + -1;
            *pdVar7 = dVar19;
            param_2 = in_xd6;
          }
          CacheBlockWriteBack(pdVar7);
          if (in_FPSCR_PR == '\0') {
            param_2 = (double)CONCAT44((int)((ulonglong)param_2 >> 0x20),SUB84(param_2,0) * fVar21);
          }
          else {
            param_2 = param_2 * dVar19;
          }
          if (in_FPSCR_SZ == '\0') {
            pdVar9 = (double *)((int)pdVar9 + -4);
            *(undefined4 *)pdVar9 = uVar31;
          }
          else {
            pdVar9 = pdVar9 + -1;
            *pdVar9 = in_xd8;
          }
          if (in_FPSCR_PR == '\0') {
            param_2 = (double)CONCAT44((float)((ulonglong)param_2 >> 0x20) * fVar21,SUB84(param_2,0)
                                      );
          }
          else {
            param_2 = param_2 * dVar19;
          }
          if (in_FPSCR_SZ == '\0') {
            pdVar9 = (double *)((int)pdVar9 + -4);
            *(int *)pdVar9 = (int)((ulonglong)param_3 >> 0x20);
          }
          else {
            pdVar9 = pdVar9 + -1;
            *pdVar9 = param_3;
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),SUB84(dVar25,0) * fVar21);
          }
          else {
            dVar25 = dVar25 * dVar19;
          }
          *(undefined4 *)((int)pdVar9 + -4) = uVar24;
          if (in_FPSCR_SZ == '\0') {
            *(int *)(pdVar9 + 3) = SUB84(param_2,0);
            *(int *)((int)pdVar9 + 0x14) = (int)((ulonglong)param_2 >> 0x20);
            *(int *)(pdVar9 + 2) = SUB84(dVar25,0);
            pdVar9 = (double *)((int)pdVar9 + 0xc);
            *(int *)pdVar9 = (int)((ulonglong)dVar25 >> 0x20);
          }
          else {
            *(double *)((int)pdVar9 + 0x14) = in_xd6;
            *(double *)((int)pdVar9 + 0xc) = param_2;
            *(double *)((int)pdVar9 + 4) = in_xd4;
            pdVar9 = (double *)((int)pdVar9 + -4);
            *pdVar9 = dVar25;
          }
          unaff_r8 = pdVar9 + 2;
          pdVar7 = unaff_r8;
        }
      }
      else {
        in_dr2 = (double)((ulonglong)uVar22 << 0x20);
        if (in_FPSCR_PR == '\0') {
          bVar34 = fVar21 < 0.0;
        }
        else {
          bVar34 = dVar25 < in_dr2;
        }
        if (bVar34) {
          if (in_FPSCR_SZ == '\0') {
            dVar25 = (double)CONCAT44(*(undefined4 *)pdVar11,SUB84(dVar25,0));
            dVar28 = (double)CONCAT44(*(undefined4 *)((int)pdVar11 + 4),SUB84(param_2,0));
            pdVar9 = pdVar11 + 1;
            dVar27 = 0.0;
            dVar19 = (double)CONCAT44(uVar29,uVar29);
          }
          else {
            dVar25 = *pdVar11;
            dVar28 = pdVar11[1];
            pdVar9 = pdVar11 + 2;
            dVar27 = in_xd2;
            dVar19 = in_xd0;
          }
          dVar27 = (double)((ulonglong)dVar27 & 0xffffffff00000000);
                    /* WARNING: Ignoring partial resolution of indirect */
          fVar20 = SUB84(dVar28,0);
          fVar21 = SUB84(dVar25,0);
          if (in_FPSCR_PR == '\0') {
            fVar23 = SUB84(dVar19,0) * fVar21 +
                     (float)((ulonglong)dVar19 >> 0x20) * (float)((ulonglong)dVar25 >> 0x20) +
                     fVar20 * 0.0 +
                     (float)((ulonglong)dVar27 >> 0x20) * (float)((ulonglong)dVar28 >> 0x20);
          }
          if (in_FPSCR_SZ == '\0') {
            param_3 = (double)CONCAT44(*(undefined4 *)pdVar9,uVar31);
            param_4 = (double)CONCAT44(*(undefined4 *)((int)pdVar9 + 4),SUB84(param_4,0));
            pdVar9 = pdVar9 + 1;
            dVar25 = (double)CONCAT44(fVar21,fVar21);
            dVar19 = (double)CONCAT44(uVar31,SUB84(dVar19,0));
          }
          else {
            param_3 = *pdVar9;
            param_4 = pdVar9[1];
            pdVar9 = pdVar9 + 2;
            uVar31 = SUB84(param_3,0);
            dVar25 = in_xd4;
            dVar19 = in_xd8;
          }
          if (in_FPSCR_PR == '\0') {
            dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) - SUB84(dVar27,0),
                                      SUB84(dVar19,0));
          }
          else {
            dVar19 = dVar19 - dVar27;
          }
          fVar21 = (float)((ulonglong)dVar19 >> 0x20);
          if (in_FPSCR_SZ == '\0') {
            pdVar10 = (double *)((int)unaff_r8 + 0xc);
            *(int *)pdVar10 = (int)((ulonglong)param_4 >> 0x20);
            pdVar9 = (double *)((int)pdVar9 + -4);
            *(float *)pdVar9 = fVar21;
            param_2 = (double)CONCAT44(fVar20,fVar20);
          }
          else {
            pdVar10 = unaff_r8 + 1;
            *pdVar10 = param_4;
            pdVar9 = pdVar9 + -1;
            *pdVar9 = dVar19;
            param_2 = in_xd6;
          }
          CacheBlockWriteBack(pdVar9);
          if (in_FPSCR_PR == '\0') {
            param_2 = (double)CONCAT44((int)((ulonglong)param_2 >> 0x20),SUB84(param_2,0) * fVar21);
          }
          else {
            param_2 = param_2 * dVar19;
          }
          if (in_FPSCR_SZ == '\0') {
            pdVar10 = (double *)((int)pdVar10 + -4);
            *(undefined4 *)pdVar10 = uVar31;
          }
          else {
            pdVar10 = pdVar10 + -1;
            *pdVar10 = in_xd8;
          }
          if (in_FPSCR_PR == '\0') {
            param_2 = (double)CONCAT44((float)((ulonglong)param_2 >> 0x20) * fVar21,SUB84(param_2,0)
                                      );
          }
          else {
            param_2 = param_2 * dVar19;
          }
          if (in_FPSCR_SZ == '\0') {
            pdVar10 = (double *)((int)pdVar10 + -4);
            *(int *)pdVar10 = (int)((ulonglong)param_3 >> 0x20);
          }
          else {
            pdVar10 = pdVar10 + -1;
            *pdVar10 = param_3;
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),SUB84(dVar25,0) * fVar21);
          }
          else {
            dVar25 = dVar25 * dVar19;
          }
          *(undefined4 *)((int)pdVar10 + -4) = uVar26;
          if (in_FPSCR_SZ == '\0') {
            *(int *)(pdVar10 + 3) = SUB84(param_2,0);
            *(int *)((int)pdVar10 + 0x14) = (int)((ulonglong)param_2 >> 0x20);
            *(int *)(pdVar10 + 2) = SUB84(dVar25,0);
            pdVar10 = (double *)((int)pdVar10 + 0xc);
            *(int *)pdVar10 = (int)((ulonglong)dVar25 >> 0x20);
          }
          else {
            *(double *)((int)pdVar10 + 0x14) = in_xd6;
            *(double *)((int)pdVar10 + 0xc) = param_2;
            *(double *)((int)pdVar10 + 4) = in_xd4;
            pdVar10 = (double *)((int)pdVar10 + -4);
            *pdVar10 = dVar25;
          }
          unaff_r8 = pdVar10 + 2;
          goto LAB_8c6159fc;
        }
      }
      puVar3 = PTR_FUN_8c615bc8;
      uVar24 = SUB84(param_2,0);
      uVar31 = SUB84(dVar25,0);
      if (*(int *)((int)pdVar18 + -0x3c) != 0) {
        *DAT_8c615bc4 = 0;
        if (in_FPSCR_SZ == '\0') {
          dVar19 = (double)CONCAT44(*(undefined4 *)pdVar11,*(undefined4 *)pdVar12);
        }
        else {
          in_xd2 = *pdVar12;
          dVar19 = *pdVar11;
        }
        if (in_FPSCR_PR == '\0') {
          dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) + SUB84(dVar19,0),
                                    SUB84(dVar19,0));
        }
        else {
          dVar19 = dVar19 + dVar19;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),*(undefined4 *)pdVar13);
        }
        else {
          in_xd2 = *pdVar13;
        }
        if (in_FPSCR_PR == '\0') {
          dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) + SUB84(dVar19,0),
                                    SUB84(dVar19,0));
        }
        else {
          dVar19 = dVar19 + dVar19;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)pdVar18 + -0x10) = (int)((ulonglong)dVar19 >> 0x20);
          dVar19 = (double)CONCAT44(*(undefined4 *)((int)pdVar11 + 4),
                                    *(undefined4 *)((int)pdVar12 + 4));
        }
        else {
          *(double *)((int)pdVar18 + -0x10) = dVar19;
          in_xd2 = *(double *)((int)pdVar12 + 4);
          dVar19 = *(double *)((int)pdVar11 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) + SUB84(dVar19,0),
                                    SUB84(dVar19,0));
        }
        else {
          dVar19 = dVar19 + dVar19;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),
                                    *(undefined4 *)((int)pdVar13 + 4));
        }
        else {
          in_xd2 = *(double *)((int)pdVar13 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) + SUB84(dVar19,0),
                                    SUB84(dVar19,0));
        }
        else {
          dVar19 = dVar19 + dVar19;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)pdVar18 + -0xc) = (int)((ulonglong)dVar19 >> 0x20);
          dVar19 = (double)CONCAT44(*(undefined4 *)(pdVar11 + 1),*(undefined4 *)(pdVar12 + 1));
        }
        else {
          *(double *)((int)pdVar18 + -0xc) = dVar19;
          in_xd2 = pdVar12[1];
          dVar19 = pdVar11[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) + SUB84(dVar19,0),
                                    SUB84(dVar19,0));
        }
        else {
          dVar19 = dVar19 + dVar19;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),*(undefined4 *)(pdVar13 + 1));
        }
        else {
          in_xd2 = pdVar13[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) + SUB84(dVar19,0),
                                    SUB84(dVar19,0));
        }
        else {
          dVar19 = dVar19 + dVar19;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)pdVar18 + -8) = (int)((ulonglong)dVar19 >> 0x20);
        }
        else {
          *(double *)((int)pdVar18 + -8) = dVar19;
        }
        dVar19 = (double)(*(code *)puVar3)((undefined1 *)((int)pdVar18 + -0x10));
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)DAT_8c615bd0,
                                    *(undefined4 *)((int)pdVar18 + -0x10));
          dVar25 = in_xd2;
        }
        else {
          dVar25 = *(double *)((int)pdVar18 + -0x10);
          dVar27 = *DAT_8c615bd0;
        }
        if (in_FPSCR_PR == '\0') {
          dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) * SUB84(dVar27,0),
                                    SUB84(dVar27,0));
        }
        else {
          dVar27 = dVar27 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),*(undefined4 *)DAT_8c615bcc);
        }
        else {
          dVar25 = *DAT_8c615bcc;
        }
        if (in_FPSCR_PR == '\0') {
          fVar21 = SUB84(dVar27,0) - (float)((ulonglong)dVar27 >> 0x20);
        }
        else {
          fVar21 = SUB84(dVar27 - dVar27,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(*(undefined4 *)((int)pdVar18 + -0xc),fVar21);
          dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),
                                    *(undefined4 *)((int)DAT_8c615bd0 + 4));
        }
        else {
          dVar27 = *(double *)((int)pdVar18 + -0xc);
          in_xd0 = *(double *)((int)DAT_8c615bd0 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),
                                    SUB84(dVar19,0) * (float)((ulonglong)dVar27 >> 0x20));
        }
        else {
          dVar19 = dVar19 * dVar27;
        }
        if (in_FPSCR_SZ == '\0') {
          in_dr2 = (double)CONCAT44(*(undefined4 *)((int)pdVar18 + -8),SUB84(dVar27,0));
        }
        else {
          in_dr2 = *(double *)((int)pdVar18 + -8);
        }
        if (in_FPSCR_PR == '\0') {
          in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),
                                    SUB84(in_dr2,0) - SUB84(dVar19,0));
        }
        else {
          in_dr2 = in_dr2 - dVar19;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),
                                    *(undefined4 *)(DAT_8c615bd0 + 1));
        }
        else {
          in_xd0 = DAT_8c615bd0[1];
        }
        fVar21 = (float)((ulonglong)in_dr2 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),SUB84(dVar19,0) * fVar21);
        }
        else {
          dVar19 = dVar19 * in_dr2;
        }
        if (in_FPSCR_SZ == '\0') {
          uVar26 = SUB84(in_dr2,0);
          in_dr2 = (double)CONCAT44(fVar21,*(undefined4 *)(DAT_8c615bd4 + 0x10));
          in_xd2 = dVar25;
          dVar25 = (double)CONCAT44(uVar26,uVar31);
        }
        else {
          in_xd2 = *(double *)(DAT_8c615bd4 + 0x10);
        }
        if (in_FPSCR_PR == '\0') {
          dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) - SUB84(dVar19,0),
                                    SUB84(dVar25,0));
        }
        else {
          dVar25 = dVar25 - dVar19;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)pdVar18 + -0x20) = SUB84(in_dr2,0);
          in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),
                                    *(undefined4 *)(DAT_8c615bd4 + 0x14));
        }
        else {
          *(double *)((int)pdVar18 + -0x20) = in_xd2;
          in_xd2 = *(double *)(DAT_8c615bd4 + 0x14);
        }
        fVar21 = (float)((ulonglong)dVar25 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),SUB84(in_dr2,0) * fVar21);
        }
        else {
          in_dr2 = in_dr2 * dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)pdVar18 + -0x1c) = SUB84(in_dr2,0);
          in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),
                                    *(undefined4 *)(DAT_8c615bd4 + 0x18));
        }
        else {
          *(double *)((int)pdVar18 + -0x1c) = in_xd2;
          in_xd2 = *(double *)(DAT_8c615bd4 + 0x18);
        }
        if (in_FPSCR_PR == '\0') {
          in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),SUB84(in_dr2,0) * fVar21);
        }
        else {
          in_dr2 = in_dr2 * dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)pdVar18 + -0x18) = SUB84(in_dr2,0);
          in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),
                                    *(undefined4 *)(DAT_8c615bd4 + 0x1c));
        }
        else {
          *(double *)((int)pdVar18 + -0x18) = in_xd2;
          in_xd2 = *(double *)(DAT_8c615bd4 + 0x1c);
        }
        if (in_FPSCR_PR == '\0') {
          in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),SUB84(in_dr2,0) * fVar21);
        }
        else {
          in_dr2 = in_dr2 * dVar25;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)pdVar18 + -0x14) = SUB84(in_dr2,0);
        }
        else {
          *(double *)((int)pdVar18 + -0x14) = in_xd2;
        }
        dVar19 = (double)(*(code *)PTR_FUN_8c615bd8)
                                   (pdVar11,pdVar12,pdVar13,(undefined1 *)((int)pdVar18 + -0x20));
        pdVar7 = unaff_r8;
      }
      uVar26 = DAT_8c615bbc;
      uVar31 = SUB84(param_3,0);
      param_2 = in_xd12;
      if (in_FPSCR_SZ == '\0') {
        param_2 = (double)CONCAT44(SUB84(dVar30,0),uVar24);
      }
      iVar6 = *(int *)((int)pdVar18 + -0x44);
      if (in_FPSCR_SZ == '\0') {
        dVar30 = (double)CONCAT44((int)((ulonglong)dVar30 >> 0x20),*(undefined4 *)(pdVar13 + 2));
        uVar32 = CONCAT44(*(undefined4 *)((int)pdVar13 + 0x14),SUB84(in_dr14,0));
        param_1 = (double)((ulonglong)in_dr14 >> 0x20);
      }
      else {
        in_xd12 = pdVar13[2];
        uVar32 = *(undefined8 *)((int)pdVar13 + 0x14);
        param_1 = dVar25;
        in_xd4 = in_dr14;
      }
      in_dr14 = (double)CONCAT44((int)((ulonglong)uVar32 >> 0x20),-(float)uVar32);
      *(int *)((int)pdVar18 + -0x44) = iVar6 + -1;
      pdVar11 = pdVar12;
      pdVar12 = pdVar13;
    } while (iVar6 + -1 != 0);
    if (pdVar7 < unaff_r8) {
      if (in_FPSCR_SZ == '\0') {
        dVar25 = (double)CONCAT44(*(undefined4 *)pdVar13,SUB84(param_1,0));
        dVar27 = (double)CONCAT44(*(undefined4 *)((int)pdVar13 + 4),SUB84(param_2,0));
        pdVar13 = pdVar13 + 1;
        in_dr2 = (double)CONCAT44(SUB84(in_dr2,0),SUB84(in_dr2,0));
        dVar19 = (double)CONCAT44(SUB84(dVar19,0),SUB84(dVar19,0));
      }
      else {
        dVar25 = *pdVar13;
        dVar27 = pdVar13[1];
        pdVar13 = pdVar13 + 2;
        in_dr2 = in_xd2;
        dVar19 = in_xd0;
      }
      in_dr2 = (double)((ulonglong)in_dr2 & 0xffffffff00000000);
                    /* WARNING: Ignoring partial resolution of indirect */
      fVar20 = SUB84(dVar27,0);
      fVar21 = SUB84(dVar25,0);
      if (in_FPSCR_PR == '\0') {
        fVar23 = SUB84(dVar19,0) * fVar21 +
                 (float)((ulonglong)dVar19 >> 0x20) * (float)((ulonglong)dVar25 >> 0x20) +
                 fVar20 * 0.0 +
                 (float)((ulonglong)in_dr2 >> 0x20) * (float)((ulonglong)dVar27 >> 0x20);
      }
      if (in_FPSCR_SZ == '\0') {
        param_3 = (double)CONCAT44(*(undefined4 *)pdVar13,uVar31);
        param_4 = (double)CONCAT44(*(undefined4 *)((int)pdVar13 + 4),SUB84(param_4,0));
        pdVar13 = pdVar13 + 1;
        param_1 = (double)CONCAT44(fVar21,fVar21);
        dVar19 = (double)CONCAT44(uVar31,SUB84(dVar19,0));
      }
      else {
        param_3 = *pdVar13;
        param_4 = pdVar13[1];
        pdVar13 = pdVar13 + 2;
        uVar31 = SUB84(param_3,0);
        param_1 = in_xd4;
        dVar19 = in_xd8;
      }
      if (in_FPSCR_PR == '\0') {
        dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) - SUB84(in_dr2,0),
                                  SUB84(dVar19,0));
      }
      else {
        dVar19 = dVar19 - in_dr2;
      }
      fVar21 = (float)((ulonglong)dVar19 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        pdVar7 = (double *)((int)unaff_r8 + 0xc);
        *(int *)pdVar7 = (int)((ulonglong)param_4 >> 0x20);
        pdVar13 = (double *)((int)pdVar13 + -4);
        *(float *)pdVar13 = fVar21;
        param_2 = (double)CONCAT44(fVar20,fVar20);
      }
      else {
        pdVar7 = unaff_r8 + 1;
        *pdVar7 = param_4;
        pdVar13 = pdVar13 + -1;
        *pdVar13 = dVar19;
        param_2 = in_xd6;
      }
      CacheBlockWriteBack(pdVar13);
      if (in_FPSCR_PR == '\0') {
        param_2 = (double)CONCAT44((int)((ulonglong)param_2 >> 0x20),SUB84(param_2,0) * fVar21);
      }
      else {
        param_2 = param_2 * dVar19;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar7 = (double *)((int)pdVar7 + -4);
        *(undefined4 *)pdVar7 = uVar31;
      }
      else {
        pdVar7 = pdVar7 + -1;
        *pdVar7 = in_xd8;
      }
      if (in_FPSCR_PR == '\0') {
        param_2 = (double)CONCAT44((float)((ulonglong)param_2 >> 0x20) * fVar21,SUB84(param_2,0));
      }
      else {
        param_2 = param_2 * dVar19;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar7 = (double *)((int)pdVar7 + -4);
        *(int *)pdVar7 = (int)((ulonglong)param_3 >> 0x20);
      }
      else {
        pdVar7 = pdVar7 + -1;
        *pdVar7 = param_3;
      }
      if (in_FPSCR_PR == '\0') {
        param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),SUB84(param_1,0) * fVar21);
      }
      else {
        param_1 = param_1 * dVar19;
      }
      *(undefined4 *)((int)pdVar7 + -4) = uVar26;
      if (in_FPSCR_SZ == '\0') {
        *(int *)(pdVar7 + 3) = SUB84(param_2,0);
        *(int *)((int)pdVar7 + 0x14) = (int)((ulonglong)param_2 >> 0x20);
        *(int *)(pdVar7 + 2) = SUB84(param_1,0);
        pdVar7 = (double *)((int)pdVar7 + 0xc);
        *(int *)pdVar7 = (int)((ulonglong)param_1 >> 0x20);
      }
      else {
        *(double *)((int)pdVar7 + 0x14) = in_xd6;
        *(double *)((int)pdVar7 + 0xc) = param_2;
        *(double *)((int)pdVar7 + 4) = in_xd4;
        pdVar7 = (double *)((int)pdVar7 + -4);
        *pdVar7 = param_1;
      }
      unaff_r8 = pdVar7 + 2;
    }
    iVar6 = *(int *)((int)pdVar18 + -0x38);
    *(int *)((int)pdVar18 + -0x38) = iVar6 + -1;
    if (iVar6 + -1 == 0) {
      *DAT_8c615bdc = *DAT_8c615bdc + *(int *)((int)pdVar18 + -0x48);
      *DAT_8c615be0 = *DAT_8c615be0 + *(int *)((int)pdVar18 + -0x48);
      return;
    }
  } while( true );
}

