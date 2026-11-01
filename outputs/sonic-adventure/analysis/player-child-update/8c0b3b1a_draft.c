/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c0b3d66) */
/* WARNING: Removing unreachable block (ram,0x8c0b3bc6) */
/* WARNING: Removing unreachable block (ram,0x8c0b3c36) */
/* WARNING: Removing unreachable block (ram,0x8c0b3eb4) */
/* WARNING: Removing unreachable block (ram,0x8c0b3ed0) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void callback_8c0b3b1a(undefined4 param_1,double param_2,double param_3,double param_4,int param_5)

{
  short sVar1;
  undefined *puVar2;
  undefined1 uVar3;
  int iVar4;
  double *pdVar5;
  double *pdVar6;
  double *pdVar7;
  double *pdVar8;
  int iVar9;
  double *pdVar10;
  double *pdVar11;
  double *pdVar12;
  float fVar13;
  double extraout_dr0;
  double extraout_dr0_00;
  undefined8 uVar14;
  float fVar15;
  float fVar16;
  double in_dr2;
  double dVar17;
  undefined *puVar18;
  undefined4 uVar19;
  double dVar20;
  float fVar21;
  double dVar22;
  double in_dr12;
  double in_dr14;
  double dVar23;
  double in_xd0;
  double in_xd2;
  double in_xd4;
  double dVar24;
  double in_xd6;
  double in_xd8;
  double in_xd12;
  undefined8 in_xd14;
  bool bVar25;
  undefined4 in_PR;
  char cVar26;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_38 [16];
  double dStack_28;
  undefined8 uStack_20;
  
  if (in_FPSCR_SZ == '\0') {
    uStack_20 = CONCAT44(SUB84(in_dr14,0),(undefined4)uStack_20);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_20._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
    pdVar10 = &dStack_28;
    dStack_28 = (double)CONCAT44(SUB84(in_dr12,0),(int)((ulonglong)in_dr12 >> 0x20));
  }
  else {
    pdVar10 = (double *)auStack_38;
    uStack_20 = in_xd14;
    dStack_28 = in_dr14;
  }
  *(undefined4 *)((int)pdVar10 + -4) = in_PR;
  pdVar12 = (double *)((int)pdVar10 + -100);
  iVar9 = *(int *)(param_5 + 0x20);
  iVar4 = (*(code *)PTR_FUN_8c0b3c00)();
  if (iVar4 != 0) {
    (*(code *)PTR_FUN_8c0b3c0c)(DAT_8c0b3c08,PTR_s_player_8c0b3c04);
    return;
  }
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(iVar9 + 0x34));
  }
  else {
    in_xd2 = *(double *)(iVar9 + 0x34);
  }
  if (in_FPSCR_PR == '\0') {
    cVar26 = (char)(int)SUB84(in_dr2,0);
  }
  else {
    cVar26 = (char)(int)in_dr2;
  }
  if ((*(int *)(cVar26 * 4 + DAT_8c0b3c10) == 0) ||
     ((uint)*(byte *)(iVar9 + 3) * 0x48 + *(int *)(iVar9 + 8) == 0)) {
    (*(code *)PTR_FUN_8c0b3c14)(param_5);
    return;
  }
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(iVar9 + 0x34));
  }
  else {
    in_xd2 = *(double *)(iVar9 + 0x34);
  }
  if (in_FPSCR_PR == '\0') {
    cVar26 = (char)(int)SUB84(in_dr2,0);
  }
  else {
    cVar26 = (char)(int)in_dr2;
  }
  if (*(char *)(*(int *)(cVar26 * 4 + DAT_8c0b3c10) + 1) == '\'') {
    if (*(char *)(iVar9 + 1) == '\0') {
      uVar3 = 2;
    }
    else {
      uVar3 = 1;
    }
    *(undefined1 *)(iVar9 + 3) = uVar3;
  }
  else {
    *(undefined1 *)(iVar9 + 3) = 0;
  }
  if (*(char *)(iVar9 + 1) == '\0') {
    if (*(char *)(iVar9 + 3) != '\0') {
      iVar4 = (*(code *)PTR_FUN_8c0b3e80)();
      puVar18 = PTR_FUN_8c0b3e88;
      if (in_FPSCR_SZ == '\0') {
        puVar18 = _DAT_8c0b3e84;
      }
      in_dr2 = (double)CONCAT44(puVar18,(float)iVar4);
      if (in_FPSCR_PR == '\0') {
        in_dr2 = (double)CONCAT44(puVar18,(float)iVar4 * (float)puVar18);
      }
      else {
        in_dr2 = in_dr2 * in_dr2;
      }
      iVar4 = (uint)*(byte *)(iVar9 + 3) * 0x48 + *(int *)(iVar9 + 8);
      if (in_FPSCR_SZ == '\0') {
        dVar17 = (double)CONCAT44((int)((ulonglong)extraout_dr0_00 >> 0x20),
                                  *(undefined4 *)(iVar4 + 0x34));
      }
      else {
        in_xd0 = *(double *)(iVar4 + 0x34);
        dVar17 = extraout_dr0_00;
      }
      if (in_FPSCR_PR == '\0') {
        bVar25 = SUB84(in_dr2,0) < SUB84(dVar17,0);
      }
      else {
        bVar25 = in_dr2 < dVar17;
      }
      if (bVar25) {
        FUN_8c0b37f2(iVar9);
      }
    }
    if (in_FPSCR_SZ == '\0') {
      in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(iVar9 + 0x34));
    }
    else {
      in_xd2 = *(double *)(iVar9 + 0x34);
    }
    if (in_FPSCR_PR == '\0') {
      iVar4 = (int)SUB84(in_dr2,0);
    }
    else {
      iVar4 = (int)in_dr2;
    }
    iVar4 = (*(code *)PTR_FUN_8c0b3e88)(iVar4);
    if (in_FPSCR_SZ == '\0') {
      uVar14 = CONCAT44(*(undefined4 *)(iVar4 + 0x130),0x3f800000);
    }
    else {
      uVar14 = *(undefined8 *)(iVar4 + 0x130);
    }
    if (in_FPSCR_PR != '\0' || (float)((ulonglong)uVar14 >> 0x20) <= (float)uVar14) {
      *(undefined1 *)(iVar9 + 1) = 1;
      *(undefined2 *)(iVar9 + 6) = 0x28;
    }
    goto LAB_8c0b3c76;
  }
  if (*(char *)(iVar9 + 3) == '\0') {
    iVar4 = (*(code *)PTR_FUN_8c0b3c18)();
    fVar16 = fRam8c0b3c20;
    if (in_FPSCR_SZ == '\0') {
      fVar16 = DAT_8c0b3c1c;
    }
    dVar17 = (double)CONCAT44(fVar16,(float)iVar4);
    if (in_FPSCR_PR == '\0') {
      dVar17 = (double)CONCAT44(fVar16,(float)iVar4 * fVar16);
    }
    else {
      dVar17 = dVar17 * dVar17;
    }
    iVar4 = (uint)*(byte *)(iVar9 + 3) * 0x48 + *(int *)(iVar9 + 8);
    if (in_FPSCR_SZ == '\0') {
      dVar20 = (double)CONCAT44((int)((ulonglong)extraout_dr0 >> 0x20),*(undefined4 *)(iVar4 + 0x34)
                               );
    }
    else {
      in_xd0 = *(double *)(iVar4 + 0x34);
      dVar20 = extraout_dr0;
    }
    if (in_FPSCR_PR == '\0') {
      bVar25 = SUB84(dVar17,0) < SUB84(dVar20,0);
    }
    else {
      bVar25 = dVar17 < dVar20;
    }
    if (bVar25) goto LAB_8c0b3be2;
  }
  else {
LAB_8c0b3be2:
    FUN_8c0b37f2(iVar9);
  }
  sVar1 = *(short *)(iVar9 + 6);
  *(short *)(iVar9 + 6) = sVar1 + -1;
  if (sVar1 == 0) {
    *(undefined1 *)(iVar9 + 1) = 0;
  }
LAB_8c0b3c76:
  FUN_8c0b3f2c(param_5);
  if (*DAT_8c0b3e8c == 0) {
    FUN_8c0b3480(iVar9,(undefined1 *)((int)pdVar10 + -0x10),(undefined1 *)((int)pdVar10 + -0x1c),
                 (undefined1 *)((int)pdVar10 + -0x28));
    if (in_FPSCR_SZ == '\0') {
      pdVar5 = (double *)((int)pdVar10 + -0xc);
      dVar17 = (double)CONCAT44(*(undefined4 *)((int)pdVar10 + -0x28),
                                *(undefined4 *)((int)pdVar10 + -0x10));
      pdVar6 = (double *)((int)pdVar10 + -0x24);
    }
    else {
      in_xd2 = *(double *)((int)pdVar10 + -0x10);
      pdVar5 = (double *)((int)pdVar10 + -8);
      dVar17 = *(double *)((int)pdVar10 + -0x28);
      pdVar6 = (double *)((int)pdVar10 + -0x20);
    }
    if (in_FPSCR_PR == '\0') {
      dVar20 = (double)CONCAT44(0x40000000,param_1);
      dVar17 = (double)CONCAT44((float)((ulonglong)dVar17 >> 0x20) - SUB84(dVar17,0),SUB84(dVar17,0)
                               );
    }
    else {
      dVar20 = (double)CONCAT44(0x3f800000,param_1) + (double)CONCAT44(0x3f800000,param_1);
      dVar17 = dVar17 - dVar17;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)pdVar10 + -0x34) = (int)((ulonglong)dVar17 >> 0x20);
      pdVar7 = (double *)((int)pdVar6 + 4);
      dVar17 = (double)CONCAT44(*(undefined4 *)pdVar6,*(undefined4 *)pdVar5);
      pdVar5 = (double *)((int)pdVar5 + 4);
    }
    else {
      *(double *)((int)pdVar10 + -0x34) = dVar17;
      dVar17 = *pdVar6;
      pdVar7 = pdVar6 + 1;
      in_xd2 = *pdVar5;
      pdVar5 = pdVar5 + 1;
    }
    if (in_FPSCR_PR == '\0') {
      dVar17 = (double)CONCAT44((float)((ulonglong)dVar17 >> 0x20) - SUB84(dVar17,0),SUB84(dVar17,0)
                               );
    }
    else {
      dVar17 = dVar17 - dVar17;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)pdVar10 + -0x30) = (int)((ulonglong)dVar17 >> 0x20);
      dVar17 = (double)CONCAT44(*(undefined4 *)pdVar7,*(undefined4 *)pdVar5);
    }
    else {
      *(double *)((int)pdVar10 + -0x30) = dVar17;
      dVar17 = *pdVar7;
      in_xd2 = *pdVar5;
    }
    pdVar5 = (double *)((int)pdVar10 + -0x34);
    if (in_FPSCR_PR == '\0') {
      dVar17 = (double)CONCAT44((float)((ulonglong)dVar17 >> 0x20) - SUB84(dVar17,0),SUB84(dVar17,0)
                               );
    }
    else {
      dVar17 = dVar17 - dVar17;
    }
    uVar19 = (undefined4)((ulonglong)dVar17 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)((int)pdVar10 + -0x2c) = uVar19;
      dVar17 = (double)CONCAT44(uVar19,*(undefined4 *)pdVar5);
    }
    else {
      *(double *)((int)pdVar10 + -0x2c) = dVar17;
      in_xd2 = *pdVar5;
    }
    fVar16 = (float)((ulonglong)dVar20 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      dVar17 = (double)CONCAT44((int)((ulonglong)dVar17 >> 0x20),SUB84(dVar17,0) * fVar16);
    }
    else {
      dVar17 = dVar17 * dVar20;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)pdVar5 = SUB84(dVar17,0);
      dVar17 = (double)CONCAT44((int)((ulonglong)dVar17 >> 0x20),
                                *(undefined4 *)((int)pdVar10 + -0x30));
    }
    else {
      *pdVar5 = in_xd2;
      in_xd2 = *(double *)((int)pdVar10 + -0x30);
    }
    if (in_FPSCR_PR == '\0') {
      fVar15 = SUB84(dVar17,0) * fVar16;
    }
    else {
      fVar15 = SUB84(dVar17 * dVar20,0);
    }
    if (in_FPSCR_SZ == '\0') {
      *(float *)((int)pdVar10 + -0x30) = fVar15;
      dVar17 = (double)CONCAT44(*(undefined4 *)((int)pdVar10 + -0x2c),fVar15);
    }
    else {
      *(double *)((int)pdVar10 + -0x30) = in_xd2;
      dVar17 = *(double *)((int)pdVar10 + -0x2c);
    }
    if (in_FPSCR_PR == '\0') {
      dVar17 = (double)CONCAT44((float)((ulonglong)dVar17 >> 0x20) * fVar16,SUB84(dVar17,0));
    }
    else {
      dVar17 = dVar17 * dVar20;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)pdVar10 + -0x2c) = (int)((ulonglong)dVar17 >> 0x20);
    }
    else {
      *(double *)((int)pdVar10 + -0x2c) = dVar17;
    }
    uVar14 = (*(code *)PTR_FUN_8c0b3e90)((undefined1 *)((int)pdVar10 + -0x34));
    puVar2 = PTR_FUN_8c0b3e94;
    puVar18 = PTR_FUN_8c0b3e80;
    pdVar5 = (double *)((int)pdVar10 + -0x34);
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)pdVar10 + -0x38) = (int)((ulonglong)uVar14 >> 0x20);
      pdVar6 = (double *)((int)pdVar10 + -0xc);
      dVar17 = (double)CONCAT44(*(undefined4 *)pdVar5,*(undefined4 *)((int)pdVar10 + -0x10));
    }
    else {
      *(undefined8 *)((int)pdVar10 + -0x38) = uVar14;
      in_xd2 = *(double *)((int)pdVar10 + -0x10);
      pdVar6 = (double *)((int)pdVar10 + -8);
      dVar17 = *pdVar5;
    }
    if (in_FPSCR_PR == '\0') {
      dVar17 = (double)CONCAT44((float)((ulonglong)dVar17 >> 0x20) + SUB84(dVar17,0),SUB84(dVar17,0)
                               );
    }
    else {
      dVar17 = dVar17 + dVar17;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)pdVar5 = (int)((ulonglong)dVar17 >> 0x20);
      pdVar5 = (double *)((int)pdVar6 + 4);
      dVar17 = (double)CONCAT44(*(undefined4 *)((int)pdVar10 + -0x30),*(undefined4 *)pdVar6);
    }
    else {
      *pdVar5 = dVar17;
      in_xd2 = *pdVar6;
      pdVar5 = pdVar6 + 1;
      dVar17 = *(double *)((int)pdVar10 + -0x30);
    }
    if (in_FPSCR_PR == '\0') {
      dVar17 = (double)CONCAT44((float)((ulonglong)dVar17 >> 0x20) + SUB84(dVar17,0),SUB84(dVar17,0)
                               );
    }
    else {
      dVar17 = dVar17 + dVar17;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)pdVar10 + -0x30) = (int)((ulonglong)dVar17 >> 0x20);
      dVar17 = (double)CONCAT44(*(undefined4 *)((int)pdVar10 + -0x2c),*(undefined4 *)pdVar5);
    }
    else {
      *(double *)((int)pdVar10 + -0x30) = dVar17;
      in_xd2 = *pdVar5;
      dVar17 = *(double *)((int)pdVar10 + -0x2c);
    }
    if (in_FPSCR_PR == '\0') {
      dVar17 = (double)CONCAT44((float)((ulonglong)dVar17 >> 0x20) + SUB84(dVar17,0),SUB84(dVar17,0)
                               );
    }
    else {
      dVar17 = dVar17 + dVar17;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)pdVar10 + -0x2c) = (int)((ulonglong)dVar17 >> 0x20);
      *(undefined4 *)((int)pdVar10 + -0x54) = *(undefined4 *)((int)pdVar10 + -0x34);
      *(undefined4 *)((int)pdVar10 + -0x50) = *(undefined4 *)((int)pdVar10 + -0x30);
      dVar17 = (double)(ulonglong)*(uint *)((int)pdVar10 + -0x2c);
      *(uint *)((int)pdVar10 + -0x58) = *(uint *)((int)pdVar10 + -0x2c);
    }
    else {
      *(double *)((int)pdVar10 + -0x2c) = dVar17;
      *(undefined8 *)((int)pdVar10 + -0x54) = *(undefined8 *)((int)pdVar10 + -0x34);
      *(undefined8 *)((int)pdVar10 + -0x50) = *(undefined8 *)((int)pdVar10 + -0x30);
      in_xd2 = *(double *)((int)pdVar10 + -0x2c);
      *(double *)((int)pdVar10 + -0x58) = in_xd2;
    }
    fVar16 = SUB84(dVar17,0);
    iVar4 = *(int *)(iVar9 + 8) + (uint)*(byte *)(iVar9 + 3) * 0x48;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)((int)pdVar10 + -100) = *(undefined4 *)(iVar4 + 0x28);
      *(undefined4 *)((int)pdVar10 + -0x48) = *(undefined4 *)(iVar4 + 0x2c);
      *(undefined4 *)((int)pdVar10 + -0x3c) = *(undefined4 *)(iVar4 + 0x3c);
      *(undefined4 *)((int)pdVar10 + -0x4c) = *(undefined4 *)(iVar4 + 0x40);
      *(undefined4 *)((int)pdVar10 + -0x5c) = *(undefined4 *)(iVar4 + 0x44);
      fVar16 = *(float *)((int)pdVar10 + -0x54);
    }
    else {
      *(undefined8 *)((int)pdVar10 + -100) = *(undefined8 *)(iVar4 + 0x28);
      *(undefined8 *)((int)pdVar10 + -0x48) = *(undefined8 *)(iVar4 + 0x2c);
      *(undefined8 *)((int)pdVar10 + -0x3c) = *(undefined8 *)(iVar4 + 0x3c);
      *(undefined8 *)((int)pdVar10 + -0x4c) = *(undefined8 *)(iVar4 + 0x40);
      *(undefined8 *)((int)pdVar10 + -0x5c) = *(undefined8 *)(iVar4 + 0x44);
      in_xd2 = *(double *)((int)pdVar10 + -0x54);
    }
    pdVar5 = *(double **)(iVar9 + 0x10);
    if (in_FPSCR_SZ == '\0') {
      *(float *)((int)pdVar10 + -0x44) = -fVar16;
      dVar17 = (double)CONCAT44(*(undefined4 *)((int)pdVar10 + -0x50),-fVar16);
    }
    else {
      *(double *)((int)pdVar10 + -0x44) = in_xd2;
      dVar17 = *(double *)((int)pdVar10 + -0x50);
    }
    if (in_FPSCR_PR == '\0') {
      dVar17 = (double)CONCAT44(-(float)((ulonglong)dVar17 >> 0x20),SUB84(dVar17,0));
    }
    else {
      dVar17 = -dVar17;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)pdVar10 + -0x40) = (int)((ulonglong)dVar17 >> 0x20);
      in_dr14 = (double)ZEXT48(_DAT_8c0b3e84);
    }
    else {
      *(double *)((int)pdVar10 + -0x40) = dVar17;
      in_xd14 = CONCAT44(PTR_FUN_8c0b3e88,_DAT_8c0b3e84);
    }
    while (pdVar5 != (double *)0x0) {
      pdVar6 = pdVar5 + 2;
      if (in_FPSCR_SZ == '\0') {
        param_4 = (double)CONCAT44(*(undefined4 *)pdVar12,SUB84(param_4,0));
        param_3 = (double)CONCAT44((int)((ulonglong)param_3 >> 0x20),
                                   *(undefined4 *)((int)pdVar5 + 4));
        pdVar7 = pdVar5 + 3;
        param_2 = (double)CONCAT44((int)((ulonglong)param_2 >> 0x20),*(undefined4 *)(pdVar5 + 1));
        pdVar8 = (double *)((int)pdVar5 + 0xc);
      }
      else {
        param_4 = *pdVar12;
        in_xd4 = *pdVar6;
        pdVar6 = pdVar5 + 3;
        in_xd8 = *(double *)((int)pdVar5 + 4);
        pdVar7 = pdVar5 + 4;
        in_xd6 = *(double *)((int)pdVar5 + 0xc);
        pdVar8 = (double *)((int)pdVar5 + 0x14);
      }
      dVar20 = *pdVar6;
      if (in_FPSCR_PR == '\0') {
        fVar16 = SUB84(param_3,0) + SUB84(dVar20,0);
      }
      else {
        fVar16 = SUB84(param_3 + dVar20,0);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar17 = (double)CONCAT44((int)((ulonglong)dVar17 >> 0x20),
                                  *(undefined4 *)((int)pdVar12 + 0x1c));
      }
      else {
        in_xd2 = *(double *)((int)pdVar12 + 0x1c);
      }
      fVar15 = (float)((ulonglong)param_4 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar13 = (float)((ulonglong)dVar20 >> 0x20);
        dVar20 = (double)CONCAT44(fVar13,SUB84(dVar20,0) * fVar15);
        fVar21 = SUB84(param_2,0) + fVar13;
      }
      else {
        dVar20 = dVar20 * param_4;
        fVar13 = (float)((ulonglong)dVar20 >> 0x20);
        fVar21 = SUB84(param_2 + dVar20,0);
      }
      if (in_FPSCR_SZ == '\0') {
        param_3 = (double)CONCAT44(*(undefined4 *)pdVar8,fVar16);
        dVar22 = (double)CONCAT44(*(undefined4 *)pdVar7,fVar21);
      }
      else {
        param_3 = *pdVar8;
        dVar22 = *pdVar7;
      }
      if (in_FPSCR_PR == '\0') {
        dVar20 = (double)CONCAT44(fVar13 + SUB84(dVar17,0),SUB84(dVar20,0));
      }
      else {
        dVar20 = dVar20 + dVar17;
      }
      pdVar6 = *(double **)(pdVar5 + 6);
      fVar16 = (float)((ulonglong)dVar22 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        param_3 = (double)CONCAT44((float)((ulonglong)param_3 >> 0x20) + fVar16,SUB84(param_3,0));
        dVar22 = (double)CONCAT44(fVar16 * fVar15,SUB84(dVar22,0));
        dVar20 = (double)CONCAT44((float)((ulonglong)dVar20 >> 0x20) * fVar15,SUB84(dVar20,0));
      }
      else {
        param_3 = param_3 + dVar22;
        dVar22 = dVar22 * param_4;
        dVar20 = dVar20 * param_4;
      }
      uVar19 = (undefined4)((ulonglong)dVar20 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        *(int *)pdVar7 = (int)((ulonglong)dVar22 >> 0x20);
        *(undefined4 *)((int)pdVar7 + -4) = uVar19;
        *(int *)(pdVar7 + -1) = SUB84(dVar20,0);
        *(int *)pdVar8 = (int)((ulonglong)param_3 >> 0x20);
        *(int *)((int)pdVar8 + -4) = SUB84(dVar22,0);
        *(int *)(pdVar8 + -1) = SUB84(param_3,0);
        dVar17 = (double)CONCAT44((int)((ulonglong)dVar17 >> 0x20),*(undefined4 *)((int)pdVar5 + 4))
        ;
        pdVar7 = pdVar5 + 1;
        dVar20 = (double)CONCAT44(uVar19,*(undefined4 *)((int)pdVar5 + 0x24));
        in_dr12 = (double)CONCAT44((int)((ulonglong)in_dr12 >> 0x20),*(undefined4 *)(pdVar12 + 4));
        dVar23 = (double)CONCAT44(*(undefined4 *)((int)pdVar12 + 0x24),SUB84(in_dr14,0));
      }
      else {
        *pdVar7 = dVar22;
        pdVar7[-1] = dVar20;
        pdVar7[-2] = in_xd4;
        *pdVar8 = param_3;
        pdVar8[-1] = in_xd6;
        pdVar8[-2] = in_xd8;
        in_xd2 = *(double *)((int)pdVar5 + 4);
        pdVar7 = (double *)((int)pdVar5 + 0xc);
        in_xd4 = *(double *)((int)pdVar5 + 0x24);
        in_xd12 = pdVar12[4];
        dVar23 = *(double *)((int)pdVar12 + 0x24);
      }
      if (in_FPSCR_PR == '\0') {
        fVar16 = SUB84(in_dr12,0) + SUB84(dVar17,0);
      }
      else {
        fVar16 = SUB84(in_dr12 + dVar17,0);
      }
      if (in_FPSCR_SZ == '\0') {
        dVar17 = (double)CONCAT44((int)((ulonglong)dVar17 >> 0x20),*(undefined4 *)pdVar7);
        pdVar7 = (double *)((int)pdVar7 + 4);
      }
      else {
        in_xd2 = *pdVar7;
        pdVar7 = pdVar7 + 1;
      }
      if (in_FPSCR_PR == '\0') {
        in_dr14 = (double)CONCAT44((float)((ulonglong)dVar23 >> 0x20) + SUB84(dVar17,0),
                                   SUB84(dVar23,0));
      }
      else {
        in_dr14 = dVar23 + dVar17;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar17 = (double)CONCAT44((int)((ulonglong)dVar17 >> 0x20),
                                  *(undefined4 *)((int)pdVar12 + 0xc));
        dVar23 = (double)CONCAT44(*(undefined4 *)pdVar7,fVar16);
        dVar24 = (double)CONCAT44(fVar16,*(undefined4 *)((int)pdVar12 + 0x2c));
      }
      else {
        in_xd2 = *(double *)((int)pdVar12 + 0xc);
        dVar23 = *pdVar7;
        in_xd0 = *(double *)((int)pdVar12 + 0x2c);
        fVar16 = SUB84(dVar23,0);
        dVar24 = in_xd12;
      }
      if (in_FPSCR_PR == '\0') {
        in_dr12 = (double)CONCAT44((float)((ulonglong)dVar23 >> 0x20) - SUB84(dVar17,0),fVar16);
      }
      else {
        in_dr12 = dVar23 - dVar17;
      }
      fVar16 = (float)((ulonglong)in_dr14 >> 0x20);
      dVar23 = in_dr14;
      if (in_FPSCR_SZ == '\0') {
        dVar23 = (double)CONCAT44(fVar16,SUB84(dVar17,0));
      }
      fVar15 = SUB84(in_dr12,0);
      if (in_FPSCR_PR == '\0') {
        fVar13 = SUB84(dVar24,0) + SUB84(dVar20,0);
        dVar23 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * fVar15 +
                                  (float)((ulonglong)dVar23 >> 0x20) * fVar16,SUB84(dVar23,0));
      }
      else {
        dVar23 = dVar23 * in_dr14;
        fVar13 = SUB84(dVar24 + dVar20,0);
      }
      fVar16 = (float)((ulonglong)in_dr12 >> 0x20);
      dVar24 = in_xd0;
      dVar17 = in_dr12;
      if (in_FPSCR_SZ == '\0') {
        dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),fVar13);
        dVar24 = in_xd4;
        fVar15 = fVar13;
        dVar17 = (double)CONCAT44(fVar16,fVar13);
      }
      if (in_FPSCR_PR == '\0') {
        fVar13 = SUB84(dVar20,0) * fVar15;
        dVar23 = (double)CONCAT44((float)((ulonglong)dVar17 >> 0x20) * fVar16 +
                                  (float)((ulonglong)dVar23 >> 0x20),SUB84(dVar23,0));
      }
      else {
        fVar13 = SUB84(dVar20 * dVar17,0);
      }
      dVar17 = dVar23;
      if (in_FPSCR_SZ == '\0') {
        dVar17 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),fVar13);
      }
      fVar16 = (float)((ulonglong)dVar17 >> 0x20);
      if (in_FPSCR_SZ == '\0') {
        *(float *)((int)pdVar12 + 4) = fVar15;
      }
      else {
        *(double *)((int)pdVar12 + 4) = in_xd0;
      }
      if (in_FPSCR_PR == '\0' && fVar16 < SUB84(dVar17,0)) {
        if (in_FPSCR_SZ == '\0') {
          dVar23 = (double)CONCAT44(DAT_8c0b3e98,SUB84(dVar23,0));
        }
        else {
          dVar23 = (double)CONCAT44(DAT_8c0b3e9c,DAT_8c0b3e98);
        }
        if (in_FPSCR_PR == '\0') {
          bVar25 = (float)((ulonglong)dVar23 >> 0x20) < fVar16;
        }
        else {
          bVar25 = dVar23 < dVar17;
        }
        if (bVar25) {
          if (in_FPSCR_SZ == '\0') {
            dVar23 = (double)CONCAT44(*(undefined4 *)((int)pdVar12 + 4),DAT_8c0b3e9c);
          }
          else {
            in_xd2 = (double)CONCAT44(uRam8c0b3ea0,DAT_8c0b3e9c);
            dVar23 = *(double *)((int)pdVar12 + 4);
          }
          if (in_FPSCR_PR == '\0') {
            dVar23 = (double)CONCAT44((float)((ulonglong)dVar23 >> 0x20) + SUB84(dVar23,0),
                                      SUB84(dVar23,0));
          }
          else {
            dVar23 = dVar23 + dVar23;
          }
          if (in_FPSCR_SZ == '\0') {
            pdVar11 = (double *)((int)pdVar12 + -4);
            *(int *)((int)pdVar12 + -4) = (int)((ulonglong)dVar23 >> 0x20);
          }
          else {
            pdVar11 = pdVar12 + -1;
            pdVar12[-1] = dVar23;
          }
          dVar20 = (double)(*(code *)puVar2)();
          if (in_FPSCR_SZ == '\0') {
            dVar17 = (double)CONCAT44(*(undefined4 *)pdVar11,SUB84(dVar17,0));
            pdVar12 = (double *)((int)pdVar11 + 4);
          }
          else {
            dVar17 = *pdVar11;
            pdVar12 = pdVar11 + 1;
          }
          if (in_FPSCR_PR == '\0') {
            dVar17 = (double)CONCAT44((float)((ulonglong)dVar17 >> 0x20) /
                                      (float)((ulonglong)dVar20 >> 0x20),SUB84(dVar17,0));
          }
          else {
            dVar17 = dVar17 / dVar20;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),
                                      *(undefined4 *)((int)pdVar12 + 0x10));
          }
          else {
            in_xd2 = *(double *)((int)pdVar12 + 0x10);
          }
          fVar16 = (float)((ulonglong)dVar17 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            fVar15 = SUB84(in_dr12,0) * fVar16;
            in_dr12 = (double)CONCAT44((float)((ulonglong)in_dr12 >> 0x20) * fVar16,fVar15);
            in_dr14 = (double)CONCAT44((float)((ulonglong)in_dr14 >> 0x20) * fVar16,SUB84(in_dr14,0)
                                      );
            dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),SUB84(dVar23,0) + fVar15);
          }
          else {
            in_dr12 = in_dr12 * dVar17 * dVar17;
            in_dr14 = in_dr14 * dVar17;
            dVar23 = dVar23 + in_dr12;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)((int)pdVar5 + 4) = SUB84(dVar23,0);
            dVar17 = (double)CONCAT44(*(undefined4 *)((int)pdVar12 + 0xc),SUB84(dVar17,0));
          }
          else {
            *(double *)((int)pdVar5 + 4) = in_xd2;
            dVar17 = *(double *)((int)pdVar12 + 0xc);
          }
          dVar23 = (double)((ulonglong)dVar23 & 0xffffffff00000000);
          fVar16 = (float)((ulonglong)in_dr14 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            bVar25 = fVar16 < 0.0;
            dVar17 = (double)CONCAT44((float)((ulonglong)dVar17 >> 0x20) +
                                      (float)((ulonglong)in_dr12 >> 0x20),SUB84(dVar17,0));
          }
          else {
            bVar25 = in_dr14 < dVar23;
            dVar17 = dVar17 + in_dr12;
          }
          uVar19 = (undefined4)((ulonglong)dVar17 >> 0x20);
          if (bVar25) {
            if (in_FPSCR_SZ == '\0') {
              *(undefined4 *)((int)pdVar5 + 0xc) = uVar19;
            }
            else {
              *(double *)((int)pdVar5 + 0xc) = dVar17;
            }
          }
          else {
            if (in_FPSCR_SZ == '\0') {
              dVar23 = (double)((ulonglong)*(uint *)((int)pdVar12 + 0x14) << 0x20);
            }
            else {
              dVar23 = *(double *)((int)pdVar12 + 0x14);
            }
            if (in_FPSCR_PR == '\0') {
              dVar23 = (double)CONCAT44((float)((ulonglong)dVar23 >> 0x20) + fVar16,SUB84(dVar23,0))
              ;
            }
            else {
              dVar23 = dVar23 + in_dr14;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)(pdVar5 + 1) = (int)((ulonglong)dVar23 >> 0x20);
              *(undefined4 *)((int)pdVar5 + 0xc) = uVar19;
            }
            else {
              pdVar5[1] = dVar23;
              *(double *)((int)pdVar5 + 0xc) = dVar17;
            }
          }
        }
      }
      if (in_FPSCR_SZ == '\0') {
        param_2 = (double)CONCAT44(*(undefined4 *)(pdVar5 + 1),SUB84(dVar22,0));
        dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),*(undefined4 *)(pdVar12 + 5));
        dVar17 = (double)CONCAT44(*(undefined4 *)((int)pdVar5 + 0x24),*(undefined4 *)(pdVar5 + 5));
      }
      else {
        param_2 = pdVar5[1];
        dVar24 = pdVar5[5];
        in_xd2 = pdVar12[5];
        dVar17 = *(double *)((int)pdVar5 + 0x24);
      }
      if (in_FPSCR_PR == '\0') {
        dVar17 = (double)CONCAT44((float)((ulonglong)dVar17 >> 0x20) + SUB84(dVar23,0),
                                  SUB84(dVar17,0));
      }
      else {
        dVar17 = dVar17 + dVar23;
      }
      fVar16 = SUB84(dVar17,0);
      if (in_FPSCR_SZ == '\0') {
        param_2 = (double)CONCAT44((int)((ulonglong)param_2 >> 0x20),*(undefined4 *)pdVar5);
      }
      else {
        in_xd6 = *pdVar5;
      }
      if ((in_FPSCR_PR == '\0' && fVar16 < (float)((ulonglong)dVar17 >> 0x20)) &&
         (dVar17 = dVar24, in_FPSCR_SZ == '\0')) {
        dVar17 = (double)CONCAT44(fVar16,fVar16);
      }
      dVar20 = in_xd6;
      if (in_FPSCR_SZ == '\0') {
        dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),SUB84(param_2,0));
        dVar20 = in_xd2;
      }
      fVar16 = (float)((ulonglong)dVar17 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),SUB84(dVar23,0) - fVar16);
      }
      else {
        dVar23 = dVar23 - dVar17;
      }
      fVar15 = (float)((ulonglong)param_2 >> 0x20);
      in_xd4 = dVar20;
      if (in_FPSCR_SZ == '\0') {
        dVar17 = (double)CONCAT44(fVar16,SUB84(dVar23,0));
        in_xd4 = dVar24;
      }
      if (in_FPSCR_PR == '\0') {
        dVar17 = (double)CONCAT44((int)((ulonglong)dVar17 >> 0x20),SUB84(dVar17,0) - fVar15);
      }
      else {
        dVar17 = dVar17 - param_2;
      }
      if (in_FPSCR_PR == '\0' && fVar15 < SUB84(param_2,0)) {
        fVar16 = SUB84(dVar17,0);
        if (in_FPSCR_PR == '\0') {
          bVar25 = fVar16 < 0.0;
        }
        else {
          bVar25 = dVar17 < (double)((ulonglong)dVar23 & 0xffffffff00000000);
        }
        if (bVar25) {
          if (in_FPSCR_PR == '\0') {
            dVar17 = (double)CONCAT44((float)((ulonglong)dVar17 >> 0x20) + fVar16,fVar16);
          }
          else {
            dVar17 = dVar17 + dVar17;
          }
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)pdVar5 + 0x24) = (int)((ulonglong)dVar17 >> 0x20);
        }
        else {
          *(double *)((int)pdVar5 + 0x24) = dVar17;
        }
      }
      else {
        FUN_8c0b33ce(iVar9,pdVar5);
      }
      iVar4 = (*(code *)puVar18)();
      if (in_FPSCR_SZ == '\0') {
        uVar19 = *(undefined4 *)(pdVar12 + 3);
      }
      else {
        uVar19 = (undefined4)((ulonglong)pdVar12[3] >> 0x20);
      }
      if (in_FPSCR_PR == '\0') {
        dVar17 = (double)CONCAT44(uVar19,(float)iVar4 * SUB84(in_dr14,0));
      }
      else {
        dVar17 = (double)CONCAT44(uVar19,(float)iVar4) * in_dr14;
      }
      if (in_FPSCR_PR == '\0' && SUB84(dVar17,0) < (float)((ulonglong)dVar17 >> 0x20)) {
        *(double **)((int)pdVar12 + -4) = pdVar5 + 2;
        iVar4 = (*(code *)puVar18)();
        fVar15 = (float)((ulonglong)in_xd14 >> 0x20);
        fVar16 = DAT_8c0b4000._4_4_;
        if (in_FPSCR_SZ == '\0') {
          fVar15 = SUB84(in_dr14,0);
          fVar16 = (float)DAT_8c0b4000;
        }
        fVar13 = (float)iVar4;
        if (in_FPSCR_PR == '\0') {
          fVar16 = fVar15 * fVar13 + fVar16;
        }
        if (in_FPSCR_SZ == '\0') {
          fVar15 = *(float *)(pdVar12 + 1);
        }
        else {
          fVar15 = (float)((ulonglong)pdVar12[1] >> 0x20);
        }
        pdVar7 = *(double **)((int)pdVar12 + -4);
        if (in_FPSCR_SZ == '\0') {
          fVar13 = *(float *)pdVar7;
        }
        else {
          dVar20 = *pdVar7;
        }
        if (in_FPSCR_PR == '\0') {
          fVar13 = fVar15 * fVar16 + fVar13;
        }
        if (in_FPSCR_SZ == '\0') {
          *(float *)pdVar7 = fVar13;
        }
        else {
          *pdVar7 = dVar20;
        }
        iVar4 = (*(code *)puVar18)();
        fVar16 = (float)((ulonglong)in_xd14 >> 0x20);
        if (in_FPSCR_SZ == '\0') {
          fVar16 = SUB84(in_dr14,0);
          fVar13 = (float)DAT_8c0b4000;
        }
        else {
          dVar20 = (double)CONCAT44(DAT_8c0b4000._4_4_,(float)DAT_8c0b4000);
        }
        if (in_FPSCR_PR == '\0') {
          fVar13 = fVar16 * (float)iVar4 + fVar13;
        }
        else {
          fVar13 = SUB84((double)iVar4,0);
        }
        if (in_FPSCR_SZ == '\0') {
          fVar16 = *(float *)(pdVar12 + 1);
          dVar17 = (double)CONCAT44(*(undefined4 *)(pdVar5 + 3),fVar13);
        }
        else {
          fVar16 = (float)((ulonglong)pdVar12[1] >> 0x20);
          dVar17 = pdVar5[3];
        }
        if (in_FPSCR_PR == '\0') {
          dVar17 = (double)CONCAT44(fVar16 * SUB84(dVar17,0) + (float)((ulonglong)dVar17 >> 0x20),
                                    SUB84(dVar17,0));
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(pdVar5 + 3) = (int)((ulonglong)dVar17 >> 0x20);
        }
        else {
          pdVar5[3] = dVar17;
        }
      }
      *(short *)((int)pdVar5 + 0x1c) =
           *(short *)((int)pdVar5 + 0x1c) + *(short *)((int)pdVar5 + 0x1e);
      pdVar5 = pdVar6;
      in_xd2 = dVar20;
    }
    *(short *)(iVar9 + 6) = *(short *)(iVar9 + 6) + 1;
  }
  return;
}

