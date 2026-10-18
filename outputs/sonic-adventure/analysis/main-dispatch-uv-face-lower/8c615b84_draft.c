/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c615e5e) */
/* WARNING: Removing unreachable block (ram,0x8c615da0) */
/* WARNING: Removing unreachable block (ram,0x8c615f30) */
/* WARNING: Removing unreachable block (ram,0x8c615e08) */
/* WARNING: Removing unreachable block (ram,0x8c615ea4) */
/* WARNING: Removing unreachable block (ram,0x8c615f40) */
/* WARNING: Removing unreachable block (ram,0x8c615f50) */
/* WARNING: Removing unreachable block (ram,0x8c615f60) */
/* WARNING: Removing unreachable block (ram,0x8c615f70) */
/* WARNING: Removing unreachable block (ram,0x8c615f7e) */
/* WARNING: Removing unreachable block (ram,0x8c615fac) */
/* WARNING: Removing unreachable block (ram,0x8c615fec) */
/* WARNING: Restarted to delay deadcode elimination for space: register */

void FUN_8c615b84(double param_1,double param_2,double param_3,double param_4,int param_5)

{
  short sVar1;
  ushort uVar2;
  short sVar3;
  double *pdVar4;
  int iVar5;
  undefined *puVar6;
  undefined2 *puVar7;
  int *piVar8;
  int iVar9;
  int iVar10;
  undefined4 *puVar11;
  double *unaff_r8;
  double *pdVar12;
  double *pdVar13;
  double *pdVar14;
  double *pdVar15;
  double *pdVar16;
  ushort *puVar17;
  ushort *puVar18;
  ushort *puVar19;
  short *psVar20;
  short *psVar21;
  short *psVar22;
  double *pdVar23;
  double dVar24;
  float fVar25;
  double in_dr2;
  uint uVar28;
  double dVar26;
  float fVar29;
  double dVar27;
  undefined4 uVar30;
  undefined4 uVar31;
  double dVar32;
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
  double in_xd8;
  double in_xd10;
  double in_xd12;
  double in_xd14;
  bool bVar40;
  undefined4 in_PR;
  undefined2 uVar41;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  int aiStack_48 [4];
  undefined1 local_38 [16];
  double dStack_28;
  undefined8 uStack_20;
  undefined4 uVar33;
  
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
  dVar24 = (double)(*(code *)PTR_FUN_8c615be4)();
  puVar17 = *(ushort **)(param_5 + 4);
  psVar20 = *(short **)(param_5 + 0x14);
  *(uint *)((int)pdVar23 + -0x38) = (uint)*(ushort *)(param_5 + 2);
  *(undefined4 *)((int)pdVar23 + -0x48) = *DAT_8c615be8;
  *(undefined4 *)((int)pdVar23 + -0x44) = 0;
  do {
    uVar35 = SUB84(in_dr14,0);
    uVar2 = *puVar17;
    puVar18 = puVar17 + 1;
    *(uint *)((int)pdVar23 + -0x4c) = (uint)uVar2;
    uVar33 = (undefined4)((ulonglong)dVar34 >> 0x20);
    dVar27 = dVar34;
    if (in_FPSCR_SZ == '\0') {
      dVar27 = in_xd14;
      uVar35 = uVar33;
    }
    in_xd14 = dVar27;
    if ((uVar2 & DAT_8c615d0c) != 0) {
      if (in_FPSCR_PR == '\0') {
        param_1 = (double)CONCAT44(0xbf800000,SUB84(param_1,0));
      }
      else {
        param_1 = -(double)CONCAT44(0x3f800000,SUB84(param_1,0));
      }
      *(uint *)((int)pdVar23 + -0x4c) = *(uint *)((int)pdVar23 + -0x4c) & (int)DAT_8c615d08;
      in_xd14 = param_1;
      if (in_FPSCR_SZ == '\0') {
        uVar35 = (undefined4)((ulonglong)param_1 >> 0x20);
        in_xd14 = dVar27;
      }
    }
    *(int *)((int)pdVar23 + -0x44) = *(int *)((int)pdVar23 + -0x44) + *(int *)((int)pdVar23 + -0x4c)
    ;
    *(int *)((int)pdVar23 + -0x4c) = *(int *)((int)pdVar23 + -0x4c) + -2;
    puVar19 = puVar17 + 2;
    puVar17 = puVar17 + 3;
    pdVar15 = (double *)((uint)*puVar19 * 0x20 + *(int *)((int)pdVar23 + -0x48));
    pdVar14 = (double *)((uint)*puVar18 * 0x20 + *(int *)((int)pdVar23 + -0x48));
    pdVar12 = unaff_r8;
    if (in_FPSCR_SZ == '\0') {
      dVar34 = (double)CONCAT44(uVar33,*(undefined4 *)(pdVar15 + 2));
      param_1 = (double)(ulonglong)*(uint *)(pdVar14 + 2);
      in_dr14 = (double)CONCAT44(*(undefined4 *)((int)pdVar15 + 0x14),uVar35);
      param_2 = (double)CONCAT44(*(undefined4 *)((int)pdVar14 + 0x14),SUB84(param_2,0));
      dVar27 = in_xd4;
    }
    else {
      in_xd12 = pdVar15[2];
      in_dr14 = *(double *)((int)pdVar15 + 0x14);
      param_2 = *(double *)((int)pdVar14 + 0x14);
      dVar27 = pdVar14[2];
    }
    do {
      uVar2 = *puVar17;
      puVar17 = puVar17 + 1;
      pdVar16 = (double *)((uint)uVar2 * 0x20 + *(int *)((int)pdVar23 + -0x48));
      *(undefined4 *)((int)pdVar23 + -0x3c) = 0;
      uVar35 = (undefined4)((ulonglong)dVar34 >> 0x20);
      dVar26 = dVar34;
      if (in_FPSCR_SZ == '\0') {
        dVar26 = (double)CONCAT44(uVar35,SUB84(param_1,0));
      }
      param_1 = dVar26;
      if ((*(uint *)((int)pdVar14 + 0xc) & *(uint *)((int)pdVar15 + 0xc) &
           *(uint *)((int)pdVar16 + 0xc) & 0x3f) == 0) {
        if (((*(uint *)((int)pdVar14 + 0xc) | *(uint *)((int)pdVar15 + 0xc) |
             *(uint *)((int)pdVar16 + 0xc)) & 1) == 0) {
          if (in_FPSCR_SZ == '\0') {
            dVar26 = (double)CONCAT44(*(undefined4 *)((int)pdVar16 + 0x14),SUB84(dVar34,0));
          }
          else {
            dVar26 = *(double *)((int)pdVar16 + 0x14);
            in_xd2 = in_xd12;
          }
          fVar25 = (float)((ulonglong)in_dr14 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            dVar26 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) - fVar25,
                                      SUB84(dVar26,0) - SUB84(param_1,0));
          }
          else {
            dVar26 = (dVar26 - param_1) - in_dr14;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),*(undefined4 *)(pdVar16 + 2))
            ;
          }
          else {
            in_xd0 = pdVar16[2];
          }
          if (in_FPSCR_PR == '\0') {
            dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                      SUB84(dVar24,0) - SUB84(dVar34,0));
            fVar29 = SUB84(dVar26,0) * (float)((ulonglong)dVar26 >> 0x20);
          }
          else {
            dVar24 = dVar24 - dVar34;
            fVar29 = SUB84(dVar26 * dVar26,0);
          }
          dVar26 = in_dr14;
          if (in_FPSCR_SZ == '\0') {
            dVar26 = (double)CONCAT44(fVar25,fVar29);
          }
          if (in_FPSCR_PR == '\0') {
            fVar25 = ((float)((ulonglong)dVar26 >> 0x20) - (float)((ulonglong)param_2 >> 0x20)) *
                     SUB84(dVar24,0);
            in_dr2 = (double)CONCAT44(fVar25,SUB84(dVar26,0) - fVar25);
          }
          else {
            in_dr2 = (dVar26 - param_2) * dVar24;
            in_dr2 = in_dr2 - in_dr2;
          }
          dVar26 = in_xd2;
          if (in_FPSCR_SZ == '\0') {
            dVar26 = (double)CONCAT44(SUB84(in_dr2,0),SUB84(param_1,0));
          }
          if (in_FPSCR_PR == '\0') {
            param_1 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) * SUB84(in_dr14,0),
                                       SUB84(dVar26,0));
          }
          else {
            param_1 = dVar26 * in_dr14;
          }
        }
        else {
          puVar11 = (undefined4 *)((int)pdVar23 + -0x10);
          *puVar11 = pdVar14;
          *(double **)((int)pdVar23 + -0xc) = pdVar15;
          *(double **)((int)pdVar23 + -8) = pdVar16;
          iVar10 = 0;
          while( true ) {
            iVar5 = DAT_8c615d10;
            pdVar13 = (double *)((int)pdVar23 + -0x34);
            uVar33 = (undefined4)((ulonglong)in_dr2 >> 0x20);
            if (2 < iVar10) break;
            piVar8 = puVar11 + iVar10;
            *(int **)((int)pdVar23 + -0x50) = piVar8;
            *(int *)((int)pdVar23 + -0x54) = iVar10 * 0xc + (int)pdVar13;
            iVar10 = iVar10 + 1;
            if ((*(uint *)(*piVar8 + 0xc) & 1) == 0) {
              dVar26 = dVar34;
              if (in_FPSCR_SZ == '\0') {
                dVar26 = (double)CONCAT44(uVar35,SUB84(param_1,0));
              }
              iVar9 = **(int **)((int)pdVar23 + -0x50);
              *(int *)((int)pdVar23 + -0x40) = iVar9;
              if (in_FPSCR_SZ == '\0') {
                in_dr2 = (double)CONCAT44(uVar33,*(undefined4 *)(iVar9 + 0x18));
              }
              else {
                in_xd2 = *(double *)(iVar9 + 0x18);
              }
              if (in_FPSCR_PR == '\0') {
                param_1 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) / SUB84(in_dr2,0),
                                           SUB84(dVar26,0));
              }
              else {
                param_1 = dVar26 / in_dr2;
              }
              if (in_FPSCR_SZ == '\0') {
                in_dr2 = (double)(ulonglong)*(uint *)(DAT_8c615d10 + 0xc);
              }
              else {
                in_xd2 = *(double *)(DAT_8c615d10 + 0xc);
              }
              if (in_FPSCR_SZ == '\0') {
                dVar26 = (double)CONCAT44(*(undefined4 *)(*(int *)((int)pdVar23 + -0x40) + 0x10),
                                          SUB84(in_dr2,0));
              }
              else {
                dVar26 = *(double *)(*(int *)((int)pdVar23 + -0x40) + 0x10);
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
                                          *(undefined4 *)DAT_8c615d14);
              }
              else {
                in_xd2 = *DAT_8c615d14;
              }
              fVar25 = (float)((ulonglong)param_1 >> 0x20);
              if (in_FPSCR_PR == '\0') {
                dVar26 = (double)CONCAT44(((float)((ulonglong)dVar26 >> 0x20) * fVar25) /
                                          SUB84(dVar26,0),SUB84(dVar26,0));
              }
              else {
                dVar26 = (dVar26 * param_1) / (dVar26 * param_1);
              }
              uVar33 = (undefined4)((ulonglong)dVar26 >> 0x20);
              if (in_FPSCR_SZ == '\0') {
                *(undefined4 *)*(double **)((int)pdVar23 + -0x54) = uVar33;
              }
              else {
                **(double **)((int)pdVar23 + -0x54) = dVar26;
              }
              if (in_FPSCR_SZ == '\0') {
                dVar26 = (double)CONCAT44(uVar33,*(undefined4 *)(iVar5 + 0x10));
              }
              else {
                in_xd2 = *(double *)(iVar5 + 0x10);
              }
              if (in_FPSCR_SZ == '\0') {
                dVar26 = (double)CONCAT44(*(undefined4 *)(**(int **)((int)pdVar23 + -0x50) + 0x14),
                                          SUB84(dVar26,0));
              }
              else {
                dVar26 = *(double *)(**(int **)((int)pdVar23 + -0x50) + 0x14);
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
                                          *(undefined4 *)DAT_8c615d18);
              }
              else {
                in_xd2 = *DAT_8c615d18;
              }
              if (in_FPSCR_PR == '\0') {
                in_dr2 = (double)CONCAT44(((float)((ulonglong)dVar26 >> 0x20) * fVar25) /
                                          SUB84(dVar26,0),SUB84(dVar26,0));
              }
              else {
                in_dr2 = (dVar26 * param_1) / (dVar26 * param_1);
              }
              if (in_FPSCR_SZ == '\0') {
                *(int *)(*(int *)((int)pdVar23 + -0x54) + 4) = (int)((ulonglong)in_dr2 >> 0x20);
              }
              else {
                *(double *)(*(int *)((int)pdVar23 + -0x54) + 4) = in_dr2;
              }
              if (in_FPSCR_SZ == '\0') {
                *(float *)(*(int *)((int)pdVar23 + -0x54) + 8) = fVar25;
              }
              else {
                *(double *)(*(int *)((int)pdVar23 + -0x54) + 8) = param_1;
              }
            }
            else {
              if (in_FPSCR_SZ == '\0') {
                uVar31 = *(undefined4 *)(**(int **)((int)pdVar23 + -0x50) + 0x10);
                in_dr2 = (double)CONCAT44(uVar33,uVar31);
                *(undefined4 *)*(double **)((int)pdVar23 + -0x54) = uVar31;
              }
              else {
                in_xd2 = *(double *)(**(int **)((int)pdVar23 + -0x50) + 0x10);
                **(double **)((int)pdVar23 + -0x54) = in_xd2;
              }
              if (in_FPSCR_SZ == '\0') {
                uVar33 = *(undefined4 *)(**(int **)((int)pdVar23 + -0x50) + 0x14);
                in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),uVar33);
                *(undefined4 *)(*(int *)((int)pdVar23 + -0x54) + 4) = uVar33;
              }
              else {
                in_xd2 = *(double *)(**(int **)((int)pdVar23 + -0x50) + 0x14);
                *(double *)(*(int *)((int)pdVar23 + -0x54) + 4) = in_xd2;
              }
              if (in_FPSCR_SZ == '\0') {
                uVar33 = *(undefined4 *)(**(int **)((int)pdVar23 + -0x50) + 0x18);
                in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),uVar33);
                *(undefined4 *)(*(int *)((int)pdVar23 + -0x54) + 8) = uVar33;
              }
              else {
                in_xd2 = *(double *)(**(int **)((int)pdVar23 + -0x50) + 0x18);
                *(double *)(*(int *)((int)pdVar23 + -0x54) + 8) = in_xd2;
              }
            }
          }
          if (in_FPSCR_SZ == '\0') {
            uVar31 = *(undefined4 *)((int)pdVar23 + -0x2c);
            dVar24 = (double)CONCAT44(uVar31,SUB84(dVar24,0));
          }
          else {
            dVar24 = *(double *)((int)pdVar23 + -0x2c);
            uVar31 = SUB84(param_3,0);
            in_xd8 = dVar24;
          }
          pdVar4 = (double *)((int)pdVar23 + -0x28);
          *(double **)((int)pdVar23 + -0x54) = pdVar4;
          if (in_FPSCR_SZ == '\0') {
            uVar30 = *(undefined4 *)((int)pdVar23 + -0x30);
            dVar26 = (double)CONCAT44(*(undefined4 *)((int)pdVar23 + -0x24),SUB84(param_1,0));
            in_dr2 = (double)CONCAT44(uVar33,uVar30);
            param_2 = (double)CONCAT44(uVar30,*(undefined4 *)((int)pdVar23 + -0x20));
          }
          else {
            in_xd2 = *(double *)((int)pdVar23 + -0x30);
            dVar26 = *(double *)((int)pdVar23 + -0x24);
            uVar30 = (undefined4)((ulonglong)in_xd2 >> 0x20);
            param_2 = in_xd2;
          }
          fVar25 = (float)((ulonglong)dVar26 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * fVar25,SUB84(dVar24,0));
            fVar29 = SUB84(in_dr2,0) * SUB84(param_2,0);
          }
          else {
            dVar24 = dVar24 * dVar26;
            fVar29 = SUB84(in_dr2 * param_2,0);
          }
          if (in_FPSCR_SZ == '\0') {
            uVar33 = *(undefined4 *)pdVar13;
            dVar26 = (double)CONCAT44(fVar25,*(undefined4 *)pdVar4);
            dVar32 = (double)CONCAT44(uVar33,fVar29);
            param_3 = (double)CONCAT44(uVar33,uVar31);
          }
          else {
            dVar32 = *pdVar13;
            dVar27 = *pdVar4;
            uVar33 = (undefined4)((ulonglong)dVar32 >> 0x20);
            param_3 = dVar32;
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) * SUB84(param_2,0),
                                      SUB84(dVar32,0));
          }
          else {
            dVar32 = dVar32 * param_2;
          }
          dVar37 = param_2;
          if (in_FPSCR_SZ == '\0') {
            dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),uVar30);
            dVar37 = in_xd0;
          }
          if (in_FPSCR_PR == '\0') {
            dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) - SUB84(dVar32,0),
                                      SUB84(dVar24,0));
          }
          else {
            dVar24 = dVar24 - dVar32;
          }
          dVar38 = in_xd8;
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),SUB84(param_3,0));
            dVar38 = in_xd2;
          }
          fVar25 = SUB84(dVar26,0);
          if (in_FPSCR_PR == '\0') {
            fVar29 = SUB84(dVar32,0) * fVar25;
            dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) - fVar29,fVar29);
          }
          else {
            dVar32 = dVar32 * dVar26 - dVar32 * dVar26;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                      *(undefined4 *)((int)pdVar23 + -0x18));
          }
          else {
            dVar38 = *(double *)((int)pdVar23 + -0x18);
          }
          if (in_FPSCR_PR == '\0') {
            fVar29 = SUB84(dVar32,0) * (float)((ulonglong)dVar32 >> 0x20);
          }
          else {
            fVar29 = SUB84(dVar32 * dVar32,0);
          }
          if (in_FPSCR_SZ == '\0') {
            uVar36 = CONCAT44(*(undefined4 *)((int)pdVar23 + -0x1c),fVar29);
          }
          else {
            uVar36 = *(undefined8 *)((int)pdVar23 + -0x1c);
          }
          if (in_FPSCR_PR == '\0') {
            fVar29 = (float)((ulonglong)uVar36 >> 0x20);
            uVar36 = CONCAT44(fVar29,(float)((ulonglong)dVar24 >> 0x20) * fVar29 + (float)uVar36);
          }
          dVar32 = dVar38;
          dVar39 = param_3;
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44((int)uVar36,uVar33);
            dVar39 = dVar38;
          }
          if (in_FPSCR_PR == '\0') {
            fVar29 = SUB84(dVar32,0) * (float)((ulonglong)dVar26 >> 0x20);
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),fVar29);
            fVar29 = SUB84(dVar24,0) * fVar25 - fVar29;
          }
          else {
            dVar32 = dVar32 * dVar26;
            fVar29 = SUB84(dVar24 * dVar26 - dVar32,0);
          }
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                      *(undefined4 *)((int)pdVar23 + -0x14));
            dVar24 = (double)CONCAT44(fVar29,fVar29);
          }
          else {
            dVar39 = *(double *)((int)pdVar23 + -0x14);
            dVar24 = dVar37;
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * SUB84(dVar32,0) +
                                      (float)((ulonglong)dVar32 >> 0x20),SUB84(dVar32,0));
          }
          in_xd2 = in_xd14;
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),SUB84(in_dr14,0));
            in_xd2 = dVar39;
          }
          uVar33 = (undefined4)((ulonglong)dVar32 >> 0x20);
          fVar29 = -SUB84(dVar32,0);
          dVar32 = (double)CONCAT44(uVar33,fVar29);
          dVar26 = dVar32;
          if (in_FPSCR_SZ == '\0') {
            dVar26 = (double)CONCAT44(uVar33,fVar25);
          }
          if (in_FPSCR_PR == '\0') {
            dVar26 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) * fVar29,SUB84(dVar26,0));
          }
          else {
            dVar26 = dVar26 * dVar32;
          }
          in_dr2 = (double)(ulonglong)(uint)fVar29;
          if (in_FPSCR_PR == '\0') {
            bVar40 = 0.0 < (float)((ulonglong)dVar26 >> 0x20);
          }
          else {
            bVar40 = in_dr2 < dVar26;
          }
          param_1 = dVar26;
          in_xd0 = dVar37;
          if ((!bVar40) &&
             (*(undefined4 *)((int)pdVar23 + -0x3c) = 1, param_1 = dVar34, in_FPSCR_SZ == '\0')) {
            param_1 = (double)CONCAT44(uVar35,SUB84(dVar26,0));
          }
        }
      }
      uVar31 = DAT_8c616044;
      uVar33 = DAT_8c616040;
      uVar28 = (uint)((ulonglong)in_dr2 >> 0x20);
      fVar25 = (float)((ulonglong)param_1 >> 0x20);
      uVar35 = SUB84(param_1,0);
      if (pdVar12 < unaff_r8) {
        if (in_FPSCR_PR == '\0') {
          bVar40 = fVar25 < 0.0;
        }
        else {
          bVar40 = param_1 < (double)((ulonglong)uVar28 << 0x20);
        }
        psVar21 = psVar20;
        if (bVar40) {
LAB_8c615e76:
          uVar33 = DAT_8c616044;
          uVar35 = SUB84(param_1,0);
          psVar22 = psVar21 + 2;
          if (in_FPSCR_SZ == '\0') {
            dVar24 = (double)CONCAT44(*(undefined4 *)pdVar15,SUB84(dVar24,0));
            dVar26 = (double)((ulonglong)*(uint *)((int)pdVar15 + 4) << 0x20);
            pdVar13 = pdVar15 + 1;
          }
          else {
            dVar24 = *pdVar15;
            dVar26 = pdVar15[1];
            pdVar13 = pdVar15 + 2;
          }
          dVar26 = (double)((ulonglong)dVar26 & 0xffffffff00000000);
          dVar32 = in_xd2;
          dVar37 = in_xd0;
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)((ulonglong)param_4 & 0xffffffff);
            dVar37 = (double)CONCAT44(SUB84(dVar24,0),SUB84(param_3,0));
          }
                    /* WARNING: Ignoring partial resolution of indirect */
          fVar25 = (float)((ulonglong)dVar24 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            fVar29 = SUB84(dVar24,0) * SUB84(dVar37,0) + fVar25 * (float)((ulonglong)dVar37 >> 0x20)
                     + SUB84(dVar32,0) * 0.0 +
                     (float)((ulonglong)dVar26 >> 0x20) * (float)((ulonglong)dVar32 >> 0x20);
          }
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44(*(undefined4 *)pdVar13,uVar35);
            param_2 = (double)CONCAT44(*(undefined4 *)((int)pdVar13 + 4),SUB84(param_2,0));
            param_3 = (double)CONCAT44(uVar35,SUB84(dVar37,0));
          }
          else {
            param_1 = *pdVar13;
            param_2 = pdVar13[1];
            uVar35 = SUB84(param_1,0);
            param_3 = dVar27;
          }
          sVar3 = *psVar21;
          sVar1 = psVar21[1];
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)unaff_r8 + 0xc) = (int)((ulonglong)param_2 >> 0x20);
            unaff_r8 = unaff_r8 + 1;
            *(undefined4 *)unaff_r8 = uVar35;
          }
          else {
            unaff_r8[1] = param_2;
            *unaff_r8 = dVar27;
          }
          if (in_FPSCR_PR == '\0') {
            param_4 = (double)(ulonglong)(uint)(SUB84(dVar32,0) - SUB84(dVar26,0));
          }
          else {
            param_4 = dVar32 - dVar26;
          }
          if (in_FPSCR_SZ == '\0') {
            pdVar13 = (double *)((int)unaff_r8 + -4);
            *(int *)pdVar13 = (int)((ulonglong)param_1 >> 0x20);
          }
          else {
            pdVar13 = unaff_r8 + -1;
            *pdVar13 = param_1;
          }
          fVar29 = (float)((ulonglong)param_3 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            dVar24 = (double)CONCAT44(fVar25,(float)(int)sVar1 * fVar29);
          }
          else {
            dVar24 = (double)CONCAT44(fVar25,(float)(int)sVar1) * param_3;
          }
          *(undefined4 *)((int)pdVar13 + -4) = uVar33;
          if (in_FPSCR_PR == '\0') {
            in_dr2 = (double)CONCAT44((float)(int)sVar3 * fVar29,SUB84(dVar26,0));
          }
          else {
            in_dr2 = (double)(int)sVar3 * param_3;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)pdVar13 + 0x14) = SUB84(param_4,0);
            *(int *)(pdVar13 + 2) = SUB84(dVar24,0);
            pdVar13 = (double *)((int)pdVar13 + 0xc);
            *(int *)pdVar13 = (int)((ulonglong)in_dr2 >> 0x20);
          }
          else {
            pdVar13[2] = in_xd10;
            pdVar13[1] = in_xd0;
            *pdVar13 = in_dr2;
          }
          unaff_r8 = pdVar13 + 2;
        }
        else {
          if (in_FPSCR_SZ == '\0') {
            dVar24 = (double)CONCAT44(*(undefined4 *)pdVar15,SUB84(dVar24,0));
            dVar26 = (double)((ulonglong)*(uint *)((int)pdVar15 + 4) << 0x20);
            pdVar12 = pdVar15 + 1;
          }
          else {
            dVar24 = *pdVar15;
            dVar26 = pdVar15[1];
            pdVar12 = pdVar15 + 2;
          }
          dVar26 = (double)((ulonglong)dVar26 & 0xffffffff00000000);
          dVar32 = in_xd2;
          dVar37 = in_xd0;
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)((ulonglong)param_4 & 0xffffffff);
            dVar37 = (double)CONCAT44(SUB84(dVar24,0),SUB84(param_3,0));
          }
                    /* WARNING: Ignoring partial resolution of indirect */
          fVar25 = (float)((ulonglong)dVar24 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            fVar29 = SUB84(dVar24,0) * SUB84(dVar37,0) + fVar25 * (float)((ulonglong)dVar37 >> 0x20)
                     + SUB84(dVar32,0) * 0.0 +
                     (float)((ulonglong)dVar26 >> 0x20) * (float)((ulonglong)dVar32 >> 0x20);
          }
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44(*(undefined4 *)pdVar12,uVar35);
            param_2 = (double)CONCAT44(*(undefined4 *)((int)pdVar12 + 4),SUB84(param_2,0));
            param_3 = (double)CONCAT44(uVar35,SUB84(dVar37,0));
          }
          else {
            param_1 = *pdVar12;
            param_2 = pdVar12[1];
            uVar35 = SUB84(param_1,0);
            param_3 = dVar27;
          }
          sVar3 = *psVar20;
          sVar1 = psVar20[1];
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)unaff_r8 + 0xc) = (int)((ulonglong)param_2 >> 0x20);
            unaff_r8 = unaff_r8 + 1;
            *(undefined4 *)unaff_r8 = uVar35;
          }
          else {
            unaff_r8[1] = param_2;
            *unaff_r8 = dVar27;
          }
          if (in_FPSCR_PR == '\0') {
            param_4 = (double)(ulonglong)(uint)(SUB84(dVar32,0) - SUB84(dVar26,0));
          }
          else {
            param_4 = dVar32 - dVar26;
          }
          if (in_FPSCR_SZ == '\0') {
            pdVar12 = (double *)((int)unaff_r8 + -4);
            *(int *)pdVar12 = (int)((ulonglong)param_1 >> 0x20);
          }
          else {
            pdVar12 = unaff_r8 + -1;
            *pdVar12 = param_1;
          }
          fVar29 = (float)((ulonglong)param_3 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            dVar24 = (double)CONCAT44(fVar25,(float)(int)sVar1 * fVar29);
          }
          else {
            dVar24 = (double)CONCAT44(fVar25,(float)(int)sVar1) * param_3;
          }
          *(undefined4 *)((int)pdVar12 + -4) = uVar33;
          if (in_FPSCR_PR == '\0') {
            in_dr2 = (double)CONCAT44((float)(int)sVar3 * fVar29,SUB84(dVar26,0));
          }
          else {
            in_dr2 = (double)(int)sVar3 * param_3;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)pdVar12 + 0x14) = SUB84(param_4,0);
            *(int *)(pdVar12 + 2) = SUB84(dVar24,0);
            pdVar12 = (double *)((int)pdVar12 + 0xc);
            *(int *)pdVar12 = (int)((ulonglong)in_dr2 >> 0x20);
          }
          else {
            pdVar12[2] = in_xd10;
            pdVar12[1] = in_xd0;
            *pdVar12 = in_dr2;
          }
          unaff_r8 = pdVar12 + 2;
          pdVar12 = unaff_r8;
          psVar22 = psVar20;
        }
      }
      else {
        in_dr2 = (double)((ulonglong)uVar28 << 0x20);
        if (in_FPSCR_PR == '\0') {
          bVar40 = fVar25 < 0.0;
        }
        else {
          bVar40 = param_1 < in_dr2;
        }
        psVar22 = psVar20 + 2;
        if (bVar40) {
          psVar21 = psVar20 + 2;
          if (in_FPSCR_SZ == '\0') {
            dVar24 = (double)CONCAT44(*(undefined4 *)pdVar14,SUB84(dVar24,0));
            dVar26 = (double)((ulonglong)*(uint *)((int)pdVar14 + 4) << 0x20);
            pdVar13 = pdVar14 + 1;
          }
          else {
            dVar24 = *pdVar14;
            dVar26 = pdVar14[1];
            pdVar13 = pdVar14 + 2;
          }
          dVar26 = (double)((ulonglong)dVar26 & 0xffffffff00000000);
          dVar32 = in_xd2;
          dVar37 = in_xd0;
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)((ulonglong)param_4 & 0xffffffff);
            dVar37 = (double)CONCAT44(SUB84(dVar24,0),SUB84(param_3,0));
          }
                    /* WARNING: Ignoring partial resolution of indirect */
          fVar25 = (float)((ulonglong)dVar24 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            fVar29 = SUB84(dVar24,0) * SUB84(dVar37,0) + fVar25 * (float)((ulonglong)dVar37 >> 0x20)
                     + SUB84(dVar32,0) * 0.0 +
                     (float)((ulonglong)dVar26 >> 0x20) * (float)((ulonglong)dVar32 >> 0x20);
          }
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44(*(undefined4 *)pdVar13,uVar35);
            param_2 = (double)CONCAT44(*(undefined4 *)((int)pdVar13 + 4),SUB84(param_2,0));
            param_3 = (double)CONCAT44(uVar35,SUB84(dVar37,0));
          }
          else {
            param_1 = *pdVar13;
            param_2 = pdVar13[1];
            uVar35 = SUB84(param_1,0);
            param_3 = dVar27;
          }
          sVar3 = *psVar20;
          sVar1 = psVar20[1];
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)unaff_r8 + 0xc) = (int)((ulonglong)param_2 >> 0x20);
            unaff_r8 = unaff_r8 + 1;
            *(undefined4 *)unaff_r8 = uVar35;
          }
          else {
            unaff_r8[1] = param_2;
            *unaff_r8 = dVar27;
          }
          if (in_FPSCR_PR == '\0') {
            param_4 = (double)(ulonglong)(uint)(SUB84(dVar32,0) - SUB84(dVar26,0));
          }
          else {
            param_4 = dVar32 - dVar26;
          }
          if (in_FPSCR_SZ == '\0') {
            pdVar13 = (double *)((int)unaff_r8 + -4);
            *(int *)pdVar13 = (int)((ulonglong)param_1 >> 0x20);
          }
          else {
            pdVar13 = unaff_r8 + -1;
            *pdVar13 = param_1;
          }
          fVar29 = (float)((ulonglong)param_3 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            dVar24 = (double)(ulonglong)(uint)((float)(int)sVar1 * fVar29);
          }
          else {
            dVar24 = (double)CONCAT44(fVar25,(float)(int)sVar1) * param_3;
          }
          *(undefined4 *)((int)pdVar13 + -4) = uVar31;
          if (in_FPSCR_PR == '\0') {
            dVar26 = (double)CONCAT44((float)(int)sVar3 * fVar29,SUB84(dVar26,0));
          }
          else {
            dVar26 = (double)(int)sVar3 * param_3;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)pdVar13 + 0x14) = SUB84(param_4,0);
            *(int *)(pdVar13 + 2) = SUB84(dVar24,0);
            pdVar13 = (double *)((int)pdVar13 + 0xc);
            *(int *)pdVar13 = (int)((ulonglong)dVar26 >> 0x20);
          }
          else {
            pdVar13[2] = in_xd10;
            pdVar13[1] = in_xd0;
            *pdVar13 = dVar26;
          }
          unaff_r8 = pdVar13 + 2;
          goto LAB_8c615e76;
        }
      }
      puVar6 = PTR_FUN_8c61604c;
      uVar33 = SUB84(param_2,0);
      uVar35 = SUB84(param_1,0);
      if (*(int *)((int)pdVar23 + -0x3c) != 0) {
        *DAT_8c616048 = 0;
        if (in_FPSCR_SZ == '\0') {
          dVar24 = (double)CONCAT44(*(undefined4 *)pdVar14,*(undefined4 *)pdVar15);
        }
        else {
          in_xd2 = *pdVar15;
          dVar24 = *pdVar14;
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) + SUB84(dVar24,0),
                                    SUB84(dVar24,0));
        }
        else {
          dVar24 = dVar24 + dVar24;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),*(undefined4 *)pdVar16);
        }
        else {
          in_xd2 = *pdVar16;
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) + SUB84(dVar24,0),
                                    SUB84(dVar24,0));
        }
        else {
          dVar24 = dVar24 + dVar24;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)pdVar23 + -0x10) = (int)((ulonglong)dVar24 >> 0x20);
          dVar24 = (double)CONCAT44(*(undefined4 *)((int)pdVar14 + 4),
                                    *(undefined4 *)((int)pdVar15 + 4));
        }
        else {
          *(double *)((int)pdVar23 + -0x10) = dVar24;
          dVar24 = *(double *)((int)pdVar14 + 4);
          in_xd2 = *(double *)((int)pdVar15 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) + SUB84(dVar24,0),
                                    SUB84(dVar24,0));
        }
        else {
          dVar24 = dVar24 + dVar24;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                    *(undefined4 *)((int)pdVar16 + 4));
        }
        else {
          in_xd2 = *(double *)((int)pdVar16 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) + SUB84(dVar24,0),
                                    SUB84(dVar24,0));
        }
        else {
          dVar24 = dVar24 + dVar24;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)pdVar23 + -0xc) = (int)((ulonglong)dVar24 >> 0x20);
          dVar24 = (double)CONCAT44(*(undefined4 *)(pdVar14 + 1),*(undefined4 *)(pdVar15 + 1));
        }
        else {
          *(double *)((int)pdVar23 + -0xc) = dVar24;
          in_xd2 = pdVar15[1];
          dVar24 = pdVar14[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) + SUB84(dVar24,0),
                                    SUB84(dVar24,0));
        }
        else {
          dVar24 = dVar24 + dVar24;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),*(undefined4 *)(pdVar16 + 1));
        }
        else {
          in_xd2 = pdVar16[1];
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) + SUB84(dVar24,0),
                                    SUB84(dVar24,0));
        }
        else {
          dVar24 = dVar24 + dVar24;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)pdVar23 + -8) = (int)((ulonglong)dVar24 >> 0x20);
        }
        else {
          *(double *)((int)pdVar23 + -8) = dVar24;
        }
        dVar24 = (double)(*(code *)puVar6)((undefined1 *)((int)pdVar23 + -0x10));
        puVar7 = DAT_8c616058;
        if (in_FPSCR_SZ == '\0') {
          dVar26 = (double)CONCAT44(*(undefined4 *)DAT_8c616054,
                                    *(undefined4 *)((int)pdVar23 + -0x10));
        }
        else {
          in_xd2 = *(double *)((int)pdVar23 + -0x10);
          dVar26 = *DAT_8c616054;
        }
        if (in_FPSCR_PR == '\0') {
          dVar26 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) * SUB84(dVar26,0),
                                    SUB84(dVar26,0));
        }
        else {
          dVar26 = dVar26 * dVar26;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),*(undefined4 *)DAT_8c616050);
        }
        else {
          in_xd2 = *DAT_8c616050;
        }
        if (in_FPSCR_PR == '\0') {
          fVar25 = SUB84(dVar26,0) - (float)((ulonglong)dVar26 >> 0x20);
        }
        else {
          fVar25 = SUB84(dVar26 - dVar26,0);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar26 = (double)CONCAT44(*(undefined4 *)((int)pdVar23 + -0xc),fVar25);
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                    *(undefined4 *)((int)DAT_8c616054 + 4));
        }
        else {
          dVar26 = *(double *)((int)pdVar23 + -0xc);
          in_xd0 = *(double *)((int)DAT_8c616054 + 4);
        }
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                    SUB84(dVar24,0) * (float)((ulonglong)dVar26 >> 0x20));
        }
        else {
          dVar24 = dVar24 * dVar26;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar26 = (double)CONCAT44(*(undefined4 *)((int)pdVar23 + -8),SUB84(dVar26,0));
        }
        else {
          dVar26 = *(double *)((int)pdVar23 + -8);
        }
        if (in_FPSCR_PR == '\0') {
          dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),
                                    SUB84(dVar26,0) - SUB84(dVar24,0));
        }
        else {
          dVar26 = dVar26 - dVar24;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                    *(undefined4 *)(DAT_8c616054 + 1));
        }
        else {
          in_xd0 = DAT_8c616054[1];
        }
        fVar25 = (float)((ulonglong)dVar26 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),SUB84(dVar24,0) * fVar25);
        }
        else {
          dVar24 = dVar24 * dVar26;
        }
        param_1 = in_xd2;
        if (in_FPSCR_SZ == '\0') {
          param_1 = (double)CONCAT44(SUB84(dVar26,0),uVar35);
        }
        if (in_FPSCR_PR == '\0') {
          param_1 = (double)((ulonglong)param_1 & 0xffffffff);
          uVar41 = (undefined2)(int)(float)(int)psVar22[-2];
        }
        else {
          param_1 = param_1 - dVar24;
          uVar41 = (undefined2)(int)(double)CONCAT44(fVar25,(float)(int)psVar22[-2]);
        }
        *DAT_8c616058 = uVar41;
        if (in_FPSCR_PR == '\0') {
          uVar41 = (undefined2)(int)(float)(int)psVar22[-1];
        }
        else {
          uVar41 = (undefined2)(int)(double)CONCAT44(fVar25,(float)(int)psVar22[-1]);
        }
        puVar7[1] = uVar41;
        if (in_FPSCR_PR == '\0') {
          uVar41 = (undefined2)(int)(float)(int)*psVar22;
        }
        else {
          uVar41 = (undefined2)(int)(double)CONCAT44(fVar25,(float)(int)*psVar22);
        }
        puVar7[2] = uVar41;
        if (in_FPSCR_PR == '\0') {
          uVar41 = (undefined2)(int)(float)(int)psVar22[1];
        }
        else {
          uVar41 = (undefined2)(int)(double)CONCAT44(fVar25,(float)(int)psVar22[1]);
        }
        puVar7[3] = uVar41;
        if (in_FPSCR_PR == '\0') {
          uVar41 = (undefined2)(int)(float)(int)psVar22[2];
        }
        else {
          uVar41 = (undefined2)(int)(double)CONCAT44(fVar25,(float)(int)psVar22[2]);
        }
        puVar7[4] = uVar41;
        puVar6 = PTR_FUN_8c61605c;
        in_dr2 = (double)CONCAT44(fVar25,(float)(int)psVar22[3]);
        if (in_FPSCR_PR == '\0') {
          uVar41 = (undefined2)(int)(float)(int)psVar22[3];
        }
        else {
          uVar41 = (undefined2)(int)in_dr2;
        }
        puVar7[5] = uVar41;
        dVar24 = (double)(*(code *)puVar6)(pdVar14,pdVar15,pdVar16,puVar7);
        pdVar12 = unaff_r8;
      }
      uVar35 = DAT_8c616040;
      in_xd4 = in_xd12;
      if (in_FPSCR_SZ == '\0') {
        param_1 = (double)((ulonglong)dVar34 & 0xffffffff);
        in_xd4 = dVar27;
      }
      uVar31 = SUB84(param_1,0);
      iVar10 = *(int *)((int)pdVar23 + -0x4c);
      if (in_FPSCR_SZ == '\0') {
        dVar34 = (double)CONCAT44((int)((ulonglong)dVar34 >> 0x20),*(undefined4 *)(pdVar16 + 2));
        uVar36 = CONCAT44(*(undefined4 *)((int)pdVar16 + 0x14),SUB84(in_dr14,0));
        param_2 = (double)CONCAT44((int)((ulonglong)in_dr14 >> 0x20),uVar33);
      }
      else {
        in_xd12 = pdVar16[2];
        uVar36 = *(undefined8 *)((int)pdVar16 + 0x14);
        param_2 = in_dr14;
      }
      in_dr14 = (double)CONCAT44((int)((ulonglong)uVar36 >> 0x20),-(float)uVar36);
      *(int *)((int)pdVar23 + -0x4c) = iVar10 + -1;
      pdVar14 = pdVar15;
      pdVar15 = pdVar16;
      psVar20 = psVar22;
      dVar27 = in_xd4;
    } while (iVar10 + -1 != 0);
    psVar20 = psVar22 + 4;
    if (pdVar12 < unaff_r8) {
      psVar20 = psVar22 + 2;
      if (in_FPSCR_SZ == '\0') {
        dVar24 = (double)CONCAT44(*(undefined4 *)pdVar16,SUB84(dVar24,0));
        dVar27 = (double)((ulonglong)*(uint *)((int)pdVar16 + 4) << 0x20);
        pdVar16 = pdVar16 + 1;
      }
      else {
        dVar24 = *pdVar16;
        dVar27 = pdVar16[1];
        pdVar16 = pdVar16 + 2;
      }
      dVar27 = (double)((ulonglong)dVar27 & 0xffffffff00000000);
      dVar26 = in_xd2;
      dVar32 = in_xd0;
      if (in_FPSCR_SZ == '\0') {
        dVar26 = (double)((ulonglong)param_4 & 0xffffffff);
        dVar32 = (double)CONCAT44(SUB84(dVar24,0),SUB84(param_3,0));
      }
                    /* WARNING: Ignoring partial resolution of indirect */
      fVar25 = (float)((ulonglong)dVar24 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar29 = SUB84(dVar24,0) * SUB84(dVar32,0) + fVar25 * (float)((ulonglong)dVar32 >> 0x20) +
                 SUB84(dVar26,0) * 0.0 +
                 (float)((ulonglong)dVar27 >> 0x20) * (float)((ulonglong)dVar26 >> 0x20);
      }
      if (in_FPSCR_SZ == '\0') {
        param_1 = (double)CONCAT44(*(undefined4 *)pdVar16,uVar31);
        param_2 = (double)CONCAT44(*(undefined4 *)((int)pdVar16 + 4),SUB84(param_2,0));
        param_3 = (double)CONCAT44(uVar31,SUB84(dVar32,0));
      }
      else {
        param_1 = *pdVar16;
        param_2 = pdVar16[1];
        uVar31 = SUB84(param_1,0);
        param_3 = in_xd4;
      }
      sVar3 = *psVar22;
      sVar1 = psVar22[1];
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)unaff_r8 + 0xc) = (int)((ulonglong)param_2 >> 0x20);
        unaff_r8 = unaff_r8 + 1;
        *(undefined4 *)unaff_r8 = uVar31;
      }
      else {
        unaff_r8[1] = param_2;
        *unaff_r8 = in_xd4;
      }
      if (in_FPSCR_PR == '\0') {
        param_4 = (double)(ulonglong)(uint)(SUB84(dVar26,0) - SUB84(dVar27,0));
      }
      else {
        param_4 = dVar26 - dVar27;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar12 = (double *)((int)unaff_r8 + -4);
        *(int *)pdVar12 = (int)((ulonglong)param_1 >> 0x20);
      }
      else {
        pdVar12 = unaff_r8 + -1;
        *pdVar12 = param_1;
      }
      fVar29 = (float)((ulonglong)param_3 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar24 = (double)CONCAT44(fVar25,(float)(int)sVar1 * fVar29);
      }
      else {
        dVar24 = (double)CONCAT44(fVar25,(float)(int)sVar1) * param_3;
      }
      *(undefined4 *)((int)pdVar12 + -4) = uVar35;
      if (in_FPSCR_PR == '\0') {
        in_dr2 = (double)CONCAT44((float)(int)sVar3 * fVar29,SUB84(dVar27,0));
      }
      else {
        in_dr2 = (double)(int)sVar3 * param_3;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar12 + 0x14) = SUB84(param_4,0);
        *(int *)(pdVar12 + 2) = SUB84(dVar24,0);
        pdVar12 = (double *)((int)pdVar12 + 0xc);
        *(int *)pdVar12 = (int)((ulonglong)in_dr2 >> 0x20);
      }
      else {
        pdVar12[2] = in_xd10;
        pdVar12[1] = in_xd0;
        *pdVar12 = in_dr2;
      }
      unaff_r8 = pdVar12 + 2;
    }
    iVar10 = *(int *)((int)pdVar23 + -0x38);
    *(int *)((int)pdVar23 + -0x38) = iVar10 + -1;
    if (iVar10 + -1 == 0) {
      *DAT_8c616060 = *DAT_8c616060 + *(int *)((int)pdVar23 + -0x44);
      *DAT_8c616064 = *DAT_8c616064 + *(int *)((int)pdVar23 + -0x44);
      return;
    }
  } while( true );
}

