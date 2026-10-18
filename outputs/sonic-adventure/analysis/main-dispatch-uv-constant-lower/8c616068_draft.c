/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c616302) */
/* WARNING: Removing unreachable block (ram,0x8c61625c) */
/* WARNING: Removing unreachable block (ram,0x8c6163ea) */
/* WARNING: Removing unreachable block (ram,0x8c6162b6) */
/* WARNING: Removing unreachable block (ram,0x8c61633e) */
/* WARNING: Removing unreachable block (ram,0x8c616370) */
/* WARNING: Removing unreachable block (ram,0x8c616380) */
/* WARNING: Removing unreachable block (ram,0x8c61638e) */
/* WARNING: Removing unreachable block (ram,0x8c61639c) */
/* WARNING: Removing unreachable block (ram,0x8c6163b4) */
/* WARNING: Removing unreachable block (ram,0x8c6163c4) */
/* WARNING: Removing unreachable block (ram,0x8c61641c) */

void FUN_8c616068(double param_1,double param_2,double param_3,int param_4)

{
  short sVar1;
  ushort uVar2;
  short sVar3;
  double *pdVar4;
  int iVar5;
  undefined2 *puVar6;
  undefined *puVar7;
  int *piVar8;
  int iVar9;
  int iVar10;
  int *piVar11;
  double *unaff_r8;
  double *pdVar12;
  double *pdVar13;
  int iVar14;
  int iVar15;
  int iVar16;
  ushort *puVar17;
  ushort *puVar18;
  ushort *puVar19;
  short *psVar20;
  short *psVar21;
  short *psVar22;
  double *pdVar23;
  double dVar24;
  float fVar25;
  float fVar26;
  double in_dr2;
  ulonglong uVar27;
  uint uVar29;
  float fVar30;
  double dVar28;
  undefined4 uVar31;
  undefined4 uVar32;
  undefined4 uVar33;
  undefined8 in_dr12;
  double dVar34;
  undefined4 uVar35;
  double in_dr14;
  undefined8 uVar36;
  double in_xd0;
  double dVar37;
  double in_xd2;
  double dVar38;
  double dVar39;
  double in_xd4;
  double dVar40;
  double in_xd6;
  double dVar41;
  double in_xd8;
  double in_xd12;
  double in_xd14;
  bool bVar42;
  undefined4 in_PR;
  undefined2 uVar43;
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
    pdVar23 = &dStack_28;
    dStack_28 = (double)CONCAT44((int)in_dr12,(int)((ulonglong)in_dr12 >> 0x20));
  }
  else {
    pdVar23 = (double *)local_38;
    uStack_20 = in_xd14;
    dStack_28 = in_dr14;
  }
  *(undefined4 *)((int)pdVar23 + -4) = in_PR;
  dVar34 = (double)CONCAT44(0x3f800000,(int)in_dr12);
  dVar24 = (double)(*(code *)PTR_FUN_8c6160a0)();
  puVar17 = *(ushort **)(param_4 + 4);
  psVar20 = *(short **)(param_4 + 0x14);
  *(uint *)((int)pdVar23 + -0x38) = (uint)*(ushort *)(param_4 + 2);
  *(undefined4 *)((int)pdVar23 + -0x48) = *DAT_8c6160a4;
  *(undefined4 *)((int)pdVar23 + -0x44) = 0;
  do {
    uVar35 = SUB84(in_dr14,0);
    uVar2 = *puVar17;
    puVar18 = puVar17 + 1;
    *(uint *)((int)pdVar23 + -0x4c) = (uint)uVar2;
    uVar31 = (undefined4)((ulonglong)dVar34 >> 0x20);
    dVar40 = dVar34;
    if (in_FPSCR_SZ == '\0') {
      dVar40 = in_xd14;
      uVar35 = uVar31;
    }
    in_xd14 = dVar40;
    if ((uVar2 & DAT_8c6161c8) != 0) {
      if (in_FPSCR_PR == '\0') {
        param_1 = (double)CONCAT44(0xbf800000,SUB84(param_1,0));
      }
      else {
        param_1 = -(double)CONCAT44(0x3f800000,SUB84(param_1,0));
      }
      *(uint *)((int)pdVar23 + -0x4c) = *(uint *)((int)pdVar23 + -0x4c) & (int)DAT_8c6161c4;
      in_xd14 = param_1;
      if (in_FPSCR_SZ == '\0') {
        uVar35 = (undefined4)((ulonglong)param_1 >> 0x20);
        in_xd14 = dVar40;
      }
    }
    *(int *)((int)pdVar23 + -0x44) = *(int *)((int)pdVar23 + -0x44) + *(int *)((int)pdVar23 + -0x4c)
    ;
    *(int *)((int)pdVar23 + -0x4c) = *(int *)((int)pdVar23 + -0x4c) + -2;
    puVar19 = puVar17 + 2;
    puVar17 = puVar17 + 3;
    iVar15 = (uint)*puVar19 * 0x20 + *(int *)((int)pdVar23 + -0x48);
    iVar14 = (uint)*puVar18 * 0x20 + *(int *)((int)pdVar23 + -0x48);
    pdVar12 = unaff_r8;
    if (in_FPSCR_SZ == '\0') {
      dVar34 = (double)CONCAT44(uVar31,*(undefined4 *)(iVar15 + 0x10));
      param_1 = (double)(ulonglong)*(uint *)(iVar14 + 0x10);
      in_dr14 = (double)CONCAT44(*(undefined4 *)(iVar15 + 0x14),uVar35);
      param_2 = (double)CONCAT44(*(undefined4 *)(iVar14 + 0x14),SUB84(param_2,0));
      dVar40 = in_xd4;
    }
    else {
      in_xd12 = *(double *)(iVar15 + 0x10);
      in_dr14 = *(double *)(iVar15 + 0x14);
      param_2 = *(double *)(iVar14 + 0x14);
      dVar40 = *(double *)(iVar14 + 0x10);
    }
    do {
      uVar2 = *puVar17;
      puVar17 = puVar17 + 1;
      iVar16 = (uint)uVar2 * 0x20 + *(int *)((int)pdVar23 + -0x48);
      *(undefined4 *)((int)pdVar23 + -0x3c) = 0;
      uVar35 = (undefined4)((ulonglong)dVar34 >> 0x20);
      dVar28 = dVar34;
      if (in_FPSCR_SZ == '\0') {
        dVar28 = (double)CONCAT44(uVar35,SUB84(param_1,0));
      }
      param_1 = dVar28;
      if ((*(uint *)(iVar14 + 0xc) & *(uint *)(iVar15 + 0xc) & *(uint *)(iVar16 + 0xc) & 0x3f) == 0)
      {
        if (((*(uint *)(iVar14 + 0xc) | *(uint *)(iVar15 + 0xc) | *(uint *)(iVar16 + 0xc)) & 1) == 0
           ) {
          if (in_FPSCR_SZ == '\0') {
            dVar28 = (double)CONCAT44(*(undefined4 *)(iVar16 + 0x14),SUB84(dVar34,0));
          }
          else {
            dVar28 = *(double *)(iVar16 + 0x14);
            in_xd2 = in_xd12;
          }
          fVar30 = (float)((ulonglong)in_dr14 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) - fVar30,
                                      SUB84(dVar28,0) - SUB84(param_1,0));
          }
          else {
            dVar28 = (dVar28 - param_1) - in_dr14;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                      *(undefined4 *)(iVar16 + 0x10));
          }
          else {
            in_xd0 = *(double *)(iVar16 + 0x10);
          }
          if (in_FPSCR_PR == '\0') {
            dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                      SUB84(dVar24,0) - SUB84(dVar34,0));
            fVar26 = SUB84(dVar28,0) * (float)((ulonglong)dVar28 >> 0x20);
          }
          else {
            dVar24 = dVar24 - dVar34;
            fVar26 = SUB84(dVar28 * dVar28,0);
          }
          dVar28 = in_dr14;
          if (in_FPSCR_SZ == '\0') {
            dVar28 = (double)CONCAT44(fVar30,fVar26);
          }
          if (in_FPSCR_PR == '\0') {
            fVar30 = ((float)((ulonglong)dVar28 >> 0x20) - (float)((ulonglong)param_2 >> 0x20)) *
                     SUB84(dVar24,0);
            in_dr2 = (double)CONCAT44(fVar30,SUB84(dVar28,0) - fVar30);
          }
          else {
            in_dr2 = (dVar28 - param_2) * dVar24;
            in_dr2 = in_dr2 - in_dr2;
          }
          dVar28 = in_xd2;
          if (in_FPSCR_SZ == '\0') {
            dVar28 = (double)CONCAT44(SUB84(in_dr2,0),SUB84(param_1,0));
          }
          if (in_FPSCR_PR == '\0') {
            param_1 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) * SUB84(in_dr14,0),
                                       SUB84(dVar28,0));
          }
          else {
            param_1 = dVar28 * in_dr14;
          }
        }
        else {
          piVar11 = (int *)((int)pdVar23 + -0x10);
          *piVar11 = iVar14;
          *(int *)((int)pdVar23 + -0xc) = iVar15;
          *(int *)((int)pdVar23 + -8) = iVar16;
          iVar10 = 0;
          while( true ) {
            iVar5 = DAT_8c6161cc;
            pdVar13 = (double *)((int)pdVar23 + -0x34);
            uVar31 = (undefined4)((ulonglong)in_dr2 >> 0x20);
            if (2 < iVar10) break;
            piVar8 = piVar11 + iVar10;
            *(int **)((int)pdVar23 + -0x50) = piVar8;
            *(int *)((int)pdVar23 + -0x54) = iVar10 * 0xc + (int)pdVar13;
            iVar10 = iVar10 + 1;
            if ((*(uint *)(*piVar8 + 0xc) & 1) == 0) {
              dVar28 = dVar34;
              if (in_FPSCR_SZ == '\0') {
                dVar28 = (double)CONCAT44(uVar35,SUB84(param_1,0));
              }
              iVar9 = **(int **)((int)pdVar23 + -0x50);
              *(int *)((int)pdVar23 + -0x40) = iVar9;
              if (in_FPSCR_SZ == '\0') {
                in_dr2 = (double)CONCAT44(uVar31,*(undefined4 *)(iVar9 + 0x18));
              }
              else {
                in_xd2 = *(double *)(iVar9 + 0x18);
              }
              if (in_FPSCR_PR == '\0') {
                param_1 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) / SUB84(in_dr2,0),
                                           SUB84(dVar28,0));
              }
              else {
                param_1 = dVar28 / in_dr2;
              }
              if (in_FPSCR_SZ == '\0') {
                in_dr2 = (double)(ulonglong)*(uint *)(DAT_8c6161cc + 0xc);
              }
              else {
                in_xd2 = *(double *)(DAT_8c6161cc + 0xc);
              }
              if (in_FPSCR_SZ == '\0') {
                dVar28 = (double)CONCAT44(*(undefined4 *)(*(int *)((int)pdVar23 + -0x40) + 0x10),
                                          SUB84(in_dr2,0));
              }
              else {
                dVar28 = *(double *)(*(int *)((int)pdVar23 + -0x40) + 0x10);
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
                                          *(undefined4 *)DAT_8c6161d0);
              }
              else {
                in_xd2 = *DAT_8c6161d0;
              }
              fVar30 = (float)((ulonglong)param_1 >> 0x20);
              if (in_FPSCR_PR == '\0') {
                dVar28 = (double)CONCAT44(((float)((ulonglong)dVar28 >> 0x20) * fVar30) /
                                          SUB84(dVar28,0),SUB84(dVar28,0));
              }
              else {
                dVar28 = (dVar28 * param_1) / (dVar28 * param_1);
              }
              if (in_FPSCR_SZ == '\0') {
                *(int *)*(double **)((int)pdVar23 + -0x54) = (int)((ulonglong)dVar28 >> 0x20);
              }
              else {
                **(double **)((int)pdVar23 + -0x54) = dVar28;
              }
              if (in_FPSCR_SZ == '\0') {
                dVar28 = (double)(ulonglong)*(uint *)(iVar5 + 0x10);
              }
              else {
                in_xd2 = *(double *)(iVar5 + 0x10);
              }
              if (in_FPSCR_SZ == '\0') {
                dVar28 = (double)CONCAT44(*(undefined4 *)(**(int **)((int)pdVar23 + -0x50) + 0x14),
                                          SUB84(dVar28,0));
              }
              else {
                dVar28 = *(double *)(**(int **)((int)pdVar23 + -0x50) + 0x14);
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
                                          *(undefined4 *)DAT_8c6161d4);
              }
              else {
                in_xd2 = *DAT_8c6161d4;
              }
              if (in_FPSCR_PR == '\0') {
                in_dr2 = (double)CONCAT44(((float)((ulonglong)dVar28 >> 0x20) * fVar30) /
                                          SUB84(dVar28,0),SUB84(dVar28,0));
              }
              else {
                in_dr2 = (dVar28 * param_1) / (dVar28 * param_1);
              }
              if (in_FPSCR_SZ == '\0') {
                *(int *)(*(int *)((int)pdVar23 + -0x54) + 4) = (int)((ulonglong)in_dr2 >> 0x20);
              }
              else {
                *(double *)(*(int *)((int)pdVar23 + -0x54) + 4) = in_dr2;
              }
              if (in_FPSCR_SZ == '\0') {
                *(float *)(*(int *)((int)pdVar23 + -0x54) + 8) = fVar30;
              }
              else {
                *(double *)(*(int *)((int)pdVar23 + -0x54) + 8) = param_1;
              }
            }
            else {
              if (in_FPSCR_SZ == '\0') {
                uVar33 = *(undefined4 *)(**(int **)((int)pdVar23 + -0x50) + 0x10);
                in_dr2 = (double)CONCAT44(uVar31,uVar33);
                *(undefined4 *)*(double **)((int)pdVar23 + -0x54) = uVar33;
              }
              else {
                in_xd2 = *(double *)(**(int **)((int)pdVar23 + -0x50) + 0x10);
                **(double **)((int)pdVar23 + -0x54) = in_xd2;
              }
              if (in_FPSCR_SZ == '\0') {
                uVar31 = *(undefined4 *)(**(int **)((int)pdVar23 + -0x50) + 0x14);
                in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),uVar31);
                *(undefined4 *)(*(int *)((int)pdVar23 + -0x54) + 4) = uVar31;
              }
              else {
                in_xd2 = *(double *)(**(int **)((int)pdVar23 + -0x50) + 0x14);
                *(double *)(*(int *)((int)pdVar23 + -0x54) + 4) = in_xd2;
              }
              if (in_FPSCR_SZ == '\0') {
                uVar31 = *(undefined4 *)(**(int **)((int)pdVar23 + -0x50) + 0x18);
                in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),uVar31);
                *(undefined4 *)(*(int *)((int)pdVar23 + -0x54) + 8) = uVar31;
              }
              else {
                in_xd2 = *(double *)(**(int **)((int)pdVar23 + -0x50) + 0x18);
                *(double *)(*(int *)((int)pdVar23 + -0x54) + 8) = in_xd2;
              }
            }
          }
          if (in_FPSCR_SZ == '\0') {
            uVar33 = *(undefined4 *)((int)pdVar23 + -0x2c);
            dVar24 = (double)CONCAT44(uVar33,SUB84(dVar24,0));
            param_3 = (double)CONCAT44(uVar33,SUB84(param_3,0));
          }
          else {
            dVar24 = *(double *)((int)pdVar23 + -0x2c);
            uVar33 = (undefined4)((ulonglong)dVar24 >> 0x20);
            param_3 = dVar24;
          }
          pdVar4 = (double *)((int)pdVar23 + -0x28);
          *(double **)((int)pdVar23 + -0x54) = pdVar4;
          if (in_FPSCR_SZ == '\0') {
            uVar32 = *(undefined4 *)((int)pdVar23 + -0x30);
            param_3 = (double)CONCAT44(uVar33,uVar32);
            dVar28 = (double)CONCAT44(*(undefined4 *)((int)pdVar23 + -0x24),SUB84(param_1,0));
            param_2 = (double)CONCAT44(*(undefined4 *)((int)pdVar23 + -0x20),SUB84(param_2,0));
            in_dr2 = (double)CONCAT44(uVar31,uVar32);
          }
          else {
            in_xd2 = *(double *)((int)pdVar23 + -0x30);
            dVar28 = *(double *)((int)pdVar23 + -0x24);
            param_2 = *(double *)((int)pdVar23 + -0x20);
            uVar32 = SUB84(param_3,0);
            in_xd8 = in_xd2;
          }
          fVar30 = (float)((ulonglong)dVar28 >> 0x20);
          fVar26 = (float)((ulonglong)param_2 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * fVar30,SUB84(dVar24,0));
            fVar25 = SUB84(in_dr2,0) * fVar26;
          }
          else {
            dVar24 = dVar24 * dVar28;
            fVar25 = SUB84(in_dr2 * param_2,0);
          }
          if (in_FPSCR_SZ == '\0') {
            uVar31 = *(undefined4 *)pdVar13;
            param_2 = (double)CONCAT44(fVar26,uVar31);
            dVar28 = (double)CONCAT44(fVar30,*(undefined4 *)pdVar4);
            dVar41 = (double)CONCAT44(uVar31,fVar25);
          }
          else {
            dVar41 = *pdVar13;
            dVar40 = *pdVar4;
            uVar31 = SUB84(param_2,0);
            in_xd6 = dVar41;
          }
          if (in_FPSCR_PR == '\0') {
            dVar41 = (double)CONCAT44((float)((ulonglong)dVar41 >> 0x20) *
                                      (float)((ulonglong)param_2 >> 0x20),SUB84(dVar41,0));
          }
          else {
            dVar41 = dVar41 * param_2;
          }
          dVar37 = in_xd8;
          if (in_FPSCR_SZ == '\0') {
            dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),uVar32);
            dVar37 = in_xd0;
          }
          if (in_FPSCR_PR == '\0') {
            dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) - SUB84(dVar41,0),
                                      SUB84(dVar24,0));
          }
          else {
            dVar24 = dVar24 - dVar41;
          }
          dVar38 = param_3;
          if (in_FPSCR_SZ == '\0') {
            dVar41 = (double)CONCAT44((int)((ulonglong)dVar41 >> 0x20),
                                      (int)((ulonglong)param_3 >> 0x20));
            dVar38 = in_xd2;
          }
          fVar30 = SUB84(dVar28,0);
          if (in_FPSCR_PR == '\0') {
            fVar26 = SUB84(dVar41,0) * fVar30;
            dVar41 = (double)CONCAT44((float)((ulonglong)dVar41 >> 0x20) - fVar26,fVar26);
          }
          else {
            dVar41 = dVar41 * dVar28 - dVar41 * dVar28;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar41 = (double)CONCAT44((int)((ulonglong)dVar41 >> 0x20),
                                      *(undefined4 *)((int)pdVar23 + -0x18));
          }
          else {
            dVar38 = *(double *)((int)pdVar23 + -0x18);
          }
          if (in_FPSCR_PR == '\0') {
            fVar26 = SUB84(dVar41,0) * (float)((ulonglong)dVar41 >> 0x20);
          }
          else {
            fVar26 = SUB84(dVar41 * dVar41,0);
          }
          if (in_FPSCR_SZ == '\0') {
            uVar27 = CONCAT44((int)*(ulonglong *)((int)pdVar23 + -0x1c),fVar26);
          }
          else {
            uVar27 = *(ulonglong *)((int)pdVar23 + -0x1c);
          }
          if (in_FPSCR_PR == '\0') {
            uVar27 = (ulonglong)
                     (uint)((float)((ulonglong)dVar24 >> 0x20) * (float)(uVar27 >> 0x20) +
                           (float)uVar27);
          }
          dVar41 = dVar38;
          dVar39 = in_xd6;
          if (in_FPSCR_SZ == '\0') {
            dVar41 = (double)CONCAT44((int)uVar27,uVar31);
            dVar39 = dVar38;
          }
          if (in_FPSCR_PR == '\0') {
            fVar26 = SUB84(dVar41,0) * (float)((ulonglong)dVar28 >> 0x20);
            dVar41 = (double)CONCAT44((int)((ulonglong)dVar41 >> 0x20),fVar26);
            fVar26 = SUB84(dVar24,0) * fVar30 - fVar26;
          }
          else {
            dVar41 = dVar41 * dVar28;
            fVar26 = SUB84(dVar24 * dVar28 - dVar41,0);
          }
          if (in_FPSCR_SZ == '\0') {
            dVar41 = (double)CONCAT44((int)((ulonglong)dVar41 >> 0x20),
                                      *(undefined4 *)((int)pdVar23 + -0x14));
            dVar24 = (double)CONCAT44(fVar26,fVar26);
          }
          else {
            dVar39 = *(double *)((int)pdVar23 + -0x14);
            dVar24 = dVar37;
          }
          if (in_FPSCR_PR == '\0') {
            dVar41 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar41,0) +
                                      (float)((ulonglong)dVar41 >> 0x20),SUB84(dVar41,0));
          }
          in_xd2 = in_xd14;
          if (in_FPSCR_SZ == '\0') {
            dVar41 = (double)CONCAT44((int)((ulonglong)dVar41 >> 0x20),SUB84(in_dr14,0));
            in_xd2 = dVar39;
          }
          uVar31 = (undefined4)((ulonglong)dVar41 >> 0x20);
          fVar26 = -SUB84(dVar41,0);
          dVar41 = (double)CONCAT44(uVar31,fVar26);
          dVar28 = dVar41;
          if (in_FPSCR_SZ == '\0') {
            dVar28 = (double)CONCAT44(uVar31,fVar30);
          }
          if (in_FPSCR_PR == '\0') {
            dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) * fVar26,SUB84(dVar28,0));
          }
          else {
            dVar28 = dVar28 * dVar41;
          }
          in_dr2 = (double)(ulonglong)(uint)fVar26;
          if (in_FPSCR_PR == '\0') {
            bVar42 = 0.0 < (float)((ulonglong)dVar28 >> 0x20);
          }
          else {
            bVar42 = in_dr2 < dVar28;
          }
          param_1 = dVar28;
          in_xd0 = dVar37;
          if ((!bVar42) &&
             (*(undefined4 *)((int)pdVar23 + -0x3c) = 1, param_1 = dVar34, in_FPSCR_SZ == '\0')) {
            param_1 = (double)CONCAT44(uVar35,SUB84(dVar28,0));
          }
        }
      }
      uVar31 = DAT_8c6164b0;
      uVar35 = DAT_8c6164ac;
      uVar29 = (uint)((ulonglong)in_dr2 >> 0x20);
      fVar30 = (float)((ulonglong)param_1 >> 0x20);
      if (pdVar12 < unaff_r8) {
        if (in_FPSCR_PR == '\0') {
          bVar42 = fVar30 < 0.0;
        }
        else {
          bVar42 = param_1 < (double)((ulonglong)uVar29 << 0x20);
        }
        psVar21 = psVar20;
        if (bVar42) {
LAB_8c61631e:
          uVar35 = DAT_8c6164b0;
          psVar22 = psVar21 + 2;
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44(*(undefined4 *)(iVar15 + 0x10),SUB84(param_1,0));
            param_2 = (double)CONCAT44(*(undefined4 *)(iVar15 + 0x14),SUB84(param_2,0));
            dVar24 = (double)((longlong)dVar24 << 0x20);
          }
          else {
            param_1 = *(double *)(iVar15 + 0x10);
            param_2 = *(double *)(iVar15 + 0x18);
            dVar24 = in_xd0;
          }
          sVar3 = *psVar21;
          sVar1 = psVar21[1];
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)unaff_r8 + 0xc) = (int)((ulonglong)param_2 >> 0x20);
            unaff_r8 = unaff_r8 + 1;
            *(int *)unaff_r8 = SUB84(param_1,0);
          }
          else {
            unaff_r8[1] = param_2;
            *unaff_r8 = dVar40;
          }
          fVar30 = (float)((ulonglong)dVar24 >> 0x20);
          dVar24 = (double)CONCAT44(fVar30,(float)(int)sVar1);
          if (in_FPSCR_SZ == '\0') {
            pdVar13 = (double *)((int)unaff_r8 + -4);
            *(int *)pdVar13 = (int)((ulonglong)param_1 >> 0x20);
          }
          else {
            pdVar13 = unaff_r8 + -1;
            *pdVar13 = param_1;
          }
          if (in_FPSCR_PR == '\0') {
            dVar24 = (double)CONCAT44(fVar30,(float)(int)sVar1 * fVar30);
          }
          else {
            dVar24 = dVar24 * dVar24;
          }
          *(undefined4 *)((int)pdVar13 + -4) = uVar35;
          if (in_FPSCR_PR == '\0') {
            fVar30 = (float)(int)sVar3;
          }
          else {
            fVar30 = (float)((ulonglong)(double)(int)sVar3 >> 0x20);
          }
          if (in_FPSCR_PR == '\0') {
            in_dr2 = (double)CONCAT44(fVar30 * (float)((ulonglong)dVar24 >> 0x20),0x3f800000);
          }
          else {
            in_dr2 = (double)CONCAT44(fVar30,0x3f800000) * dVar24;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)pdVar13 + 0x14) = SUB84(in_dr2,0);
            *(int *)(pdVar13 + 2) = SUB84(dVar24,0);
            pdVar13 = (double *)((int)pdVar13 + 0xc);
            *(int *)pdVar13 = (int)((ulonglong)in_dr2 >> 0x20);
          }
          else {
            pdVar13[2] = in_xd2;
            pdVar13[1] = in_xd0;
            *pdVar13 = in_dr2;
          }
          unaff_r8 = pdVar13 + 2;
        }
        else {
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44(*(undefined4 *)(iVar15 + 0x10),SUB84(param_1,0));
            param_2 = (double)CONCAT44(*(undefined4 *)(iVar15 + 0x14),SUB84(param_2,0));
            dVar24 = (double)((longlong)dVar24 << 0x20);
          }
          else {
            param_1 = *(double *)(iVar15 + 0x10);
            param_2 = *(double *)(iVar15 + 0x18);
            dVar24 = in_xd0;
          }
          sVar3 = *psVar20;
          sVar1 = psVar20[1];
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)unaff_r8 + 0xc) = (int)((ulonglong)param_2 >> 0x20);
            unaff_r8 = unaff_r8 + 1;
            *(int *)unaff_r8 = SUB84(param_1,0);
          }
          else {
            unaff_r8[1] = param_2;
            *unaff_r8 = dVar40;
          }
          fVar30 = (float)((ulonglong)dVar24 >> 0x20);
          dVar24 = (double)CONCAT44(fVar30,(float)(int)sVar1);
          if (in_FPSCR_SZ == '\0') {
            pdVar12 = (double *)((int)unaff_r8 + -4);
            *(int *)pdVar12 = (int)((ulonglong)param_1 >> 0x20);
          }
          else {
            pdVar12 = unaff_r8 + -1;
            *pdVar12 = param_1;
          }
          if (in_FPSCR_PR == '\0') {
            dVar24 = (double)CONCAT44(fVar30,(float)(int)sVar1 * fVar30);
          }
          else {
            dVar24 = dVar24 * dVar24;
          }
          *(undefined4 *)((int)pdVar12 + -4) = uVar35;
          if (in_FPSCR_PR == '\0') {
            fVar30 = (float)(int)sVar3;
          }
          else {
            fVar30 = (float)((ulonglong)(double)(int)sVar3 >> 0x20);
          }
          if (in_FPSCR_PR == '\0') {
            in_dr2 = (double)CONCAT44(fVar30 * (float)((ulonglong)dVar24 >> 0x20),0x3f800000);
          }
          else {
            in_dr2 = (double)CONCAT44(fVar30,0x3f800000) * dVar24;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)pdVar12 + 0x14) = SUB84(in_dr2,0);
            *(int *)(pdVar12 + 2) = SUB84(dVar24,0);
            pdVar12 = (double *)((int)pdVar12 + 0xc);
            *(int *)pdVar12 = (int)((ulonglong)in_dr2 >> 0x20);
          }
          else {
            pdVar12[2] = in_xd2;
            pdVar12[1] = in_xd0;
            *pdVar12 = in_dr2;
          }
          unaff_r8 = pdVar12 + 2;
          pdVar12 = unaff_r8;
          psVar22 = psVar20;
        }
      }
      else {
        in_dr2 = (double)((ulonglong)uVar29 << 0x20);
        if (in_FPSCR_PR == '\0') {
          bVar42 = fVar30 < 0.0;
        }
        else {
          bVar42 = param_1 < in_dr2;
        }
        psVar22 = psVar20 + 2;
        if (bVar42) {
          psVar21 = psVar20 + 2;
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44(*(undefined4 *)(iVar14 + 0x10),SUB84(param_1,0));
            param_2 = (double)CONCAT44(*(undefined4 *)(iVar14 + 0x14),SUB84(param_2,0));
            dVar24 = (double)((longlong)dVar24 << 0x20);
          }
          else {
            param_1 = *(double *)(iVar14 + 0x10);
            param_2 = *(double *)(iVar14 + 0x18);
            dVar24 = in_xd0;
          }
          sVar3 = *psVar20;
          sVar1 = psVar20[1];
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)unaff_r8 + 0xc) = (int)((ulonglong)param_2 >> 0x20);
            unaff_r8 = unaff_r8 + 1;
            *(int *)unaff_r8 = SUB84(param_1,0);
          }
          else {
            unaff_r8[1] = param_2;
            *unaff_r8 = dVar40;
          }
          fVar30 = (float)((ulonglong)dVar24 >> 0x20);
          dVar24 = (double)CONCAT44(fVar30,(float)(int)sVar1);
          if (in_FPSCR_SZ == '\0') {
            pdVar13 = (double *)((int)unaff_r8 + -4);
            *(int *)pdVar13 = (int)((ulonglong)param_1 >> 0x20);
          }
          else {
            pdVar13 = unaff_r8 + -1;
            *pdVar13 = param_1;
          }
          if (in_FPSCR_PR == '\0') {
            dVar24 = (double)CONCAT44(fVar30,(float)(int)sVar1 * fVar30);
          }
          else {
            dVar24 = dVar24 * dVar24;
          }
          *(undefined4 *)((int)pdVar13 + -4) = uVar31;
          if (in_FPSCR_PR == '\0') {
            fVar30 = (float)(int)sVar3;
          }
          else {
            fVar30 = (float)((ulonglong)(double)(int)sVar3 >> 0x20);
          }
          if (in_FPSCR_PR == '\0') {
            dVar28 = (double)CONCAT44(fVar30 * (float)((ulonglong)dVar24 >> 0x20),0x3f800000);
          }
          else {
            dVar28 = (double)CONCAT44(fVar30,0x3f800000) * dVar24;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)pdVar13 + 0x14) = SUB84(dVar28,0);
            *(int *)(pdVar13 + 2) = SUB84(dVar24,0);
            pdVar13 = (double *)((int)pdVar13 + 0xc);
            *(int *)pdVar13 = (int)((ulonglong)dVar28 >> 0x20);
          }
          else {
            pdVar13[2] = in_xd2;
            pdVar13[1] = in_xd0;
            *pdVar13 = dVar28;
          }
          unaff_r8 = pdVar13 + 2;
          goto LAB_8c61631e;
        }
      }
      puVar6 = DAT_8c6164b8;
      uVar35 = SUB84(param_2,0);
      if (*(int *)((int)pdVar23 + -0x3c) != 0) {
        *DAT_8c6164b4 = 0;
        uVar31 = (undefined4)((ulonglong)in_dr2 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          uVar43 = (undefined2)(int)(float)(int)psVar22[-2];
        }
        else {
          uVar43 = (undefined2)(int)(double)CONCAT44(uVar31,(float)(int)psVar22[-2]);
        }
        *puVar6 = uVar43;
        if (in_FPSCR_PR == '\0') {
          uVar43 = (undefined2)(int)(float)(int)psVar22[-1];
        }
        else {
          uVar43 = (undefined2)(int)(double)CONCAT44(uVar31,(float)(int)psVar22[-1]);
        }
        puVar6[1] = uVar43;
        if (in_FPSCR_PR == '\0') {
          uVar43 = (undefined2)(int)(float)(int)*psVar22;
        }
        else {
          uVar43 = (undefined2)(int)(double)CONCAT44(uVar31,(float)(int)*psVar22);
        }
        puVar6[2] = uVar43;
        if (in_FPSCR_PR == '\0') {
          uVar43 = (undefined2)(int)(float)(int)psVar22[1];
        }
        else {
          uVar43 = (undefined2)(int)(double)CONCAT44(uVar31,(float)(int)psVar22[1]);
        }
        puVar6[3] = uVar43;
        if (in_FPSCR_PR == '\0') {
          uVar43 = (undefined2)(int)(float)(int)psVar22[2];
        }
        else {
          uVar43 = (undefined2)(int)(double)CONCAT44(uVar31,(float)(int)psVar22[2]);
        }
        puVar6[4] = uVar43;
        puVar7 = PTR_FUN_8c6164bc;
        in_dr2 = (double)CONCAT44(uVar31,(float)(int)psVar22[3]);
        if (in_FPSCR_PR == '\0') {
          uVar43 = (undefined2)(int)(float)(int)psVar22[3];
        }
        else {
          uVar43 = (undefined2)(int)in_dr2;
        }
        puVar6[5] = uVar43;
        dVar24 = (double)(*(code *)puVar7)(iVar14,iVar15,iVar16,puVar6);
        pdVar12 = unaff_r8;
      }
      uVar31 = DAT_8c6164ac;
      in_xd4 = in_xd12;
      if (in_FPSCR_SZ == '\0') {
        param_1 = (double)((ulonglong)dVar34 & 0xffffffff);
        in_xd4 = dVar40;
      }
      iVar10 = *(int *)((int)pdVar23 + -0x4c);
      if (in_FPSCR_SZ == '\0') {
        dVar34 = (double)CONCAT44((int)((ulonglong)dVar34 >> 0x20),*(undefined4 *)(iVar16 + 0x10));
        uVar36 = CONCAT44(*(undefined4 *)(iVar16 + 0x14),SUB84(in_dr14,0));
        param_2 = (double)CONCAT44((int)((ulonglong)in_dr14 >> 0x20),uVar35);
      }
      else {
        in_xd12 = *(double *)(iVar16 + 0x10);
        uVar36 = *(undefined8 *)(iVar16 + 0x14);
        param_2 = in_dr14;
      }
      in_dr14 = (double)CONCAT44((int)((ulonglong)uVar36 >> 0x20),-(float)uVar36);
      *(int *)((int)pdVar23 + -0x4c) = iVar10 + -1;
      iVar14 = iVar15;
      iVar15 = iVar16;
      psVar20 = psVar22;
      dVar40 = in_xd4;
    } while (iVar10 + -1 != 0);
    psVar20 = psVar22 + 4;
    if (pdVar12 < unaff_r8) {
      psVar20 = psVar22 + 2;
      if (in_FPSCR_SZ == '\0') {
        param_1 = (double)CONCAT44(*(undefined4 *)(iVar16 + 0x10),SUB84(param_1,0));
        param_2 = (double)CONCAT44(*(undefined4 *)(iVar16 + 0x14),SUB84(param_2,0));
        dVar24 = (double)((longlong)dVar24 << 0x20);
      }
      else {
        param_1 = *(double *)(iVar16 + 0x10);
        param_2 = *(double *)(iVar16 + 0x18);
        dVar24 = in_xd0;
      }
      sVar3 = *psVar22;
      sVar1 = psVar22[1];
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)unaff_r8 + 0xc) = (int)((ulonglong)param_2 >> 0x20);
        unaff_r8 = unaff_r8 + 1;
        *(int *)unaff_r8 = SUB84(param_1,0);
      }
      else {
        unaff_r8[1] = param_2;
        *unaff_r8 = in_xd4;
      }
      fVar30 = (float)((ulonglong)dVar24 >> 0x20);
      dVar24 = (double)CONCAT44(fVar30,(float)(int)sVar1);
      if (in_FPSCR_SZ == '\0') {
        pdVar12 = (double *)((int)unaff_r8 + -4);
        *(int *)pdVar12 = (int)((ulonglong)param_1 >> 0x20);
      }
      else {
        pdVar12 = unaff_r8 + -1;
        *pdVar12 = param_1;
      }
      if (in_FPSCR_PR == '\0') {
        dVar24 = (double)CONCAT44(fVar30,(float)(int)sVar1 * fVar30);
      }
      else {
        dVar24 = dVar24 * dVar24;
      }
      *(undefined4 *)((int)pdVar12 + -4) = uVar31;
      if (in_FPSCR_PR == '\0') {
        fVar30 = (float)(int)sVar3;
      }
      else {
        fVar30 = (float)((ulonglong)(double)(int)sVar3 >> 0x20);
      }
      if (in_FPSCR_PR == '\0') {
        in_dr2 = (double)CONCAT44(fVar30 * (float)((ulonglong)dVar24 >> 0x20),0x3f800000);
      }
      else {
        in_dr2 = (double)CONCAT44(fVar30,0x3f800000) * dVar24;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar12 + 0x14) = SUB84(in_dr2,0);
        *(int *)(pdVar12 + 2) = SUB84(dVar24,0);
        pdVar12 = (double *)((int)pdVar12 + 0xc);
        *(int *)pdVar12 = (int)((ulonglong)in_dr2 >> 0x20);
      }
      else {
        pdVar12[2] = in_xd2;
        pdVar12[1] = in_xd0;
        *pdVar12 = in_dr2;
      }
      unaff_r8 = pdVar12 + 2;
    }
    iVar14 = *(int *)((int)pdVar23 + -0x38);
    *(int *)((int)pdVar23 + -0x38) = iVar14 + -1;
    if (iVar14 + -1 == 0) {
      *DAT_8c6164c0 = *DAT_8c6164c0 + *(int *)((int)pdVar23 + -0x44);
      *DAT_8c6164c4 = *DAT_8c6164c4 + *(int *)((int)pdVar23 + -0x44);
      return;
    }
  } while( true );
}

