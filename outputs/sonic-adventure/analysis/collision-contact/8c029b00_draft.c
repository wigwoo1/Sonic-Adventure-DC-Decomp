/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c029c8e) */
/* WARNING: Removing unreachable block (ram,0x8c02a5fe) */
/* WARNING: Removing unreachable block (ram,0x8c029dce) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4
FUN_8c029b00(double param_1,double param_2,double param_3,double *param_4,undefined4 param_5)

{
  bool bVar1;
  undefined *puVar2;
  int iVar3;
  int iVar4;
  undefined8 *puVar5;
  undefined4 *puVar6;
  int *piVar7;
  double *pdVar8;
  double *pdVar9;
  int iVar10;
  undefined4 *puVar11;
  double *pdVar12;
  int iVar13;
  short sVar14;
  int iVar15;
  double *pdVar16;
  double *pdVar17;
  undefined8 *puVar18;
  undefined8 uVar19;
  float fVar20;
  float fVar21;
  float fVar22;
  float fVar23;
  float fVar24;
  double dVar25;
  double dVar26;
  uint uVar27;
  uint uVar28;
  undefined4 uVar29;
  double dVar30;
  undefined4 uVar31;
  double dVar32;
  double dVar33;
  double in_dr12;
  double dVar34;
  undefined8 in_dr14;
  undefined *puVar36;
  double dVar35;
  double in_xd0;
  double in_xd2;
  double dVar37;
  double in_xd4;
  double dVar38;
  double in_xd6;
  double in_xd8;
  undefined8 in_xd14;
  bool bVar39;
  short sVar40;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_3c [16];
  undefined8 uStack_2c;
  undefined8 uStack_24;
  
  if (in_FPSCR_SZ == '\0') {
    uStack_24 = CONCAT44((int)in_dr14,(undefined4)uStack_24);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_24._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
    puVar18 = &uStack_2c;
    uStack_2c = CONCAT44(SUB84(in_dr12,0),(int)((ulonglong)in_dr12 >> 0x20));
  }
  else {
    puVar18 = (undefined8 *)auStack_3c;
    uStack_24 = in_xd14;
    uStack_2c = in_dr14;
  }
  *(undefined4 *)((int)puVar18 + -4) = in_PR;
  iVar3 = (int)DAT_8c029b80;
  *(undefined4 *)((int)puVar18 + iVar3 + -4) = param_5;
  if (in_FPSCR_SZ == '\0') {
    dVar25 = (double)((ulonglong)*(uint *)param_4 << 0x20);
  }
  else {
    dVar25 = *param_4;
  }
  if (in_FPSCR_PR == '\0' && SUB84(dVar25,0) < (float)((ulonglong)dVar25 >> 0x20)) {
    *DAT_8c029b84 = 0;
    uVar19 = (*(code *)PTR_FUN_8c029b88)(param_4 + 2);
    puVar2 = PTR_FUN_8c029cd8;
    uVar29 = SUB84(param_1,0);
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)puVar18 + iVar3 + 0x18) = (int)((ulonglong)uVar19 >> 0x20);
      dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),DAT_8c029b8c);
    }
    else {
      *(undefined8 *)((int)puVar18 + iVar3 + 0x18) = uVar19;
      in_xd2 = (double)CONCAT44(_DAT_8c029b90,DAT_8c029b8c);
    }
    bVar39 = false;
    if (in_FPSCR_SZ == '\0') {
      in_dr12 = (double)CONCAT44((int)((ulonglong)in_dr12 >> 0x20),*(undefined4 *)param_4);
    }
    *(undefined4 *)((int)puVar18 + iVar3 + 0xc) = _DAT_8c029b90;
    if (in_FPSCR_PR == '\0') {
      dVar34 = (double)(ulonglong)(uint)(SUB84(in_dr12,0) * SUB84(dVar25,0));
    }
    else {
      dVar34 = in_dr12 * dVar25;
    }
    for (piVar7 = *(int **)PTR_DAT_8c029b94; piVar7 != (int *)0x0; piVar7 = (int *)*piVar7) {
      if (piVar7[2] == *(int *)((int)puVar18 + iVar3 + -4)) {
        bVar39 = true;
        break;
      }
    }
    if (bVar39) {
      *(int *)((int)puVar18 + iVar3 + 0x10) = piVar7[9];
      piVar7 = (int *)piVar7[7];
      pdVar8 = (double *)((int)puVar18 + (int)DAT_8c029cc0 + iVar3 + -4);
      *(double **)((int)puVar18 + iVar3 + -4) = pdVar8;
      pdVar16 = (double *)((int)pdVar8 + 0xc);
      *(undefined **)((int)puVar18 + iVar3 + 0x24) = PTR_DAT_8c029ccc;
      *(int *)((int)puVar18 + iVar3 + 8) = DAT_8c029cd0;
      *(int *)((int)puVar18 + iVar3) = DAT_8c029cd0 + 0xc;
      *(int *)((int)puVar18 + iVar3 + 4) = DAT_8c029cd0 + 0x18;
      *(undefined4 *)((int)puVar18 + iVar3 + 0x20) = DAT_8c029cd4;
      *(undefined4 *)((int)puVar18 + iVar3 + 0x1c) = DAT_8c029cd4;
      pdVar17 = pdVar8 + 3;
      if (0 < *(int *)((int)puVar18 + iVar3 + 0x10)) {
        puVar36 = PTR_FUN_8c029ce0;
        if (in_FPSCR_SZ == '\0') {
          puVar36 = _DAT_8c029cdc;
        }
        dVar35 = (double)(ZEXT48(puVar36) << 0x20);
        do {
          uVar29 = SUB84(dVar34,0);
          puVar5 = *(undefined8 **)((int)puVar18 + iVar3 + -4);
          uVar31 = SUB84(dVar35,0);
          if (in_FPSCR_SZ == '\0') {
            *(undefined4 *)(puVar5 + 1) = uVar31;
            *(undefined4 *)((int)puVar5 + 4) = uVar31;
            *(undefined4 *)puVar5 = uVar31;
          }
          else {
            puVar5[1] = in_xd14;
            *(undefined8 *)((int)puVar5 + 4) = in_xd14;
            *puVar5 = in_xd14;
          }
          iVar15 = *piVar7;
          if (in_FPSCR_SZ == '\0') {
            param_2 = (double)CONCAT44((int)((ulonglong)param_2 >> 0x20),
                                       *(undefined4 *)(iVar15 + 0x24));
            dVar34 = (double)CONCAT44(*(undefined4 *)(iVar15 + 0x1c),SUB84(param_1,0));
            pdVar12 = (double *)(iVar15 + 0x20);
            dVar32 = (double)CONCAT44(*(undefined4 *)(iVar15 + 0x18),*(undefined4 *)(iVar15 + 0x28))
            ;
            pdVar9 = (double *)(iVar15 + 0x2c);
          }
          else {
            dVar32 = *(double *)(iVar15 + 0x18);
            in_xd8 = *(double *)(iVar15 + 0x24);
            dVar34 = *(double *)(iVar15 + 0x20);
            pdVar12 = (double *)(iVar15 + 0x28);
            in_xd6 = *(double *)(iVar15 + 0x2c);
            pdVar9 = (double *)(iVar15 + 0x34);
          }
          fVar20 = (float)((ulonglong)dVar32 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            fVar24 = SUB84(param_2,0) - fVar20;
          }
          else {
            fVar24 = SUB84(param_2 - dVar32,0);
          }
          if (in_FPSCR_SZ == '\0') {
            dVar34 = (double)CONCAT44((int)((ulonglong)dVar34 >> 0x20),*(undefined4 *)pdVar12);
            dVar33 = (double)CONCAT44(*(undefined4 *)pdVar9,fVar24);
          }
          else {
            in_xd4 = *pdVar12;
            dVar33 = *pdVar9;
          }
          fVar21 = (float)((ulonglong)dVar34 >> 0x20);
          fVar24 = SUB84(dVar34,0);
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44(fVar20,SUB84(dVar32,0) - fVar21);
            dVar33 = (double)CONCAT44((float)((ulonglong)dVar33 >> 0x20) - fVar24,SUB84(dVar33,0));
          }
          else {
            dVar32 = dVar32 - dVar34;
            dVar33 = dVar33 - dVar34;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)pdVar8 + 0x14) = (int)((ulonglong)dVar33 >> 0x20);
            *(int *)(pdVar8 + 2) = SUB84(dVar32,0);
            *(int *)((int)pdVar8 + 0xc) = SUB84(dVar33,0);
            param_2 = (double)CONCAT44(*(undefined4 *)(iVar15 + 0x30),SUB84(dVar33,0));
            pdVar12 = (double *)(iVar15 + 0x34);
          }
          else {
            pdVar8[2] = dVar33;
            pdVar8[1] = in_xd6;
            *pdVar8 = in_xd8;
            param_2 = *(double *)(iVar15 + 0x30);
            pdVar12 = (double *)(iVar15 + 0x38);
          }
          fVar20 = (float)((ulonglong)dVar32 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            param_2 = (double)CONCAT44((float)((ulonglong)param_2 >> 0x20) - fVar20,SUB84(param_2,0)
                                      );
          }
          else {
            param_2 = param_2 - dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar32 = (double)CONCAT44(fVar20,*(undefined4 *)pdVar12);
            param_2 = (double)CONCAT44((int)((ulonglong)param_2 >> 0x20),
                                       *(undefined4 *)((int)pdVar12 + 4));
          }
          else {
            in_xd6 = *pdVar12;
            in_xd8 = pdVar12[1];
          }
          if (in_FPSCR_PR == '\0') {
            param_2 = (double)CONCAT44((int)((ulonglong)param_2 >> 0x20),SUB84(param_2,0) - fVar24);
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),SUB84(dVar32,0) - fVar21);
          }
          else {
            param_2 = param_2 - dVar34;
            dVar32 = dVar32 - dVar34;
          }
          puVar11 = (undefined4 *)((int)puVar18 + (int)DAT_8c029cc8 + iVar3 + -4);
          if (in_FPSCR_SZ == '\0') {
            *(int *)(pdVar8 + 4) = SUB84(param_2,0);
            *(int *)((int)pdVar8 + 0x1c) = SUB84(dVar32,0);
            *(int *)(pdVar8 + 3) = (int)((ulonglong)param_2 >> 0x20);
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                      *(undefined4 *)((int)param_4 + 4));
          }
          else {
            *(double *)((int)pdVar8 + 0x1c) = in_xd8;
            *(double *)((int)pdVar8 + 0x14) = in_xd6;
            *(double *)((int)pdVar8 + 0xc) = param_2;
            in_xd2 = *(double *)((int)param_4 + 4);
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                      SUB84(dVar25,0) - (float)((ulonglong)dVar32 >> 0x20));
          }
          else {
            dVar25 = dVar25 - dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)puVar18 + (int)DAT_8c029cc2 + iVar3 + -4) = SUB84(dVar25,0);
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)(param_4 + 1))
            ;
          }
          else {
            *(double *)((int)puVar18 + (int)DAT_8c029cc2 + iVar3 + -4) = in_xd2;
            in_xd2 = param_4[1];
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),SUB84(dVar25,0) - fVar21);
          }
          else {
            dVar25 = dVar25 - dVar34;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)puVar18 + (int)DAT_8c029cc4 + iVar3 + -4) = SUB84(dVar25,0);
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                      *(undefined4 *)((int)param_4 + 0xc));
          }
          else {
            *(double *)((int)puVar18 + (int)DAT_8c029cc4 + iVar3 + -4) = in_xd2;
            in_xd2 = *(double *)((int)param_4 + 0xc);
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),SUB84(dVar25,0) - fVar24);
          }
          else {
            dVar25 = dVar25 - dVar34;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)puVar18 + (int)DAT_8c029cc6 + iVar3 + -4) = SUB84(dVar25,0);
          }
          else {
            *(double *)((int)puVar18 + (int)DAT_8c029cc6 + iVar3 + -4) = in_xd2;
          }
          *puVar11 = *(undefined4 *)(param_4 + 2);
          puVar11[1] = *(undefined4 *)((int)param_4 + 0x14);
          puVar11[2] = *(undefined4 *)(param_4 + 3);
          (*(code *)PTR_FUN_8c029ce0)(*(undefined4 *)((int)puVar18 + iVar3 + 0x24));
          uVar27 = SUB84(dVar34,0);
          uVar28 = (uint)((ulonglong)dVar25 >> 0x20);
          (*(code *)PTR_FUN_8c029ce4)
                    (*(undefined4 *)((int)puVar18 + iVar3 + -4),pdVar16,pdVar17,
                     (undefined1 *)((int)puVar18 + (int)DAT_8c029cca + iVar3 + -4));
          fVar20 = (float)(*(code *)PTR_FUN_8c029ce8)
                                    ((undefined1 *)((int)puVar18 + (int)DAT_8c029cca + iVar3 + -4));
          dVar25 = (double)((ulonglong)uVar28 << 0x20);
          if (fVar20 == 0.0) {
            dVar25 = 0.0078125;
            iVar15 = (int)DAT_8c029cca + iVar3 + -4;
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)puVar18 + (int)DAT_8c029cca + iVar3 + -4) = SUB84(dVar35,0);
              *(undefined4 *)((int)puVar18 + iVar15 + 4) = 0x3f800000;
            }
            else {
              *(undefined8 *)((int)puVar18 + (int)DAT_8c029cca + iVar3 + -4) = in_xd14;
              *(undefined8 *)((int)puVar18 + iVar15 + 4) = 0x3f80000000000000;
            }
            iVar15 = (int)DAT_8c029cca + iVar3 + -4;
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)puVar18 + iVar15 + 8) = SUB84(dVar35,0);
            }
            else {
              *(undefined8 *)((int)puVar18 + iVar15 + 8) = in_xd14;
            }
          }
          else {
            (*(code *)PTR_FUN_8c029e10)
                      ((undefined1 *)((int)puVar18 + (int)DAT_8c029e00 + iVar3 + -4));
          }
          if (in_FPSCR_SZ != '\0') {
            uVar27 = (uint)*(undefined8 *)((int)puVar18 + (int)DAT_8c029e02 + iVar3 + -4);
          }
          dVar34 = (double)(*(code *)PTR_FUN_8c029e14)();
          if (in_FPSCR_PR == '\0') {
            iVar15 = (int)((float)((ulonglong)dVar34 >> 0x20) * (float)((ulonglong)dVar35 >> 0x20));
          }
          else {
            iVar15 = (int)(dVar34 * dVar35);
          }
          if (iVar15 != 0) {
            if (in_FPSCR_SZ != '\0') {
              uVar27 = (uint)*(undefined8 *)((int)puVar18 + (int)DAT_8c029e02 + iVar3 + -4);
            }
            dVar34 = (double)(*(code *)PTR_FUN_8c029e14)();
            if (in_FPSCR_PR == '\0') {
              iVar15 = (int)((float)((ulonglong)dVar34 >> 0x20) * (float)((ulonglong)dVar35 >> 0x20)
                            );
            }
            else {
              iVar15 = (int)(dVar34 * dVar35);
            }
            (*(code *)PTR_FUN_8c029e18)(0,-iVar15);
          }
          if (in_FPSCR_SZ == '\0') {
            uVar27 = *(uint *)((int)puVar18 + (int)DAT_8c029e04 + iVar3 + -4);
          }
          else {
            in_xd4 = *(double *)((int)puVar18 + (int)DAT_8c029e04 + iVar3 + -4);
          }
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)(ulonglong)uVar27;
          }
          else {
            param_1 = *(double *)((int)puVar18 + (int)DAT_8c029e00 + iVar3 + -4);
          }
          dVar34 = (double)(*(code *)PTR_FUN_8c029e1c)();
          uVar28 = SUB84(param_1,0);
          if (in_FPSCR_PR == '\0') {
            iVar15 = (int)((float)((ulonglong)dVar34 >> 0x20) * (float)((ulonglong)dVar35 >> 0x20));
          }
          else {
            iVar15 = (int)(dVar34 * dVar35);
          }
          if (iVar15 != 0) {
            if (in_FPSCR_SZ == '\0') {
              uVar28 = *(uint *)((int)puVar18 + (int)DAT_8c029e04 + iVar3 + -4);
            }
            else {
              in_xd4 = *(double *)((int)puVar18 + (int)DAT_8c029e04 + iVar3 + -4);
            }
            if (in_FPSCR_SZ == '\0') {
              param_1 = (double)(ulonglong)uVar28;
            }
            else {
              param_1 = *(double *)((int)puVar18 + (int)DAT_8c029e00 + iVar3 + -4);
            }
            dVar34 = (double)(*(code *)PTR_FUN_8c029e1c)();
            if (in_FPSCR_PR == '\0') {
              iVar15 = (int)((float)((ulonglong)dVar34 >> 0x20) * (float)((ulonglong)dVar35 >> 0x20)
                            );
            }
            else {
              iVar15 = (int)(dVar34 * dVar35);
            }
            (*(code *)PTR_FUN_8c029e20)(0,-iVar15);
          }
          (*(code *)puVar2)(0,pdVar16,pdVar16);
          (*(code *)puVar2)(0,pdVar17,pdVar17);
          (*(code *)puVar2)(0,(undefined1 *)((int)puVar18 + (int)DAT_8c029e06 + iVar3 + -4),
                            (undefined1 *)((int)puVar18 + (int)DAT_8c029e06 + iVar3 + -4));
          (*(code *)puVar2)(0,(undefined1 *)((int)puVar18 + (int)DAT_8c029e08 + iVar3 + -4),
                            (undefined1 *)((int)puVar18 + (int)DAT_8c029e08 + iVar3 + -4));
          if (in_FPSCR_SZ == '\0') {
            dVar34 = (double)CONCAT44(*(undefined4 *)param_4,uVar29);
          }
          else {
            dVar34 = *param_4;
          }
          uVar28 = *(uint *)(*piVar7 + 0x3c) & DAT_8c029e24;
          *(uint *)((int)puVar18 + iVar3 + 0x14) = uVar28;
          if (uVar28 != 0) {
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)((ulonglong)*(uint *)((int)puVar18 + (int)DAT_8c029e0a + iVar3 + -4)
                               << 0x20);
            }
            else {
              dVar25 = *(double *)((int)puVar18 + (int)DAT_8c029e0a + iVar3 + -4);
            }
            if (in_FPSCR_PR == '\0' && (float)((ulonglong)dVar25 >> 0x20) < SUB84(dVar25,0)) {
              if (in_FPSCR_SZ == '\0') {
                dVar25 = (double)CONCAT44(DAT_8c029e28,SUB84(dVar25,0));
              }
              else {
                dVar25 = (double)CONCAT44(PTR_FUN_8c029e2c,DAT_8c029e28);
              }
              if (in_FPSCR_PR == '\0') {
                dVar34 = (double)CONCAT44((float)((ulonglong)dVar34 >> 0x20) *
                                          (float)((ulonglong)dVar25 >> 0x20),SUB84(dVar34,0));
              }
              else {
                dVar34 = dVar34 * dVar25;
              }
            }
          }
          uVar28 = (uint)((ulonglong)dVar25 >> 0x20);
          puVar6 = (undefined4 *)((int)puVar18 + (int)DAT_8c029e08 + iVar3 + -4);
          *(undefined4 *)((int)puVar18 + iVar3 + 0x28) = 0;
          puVar11 = (undefined4 *)((int)puVar18 + (int)DAT_8c029e0c + iVar3 + -4);
          *puVar11 = *puVar6;
          puVar11[1] = puVar6[1];
          puVar11[2] = puVar6[2];
          dVar33 = (double)(*(code *)PTR_FUN_8c029e2c)
                                     ((undefined1 *)((int)puVar18 + (int)DAT_8c029e0c + iVar3 + -4))
          ;
          dVar25 = (double)((ulonglong)uVar28 << 0x20);
          if ((float)((ulonglong)dVar33 >> 0x20) == 0.0) {
            iVar15 = (int)DAT_8c029e0c + iVar3 + -4;
            uVar29 = SUB84(dVar35,0);
            if (in_FPSCR_SZ == '\0') {
              *(undefined4 *)((int)puVar18 + iVar15 + 8) = uVar29;
            }
            else {
              *(undefined8 *)((int)puVar18 + iVar15 + 8) = in_xd14;
            }
            iVar15 = (int)DAT_8c029e0c + iVar3 + -4;
            if (in_FPSCR_SZ == '\0') {
              *(undefined4 *)((int)puVar18 + iVar15 + 4) = uVar29;
            }
            else {
              *(undefined8 *)((int)puVar18 + iVar15 + 4) = in_xd14;
            }
            if (in_FPSCR_SZ == '\0') {
              *(undefined4 *)((int)puVar18 + (int)DAT_8c029e0c + iVar3 + -4) = uVar29;
            }
            else {
              *(undefined8 *)((int)puVar18 + (int)DAT_8c029e0c + iVar3 + -4) = in_xd14;
            }
          }
          else {
            dVar33 = (double)(*(code *)PTR_FUN_8c029f98)
                                       ((undefined1 *)
                                        ((int)puVar18 + (int)DAT_8c029f80 + iVar3 + -4));
          }
          if (in_FPSCR_SZ == '\0') {
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                      *(undefined4 *)((int)puVar18 + (int)DAT_8c029f80 + iVar3 + -4)
                                     );
          }
          else {
            in_xd2 = *(double *)((int)puVar18 + (int)DAT_8c029f80 + iVar3 + -4);
          }
          puVar11 = (undefined4 *)((int)puVar18 + (int)DAT_8c029f86 + iVar3 + -4);
          fVar20 = SUB84(dVar34,0);
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),SUB84(dVar25,0) * fVar20);
          }
          else {
            dVar25 = dVar25 * dVar34;
          }
          if (in_FPSCR_SZ == '\0') {
            param_3 = (double)CONCAT44((int)((ulonglong)param_3 >> 0x20),SUB84(dVar35,0));
          }
          puVar6 = (undefined4 *)((int)puVar18 + (int)DAT_8c029f88 + iVar3 + -4);
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)puVar18 + (int)DAT_8c029f80 + iVar3 + -4) = SUB84(dVar25,0);
          }
          else {
            *(double *)((int)puVar18 + (int)DAT_8c029f80 + iVar3 + -4) = in_xd2;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                      *(undefined4 *)((int)puVar18 + (int)DAT_8c029f82 + iVar3 + -4)
                                     );
          }
          else {
            in_xd2 = *(double *)((int)puVar18 + (int)DAT_8c029f82 + iVar3 + -4);
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),SUB84(dVar25,0) * fVar20);
          }
          else {
            dVar25 = dVar25 * dVar34;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)puVar18 + (int)DAT_8c029f82 + iVar3 + -4) = SUB84(dVar25,0);
          }
          else {
            *(double *)((int)puVar18 + (int)DAT_8c029f82 + iVar3 + -4) = in_xd2;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                      *(undefined4 *)((int)puVar18 + (int)DAT_8c029f84 + iVar3 + -4)
                                     );
          }
          else {
            in_xd2 = *(double *)((int)puVar18 + (int)DAT_8c029f84 + iVar3 + -4);
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),SUB84(dVar25,0) * fVar20);
          }
          else {
            dVar25 = dVar25 * dVar34;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)puVar18 + (int)DAT_8c029f84 + iVar3 + -4) = SUB84(dVar25,0);
          }
          else {
            *(double *)((int)puVar18 + (int)DAT_8c029f84 + iVar3 + -4) = in_xd2;
          }
          *puVar6 = *puVar11;
          puVar6[1] = puVar11[1];
          puVar6[2] = puVar11[2];
LAB_8c029e7c:
          uVar31 = SUB84(dVar32,0);
          uVar29 = SUB84(param_1,0);
          if (in_FPSCR_SZ == '\0') {
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                      *(undefined4 *)((int)puVar18 + iVar3 + 0x18));
          }
          else {
            in_xd2 = *(double *)((int)puVar18 + iVar3 + 0x18);
          }
          if (in_FPSCR_PR == '\0') {
            bVar39 = SUB84(param_3,0) < SUB84(dVar25,0);
          }
          else {
            bVar39 = param_3 < dVar25;
          }
          bVar1 = true;
          if (!bVar39) {
            bVar1 = false;
            puVar11 = (undefined4 *)((int)puVar18 + (int)DAT_8c029f86 + iVar3 + -4);
            puVar6 = (undefined4 *)((int)puVar18 + (int)DAT_8c029f88 + iVar3 + -4);
            *puVar6 = *puVar11;
            puVar6[1] = puVar11[1];
            puVar6[2] = puVar11[2];
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44(*(undefined4 *)
                                         ((int)puVar18 + (int)DAT_8c029f8a + iVar3 + -4),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = *(double *)((int)puVar18 + (int)DAT_8c029f8a + iVar3 + -4);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),
                                        *(undefined4 *)
                                         ((int)puVar18 + (int)DAT_8c029f86 + iVar3 + -4));
            }
            else {
              in_xd0 = *(double *)((int)puVar18 + (int)DAT_8c029f86 + iVar3 + -4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),
                                        SUB84(dVar33,0) + (float)((ulonglong)dVar25 >> 0x20));
            }
            else {
              dVar33 = dVar33 + dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)puVar18 + (int)DAT_8c029f88 + iVar3 + -4) = SUB84(dVar33,0);
            }
            else {
              *(double *)((int)puVar18 + (int)DAT_8c029f88 + iVar3 + -4) = in_xd0;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44(*(undefined4 *)
                                         ((int)puVar18 + (int)DAT_8c029f8c + iVar3 + -4),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = *(double *)((int)puVar18 + (int)DAT_8c029f8c + iVar3 + -4);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),
                                        *(undefined4 *)
                                         ((int)puVar18 + (int)DAT_8c029f8e + iVar3 + -4));
            }
            else {
              in_xd0 = *(double *)((int)puVar18 + (int)DAT_8c029f8e + iVar3 + -4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),
                                        SUB84(dVar33,0) + (float)((ulonglong)dVar25 >> 0x20));
            }
            else {
              dVar33 = dVar33 + dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)puVar18 + (int)DAT_8c029f90 + iVar3 + -4) = SUB84(dVar33,0);
            }
            else {
              *(double *)((int)puVar18 + (int)DAT_8c029f90 + iVar3 + -4) = in_xd0;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44(*(undefined4 *)
                                         ((int)puVar18 + (int)DAT_8c029f92 + iVar3 + -4),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = *(double *)((int)puVar18 + (int)DAT_8c029f92 + iVar3 + -4);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),
                                        *(undefined4 *)
                                         ((int)puVar18 + (int)DAT_8c029f94 + iVar3 + -4));
            }
            else {
              in_xd0 = *(double *)((int)puVar18 + (int)DAT_8c029f94 + iVar3 + -4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),
                                        SUB84(dVar33,0) + (float)((ulonglong)dVar25 >> 0x20));
            }
            else {
              dVar33 = dVar33 + dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)puVar18 + (int)DAT_8c029f96 + iVar3 + -4) = SUB84(dVar33,0);
            }
            else {
              *(double *)((int)puVar18 + (int)DAT_8c029f96 + iVar3 + -4) = in_xd0;
            }
          }
          if (in_FPSCR_SZ == '\0') {
            dVar25 = (double)CONCAT44(*(undefined4 *)param_4,
                                      *(undefined4 *)((int)puVar18 + (int)DAT_8c029f90 + iVar3 + -4)
                                     );
          }
          else {
            dVar25 = *param_4;
            in_xd2 = *(double *)((int)puVar18 + (int)DAT_8c029f90 + iVar3 + -4);
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),ABS(SUB84(dVar25,0)));
          }
          else {
            dVar25 = ABS(dVar25);
          }
          fVar24 = (float)((ulonglong)dVar25 >> 0x20);
          if (in_FPSCR_PR == '\0' && fVar24 < SUB84(dVar25,0)) {
LAB_8c029fa0:
            if (!bVar1) goto LAB_8c02a000;
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)(ulonglong)*(uint *)((int)puVar18 + (int)DAT_8c029fe0 + iVar3 + -4);
            }
            else {
              in_xd2 = *(double *)((int)puVar18 + (int)DAT_8c029fe0 + iVar3 + -4);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44(*(undefined4 *)
                                         ((int)puVar18 + (int)DAT_8c029fe2 + iVar3 + -4),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = *(double *)((int)puVar18 + (int)DAT_8c029fe2 + iVar3 + -4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = dVar25 + dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)puVar18 + (int)DAT_8c029fe2 + iVar3 + -4) =
                   (int)((ulonglong)dVar25 >> 0x20);
            }
            else {
              *(double *)((int)puVar18 + (int)DAT_8c029fe2 + iVar3 + -4) = dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)(ulonglong)*(uint *)((int)puVar18 + (int)DAT_8c029fe4 + iVar3 + -4);
            }
            else {
              in_xd2 = *(double *)((int)puVar18 + (int)DAT_8c029fe4 + iVar3 + -4);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44(*(undefined4 *)
                                         ((int)puVar18 + (int)DAT_8c029fe6 + iVar3 + -4),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = *(double *)((int)puVar18 + (int)DAT_8c029fe6 + iVar3 + -4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = dVar25 + dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)puVar18 + (int)DAT_8c029fe6 + iVar3 + -4) =
                   (int)((ulonglong)dVar25 >> 0x20);
            }
            else {
              *(double *)((int)puVar18 + (int)DAT_8c029fe6 + iVar3 + -4) = dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)(ulonglong)*(uint *)((int)puVar18 + (int)DAT_8c029fe8 + iVar3 + -4);
            }
            else {
              in_xd2 = *(double *)((int)puVar18 + (int)DAT_8c029fe8 + iVar3 + -4);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44(*(undefined4 *)
                                         ((int)puVar18 + (int)DAT_8c029fea + iVar3 + -4),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = *(double *)((int)puVar18 + (int)DAT_8c029fea + iVar3 + -4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = dVar25 + dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)puVar18 + (int)DAT_8c029fea + iVar3 + -4) =
                   (int)((ulonglong)dVar25 >> 0x20);
            }
            else {
              *(double *)((int)puVar18 + (int)DAT_8c029fea + iVar3 + -4) = dVar25;
            }
            if (in_FPSCR_PR == '\0') {
              param_3 = (double)CONCAT44((int)((ulonglong)param_3 >> 0x20),SUB84(param_3,0) + fVar20
                                        );
            }
            else {
              param_3 = param_3 + dVar34;
            }
            goto LAB_8c029e7c;
          }
          if (in_FPSCR_SZ == '\0') {
            uVar29 = *(undefined4 *)((int)pdVar8 + 0x14);
            uVar31 = *(undefined4 *)(*(int *)((int)puVar18 + iVar3 + -4) + 8);
            dVar38 = in_xd4;
          }
          else {
            dVar38 = *(double *)((int)pdVar8 + 0x14);
            in_xd6 = *(double *)(*(int *)((int)puVar18 + iVar3 + -4) + 8);
          }
          if (in_FPSCR_SZ == '\0') {
            dVar25 = (double)CONCAT44(fVar24,uVar29);
            dVar32 = (double)CONCAT44(*(undefined4 *)*(double **)((int)puVar18 + iVar3 + -4),uVar31)
            ;
          }
          else {
            dVar32 = **(double **)((int)puVar18 + iVar3 + -4);
            in_xd2 = dVar38;
          }
          fVar24 = SUB84(dVar32,0);
          if (in_FPSCR_PR == '\0') {
            fVar21 = SUB84(dVar25,0) - fVar24;
          }
          else {
            fVar21 = SUB84(dVar25 - dVar32,0);
          }
          if (in_FPSCR_SZ == '\0') {
            dVar25 = (double)CONCAT44(*(undefined4 *)((int)puVar18 + (int)DAT_8c029f88 + iVar3 + -4)
                                      ,fVar21);
          }
          else {
            dVar25 = *(double *)((int)puVar18 + (int)DAT_8c029f88 + iVar3 + -4);
          }
          fVar21 = (float)((ulonglong)dVar32 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) - fVar21,SUB84(dVar25,0));
          }
          else {
            dVar25 = dVar25 - dVar32;
          }
          if (in_FPSCR_SZ == '\0') {
            param_2 = (double)(ulonglong)*(uint *)pdVar16;
          }
          else {
            in_xd8 = *pdVar16;
          }
          iVar15 = (int)DAT_8c029f88 + iVar3 + -4;
          if (in_FPSCR_SZ == '\0') {
            dVar30 = (double)CONCAT44(*(undefined4 *)(pdVar8 + 4),uVar29);
            dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),SUB84(param_2,0));
          }
          else {
            dVar30 = pdVar8[4];
            in_xd0 = in_xd8;
          }
          if (in_FPSCR_PR == '\0') {
            dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),SUB84(dVar33,0) - fVar21);
            fVar22 = SUB84(dVar25,0) * (float)((ulonglong)dVar25 >> 0x20);
          }
          else {
            dVar33 = dVar33 - dVar32;
            fVar22 = SUB84(dVar25 * dVar25,0);
          }
          if (in_FPSCR_SZ == '\0') {
            dVar25 = (double)CONCAT44(*(undefined4 *)((int)puVar18 + iVar15 + 8),fVar22);
          }
          else {
            dVar25 = *(double *)((int)puVar18 + iVar15 + 8);
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) - fVar24,SUB84(dVar25,0));
          }
          else {
            dVar25 = dVar25 - dVar32;
          }
          fVar22 = (float)((ulonglong)dVar30 >> 0x20);
          dVar37 = dVar30;
          if (in_FPSCR_SZ == '\0') {
            uVar29 = SUB84(dVar25,0);
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),fVar22);
            dVar37 = in_xd2;
            in_xd2 = (double)CONCAT44(uVar29,SUB84(param_2,0));
          }
          if (in_FPSCR_PR == '\0') {
            fVar23 = (float)((ulonglong)dVar25 >> 0x20);
            dVar25 = (double)CONCAT44(fVar23,SUB84(dVar25,0) - SUB84(dVar30,0));
            dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),SUB84(dVar33,0) * fVar23);
          }
          else {
            dVar25 = dVar25 - dVar30;
            dVar33 = dVar33 * dVar25;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar25 = (double)CONCAT44(*(undefined4 *)((int)puVar18 + (int)DAT_8c029f88 + iVar3 + -4)
                                      ,SUB84(dVar25,0));
          }
          else {
            dVar25 = *(double *)((int)puVar18 + (int)DAT_8c029f88 + iVar3 + -4);
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) - SUB84(in_xd2,0),
                                      SUB84(dVar25,0));
            param_2 = (double)CONCAT44((float)((ulonglong)in_xd2 >> 0x20) - SUB84(dVar33,0),
                                       SUB84(in_xd2,0));
          }
          else {
            dVar25 = dVar25 - in_xd2;
            param_2 = in_xd2 - dVar33;
          }
          if (in_FPSCR_PR == '\0') {
            fVar23 = SUB84(dVar25,0) * (float)((ulonglong)dVar25 >> 0x20);
          }
          else {
            fVar23 = SUB84(dVar25 * dVar25,0);
          }
          if (in_FPSCR_SZ == '\0') {
            param_3 = (double)CONCAT44(*(undefined4 *)pdVar17,SUB84(param_3,0));
          }
          else {
            param_3 = *pdVar17;
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44(fVar21,fVar24 - fVar22);
          }
          else {
            dVar32 = dVar32 - dVar30;
          }
          iVar15 = (int)DAT_8c029f88 + iVar3 + -4;
          if (in_FPSCR_SZ == '\0') {
            dVar26 = (double)CONCAT44(*(undefined4 *)((int)puVar18 + iVar15 + 8),fVar23);
          }
          else {
            dVar26 = *(double *)((int)puVar18 + iVar15 + 8);
          }
          fVar24 = (float)((ulonglong)param_3 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) - fVar24,SUB84(dVar32,0));
          }
          else {
            dVar32 = dVar32 - param_3;
          }
          if (in_FPSCR_PR == '\0') {
            dVar26 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) - SUB84(dVar30,0),
                                      SUB84(dVar26,0));
          }
          else {
            dVar26 = dVar26 - dVar30;
          }
          in_xd4 = dVar37;
          if (in_FPSCR_SZ == '\0') {
            dVar30 = (double)CONCAT44(fVar22,SUB84(dVar26,0));
            in_xd4 = dVar38;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),fVar24);
            dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),
                                      *(undefined4 *)((int)puVar18 + (int)DAT_8c029f88 + iVar3 + -4)
                                     );
          }
          else {
            dVar37 = *(double *)((int)puVar18 + (int)DAT_8c029f88 + iVar3 + -4);
            in_xd0 = param_3;
          }
          if (in_FPSCR_PR == '\0') {
            dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),
                                      SUB84(dVar33,0) - SUB84(param_2,0));
          }
          else {
            dVar33 = dVar33 - param_2;
          }
          if (in_FPSCR_PR == '\0') {
            dVar26 = (double)CONCAT44((int)((ulonglong)dVar26 >> 0x20),SUB84(dVar26,0) - fVar24);
          }
          else {
            dVar26 = dVar26 - param_3;
          }
          iVar15 = (int)DAT_8c029f88 + iVar3 + -4;
          if (in_FPSCR_PR == '\0') {
            dVar33 = (double)CONCAT44((int)((ulonglong)dVar33 >> 0x20),
                                      SUB84(dVar33,0) * (float)((ulonglong)dVar26 >> 0x20));
          }
          else {
            dVar33 = dVar33 * dVar26;
          }
          dVar25 = (double)((ulonglong)dVar26 & 0xffffffff);
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((int)((ulonglong)dVar32 >> 0x20),
                                      SUB84(dVar32,0) * SUB84(dVar26,0));
          }
          else {
            dVar32 = dVar32 * dVar25;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar25 = (double)(ulonglong)*(uint *)((int)puVar18 + iVar15 + 8);
          }
          else {
            dVar37 = *(double *)((int)puVar18 + iVar15 + 8);
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)(ulonglong)(uint)(SUB84(dVar25,0) - (float)((ulonglong)dVar30 >> 0x20))
            ;
            fVar24 = SUB84(dVar30,0) - SUB84(dVar33,0);
          }
          else {
            dVar25 = dVar25 - dVar30;
            fVar24 = SUB84(dVar30 - dVar33,0);
          }
          param_1 = in_xd6;
          if (in_FPSCR_SZ == '\0') {
            param_1 = (double)CONCAT44(SUB84(dVar32,0),fVar24);
          }
          if (in_FPSCR_PR == '\0') {
            dVar32 = (double)CONCAT44((float)((ulonglong)dVar32 >> 0x20) * SUB84(dVar25,0),
                                      SUB84(dVar32,0));
          }
          else {
            dVar32 = dVar32 * dVar25;
          }
          uVar29 = (undefined4)((ulonglong)param_2 >> 0x20);
          in_xd2 = param_2;
          if (in_FPSCR_SZ == '\0') {
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),uVar29);
            in_xd2 = dVar37;
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                      SUB84(dVar25,0) * SUB84(param_1,0));
          }
          else {
            dVar25 = dVar25 * param_1;
          }
          fVar24 = (float)((ulonglong)dVar25 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            param_1 = (double)CONCAT44((float)((ulonglong)param_1 >> 0x20) -
                                       (float)((ulonglong)dVar32 >> 0x20),SUB84(param_1,0));
          }
          else {
            param_1 = param_1 - dVar32;
          }
          if (in_FPSCR_PR == '\0' && SUB84(dVar25,0) < fVar24) goto LAB_8c029fa0;
          dVar38 = in_xd4;
          if (in_FPSCR_SZ == '\0') {
            dVar25 = (double)CONCAT44(fVar24,SUB84(param_1,0));
            dVar38 = in_xd2;
          }
          fVar24 = (float)((ulonglong)param_1 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),SUB84(dVar25,0) * fVar24);
          }
          else {
            dVar25 = dVar25 * param_1;
          }
          fVar21 = (float)((ulonglong)dVar25 >> 0x20);
          in_xd2 = dVar38;
          if (in_FPSCR_PR == '\0' && SUB84(dVar25,0) < fVar21) goto LAB_8c029fa0;
          in_xd2 = param_2;
          if (in_FPSCR_SZ == '\0') {
            dVar25 = (double)CONCAT44(fVar21,uVar29);
            in_xd2 = dVar38;
          }
          if (in_FPSCR_PR == '\0') {
            fVar24 = SUB84(dVar25,0) * fVar24;
          }
          else {
            fVar24 = SUB84(dVar25 * param_1,0);
          }
          dVar25 = (double)(ulonglong)(uint)fVar24;
          if (in_FPSCR_PR == '\0' && fVar24 < 0.0) goto LAB_8c029fa0;
          *(undefined4 *)((int)puVar18 + iVar3 + 0x28) = 1;
LAB_8c02a000:
          if (*(int *)((int)puVar18 + iVar3 + 0x28) != 0) {
            if (-1 < *DAT_8c02a14c) {
              iVar15 = *piVar7;
              puVar11 = *(undefined4 **)((int)puVar18 + iVar3 + 8);
              *puVar11 = *(undefined4 *)(iVar15 + 0x18);
              puVar11[1] = *(undefined4 *)(iVar15 + 0x1c);
              puVar11[2] = *(undefined4 *)(iVar15 + 0x20);
              iVar15 = *piVar7;
              puVar11 = *(undefined4 **)((int)puVar18 + iVar3);
              *puVar11 = *(undefined4 *)(iVar15 + 0x24);
              puVar11[1] = *(undefined4 *)(iVar15 + 0x28);
              puVar11[2] = *(undefined4 *)(iVar15 + 0x2c);
              iVar15 = *piVar7;
              puVar11 = *(undefined4 **)((int)puVar18 + iVar3 + 4);
              *puVar11 = *(undefined4 *)(iVar15 + 0x30);
              puVar11[1] = *(undefined4 *)(iVar15 + 0x34);
              puVar11[2] = *(undefined4 *)(iVar15 + 0x38);
              pdVar12 = *(double **)((int)puVar18 + iVar3 + 8);
              if (in_FPSCR_SZ == '\0') {
                dVar25 = (double)CONCAT44(*(undefined4 *)pdVar12,
                                          *(undefined4 *)
                                           ((int)puVar18 + (int)DAT_8c02a140 + iVar3 + -4));
              }
              else {
                in_xd2 = *(double *)((int)puVar18 + (int)DAT_8c02a140 + iVar3 + -4);
                dVar25 = *pdVar12;
              }
              iVar15 = (int)DAT_8c02a140;
              if (in_FPSCR_PR == '\0') {
                dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                          SUB84(dVar25,0));
              }
              else {
                dVar25 = dVar25 + dVar25;
              }
              if (in_FPSCR_SZ == '\0') {
                *(int *)pdVar12 = (int)((ulonglong)dVar25 >> 0x20);
              }
              else {
                *pdVar12 = dVar25;
              }
              pdVar12 = *(double **)((int)puVar18 + iVar3);
              if (in_FPSCR_SZ == '\0') {
                dVar25 = (double)CONCAT44(*(undefined4 *)pdVar12,
                                          *(undefined4 *)((int)puVar18 + iVar15 + iVar3 + -4));
              }
              else {
                dVar25 = *pdVar12;
                in_xd2 = *(double *)((int)puVar18 + iVar15 + iVar3 + -4);
              }
              iVar15 = (int)DAT_8c02a140;
              if (in_FPSCR_PR == '\0') {
                dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                          SUB84(dVar25,0));
              }
              else {
                dVar25 = dVar25 + dVar25;
              }
              iVar4 = (int)DAT_8c02a142;
              if (in_FPSCR_SZ == '\0') {
                *(int *)pdVar12 = (int)((ulonglong)dVar25 >> 0x20);
              }
              else {
                *pdVar12 = dVar25;
              }
              pdVar12 = *(double **)((int)puVar18 + iVar3 + 4);
              if (in_FPSCR_SZ == '\0') {
                dVar25 = (double)CONCAT44(*(undefined4 *)pdVar12,
                                          *(undefined4 *)((int)puVar18 + iVar15 + iVar3 + -4));
              }
              else {
                in_xd2 = *(double *)((int)puVar18 + iVar15 + iVar3 + -4);
                dVar25 = *pdVar12;
              }
              if (in_FPSCR_PR == '\0') {
                dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                          SUB84(dVar25,0));
              }
              else {
                dVar25 = dVar25 + dVar25;
              }
              if (in_FPSCR_SZ == '\0') {
                *(int *)pdVar12 = (int)((ulonglong)dVar25 >> 0x20);
              }
              else {
                *pdVar12 = dVar25;
              }
              iVar15 = *(int *)((int)puVar18 + iVar3 + 8);
              if (in_FPSCR_SZ == '\0') {
                dVar25 = (double)CONCAT44(*(undefined4 *)(iVar15 + 4),
                                          *(undefined4 *)((int)puVar18 + iVar4 + iVar3 + -4));
              }
              else {
                in_xd2 = *(double *)((int)puVar18 + iVar4 + iVar3 + -4);
                dVar25 = *(double *)(iVar15 + 4);
              }
              iVar4 = (int)DAT_8c02a142;
              if (in_FPSCR_PR == '\0') {
                dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                          SUB84(dVar25,0));
              }
              else {
                dVar25 = dVar25 + dVar25;
              }
              if (in_FPSCR_SZ == '\0') {
                *(int *)(iVar15 + 4) = (int)((ulonglong)dVar25 >> 0x20);
              }
              else {
                *(double *)(iVar15 + 4) = dVar25;
              }
              iVar15 = *(int *)((int)puVar18 + iVar3);
              if (in_FPSCR_SZ == '\0') {
                dVar25 = (double)CONCAT44(*(undefined4 *)(iVar15 + 4),
                                          *(undefined4 *)((int)puVar18 + iVar4 + iVar3 + -4));
              }
              else {
                in_xd2 = *(double *)((int)puVar18 + iVar4 + iVar3 + -4);
                dVar25 = *(double *)(iVar15 + 4);
              }
              iVar4 = (int)DAT_8c02a142;
              if (in_FPSCR_PR == '\0') {
                dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                          SUB84(dVar25,0));
              }
              else {
                dVar25 = dVar25 + dVar25;
              }
              if (in_FPSCR_SZ == '\0') {
                *(int *)(iVar15 + 4) = (int)((ulonglong)dVar25 >> 0x20);
              }
              else {
                *(double *)(iVar15 + 4) = dVar25;
              }
              iVar15 = *(int *)((int)puVar18 + iVar3 + 4);
              if (in_FPSCR_SZ == '\0') {
                dVar25 = (double)CONCAT44(*(undefined4 *)(iVar15 + 4),
                                          *(undefined4 *)((int)puVar18 + iVar4 + iVar3 + -4));
              }
              else {
                in_xd2 = *(double *)((int)puVar18 + iVar4 + iVar3 + -4);
                dVar25 = *(double *)(iVar15 + 4);
              }
              if (in_FPSCR_PR == '\0') {
                dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                          SUB84(dVar25,0));
              }
              else {
                dVar25 = dVar25 + dVar25;
              }
              iVar4 = (int)DAT_8c02a144;
              if (in_FPSCR_SZ == '\0') {
                *(int *)(iVar15 + 4) = (int)((ulonglong)dVar25 >> 0x20);
              }
              else {
                *(double *)(iVar15 + 4) = dVar25;
              }
              iVar15 = *(int *)((int)puVar18 + iVar3 + 8);
              if (in_FPSCR_SZ == '\0') {
                dVar25 = (double)CONCAT44(*(undefined4 *)(iVar15 + 8),
                                          *(undefined4 *)((int)puVar18 + iVar4 + iVar3 + -4));
              }
              else {
                in_xd2 = *(double *)((int)puVar18 + iVar4 + iVar3 + -4);
                dVar25 = *(double *)(iVar15 + 8);
              }
              iVar4 = (int)DAT_8c02a144;
              if (in_FPSCR_PR == '\0') {
                dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                          SUB84(dVar25,0));
              }
              else {
                dVar25 = dVar25 + dVar25;
              }
              if (in_FPSCR_SZ == '\0') {
                *(int *)(iVar15 + 8) = (int)((ulonglong)dVar25 >> 0x20);
              }
              else {
                *(double *)(iVar15 + 8) = dVar25;
              }
              iVar15 = *(int *)((int)puVar18 + iVar3);
              if (in_FPSCR_SZ == '\0') {
                dVar25 = (double)CONCAT44(*(undefined4 *)(iVar15 + 8),
                                          *(undefined4 *)((int)puVar18 + iVar4 + iVar3 + -4));
              }
              else {
                in_xd2 = *(double *)((int)puVar18 + iVar4 + iVar3 + -4);
                dVar25 = *(double *)(iVar15 + 8);
              }
              iVar4 = (int)DAT_8c02a144;
              if (in_FPSCR_PR == '\0') {
                dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                          SUB84(dVar25,0));
              }
              else {
                dVar25 = dVar25 + dVar25;
              }
              if (in_FPSCR_SZ == '\0') {
                *(int *)(iVar15 + 8) = (int)((ulonglong)dVar25 >> 0x20);
              }
              else {
                *(double *)(iVar15 + 8) = dVar25;
              }
              iVar15 = *(int *)((int)puVar18 + iVar3 + 4);
              if (in_FPSCR_SZ == '\0') {
                dVar25 = (double)CONCAT44(*(undefined4 *)(iVar15 + 8),
                                          *(undefined4 *)((int)puVar18 + iVar4 + iVar3 + -4));
              }
              else {
                in_xd2 = *(double *)((int)puVar18 + iVar4 + iVar3 + -4);
                dVar25 = *(double *)(iVar15 + 8);
              }
              if (in_FPSCR_PR == '\0') {
                dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                          SUB84(dVar25,0));
              }
              else {
                dVar25 = dVar25 + dVar25;
              }
              if (in_FPSCR_SZ == '\0') {
                *(int *)(iVar15 + 8) = (int)((ulonglong)dVar25 >> 0x20);
              }
              else {
                *(double *)(iVar15 + 8) = dVar25;
              }
              (*(code *)PTR_FUN_8c02a150)(*(undefined4 *)((int)puVar18 + iVar3 + 0x20));
              (*(code *)PTR_FUN_8c02a154)(1);
              (*(code *)PTR_FUN_8c02a15c)(PTR_DAT_8c02a158,3,0x20);
              (*(code *)PTR_FUN_8c02a160)(*(undefined4 *)((int)puVar18 + iVar3 + 0x1c));
            }
            uVar29 = SUB84(dVar25,0);
            if (in_FPSCR_SZ == '\0') {
              uVar29 = *(undefined4 *)((int)puVar18 + (int)DAT_8c02a146 + iVar3 + -4);
            }
            else {
              in_xd2 = *(double *)((int)puVar18 + (int)DAT_8c02a146 + iVar3 + -4);
            }
            puVar11 = (undefined4 *)((int)puVar18 + (int)DAT_8c02a140 + iVar3 + -4);
            if (in_FPSCR_SZ == '\0') {
              *(undefined4 *)((int)puVar18 + iVar3 + 0x88) = uVar29;
            }
            else {
              *(double *)((int)puVar18 + iVar3 + 0x88) = in_xd2;
            }
            if (in_FPSCR_SZ == '\0') {
              uVar29 = *(undefined4 *)((int)puVar18 + (int)DAT_8c02a148 + iVar3 + -4);
            }
            else {
              in_xd2 = *(double *)((int)puVar18 + (int)DAT_8c02a148 + iVar3 + -4);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44(*(undefined4 *)
                                         ((int)puVar18 + (int)DAT_8c02a146 + iVar3 + -4),uVar29);
            }
            else {
              dVar25 = *(double *)((int)puVar18 + (int)DAT_8c02a146 + iVar3 + -4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = dVar25 + dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)puVar18 + iVar3 + 0x8c) = (int)((ulonglong)dVar25 >> 0x20);
            }
            else {
              *(double *)((int)puVar18 + iVar3 + 0x8c) = dVar25;
            }
            *(undefined4 *)((int)puVar18 + iVar3 + 0x40) = *puVar11;
            *(undefined4 *)((int)puVar18 + iVar3 + 0x44) = puVar11[1];
            *(undefined4 *)((int)puVar18 + iVar3 + 0x48) = puVar11[2];
            if (*(int *)((int)puVar18 + iVar3 + 0x14) == 0) {
LAB_8c02a180:
              puVar11 = (undefined4 *)((int)puVar18 + (int)DAT_8c02a280 + iVar3 + -4);
              *(undefined4 *)((int)puVar18 + iVar3 + 0x4c) = *puVar11;
              *(undefined4 *)((int)puVar18 + iVar3 + 0x50) = puVar11[1];
              *(undefined4 *)((int)puVar18 + iVar3 + 0x54) = puVar11[2];
            }
            else {
              if (in_FPSCR_SZ == '\0') {
                dVar25 = (double)((ulonglong)
                                  *(uint *)((int)puVar18 + (int)DAT_8c02a146 + iVar3 + -4) << 0x20);
              }
              else {
                dVar25 = *(double *)((int)puVar18 + (int)DAT_8c02a146 + iVar3 + -4);
              }
              if (in_FPSCR_PR != '\0' || SUB84(dVar25,0) <= (float)((ulonglong)dVar25 >> 0x20))
              goto LAB_8c02a180;
              if (in_FPSCR_SZ == '\0') {
                dVar25 = (double)CONCAT44(*(undefined4 *)
                                           ((int)puVar18 + (int)DAT_8c02a140 + iVar3 + -4),
                                          SUB84(dVar25,0));
              }
              else {
                dVar25 = *(double *)((int)puVar18 + (int)DAT_8c02a140 + iVar3 + -4);
              }
              if (in_FPSCR_PR == '\0') {
                dVar25 = (double)CONCAT44(-(float)((ulonglong)dVar25 >> 0x20),SUB84(dVar25,0));
              }
              else {
                dVar25 = -dVar25;
              }
              if (in_FPSCR_SZ == '\0') {
                *(int *)((int)puVar18 + iVar3 + 0x4c) = (int)((ulonglong)dVar25 >> 0x20);
              }
              else {
                *(double *)((int)puVar18 + iVar3 + 0x4c) = dVar25;
              }
              if (in_FPSCR_SZ == '\0') {
                dVar25 = (double)CONCAT44(*(undefined4 *)
                                           ((int)puVar18 + (int)DAT_8c02a142 + iVar3 + -4),
                                          SUB84(dVar25,0));
              }
              else {
                dVar25 = *(double *)((int)puVar18 + (int)DAT_8c02a142 + iVar3 + -4);
              }
              if (in_FPSCR_PR == '\0') {
                dVar25 = (double)CONCAT44(-(float)((ulonglong)dVar25 >> 0x20),SUB84(dVar25,0));
              }
              else {
                dVar25 = -dVar25;
              }
              if (in_FPSCR_SZ == '\0') {
                *(int *)((int)puVar18 + iVar3 + 0x50) = (int)((ulonglong)dVar25 >> 0x20);
              }
              else {
                *(double *)((int)puVar18 + iVar3 + 0x50) = dVar25;
              }
              if (in_FPSCR_SZ == '\0') {
                dVar25 = (double)CONCAT44(*(undefined4 *)
                                           ((int)puVar18 + (int)DAT_8c02a144 + iVar3 + -4),
                                          SUB84(dVar25,0));
              }
              else {
                dVar25 = *(double *)((int)puVar18 + (int)DAT_8c02a144 + iVar3 + -4);
              }
              if (in_FPSCR_PR == '\0') {
                dVar25 = (double)CONCAT44(-(float)((ulonglong)dVar25 >> 0x20),SUB84(dVar25,0));
              }
              else {
                dVar25 = -dVar25;
              }
              if (in_FPSCR_SZ == '\0') {
                *(int *)((int)puVar18 + iVar3 + 0x54) = (int)((ulonglong)dVar25 >> 0x20);
              }
              else {
                *(double *)((int)puVar18 + iVar3 + 0x54) = dVar25;
              }
            }
            uVar29 = SUB84(dVar25,0);
            if (in_FPSCR_SZ == '\0') {
              uVar31 = *(undefined4 *)((int)puVar18 + iVar3 + 0x50);
            }
            else {
              in_xd4 = *(double *)((int)puVar18 + iVar3 + 0x50);
              uVar31 = (undefined4)*(undefined8 *)((int)puVar18 + iVar3 + 0x4c);
            }
            dVar25 = (double)(*(code *)PTR_FUN_8c02a298)();
            if (in_FPSCR_PR == '\0') {
              iVar15 = (int)((float)((ulonglong)dVar25 >> 0x20) * (float)((ulonglong)dVar35 >> 0x20)
                            );
            }
            else {
              iVar15 = (int)(dVar25 * dVar35);
            }
            *(int *)((int)puVar18 + iVar3 + 0x3c) = -iVar15;
            if (in_FPSCR_SZ != '\0') {
              uVar31 = (undefined4)*(undefined8 *)((int)puVar18 + iVar3 + 0x54);
            }
            dVar25 = (double)(*(code *)PTR_FUN_8c02a29c)();
            if (in_FPSCR_PR == '\0') {
              iVar15 = (int)((float)((ulonglong)dVar25 >> 0x20) * (float)((ulonglong)dVar35 >> 0x20)
                            );
            }
            else {
              iVar15 = (int)(dVar25 * dVar35);
            }
            *(int *)((int)puVar18 + iVar3 + 0x38) = iVar15;
            (*(code *)PTR_FUN_8c02a2a0)(0);
            if (in_FPSCR_SZ == '\0') {
              uVar29 = *(undefined4 *)((int)puVar18 + (int)DAT_8c02a282 + iVar3 + -4);
            }
            else {
              in_xd2 = *(double *)((int)puVar18 + (int)DAT_8c02a282 + iVar3 + -4);
            }
            if (in_FPSCR_SZ == '\0') {
              *(undefined4 *)((int)puVar18 + (int)DAT_8c02a284 + iVar3 + -4) = uVar29;
            }
            else {
              *(double *)((int)puVar18 + (int)DAT_8c02a284 + iVar3 + -4) = in_xd2;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)puVar18 + (int)DAT_8c02a286 + iVar3 + -4) = SUB84(dVar35,0);
            }
            else {
              *(undefined8 *)((int)puVar18 + (int)DAT_8c02a286 + iVar3 + -4) = in_xd14;
            }
            if (in_FPSCR_SZ == '\0') {
              uVar29 = *(undefined4 *)((int)puVar18 + (int)DAT_8c02a288 + iVar3 + -4);
            }
            else {
              in_xd2 = *(double *)((int)puVar18 + (int)DAT_8c02a288 + iVar3 + -4);
            }
            if (in_FPSCR_SZ == '\0') {
              *(undefined4 *)((int)puVar18 + (int)DAT_8c02a28a + iVar3 + -4) = uVar29;
            }
            else {
              *(double *)((int)puVar18 + (int)DAT_8c02a28a + iVar3 + -4) = in_xd2;
            }
            (*(code *)puVar2)(0,(undefined1 *)((int)puVar18 + (int)DAT_8c02a284 + iVar3 + -4),
                              (undefined1 *)((int)puVar18 + (int)DAT_8c02a28c + iVar3 + -4));
            if (in_FPSCR_SZ == '\0') {
              uVar29 = *(undefined4 *)(*piVar7 + 0x18);
            }
            else {
              in_xd2 = *(double *)(*piVar7 + 0x18);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44(*(undefined4 *)
                                         ((int)puVar18 + (int)DAT_8c02a28c + iVar3 + -4),uVar29);
            }
            else {
              dVar25 = *(double *)((int)puVar18 + (int)DAT_8c02a28c + iVar3 + -4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = dVar25 + dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)puVar18 + iVar3 + 0x58) = (int)((ulonglong)dVar25 >> 0x20);
            }
            else {
              *(double *)((int)puVar18 + iVar3 + 0x58) = dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)(ulonglong)*(uint *)(*piVar7 + 0x1c);
            }
            else {
              in_xd2 = *(double *)(*piVar7 + 0x1c);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44(*(undefined4 *)
                                         ((int)puVar18 + (int)DAT_8c02a28e + iVar3 + -4),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = *(double *)((int)puVar18 + (int)DAT_8c02a28e + iVar3 + -4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = dVar25 + dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)puVar18 + iVar3 + 0x5c) = (int)((ulonglong)dVar25 >> 0x20);
            }
            else {
              *(double *)((int)puVar18 + iVar3 + 0x5c) = dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)(ulonglong)*(uint *)(*piVar7 + 0x20);
            }
            else {
              in_xd2 = *(double *)(*piVar7 + 0x20);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44(*(undefined4 *)
                                         ((int)puVar18 + (int)DAT_8c02a290 + iVar3 + -4),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = *(double *)((int)puVar18 + (int)DAT_8c02a290 + iVar3 + -4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = dVar25 + dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)puVar18 + iVar3 + 0x60) = (int)((ulonglong)dVar25 >> 0x20);
            }
            else {
              *(double *)((int)puVar18 + iVar3 + 0x60) = dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)puVar18 + (int)DAT_8c02a286 + iVar3 + -4) =
                   (int)((ulonglong)dVar34 >> 0x20);
            }
            else {
              *(double *)((int)puVar18 + (int)DAT_8c02a286 + iVar3 + -4) = dVar34;
            }
            (*(code *)puVar2)(0,(undefined1 *)((int)puVar18 + (int)DAT_8c02a284 + iVar3 + -4),
                              (undefined1 *)((int)puVar18 + (int)DAT_8c02a28c + iVar3 + -4));
            uVar29 = SUB84(dVar25,0);
            if (in_FPSCR_SZ == '\0') {
              uVar29 = *(undefined4 *)(*piVar7 + 0x18);
            }
            else {
              in_xd2 = *(double *)(*piVar7 + 0x18);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44(*(undefined4 *)
                                         ((int)puVar18 + (int)DAT_8c02a28c + iVar3 + -4),uVar29);
            }
            else {
              dVar25 = *(double *)((int)puVar18 + (int)DAT_8c02a28c + iVar3 + -4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = dVar25 + dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)puVar18 + iVar3 + 100) = (int)((ulonglong)dVar25 >> 0x20);
            }
            else {
              *(double *)((int)puVar18 + iVar3 + 100) = dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)(ulonglong)*(uint *)(*piVar7 + 0x1c);
            }
            else {
              in_xd2 = *(double *)(*piVar7 + 0x1c);
            }
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44(*(undefined4 *)
                                         ((int)puVar18 + (int)DAT_8c02a28e + iVar3 + -4),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = *(double *)((int)puVar18 + (int)DAT_8c02a28e + iVar3 + -4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = dVar25 + dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)puVar18 + iVar3 + 0x68) = (int)((ulonglong)dVar25 >> 0x20);
            }
            else {
              *(double *)((int)puVar18 + iVar3 + 0x68) = dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)(ulonglong)*(uint *)(*piVar7 + 0x20);
            }
            else {
              in_xd2 = *(double *)(*piVar7 + 0x20);
            }
            puVar11 = (undefined4 *)((int)puVar18 + (int)DAT_8c02a292 + iVar3 + -4);
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44(*(undefined4 *)
                                         ((int)puVar18 + (int)DAT_8c02a290 + iVar3 + -4),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = *(double *)((int)puVar18 + (int)DAT_8c02a290 + iVar3 + -4);
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = dVar25 + dVar25;
            }
            uVar28 = (uint)((ulonglong)dVar25 >> 0x20);
            if (in_FPSCR_SZ == '\0') {
              *(uint *)((int)puVar18 + iVar3 + 0x6c) = uVar28;
            }
            else {
              *(double *)((int)puVar18 + iVar3 + 0x6c) = dVar25;
            }
            *(undefined4 *)((int)puVar18 + iVar3 + 0x2c) = *puVar11;
            *(undefined4 *)((int)puVar18 + iVar3 + 0x30) = puVar11[1];
            *(undefined4 *)((int)puVar18 + iVar3 + 0x34) = puVar11[2];
            if (in_FPSCR_SZ == '\0') {
              param_1 = (double)CONCAT44(*(undefined4 *)
                                          ((int)puVar18 + (int)DAT_8c02a294 + iVar3 + -4),uVar31);
            }
            else {
              param_1 = *(double *)((int)puVar18 + (int)DAT_8c02a294 + iVar3 + -4);
            }
            fVar20 = (float)((ulonglong)param_1 >> 0x20);
            if (*(int *)((int)puVar18 + iVar3 + 0x14) == 0) {
              if (in_FPSCR_PR == '\0') {
                dVar34 = (double)CONCAT44((float)((ulonglong)dVar34 >> 0x20) * fVar20,
                                          SUB84(dVar34,0));
              }
              else {
                dVar34 = dVar34 * param_1;
              }
              dVar25 = (double)((ulonglong)uVar28 << 0x20);
              if (in_FPSCR_PR == '\0') {
                bVar39 = (float)((ulonglong)dVar34 >> 0x20) < 0.0;
              }
              else {
                bVar39 = dVar34 < dVar25;
              }
              if (bVar39) {
                if (in_FPSCR_PR == '\0') {
                  param_1 = (double)CONCAT44(-fVar20,SUB84(param_1,0));
                }
                else {
                  param_1 = -param_1;
                }
                if (in_FPSCR_SZ == '\0') {
                  *(int *)((int)puVar18 + iVar3 + 0x30) = (int)((ulonglong)param_1 >> 0x20);
                }
                else {
                  *(double *)((int)puVar18 + iVar3 + 0x30) = param_1;
                }
              }
            }
            else {
              dVar25 = (double)((ulonglong)dVar25 & 0xffffffff);
              if (in_FPSCR_SZ == '\0') {
                dVar25 = (double)(ulonglong)*(uint *)((int)puVar18 + (int)DAT_8c02a296 + iVar3 + -4)
                ;
              }
              else {
                in_xd2 = *(double *)((int)puVar18 + (int)DAT_8c02a296 + iVar3 + -4);
              }
              if (in_FPSCR_PR == '\0') {
                dVar25 = (double)(ulonglong)(uint)(SUB84(dVar25,0) * fVar20);
              }
              else {
                dVar25 = dVar25 * param_1;
              }
              if (in_FPSCR_PR == '\0' && SUB84(dVar25,0) < (float)((ulonglong)dVar25 >> 0x20)) {
                if (in_FPSCR_PR == '\0') {
                  param_1 = (double)CONCAT44(-fVar20,SUB84(param_1,0));
                }
                else {
                  param_1 = -param_1;
                }
                if (in_FPSCR_SZ == '\0') {
                  *(int *)((int)puVar18 + iVar3 + 0x30) = (int)((ulonglong)param_1 >> 0x20);
                }
                else {
                  *(double *)((int)puVar18 + iVar3 + 0x30) = param_1;
                }
              }
            }
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                        *(undefined4 *)
                                         ((int)puVar18 + (int)DAT_8c02a3c0 + iVar3 + -4));
            }
            else {
              in_xd2 = *(double *)((int)puVar18 + (int)DAT_8c02a3c0 + iVar3 + -4);
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)puVar18 + iVar3 + 0x2c) = SUB84(dVar25,0);
              *(int *)((int)puVar18 + iVar3 + 0x30) = SUB84(dVar35,0);
            }
            else {
              *(double *)((int)puVar18 + iVar3 + 0x2c) = in_xd2;
              *(undefined8 *)((int)puVar18 + iVar3 + 0x30) = in_xd14;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                        *(undefined4 *)
                                         ((int)puVar18 + (int)DAT_8c02a3c2 + iVar3 + -4));
              *(undefined4 *)((int)puVar18 + iVar3 + 0x34) =
                   *(undefined4 *)((int)puVar18 + (int)DAT_8c02a3c2 + iVar3 + -4);
            }
            else {
              in_xd2 = *(double *)((int)puVar18 + (int)DAT_8c02a3c2 + iVar3 + -4);
              *(double *)((int)puVar18 + iVar3 + 0x34) = in_xd2;
            }
            (*(code *)puVar2)(0,(undefined1 *)((int)puVar18 + iVar3 + 0x2c),
                              (undefined1 *)((int)puVar18 + (int)DAT_8c02a3c4 + iVar3 + -4));
            puVar11 = (undefined4 *)((int)puVar18 + (int)DAT_8c02a3c4 + iVar3 + -4);
            *(undefined4 *)((int)puVar18 + iVar3 + 0x70) = *puVar11;
            *(undefined4 *)((int)puVar18 + iVar3 + 0x74) = puVar11[1];
            *(undefined4 *)((int)puVar18 + iVar3 + 0x78) = puVar11[2];
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)puVar18 + iVar3 + 0x2c) = SUB84(dVar35,0);
            }
            else {
              *(undefined8 *)((int)puVar18 + iVar3 + 0x2c) = in_xd14;
            }
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                        *(undefined4 *)
                                         ((int)puVar18 + (int)DAT_8c02a3c6 + iVar3 + -4));
              *(undefined4 *)((int)puVar18 + iVar3 + 0x30) =
                   *(undefined4 *)((int)puVar18 + (int)DAT_8c02a3c6 + iVar3 + -4);
              *(int *)((int)puVar18 + iVar3 + 0x34) = SUB84(dVar35,0);
            }
            else {
              in_xd2 = *(double *)((int)puVar18 + (int)DAT_8c02a3c6 + iVar3 + -4);
              *(double *)((int)puVar18 + iVar3 + 0x30) = in_xd2;
              *(undefined8 *)((int)puVar18 + iVar3 + 0x34) = in_xd14;
            }
            (*(code *)puVar2)(0,(undefined1 *)((int)puVar18 + iVar3 + 0x2c),
                              (undefined1 *)((int)puVar18 + (int)DAT_8c02a3c4 + iVar3 + -4));
            puVar11 = (undefined4 *)((int)puVar18 + (int)DAT_8c02a3c4 + iVar3 + -4);
            *(undefined4 *)((int)puVar18 + iVar3 + 0x7c) = *puVar11;
            *(undefined4 *)((int)puVar18 + iVar3 + 0x80) = puVar11[1];
            *(undefined4 *)((int)puVar18 + iVar3 + 0x84) = puVar11[2];
            puVar11 = (undefined4 *)*piVar7;
            iVar15 = *(int *)((int)puVar18 + iVar3 + 0xc);
            *(undefined4 *)(iVar15 + 4) = *puVar11;
            *(undefined4 *)(iVar15 + 8) = puVar11[1];
            *(undefined4 *)(iVar15 + 0xc) = puVar11[2];
            *(undefined4 *)(iVar15 + 0x10) = puVar11[3];
            *(undefined4 *)(iVar15 + 0x14) = puVar11[4];
            *(undefined4 *)(iVar15 + 0x18) = puVar11[5];
            *(undefined4 *)(iVar15 + 0x1c) = puVar11[6];
            *(undefined4 *)(iVar15 + 0x20) = puVar11[7];
            *(undefined4 *)(iVar15 + 0x24) = puVar11[8];
            *(undefined4 *)(iVar15 + 0x28) = puVar11[9];
            *(undefined4 *)(iVar15 + 0x2c) = puVar11[10];
            *(undefined4 *)(iVar15 + 0x30) = puVar11[0xb];
            *(undefined4 *)(iVar15 + 0x34) = puVar11[0xc];
            *(undefined4 *)(iVar15 + 0x38) = puVar11[0xd];
            *(undefined4 *)(iVar15 + 0x3c) = puVar11[0xe];
            puVar36 = PTR_FUN_8c02a3cc;
            *(undefined4 *)(iVar15 + 0x40) = puVar11[0xf];
            (*(code *)puVar36)();
            uVar29 = SUB84(param_1,0);
            *(int *)((int)puVar18 + iVar3 + 0xc) =
                 *(int *)((int)puVar18 + iVar3 + 0xc) + (int)DAT_8c02a3c8;
            iVar15 = *DAT_8c02a3d0;
            *DAT_8c02a3d0 = iVar15 + 1;
            if (iVar15 + 1 == 0x10) {
              if (-1 < *DAT_8c02a3d4) {
                (*(code *)PTR_FUN_8c02a3e0)
                          (DAT_8c02a3dc,PTR_s_WARNING__TOUCH_poly_buf_is_FULL_8c02a3d8);
              }
              break;
            }
          }
          (*(code *)PTR_FUN_8c02a440)(1);
          uVar29 = SUB84(param_1,0);
          iVar15 = *(int *)((int)puVar18 + iVar3 + 0x10);
          *(int *)((int)puVar18 + iVar3 + 0x10) = iVar15 + -1;
          piVar7 = piVar7 + 1;
        } while (0 < iVar15 + -1);
      }
      iVar3 = *DAT_8c02a444;
      if (0 < iVar3) {
        iVar4 = (int)DAT_8c02a540;
        iVar10 = 1;
        iVar15 = 0;
        if (in_FPSCR_SZ == '\0') {
          dVar34 = (double)CONCAT44(*(undefined4 *)(iVar4 + DAT_8c02a544),uVar29);
        }
        else {
          dVar34 = *(double *)(iVar4 + DAT_8c02a544);
        }
        iVar13 = DAT_8c02a542 + DAT_8c02a544;
        if (1 < iVar3) {
          do {
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),
                                        *(undefined4 *)(iVar4 + iVar13));
            }
            else {
              in_xd2 = *(double *)(iVar4 + iVar13);
            }
            if (in_FPSCR_PR == '\0') {
              bVar39 = SUB84(dVar25,0) < (float)((ulonglong)dVar34 >> 0x20);
            }
            else {
              bVar39 = dVar25 < dVar34;
            }
            if (bVar39) {
              iVar15 = iVar10;
              if (in_FPSCR_SZ == '\0') {
                dVar34 = (double)CONCAT44(*(undefined4 *)(iVar4 + iVar13),SUB84(dVar34,0));
              }
              else {
                dVar34 = *(double *)(iVar4 + iVar13);
              }
            }
            iVar10 = iVar10 + 1;
            iVar13 = iVar13 + DAT_8c02a542;
          } while (iVar10 < iVar3);
        }
        uVar29 = SUB84(dVar34,0);
        sVar14 = (short)iVar15;
        iVar4 = DAT_8c02a548;
        for (; 0 < iVar3; iVar3 = iVar3 + -1) {
          (*(code *)PTR_FUN_8c02a54c)(iVar4);
          uVar29 = SUB84(dVar34,0);
          sVar14 = (short)iVar15;
          iVar4 = iVar4 + 0x58;
        }
        sVar40 = sVar14 * DAT_8c02a542;
        *(short *)(param_4 + 5) = (short)*DAT_8c02a550;
        iVar3 = DAT_8c02a554;
        *(short *)((int)param_4 + 0x2a) = sVar14;
        puVar11 = (undefined4 *)(sVar40 + iVar3);
        *(undefined4 *)((int)param_4 + 0x34) = *puVar11;
        *(undefined4 *)(param_4 + 7) = puVar11[1];
        *(undefined4 *)((int)param_4 + 0x3c) = puVar11[2];
        *(undefined4 *)(param_4 + 8) = puVar11[3];
        *(undefined4 *)((int)param_4 + 0x44) = puVar11[4];
        *(undefined4 *)(param_4 + 9) = puVar11[5];
        *(undefined4 *)((int)param_4 + 0x4c) = puVar11[6];
        *(undefined4 *)(param_4 + 10) = puVar11[7];
        *(undefined4 *)((int)param_4 + 0x6c) = puVar11[0xe];
        *(undefined4 *)(param_4 + 0xe) = puVar11[0xf];
        *(undefined4 *)((int)param_4 + 0x74) = puVar11[0x10];
        *(undefined4 *)(param_4 + 0xf) = puVar11[0x11];
        *(undefined4 *)((int)param_4 + 0x7c) = puVar11[0x12];
        piVar7 = DAT_8c02a550;
        *(undefined4 *)(param_4 + 0x10) = puVar11[0x13];
        iVar3 = DAT_8c02a664;
        if (*piVar7 == 1) {
          *(undefined4 *)((int)param_4 + 0x54) = puVar11[8];
          *(undefined4 *)(param_4 + 0xb) = puVar11[9];
          *(undefined4 *)((int)param_4 + 0x5c) = puVar11[10];
          *(undefined4 *)(param_4 + 0xc) = puVar11[0xb];
          *(undefined4 *)((int)param_4 + 100) = puVar11[0xc];
          *(undefined4 *)(param_4 + 0xd) = puVar11[0xd];
        }
        else {
          *(undefined4 *)((int)param_4 + 0x54) = *(undefined4 *)(DAT_8c02a664 + 100);
          *(undefined4 *)(param_4 + 0xb) = *(undefined4 *)(iVar3 + 0x68);
          *(undefined4 *)((int)param_4 + 0x5c) = *(undefined4 *)(iVar3 + 0x6c);
          *(undefined4 *)(param_4 + 0xc) = *(undefined4 *)(iVar3 + 0x70);
          *(undefined4 *)((int)param_4 + 100) = *(undefined4 *)(iVar3 + 0x74);
          iVar4 = (int)DAT_8c02a660;
          *(undefined4 *)(param_4 + 0xd) = *(undefined4 *)(iVar3 + 0x78);
          iVar15 = *piVar7;
          iVar3 = iVar3 + iVar4;
          while (iVar15 = iVar15 + -1, 0 < iVar15) {
            if (in_FPSCR_SZ == '\0') {
              dVar25 = (double)CONCAT44(*(undefined4 *)((int)param_4 + 0x54),
                                        *(undefined4 *)(iVar3 + 100));
            }
            else {
              in_xd2 = *(double *)(iVar3 + 100);
              dVar25 = *(double *)((int)param_4 + 0x54);
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = dVar25 + dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)param_4 + 0x54) = (int)((ulonglong)dVar25 >> 0x20);
              dVar25 = (double)CONCAT44(*(undefined4 *)(param_4 + 0xb),*(undefined4 *)(iVar3 + 0x68)
                                       );
            }
            else {
              *(double *)((int)param_4 + 0x54) = dVar25;
              dVar25 = param_4[0xb];
              in_xd2 = *(double *)(iVar3 + 0x68);
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = dVar25 + dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)(param_4 + 0xb) = (int)((ulonglong)dVar25 >> 0x20);
              dVar25 = (double)CONCAT44(*(undefined4 *)((int)param_4 + 0x5c),
                                        *(undefined4 *)(iVar3 + 0x6c));
            }
            else {
              param_4[0xb] = dVar25;
              dVar25 = *(double *)((int)param_4 + 0x5c);
              in_xd2 = *(double *)(iVar3 + 0x6c);
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = dVar25 + dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)param_4 + 0x5c) = (int)((ulonglong)dVar25 >> 0x20);
              dVar25 = (double)CONCAT44(*(undefined4 *)(param_4 + 0xc),*(undefined4 *)(iVar3 + 0x70)
                                       );
            }
            else {
              *(double *)((int)param_4 + 0x5c) = dVar25;
              dVar25 = param_4[0xc];
              in_xd2 = *(double *)(iVar3 + 0x70);
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = dVar25 + dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)(param_4 + 0xc) = (int)((ulonglong)dVar25 >> 0x20);
              dVar25 = (double)CONCAT44(*(undefined4 *)((int)param_4 + 100),
                                        *(undefined4 *)(iVar3 + 0x74));
            }
            else {
              param_4[0xc] = dVar25;
              in_xd2 = *(double *)(iVar3 + 0x74);
              dVar25 = *(double *)((int)param_4 + 100);
            }
            iVar4 = (int)DAT_8c02a660;
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = dVar25 + dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)param_4 + 100) = (int)((ulonglong)dVar25 >> 0x20);
              dVar25 = (double)CONCAT44(*(undefined4 *)(param_4 + 0xd),*(undefined4 *)(iVar3 + 0x78)
                                       );
            }
            else {
              *(double *)((int)param_4 + 100) = dVar25;
              dVar25 = param_4[0xd];
              in_xd2 = *(double *)(iVar3 + 0x78);
            }
            if (in_FPSCR_PR == '\0') {
              dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) + SUB84(dVar25,0),
                                        SUB84(dVar25,0));
            }
            else {
              dVar25 = dVar25 + dVar25;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)(param_4 + 0xd) = (int)((ulonglong)dVar25 >> 0x20);
            }
            else {
              param_4[0xd] = dVar25;
            }
            iVar3 = iVar3 + iVar4;
          }
          uVar31 = (undefined4)((ulonglong)dVar25 >> 0x20);
          dVar25 = (double)CONCAT44(uVar31,(float)*DAT_8c02a668);
          if (in_FPSCR_PR == '\0') {
            dVar34 = (double)CONCAT44(1.0 / (float)*DAT_8c02a668,uVar29);
          }
          else {
            dVar34 = (double)CONCAT44(0x3f800000,uVar29) / dVar25;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar25 = (double)CONCAT44(uVar31,*(undefined4 *)((int)param_4 + 0x54));
          }
          else {
            in_xd2 = *(double *)((int)param_4 + 0x54);
          }
          fVar20 = (float)((ulonglong)dVar34 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            fVar24 = SUB84(dVar25,0) * fVar20;
          }
          else {
            fVar24 = SUB84(dVar25 * dVar34,0);
          }
          if (in_FPSCR_SZ == '\0') {
            *(float *)((int)param_4 + 0x54) = fVar24;
            dVar25 = (double)CONCAT44(*(undefined4 *)(param_4 + 0xb),fVar24);
          }
          else {
            *(double *)((int)param_4 + 0x54) = in_xd2;
            dVar25 = param_4[0xb];
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * fVar20,SUB84(dVar25,0));
          }
          else {
            dVar25 = dVar25 * dVar34;
          }
          uVar29 = (undefined4)((ulonglong)dVar25 >> 0x20);
          if (in_FPSCR_SZ == '\0') {
            *(undefined4 *)(param_4 + 0xb) = uVar29;
            dVar25 = (double)CONCAT44(uVar29,*(undefined4 *)((int)param_4 + 0x5c));
          }
          else {
            param_4[0xb] = dVar25;
            in_xd2 = *(double *)((int)param_4 + 0x5c);
          }
          if (in_FPSCR_PR == '\0') {
            fVar24 = SUB84(dVar25,0) * fVar20;
          }
          else {
            fVar24 = SUB84(dVar25 * dVar34,0);
          }
          if (in_FPSCR_SZ == '\0') {
            *(float *)((int)param_4 + 0x5c) = fVar24;
            dVar25 = (double)CONCAT44(*(undefined4 *)(param_4 + 0xc),fVar24);
          }
          else {
            *(double *)((int)param_4 + 0x5c) = in_xd2;
            dVar25 = param_4[0xc];
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * fVar20,SUB84(dVar25,0));
          }
          else {
            dVar25 = dVar25 * dVar34;
          }
          uVar29 = (undefined4)((ulonglong)dVar25 >> 0x20);
          if (in_FPSCR_SZ == '\0') {
            *(undefined4 *)(param_4 + 0xc) = uVar29;
            dVar25 = (double)CONCAT44(uVar29,*(undefined4 *)((int)param_4 + 100));
          }
          else {
            param_4[0xc] = dVar25;
            in_xd2 = *(double *)((int)param_4 + 100);
          }
          if (in_FPSCR_PR == '\0') {
            fVar24 = SUB84(dVar25,0) * fVar20;
          }
          else {
            fVar24 = SUB84(dVar25 * dVar34,0);
          }
          if (in_FPSCR_SZ == '\0') {
            *(float *)((int)param_4 + 100) = fVar24;
            dVar25 = (double)CONCAT44(*(undefined4 *)(param_4 + 0xd),fVar24);
          }
          else {
            *(double *)((int)param_4 + 100) = in_xd2;
            dVar25 = param_4[0xd];
          }
          if (in_FPSCR_PR == '\0') {
            dVar25 = (double)CONCAT44((float)((ulonglong)dVar25 >> 0x20) * fVar20,SUB84(dVar25,0));
          }
          else {
            dVar25 = dVar25 * dVar34;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)(param_4 + 0xd) = (int)((ulonglong)dVar25 >> 0x20);
          }
          else {
            param_4[0xd] = dVar25;
          }
        }
        return 1;
      }
    }
  }
  return 0;
}

