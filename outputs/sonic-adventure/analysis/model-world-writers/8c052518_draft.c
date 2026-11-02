/* Ghidra draft only; not buildable/verified source. */

void FUN_8c052518(double param_1,double param_2,double param_3,double param_4)

{
  short *psVar1;
  undefined *puVar2;
  undefined4 uVar3;
  uint uVar4;
  uint *puVar5;
  undefined4 *puVar6;
  int iVar7;
  double *pdVar8;
  undefined4 *puVar9;
  double *pdVar10;
  uint *puVar11;
  uint uVar12;
  uint *puVar13;
  undefined8 *puVar14;
  double dVar15;
  double in_dr2;
  float fVar17;
  float fVar18;
  double dVar16;
  undefined4 uVar19;
  undefined8 in_dr12;
  double dVar20;
  undefined8 in_dr14;
  undefined8 in_xd14;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_3c [16];
  undefined8 uStack_2c;
  undefined8 uStack_24;
  
  puVar2 = PTR_DAT_8c052614;
  psVar1 = DAT_8c052610;
  puVar13 = DAT_8c052604;
  if (in_FPSCR_SZ == '\0') {
    uStack_24 = CONCAT44((int)in_dr14,(undefined4)uStack_24);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_24._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
    puVar14 = &uStack_2c;
    uStack_2c = CONCAT44((int)in_dr12,(int)((ulonglong)in_dr12 >> 0x20));
  }
  else {
    puVar14 = (undefined8 *)auStack_3c;
    uStack_24 = in_xd14;
    uStack_2c = in_dr14;
  }
  *(undefined4 *)((int)puVar14 + -4) = in_PR;
  puVar9 = (undefined4 *)((int)puVar14 + -0x1c);
  iVar7 = *(int *)PTR_DAT_8c05260c;
  uVar3 = *(undefined4 *)(iVar7 + 0x24);
  *puVar9 = *(undefined4 *)(iVar7 + 0x20);
  *(undefined4 *)((int)puVar14 + -0x18) = uVar3;
  *(undefined4 *)((int)puVar14 + -0x14) = *(undefined4 *)(iVar7 + 0x28);
  *(undefined4 *)((int)puVar14 + -0x34) = 1;
  if (*DAT_8c052608 == 0) {
    if (DAT_8c052608[1] != 0) {
      iVar7 = DAT_8c052608[1];
      uVar3 = *(undefined4 *)(iVar7 + 0x24);
      *puVar9 = *(undefined4 *)(iVar7 + 0x20);
      *(undefined4 *)((int)puVar14 + -0x18) = uVar3;
      *(undefined4 *)((int)puVar14 + -0x14) = *(undefined4 *)(iVar7 + 0x28);
    }
  }
  else {
    iVar7 = *DAT_8c052608;
    uVar3 = *(undefined4 *)(iVar7 + 0x24);
    *puVar9 = *(undefined4 *)(iVar7 + 0x20);
    *(undefined4 *)((int)puVar14 + -0x18) = uVar3;
    *(undefined4 *)((int)puVar14 + -0x14) = *(undefined4 *)(iVar7 + 0x28);
    if (DAT_8c052608[1] != 0) {
      iVar7 = DAT_8c052608[1];
      uVar3 = *(undefined4 *)(iVar7 + 0x24);
      *(undefined4 *)((int)puVar14 + -0x10) = *(undefined4 *)(iVar7 + 0x20);
      *(undefined4 *)((int)puVar14 + -0xc) = uVar3;
      *(undefined4 *)((int)puVar14 + -8) = *(undefined4 *)(iVar7 + 0x28);
      *(int *)((int)puVar14 + -0x34) = *(int *)((int)puVar14 + -0x34) + 1;
    }
  }
  *DAT_8c052610 = 0;
  *(undefined4 *)puVar2 = 0;
  *(undefined4 *)((int)puVar14 + -0x38) = DAT_8c052618;
  (*(code *)PTR_FUN_8c05261c)(0);
  puVar11 = DAT_8c052624 + (*DAT_8c052620 + -1) * 3;
  if (in_FPSCR_SZ == '\0') {
    dVar20 = (double)CONCAT44((undefined4)DAT_8c052628,(int)in_dr12);
  }
  else {
    dVar20 = (double)CONCAT44(DAT_8c052628._4_4_,(undefined4)DAT_8c052628);
  }
  if (DAT_8c052624 <= puVar11) {
    do {
      uVar19 = SUB84(param_4,0);
      uVar3 = SUB84(param_3,0);
      uVar12 = puVar11[1];
      if ((*(ushort *)(*(int *)(puVar11[2] + 0x20) + 4) & 0x100) != 0) {
        if ((*puVar11 & DAT_8c052628._4_4_) == 0) {
          puVar9 = (undefined4 *)(*(int *)(uVar12 + 4) + 0x18);
        }
        else {
          (*(code *)PTR_FUN_8c052820)(0);
          iVar7 = *(int *)(uVar12 + 0x1c);
          *(int *)((int)puVar14 + -0x3c) = iVar7;
          if (iVar7 != 0) {
            (*(code *)PTR_FUN_8c052824)(0,*(undefined4 *)((int)puVar14 + -0x3c));
          }
          iVar7 = *(int *)(uVar12 + 0x14);
          *(int *)((int)puVar14 + -0x3c) = iVar7;
          if (iVar7 != 0) {
            (*(code *)PTR_FUN_8c052828)(0,*(undefined4 *)((int)puVar14 + -0x3c));
          }
          iVar7 = *(int *)(uVar12 + 0x18);
          *(int *)((int)puVar14 + -0x3c) = iVar7;
          if (iVar7 != 0) {
            (*(code *)PTR_FUN_8c05282c)(0,*(undefined4 *)((int)puVar14 + -0x3c));
          }
          (*(code *)PTR_FUN_8c052830)
                    (0,*(int *)(uVar12 + 4) + 0x18,(undefined1 *)((int)puVar14 + -0x28));
          puVar9 = (undefined4 *)((int)puVar14 + -0x28);
        }
        if (in_FPSCR_SZ == '\0') {
          param_3 = (double)CONCAT44(*(undefined4 *)(uVar12 + 8),uVar3);
          in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*puVar9);
          puVar9 = puVar9 + 1;
          param_4 = (double)CONCAT44(*(undefined4 *)(uVar12 + 0xc),uVar19);
          puVar6 = (undefined4 *)(uVar12 + 0x10);
        }
        else {
          param_3 = *(double *)(uVar12 + 8);
          puVar9 = puVar9 + 2;
          param_4 = *(double *)(uVar12 + 0x10);
          puVar6 = (undefined4 *)(uVar12 + 0x18);
        }
        if (in_FPSCR_PR == '\0') {
          param_3 = (double)CONCAT44((float)((ulonglong)param_3 >> 0x20) + SUB84(in_dr2,0),
                                     SUB84(param_3,0));
        }
        else {
          param_3 = param_3 + in_dr2;
        }
        if (in_FPSCR_SZ == '\0') {
          in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*puVar9);
          puVar9 = puVar9 + 1;
          param_3 = (double)CONCAT44((int)((ulonglong)param_3 >> 0x20),*puVar6);
        }
        else {
          puVar9 = puVar9 + 2;
        }
        if (in_FPSCR_PR == '\0') {
          param_4 = (double)CONCAT44((float)((ulonglong)param_4 >> 0x20) + SUB84(in_dr2,0),
                                     SUB84(param_4,0));
        }
        else {
          param_4 = param_4 + in_dr2;
        }
        if (in_FPSCR_SZ == '\0') {
          in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*puVar9);
        }
        if (in_FPSCR_PR == '\0') {
          param_3 = (double)CONCAT44((int)((ulonglong)param_3 >> 0x20),
                                     SUB84(param_3,0) + SUB84(in_dr2,0));
        }
        else {
          param_3 = param_3 + in_dr2;
        }
        iVar7 = *(int *)((int)puVar14 + -0x34);
        if (in_FPSCR_SZ == '\0') {
          param_2 = (double)(ulonglong)*(uint *)PTR_DAT_8c052834;
        }
        pdVar10 = (double *)((int)puVar14 + -0x1c);
        while (0 < iVar7) {
          if (in_FPSCR_SZ == '\0') {
            dVar16 = (double)CONCAT44(*(undefined4 *)pdVar10,SUB84(param_1,0));
            pdVar8 = (double *)((int)pdVar10 + 4);
          }
          else {
            dVar16 = *pdVar10;
            pdVar8 = pdVar10 + 1;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),*(undefined4 *)pdVar8);
            pdVar8 = (double *)((int)pdVar8 + 4);
          }
          else {
            pdVar8 = pdVar8 + 1;
          }
          if (in_FPSCR_PR == '\0') {
            dVar16 = (double)CONCAT44((float)((ulonglong)dVar16 >> 0x20) -
                                      (float)((ulonglong)param_3 >> 0x20),
                                      SUB84(dVar16,0) - (float)((ulonglong)param_4 >> 0x20));
          }
          else {
            dVar16 = (dVar16 - param_3) - param_4;
          }
          fVar17 = (float)((ulonglong)dVar16 >> 0x20);
          if (in_FPSCR_SZ == '\0') {
            param_2 = (double)CONCAT44(*(undefined4 *)pdVar8,SUB84(param_2,0));
            dVar15 = (double)((ulonglong)(uint)fVar17 << 0x20);
          }
          else {
            param_2 = *pdVar8;
            dVar15 = dVar16;
          }
          if (in_FPSCR_PR == '\0') {
            param_2 = (double)CONCAT44((float)((ulonglong)param_2 >> 0x20) - SUB84(param_3,0),
                                       SUB84(param_2,0));
          }
          else {
            param_2 = param_2 - param_3;
          }
          fVar18 = SUB84(dVar16,0);
          if (in_FPSCR_SZ == '\0') {
            in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),fVar18);
          }
          if (in_FPSCR_PR == '\0') {
            in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),
                                      (float)((ulonglong)dVar15 >> 0x20) * fVar17 +
                                      SUB84(in_dr2,0) * fVar18);
          }
          else {
            in_dr2 = in_dr2 * dVar16;
          }
          fVar17 = (float)((ulonglong)param_2 >> 0x20);
          if (in_FPSCR_SZ == '\0') {
            dVar16 = (double)CONCAT44(*(undefined4 *)(*(int *)(uVar12 + 4) + 0x24),fVar18);
            dVar15 = (double)((ulonglong)(uint)fVar17 << 0x20);
          }
          else {
            dVar16 = *(double *)(*(int *)(uVar12 + 4) + 0x24);
            dVar15 = param_2;
          }
          if (in_FPSCR_PR == '\0') {
            fVar18 = (float)((ulonglong)dVar16 >> 0x20) + SUB84(param_2,0);
            in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),
                                      (float)((ulonglong)dVar15 >> 0x20) * fVar17 + SUB84(in_dr2,0))
            ;
            param_1 = (double)CONCAT44(fVar18 * fVar18,SUB84(dVar16,0));
          }
          else {
            param_1 = (dVar16 + param_2) * (dVar16 + param_2);
          }
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),SUB84(in_dr2,0));
          }
          if (in_FPSCR_PR == '\0' && SUB84(param_1,0) < (float)((ulonglong)param_1 >> 0x20)) {
            puVar5 = *(uint **)((int)puVar14 + -0x38);
            uVar4 = puVar11[1];
            *puVar5 = *puVar11;
            puVar5[1] = uVar4;
            puVar5[2] = puVar11[2];
            *(int *)((int)puVar14 + -0x38) = *(int *)((int)puVar14 + -0x38) + 0xc;
            *(int *)puVar2 = *(int *)puVar2 + 1;
            puVar13[1] = uVar12;
            puVar13[2] = puVar11[2];
            *puVar13 = *puVar11;
            *psVar1 = *psVar1 + 1;
            puVar13 = puVar13 + 3;
            if (0x7f < *(int *)puVar2) goto LAB_8c05272e;
            if (0x3ff < *psVar1) {
              (*(code *)PTR_FUN_8c052838)(1);
              return;
            }
            break;
          }
          iVar7 = iVar7 + -1;
          if (in_FPSCR_SZ == '\0') {
            param_2 = (double)((ulonglong)dVar20 >> 0x20);
          }
          pdVar10 = (double *)((int)pdVar10 + 0xc);
        }
      }
      puVar11 = puVar11 + -3;
    } while (DAT_8c05283c <= puVar11);
  }
