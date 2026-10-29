/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c077df4) */
/* WARNING: Removing unreachable block (ram,0x8c07810e) */
/* WARNING: Removing unreachable block (ram,0x8c077dfc) */
/* WARNING: Removing unreachable block (ram,0x8c078116) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_8c0777e0(double param_1,int param_2,int param_3,int param_4)

{
  ushort uVar1;
  short sVar2;
  ulonglong uVar3;
  undefined *puVar4;
  char cVar5;
  int iVar6;
  int iVar7;
  uint uVar8;
  undefined4 *puVar9;
  uint *puVar10;
  undefined4 uVar11;
  undefined4 *puVar12;
  undefined4 *puVar13;
  undefined4 *puVar14;
  int iVar15;
  uint *puVar16;
  float *pfVar17;
  undefined8 *puVar18;
  int *piVar19;
  int *piVar20;
  float fVar21;
  double dVar22;
  undefined8 uVar23;
  double dVar24;
  double extraout_dr0;
  double dVar25;
  float fVar26;
  undefined8 in_dr2;
  double dVar27;
  longlong lVar28;
  undefined8 in_dr12;
  undefined4 uVar29;
  undefined8 in_dr14;
  double dVar30;
  undefined8 in_xd0;
  undefined8 in_xd2;
  undefined8 in_xd14;
  bool bVar31;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_3c [16];
  undefined8 uStack_2c;
  undefined8 uStack_24;
  
  puVar4 = PTR_FUN_8c0779e0;
  uVar11 = (undefined4)((ulonglong)in_dr2 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    uStack_24 = CONCAT44((int)in_dr14,(undefined4)uStack_24);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_24._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
    puVar18 = &uStack_2c;
    uStack_2c = CONCAT44((int)in_dr12,(int)((ulonglong)in_dr12 >> 0x20));
  }
  else {
    puVar18 = (undefined8 *)auStack_3c;
    uStack_24 = in_xd14;
    uStack_2c = in_dr14;
  }
  *(undefined4 *)((int)puVar18 + -4) = in_PR;
  piVar20 = (int *)((int)puVar18 + -0x20c);
  pfVar17 = *(float **)(param_4 + 100);
  *(int *)((int)puVar18 + -0x1f8) = (int)*(short *)(param_4 + 0x10);
  iVar15 = 0;
  *(undefined2 *)(param_4 + 0x10) = *(undefined2 *)(param_2 + 4);
  *(ushort *)(param_2 + 4) = *(ushort *)(param_2 + 4) & 0xfffe;
  (*(code *)puVar4)();
  iVar7 = *(int *)(param_2 + 0x1c);
  *(int *)((int)puVar18 + -0x208) = iVar7;
  if (iVar7 != 0) {
    (*(code *)PTR_FUN_8c0779e4)(0,*(undefined4 *)((int)puVar18 + -0x208));
  }
  iVar7 = *(int *)(param_2 + 0x14);
  *(int *)((int)puVar18 + -0x208) = iVar7;
  if (iVar7 != 0) {
    (*(code *)PTR_FUN_8c0779e8)(0,*(undefined4 *)((int)puVar18 + -0x208));
  }
  iVar7 = -*(int *)(param_2 + 0x18);
  *(int *)((int)puVar18 + -0x208) = iVar7;
  if (iVar7 != 0) {
    (*(code *)PTR_FUN_8c0779ec)(0,iVar7);
  }
  uVar29 = 0;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)((int)puVar18 + -0x20) = 0;
  }
  else {
    *(undefined8 *)((int)puVar18 + -0x20) = in_xd14;
  }
  dVar27 = (double)CONCAT44(uVar11,0x3f800000);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)((int)puVar18 + -0x28) = 0;
    *(undefined4 *)((int)puVar18 + -0x24) = 0x3f800000;
  }
  else {
    *(undefined8 *)((int)puVar18 + -0x28) = in_xd14;
    *(undefined8 *)((int)puVar18 + -0x24) = in_xd2;
  }
  pfVar17[0xd] = *(float *)(param_2 + 0x14);
  puVar4 = PTR_FUN_8c0779f0;
  pfVar17[0xe] = *(float *)(param_2 + 0x1c);
  (*(code *)puVar4)(0,(undefined1 *)((int)puVar18 + -0x28),pfVar17 + 7);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)((int)puVar18 + -0x20) = uVar29;
    *(undefined4 *)((int)puVar18 + -0x28) = uVar29;
    dVar30 = (double)CONCAT44(DAT_8c0779f4,uVar29);
    dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),*(undefined4 *)(param_4 + 0x110));
  }
  else {
    *(undefined8 *)((int)puVar18 + -0x20) = in_xd14;
    *(undefined8 *)((int)puVar18 + -0x28) = in_xd14;
    dVar30 = (double)CONCAT44(PTR_FUN_8c0779f8,DAT_8c0779f4);
    in_xd2 = *(undefined8 *)(param_4 + 0x110);
  }
  if (in_FPSCR_PR == '\0') {
    fVar26 = SUB84(dVar27,0) * (float)((ulonglong)dVar30 >> 0x20);
  }
  else {
    fVar26 = SUB84(dVar27 * dVar30,0);
  }
  if (in_FPSCR_SZ == '\0') {
    *(float *)((int)puVar18 + -0x24) = fVar26;
  }
  else {
    *(undefined8 *)((int)puVar18 + -0x24) = in_xd2;
  }
  (*(code *)PTR_FUN_8c0779f0)
            (0,(undefined1 *)((int)puVar18 + -0x28),(undefined1 *)((int)puVar18 + -0x10));
  (*(code *)PTR_FUN_8c0779f8)(param_3,param_4,pfVar17,(undefined1 *)((int)puVar18 + -0x28));
  (*(code *)PTR_FUN_8c0779f0)
            (0,(undefined1 *)((int)puVar18 + -0x28),(undefined1 *)((int)puVar18 + -0x1c));
  (*(code *)PTR_FUN_8c0779fc)(1);
  if (in_FPSCR_SZ == '\0') {
    dVar27 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x20),*(undefined4 *)((int)puVar18 + -0x1c))
    ;
  }
  else {
    in_xd2 = *(undefined8 *)((int)puVar18 + -0x1c);
    dVar27 = *(double *)(param_2 + 0x20);
  }
  if (in_FPSCR_PR == '\0') {
    dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) + SUB84(dVar27,0),SUB84(dVar27,0));
  }
  else {
    dVar27 = dVar27 + dVar27;
  }
  if (in_FPSCR_SZ == '\0') {
    pfVar17[1] = (float)((ulonglong)dVar27 >> 0x20);
    dVar27 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x24),*(undefined4 *)((int)puVar18 + -0x18))
    ;
  }
  else {
    *(double *)(pfVar17 + 1) = dVar27;
    in_xd2 = *(undefined8 *)((int)puVar18 + -0x18);
    dVar27 = *(double *)(param_2 + 0x24);
  }
  if (in_FPSCR_PR == '\0') {
    dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) + SUB84(dVar27,0),SUB84(dVar27,0));
  }
  else {
    dVar27 = dVar27 + dVar27;
  }
  if (in_FPSCR_SZ == '\0') {
    pfVar17[2] = (float)((ulonglong)dVar27 >> 0x20);
    dVar27 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x28),*(undefined4 *)((int)puVar18 + -0x14))
    ;
  }
  else {
    *(double *)(pfVar17 + 2) = dVar27;
    in_xd2 = *(undefined8 *)((int)puVar18 + -0x14);
    dVar27 = *(double *)(param_2 + 0x28);
  }
  if (in_FPSCR_PR == '\0') {
    dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) + SUB84(dVar27,0),SUB84(dVar27,0));
  }
  else {
    dVar27 = dVar27 + dVar27;
  }
  fVar26 = (float)((ulonglong)dVar27 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    pfVar17[3] = fVar26;
    dVar27 = (double)CONCAT44(fVar26,*(undefined4 *)(param_4 + 0x110));
  }
  else {
    *(double *)(pfVar17 + 3) = dVar27;
    in_xd2 = *(undefined8 *)(param_4 + 0x110);
  }
  if (in_FPSCR_PR == '\0') {
    fVar26 = SUB84(dVar27,0) * (float)((ulonglong)dVar30 >> 0x20);
  }
  else {
    fVar26 = SUB84(dVar27 * dVar30,0);
  }
  if (in_FPSCR_SZ == '\0') {
    *pfVar17 = fVar26;
  }
  else {
    *(undefined8 *)pfVar17 = in_xd2;
  }
  fVar26 = *(float *)(param_3 + 8);
  pfVar17[4] = *(float *)(param_3 + 4);
  pfVar17[5] = fVar26;
  puVar4 = PTR_FUN_8c077a00;
  pfVar17[6] = *(float *)(param_3 + 0xc);
  uVar11 = *(undefined4 *)(param_2 + 0x24);
  *(undefined4 *)((int)puVar18 + -0x34) = *(undefined4 *)(param_2 + 0x20);
  *(undefined4 *)((int)puVar18 + -0x30) = uVar11;
  *(undefined4 *)((int)puVar18 + -0x2c) = *(undefined4 *)(param_2 + 0x28);
  (*(code *)puVar4)(pfVar17);
  if (*DAT_8c077a04 == 0) {
    if (in_FPSCR_SZ == '\0') {
      dVar27 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x20),*(undefined4 *)(param_3 + 4));
    }
    else {
      dVar27 = *(double *)(param_2 + 0x20);
    }
    if (in_FPSCR_PR == '\0') {
      dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) + SUB84(dVar27,0),SUB84(dVar27,0)
                               );
    }
    else {
      dVar27 = dVar27 + dVar27;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_2 + 0x20) = (int)((ulonglong)dVar27 >> 0x20);
      dVar27 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x24),*(undefined4 *)(param_3 + 8));
    }
    else {
      *(double *)(param_2 + 0x20) = dVar27;
      dVar27 = *(double *)(param_2 + 0x24);
    }
    if (in_FPSCR_PR == '\0') {
      dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) + SUB84(dVar27,0),SUB84(dVar27,0)
                               );
    }
    else {
      dVar27 = dVar27 + dVar27;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_2 + 0x24) = (int)((ulonglong)dVar27 >> 0x20);
      dVar27 = (double)(ulonglong)*(uint *)(param_3 + 0xc);
    }
    else {
      *(double *)(param_2 + 0x24) = dVar27;
    }
LAB_8c077a9c:
    puVar4 = PTR_FUN_8c077ba8;
    if (in_FPSCR_SZ == '\0') {
      dVar27 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x28),SUB84(dVar27,0));
    }
    else {
      dVar27 = *(double *)(param_2 + 0x28);
    }
    if (in_FPSCR_PR == '\0') {
      dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) + SUB84(dVar27,0),SUB84(dVar27,0)
                               );
    }
    else {
      dVar27 = dVar27 + dVar27;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_2 + 0x28) = (int)((ulonglong)dVar27 >> 0x20);
    }
    else {
      *(double *)(param_2 + 0x28) = dVar27;
    }
    (*(code *)puVar4)(param_2,param_3,param_4,0);
    *(undefined4 *)(param_4 + 0x6c) = 0;
    *(undefined4 *)(param_4 + 0x74) = 0;
    return 0;
  }
  bVar31 = false;
  *(undefined4 *)((int)puVar18 + -0x1ec) = 0;
  *(undefined4 *)((int)puVar18 + -0x20c) = DAT_8c077a08;
  *(undefined4 *)((int)puVar18 + -0x1e8) = 0;
  *(undefined4 *)((int)puVar18 + -0x200) = 0;
  *(undefined4 *)((int)puVar18 + -0x208) = 0;
  *(undefined4 *)((int)puVar18 + -0x1fc) = 0;
  while (*(int *)((int)puVar18 + -0x1fc) < (int)*DAT_8c077ba4) {
    if ((((**(uint **)((int)puVar18 + -0x20c) & DAT_8c077a0c) == 0) ||
        (*(char *)(param_2 + 9) == '\x06')) &&
       ((*(int *)(*(int *)((int)puVar18 + -0x20c) + 8) == 0 ||
        ((*(ushort *)(*(int *)(*(int *)(*(int *)((int)puVar18 + -0x20c) + 8) + 0x20) + 4) & 0x100)
         != 0)))) {
      iVar7 = (*(code *)PTR_FUN_8c077a10)
                        (pfVar17,*(undefined4 *)(*(int *)((int)puVar18 + -0x20c) + 4),
                         **(undefined4 **)((int)puVar18 + -0x20c));
      if (iVar7 != 0) {
        if ((*(ushort *)(param_4 + 0x10) & 3) == 0) {
          iVar7 = (*(code *)PTR_FUN_8c077ba0)(param_2,param_3,param_4,pfVar17);
          if (iVar7 == 1) {
            bVar31 = true;
            *(undefined4 *)((int)puVar18 + -0x200) = *(undefined4 *)((int)puVar18 + -0x208);
          }
        }
        else {
          *(undefined4 *)((int)puVar18 + -0x210) = **(undefined4 **)((int)puVar18 + -0x20c);
          iVar7 = (*(code *)PTR_FUN_8c077a14)(param_2,param_3,param_4,pfVar17);
          if (iVar7 == 1) {
            bVar31 = true;
            *(undefined4 *)((int)puVar18 + -0x200) = *(undefined4 *)((int)puVar18 + -0x208);
          }
        }
      }
      puVar14 = *(undefined4 **)((int)puVar18 + -0x20c);
      puVar9 = (undefined4 *)((int)puVar18 + *(int *)((int)puVar18 + -0x208) * 0xc + -0xf4);
      uVar11 = puVar14[1];
      *puVar9 = *puVar14;
      puVar9[1] = uVar11;
      puVar9[2] = puVar14[2];
      *(int *)((int)puVar18 + -0x208) = *(int *)((int)puVar18 + -0x208) + 1;
    }
    *(int *)((int)puVar18 + -0x1fc) = *(int *)((int)puVar18 + -0x1fc) + 1;
    *(int *)((int)puVar18 + -0x20c) = *(int *)((int)puVar18 + -0x20c) + 0xc;
  }
  if (*(int *)((int)puVar18 + -0x208) == 0) {
    if (in_FPSCR_SZ == '\0') {
      dVar27 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x20),*(undefined4 *)(param_3 + 4));
    }
    else {
      dVar27 = *(double *)(param_2 + 0x20);
    }
    if (in_FPSCR_PR == '\0') {
      dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) + SUB84(dVar27,0),SUB84(dVar27,0)
                               );
    }
    else {
      dVar27 = dVar27 + dVar27;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_2 + 0x20) = (int)((ulonglong)dVar27 >> 0x20);
      dVar27 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x24),*(undefined4 *)(param_3 + 8));
    }
    else {
      *(double *)(param_2 + 0x20) = dVar27;
      dVar27 = *(double *)(param_2 + 0x24);
    }
    if (in_FPSCR_PR == '\0') {
      dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) + SUB84(dVar27,0),SUB84(dVar27,0)
                               );
    }
    else {
      dVar27 = dVar27 + dVar27;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_2 + 0x24) = (int)((ulonglong)dVar27 >> 0x20);
      dVar27 = (double)(ulonglong)*(uint *)(param_3 + 0xc);
    }
    else {
      *(double *)(param_2 + 0x24) = dVar27;
    }
    goto LAB_8c077a9c;
  }
  puVar14 = (undefined4 *)((int)puVar18 + -0x1b4);
  if (bVar31) {
    puVar9 = (undefined4 *)((int)puVar18 + *(int *)((int)puVar18 + -0x200) * 0xc + -0xf4);
    uVar11 = puVar9[1];
    *puVar14 = *puVar9;
    *(undefined4 *)((int)puVar18 + -0x1b0) = uVar11;
    *(undefined4 *)((int)puVar18 + -0x1ac) = puVar9[2];
  }
  *(int *)((int)puVar18 + -0x204) = *(int *)((int)puVar18 + -0x208) + -1;
  puVar9 = puVar14 + (uint)bVar31 * 3;
  if (*(int *)(param_4 + 0x6c) == 0) {
    iVar7 = *(int *)((int)puVar18 + -0x204);
    *(undefined4 **)((int)puVar18 + -0x20c) = puVar14 + iVar7 * 3;
    for (; -1 < iVar7; iVar7 = iVar7 + -1) {
      if ((bVar31) && (iVar7 == *(int *)((int)puVar18 + -0x200))) {
        bVar31 = false;
      }
      else {
        puVar13 = (undefined4 *)((int)puVar18 + iVar7 * 0xc + -0xf4);
        if (*(int *)(param_4 + 0x74) == puVar13[1]) {
          puVar12 = *(undefined4 **)((int)puVar18 + -0x20c);
          uVar11 = puVar13[1];
          *puVar12 = *puVar13;
          puVar12[1] = uVar11;
          puVar12[2] = puVar13[2];
        }
        else {
          uVar11 = puVar13[1];
          *puVar9 = *puVar13;
          puVar9[1] = uVar11;
          puVar9[2] = puVar13[2];
          puVar9 = puVar9 + 3;
        }
      }
    }
  }
  else {
    iVar7 = 0;
    *(undefined4 **)((int)puVar18 + -0x20c) = puVar14 + *(int *)((int)puVar18 + -0x204) * 3;
    if (0 < *(int *)((int)puVar18 + -0x208)) {
      do {
        if ((bVar31) && (iVar7 == *(int *)((int)puVar18 + -0x200))) {
          bVar31 = false;
        }
        else {
          puVar13 = (undefined4 *)((int)puVar18 + iVar7 * 0xc + -0xf4);
          if (*(int *)(param_4 + 0x74) == puVar13[1]) {
            puVar12 = *(undefined4 **)((int)puVar18 + -0x20c);
            uVar11 = puVar13[1];
            *puVar12 = *puVar13;
            puVar12[1] = uVar11;
            puVar12[2] = puVar13[2];
          }
          else {
            uVar11 = puVar13[1];
            *puVar9 = *puVar13;
            puVar9[1] = uVar11;
            puVar9[2] = puVar13[2];
            puVar9 = puVar9 + 3;
          }
        }
        iVar7 = iVar7 + 1;
      } while (iVar7 < *(int *)((int)puVar18 + -0x208));
    }
  }
  if (in_FPSCR_SZ == '\0') {
    dVar27 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x20),*(undefined4 *)((int)puVar18 + -0x1c))
    ;
  }
  else {
    in_xd2 = *(undefined8 *)((int)puVar18 + -0x1c);
    dVar27 = *(double *)(param_2 + 0x20);
  }
  if (in_FPSCR_PR == '\0') {
    dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) + SUB84(dVar27,0),SUB84(dVar27,0));
  }
  else {
    dVar27 = dVar27 + dVar27;
  }
  if (in_FPSCR_SZ == '\0') {
    pfVar17[1] = (float)((ulonglong)dVar27 >> 0x20);
    dVar27 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x24),*(undefined4 *)((int)puVar18 + -0x18))
    ;
  }
  else {
    *(double *)(pfVar17 + 1) = dVar27;
    in_xd2 = *(undefined8 *)((int)puVar18 + -0x18);
    dVar27 = *(double *)(param_2 + 0x24);
  }
  if (in_FPSCR_PR == '\0') {
    dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) + SUB84(dVar27,0),SUB84(dVar27,0));
  }
  else {
    dVar27 = dVar27 + dVar27;
  }
  if (in_FPSCR_SZ == '\0') {
    pfVar17[2] = (float)((ulonglong)dVar27 >> 0x20);
    dVar27 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x28),*(undefined4 *)((int)puVar18 + -0x14))
    ;
  }
  else {
    *(double *)(pfVar17 + 2) = dVar27;
    in_xd2 = *(undefined8 *)((int)puVar18 + -0x14);
    dVar27 = *(double *)(param_2 + 0x28);
  }
  if (in_FPSCR_PR == '\0') {
    dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) + SUB84(dVar27,0),SUB84(dVar27,0));
  }
  else {
    dVar27 = dVar27 + dVar27;
  }
  if (in_FPSCR_SZ == '\0') {
    pfVar17[3] = (float)((ulonglong)dVar27 >> 0x20);
  }
  else {
    *(double *)(pfVar17 + 3) = dVar27;
  }
  fVar26 = *(float *)(param_3 + 8);
  pfVar17[4] = *(float *)(param_3 + 4);
  pfVar17[5] = fVar26;
  pfVar17[6] = *(float *)(param_3 + 0xc);
  *(undefined4 *)(param_4 + 0x6c) = 0;
  *(undefined4 **)((int)puVar18 + -0x1fc) = puVar14;
  *(undefined4 *)((int)puVar18 + -0x20c) = 0;
  *(uint *)((int)puVar18 + -0x1f0) = (uint)((*(uint *)((int)puVar18 + -0x1f8) & 3) == 0);
  *(int *)((int)puVar18 + -0x1f8) =
       *(int *)((int)puVar18 + -0x204) * 0xc + *(int *)((int)puVar18 + -0x1fc);
  if (in_FPSCR_SZ == '\0') {
    dVar25 = (double)CONCAT44((undefined4)DAT_8c077d80,DAT_8c077d80._4_4_);
    piVar20 = (int *)((int)puVar18 + -0x20c);
  }
  else {
    dVar25 = (double)CONCAT44(DAT_8c077d80._4_4_,(undefined4)DAT_8c077d80);
  }
  while (*piVar20 < piVar20[1]) {
    puVar16 = (uint *)(*piVar20 * 0xc + piVar20[4]);
    piVar20[3] = puVar16[2];
    iVar7 = (*(code *)PTR_FUN_8c077d88)(pfVar17,puVar16[1],*puVar16);
    if (iVar7 != 0) {
      iVar7 = (*(code *)PTR_FUN_8c077d8c)(pfVar17 + 7,pfVar17 + 0x12,0);
      piVar20[2] = iVar7;
      if ((*(ushort *)(param_2 + 4) & 2) == 0) {
        if ((*puVar16 & DAT_8c077d90) == 0) {
LAB_8c077cda:
          if (0x2c00 < piVar20[2]) {
            if (in_FPSCR_SZ == '\0') {
              dVar27 = (double)CONCAT44(pfVar17[8],pfVar17[0x13]);
            }
            else {
              in_xd2 = *(undefined8 *)(pfVar17 + 0x13);
              dVar27 = *(double *)(pfVar17 + 8);
            }
            if (in_FPSCR_PR != '\0' || SUB84(dVar27,0) <= (float)((ulonglong)dVar27 >> 0x20))
            goto LAB_8c0781a0;
          }
          if ((piVar20[7] != 0) || ((*(ushort *)(param_4 + 0x10) & 3) != 0)) {
            dVar22 = (double)(*(code *)PTR_FUN_8c077da4)(_DAT_8c077da0,pfVar17 + 0x12);
            uVar3 = (ulonglong)dVar27 >> 0x20;
            dVar27 = (double)(uVar3 << 0x20);
            if (in_FPSCR_PR == '\0') {
              bVar31 = 0.0 < (float)((ulonglong)dVar22 >> 0x20);
            }
            else {
              bVar31 = dVar27 < dVar22;
            }
            if ((!bVar31) || ((*(ushort *)(param_4 + 0x10) & 3) != 0)) {
              dVar27 = (double)(uVar3 << 0x20);
              if (in_FPSCR_SZ == '\0') {
                param_1 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x3c),SUB84(param_1,0));
              }
              else {
                param_1 = *(double *)(param_4 + 0x3c);
              }
              fVar26 = (float)((ulonglong)param_1 >> 0x20);
              if (in_FPSCR_PR == '\0') {
                bVar31 = 0.0 < fVar26;
              }
              else {
                bVar31 = dVar27 < param_1;
              }
              if (bVar31) {
                if (in_FPSCR_PR == '\0') {
                  bVar31 = 0.0 < fVar26;
                }
                else {
                  bVar31 = dVar27 < param_1;
                }
                if ((!bVar31) || ((*(ushort *)(param_4 + 0x10) & 3) != 0)) {
                  piVar20[-1] = *puVar16;
                  piVar20[-2] = iVar15;
                  piVar20[-3] = (int)(piVar20 + 0x7f);
                  piVar20[-4] = (int)(piVar20 + 0x76);
                  iVar15 = (*(code *)PTR_FUN_8c078198)(param_2,param_3,param_4,pfVar17);
                  goto LAB_8c0781c8;
                }
              }
              piVar20[8] = 1;
              piVar20[9] = puVar16[1];
              piVar20[-1] = *puVar16;
              iVar6 = (*(code *)PTR_FUN_8c077da8)(param_2,param_3,param_4,pfVar17);
              iVar7 = iVar15;
              if (((iVar6 != 0) && (iVar7 = iVar6, iVar15 != 0)) && (iVar7 = iVar15, iVar6 == 2)) {
                iVar7 = iVar6;
              }
              if (in_FPSCR_SZ == '\0') {
                fVar26 = pfVar17[0x13];
              }
              else {
                fVar26 = (float)*(undefined8 *)(pfVar17 + 0x12);
              }
              dVar27 = (double)(*(code *)PTR_FUN_8c077fa0)();
              puVar4 = PTR_FUN_8c077fa4;
              if (in_FPSCR_PR == '\0') {
                iVar15 = (int)((float)((ulonglong)dVar27 >> 0x20) * SUB84(dVar25,0));
              }
              else {
                iVar15 = (int)(dVar27 * dVar25);
              }
              pfVar17[0xe] = (float)-iVar15;
              if (in_FPSCR_SZ == '\0') {
                dVar27 = (double)(ulonglong)(uint)fVar26;
              }
              else {
                dVar27 = *(double *)(pfVar17 + 0x14);
              }
              dVar22 = (double)(*(code *)puVar4)();
              if (in_FPSCR_PR == '\0') {
                fVar26 = (float)(int)((float)((ulonglong)dVar22 >> 0x20) * SUB84(dVar25,0));
              }
              else {
                fVar26 = (float)(int)(dVar22 * dVar25);
              }
              pfVar17[0xd] = fVar26;
              if (in_FPSCR_SZ == '\0') {
                dVar22 = (double)((ulonglong)*(uint *)(param_3 + 4) << 0x20);
              }
              else {
                dVar22 = *(double *)(param_3 + 4);
              }
              if ((float)((ulonglong)dVar22 >> 0x20) == SUB84(dVar22,0)) {
                if (in_FPSCR_SZ == '\0') {
                  dVar22 = (double)CONCAT44(*(undefined4 *)(param_3 + 0xc),SUB84(dVar22,0));
                }
                else {
                  dVar22 = *(double *)(param_3 + 0xc);
                }
                if (((float)((ulonglong)dVar22 >> 0x20) != SUB84(dVar22,0)) ||
                   ((*(ushort *)(param_4 + 0x10) & 1) == 0)) goto LAB_8c077e22;
                uVar23 = (*(code *)PTR_FUN_8c077fac)(DAT_8c077fa8,pfVar17 + 0x12);
                if (in_FPSCR_SZ == '\0') {
                  piVar19 = piVar20 + -1;
                  piVar20[-1] = (int)((ulonglong)uVar23 >> 0x20);
                }
                else {
                  piVar19 = piVar20 + -2;
                  *(undefined8 *)(piVar20 + -2) = uVar23;
                }
                dVar24 = (double)(*(code *)PTR_FUN_8c077fac)(DAT_8c077fa8,pfVar17 + 7);
                if (in_FPSCR_SZ == '\0') {
                  dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*piVar19);
                  piVar20 = (int *)((int)piVar19 + 4);
                }
                else {
                  in_xd2 = *(undefined8 *)piVar19;
                  piVar20 = (int *)((int)piVar19 + 8);
                }
                if (in_FPSCR_PR == '\0') {
                  bVar31 = SUB84(dVar22,0) < (float)((ulonglong)dVar24 >> 0x20);
                }
                else {
                  bVar31 = dVar22 < dVar24;
                }
                if (bVar31) goto LAB_8c077e22;
              }
              else {
LAB_8c077e22:
                (*(code *)PTR_FUN_8c077fb0)(param_2,param_4,pfVar17);
              }
              uVar11 = (undefined4)((ulonglong)dVar22 >> 0x20);
              if (iVar7 == 0) {
                (*(code *)PTR_FUN_8c077fb4)();
                iVar15 = *(int *)(param_2 + 0x1c);
                piVar20[2] = iVar15;
                if (iVar15 != 0) {
                  (*(code *)PTR_FUN_8c077fb8)(0,piVar20[2]);
                }
                iVar15 = *(int *)(param_2 + 0x14);
                piVar20[2] = iVar15;
                if (iVar15 != 0) {
                  (*(code *)PTR_FUN_8c077fbc)(0,piVar20[2]);
                }
                iVar15 = -*(int *)(param_2 + 0x18);
                piVar20[2] = iVar15;
                if (iVar15 != 0) {
                  (*(code *)PTR_FUN_8c077fc0)(0,iVar15);
                }
                dVar22 = (double)CONCAT44(uVar11,0x3f800000);
                if (in_FPSCR_SZ == '\0') {
                  piVar20[0x7b] = SUB84(dVar30,0);
                  piVar20[0x79] = SUB84(dVar30,0);
                  piVar20[0x7a] = 0x3f800000;
                }
                else {
                  *(undefined8 *)(piVar20 + 0x7b) = in_xd14;
                  *(undefined8 *)(piVar20 + 0x79) = in_xd14;
                  *(undefined8 *)(piVar20 + 0x7a) = in_xd2;
                }
                pfVar17[0xd] = *(float *)(param_2 + 0x14);
                puVar4 = PTR_FUN_8c077fc4;
                pfVar17[0xe] = *(float *)(param_2 + 0x1c);
                (*(code *)puVar4)(0,piVar20 + 0x79,pfVar17 + 7);
                if (in_FPSCR_SZ == '\0') {
                  piVar20[0x7b] = SUB84(dVar30,0);
                  piVar20[0x79] = SUB84(dVar30,0);
                  dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                            *(undefined4 *)(param_4 + 0x110));
                }
                else {
                  *(undefined8 *)(piVar20 + 0x7b) = in_xd14;
                  *(undefined8 *)(piVar20 + 0x79) = in_xd14;
                  in_xd2 = *(undefined8 *)(param_4 + 0x110);
                }
                if (in_FPSCR_PR == '\0') {
                  fVar26 = SUB84(dVar22,0) * (float)((ulonglong)dVar30 >> 0x20);
                }
                else {
                  fVar26 = SUB84(dVar22 * dVar30,0);
                }
                if (in_FPSCR_SZ == '\0') {
                  piVar20[0x7a] = (int)fVar26;
                }
                else {
                  *(undefined8 *)(piVar20 + 0x7a) = in_xd2;
                }
                (*(code *)PTR_FUN_8c077fc4)(0,piVar20 + 0x79,piVar20 + 0x7f);
                (*(code *)PTR_FUN_8c077fc8)(param_3,param_4,pfVar17,piVar20 + 0x79);
                (*(code *)PTR_FUN_8c077fc4)(0,piVar20 + 0x79,piVar20 + 0x7c);
                (*(code *)PTR_FUN_8c077fcc)(1);
              }
              *(float *)(param_3 + 0x1c) = pfVar17[0xd];
              *(float *)(param_3 + 0x24) = pfVar17[0xe];
              fVar26 = pfVar17[0x13];
              *(float *)(param_4 + 0x50) = pfVar17[0x12];
              *(float *)(param_4 + 0x54) = fVar26;
              puVar4 = PTR_FUN_8c077fd0;
              *(float *)(param_4 + 0x58) = pfVar17[0x14];
              dVar22 = (double)(*(code *)puVar4)(pfVar17 + 0x1b);
              uVar11 = SUB84(dVar27,0);
              if (in_FPSCR_PR == '\0') {
                bVar31 = (float)((ulonglong)dVar25 >> 0x20) < (float)((ulonglong)dVar22 >> 0x20);
              }
              else {
                bVar31 = dVar25 < dVar22;
              }
              if (bVar31) {
                (*(code *)PTR_FUN_8c077fd4)(pfVar17 + 0x1b);
                dVar22 = (double)(*(code *)PTR_FUN_8c077fd0)(pfVar17 + 4);
                uVar29 = (undefined4)((ulonglong)dVar22 >> 0x20);
                if (in_FPSCR_SZ == '\0') {
                  dVar24 = (double)CONCAT44(pfVar17[0x1b],DAT_8c077fd8);
                  dVar27 = (double)CONCAT44(uVar29,uVar11);
                }
                else {
                  in_xd2 = CONCAT44(uRam8c077fdc,DAT_8c077fd8);
                  dVar24 = *(double *)(pfVar17 + 0x1b);
                  dVar27 = dVar22;
                }
                if (in_FPSCR_PR == '\0') {
                  fVar26 = (float)((ulonglong)dVar27 >> 0x20) + SUB84(dVar24,0);
                  dVar27 = (double)CONCAT44(fVar26,SUB84(dVar27,0));
                  dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * fVar26,
                                            SUB84(dVar24,0));
                }
                else {
                  dVar27 = dVar27 + dVar24;
                  fVar26 = (float)((ulonglong)dVar27 >> 0x20);
                  dVar24 = dVar24 * dVar27;
                }
                if (in_FPSCR_SZ == '\0') {
                  pfVar17[4] = (float)((ulonglong)dVar24 >> 0x20);
                  dVar24 = (double)CONCAT44(pfVar17[0x1c],SUB84(dVar24,0));
                }
                else {
                  *(double *)(pfVar17 + 4) = dVar24;
                  dVar24 = *(double *)(pfVar17 + 0x1c);
                }
                if (in_FPSCR_PR == '\0') {
                  dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * fVar26,
                                            SUB84(dVar24,0));
                }
                else {
                  dVar24 = dVar24 * dVar27;
                }
                if (in_FPSCR_SZ == '\0') {
                  pfVar17[5] = (float)((ulonglong)dVar24 >> 0x20);
                  dVar24 = (double)CONCAT44(pfVar17[0x1d],SUB84(dVar24,0));
                }
                else {
                  *(double *)(pfVar17 + 5) = dVar24;
                  dVar24 = *(double *)(pfVar17 + 0x1d);
                }
                if (in_FPSCR_PR == '\0') {
                  dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * fVar26,
                                            SUB84(dVar24,0));
                }
                else {
                  dVar24 = dVar24 * dVar27;
                }
                if (in_FPSCR_SZ == '\0') {
                  pfVar17[6] = (float)((ulonglong)dVar24 >> 0x20);
                  dVar24 = (double)CONCAT44(pfVar17[0x1b],SUB84(dVar24,0));
                  dVar27 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),SUB84(dVar27,0));
                }
                else {
                  *(double *)(pfVar17 + 6) = dVar24;
                  dVar24 = *(double *)(pfVar17 + 0x1b);
                  dVar27 = dVar25;
                }
                fVar26 = (float)((ulonglong)dVar27 >> 0x20);
                if (in_FPSCR_PR == '\0') {
                  dVar24 = (double)CONCAT44((float)((ulonglong)dVar24 >> 0x20) * fVar26,
                                            SUB84(dVar24,0));
                }
                else {
                  dVar24 = dVar24 * dVar27;
                }
                if (in_FPSCR_SZ == '\0') {
                  pfVar17[0x1b] = (float)((ulonglong)dVar24 >> 0x20);
                  dVar22 = (double)CONCAT44(uVar29,pfVar17[0x1c]);
                }
                else {
                  *(double *)(pfVar17 + 0x1b) = dVar24;
                  in_xd0 = *(undefined8 *)(pfVar17 + 0x1c);
                }
                if (in_FPSCR_PR == '\0') {
                  fVar21 = SUB84(dVar22,0) * fVar26;
                }
                else {
                  fVar21 = SUB84(dVar22 * dVar27,0);
                }
                if (in_FPSCR_SZ == '\0') {
                  pfVar17[0x1c] = fVar21;
                  dVar22 = (double)CONCAT44(pfVar17[0x1d],SUB84(dVar24,0));
                }
                else {
                  *(undefined8 *)(pfVar17 + 0x1c) = in_xd0;
                  dVar22 = *(double *)(pfVar17 + 0x1d);
                }
                if (in_FPSCR_PR == '\0') {
                  dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) * fVar26,
                                            SUB84(dVar22,0));
                }
                else {
                  dVar22 = dVar22 * dVar27;
                }
                if (in_FPSCR_SZ == '\0') {
                  pfVar17[0x1d] = (float)((ulonglong)dVar22 >> 0x20);
                }
                else {
                  *(double *)(pfVar17 + 0x1d) = dVar22;
                }
                puVar14 = (undefined4 *)piVar20[5];
                uVar11 = puVar14[1];
                piVar20[10] = *puVar14;
                piVar20[0xb] = uVar11;
                piVar20[0xc] = puVar14[2];
                puVar10 = (uint *)piVar20[5];
                uVar8 = puVar16[1];
                *puVar10 = *puVar16;
                puVar10[1] = uVar8;
                puVar10[2] = puVar16[2];
                uVar8 = piVar20[0xb];
                *puVar16 = piVar20[10];
                puVar16[1] = uVar8;
                puVar16[2] = piVar20[0xc];
                *piVar20 = 0xffffffff;
              }
              else {
                fVar26 = SUB84(dVar30,0);
                if (in_FPSCR_SZ == '\0') {
                  pfVar17[6] = fVar26;
                  pfVar17[5] = fVar26;
                  pfVar17[4] = fVar26;
                }
                else {
                  *(undefined8 *)(pfVar17 + 6) = in_xd14;
                  *(undefined8 *)(pfVar17 + 5) = in_xd14;
                  *(undefined8 *)(pfVar17 + 4) = in_xd14;
                }
              }
              iVar15 = DAT_8c078180;
              uVar11 = SUB84(dVar27,0);
              if (in_FPSCR_SZ == '\0') {
                dVar27 = (double)CONCAT44(pfVar17[0x18],pfVar17[0x1b]);
              }
              else {
                in_xd2 = *(undefined8 *)(pfVar17 + 0x1b);
                dVar27 = *(double *)(pfVar17 + 0x18);
              }
              if (in_FPSCR_PR == '\0') {
                dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) + SUB84(dVar27,0),
                                          SUB84(dVar27,0));
              }
              else {
                dVar27 = dVar27 + dVar27;
              }
              if (in_FPSCR_SZ == '\0') {
                pfVar17[1] = (float)((ulonglong)dVar27 >> 0x20);
                dVar27 = (double)CONCAT44(pfVar17[0x19],pfVar17[0x1c]);
              }
              else {
                *(double *)(pfVar17 + 1) = dVar27;
                in_xd2 = *(undefined8 *)(pfVar17 + 0x1c);
                dVar27 = *(double *)(pfVar17 + 0x19);
              }
              if (in_FPSCR_PR == '\0') {
                dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) + SUB84(dVar27,0),
                                          SUB84(dVar27,0));
              }
              else {
                dVar27 = dVar27 + dVar27;
              }
              if (in_FPSCR_SZ == '\0') {
                pfVar17[2] = (float)((ulonglong)dVar27 >> 0x20);
                dVar27 = (double)CONCAT44(pfVar17[0x1a],pfVar17[0x1d]);
              }
              else {
                *(double *)(pfVar17 + 2) = dVar27;
                in_xd2 = *(undefined8 *)(pfVar17 + 0x1d);
                dVar27 = *(double *)(pfVar17 + 0x1a);
              }
              if (in_FPSCR_PR == '\0') {
                dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) + SUB84(dVar27,0),
                                          SUB84(dVar27,0));
              }
              else {
                dVar27 = dVar27 + dVar27;
              }
              if (in_FPSCR_SZ == '\0') {
                pfVar17[3] = (float)((ulonglong)dVar27 >> 0x20);
              }
              else {
                *(double *)(pfVar17 + 3) = dVar27;
              }
              uVar1 = *(ushort *)((int)pfVar17 + 0x2a);
              if (in_FPSCR_SZ == '\0') {
                dVar27 = (double)(ulonglong)(uint)pfVar17[7];
              }
              else {
                in_xd2 = *(undefined8 *)(pfVar17 + 7);
              }
              if (in_FPSCR_SZ == '\0') {
                piVar20[0x13] = SUB84(dVar27,0);
              }
              else {
                *(undefined8 *)(piVar20 + 0x13) = in_xd2;
              }
              iVar15 = (uint)uVar1 * 0x58 + iVar15;
              if (in_FPSCR_SZ == '\0') {
                piVar20[0x14] = (int)pfVar17[8];
                piVar20[0x15] = (int)pfVar17[9];
                piVar20[0x10] = *(undefined4 *)(iVar15 + 0x20);
                piVar20[0x11] = *(undefined4 *)(iVar15 + 0x24);
                piVar20[0x12] = *(undefined4 *)(iVar15 + 0x28);
              }
              else {
                *(undefined8 *)(piVar20 + 0x14) = *(undefined8 *)(pfVar17 + 8);
                *(undefined8 *)(piVar20 + 0x15) = *(undefined8 *)(pfVar17 + 9);
                *(undefined8 *)(piVar20 + 0x10) = *(undefined8 *)(iVar15 + 0x20);
                *(undefined8 *)(piVar20 + 0x11) = *(undefined8 *)(iVar15 + 0x24);
                in_xd2 = *(undefined8 *)(iVar15 + 0x28);
                *(undefined8 *)(piVar20 + 0x12) = in_xd2;
              }
              (*(code *)PTR_FUN_8c078184)(pfVar17 + 1,piVar20 + 0x10,piVar20 + 0xd);
              if (in_FPSCR_SZ == '\0') {
                dVar27 = (double)CONCAT44(piVar20[0x7f],piVar20[0xd]);
              }
              else {
                in_xd2 = *(undefined8 *)(piVar20 + 0xd);
                dVar27 = *(double *)(piVar20 + 0x7f);
              }
              if (in_FPSCR_PR == '\0') {
                dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) + SUB84(dVar27,0),
                                          SUB84(dVar27,0));
              }
              else {
                dVar27 = dVar27 + dVar27;
              }
              if (in_FPSCR_SZ == '\0') {
                pfVar17[1] = (float)((ulonglong)dVar27 >> 0x20);
                dVar27 = (double)CONCAT44(piVar20[0x80],piVar20[0xe]);
              }
              else {
                *(double *)(pfVar17 + 1) = dVar27;
                in_xd2 = *(undefined8 *)(piVar20 + 0xe);
                dVar27 = *(double *)(piVar20 + 0x80);
              }
              if (in_FPSCR_PR == '\0') {
                dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) + SUB84(dVar27,0),
                                          SUB84(dVar27,0));
              }
              else {
                dVar27 = dVar27 + dVar27;
              }
              if (in_FPSCR_SZ == '\0') {
                pfVar17[2] = (float)((ulonglong)dVar27 >> 0x20);
                dVar27 = (double)CONCAT44(piVar20[0x81],piVar20[0xf]);
              }
              else {
                *(double *)(pfVar17 + 2) = dVar27;
                in_xd2 = *(undefined8 *)(piVar20 + 0xf);
                dVar27 = *(double *)(piVar20 + 0x81);
              }
              if (in_FPSCR_PR == '\0') {
                dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) + SUB84(dVar27,0),
                                          SUB84(dVar27,0));
              }
              else {
                dVar27 = dVar27 + dVar27;
              }
              if (in_FPSCR_SZ == '\0') {
                pfVar17[3] = (float)((ulonglong)dVar27 >> 0x20);
              }
              else {
                *(double *)(pfVar17 + 3) = dVar27;
              }
              if (piVar20[3] == 0) {
                *(undefined4 *)(param_4 + 0x6c) = 0;
              }
              else {
                *(int *)(param_4 + 0x6c) = piVar20[3];
                (*(code *)PTR_FUN_8c078188)(param_2,param_4,pfVar17,piVar20 + 0x7f);
              }
              if (in_FPSCR_SZ == '\0') {
                dVar27 = (double)CONCAT44(pfVar17[1],piVar20[0x7f]);
              }
              else {
                in_xd2 = *(undefined8 *)(piVar20 + 0x7f);
                dVar27 = *(double *)(pfVar17 + 1);
              }
              if (in_FPSCR_PR == '\0') {
                dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) - SUB84(dVar27,0),
                                          SUB84(dVar27,0));
              }
              else {
                dVar27 = dVar27 - dVar27;
              }
              if (in_FPSCR_SZ == '\0') {
                piVar20[0x79] = (int)((ulonglong)dVar27 >> 0x20);
                dVar27 = (double)CONCAT44(pfVar17[2],piVar20[0x80]);
              }
              else {
                *(double *)(piVar20 + 0x79) = dVar27;
                in_xd2 = *(undefined8 *)(piVar20 + 0x80);
                dVar27 = *(double *)(pfVar17 + 2);
              }
              if (in_FPSCR_PR == '\0') {
                dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) - SUB84(dVar27,0),
                                          SUB84(dVar27,0));
              }
              else {
                dVar27 = dVar27 - dVar27;
              }
              if (in_FPSCR_SZ == '\0') {
                piVar20[0x7a] = (int)((ulonglong)dVar27 >> 0x20);
                dVar27 = (double)CONCAT44(pfVar17[3],piVar20[0x81]);
              }
              else {
                *(double *)(piVar20 + 0x7a) = dVar27;
                in_xd2 = *(undefined8 *)(piVar20 + 0x81);
                dVar27 = *(double *)(pfVar17 + 3);
              }
              if (in_FPSCR_PR == '\0') {
                dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) - SUB84(dVar27,0),
                                          SUB84(dVar27,0));
              }
              else {
                dVar27 = dVar27 - dVar27;
              }
              if (in_FPSCR_SZ == '\0') {
                piVar20[0x7b] = (int)((ulonglong)dVar27 >> 0x20);
              }
              else {
                *(double *)(piVar20 + 0x7b) = dVar27;
              }
              param_1 = (double)(*(code *)PTR_FUN_8c07818c)(param_2 + 0x20,piVar20 + 0x79);
              if (in_FPSCR_SZ == '\0') {
                param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),uVar11);
              }
              fVar26 = (float)((ulonglong)param_1 >> 0x20);
              if (piVar20[3] == 0) {
                if (in_FPSCR_SZ == '\0') {
                  dVar27 = (double)((ulonglong)*(uint *)(param_4 + 0x38) << 0x20);
                }
                else {
                  dVar27 = *(double *)(param_4 + 0x38);
                }
                if ((float)((ulonglong)dVar27 >> 0x20) != SUB84(dVar27,0)) goto LAB_8c078120;
                if (in_FPSCR_SZ == '\0') {
                  dVar27 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x40),SUB84(dVar27,0));
                }
                else {
                  dVar27 = *(double *)(param_4 + 0x40);
                }
                fVar21 = (float)((ulonglong)dVar27 >> 0x20);
                if (fVar21 != SUB84(dVar27,0)) goto LAB_8c078120;
                dVar27 = (double)CONCAT44(fVar21,0x3f800000);
                if (in_FPSCR_PR == '\0') {
                  bVar31 = 1.0 < fVar26;
                }
                else {
                  bVar31 = dVar27 < param_1;
                }
                if (bVar31) goto LAB_8c078120;
              }
              else {
LAB_8c078120:
                if (in_FPSCR_SZ == '\0') {
                  dVar27 = (double)CONCAT44((int)((ulonglong)dVar27 >> 0x20),DAT_8c078190);
                }
                else {
                  in_xd2 = CONCAT44(PTR_FUN_8c078194,DAT_8c078190);
                }
                if (in_FPSCR_PR == '\0') {
                  bVar31 = SUB84(dVar27,0) < fVar26;
                }
                else {
                  bVar31 = dVar27 < param_1;
                }
                if (bVar31) {
                  uVar11 = piVar20[0x7a];
                  *(int *)(param_2 + 0x20) = piVar20[0x79];
                  *(undefined4 *)(param_2 + 0x24) = uVar11;
                  *(int *)(param_2 + 0x28) = piVar20[0x7b];
                }
              }
              puVar4 = PTR_FUN_8c078194;
              *(ushort *)(param_2 + 4) = *(ushort *)(param_2 + 4) | 1;
              (*(code *)puVar4)(param_2,param_3,param_4,*puVar16);
              iVar15 = iVar7;
              goto LAB_8c0781c8;
            }
          }
        }
        else if (((*puVar16 & DAT_8c077d94) != 0) &&
                (cVar5 = (*(code *)PTR_FUN_8c077d98)(), cVar5 == '\x03')) {
          if (in_FPSCR_SZ == '\0') {
            dVar27 = (double)CONCAT44(DAT_8c077d9c,pfVar17[0x13]);
          }
          else {
            in_xd2 = *(undefined8 *)(pfVar17 + 0x13);
            dVar27 = (double)CONCAT44(_DAT_8c077da0,DAT_8c077d9c);
          }
          if (in_FPSCR_PR == '\0' && (float)((ulonglong)dVar27 >> 0x20) < SUB84(dVar27,0))
          goto LAB_8c077cda;
        }
      }
LAB_8c0781a0:
      piVar20[-1] = *puVar16;
      piVar20[-2] = iVar15;
      piVar20[-3] = (int)(piVar20 + 0x7f);
      piVar20[-4] = (int)(piVar20 + 0x76);
      iVar15 = (*(code *)PTR_FUN_8c078380)(param_2,param_3,param_4,pfVar17);
    }
LAB_8c0781c8:
    *piVar20 = *piVar20 + 1;
  }
  if (piVar20[8] == 1) {
    *(int *)(param_4 + 0x74) = piVar20[9];
    if (in_FPSCR_SZ == '\0') {
      dVar27 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x20),pfVar17[4]);
    }
    else {
      dVar27 = *(double *)(param_2 + 0x20);
    }
    if (in_FPSCR_PR == '\0') {
      dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) + SUB84(dVar27,0),SUB84(dVar27,0)
                               );
    }
    else {
      dVar27 = dVar27 + dVar27;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_2 + 0x20) = (int)((ulonglong)dVar27 >> 0x20);
      dVar27 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x24),pfVar17[5]);
    }
    else {
      *(double *)(param_2 + 0x20) = dVar27;
      dVar27 = *(double *)(param_2 + 0x24);
    }
    if (in_FPSCR_PR == '\0') {
      dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) + SUB84(dVar27,0),SUB84(dVar27,0)
                               );
    }
    else {
      dVar27 = dVar27 + dVar27;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_2 + 0x24) = (int)((ulonglong)dVar27 >> 0x20);
      dVar27 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x28),pfVar17[6]);
    }
    else {
      *(double *)(param_2 + 0x24) = dVar27;
      dVar27 = *(double *)(param_2 + 0x28);
    }
    if (in_FPSCR_PR == '\0') {
      dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) + SUB84(dVar27,0),SUB84(dVar27,0)
                               );
    }
    else {
      dVar27 = dVar27 + dVar27;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_2 + 0x28) = (int)((ulonglong)dVar27 >> 0x20);
      return iVar15;
    }
    *(double *)(param_2 + 0x28) = dVar27;
    return iVar15;
  }
  if (in_FPSCR_SZ == '\0') {
    piVar20[6] = SUB84(dVar30,0);
  }
  else {
    *(undefined8 *)(piVar20 + 6) = in_xd14;
  }
  sVar2 = *(short *)(param_4 + 0x10);
  piVar20[1] = (int)sVar2;
  if ((((int)sVar2 & 1U) != 0) && ((piVar20[1] & 0x4000U) == 0)) {
    iVar7 = (*(code *)PTR_FUN_8c078384)(param_2,0,piVar20 + 6);
    if (iVar7 != 0) {
      if (in_FPSCR_SZ == '\0') {
        uVar23 = CONCAT44(piVar20[6],(undefined4)DAT_8c078388);
      }
      else {
        uVar23 = *(undefined8 *)(piVar20 + 6);
      }
      if ((in_FPSCR_PR == '\0' && (float)uVar23 < (float)((ulonglong)uVar23 >> 0x20)) ||
         (*(short *)(param_4 + 10) != 0)) goto LAB_8c07827a;
    }
    if (in_FPSCR_SZ == '\0') {
      lVar28 = (ulonglong)*(uint *)(param_4 + 0x3c) << 0x20;
    }
    else {
      lVar28 = *(longlong *)(param_4 + 0x3c);
    }
    if (in_FPSCR_PR != '\0' || (float)((ulonglong)lVar28 >> 0x20) <= (float)lVar28) {
      if (in_FPSCR_SZ == '\0') {
        dVar27 = (double)CONCAT44(*(undefined4 *)(param_4 + 0xc4),(float)lVar28);
        dVar25 = (double)CONCAT44((int)((ulonglong)extraout_dr0 >> 0x20),
                                  *(undefined4 *)(param_4 + 0x38));
      }
      else {
        dVar27 = *(double *)(param_4 + 0xc4);
        dVar25 = extraout_dr0;
      }
      if (in_FPSCR_PR == '\0') {
        bVar31 = (float)((ulonglong)dVar27 >> 0x20) < SUB84(dVar25,0);
      }
      else {
        bVar31 = dVar27 < dVar25;
      }
      if (!bVar31) {
        if (in_FPSCR_SZ == '\0') {
          dVar27 = (double)CONCAT44(DAT_8c078388._4_4_,SUB84(dVar27,0));
          dVar25 = (double)CONCAT44((int)((ulonglong)dVar25 >> 0x20),*(undefined4 *)(param_4 + 0x1c)
                                   );
        }
        else {
          dVar27 = (double)CONCAT44(PTR_FUN_8c078390,DAT_8c078388._4_4_);
        }
        if (in_FPSCR_PR == '\0') {
          bVar31 = (float)((ulonglong)dVar27 >> 0x20) < SUB84(dVar25,0);
        }
        else {
          bVar31 = dVar27 < dVar25;
        }
        if (bVar31) {
          if (in_FPSCR_SZ == '\0') {
            dVar27 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x20),*(undefined4 *)(param_3 + 4));
          }
          else {
            dVar27 = *(double *)(param_2 + 0x20);
          }
          fVar26 = (float)((ulonglong)dVar30 >> 0x20);
          if (in_FPSCR_PR == '\0') {
            fVar21 = SUB84(dVar27,0) * fVar26;
            dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) - fVar21,fVar21);
          }
          else {
            dVar27 = dVar27 * dVar30 - dVar27 * dVar30;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)(param_2 + 0x20) = (int)((ulonglong)dVar27 >> 0x20);
            dVar27 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x24),*(undefined4 *)(param_3 + 8));
          }
          else {
            *(double *)(param_2 + 0x20) = dVar27;
            dVar27 = *(double *)(param_2 + 0x24);
          }
          if (in_FPSCR_PR == '\0') {
            fVar21 = SUB84(dVar27,0) * fVar26;
            dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) - fVar21,fVar21);
          }
          else {
            dVar27 = dVar27 * dVar30 - dVar27 * dVar30;
          }
          if (in_FPSCR_SZ == '\0') {
            *(int *)(param_2 + 0x24) = (int)((ulonglong)dVar27 >> 0x20);
            dVar27 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x28),*(undefined4 *)(param_3 + 0xc)
                                     );
          }
          else {
            *(double *)(param_2 + 0x24) = dVar27;
            dVar27 = *(double *)(param_2 + 0x28);
          }
          if (in_FPSCR_PR == '\0') {
            fVar26 = SUB84(dVar27,0) * fVar26;
            dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) - fVar26,fVar26);
          }
          else {
            dVar27 = dVar27 * dVar30 - dVar27 * dVar30;
          }
          uVar11 = SUB84(dVar30,0);
          if (in_FPSCR_SZ == '\0') {
            *(int *)(param_2 + 0x28) = (int)((ulonglong)dVar27 >> 0x20);
            *(undefined4 *)(param_3 + 4) = uVar11;
            *(undefined4 *)(param_3 + 8) = uVar11;
            *(undefined4 *)(param_3 + 0xc) = uVar11;
          }
          else {
            *(double *)(param_2 + 0x28) = dVar27;
            *(undefined8 *)(param_3 + 4) = in_xd14;
            *(undefined8 *)(param_3 + 8) = in_xd14;
            *(undefined8 *)(param_3 + 0xc) = in_xd14;
          }
          goto LAB_8c0782a4;
        }
      }
    }
  }
LAB_8c07827a:
  if (in_FPSCR_SZ == '\0') {
    dVar27 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x20),pfVar17[4]);
  }
  else {
    dVar27 = *(double *)(param_2 + 0x20);
  }
  if (in_FPSCR_PR == '\0') {
    dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) + SUB84(dVar27,0),SUB84(dVar27,0));
  }
  else {
    dVar27 = dVar27 + dVar27;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_2 + 0x20) = (int)((ulonglong)dVar27 >> 0x20);
    dVar27 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x24),pfVar17[5]);
  }
  else {
    *(double *)(param_2 + 0x20) = dVar27;
    dVar27 = *(double *)(param_2 + 0x24);
  }
  if (in_FPSCR_PR == '\0') {
    dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) + SUB84(dVar27,0),SUB84(dVar27,0));
  }
  else {
    dVar27 = dVar27 + dVar27;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_2 + 0x24) = (int)((ulonglong)dVar27 >> 0x20);
    dVar27 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x28),pfVar17[6]);
  }
  else {
    *(double *)(param_2 + 0x24) = dVar27;
    dVar27 = *(double *)(param_2 + 0x28);
  }
  if (in_FPSCR_PR == '\0') {
    dVar27 = (double)CONCAT44((float)((ulonglong)dVar27 >> 0x20) + SUB84(dVar27,0),SUB84(dVar27,0));
  }
  else {
    dVar27 = dVar27 + dVar27;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_2 + 0x28) = (int)((ulonglong)dVar27 >> 0x20);
  }
  else {
    *(double *)(param_2 + 0x28) = dVar27;
  }
LAB_8c0782a4:
  (*(code *)PTR_FUN_8c078390)(param_2,param_3,param_4,0);
  return iVar15;
}

