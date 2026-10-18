/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c616dcc) */
/* WARNING: Removing unreachable block (ram,0x8c616eaa) */

void FUN_8c616bc4(double param_1,double param_2,double param_3,undefined4 param_4)

{
  ushort uVar1;
  int iVar2;
  undefined *puVar3;
  int *piVar4;
  int iVar5;
  int iVar6;
  int *piVar7;
  uint unaff_r8;
  double *pdVar8;
  uint uVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  int iVar13;
  ushort *puVar14;
  ushort *puVar15;
  ushort *puVar16;
  double *pdVar17;
  undefined4 uVar18;
  double dVar19;
  float fVar20;
  double in_dr2;
  ulonglong uVar21;
  float fVar23;
  uint uVar24;
  double dVar22;
  undefined4 uVar26;
  double dVar25;
  undefined4 uVar28;
  undefined4 uVar29;
  double dVar30;
  undefined4 uVar32;
  undefined8 in_dr12;
  double dVar33;
  double in_dr14;
  undefined8 uVar34;
  double in_xd0;
  double in_xd2;
  double dVar35;
  double dVar36;
  double in_xd4;
  double in_xd12;
  double in_xd14;
  bool bVar37;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  int aiStack_48 [4];
  undefined1 local_38 [16];
  double dStack_28;
  undefined8 uStack_20;
  undefined4 uVar27;
  undefined4 uVar31;
  
  if (in_FPSCR_SZ == '\0') {
    uStack_20 = (double)CONCAT44(SUB84(in_dr14,0),(undefined4)uStack_20);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_20._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
    pdVar17 = &dStack_28;
    dStack_28 = (double)CONCAT44((int)in_dr12,(int)((ulonglong)in_dr12 >> 0x20));
  }
  else {
    pdVar17 = (double *)local_38;
    uStack_20 = in_xd14;
    dStack_28 = in_dr14;
  }
  *(undefined4 *)((int)pdVar17 + -4) = in_PR;
  dVar33 = (double)CONCAT44(0x3f800000,(int)in_dr12);
  *(undefined4 *)((int)pdVar17 + -0x44) = param_4;
  dVar19 = (double)(*(code *)PTR_FUN_8c616c14)();
  puVar14 = *(ushort **)(*(int *)((int)pdVar17 + -0x44) + 4);
  *(uint *)((int)pdVar17 + -0x38) = (uint)*(ushort *)(*(int *)((int)pdVar17 + -0x44) + 2);
  iVar13 = *DAT_8c616c18;
  *(undefined4 *)((int)pdVar17 + -0x44) = 0;
  do {
    uVar18 = SUB84(in_dr14,0);
    uVar1 = *puVar14;
    puVar15 = puVar14 + 1;
    *(uint *)((int)pdVar17 + -0x48) = (uint)uVar1;
    uVar28 = (undefined4)((ulonglong)dVar33 >> 0x20);
    dVar25 = dVar33;
    if (in_FPSCR_SZ == '\0') {
      dVar25 = in_xd14;
      uVar18 = uVar28;
    }
    in_xd14 = dVar25;
    if ((uVar1 & DAT_8c616d38) != 0) {
      if (in_FPSCR_PR == '\0') {
        param_1 = (double)CONCAT44(0xbf800000,SUB84(param_1,0));
      }
      else {
        param_1 = -(double)CONCAT44(0x3f800000,SUB84(param_1,0));
      }
      *(uint *)((int)pdVar17 + -0x48) = *(uint *)((int)pdVar17 + -0x48) & (int)DAT_8c616d34;
      in_xd14 = param_1;
      if (in_FPSCR_SZ == '\0') {
        uVar18 = (undefined4)((ulonglong)param_1 >> 0x20);
        in_xd14 = dVar25;
      }
    }
    *(int *)((int)pdVar17 + -0x44) = *(int *)((int)pdVar17 + -0x44) + *(int *)((int)pdVar17 + -0x48)
    ;
    *(int *)((int)pdVar17 + -0x48) = *(int *)((int)pdVar17 + -0x48) + -2;
    puVar16 = puVar14 + 2;
    puVar14 = puVar14 + 3;
    iVar11 = (uint)*puVar16 * 0x20 + iVar13;
    iVar10 = (uint)*puVar15 * 0x20 + iVar13;
    uVar9 = unaff_r8;
    if (in_FPSCR_SZ == '\0') {
      dVar33 = (double)CONCAT44(uVar28,*(undefined4 *)(iVar11 + 0x10));
      param_2 = (double)CONCAT44(*(undefined4 *)(iVar10 + 0x10),SUB84(param_2,0));
      in_dr14 = (double)CONCAT44(*(undefined4 *)(iVar11 + 0x14),uVar18);
      param_1 = (double)(ulonglong)*(uint *)(iVar10 + 0x14);
    }
    else {
      in_xd12 = *(double *)(iVar11 + 0x10);
      param_2 = *(double *)(iVar10 + 0x10);
      in_dr14 = *(double *)(iVar11 + 0x14);
      in_xd4 = *(double *)(iVar10 + 0x14);
    }
    do {
      uVar1 = *puVar14;
      puVar14 = puVar14 + 1;
      iVar12 = (uint)uVar1 * 0x20 + iVar13;
      *(undefined4 *)((int)pdVar17 + -0x3c) = 0;
      uVar18 = (undefined4)((ulonglong)dVar33 >> 0x20);
      dVar25 = dVar33;
      if (in_FPSCR_SZ == '\0') {
        dVar25 = (double)CONCAT44(uVar18,SUB84(param_1,0));
      }
      if ((*(uint *)(iVar10 + 0xc) & *(uint *)(iVar11 + 0xc) & *(uint *)(iVar12 + 0xc) & 0x3f) == 0)
      {
        if (((*(uint *)(iVar10 + 0xc) | *(uint *)(iVar11 + 0xc) | *(uint *)(iVar12 + 0xc)) & 1) == 0
           ) {
          if (in_FPSCR_SZ == '\0') {
            dVar30 = (double)CONCAT44(*(undefined4 *)(iVar12 + 0x14),SUB84(dVar33,0));
          }
          else {
            dVar30 = *(double *)(iVar12 + 0x14);
            in_xd2 = in_xd12;
          }
          fVar23 = (float)((ulonglong)in_dr14 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            dVar30 = (double)CONCAT44((float)((ulonglong)dVar30 >> 0x20) - fVar23,
                                      SUB84(dVar30,0) - (float)((ulonglong)param_2 >> 0x20));
          }
          else {
            dVar30 = (dVar30 - param_2) - in_dr14;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),
                                      *(undefined4 *)(iVar12 + 0x10));
          }
          else {
            in_xd0 = *(double *)(iVar12 + 0x10);
          }
          if (in_FPSCR_PR == '\0') {
            dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),
                                      SUB84(dVar19,0) - SUB84(dVar33,0));
            fVar20 = SUB84(dVar30,0) * (float)((ulonglong)dVar30 >> 0x20);
          }
          else {
            dVar19 = dVar19 - dVar33;
            fVar20 = SUB84(dVar30 * dVar30,0);
          }
          dVar30 = in_dr14;
          if (in_FPSCR_SZ == '\0') {
            dVar30 = (double)CONCAT44(fVar23,fVar20);
          }
          if (in_FPSCR_PR == '\0') {
            fVar23 = ((float)((ulonglong)dVar30 >> 0x20) - SUB84(dVar25,0)) * SUB84(dVar19,0);
            in_dr2 = (double)CONCAT44(fVar23,SUB84(dVar30,0) - fVar23);
          }
          else {
            in_dr2 = (dVar30 - dVar25) * dVar19;
            in_dr2 = in_dr2 - in_dr2;
          }
          dVar30 = in_xd2;
          if (in_FPSCR_SZ == '\0') {
            dVar30 = (double)CONCAT44(SUB84(in_dr2,0),SUB84(dVar25,0));
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((float)((ulonglong)dVar30 >> 0x20) * SUB84(in_dr14,0),
                                      SUB84(dVar30,0));
          }
          else {
            dVar25 = dVar30 * in_dr14;
          }
        }
        else {
          piVar7 = (int *)((int)pdVar17 + -0x10);
          *piVar7 = iVar10;
          *(int *)((int)pdVar17 + -0xc) = iVar11;
          *(int *)((int)pdVar17 + -8) = iVar12;
          iVar6 = 0;
          while( true ) {
            iVar2 = DAT_8c616d3c;
            pdVar8 = (double *)((int)pdVar17 + -0x34);
            uVar28 = (undefined4)((ulonglong)in_dr2 >> 0x20);
            if (2 < iVar6) break;
            piVar4 = piVar7 + iVar6;
            *(int **)((int)pdVar17 + -0x4c) = piVar4;
            *(int *)((int)pdVar17 + -0x50) = iVar6 * 0xc + (int)pdVar8;
            iVar6 = iVar6 + 1;
            if ((*(uint *)(*piVar4 + 0xc) & 1) == 0) {
              dVar30 = dVar33;
              if (in_FPSCR_SZ == '\0') {
                dVar30 = (double)CONCAT44(uVar18,SUB84(dVar25,0));
              }
              iVar5 = **(int **)((int)pdVar17 + -0x4c);
              *(int *)((int)pdVar17 + -0x40) = iVar5;
              if (in_FPSCR_SZ == '\0') {
                in_dr2 = (double)CONCAT44(uVar28,*(undefined4 *)(iVar5 + 0x18));
              }
              else {
                in_xd2 = *(double *)(iVar5 + 0x18);
              }
              if (in_FPSCR_PR == '\0') {
                dVar25 = (double)CONCAT44((float)((ulonglong)dVar30 >> 0x20) / SUB84(in_dr2,0),
                                          SUB84(dVar30,0));
              }
              else {
                dVar25 = dVar30 / in_dr2;
              }
              if (in_FPSCR_SZ == '\0') {
                in_dr2 = (double)(ulonglong)*(uint *)(DAT_8c616d3c + 0xc);
              }
              else {
                in_xd2 = *(double *)(DAT_8c616d3c + 0xc);
              }
              if (in_FPSCR_SZ == '\0') {
                dVar30 = (double)CONCAT44(*(undefined4 *)(*(int *)((int)pdVar17 + -0x40) + 0x10),
                                          SUB84(in_dr2,0));
              }
              else {
                dVar30 = *(double *)(*(int *)((int)pdVar17 + -0x40) + 0x10);
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
                                          *(undefined4 *)DAT_8c616d40);
              }
              else {
                in_xd2 = *DAT_8c616d40;
              }
              fVar23 = (float)((ulonglong)dVar25 >> 0x20);
              if (in_FPSCR_PR == '\0') {
                dVar30 = (double)CONCAT44(((float)((ulonglong)dVar30 >> 0x20) * fVar23) /
                                          SUB84(dVar30,0),SUB84(dVar30,0));
              }
              else {
                dVar30 = (dVar30 * dVar25) / (dVar30 * dVar25);
              }
              if (in_FPSCR_SZ == '\0') {
                *(int *)*(double **)((int)pdVar17 + -0x50) = (int)((ulonglong)dVar30 >> 0x20);
              }
              else {
                **(double **)((int)pdVar17 + -0x50) = dVar30;
              }
              if (in_FPSCR_SZ == '\0') {
                dVar30 = (double)(ulonglong)*(uint *)(iVar2 + 0x10);
              }
              else {
                in_xd2 = *(double *)(iVar2 + 0x10);
              }
              if (in_FPSCR_SZ == '\0') {
                dVar30 = (double)CONCAT44(*(undefined4 *)(**(int **)((int)pdVar17 + -0x4c) + 0x14),
                                          SUB84(dVar30,0));
              }
              else {
                dVar30 = *(double *)(**(int **)((int)pdVar17 + -0x4c) + 0x14);
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
                                          *(undefined4 *)DAT_8c616d44);
              }
              else {
                in_xd2 = *DAT_8c616d44;
              }
              if (in_FPSCR_PR == '\0') {
                in_dr2 = (double)CONCAT44(((float)((ulonglong)dVar30 >> 0x20) * fVar23) /
                                          SUB84(dVar30,0),SUB84(dVar30,0));
              }
              else {
                in_dr2 = (dVar30 * dVar25) / (dVar30 * dVar25);
              }
              if (in_FPSCR_SZ == '\0') {
                *(int *)(*(int *)((int)pdVar17 + -0x50) + 4) = (int)((ulonglong)in_dr2 >> 0x20);
              }
              else {
                *(double *)(*(int *)((int)pdVar17 + -0x50) + 4) = in_dr2;
              }
              if (in_FPSCR_SZ == '\0') {
                *(float *)(*(int *)((int)pdVar17 + -0x50) + 8) = fVar23;
              }
              else {
                *(double *)(*(int *)((int)pdVar17 + -0x50) + 8) = dVar25;
              }
            }
            else {
              if (in_FPSCR_SZ == '\0') {
                uVar27 = *(undefined4 *)(**(int **)((int)pdVar17 + -0x4c) + 0x10);
                in_dr2 = (double)CONCAT44(uVar28,uVar27);
                *(undefined4 *)*(double **)((int)pdVar17 + -0x50) = uVar27;
              }
              else {
                in_xd2 = *(double *)(**(int **)((int)pdVar17 + -0x4c) + 0x10);
                **(double **)((int)pdVar17 + -0x50) = in_xd2;
              }
              if (in_FPSCR_SZ == '\0') {
                uVar28 = *(undefined4 *)(**(int **)((int)pdVar17 + -0x4c) + 0x14);
                in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),uVar28);
                *(undefined4 *)(*(int *)((int)pdVar17 + -0x50) + 4) = uVar28;
              }
              else {
                in_xd2 = *(double *)(**(int **)((int)pdVar17 + -0x4c) + 0x14);
                *(double *)(*(int *)((int)pdVar17 + -0x50) + 4) = in_xd2;
              }
              if (in_FPSCR_SZ == '\0') {
                uVar28 = *(undefined4 *)(**(int **)((int)pdVar17 + -0x4c) + 0x18);
                in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),uVar28);
                *(undefined4 *)(*(int *)((int)pdVar17 + -0x50) + 8) = uVar28;
              }
              else {
                in_xd2 = *(double *)(**(int **)((int)pdVar17 + -0x4c) + 0x18);
                *(double *)(*(int *)((int)pdVar17 + -0x50) + 8) = in_xd2;
              }
            }
          }
          if (in_FPSCR_SZ == '\0') {
            uVar27 = *(undefined4 *)((int)pdVar17 + -0x2c);
            dVar19 = (double)CONCAT44(uVar27,SUB84(dVar19,0));
            param_2 = (double)CONCAT44(uVar27,SUB84(param_2,0));
          }
          else {
            dVar19 = *(double *)((int)pdVar17 + -0x2c);
            uVar27 = (undefined4)((ulonglong)dVar19 >> 0x20);
            param_2 = dVar19;
          }
          *(undefined4 **)((int)pdVar17 + -0x50) = (undefined4 *)((int)pdVar17 + -0x28);
          if (in_FPSCR_SZ == '\0') {
            uVar26 = *(undefined4 *)((int)pdVar17 + -0x30);
            param_3 = (double)CONCAT44((int)((ulonglong)param_3 >> 0x20),
                                       *(undefined4 *)((int)pdVar17 + -0x20));
            in_dr2 = (double)CONCAT44(uVar28,uVar26);
            dVar25 = (double)CONCAT44(uVar26,*(undefined4 *)((int)pdVar17 + -0x24));
          }
          else {
            in_xd2 = *(double *)((int)pdVar17 + -0x30);
            in_xd4 = *(double *)((int)pdVar17 + -0x24);
            uVar26 = (undefined4)((ulonglong)in_xd2 >> 0x20);
            dVar25 = in_xd2;
          }
          fVar23 = SUB84(dVar25,0);
          if (in_FPSCR_PR == '\0') {
            dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) * fVar23,SUB84(dVar19,0));
            fVar20 = SUB84(in_dr2,0) * SUB84(param_3,0);
          }
          else {
            dVar19 = dVar19 * dVar25;
            fVar20 = SUB84(in_dr2 * param_3,0);
          }
          if (in_FPSCR_SZ == '\0') {
            uVar28 = *(undefined4 *)pdVar8;
            param_2 = (double)CONCAT44(uVar27,*(undefined4 *)((int)pdVar17 + -0x28));
            dVar30 = (double)CONCAT44(uVar28,fVar20);
            param_3 = (double)CONCAT44(uVar28,SUB84(param_3,0));
          }
          else {
            dVar30 = *pdVar8;
            uVar28 = (undefined4)((ulonglong)dVar30 >> 0x20);
            param_3 = dVar30;
          }
          if (in_FPSCR_PR == '\0') {
            dVar30 = (double)CONCAT44((float)((ulonglong)dVar30 >> 0x20) * SUB84(param_3,0),
                                      SUB84(dVar30,0));
          }
          else {
            dVar30 = dVar30 * param_3;
          }
          dVar22 = dVar25;
          if (in_FPSCR_SZ == '\0') {
            dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),uVar26);
            dVar22 = in_xd0;
          }
          if (in_FPSCR_PR == '\0') {
            dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) - SUB84(dVar30,0),
                                      SUB84(dVar19,0));
          }
          else {
            dVar19 = dVar19 - dVar30;
          }
          dVar35 = param_2;
          if (in_FPSCR_SZ == '\0') {
            dVar30 = (double)CONCAT44((int)((ulonglong)dVar30 >> 0x20),
                                      (int)((ulonglong)param_2 >> 0x20));
            dVar35 = in_xd2;
          }
          if (in_FPSCR_PR == '\0') {
            fVar20 = SUB84(dVar30,0) * SUB84(param_2,0);
            dVar30 = (double)CONCAT44((float)((ulonglong)dVar30 >> 0x20) - fVar20,fVar20);
          }
          else {
            dVar30 = dVar30 * param_2 - dVar30 * param_2;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar30 = (double)CONCAT44((int)((ulonglong)dVar30 >> 0x20),
                                      *(undefined4 *)((int)pdVar17 + -0x18));
          }
          else {
            dVar35 = *(double *)((int)pdVar17 + -0x18);
          }
          if (in_FPSCR_PR == '\0') {
            fVar20 = SUB84(dVar30,0) * (float)((ulonglong)dVar30 >> 0x20);
          }
          else {
            fVar20 = SUB84(dVar30 * dVar30,0);
          }
          if (in_FPSCR_SZ == '\0') {
            uVar21 = CONCAT44((int)*(ulonglong *)((int)pdVar17 + -0x1c),fVar20);
          }
          else {
            uVar21 = *(ulonglong *)((int)pdVar17 + -0x1c);
          }
          if (in_FPSCR_PR == '\0') {
            uVar21 = (ulonglong)
                     (uint)((float)((ulonglong)dVar19 >> 0x20) * (float)(uVar21 >> 0x20) +
                           (float)uVar21);
          }
          dVar30 = dVar35;
          dVar36 = param_3;
          if (in_FPSCR_SZ == '\0') {
            dVar30 = (double)CONCAT44((int)uVar21,uVar28);
            dVar36 = dVar35;
          }
          if (in_FPSCR_PR == '\0') {
            fVar20 = SUB84(dVar30,0) * fVar23;
            dVar30 = (double)CONCAT44((int)((ulonglong)dVar30 >> 0x20),fVar20);
            fVar20 = SUB84(dVar19,0) * SUB84(param_2,0) - fVar20;
          }
          else {
            dVar30 = dVar30 * dVar25;
            fVar20 = SUB84(dVar19 * param_2 - dVar30,0);
          }
          if (in_FPSCR_SZ == '\0') {
            dVar30 = (double)CONCAT44((int)((ulonglong)dVar30 >> 0x20),
                                      *(undefined4 *)((int)pdVar17 + -0x14));
            dVar19 = (double)CONCAT44(fVar20,fVar20);
          }
          else {
            dVar36 = *(double *)((int)pdVar17 + -0x14);
            dVar19 = dVar22;
          }
          if (in_FPSCR_PR == '\0') {
            dVar30 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) * SUB84(dVar30,0) +
                                      (float)((ulonglong)dVar30 >> 0x20),SUB84(dVar30,0));
          }
          in_xd2 = in_xd14;
          if (in_FPSCR_SZ == '\0') {
            dVar30 = (double)CONCAT44((int)((ulonglong)dVar30 >> 0x20),SUB84(in_dr14,0));
            in_xd2 = dVar36;
          }
          uVar28 = (undefined4)((ulonglong)dVar30 >> 0x20);
          fVar20 = -SUB84(dVar30,0);
          dVar25 = (double)CONCAT44(uVar28,fVar20);
          dVar30 = dVar25;
          if (in_FPSCR_SZ == '\0') {
            dVar30 = (double)CONCAT44(uVar28,fVar23);
          }
          if (in_FPSCR_PR == '\0') {
            dVar30 = (double)CONCAT44((float)((ulonglong)dVar30 >> 0x20) * fVar20,SUB84(dVar30,0));
          }
          else {
            dVar30 = dVar30 * dVar25;
          }
          in_dr2 = (double)(ulonglong)(uint)fVar20;
          if (in_FPSCR_PR == '\0') {
            bVar37 = 0.0 < (float)((ulonglong)dVar30 >> 0x20);
          }
          else {
            bVar37 = in_dr2 < dVar30;
          }
          dVar25 = dVar30;
          in_xd0 = dVar22;
          if ((!bVar37) &&
             (*(undefined4 *)((int)pdVar17 + -0x3c) = 1, dVar25 = dVar33, in_FPSCR_SZ == '\0')) {
            dVar25 = (double)CONCAT44(uVar18,SUB84(dVar30,0));
          }
        }
      }
      uVar27 = DAT_8c616fb0;
      uVar28 = DAT_8c616fac;
      uVar24 = (uint)((ulonglong)in_dr2 >> 0x20);
      uVar29 = SUB84(dVar25,0);
      fVar23 = (float)((ulonglong)dVar25 >> 0x20);
      uVar18 = SUB84(param_2,0);
      uVar26 = SUB84(dVar19,0);
      if (uVar9 < unaff_r8) {
        dVar22 = (double)((ulonglong)uVar24 << 0x20);
        if (in_FPSCR_PR == '\0') {
          bVar37 = fVar23 < 0.0;
        }
        else {
          bVar37 = dVar25 < dVar22;
        }
        if (bVar37) {
LAB_8c616e58:
          uVar28 = DAT_8c616fb0;
          if (in_FPSCR_SZ == '\0') {
            uVar27 = SUB84(dVar19,0);
            uVar26 = SUB84(dVar22,0);
            uVar29 = SUB84(dVar25,0);
            uVar31 = *(undefined4 *)(iVar11 + 0x10);
            dVar25 = (double)CONCAT44(uVar31,uVar29);
            uVar32 = *(undefined4 *)(iVar11 + 0x14);
            dVar30 = (double)CONCAT44(uVar32,uVar18);
            *(undefined4 *)(unaff_r8 + 0x1c) = uVar26;
            *(undefined4 *)(unaff_r8 + 0x18) = uVar27;
            *(undefined4 *)(unaff_r8 + 0x14) = uVar32;
            *(undefined4 *)(unaff_r8 + 0x10) = uVar29;
            pdVar8 = (double *)(unaff_r8 + 0xc);
            *(undefined4 *)pdVar8 = uVar31;
            in_dr2 = (double)CONCAT44(uVar26,uVar26);
            dVar19 = (double)CONCAT44(uVar27,uVar27);
          }
          else {
            dVar25 = *(double *)(iVar11 + 0x10);
            dVar30 = *(double *)(iVar11 + 0x18);
            *(double *)(unaff_r8 + 0x18) = in_xd2;
            pdVar8 = (double *)(unaff_r8 + 0x10);
            *pdVar8 = in_xd0;
            pdVar8[-1] = dVar30;
            pdVar8[-2] = in_xd4;
            pdVar8 = pdVar8 + -3;
            *pdVar8 = dVar25;
            in_dr2 = in_xd2;
            dVar19 = in_xd0;
          }
          uVar18 = SUB84(dVar30,0);
          *(undefined4 *)((int)pdVar8 + -4) = uVar28;
          unaff_r8 = (int)pdVar8 + 0x1c;
        }
        else {
          if (in_FPSCR_SZ == '\0') {
            uVar27 = *(undefined4 *)(iVar11 + 0x10);
            dVar25 = (double)CONCAT44(uVar27,uVar29);
            uVar31 = *(undefined4 *)(iVar11 + 0x14);
            dVar30 = (double)CONCAT44(uVar31,uVar18);
            *(undefined4 *)(unaff_r8 + 0x1c) = 0;
            *(undefined4 *)(unaff_r8 + 0x18) = uVar26;
            *(undefined4 *)(unaff_r8 + 0x14) = uVar31;
            *(undefined4 *)(unaff_r8 + 0x10) = uVar29;
            pdVar8 = (double *)(unaff_r8 + 0xc);
            *(undefined4 *)pdVar8 = uVar27;
            in_dr2 = 0.0;
            dVar19 = (double)CONCAT44(uVar26,uVar26);
          }
          else {
            dVar25 = *(double *)(iVar11 + 0x10);
            dVar30 = *(double *)(iVar11 + 0x18);
            *(double *)(unaff_r8 + 0x18) = in_xd2;
            pdVar8 = (double *)(unaff_r8 + 0x10);
            *pdVar8 = in_xd0;
            pdVar8[-1] = dVar30;
            pdVar8[-2] = in_xd4;
            pdVar8 = pdVar8 + -3;
            *pdVar8 = dVar25;
            in_dr2 = in_xd2;
            dVar19 = in_xd0;
          }
          uVar18 = SUB84(dVar30,0);
          *(undefined4 *)((int)pdVar8 + -4) = uVar28;
          unaff_r8 = (int)pdVar8 + 0x1c;
          uVar9 = unaff_r8;
        }
      }
      else {
        in_dr2 = (double)((ulonglong)uVar24 << 0x20);
        if (in_FPSCR_PR == '\0') {
          bVar37 = fVar23 < 0.0;
        }
        else {
          bVar37 = dVar25 < in_dr2;
        }
        if (bVar37) {
          if (in_FPSCR_SZ == '\0') {
            uVar28 = *(undefined4 *)(iVar10 + 0x10);
            dVar25 = (double)CONCAT44(uVar28,uVar29);
            uVar31 = *(undefined4 *)(iVar10 + 0x14);
            dVar30 = (double)CONCAT44(uVar31,uVar18);
            *(undefined4 *)(unaff_r8 + 0x1c) = 0;
            *(undefined4 *)(unaff_r8 + 0x18) = uVar26;
            *(undefined4 *)(unaff_r8 + 0x14) = uVar31;
            *(undefined4 *)(unaff_r8 + 0x10) = uVar29;
            pdVar8 = (double *)(unaff_r8 + 0xc);
            *(undefined4 *)pdVar8 = uVar28;
            dVar22 = 0.0;
            dVar19 = (double)CONCAT44(uVar26,uVar26);
          }
          else {
            dVar25 = *(double *)(iVar10 + 0x10);
            dVar30 = *(double *)(iVar10 + 0x18);
            *(double *)(unaff_r8 + 0x18) = in_xd2;
            pdVar8 = (double *)(unaff_r8 + 0x10);
            *pdVar8 = in_xd0;
            pdVar8[-1] = dVar30;
            pdVar8[-2] = in_xd4;
            pdVar8 = pdVar8 + -3;
            *pdVar8 = dVar25;
            dVar22 = in_xd2;
            dVar19 = in_xd0;
          }
          uVar18 = SUB84(dVar30,0);
          *(undefined4 *)((int)pdVar8 + -4) = uVar27;
          unaff_r8 = (int)pdVar8 + 0x1c;
          goto LAB_8c616e58;
        }
      }
      puVar3 = PTR_FUN_8c616fb8;
      if (*(int *)((int)pdVar17 + -0x3c) != 0) {
        *DAT_8c616fb4 = 0;
        dVar19 = (double)(*(code *)puVar3)(iVar10,iVar11,iVar12);
        uVar9 = unaff_r8;
      }
      uVar28 = DAT_8c616fac;
      param_2 = in_xd12;
      if (in_FPSCR_SZ == '\0') {
        param_2 = (double)CONCAT44(SUB84(dVar33,0),uVar18);
      }
      iVar6 = *(int *)((int)pdVar17 + -0x48);
      if (in_FPSCR_SZ == '\0') {
        dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),*(undefined4 *)(iVar12 + 0x10));
        uVar34 = CONCAT44(*(undefined4 *)(iVar12 + 0x14),SUB84(in_dr14,0));
        param_1 = (double)((ulonglong)in_dr14 >> 0x20);
      }
      else {
        in_xd12 = *(double *)(iVar12 + 0x10);
        uVar34 = *(undefined8 *)(iVar12 + 0x14);
        param_1 = dVar25;
        in_xd4 = in_dr14;
      }
      in_dr14 = (double)CONCAT44((int)((ulonglong)uVar34 >> 0x20),-(float)uVar34);
      *(int *)((int)pdVar17 + -0x48) = iVar6 + -1;
      iVar10 = iVar11;
      iVar11 = iVar12;
    } while (iVar6 + -1 != 0);
    if (uVar9 < unaff_r8) {
      if (in_FPSCR_SZ == '\0') {
        uVar18 = SUB84(dVar19,0);
        uVar27 = SUB84(in_dr2,0);
        uVar26 = SUB84(param_1,0);
        uVar29 = *(undefined4 *)(iVar12 + 0x10);
        param_1 = (double)CONCAT44(uVar29,uVar26);
        uVar31 = *(undefined4 *)(iVar12 + 0x14);
        param_2 = (double)CONCAT44(uVar31,SUB84(param_2,0));
        *(undefined4 *)(unaff_r8 + 0x1c) = uVar27;
        *(undefined4 *)(unaff_r8 + 0x18) = uVar18;
        *(undefined4 *)(unaff_r8 + 0x14) = uVar31;
        *(undefined4 *)(unaff_r8 + 0x10) = uVar26;
        pdVar8 = (double *)(unaff_r8 + 0xc);
        *(undefined4 *)pdVar8 = uVar29;
        in_dr2 = (double)CONCAT44(uVar27,uVar27);
        dVar19 = (double)CONCAT44(uVar18,uVar18);
      }
      else {
        param_1 = *(double *)(iVar12 + 0x10);
        param_2 = *(double *)(iVar12 + 0x18);
        *(double *)(unaff_r8 + 0x18) = in_xd2;
        pdVar8 = (double *)(unaff_r8 + 0x10);
        *pdVar8 = in_xd0;
        pdVar8[-1] = param_2;
        pdVar8[-2] = in_xd4;
        pdVar8 = pdVar8 + -3;
        *pdVar8 = param_1;
        in_dr2 = in_xd2;
        dVar19 = in_xd0;
      }
      *(undefined4 *)((int)pdVar8 + -4) = uVar28;
      unaff_r8 = (int)pdVar8 + 0x1c;
    }
    iVar10 = *(int *)((int)pdVar17 + -0x38);
    *(int *)((int)pdVar17 + -0x38) = iVar10 + -1;
    if (iVar10 + -1 == 0) {
      *DAT_8c616fbc = *DAT_8c616fbc + *(int *)((int)pdVar17 + -0x44);
      *DAT_8c616fc0 = *DAT_8c616fc0 + *(int *)((int)pdVar17 + -0x44);
      return;
    }
  } while( true );
}