LAB_8c05272e:
  (*(code *)PTR_FUN_8c052838)(1);
  uVar3 = DAT_8c052844;
  puVar2 = PTR_DAT_8c052840;
  *(undefined4 *)PTR_DAT_8c052840 = 0;
  *(undefined4 *)((int)puVar14 + -0x3c) = uVar3;
  if ((*(int *)PTR_DAT_8c052848 == 1) && (*DAT_8c05284c != 0)) {
    puVar9 = *(undefined4 **)((undefined2 *)*DAT_8c05284c + 6);
    *(undefined2 *)((int)puVar14 + -0x30) = *(undefined2 *)*DAT_8c05284c;
    while (0 < *(short *)((int)puVar14 + -0x30)) {
      if ((puVar9[8] & DAT_8c052850) != 0) {
        if (in_FPSCR_SZ == '\0') {
          in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)PTR_DAT_8c052834
                                   );
          *(undefined4 *)((int)puVar14 + -0x2c) = *(undefined4 *)PTR_DAT_8c052834;
        }
        else {
          *(undefined8 *)((int)puVar14 + -0x2c) = *(undefined8 *)PTR_DAT_8c052834;
        }
        iVar7 = *(int *)((int)puVar14 + -0x34);
        *(undefined1 **)((int)puVar14 + -0x38) = (undefined1 *)((int)puVar14 + -0x1c);
        while (0 < iVar7) {
          if (in_FPSCR_SZ == '\0') {
            dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),puVar9[1]);
          }
          if (in_FPSCR_SZ == '\0') {
            puVar6 = (undefined4 *)(*(int *)((int)puVar14 + -0x38) + 4);
            in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*puVar6);
            puVar6 = puVar6 + 1;
          }
          else {
            puVar6 = (undefined4 *)(*(int *)((int)puVar14 + -0x38) + 0x10);
          }
          if (in_FPSCR_PR == '\0') {
            dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),
                                      SUB84(dVar20,0) - SUB84(in_dr2,0));
          }
          else {
            dVar20 = dVar20 - in_dr2;
          }
          if (in_FPSCR_SZ == '\0') {
            in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*puVar6);
          }
          if (in_FPSCR_PR == '\0') {
            dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),
                                      SUB84(dVar20,0) * SUB84(dVar20,0));
          }
          else {
            dVar20 = dVar20 * dVar20;
          }
          uVar3 = (*(code *)PTR_FUN_8c052854)();
          if (in_FPSCR_SZ == '\0') {
            in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),
                                      *(undefined4 *)((int)puVar14 + -0x2c));
            dVar16 = (double)CONCAT44(puVar9[3],uVar3);
          }
          else {
            dVar16 = *(double *)(puVar9 + 3);
          }
          if (in_FPSCR_PR == '\0') {
            dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),
                                      SUB84(dVar16,0) - SUB84(in_dr2,0));
          }
          else {
            dVar16 = dVar16 - in_dr2;
          }
          if (in_FPSCR_PR == '\0' && SUB84(dVar16,0) < (float)((ulonglong)dVar16 >> 0x20)) {
            puVar6 = *(undefined4 **)((int)puVar14 + -0x3c);
            uVar3 = puVar9[1];
            *puVar6 = *puVar9;
            puVar6[1] = uVar3;
            uVar3 = puVar9[3];
            puVar6[2] = puVar9[2];
            puVar6[3] = uVar3;
            uVar3 = puVar9[5];
            puVar6[4] = puVar9[4];
            puVar6[5] = uVar3;
            uVar3 = puVar9[7];
            puVar6[6] = puVar9[6];
            puVar6[7] = uVar3;
            puVar6[8] = puVar9[8];
            *(int *)((int)puVar14 + -0x3c) = *(int *)((int)puVar14 + -0x3c) + 0x24;
            *(int *)puVar2 = *(int *)puVar2 + 1;
            *puVar13 = puVar9[8];
            puVar13[2] = 0;
            puVar13[1] = puVar9[6];
            *psVar1 = *psVar1 + 1;
            puVar13 = puVar13 + 3;
            if (0x7f < *(int *)puVar2) {
              return;
            }
            if (0x3ff < *psVar1) {
              return;
            }
            break;
          }
          iVar7 = iVar7 + -1;
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)puVar14 + -0x2c) = (int)((ulonglong)dVar20 >> 0x20);
          }
          else {
            *(double *)((int)puVar14 + -0x2c) = dVar20;
          }
          *(int *)((int)puVar14 + -0x38) = *(int *)((int)puVar14 + -0x38) + 0xc;
        }
      }
      puVar9 = puVar9 + 9;
      *(short *)((int)puVar14 + -0x30) = *(short *)((int)puVar14 + -0x30) + -1;
    }
  }
  return;
}

