/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c61523a) */
/* WARNING: Removing unreachable block (ram,0x8c61517c) */
/* WARNING: Removing unreachable block (ram,0x8c6152ae) */
/* WARNING: Removing unreachable block (ram,0x8c6151e4) */
/* WARNING: Removing unreachable block (ram,0x8c615280) */
/* WARNING: Removing unreachable block (ram,0x8c6152be) */
/* WARNING: Removing unreachable block (ram,0x8c6152cc) */
/* WARNING: Removing unreachable block (ram,0x8c6152da) */
/* WARNING: Removing unreachable block (ram,0x8c6152f2) */
/* WARNING: Removing unreachable block (ram,0x8c615302) */
/* WARNING: Removing unreachable block (ram,0x8c615328) */
/* WARNING: Removing unreachable block (ram,0x8c615368) */
/* WARNING: Restarted to delay deadcode elimination for space: register */

void FUN_8c614f70(double param_1,double param_2,double param_3,double param_4,int param_5)

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
  undefined8 *puVar14;
  undefined8 *puVar15;
  undefined8 *puVar16;
  ushort *puVar17;
  ushort *puVar18;
  ushort *puVar19;
  short *psVar20;
  short *psVar21;
  short *psVar22;
  double *pdVar23;
  double dVar24;
  undefined8 uVar25;
  float fVar26;
  double in_dr2;
  float fVar29;
  uint uVar30;
  ulonglong uVar27;
  float fVar31;
  double dVar28;
  undefined4 uVar32;
  undefined4 uVar33;
  undefined4 uVar34;
  undefined8 in_dr12;
  double dVar35;
  undefined4 uVar36;
  double in_dr14;
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
  double in_xd10;
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
  dVar35 = (double)CONCAT44(0x3f800000,(int)in_dr12);
  dVar24 = (double)(*(code *)PTR_FUN_8c614fc0)();
  puVar17 = *(ushort **)(param_5 + 4);
  psVar20 = *(short **)(param_5 + 0x14);
  *(uint *)((int)pdVar23 + -0x38) = (uint)*(ushort *)(param_5 + 2);
  *(undefined4 *)((int)pdVar23 + -0x48) = *DAT_8c614fc4;
  *(undefined4 *)((int)pdVar23 + -0x44) = 0;
  do {
    uVar36 = SUB84(in_dr14,0);
    uVar2 = *puVar17;
    puVar18 = puVar17 + 1;
    *(uint *)((int)pdVar23 + -0x4c) = (uint)uVar2;
    uVar32 = (undefined4)((ulonglong)dVar35 >> 0x20);
    dVar40 = dVar35;
    if (in_FPSCR_SZ == '\0') {
      dVar40 = in_xd14;
      uVar36 = uVar32;
    }
    in_xd14 = dVar40;
    if ((uVar2 & DAT_8c6150e8) != 0) {
      if (in_FPSCR_PR == '\0') {
        param_1 = (double)CONCAT44(0xbf800000,SUB84(param_1,0));
      }
      else {
        param_1 = -(double)CONCAT44(0x3f800000,SUB84(param_1,0));
      }
      *(uint *)((int)pdVar23 + -0x4c) = *(uint *)((int)pdVar23 + -0x4c) & (int)DAT_8c6150e4;
      in_xd14 = param_1;
      if (in_FPSCR_SZ == '\0') {
        uVar36 = (undefined4)((ulonglong)param_1 >> 0x20);
        in_xd14 = dVar40;
      }
    }
    *(int *)((int)pdVar23 + -0x44) = *(int *)((int)pdVar23 + -0x44) + *(int *)((int)pdVar23 + -0x4c)
    ;
    *(int *)((int)pdVar23 + -0x4c) = *(int *)((int)pdVar23 + -0x4c) + -2;
    puVar19 = puVar17 + 2;
    puVar17 = puVar17 + 3;
    puVar15 = (undefined8 *)((uint)*puVar19 * 0x20 + *(int *)((int)pdVar23 + -0x48));
    puVar14 = (undefined8 *)((uint)*puVar18 * 0x20 + *(int *)((int)pdVar23 + -0x48));
    pdVar12 = unaff_r8;
    if (in_FPSCR_SZ == '\0') {
      dVar35 = (double)CONCAT44(uVar32,*(undefined4 *)(puVar15 + 2));
      param_1 = (double)(ulonglong)*(uint *)(puVar14 + 2);
      in_dr14 = (double)CONCAT44(*(undefined4 *)((int)puVar15 + 0x14),uVar36);
      param_2 = (double)CONCAT44(*(undefined4 *)((int)puVar14 + 0x14),SUB84(param_2,0));
      dVar40 = in_xd4;
    }
    else {
      in_xd12 = (double)puVar15[2];
      in_dr14 = *(double *)((int)puVar15 + 0x14);
      param_2 = *(double *)((int)puVar14 + 0x14);
      dVar40 = (double)puVar14[2];
    }
    do {
      uVar2 = *puVar17;
      puVar17 = puVar17 + 1;
      puVar16 = (undefined8 *)((uint)uVar2 * 0x20 + *(int *)((int)pdVar23 + -0x48));
      *(undefined4 *)((int)pdVar23 + -0x3c) = 0;
      uVar36 = (undefined4)((ulonglong)dVar35 >> 0x20);
      dVar28 = dVar35;
      if (in_FPSCR_SZ == '\0') {
        dVar28 = (double)CONCAT44(uVar36,SUB84(param_1,0));
      }
      param_1 = dVar28;
      if ((*(uint *)((int)puVar14 + 0xc) & *(uint *)((int)puVar15 + 0xc) &
           *(uint *)((int)puVar16 + 0xc) & 0x3f) == 0) {
        if (((*(uint *)((int)puVar14 + 0xc) | *(uint *)((int)puVar15 + 0xc) |
             *(uint *)((int)puVar16 + 0xc)) & 1) == 0) {
          if (in_FPSCR_SZ == '\0') {
            dVar28 = (double)CONCAT44(*(undefined4 *)((int)puVar16 + 0x14),SUB84(dVar35,0));
          }
          else {
            dVar28 = *(double *)((int)puVar16 + 0x14);
            in_xd2 = in_xd12;
          }
          fVar29 = (float)((ulonglong)in_dr14 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) - fVar29,
                                      SUB84(dVar28,0) - SUB84(param_1,0));
          }
          else {
            dVar28 = (dVar28 - param_1) - in_dr14;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),*(undefined4 *)(puVar16 + 2))
            ;
          }
          else {
            in_xd0 = (double)puVar16[2];
          }
          if (in_FPSCR_PR == '\0') {
            dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),
                                      SUB84(dVar24,0) - SUB84(dVar35,0));
            fVar31 = SUB84(dVar28,0) * (float)((ulonglong)dVar28 >> 0x20);
          }
          else {
            dVar24 = dVar24 - dVar35;
            fVar31 = SUB84(dVar28 * dVar28,0);
          }
          dVar28 = in_dr14;
          if (in_FPSCR_SZ == '\0') {
            dVar28 = (double)CONCAT44(fVar29,fVar31);
          }
          if (in_FPSCR_PR == '\0') {
            fVar29 = ((float)((ulonglong)dVar28 >> 0x20) - (float)((ulonglong)param_2 >> 0x20)) *
                     SUB84(dVar24,0);
            in_dr2 = (double)CONCAT44(fVar29,SUB84(dVar28,0) - fVar29);
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
          *piVar11 = (int)puVar14;
          *(undefined8 **)((int)pdVar23 + -0xc) = puVar15;
          *(undefined8 **)((int)pdVar23 + -8) = puVar16;
          iVar10 = 0;
          while( true ) {
            iVar5 = DAT_8c6150ec;
            pdVar13 = (double *)((int)pdVar23 + -0x34);
            uVar32 = (undefined4)((ulonglong)in_dr2 >> 0x20);
            if (2 < iVar10) break;
            piVar8 = piVar11 + iVar10;
            *(int **)((int)pdVar23 + -0x50) = piVar8;
            *(int *)((int)pdVar23 + -0x54) = iVar10 * 0xc + (int)pdVar13;
            iVar10 = iVar10 + 1;
            if ((*(uint *)(*piVar8 + 0xc) & 1) == 0) {
              dVar28 = dVar35;
              if (in_FPSCR_SZ == '\0') {
                dVar28 = (double)CONCAT44(uVar36,SUB84(param_1,0));
              }
              iVar9 = **(int **)((int)pdVar23 + -0x50);
              *(int *)((int)pdVar23 + -0x40) = iVar9;
              if (in_FPSCR_SZ == '\0') {
                in_dr2 = (double)CONCAT44(uVar32,*(undefined4 *)(iVar9 + 0x18));
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
                in_dr2 = (double)(ulonglong)*(uint *)(DAT_8c6150ec + 0xc);
              }
              else {
                in_xd2 = *(double *)(DAT_8c6150ec + 0xc);
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
                                          *(undefined4 *)DAT_8c6150f0);
              }
              else {
                in_xd2 = *DAT_8c6150f0;
              }
              fVar29 = (float)((ulonglong)param_1 >> 0x20);
              if (in_FPSCR_PR == '\0') {
                dVar28 = (double)CONCAT44(((float)((ulonglong)dVar28 >> 0x20) * fVar29) /
                                          SUB84(dVar28,0),SUB84(dVar28,0));
              }
              else {
                dVar28 = (dVar28 * param_1) / (dVar28 * param_1);
              }
              uVar32 = (undefined4)((ulonglong)dVar28 >> 0x20);
              if (in_FPSCR_SZ == '\0') {
                *(undefined4 *)*(double **)((int)pdVar23 + -0x54) = uVar32;
              }
              else {
                **(double **)((int)pdVar23 + -0x54) = dVar28;
              }
              if (in_FPSCR_SZ == '\0') {
                dVar28 = (double)CONCAT44(uVar32,*(undefined4 *)(iVar5 + 0x10));
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
                                          *(undefined4 *)DAT_8c6150f4);
              }
              else {
                in_xd2 = *DAT_8c6150f4;
              }
              if (in_FPSCR_PR == '\0') {
                in_dr2 = (double)CONCAT44(((float)((ulonglong)dVar28 >> 0x20) * fVar29) /
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
                *(float *)(*(int *)((int)pdVar23 + -0x54) + 8) = fVar29;
              }
              else {
                *(double *)(*(int *)((int)pdVar23 + -0x54) + 8) = param_1;
              }
            }
            else {
              if (in_FPSCR_SZ == '\0') {
                uVar34 = *(undefined4 *)(**(int **)((int)pdVar23 + -0x50) + 0x10);
                in_dr2 = (double)CONCAT44(uVar32,uVar34);
                *(undefined4 *)*(double **)((int)pdVar23 + -0x54) = uVar34;
              }
              else {
                in_xd2 = *(double *)(**(int **)((int)pdVar23 + -0x50) + 0x10);
                **(double **)((int)pdVar23 + -0x54) = in_xd2;
              }
              if (in_FPSCR_SZ == '\0') {
                uVar32 = *(undefined4 *)(**(int **)((int)pdVar23 + -0x50) + 0x14);
                in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),uVar32);
                *(undefined4 *)(*(int *)((int)pdVar23 + -0x54) + 4) = uVar32;
              }
              else {
                in_xd2 = *(double *)(**(int **)((int)pdVar23 + -0x50) + 0x14);
                *(double *)(*(int *)((int)pdVar23 + -0x54) + 4) = in_xd2;
              }
              if (in_FPSCR_SZ == '\0') {
                uVar32 = *(undefined4 *)(**(int **)((int)pdVar23 + -0x50) + 0x18);
                in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),uVar32);
                *(undefined4 *)(*(int *)((int)pdVar23 + -0x54) + 8) = uVar32;
              }
              else {
                in_xd2 = *(double *)(**(int **)((int)pdVar23 + -0x50) + 0x18);
                *(double *)(*(int *)((int)pdVar23 + -0x54) + 8) = in_xd2;
              }
            }
          }
          if (in_FPSCR_SZ == '\0') {
            uVar34 = *(undefined4 *)((int)pdVar23 + -0x2c);
            dVar24 = (double)CONCAT44(uVar34,SUB84(dVar24,0));
            param_3 = (double)CONCAT44(uVar34,SUB84(param_3,0));
          }
          else {
            dVar24 = *(double *)((int)pdVar23 + -0x2c);
            uVar34 = (undefined4)((ulonglong)dVar24 >> 0x20);
            param_3 = dVar24;
          }
          pdVar4 = (double *)((int)pdVar23 + -0x28);
          *(double **)((int)pdVar23 + -0x54) = pdVar4;
          if (in_FPSCR_SZ == '\0') {
            uVar33 = *(undefined4 *)((int)pdVar23 + -0x30);
            param_3 = (double)CONCAT44(uVar34,uVar33);
            dVar28 = (double)CONCAT44(*(undefined4 *)((int)pdVar23 + -0x24),SUB84(param_1,0));
            param_2 = (double)CONCAT44(*(undefined4 *)((int)pdVar23 + -0x20),SUB84(param_2,0));
            in_dr2 = (double)CONCAT44(uVar32,uVar33);
          }
          else {
            in_xd2 = *(double *)((int)pdVar23 + -0x30);
            dVar28 = *(double *)((int)pdVar23 + -0x24);
            param_2 = *(double *)((int)pdVar23 + -0x20);
            uVar33 = SUB84(param_3,0);
            in_xd8 = in_xd2;
          }
          fVar29 = (float)((ulonglong)dVar28 >> 0x20);
          fVar31 = (float)((ulonglong)param_2 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * fVar29,SUB84(dVar24,0));
            fVar26 = SUB84(in_dr2,0) * fVar31;
          }
          else {
            dVar24 = dVar24 * dVar28;
            fVar26 = SUB84(in_dr2 * param_2,0);
          }
          if (in_FPSCR_SZ == '\0') {
            uVar32 = *(undefined4 *)pdVar13;
            param_2 = (double)CONCAT44(fVar31,uVar32);
            dVar28 = (double)CONCAT44(fVar29,*(undefined4 *)pdVar4);
            dVar41 = (double)CONCAT44(uVar32,fVar26);
          }
          else {
            dVar41 = *pdVar13;
            dVar40 = *pdVar4;
            uVar32 = SUB84(param_2,0);
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
            dVar24 = (double)CONCAT44((int)((ulonglong)dVar24 >> 0x20),uVar33);
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
          fVar29 = SUB84(dVar28,0);
          if (in_FPSCR_PR == '\0') {
            fVar31 = SUB84(dVar41,0) * fVar29;
            dVar41 = (double)CONCAT44((float)((ulonglong)dVar41 >> 0x20) - fVar31,fVar31);
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
            fVar31 = SUB84(dVar41,0) * (float)((ulonglong)dVar41 >> 0x20);
          }
          else {
            fVar31 = SUB84(dVar41 * dVar41,0);
          }
          if (in_FPSCR_SZ == '\0') {
            uVar25 = CONCAT44(*(undefined4 *)((int)pdVar23 + -0x1c),fVar31);
          }
          else {
            uVar25 = *(undefined8 *)((int)pdVar23 + -0x1c);
          }
          if (in_FPSCR_PR == '\0') {
            fVar31 = (float)((ulonglong)uVar25 >> 0x20);
            uVar25 = CONCAT44(fVar31,(float)((ulonglong)dVar24 >> 0x20) * fVar31 + (float)uVar25);
          }
          dVar41 = dVar38;
          dVar39 = in_xd6;
          if (in_FPSCR_SZ == '\0') {
            dVar41 = (double)CONCAT44((int)uVar25,uVar32);
            dVar39 = dVar38;
          }
          if (in_FPSCR_PR == '\0') {
            fVar31 = SUB84(dVar41,0) * (float)((ulonglong)dVar28 >> 0x20);
            dVar41 = (double)CONCAT44((int)((ulonglong)dVar41 >> 0x20),fVar31);
            fVar31 = SUB84(dVar24,0) * fVar29 - fVar31;
          }
          else {
            dVar41 = dVar41 * dVar28;
            fVar31 = SUB84(dVar24 * dVar28 - dVar41,0);
          }
          if (in_FPSCR_SZ == '\0') {
            dVar41 = (double)CONCAT44((int)((ulonglong)dVar41 >> 0x20),
                                      *(undefined4 *)((int)pdVar23 + -0x14));
            dVar24 = (double)CONCAT44(fVar31,fVar31);
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
          uVar32 = (undefined4)((ulonglong)dVar41 >> 0x20);
          fVar31 = -SUB84(dVar41,0);
          dVar41 = (double)CONCAT44(uVar32,fVar31);
          dVar28 = dVar41;
          if (in_FPSCR_SZ == '\0') {
            dVar28 = (double)CONCAT44(uVar32,fVar29);
          }
          if (in_FPSCR_PR == '\0') {
            dVar28 = (double)CONCAT44((float)((ulonglong)dVar28 >> 0x20) * fVar31,SUB84(dVar28,0));
          }
          else {
            dVar28 = dVar28 * dVar41;
          }
          in_dr2 = (double)(ulonglong)(uint)fVar31;
          if (in_FPSCR_PR == '\0') {
            bVar42 = 0.0 < (float)((ulonglong)dVar28 >> 0x20);
          }
          else {
            bVar42 = in_dr2 < dVar28;
          }
          param_1 = dVar28;
          in_xd0 = dVar37;
          if ((!bVar42) &&
             (*(undefined4 *)((int)pdVar23 + -0x3c) = 1, param_1 = dVar35, in_FPSCR_SZ == '\0')) {
            param_1 = (double)CONCAT44(uVar36,SUB84(dVar28,0));
          }
        }
      }
      uVar34 = DAT_8c6153f8;
      uVar32 = DAT_8c6153f4;
      uVar30 = (uint)((ulonglong)in_dr2 >> 0x20);
      fVar29 = (float)((ulonglong)param_1 >> 0x20);
      uVar36 = SUB84(param_1,0);
      if (pdVar12 < unaff_r8) {
        if (in_FPSCR_PR == '\0') {
          bVar42 = fVar29 < 0.0;
        }
        else {
          bVar42 = param_1 < (double)((ulonglong)uVar30 << 0x20);
        }
        psVar21 = psVar20;
        if (bVar42) {
LAB_8c615252:
          uVar32 = DAT_8c6153f8;
          uVar36 = SUB84(param_1,0);
          psVar22 = psVar21 + 2;
          if (in_FPSCR_SZ == '\0') {
            uVar25 = CONCAT44(*(undefined4 *)puVar15,SUB84(dVar24,0));
            uVar27 = (ulonglong)*(uint *)((int)puVar15 + 4) << 0x20;
            pdVar13 = (double *)(puVar15 + 1);
          }
          else {
            uVar25 = *puVar15;
            uVar27 = puVar15[1];
            pdVar13 = (double *)(puVar15 + 2);
          }
          dVar41 = (double)(uVar27 & 0xffffffff00000000);
          dVar24 = in_xd2;
          dVar28 = in_xd0;
          if (in_FPSCR_SZ == '\0') {
            dVar24 = (double)((ulonglong)param_4 & 0xffffffff);
            dVar28 = (double)CONCAT44((float)uVar25,SUB84(param_3,0));
          }
                    /* WARNING: Ignoring partial resolution of indirect */
          fVar29 = (float)((ulonglong)uVar25 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            fVar31 = (float)uVar25 * SUB84(dVar28,0) + fVar29 * (float)((ulonglong)dVar28 >> 0x20) +
                     SUB84(dVar24,0) * 0.0 +
                     (float)((ulonglong)dVar41 >> 0x20) * (float)((ulonglong)dVar24 >> 0x20);
          }
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44(*(undefined4 *)pdVar13,uVar36);
            param_2 = (double)CONCAT44(*(undefined4 *)((int)pdVar13 + 4),SUB84(param_2,0));
            param_3 = (double)CONCAT44(uVar36,SUB84(dVar28,0));
          }
          else {
            param_1 = *pdVar13;
            param_2 = pdVar13[1];
            uVar36 = SUB84(param_1,0);
            param_3 = dVar40;
          }
          sVar3 = *psVar21;
          sVar1 = psVar21[1];
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)unaff_r8 + 0xc) = (int)((ulonglong)param_2 >> 0x20);
            unaff_r8 = unaff_r8 + 1;
            *(undefined4 *)unaff_r8 = uVar36;
          }
          else {
            unaff_r8[1] = param_2;
            *unaff_r8 = dVar40;
          }
          if (in_FPSCR_PR == '\0') {
            param_4 = (double)(ulonglong)(uint)(SUB84(dVar24,0) - SUB84(dVar41,0));
          }
          else {
            param_4 = dVar24 - dVar41;
          }
          if (in_FPSCR_SZ == '\0') {
            pdVar13 = (double *)((int)unaff_r8 + -4);
            *(int *)pdVar13 = (int)((ulonglong)param_1 >> 0x20);
          }
          else {
            pdVar13 = unaff_r8 + -1;
            *pdVar13 = param_1;
          }
          fVar31 = (float)((ulonglong)param_3 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            dVar24 = (double)CONCAT44(fVar29,(float)(int)sVar1 * fVar31);
          }
          else {
            dVar24 = (double)CONCAT44(fVar29,(float)(int)sVar1) * param_3;
          }
          *(undefined4 *)((int)pdVar13 + -4) = uVar32;
          if (in_FPSCR_PR == '\0') {
            in_dr2 = (double)CONCAT44((float)(int)sVar3 * fVar31,SUB84(dVar41,0));
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
            uVar25 = CONCAT44(*(undefined4 *)puVar15,SUB84(dVar24,0));
            uVar27 = (ulonglong)*(uint *)((int)puVar15 + 4) << 0x20;
            pdVar12 = (double *)(puVar15 + 1);
          }
          else {
            uVar25 = *puVar15;
            uVar27 = puVar15[1];
            pdVar12 = (double *)(puVar15 + 2);
          }
          dVar41 = (double)(uVar27 & 0xffffffff00000000);
          dVar24 = in_xd2;
          dVar28 = in_xd0;
          if (in_FPSCR_SZ == '\0') {
            dVar24 = (double)((ulonglong)param_4 & 0xffffffff);
            dVar28 = (double)CONCAT44((float)uVar25,SUB84(param_3,0));
          }
                    /* WARNING: Ignoring partial resolution of indirect */
          fVar29 = (float)((ulonglong)uVar25 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            fVar31 = (float)uVar25 * SUB84(dVar28,0) + fVar29 * (float)((ulonglong)dVar28 >> 0x20) +
                     SUB84(dVar24,0) * 0.0 +
                     (float)((ulonglong)dVar41 >> 0x20) * (float)((ulonglong)dVar24 >> 0x20);
          }
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44(*(undefined4 *)pdVar12,uVar36);
            param_2 = (double)CONCAT44(*(undefined4 *)((int)pdVar12 + 4),SUB84(param_2,0));
            param_3 = (double)CONCAT44(uVar36,SUB84(dVar28,0));
          }
          else {
            param_1 = *pdVar12;
            param_2 = pdVar12[1];
            uVar36 = SUB84(param_1,0);
            param_3 = dVar40;
          }
          sVar3 = *psVar20;
          sVar1 = psVar20[1];
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)unaff_r8 + 0xc) = (int)((ulonglong)param_2 >> 0x20);
            unaff_r8 = unaff_r8 + 1;
            *(undefined4 *)unaff_r8 = uVar36;
          }
          else {
            unaff_r8[1] = param_2;
            *unaff_r8 = dVar40;
          }
          if (in_FPSCR_PR == '\0') {
            param_4 = (double)(ulonglong)(uint)(SUB84(dVar24,0) - SUB84(dVar41,0));
          }
          else {
            param_4 = dVar24 - dVar41;
          }
          if (in_FPSCR_SZ == '\0') {
            pdVar12 = (double *)((int)unaff_r8 + -4);
            *(int *)pdVar12 = (int)((ulonglong)param_1 >> 0x20);
          }
          else {
            pdVar12 = unaff_r8 + -1;
            *pdVar12 = param_1;
          }
          fVar31 = (float)((ulonglong)param_3 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            dVar24 = (double)CONCAT44(fVar29,(float)(int)sVar1 * fVar31);
          }
          else {
            dVar24 = (double)CONCAT44(fVar29,(float)(int)sVar1) * param_3;
          }
          *(undefined4 *)((int)pdVar12 + -4) = uVar32;
          if (in_FPSCR_PR == '\0') {
            in_dr2 = (double)CONCAT44((float)(int)sVar3 * fVar31,SUB84(dVar41,0));
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
        in_dr2 = (double)((ulonglong)uVar30 << 0x20);
        if (in_FPSCR_PR == '\0') {
          bVar42 = fVar29 < 0.0;
        }
        else {
          bVar42 = param_1 < in_dr2;
        }
        psVar22 = psVar20 + 2;
        if (bVar42) {
          psVar21 = psVar20 + 2;
          if (in_FPSCR_SZ == '\0') {
            uVar25 = CONCAT44(*(undefined4 *)puVar14,SUB84(dVar24,0));
            uVar27 = (ulonglong)*(uint *)((int)puVar14 + 4) << 0x20;
            pdVar13 = (double *)(puVar14 + 1);
          }
          else {
            uVar25 = *puVar14;
            uVar27 = puVar14[1];
            pdVar13 = (double *)(puVar14 + 2);
          }
          dVar41 = (double)(uVar27 & 0xffffffff00000000);
          dVar24 = in_xd2;
          dVar28 = in_xd0;
          if (in_FPSCR_SZ == '\0') {
            dVar24 = (double)((ulonglong)param_4 & 0xffffffff);
            dVar28 = (double)CONCAT44((float)uVar25,SUB84(param_3,0));
          }
                    /* WARNING: Ignoring partial resolution of indirect */
          fVar29 = (float)((ulonglong)uVar25 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            fVar31 = (float)uVar25 * SUB84(dVar28,0) + fVar29 * (float)((ulonglong)dVar28 >> 0x20) +
                     SUB84(dVar24,0) * 0.0 +
                     (float)((ulonglong)dVar41 >> 0x20) * (float)((ulonglong)dVar24 >> 0x20);
          }
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44(*(undefined4 *)pdVar13,uVar36);
            param_2 = (double)CONCAT44(*(undefined4 *)((int)pdVar13 + 4),SUB84(param_2,0));
            param_3 = (double)CONCAT44(uVar36,SUB84(dVar28,0));
          }
          else {
            param_1 = *pdVar13;
            param_2 = pdVar13[1];
            uVar36 = SUB84(param_1,0);
            param_3 = dVar40;
          }
          sVar3 = *psVar20;
          sVar1 = psVar20[1];
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)unaff_r8 + 0xc) = (int)((ulonglong)param_2 >> 0x20);
            unaff_r8 = unaff_r8 + 1;
            *(undefined4 *)unaff_r8 = uVar36;
          }
          else {
            unaff_r8[1] = param_2;
            *unaff_r8 = dVar40;
          }
          if (in_FPSCR_PR == '\0') {
            param_4 = (double)(ulonglong)(uint)(SUB84(dVar24,0) - SUB84(dVar41,0));
          }
          else {
            param_4 = dVar24 - dVar41;
          }
          if (in_FPSCR_SZ == '\0') {
            pdVar13 = (double *)((int)unaff_r8 + -4);
            *(int *)pdVar13 = (int)((ulonglong)param_1 >> 0x20);
          }
          else {
            pdVar13 = unaff_r8 + -1;
            *pdVar13 = param_1;
          }
          fVar31 = (float)((ulonglong)param_3 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            dVar24 = (double)(ulonglong)(uint)((float)(int)sVar1 * fVar31);
          }
          else {
            dVar24 = (double)CONCAT44(fVar29,(float)(int)sVar1) * param_3;
          }
          *(undefined4 *)((int)pdVar13 + -4) = uVar34;
          if (in_FPSCR_PR == '\0') {
            dVar28 = (double)CONCAT44((float)(int)sVar3 * fVar31,SUB84(dVar41,0));
          }
          else {
            dVar28 = (double)(int)sVar3 * param_3;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)pdVar13 + 0x14) = SUB84(param_4,0);
            *(int *)(pdVar13 + 2) = SUB84(dVar24,0);
            pdVar13 = (double *)((int)pdVar13 + 0xc);
            *(int *)pdVar13 = (int)((ulonglong)dVar28 >> 0x20);
          }
          else {
            pdVar13[2] = in_xd10;
            pdVar13[1] = in_xd0;
            *pdVar13 = dVar28;
          }
          unaff_r8 = pdVar13 + 2;
          goto LAB_8c615252;
        }
      }
      puVar6 = DAT_8c615400;
      uVar36 = SUB84(param_2,0);
      if (*(int *)((int)pdVar23 + -0x3c) != 0) {
        *DAT_8c6153fc = 0;
        uVar32 = (undefined4)((ulonglong)in_dr2 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          uVar43 = (undefined2)(int)(float)(int)psVar22[-2];
        }
        else {
          uVar43 = (undefined2)(int)(double)CONCAT44(uVar32,(float)(int)psVar22[-2]);
        }
        *puVar6 = uVar43;
        if (in_FPSCR_PR == '\0') {
          uVar43 = (undefined2)(int)(float)(int)psVar22[-1];
        }
        else {
          uVar43 = (undefined2)(int)(double)CONCAT44(uVar32,(float)(int)psVar22[-1]);
        }
        puVar6[1] = uVar43;
        if (in_FPSCR_PR == '\0') {
          uVar43 = (undefined2)(int)(float)(int)*psVar22;
        }
        else {
          uVar43 = (undefined2)(int)(double)CONCAT44(uVar32,(float)(int)*psVar22);
        }
        puVar6[2] = uVar43;
        if (in_FPSCR_PR == '\0') {
          uVar43 = (undefined2)(int)(float)(int)psVar22[1];
        }
        else {
          uVar43 = (undefined2)(int)(double)CONCAT44(uVar32,(float)(int)psVar22[1]);
        }
        puVar6[3] = uVar43;
        if (in_FPSCR_PR == '\0') {
          uVar43 = (undefined2)(int)(float)(int)psVar22[2];
        }
        else {
          uVar43 = (undefined2)(int)(double)CONCAT44(uVar32,(float)(int)psVar22[2]);
        }
        puVar6[4] = uVar43;
        puVar7 = PTR_FUN_8c615404;
        in_dr2 = (double)CONCAT44(uVar32,(float)(int)psVar22[3]);
        if (in_FPSCR_PR == '\0') {
          uVar43 = (undefined2)(int)(float)(int)psVar22[3];
        }
        else {
          uVar43 = (undefined2)(int)in_dr2;
        }
        puVar6[5] = uVar43;
        dVar24 = (double)(*(code *)puVar7)(puVar14,puVar15,puVar16,puVar6);
        pdVar12 = unaff_r8;
      }
      uVar32 = DAT_8c6153f4;
      in_xd4 = in_xd12;
      if (in_FPSCR_SZ == '\0') {
        param_1 = (double)((ulonglong)dVar35 & 0xffffffff);
        in_xd4 = dVar40;
      }
      uVar34 = SUB84(param_1,0);
      iVar10 = *(int *)((int)pdVar23 + -0x4c);
      if (in_FPSCR_SZ == '\0') {
        dVar35 = (double)CONCAT44((int)((ulonglong)dVar35 >> 0x20),*(undefined4 *)(puVar16 + 2));
        uVar25 = CONCAT44(*(undefined4 *)((int)puVar16 + 0x14),SUB84(in_dr14,0));
        param_2 = (double)CONCAT44((int)((ulonglong)in_dr14 >> 0x20),uVar36);
      }
      else {
        in_xd12 = (double)puVar16[2];
        uVar25 = *(undefined8 *)((int)puVar16 + 0x14);
        param_2 = in_dr14;
      }
      in_dr14 = (double)CONCAT44((int)((ulonglong)uVar25 >> 0x20),-(float)uVar25);
      *(int *)((int)pdVar23 + -0x4c) = iVar10 + -1;
      puVar14 = puVar15;
      puVar15 = puVar16;
      psVar20 = psVar22;
      dVar40 = in_xd4;
    } while (iVar10 + -1 != 0);
    psVar20 = psVar22 + 4;
    if (pdVar12 < unaff_r8) {
      psVar20 = psVar22 + 2;
      if (in_FPSCR_SZ == '\0') {
        uVar25 = CONCAT44(*(undefined4 *)puVar16,SUB84(dVar24,0));
        uVar27 = (ulonglong)*(uint *)((int)puVar16 + 4) << 0x20;
        pdVar12 = (double *)(puVar16 + 1);
      }
      else {
        uVar25 = *puVar16;
        uVar27 = puVar16[1];
        pdVar12 = (double *)(puVar16 + 2);
      }
      dVar28 = (double)(uVar27 & 0xffffffff00000000);
      dVar24 = in_xd2;
      dVar40 = in_xd0;
      if (in_FPSCR_SZ == '\0') {
        dVar24 = (double)((ulonglong)param_4 & 0xffffffff);
        dVar40 = (double)CONCAT44((float)uVar25,SUB84(param_3,0));
      }
                    /* WARNING: Ignoring partial resolution of indirect */
      fVar29 = (float)((ulonglong)uVar25 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar31 = (float)uVar25 * SUB84(dVar40,0) + fVar29 * (float)((ulonglong)dVar40 >> 0x20) +
                 SUB84(dVar24,0) * 0.0 +
                 (float)((ulonglong)dVar28 >> 0x20) * (float)((ulonglong)dVar24 >> 0x20);
      }
      if (in_FPSCR_SZ == '\0') {
        param_1 = (double)CONCAT44(*(undefined4 *)pdVar12,uVar34);
        param_2 = (double)CONCAT44(*(undefined4 *)((int)pdVar12 + 4),SUB84(param_2,0));
        param_3 = (double)CONCAT44(uVar34,SUB84(dVar40,0));
      }
      else {
        param_1 = *pdVar12;
        param_2 = pdVar12[1];
        uVar34 = SUB84(param_1,0);
        param_3 = in_xd4;
      }
      sVar3 = *psVar22;
      sVar1 = psVar22[1];
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)unaff_r8 + 0xc) = (int)((ulonglong)param_2 >> 0x20);
        unaff_r8 = unaff_r8 + 1;
        *(undefined4 *)unaff_r8 = uVar34;
      }
      else {
        unaff_r8[1] = param_2;
        *unaff_r8 = in_xd4;
      }
      if (in_FPSCR_PR == '\0') {
        param_4 = (double)(ulonglong)(uint)(SUB84(dVar24,0) - SUB84(dVar28,0));
      }
      else {
        param_4 = dVar24 - dVar28;
      }
      if (in_FPSCR_SZ == '\0') {
        pdVar12 = (double *)((int)unaff_r8 + -4);
        *(int *)pdVar12 = (int)((ulonglong)param_1 >> 0x20);
      }
      else {
        pdVar12 = unaff_r8 + -1;
        *pdVar12 = param_1;
      }
      fVar31 = (float)((ulonglong)param_3 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar24 = (double)CONCAT44(fVar29,(float)(int)sVar1 * fVar31);
      }
      else {
        dVar24 = (double)CONCAT44(fVar29,(float)(int)sVar1) * param_3;
      }
      *(undefined4 *)((int)pdVar12 + -4) = uVar32;
      if (in_FPSCR_PR == '\0') {
        in_dr2 = (double)CONCAT44((float)(int)sVar3 * fVar31,SUB84(dVar28,0));
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
      *DAT_8c615408 = *DAT_8c615408 + *(int *)((int)pdVar23 + -0x44);
      *DAT_8c61540c = *DAT_8c61540c + *(int *)((int)pdVar23 + -0x44);
      return;
    }
  } while( true );
}

