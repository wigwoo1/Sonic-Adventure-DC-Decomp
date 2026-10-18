/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c614dac) */
/* WARNING: Removing unreachable block (ram,0x8c614ee4) */
/* WARNING: Restarted to delay deadcode elimination for space: register */

void FUN_8c614bb0(double param_1,double param_2,double param_3,double param_4,undefined4 param_5)

{
  ushort uVar1;
  int iVar2;
  undefined *puVar3;
  int *piVar4;
  int iVar5;
  int iVar6;
  double *pdVar7;
  int *piVar8;
  double *unaff_r8;
  double *pdVar9;
  double *pdVar10;
  undefined8 *puVar11;
  undefined8 *puVar12;
  undefined8 *puVar13;
  int iVar14;
  ushort *puVar15;
  ushort *puVar16;
  ushort *puVar17;
  double *pdVar18;
  double dVar19;
  float fVar20;
  double in_dr2;
  double dVar21;
  float fVar22;
  uint uVar23;
  float fVar24;
  undefined4 uVar27;
  undefined8 uVar25;
  double dVar26;
  undefined4 uVar29;
  undefined8 uVar28;
  undefined4 uVar30;
  undefined4 uVar31;
  undefined8 in_dr12;
  double dVar32;
  double in_dr14;
  double in_xd0;
  double dVar33;
  double in_xd2;
  double dVar34;
  double dVar35;
  double in_xd4;
  double in_xd6;
  double in_xd8;
  double in_xd12;
  double in_xd14;
  bool bVar36;
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
  dVar32 = (double)CONCAT44(0x3f800000,(int)in_dr12);
  *(undefined4 *)((int)pdVar18 + -0x44) = param_5;
  dVar19 = (double)(*(code *)PTR_FUN_8c614bf4)();
  puVar15 = *(ushort **)(*(int *)((int)pdVar18 + -0x44) + 4);
  *(uint *)((int)pdVar18 + -0x38) = (uint)*(ushort *)(*(int *)((int)pdVar18 + -0x44) + 2);
  iVar14 = *DAT_8c614bf8;
  *(undefined4 *)((int)pdVar18 + -0x44) = 0;
  do {
    uVar30 = SUB84(in_dr14,0);
    uVar1 = *puVar15;
    puVar16 = puVar15 + 1;
    *(uint *)((int)pdVar18 + -0x48) = (uint)uVar1;
    uVar31 = (undefined4)((ulonglong)dVar32 >> 0x20);
    dVar26 = dVar32;
    if (in_FPSCR_SZ == '\0') {
      dVar26 = in_xd14;
      uVar30 = uVar31;
    }
    in_xd14 = dVar26;
    if ((uVar1 & DAT_8c614d18) != 0) {
      if (in_FPSCR_PR == '\0') {
        param_1 = (double)CONCAT44(0xbf800000,SUB84(param_1,0));
      }
      else {
        param_1 = -(double)CONCAT44(0x3f800000,SUB84(param_1,0));
      }
      *(uint *)((int)pdVar18 + -0x48) = *(uint *)((int)pdVar18 + -0x48) & (int)DAT_8c614d14;
      in_xd14 = param_1;
      if (in_FPSCR_SZ == '\0') {
        uVar30 = (undefined4)((ulonglong)param_1 >> 0x20);
        in_xd14 = dVar26;
      }
    }
    *(int *)((int)pdVar18 + -0x44) = *(int *)((int)pdVar18 + -0x44) + *(int *)((int)pdVar18 + -0x48)
    ;
    *(int *)((int)pdVar18 + -0x48) = *(int *)((int)pdVar18 + -0x48) + -2;
    puVar17 = puVar15 + 2;
    puVar15 = puVar15 + 3;
    puVar12 = (undefined8 *)((uint)*puVar17 * 0x20 + iVar14);
    puVar11 = (undefined8 *)((uint)*puVar16 * 0x20 + iVar14);
    pdVar7 = unaff_r8;
    if (in_FPSCR_SZ == '\0') {
      dVar32 = (double)CONCAT44(uVar31,*(undefined4 *)(puVar12 + 2));
      param_2 = (double)CONCAT44(*(undefined4 *)(puVar11 + 2),SUB84(param_2,0));
      in_dr14 = (double)CONCAT44(*(undefined4 *)((int)puVar12 + 0x14),uVar30);
      param_1 = (double)(ulonglong)*(uint *)((int)puVar11 + 0x14);
    }
    else {
      in_xd12 = (double)puVar12[2];
      param_2 = (double)puVar11[2];
      in_dr14 = *(double *)((int)puVar12 + 0x14);
      in_xd4 = *(double *)((int)puVar11 + 0x14);
    }
    do {
      uVar1 = *puVar15;
      puVar15 = puVar15 + 1;
      puVar13 = (undefined8 *)((uint)uVar1 * 0x20 + iVar14);
      *(undefined4 *)((int)pdVar18 + -0x3c) = 0;
      uVar30 = (undefined4)((ulonglong)dVar32 >> 0x20);
      dVar26 = dVar32;
      if (in_FPSCR_SZ == '\0') {
        dVar26 = (double)CONCAT44(uVar30,SUB84(param_1,0));
      }
      if ((*(uint *)((int)puVar11 + 0xc) & *(uint *)((int)puVar12 + 0xc) &
           *(uint *)((int)puVar13 + 0xc) & 0x3f) == 0) {
        if (((*(uint *)((int)puVar11 + 0xc) | *(uint *)((int)puVar12 + 0xc) |
             *(uint *)((int)puVar13 + 0xc)) & 1) == 0) {
          if (in_FPSCR_SZ == '\0') {
            dVar21 = (double)CONCAT44(*(undefined4 *)((int)puVar13 + 0x14),SUB84(dVar32,0));
          }
          else {
            dVar21 = *(double *)((int)puVar13 + 0x14);
            in_xd2 = in_xd12;
          }
          fVar22 = (float)((ulonglong)in_dr14 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            dVar21 = (double)CONCAT44((float)((ulonglong)dVar21 >> 0x20) - fVar22,
                                      SUB84(dVar21,0) - (float)((ulonglong)param_2 >> 0x20));
          }
          else {
            dVar21 = (dVar21 - param_2) - in_dr14;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),*(undefined4 *)(puVar13 + 2))
            ;
          }
          else {
            in_xd0 = (double)puVar13[2];
          }
          if (in_FPSCR_PR == '\0') {
            dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),
                                      SUB84(dVar19,0) - SUB84(dVar32,0));
            fVar20 = SUB84(dVar21,0) * (float)((ulonglong)dVar21 >> 0x20);
          }
          else {
            dVar19 = dVar19 - dVar32;
            fVar20 = SUB84(dVar21 * dVar21,0);
          }
          dVar21 = in_dr14;
          if (in_FPSCR_SZ == '\0') {
            dVar21 = (double)CONCAT44(fVar22,fVar20);
          }
          if (in_FPSCR_PR == '\0') {
            fVar22 = ((float)((ulonglong)dVar21 >> 0x20) - SUB84(dVar26,0)) * SUB84(dVar19,0);
            in_dr2 = (double)CONCAT44(fVar22,SUB84(dVar21,0) - fVar22);
          }
          else {
            in_dr2 = (dVar21 - dVar26) * dVar19;
            in_dr2 = in_dr2 - in_dr2;
          }
          dVar21 = in_xd2;
          if (in_FPSCR_SZ == '\0') {
            dVar21 = (double)CONCAT44(SUB84(in_dr2,0),SUB84(dVar26,0));
          }
          if (in_FPSCR_PR == '\0') {
            dVar26 = (double)CONCAT44((float)((ulonglong)dVar21 >> 0x20) * SUB84(in_dr14,0),
                                      SUB84(dVar21,0));
          }
          else {
            dVar26 = dVar21 * in_dr14;
          }
        }
        else {
          piVar8 = (int *)((int)pdVar18 + -0x10);
          *piVar8 = (int)puVar11;
          *(undefined8 **)((int)pdVar18 + -0xc) = puVar12;
          *(undefined8 **)((int)pdVar18 + -8) = puVar13;
          iVar6 = 0;
          while( true ) {
            iVar2 = DAT_8c614d1c;
            pdVar9 = (double *)((int)pdVar18 + -0x34);
            uVar31 = (undefined4)((ulonglong)in_dr2 >> 0x20);
            if (2 < iVar6) break;
            piVar4 = piVar8 + iVar6;
            *(int **)((int)pdVar18 + -0x4c) = piVar4;
            *(int *)((int)pdVar18 + -0x50) = iVar6 * 0xc + (int)pdVar9;
            iVar6 = iVar6 + 1;
            if ((*(uint *)(*piVar4 + 0xc) & 1) == 0) {
              dVar21 = dVar32;
              if (in_FPSCR_SZ == '\0') {
                dVar21 = (double)CONCAT44(uVar30,SUB84(dVar26,0));
              }
              iVar5 = **(int **)((int)pdVar18 + -0x4c);
              *(int *)((int)pdVar18 + -0x40) = iVar5;
              if (in_FPSCR_SZ == '\0') {
                in_dr2 = (double)CONCAT44(uVar31,*(undefined4 *)(iVar5 + 0x18));
              }
              else {
                in_xd2 = *(double *)(iVar5 + 0x18);
              }
              if (in_FPSCR_PR == '\0') {
                dVar26 = (double)CONCAT44((float)((ulonglong)dVar21 >> 0x20) / SUB84(in_dr2,0),
                                          SUB84(dVar21,0));
              }
              else {
                dVar26 = dVar21 / in_dr2;
              }
              if (in_FPSCR_SZ == '\0') {
                in_dr2 = (double)(ulonglong)*(uint *)(DAT_8c614d1c + 0xc);
              }
              else {
                in_xd2 = *(double *)(DAT_8c614d1c + 0xc);
              }
              if (in_FPSCR_SZ == '\0') {
                dVar21 = (double)CONCAT44(*(undefined4 *)(*(int *)((int)pdVar18 + -0x40) + 0x10),
                                          SUB84(in_dr2,0));
              }
              else {
                dVar21 = *(double *)(*(int *)((int)pdVar18 + -0x40) + 0x10);
              }
              if (in_FPSCR_PR == '\0') {
                dVar21 = (double)CONCAT44((float)((ulonglong)dVar21 >> 0x20) - SUB84(dVar21,0),
                                          SUB84(dVar21,0));
              }
              else {
                dVar21 = dVar21 - dVar21;
              }
              if (in_FPSCR_SZ == '\0') {
                dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),
                                          *(undefined4 *)DAT_8c614d20);
              }
              else {
                in_xd2 = *DAT_8c614d20;
              }
              fVar22 = (float)((ulonglong)dVar26 >> 0x20);
              if (in_FPSCR_PR == '\0') {
                dVar21 = (double)CONCAT44(((float)((ulonglong)dVar21 >> 0x20) * fVar22) /
                                          SUB84(dVar21,0),SUB84(dVar21,0));
              }
              else {
                dVar21 = (dVar21 * dVar26) / (dVar21 * dVar26);
              }
              uVar31 = (undefined4)((ulonglong)dVar21 >> 0x20);
              if (in_FPSCR_SZ == '\0') {
                *(undefined4 *)*(double **)((int)pdVar18 + -0x50) = uVar31;
              }
              else {
                **(double **)((int)pdVar18 + -0x50) = dVar21;
              }
              if (in_FPSCR_SZ == '\0') {
                dVar21 = (double)CONCAT44(uVar31,*(undefined4 *)(iVar2 + 0x10));
              }
              else {
                in_xd2 = *(double *)(iVar2 + 0x10);
              }
              if (in_FPSCR_SZ == '\0') {
                dVar21 = (double)CONCAT44(*(undefined4 *)(**(int **)((int)pdVar18 + -0x4c) + 0x14),
                                          SUB84(dVar21,0));
              }
              else {
                dVar21 = *(double *)(**(int **)((int)pdVar18 + -0x4c) + 0x14);
              }
              if (in_FPSCR_PR == '\0') {
                dVar21 = (double)CONCAT44((float)((ulonglong)dVar21 >> 0x20) - SUB84(dVar21,0),
                                          SUB84(dVar21,0));
              }
              else {
                dVar21 = dVar21 - dVar21;
              }
              if (in_FPSCR_SZ == '\0') {
                dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),
                                          *(undefined4 *)DAT_8c614d24);
              }
              else {
                in_xd2 = *DAT_8c614d24;
              }
              if (in_FPSCR_PR == '\0') {
                in_dr2 = (double)CONCAT44(((float)((ulonglong)dVar21 >> 0x20) * fVar22) /
                                          SUB84(dVar21,0),SUB84(dVar21,0));
              }
              else {
                in_dr2 = (dVar21 * dVar26) / (dVar21 * dVar26);
              }
              if (in_FPSCR_SZ == '\0') {
                *(int *)(*(int *)((int)pdVar18 + -0x50) + 4) = (int)((ulonglong)in_dr2 >> 0x20);
              }
              else {
                *(double *)(*(int *)((int)pdVar18 + -0x50) + 4) = in_dr2;
              }
              if (in_FPSCR_SZ == '\0') {
                *(float *)(*(int *)((int)pdVar18 + -0x50) + 8) = fVar22;
              }
              else {
                *(double *)(*(int *)((int)pdVar18 + -0x50) + 8) = dVar26;
              }
            }
            else {
              if (in_FPSCR_SZ == '\0') {
                uVar29 = *(undefined4 *)(**(int **)((int)pdVar18 + -0x4c) + 0x10);
                in_dr2 = (double)CONCAT44(uVar31,uVar29);
                *(undefined4 *)*(double **)((int)pdVar18 + -0x50) = uVar29;
              }
              else {
                in_xd2 = *(double *)(**(int **)((int)pdVar18 + -0x4c) + 0x10);
                **(double **)((int)pdVar18 + -0x50) = in_xd2;
              }
              if (in_FPSCR_SZ == '\0') {
                uVar31 = *(undefined4 *)(**(int **)((int)pdVar18 + -0x4c) + 0x14);
                in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),uVar31);
                *(undefined4 *)(*(int *)((int)pdVar18 + -0x50) + 4) = uVar31;
              }
              else {
                in_xd2 = *(double *)(**(int **)((int)pdVar18 + -0x4c) + 0x14);
                *(double *)(*(int *)((int)pdVar18 + -0x50) + 4) = in_xd2;
              }
              if (in_FPSCR_SZ == '\0') {
                uVar31 = *(undefined4 *)(**(int **)((int)pdVar18 + -0x4c) + 0x18);
                in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),uVar31);
                *(undefined4 *)(*(int *)((int)pdVar18 + -0x50) + 8) = uVar31;
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
            uVar27 = *(undefined4 *)((int)pdVar18 + -0x30);
            param_3 = (double)CONCAT44((int)((ulonglong)param_3 >> 0x20),
                                       *(undefined4 *)((int)pdVar18 + -0x20));
            in_dr2 = (double)CONCAT44(uVar31,uVar27);
            dVar26 = (double)CONCAT44(uVar27,*(undefined4 *)((int)pdVar18 + -0x24));
          }
          else {
            in_xd2 = *(double *)((int)pdVar18 + -0x30);
            in_xd4 = *(double *)((int)pdVar18 + -0x24);
            in_xd8 = *(double *)((int)pdVar18 + -0x20);
            uVar27 = (undefined4)((ulonglong)in_xd2 >> 0x20);
            dVar26 = in_xd2;
          }
          fVar22 = SUB84(dVar26,0);
          if (in_FPSCR_PR == '\0') {
            dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) * fVar22,SUB84(dVar19,0));
            fVar20 = SUB84(in_dr2,0) * SUB84(param_3,0);
          }
          else {
            dVar19 = dVar19 * dVar26;
            fVar20 = SUB84(in_dr2 * param_3,0);
          }
          if (in_FPSCR_SZ == '\0') {
            uVar31 = *(undefined4 *)pdVar9;
            param_2 = (double)CONCAT44(uVar29,*(undefined4 *)pdVar10);
            dVar21 = (double)CONCAT44(uVar31,fVar20);
            param_3 = (double)CONCAT44(uVar31,SUB84(param_3,0));
          }
          else {
            dVar21 = *pdVar9;
            in_xd6 = *pdVar10;
            uVar31 = (undefined4)((ulonglong)dVar21 >> 0x20);
            param_3 = dVar21;
          }
          if (in_FPSCR_PR == '\0') {
            dVar21 = (double)CONCAT44((float)((ulonglong)dVar21 >> 0x20) * SUB84(param_3,0),
                                      SUB84(dVar21,0));
          }
          else {
            dVar21 = dVar21 * param_3;
          }
          dVar33 = dVar26;
          if (in_FPSCR_SZ == '\0') {
            dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),uVar27);
            dVar33 = in_xd0;
          }
          if (in_FPSCR_PR == '\0') {
            dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) - SUB84(dVar21,0),
                                      SUB84(dVar19,0));
          }
          else {
            dVar19 = dVar19 - dVar21;
          }
          dVar34 = param_2;
          if (in_FPSCR_SZ == '\0') {
            dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),
                                      (int)((ulonglong)param_2 >> 0x20));
            dVar34 = in_xd2;
          }
          if (in_FPSCR_PR == '\0') {
            fVar20 = SUB84(dVar21,0) * SUB84(param_2,0);
            dVar21 = (double)CONCAT44((float)((ulonglong)dVar21 >> 0x20) - fVar20,fVar20);
          }
          else {
            dVar21 = dVar21 * param_2 - dVar21 * param_2;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),
                                      *(undefined4 *)((int)pdVar18 + -0x18));
          }
          else {
            dVar34 = *(double *)((int)pdVar18 + -0x18);
          }
          if (in_FPSCR_PR == '\0') {
            fVar20 = SUB84(dVar21,0) * (float)((ulonglong)dVar21 >> 0x20);
          }
          else {
            fVar20 = SUB84(dVar21 * dVar21,0);
          }
          if (in_FPSCR_SZ == '\0') {
            uVar25 = CONCAT44(*(undefined4 *)((int)pdVar18 + -0x1c),fVar20);
          }
          else {
            uVar25 = *(undefined8 *)((int)pdVar18 + -0x1c);
          }
          if (in_FPSCR_PR == '\0') {
            fVar20 = (float)((ulonglong)uVar25 >> 0x20);
            uVar25 = CONCAT44(fVar20,(float)((ulonglong)dVar19 >> 0x20) * fVar20 + (float)uVar25);
          }
          dVar21 = dVar34;
          dVar35 = param_3;
          if (in_FPSCR_SZ == '\0') {
            dVar21 = (double)CONCAT44((int)uVar25,uVar31);
            dVar35 = dVar34;
          }
          if (in_FPSCR_PR == '\0') {
            fVar20 = SUB84(dVar21,0) * fVar22;
            dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),fVar20);
            fVar20 = SUB84(dVar19,0) * SUB84(param_2,0) - fVar20;
          }
          else {
            dVar21 = dVar21 * dVar26;
            fVar20 = SUB84(dVar19 * param_2 - dVar21,0);
          }
          if (in_FPSCR_SZ == '\0') {
            dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),
                                      *(undefined4 *)((int)pdVar18 + -0x14));
            dVar19 = (double)CONCAT44(fVar20,fVar20);
          }
          else {
            dVar35 = *(double *)((int)pdVar18 + -0x14);
            dVar19 = dVar33;
          }
          if (in_FPSCR_PR == '\0') {
            dVar21 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) * SUB84(dVar21,0) +
                                      (float)((ulonglong)dVar21 >> 0x20),SUB84(dVar21,0));
          }
          in_xd2 = in_xd14;
          if (in_FPSCR_SZ == '\0') {
            dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),SUB84(in_dr14,0));
            in_xd2 = dVar35;
          }
          uVar31 = (undefined4)((ulonglong)dVar21 >> 0x20);
          fVar20 = -SUB84(dVar21,0);
          dVar26 = (double)CONCAT44(uVar31,fVar20);
          dVar21 = dVar26;
          if (in_FPSCR_SZ == '\0') {
            dVar21 = (double)CONCAT44(uVar31,fVar22);
          }
          if (in_FPSCR_PR == '\0') {
            dVar21 = (double)CONCAT44((float)((ulonglong)dVar21 >> 0x20) * fVar20,SUB84(dVar21,0));
          }
          else {
            dVar21 = dVar21 * dVar26;
          }
          in_dr2 = (double)(ulonglong)(uint)fVar20;
          if (in_FPSCR_PR == '\0') {
            bVar36 = 0.0 < (float)((ulonglong)dVar21 >> 0x20);
          }
          else {
            bVar36 = in_dr2 < dVar21;
          }
          dVar26 = dVar21;
          in_xd0 = dVar33;
          if ((!bVar36) &&
             (*(undefined4 *)((int)pdVar18 + -0x3c) = 1, dVar26 = dVar32, in_FPSCR_SZ == '\0')) {
            dVar26 = (double)CONCAT44(uVar30,SUB84(dVar21,0));
          }
        }
      }
      uVar29 = DAT_8c614fac;
      uVar31 = DAT_8c614fa8;
      uVar23 = (uint)((ulonglong)in_dr2 >> 0x20);
      fVar22 = (float)((ulonglong)dVar26 >> 0x20);
      uVar27 = SUB84(dVar19,0);
      uVar30 = SUB84(param_3,0);
      if (pdVar7 < unaff_r8) {
        dVar21 = (double)((ulonglong)uVar23 << 0x20);
        if (in_FPSCR_PR == '\0') {
          bVar36 = fVar22 < 0.0;
        }
        else {
          bVar36 = dVar26 < dVar21;
        }
        if (bVar36) {
LAB_8c614e74:
          uVar31 = DAT_8c614fac;
          uVar30 = SUB84(param_3,0);
          if (in_FPSCR_SZ == '\0') {
            uVar25 = CONCAT44(*(undefined4 *)puVar12,SUB84(dVar26,0));
            uVar28 = CONCAT44(*(undefined4 *)((int)puVar12 + 4),SUB84(param_2,0));
            pdVar9 = (double *)(puVar12 + 1);
            in_dr2 = (double)CONCAT44(SUB84(dVar21,0),SUB84(dVar21,0));
            dVar19 = (double)CONCAT44(SUB84(dVar19,0),SUB84(dVar19,0));
          }
          else {
            uVar25 = *puVar12;
            uVar28 = puVar12[1];
            pdVar9 = (double *)(puVar12 + 2);
            in_dr2 = in_xd2;
            dVar19 = in_xd0;
          }
          in_dr2 = (double)((ulonglong)in_dr2 & 0xffffffff00000000);
                    /* WARNING: Ignoring partial resolution of indirect */
          fVar20 = (float)uVar28;
          fVar22 = (float)uVar25;
          if (in_FPSCR_PR == '\0') {
            fVar24 = SUB84(dVar19,0) * fVar22 +
                     (float)((ulonglong)dVar19 >> 0x20) * (float)((ulonglong)uVar25 >> 0x20) +
                     fVar20 * 0.0 +
                     (float)((ulonglong)in_dr2 >> 0x20) * (float)((ulonglong)uVar28 >> 0x20);
          }
          if (in_FPSCR_SZ == '\0') {
            param_3 = (double)CONCAT44(*(undefined4 *)pdVar9,uVar30);
            param_4 = (double)CONCAT44(*(undefined4 *)((int)pdVar9 + 4),SUB84(param_4,0));
            pdVar9 = pdVar9 + 1;
            dVar26 = (double)CONCAT44(fVar22,fVar22);
            dVar19 = (double)CONCAT44(uVar30,SUB84(dVar19,0));
          }
          else {
            param_3 = *pdVar9;
            param_4 = pdVar9[1];
            pdVar9 = pdVar9 + 2;
            uVar30 = SUB84(param_3,0);
            dVar26 = in_xd4;
            dVar19 = in_xd8;
          }
          if (in_FPSCR_PR == '\0') {
            dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) - SUB84(in_dr2,0),
                                      SUB84(dVar19,0));
          }
          else {
            dVar19 = dVar19 - in_dr2;
          }
          fVar22 = (float)((ulonglong)dVar19 >> 0x20);
          if (in_FPSCR_SZ == '\0') {
            pdVar10 = (double *)((int)unaff_r8 + 0xc);
            *(int *)pdVar10 = (int)((ulonglong)param_4 >> 0x20);
            pdVar9 = (double *)((int)pdVar9 + -4);
            *(float *)pdVar9 = fVar22;
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
            param_2 = (double)CONCAT44((int)((ulonglong)param_2 >> 0x20),SUB84(param_2,0) * fVar22);
          }
          else {
            param_2 = param_2 * dVar19;
          }
          if (in_FPSCR_SZ == '\0') {
            pdVar10 = (double *)((int)pdVar10 + -4);
            *(undefined4 *)pdVar10 = uVar30;
          }
          else {
            pdVar10 = pdVar10 + -1;
            *pdVar10 = in_xd8;
          }
          if (in_FPSCR_PR == '\0') {
            param_2 = (double)CONCAT44((float)((ulonglong)param_2 >> 0x20) * fVar22,SUB84(param_2,0)
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
            dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),SUB84(dVar26,0) * fVar22);
          }
          else {
            dVar26 = dVar26 * dVar19;
          }
          *(undefined4 *)((int)pdVar10 + -4) = uVar31;
          if (in_FPSCR_SZ == '\0') {
            *(int *)(pdVar10 + 3) = SUB84(param_2,0);
            *(int *)((int)pdVar10 + 0x14) = (int)((ulonglong)param_2 >> 0x20);
            *(int *)(pdVar10 + 2) = SUB84(dVar26,0);
            pdVar10 = (double *)((int)pdVar10 + 0xc);
            *(int *)pdVar10 = (int)((ulonglong)dVar26 >> 0x20);
          }
          else {
            *(double *)((int)pdVar10 + 0x14) = in_xd6;
            *(double *)((int)pdVar10 + 0xc) = param_2;
            *(double *)((int)pdVar10 + 4) = in_xd4;
            pdVar10 = (double *)((int)pdVar10 + -4);
            *pdVar10 = dVar26;
          }
          unaff_r8 = pdVar10 + 2;
        }
        else {
          if (in_FPSCR_SZ == '\0') {
            uVar25 = CONCAT44(*(undefined4 *)puVar12,SUB84(dVar26,0));
            uVar28 = CONCAT44(*(undefined4 *)((int)puVar12 + 4),SUB84(param_2,0));
            pdVar7 = (double *)(puVar12 + 1);
            in_dr2 = 0.0;
            dVar19 = (double)CONCAT44(uVar27,uVar27);
          }
          else {
            uVar25 = *puVar12;
            uVar28 = puVar12[1];
            pdVar7 = (double *)(puVar12 + 2);
            in_dr2 = in_xd2;
            dVar19 = in_xd0;
          }
          in_dr2 = (double)((ulonglong)in_dr2 & 0xffffffff00000000);
                    /* WARNING: Ignoring partial resolution of indirect */
          fVar20 = (float)uVar28;
          fVar22 = (float)uVar25;
          if (in_FPSCR_PR == '\0') {
            fVar24 = SUB84(dVar19,0) * fVar22 +
                     (float)((ulonglong)dVar19 >> 0x20) * (float)((ulonglong)uVar25 >> 0x20) +
                     fVar20 * 0.0 +
                     (float)((ulonglong)in_dr2 >> 0x20) * (float)((ulonglong)uVar28 >> 0x20);
          }
          if (in_FPSCR_SZ == '\0') {
            param_3 = (double)CONCAT44(*(undefined4 *)pdVar7,uVar30);
            param_4 = (double)CONCAT44(*(undefined4 *)((int)pdVar7 + 4),SUB84(param_4,0));
            pdVar7 = pdVar7 + 1;
            dVar26 = (double)CONCAT44(fVar22,fVar22);
            dVar19 = (double)CONCAT44(uVar30,SUB84(dVar19,0));
          }
          else {
            param_3 = *pdVar7;
            param_4 = pdVar7[1];
            pdVar7 = pdVar7 + 2;
            uVar30 = SUB84(param_3,0);
            dVar26 = in_xd4;
            dVar19 = in_xd8;
          }
          if (in_FPSCR_PR == '\0') {
            dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) - SUB84(in_dr2,0),
                                      SUB84(dVar19,0));
          }
          else {
            dVar19 = dVar19 - in_dr2;
          }
          fVar22 = (float)((ulonglong)dVar19 >> 0x20);
          if (in_FPSCR_SZ == '\0') {
            pdVar9 = (double *)((int)unaff_r8 + 0xc);
            *(int *)pdVar9 = (int)((ulonglong)param_4 >> 0x20);
            pdVar7 = (double *)((int)pdVar7 + -4);
            *(float *)pdVar7 = fVar22;
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
            param_2 = (double)CONCAT44((int)((ulonglong)param_2 >> 0x20),SUB84(param_2,0) * fVar22);
          }
          else {
            param_2 = param_2 * dVar19;
          }
          if (in_FPSCR_SZ == '\0') {
            pdVar9 = (double *)((int)pdVar9 + -4);
            *(undefined4 *)pdVar9 = uVar30;
          }
          else {
            pdVar9 = pdVar9 + -1;
            *pdVar9 = in_xd8;
          }
          if (in_FPSCR_PR == '\0') {
            param_2 = (double)CONCAT44((float)((ulonglong)param_2 >> 0x20) * fVar22,SUB84(param_2,0)
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
            dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),SUB84(dVar26,0) * fVar22);
          }
          else {
            dVar26 = dVar26 * dVar19;
          }
          *(undefined4 *)((int)pdVar9 + -4) = uVar31;
          if (in_FPSCR_SZ == '\0') {
            *(int *)(pdVar9 + 3) = SUB84(param_2,0);
            *(int *)((int)pdVar9 + 0x14) = (int)((ulonglong)param_2 >> 0x20);
            *(int *)(pdVar9 + 2) = SUB84(dVar26,0);
            pdVar9 = (double *)((int)pdVar9 + 0xc);
            *(int *)pdVar9 = (int)((ulonglong)dVar26 >> 0x20);
          }
          else {
            *(double *)((int)pdVar9 + 0x14) = in_xd6;
            *(double *)((int)pdVar9 + 0xc) = param_2;
            *(double *)((int)pdVar9 + 4) = in_xd4;
            pdVar9 = (double *)((int)pdVar9 + -4);
            *pdVar9 = dVar26;
          }
          unaff_r8 = pdVar9 + 2;
          pdVar7 = unaff_r8;
        }
      }
      else {
        in_dr2 = (double)((ulonglong)uVar23 << 0x20);
        if (in_FPSCR_PR == '\0') {
          bVar36 = fVar22 < 0.0;
        }
        else {
          bVar36 = dVar26 < in_dr2;
        }
        if (bVar36) {
          if (in_FPSCR_SZ == '\0') {
            uVar25 = CONCAT44(*(undefined4 *)puVar11,SUB84(dVar26,0));
            uVar28 = CONCAT44(*(undefined4 *)((int)puVar11 + 4),SUB84(param_2,0));
            pdVar9 = (double *)(puVar11 + 1);
            dVar21 = 0.0;
            dVar19 = (double)CONCAT44(uVar27,uVar27);
          }
          else {
            uVar25 = *puVar11;
            uVar28 = puVar11[1];
            pdVar9 = (double *)(puVar11 + 2);
            dVar21 = in_xd2;
            dVar19 = in_xd0;
          }
          dVar21 = (double)((ulonglong)dVar21 & 0xffffffff00000000);
                    /* WARNING: Ignoring partial resolution of indirect */
          fVar20 = (float)uVar28;
          fVar22 = (float)uVar25;
          if (in_FPSCR_PR == '\0') {
            fVar24 = SUB84(dVar19,0) * fVar22 +
                     (float)((ulonglong)dVar19 >> 0x20) * (float)((ulonglong)uVar25 >> 0x20) +
                     fVar20 * 0.0 +
                     (float)((ulonglong)dVar21 >> 0x20) * (float)((ulonglong)uVar28 >> 0x20);
          }
          if (in_FPSCR_SZ == '\0') {
            param_3 = (double)CONCAT44(*(undefined4 *)pdVar9,uVar30);
            param_4 = (double)CONCAT44(*(undefined4 *)((int)pdVar9 + 4),SUB84(param_4,0));
            pdVar9 = pdVar9 + 1;
            dVar26 = (double)CONCAT44(fVar22,fVar22);
            dVar19 = (double)CONCAT44(uVar30,SUB84(dVar19,0));
          }
          else {
            param_3 = *pdVar9;
            param_4 = pdVar9[1];
            pdVar9 = pdVar9 + 2;
            uVar30 = SUB84(param_3,0);
            dVar26 = in_xd4;
            dVar19 = in_xd8;
          }
          if (in_FPSCR_PR == '\0') {
            dVar19 = (double)CONCAT44((float)((ulonglong)dVar19 >> 0x20) - SUB84(dVar21,0),
                                      SUB84(dVar19,0));
          }
          else {
            dVar19 = dVar19 - dVar21;
          }
          fVar22 = (float)((ulonglong)dVar19 >> 0x20);
          if (in_FPSCR_SZ == '\0') {
            pdVar10 = (double *)((int)unaff_r8 + 0xc);
            *(int *)pdVar10 = (int)((ulonglong)param_4 >> 0x20);
            pdVar9 = (double *)((int)pdVar9 + -4);
            *(float *)pdVar9 = fVar22;
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
            param_2 = (double)CONCAT44((int)((ulonglong)param_2 >> 0x20),SUB84(param_2,0) * fVar22);
          }
          else {
            param_2 = param_2 * dVar19;
          }
          if (in_FPSCR_SZ == '\0') {
            pdVar10 = (double *)((int)pdVar10 + -4);
            *(undefined4 *)pdVar10 = uVar30;
          }
          else {
            pdVar10 = pdVar10 + -1;
            *pdVar10 = in_xd8;
          }
          if (in_FPSCR_PR == '\0') {
            param_2 = (double)CONCAT44((float)((ulonglong)param_2 >> 0x20) * fVar22,SUB84(param_2,0)
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
            dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),SUB84(dVar26,0) * fVar22);
          }
          else {
            dVar26 = dVar26 * dVar19;
          }
          *(undefined4 *)((int)pdVar10 + -4) = uVar29;
          if (in_FPSCR_SZ == '\0') {
            *(int *)(pdVar10 + 3) = SUB84(param_2,0);
            *(int *)((int)pdVar10 + 0x14) = (int)((ulonglong)param_2 >> 0x20);
            *(int *)(pdVar10 + 2) = SUB84(dVar26,0);
            pdVar10 = (double *)((int)pdVar10 + 0xc);
            *(int *)pdVar10 = (int)((ulonglong)dVar26 >> 0x20);
          }
          else {
            *(double *)((int)pdVar10 + 0x14) = in_xd6;
            *(double *)((int)pdVar10 + 0xc) = param_2;
            *(double *)((int)pdVar10 + 4) = in_xd4;
            pdVar10 = (double *)((int)pdVar10 + -4);
            *pdVar10 = dVar26;
          }
          unaff_r8 = pdVar10 + 2;
          goto LAB_8c614e74;
        }
      }
      puVar3 = PTR_FUN_8c614fb4;
      uVar30 = SUB84(param_2,0);
      if (*(int *)((int)pdVar18 + -0x3c) != 0) {
        *DAT_8c614fb0 = 0;
        dVar19 = (double)(*(code *)puVar3)(puVar11,puVar12,puVar13);
        pdVar7 = unaff_r8;
      }
      uVar31 = DAT_8c614fa8;
      param_2 = in_xd12;
      if (in_FPSCR_SZ == '\0') {
        param_2 = (double)CONCAT44(SUB84(dVar32,0),uVar30);
      }
      iVar6 = *(int *)((int)pdVar18 + -0x48);
      if (in_FPSCR_SZ == '\0') {
        dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),*(undefined4 *)(puVar13 + 2));
        uVar25 = CONCAT44(*(undefined4 *)((int)puVar13 + 0x14),SUB84(in_dr14,0));
        param_1 = (double)((ulonglong)in_dr14 >> 0x20);
      }
      else {
        in_xd12 = (double)puVar13[2];
        uVar25 = *(undefined8 *)((int)puVar13 + 0x14);
        param_1 = dVar26;
        in_xd4 = in_dr14;
      }
      in_dr14 = (double)CONCAT44((int)((ulonglong)uVar25 >> 0x20),-(float)uVar25);
      *(int *)((int)pdVar18 + -0x48) = iVar6 + -1;
      puVar11 = puVar12;
      puVar12 = puVar13;
    } while (iVar6 + -1 != 0);
    if (pdVar7 < unaff_r8) {
      if (in_FPSCR_SZ == '\0') {
        uVar25 = CONCAT44(*(undefined4 *)puVar13,SUB84(param_1,0));
        uVar28 = CONCAT44(*(undefined4 *)((int)puVar13 + 4),SUB84(param_2,0));
        pdVar7 = (double *)(puVar13 + 1);
        in_dr2 = (double)CONCAT44(SUB84(in_dr2,0),SUB84(in_dr2,0));
        dVar19 = (double)CONCAT44(SUB84(dVar19,0),SUB84(dVar19,0));
      }
      else {
        uVar25 = *puVar13;
        uVar28 = puVar13[1];
        pdVar7 = (double *)(puVar13 + 2);
        in_dr2 = in_xd2;
        dVar19 = in_xd0;
      }
      in_dr2 = (double)((ulonglong)in_dr2 & 0xffffffff00000000);
                    /* WARNING: Ignoring partial resolution of indirect */
      fVar20 = (float)uVar28;
      fVar22 = (float)uVar25;
      if (in_FPSCR_PR == '\0') {
        fVar24 = SUB84(dVar19,0) * fVar22 +
                 (float)((ulonglong)dVar19 >> 0x20) * (float)((ulonglong)uVar25 >> 0x20) +
                 fVar20 * 0.0 +
                 (float)((ulonglong)in_dr2 >> 0x20) * (float)((ulonglong)uVar28 >> 0x20);
      }
      if (in_FPSCR_SZ == '\0') {
        uVar30 = SUB84(param_3,0);
        param_3 = (double)CONCAT44(*(undefined4 *)pdVar7,uVar30);
        param_4 = (double)CONCAT44(*(undefined4 *)((int)pdVar7 + 4),SUB84(param_4,0));
        pdVar7 = pdVar7 + 1;
        param_1 = (double)CONCAT44(fVar22,fVar22);
        dVar19 = (double)CONCAT44(uVar30,SUB84(dVar19,0));
      }
      else {
        param_3 = *pdVar7;
        param_4 = pdVar7[1];
        pdVar7 = pdVar7 + 2;
        uVar30 = SUB84(param_3,0);
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
      fVar22 = (float)((ulonglong)dVar19 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        pdVar9 = (double *)((int)unaff_r8 + 0xc);
        *(int *)pdVar9 = (int)((ulonglong)param_4 >> 0x20);
        pdVar7 = (double *)((int)pdVar7 + -4);
        *(float *)pdVar7 = fVar22;
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
        param_2 = (double)CONCAT44((int)((ulonglong)param_2 >> 0x20),SUB84(param_2,0) * fVar22);
      }
      else {
        param_2 = param_2 * dVar19;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar9 = (double *)((int)pdVar9 + -4);
        *(undefined4 *)pdVar9 = uVar30;
      }
      else {
        pdVar9 = pdVar9 + -1;
        *pdVar9 = in_xd8;
      }
      if (in_FPSCR_PR == '\0') {
        param_2 = (double)CONCAT44((float)((ulonglong)param_2 >> 0x20) * fVar22,SUB84(param_2,0));
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
        param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),SUB84(param_1,0) * fVar22);
      }
      else {
        param_1 = param_1 * dVar19;
      }
      *(undefined4 *)((int)pdVar9 + -4) = uVar31;
      if (in_FPSCR_SZ == '\0') {
        *(int *)(pdVar9 + 3) = SUB84(param_2,0);
        *(int *)((int)pdVar9 + 0x14) = (int)((ulonglong)param_2 >> 0x20);
        *(int *)(pdVar9 + 2) = SUB84(param_1,0);
        pdVar9 = (double *)((int)pdVar9 + 0xc);
        *(int *)pdVar9 = (int)((ulonglong)param_1 >> 0x20);
      }
      else {
        *(double *)((int)pdVar9 + 0x14) = in_xd6;
        *(double *)((int)pdVar9 + 0xc) = param_2;
        *(double *)((int)pdVar9 + 4) = in_xd4;
        pdVar9 = (double *)((int)pdVar9 + -4);
        *pdVar9 = param_1;
      }
      unaff_r8 = pdVar9 + 2;
    }
    iVar6 = *(int *)((int)pdVar18 + -0x38);
    *(int *)((int)pdVar18 + -0x38) = iVar6 + -1;
    if (iVar6 + -1 == 0) {
      *DAT_8c614fb8 = *DAT_8c614fb8 + *(int *)((int)pdVar18 + -0x44);
      *DAT_8c614fbc = *DAT_8c614fbc + *(int *)((int)pdVar18 + -0x44);
      return;
    }
  } while( true );
}

