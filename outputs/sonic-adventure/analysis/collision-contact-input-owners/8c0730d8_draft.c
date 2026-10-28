/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c073ac4) */
/* WARNING: Removing unreachable block (ram,0x8c073abc) */
/* WARNING: Removing unreachable block (ram,0x8c07373e) */
/* WARNING: Removing unreachable block (ram,0x8c074004) */
/* WARNING: Removing unreachable block (ram,0x8c073812) */
/* WARNING: Removing unreachable block (ram,0x8c07400c) */
/* WARNING: Removing unreachable block (ram,0x8c073858) */
/* WARNING: Removing unreachable block (ram,0x8c073b7a) */
/* WARNING: Removing unreachable block (ram,0x8c073b82) */
/* WARNING: Removing unreachable block (ram,0x8c073d9a) */
/* WARNING: Removing unreachable block (ram,0x8c073df0) */
/* WARNING: Removing unreachable block (ram,0x8c073dfa) */
/* WARNING: Removing unreachable block (ram,0x8c073e00) */
/* WARNING: Removing unreachable block (ram,0x8c073e02) */
/* WARNING: Removing unreachable block (ram,0x8c073e04) */
/* WARNING: Removing unreachable block (ram,0x8c073e16) */
/* WARNING: Removing unreachable block (ram,0x8c073e1e) */
/* WARNING: Removing unreachable block (ram,0x8c073e24) */
/* WARNING: Removing unreachable block (ram,0x8c073e26) */
/* WARNING: Removing unreachable block (ram,0x8c073e28) */
/* WARNING: Removing unreachable block (ram,0x8c073e2a) */
/* WARNING: Removing unreachable block (ram,0x8c073e2c) */
/* WARNING: Removing unreachable block (ram,0x8c073e32) */
/* WARNING: Removing unreachable block (ram,0x8c073e34) */
/* WARNING: Removing unreachable block (ram,0x8c073e36) */
/* WARNING: Removing unreachable block (ram,0x8c073e3c) */
/* WARNING: Removing unreachable block (ram,0x8c073e3e) */
/* WARNING: Removing unreachable block (ram,0x8c073e40) */
/* WARNING: Removing unreachable block (ram,0x8c073e4e) */
/* WARNING: Removing unreachable block (ram,0x8c073e50) */
/* WARNING: Removing unreachable block (ram,0x8c073e5a) */
/* WARNING: Removing unreachable block (ram,0x8c073e5e) */
/* WARNING: Removing unreachable block (ram,0x8c073e60) */
/* WARNING: Removing unreachable block (ram,0x8c073e62) */
/* WARNING: Removing unreachable block (ram,0x8c073e6a) */
/* WARNING: Removing unreachable block (ram,0x8c073e6c) */
/* WARNING: Removing unreachable block (ram,0x8c073e6e) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_8c0730d8(double param_1,char *param_2,int param_3,int param_4)

{
  ushort uVar1;
  undefined *puVar2;
  short sVar3;
  int iVar4;
  undefined4 uVar5;
  undefined4 *puVar6;
  float fVar7;
  undefined4 uVar8;
  undefined4 *puVar9;
  undefined4 *puVar10;
  undefined4 *puVar11;
  uint *puVar12;
  float *pfVar13;
  undefined8 *puVar14;
  int *piVar15;
  int *piVar16;
  undefined8 *puVar17;
  undefined8 *puVar18;
  undefined8 uVar19;
  double dVar20;
  double extraout_dr0;
  float fVar21;
  double in_dr2;
  double dVar22;
  double dVar23;
  longlong lVar24;
  undefined8 in_dr12;
  undefined8 in_dr14;
  double dVar25;
  undefined8 in_xd2;
  undefined8 in_xd12;
  undefined8 in_xd14;
  bool bVar26;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined8 local_3c;
  undefined8 local_34;
  undefined8 uStack_2c;
  undefined8 uStack_24;
  
  sVar3 = (*(code *)PTR_FUN_8c073140)();
  if ((sVar3 == 0x902) || (sVar3 = (*(code *)PTR_FUN_8c073140)(), sVar3 == 0x903)) {
    iVar4 = (*(code *)PTR_FUN_8c073144)(param_2,param_3,param_4);
    return iVar4;
  }
  if (in_FPSCR_SZ == '\0') {
    uStack_24 = CONCAT44((int)in_dr14,(undefined4)uStack_24);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_24._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
    puVar14 = &uStack_2c;
    uStack_2c = CONCAT44((int)in_dr12,(int)((ulonglong)in_dr12 >> 0x20));
  }
  else {
    uStack_24 = in_xd14;
    uStack_2c = in_dr14;
    local_34 = in_xd12;
    puVar14 = &local_3c;
    local_3c = in_dr12;
  }
  *(undefined4 *)((int)puVar14 + -4) = in_PR;
  piVar16 = (int *)((int)puVar14 + -0x234);
  pfVar13 = *(float **)(param_4 + 100);
  uVar19 = 0;
  *(undefined4 *)((int)puVar14 + -0x220) = 0;
  *(undefined4 *)PTR_DAT_8c0733a0 = 0;
  *(int *)((int)puVar14 + -0x214) = (int)*(short *)(param_4 + 0x10);
  *(undefined2 *)(param_4 + 0x10) = *(undefined2 *)(param_2 + 4);
  *(ushort *)(param_2 + 4) = *(ushort *)(param_2 + 4) & 0xfffe;
  if (in_FPSCR_SZ == '\0') {
    pfVar13[9] = 0.0;
    pfVar13[7] = 0.0;
    pfVar13[8] = 1.0;
  }
  else {
    *(undefined8 *)(pfVar13 + 9) = in_xd14;
    *(undefined8 *)(pfVar13 + 7) = in_xd14;
    *(undefined8 *)(pfVar13 + 8) = in_xd12;
  }
  pfVar13[0xd] = *(float *)(param_2 + 0x14);
  pfVar13[0xe] = *(float *)(param_2 + 0x1c);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)((int)puVar14 + -8) = 0;
    *(undefined4 *)((int)puVar14 + -0x10) = 0;
    dVar23 = (double)CONCAT44(DAT_8c0733a4,0x3f800000);
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(param_4 + 0x110));
  }
  else {
    *(undefined8 *)((int)puVar14 + -8) = in_xd14;
    *(undefined8 *)((int)puVar14 + -0x10) = in_xd14;
    dVar23 = (double)CONCAT44(PTR_FUN_8c0733a8,DAT_8c0733a4);
    in_xd2 = *(undefined8 *)(param_4 + 0x110);
  }
  if (in_FPSCR_PR == '\0') {
    fVar21 = SUB84(in_dr2,0) * (float)((ulonglong)dVar23 >> 0x20);
  }
  else {
    fVar21 = SUB84(in_dr2 * dVar23,0);
  }
  if (in_FPSCR_SZ == '\0') {
    *(float *)((int)puVar14 + -0xc) = fVar21;
  }
  else {
    *(undefined8 *)((int)puVar14 + -0xc) = in_xd2;
  }
  *(undefined4 *)((int)puVar14 + -0x1c) = *(undefined4 *)((int)puVar14 + -0x10);
  *(undefined4 *)((int)puVar14 + -0x18) = *(undefined4 *)((int)puVar14 + -0xc);
  *(undefined4 *)((int)puVar14 + -0x14) = *(undefined4 *)((int)puVar14 + -8);
  if ((*(uint *)(param_4 + 0x60) & 0x4000) == 0) {
    (*(code *)PTR_FUN_8c0733b0)(param_2,pfVar13 + 7);
    (*(code *)PTR_FUN_8c0733b0)(param_2,(undefined1 *)((int)puVar14 + -0x10));
    (*(code *)PTR_FUN_8c0733ac)(param_3,param_4,pfVar13,(undefined1 *)((int)puVar14 + -0x1c));
    (*(code *)PTR_FUN_8c0733b0)(param_2,(undefined1 *)((int)puVar14 + -0x1c));
  }
  else {
    (*(code *)PTR_FUN_8c0733a8)(param_4 + 0x50,pfVar13 + 7,*(undefined4 *)(param_2 + 0x18));
    (*(code *)PTR_FUN_8c0733a8)
              (param_4 + 0x50,(undefined1 *)((int)puVar14 + -0x10),*(undefined4 *)(param_2 + 0x18));
    (*(code *)PTR_FUN_8c0733ac)(param_3,param_4,pfVar13,(undefined1 *)((int)puVar14 + -0x1c));
    (*(code *)PTR_FUN_8c0733a8)
              (param_4 + 0x50,(undefined1 *)((int)puVar14 + -0x1c),*(undefined4 *)(param_2 + 0x18));
  }
  if (in_FPSCR_SZ == '\0') {
    dVar22 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x20),*(undefined4 *)((int)puVar14 + -0x1c))
    ;
  }
  else {
    in_xd2 = *(undefined8 *)((int)puVar14 + -0x1c);
    dVar22 = *(double *)(param_2 + 0x20);
  }
  if (in_FPSCR_PR == '\0') {
    dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),SUB84(dVar22,0));
  }
  else {
    dVar22 = dVar22 + dVar22;
  }
  if (in_FPSCR_SZ == '\0') {
    pfVar13[1] = (float)((ulonglong)dVar22 >> 0x20);
    dVar22 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x24),*(undefined4 *)((int)puVar14 + -0x18))
    ;
  }
  else {
    *(double *)(pfVar13 + 1) = dVar22;
    in_xd2 = *(undefined8 *)((int)puVar14 + -0x18);
    dVar22 = *(double *)(param_2 + 0x24);
  }
  if (in_FPSCR_PR == '\0') {
    dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),SUB84(dVar22,0));
  }
  else {
    dVar22 = dVar22 + dVar22;
  }
  if (in_FPSCR_SZ == '\0') {
    pfVar13[2] = (float)((ulonglong)dVar22 >> 0x20);
    dVar22 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x28),*(undefined4 *)((int)puVar14 + -0x14))
    ;
  }
  else {
    *(double *)(pfVar13 + 2) = dVar22;
    in_xd2 = *(undefined8 *)((int)puVar14 + -0x14);
    dVar22 = *(double *)(param_2 + 0x28);
  }
  if (in_FPSCR_PR == '\0') {
    dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),SUB84(dVar22,0));
  }
  else {
    dVar22 = dVar22 + dVar22;
  }
  fVar21 = (float)((ulonglong)dVar22 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    pfVar13[3] = fVar21;
    dVar22 = (double)CONCAT44(fVar21,*(undefined4 *)(param_4 + 0x110));
  }
  else {
    *(double *)(pfVar13 + 3) = dVar22;
    in_xd2 = *(undefined8 *)(param_4 + 0x110);
  }
  if (in_FPSCR_PR == '\0') {
    fVar21 = SUB84(dVar22,0) * (float)((ulonglong)dVar23 >> 0x20);
  }
  else {
    fVar21 = SUB84(dVar22 * dVar23,0);
  }
  if (in_FPSCR_SZ == '\0') {
    *pfVar13 = fVar21;
  }
  else {
    *(undefined8 *)pfVar13 = in_xd2;
  }
  fVar21 = *(float *)(param_3 + 8);
  pfVar13[4] = *(float *)(param_3 + 4);
  pfVar13[5] = fVar21;
  puVar2 = PTR_FUN_8c0733b4;
  pfVar13[6] = *(float *)(param_3 + 0xc);
  uVar8 = *(undefined4 *)(param_2 + 0x24);
  *(undefined4 *)((int)puVar14 + -0x34) = *(undefined4 *)(param_2 + 0x20);
  *(undefined4 *)((int)puVar14 + -0x30) = uVar8;
  *(undefined4 *)((int)puVar14 + -0x2c) = *(undefined4 *)(param_2 + 0x28);
  (*(code *)puVar2)(pfVar13);
  if (*DAT_8c0733b8 == 0) {
    if (in_FPSCR_SZ == '\0') {
      dVar23 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x20),*(undefined4 *)(param_3 + 4));
    }
    else {
      dVar23 = *(double *)(param_2 + 0x20);
    }
    if (in_FPSCR_PR == '\0') {
      dVar23 = (double)CONCAT44((float)((ulonglong)dVar23 >> 0x20) + SUB84(dVar23,0),SUB84(dVar23,0)
                               );
    }
    else {
      dVar23 = dVar23 + dVar23;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_2 + 0x20) = (int)((ulonglong)dVar23 >> 0x20);
      dVar23 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x24),*(undefined4 *)(param_3 + 8));
    }
    else {
      *(double *)(param_2 + 0x20) = dVar23;
      dVar23 = *(double *)(param_2 + 0x24);
    }
    if (in_FPSCR_PR == '\0') {
      dVar23 = (double)CONCAT44((float)((ulonglong)dVar23 >> 0x20) + SUB84(dVar23,0),SUB84(dVar23,0)
                               );
    }
    else {
      dVar23 = dVar23 + dVar23;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_2 + 0x24) = (int)((ulonglong)dVar23 >> 0x20);
      dVar23 = (double)(ulonglong)*(uint *)(param_3 + 0xc);
    }
    else {
      *(double *)(param_2 + 0x24) = dVar23;
    }
LAB_8c073470:
    puVar2 = PTR_FUN_8c07358c;
    if (in_FPSCR_SZ == '\0') {
      dVar23 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x28),SUB84(dVar23,0));
    }
    else {
      dVar23 = *(double *)(param_2 + 0x28);
    }
    if (in_FPSCR_PR == '\0') {
      dVar23 = (double)CONCAT44((float)((ulonglong)dVar23 >> 0x20) + SUB84(dVar23,0),SUB84(dVar23,0)
                               );
    }
    else {
      dVar23 = dVar23 + dVar23;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_2 + 0x28) = (int)((ulonglong)dVar23 >> 0x20);
    }
    else {
      *(double *)(param_2 + 0x28) = dVar23;
    }
    (*(code *)puVar2)(param_2,param_3,param_4,0);
    *(undefined4 *)(param_4 + 0x6c) = 0;
    *(undefined4 *)(param_4 + 0x74) = 0;
    return 0;
  }
  *(undefined4 *)((int)puVar14 + -0x1e8) = 0;
  bVar26 = false;
  *(undefined4 *)((int)puVar14 + -0x234) = DAT_8c0733bc;
  *(undefined4 *)((int)puVar14 + -0x1fc) = 0;
  *(undefined4 *)((int)puVar14 + -0x230) = 0;
  *(undefined4 *)((int)puVar14 + -0x22c) = 0;
  *(undefined4 *)((int)puVar14 + -0x224) = 0;
  while (uVar8 = (undefined4)uVar19, *(int *)((int)puVar14 + -0x224) < (int)*DAT_8c073588) {
    if ((((**(uint **)((int)puVar14 + -0x234) & DAT_8c0733c0) == 0) || (param_2[9] == '\x06')) &&
       ((*(int *)(*(int *)((int)puVar14 + -0x234) + 8) == 0 ||
        ((*(ushort *)(*(int *)(*(int *)(*(int *)((int)puVar14 + -0x234) + 8) + 0x20) + 4) & 0x100)
         != 0)))) {
      iVar4 = (*(code *)PTR_FUN_8c0733c4)
                        (pfVar13,*(undefined4 *)(*(int *)((int)puVar14 + -0x234) + 4),
                         **(undefined4 **)((int)puVar14 + -0x234));
      if (iVar4 != 0) {
        if ((*(ushort *)(param_4 + 0x10) & 3) == 0) {
          iVar4 = (*(code *)PTR_FUN_8c073580)(param_2,param_3,param_4,pfVar13);
          if (iVar4 == 1) {
            bVar26 = true;
            *(undefined4 *)((int)puVar14 + -0x230) = *(undefined4 *)((int)puVar14 + -0x22c);
          }
        }
        else {
          *(undefined4 *)((int)puVar14 + -0x238) = **(undefined4 **)((int)puVar14 + -0x234);
          iVar4 = (*(code *)PTR_FUN_8c0733c8)(param_2,param_3,param_4,pfVar13);
          if (iVar4 == 1) {
            bVar26 = true;
            *(undefined4 *)((int)puVar14 + -0x230) = *(undefined4 *)((int)puVar14 + -0x22c);
          }
        }
      }
      puVar6 = *(undefined4 **)((int)puVar14 + -0x234);
      puVar9 = (undefined4 *)((int)puVar14 + *(int *)((int)puVar14 + -0x22c) * 0xc + -0xf4);
      uVar8 = puVar6[1];
      *puVar9 = *puVar6;
      puVar9[1] = uVar8;
      puVar9[2] = puVar6[2];
      *(int *)((int)puVar14 + -0x22c) = *(int *)((int)puVar14 + -0x22c) + 1;
      if ((**(uint **)((int)puVar14 + -0x234) & 0x4000) != 0) {
        *(undefined4 *)PTR_DAT_8c073584 = 1;
      }
    }
    *(int *)((int)puVar14 + -0x224) = *(int *)((int)puVar14 + -0x224) + 1;
    *(int *)((int)puVar14 + -0x234) = *(int *)((int)puVar14 + -0x234) + 0xc;
  }
  if (*(int *)((int)puVar14 + -0x22c) == 0) {
    if (in_FPSCR_SZ == '\0') {
      dVar23 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x20),*(undefined4 *)(param_3 + 4));
    }
    else {
      dVar23 = *(double *)(param_2 + 0x20);
    }
    if (in_FPSCR_PR == '\0') {
      dVar23 = (double)CONCAT44((float)((ulonglong)dVar23 >> 0x20) + SUB84(dVar23,0),SUB84(dVar23,0)
                               );
    }
    else {
      dVar23 = dVar23 + dVar23;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_2 + 0x20) = (int)((ulonglong)dVar23 >> 0x20);
      dVar23 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x24),*(undefined4 *)(param_3 + 8));
    }
    else {
      *(double *)(param_2 + 0x20) = dVar23;
      dVar23 = *(double *)(param_2 + 0x24);
    }
    if (in_FPSCR_PR == '\0') {
      dVar23 = (double)CONCAT44((float)((ulonglong)dVar23 >> 0x20) + SUB84(dVar23,0),SUB84(dVar23,0)
                               );
    }
    else {
      dVar23 = dVar23 + dVar23;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_2 + 0x24) = (int)((ulonglong)dVar23 >> 0x20);
      dVar23 = (double)(ulonglong)*(uint *)(param_3 + 0xc);
    }
    else {
      *(double *)(param_2 + 0x24) = dVar23;
    }
    goto LAB_8c073470;
  }
  iVar4 = (*(code *)PTR_FUN_8c073590)();
  if (iVar4 != 0) {
    *(undefined4 *)PTR_DAT_8c073584 = 0;
  }
  puVar6 = (undefined4 *)((int)puVar14 + -0x1b4);
  if (bVar26) {
    puVar9 = (undefined4 *)((int)puVar14 + *(int *)((int)puVar14 + -0x230) * 0xc + -0xf4);
    uVar5 = puVar9[1];
    *puVar6 = *puVar9;
    *(undefined4 *)((int)puVar14 + -0x1b0) = uVar5;
    *(undefined4 *)((int)puVar14 + -0x1ac) = puVar9[2];
  }
  *(int *)((int)puVar14 + -0x224) = *(int *)((int)puVar14 + -0x22c) + -1;
  puVar9 = puVar6 + (uint)bVar26 * 3;
  if (*(int *)(param_4 + 0x6c) == 0) {
    iVar4 = *(int *)((int)puVar14 + -0x224);
    *(undefined4 **)((int)puVar14 + -0x234) = puVar6 + iVar4 * 3;
    for (; -1 < iVar4; iVar4 = iVar4 + -1) {
      if ((bVar26) && (iVar4 == *(int *)((int)puVar14 + -0x230))) {
        bVar26 = false;
      }
      else {
        puVar11 = (undefined4 *)((int)puVar14 + iVar4 * 0xc + -0xf4);
        if (*(int *)(param_4 + 0x74) == puVar11[1]) {
          puVar10 = *(undefined4 **)((int)puVar14 + -0x234);
          uVar5 = puVar11[1];
          *puVar10 = *puVar11;
          puVar10[1] = uVar5;
          puVar10[2] = puVar11[2];
        }
        else {
          uVar5 = puVar11[1];
          *puVar9 = *puVar11;
          puVar9[1] = uVar5;
          puVar9[2] = puVar11[2];
          puVar9 = puVar9 + 3;
        }
      }
    }
  }
  else {
    iVar4 = 0;
    *(undefined4 **)((int)puVar14 + -0x234) = puVar6 + *(int *)((int)puVar14 + -0x224) * 3;
    if (0 < *(int *)((int)puVar14 + -0x22c)) {
      do {
        if ((bVar26) && (iVar4 == *(int *)((int)puVar14 + -0x230))) {
          bVar26 = false;
        }
        else {
          puVar11 = (undefined4 *)((int)puVar14 + iVar4 * 0xc + -0xf4);
          if (*(int *)(param_4 + 0x74) == puVar11[1]) {
            puVar10 = *(undefined4 **)((int)puVar14 + -0x234);
            uVar5 = puVar11[1];
            *puVar10 = *puVar11;
            puVar10[1] = uVar5;
            puVar10[2] = puVar11[2];
          }
          else {
            uVar5 = puVar11[1];
            *puVar9 = *puVar11;
            puVar9[1] = uVar5;
            puVar9[2] = puVar11[2];
            puVar9 = puVar9 + 3;
          }
        }
        iVar4 = iVar4 + 1;
      } while (iVar4 < *(int *)((int)puVar14 + -0x22c));
    }
  }
  if (in_FPSCR_SZ == '\0') {
    dVar22 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x20),*(undefined4 *)((int)puVar14 + -0x1c))
    ;
  }
  else {
    in_xd2 = *(undefined8 *)((int)puVar14 + -0x1c);
    dVar22 = *(double *)(param_2 + 0x20);
  }
  if (in_FPSCR_PR == '\0') {
    dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),SUB84(dVar22,0));
  }
  else {
    dVar22 = dVar22 + dVar22;
  }
  if (in_FPSCR_SZ == '\0') {
    pfVar13[1] = (float)((ulonglong)dVar22 >> 0x20);
    dVar22 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x24),*(undefined4 *)((int)puVar14 + -0x18))
    ;
  }
  else {
    *(double *)(pfVar13 + 1) = dVar22;
    in_xd2 = *(undefined8 *)((int)puVar14 + -0x18);
    dVar22 = *(double *)(param_2 + 0x24);
  }
  if (in_FPSCR_PR == '\0') {
    dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),SUB84(dVar22,0));
  }
  else {
    dVar22 = dVar22 + dVar22;
  }
  if (in_FPSCR_SZ == '\0') {
    pfVar13[2] = (float)((ulonglong)dVar22 >> 0x20);
    dVar22 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x28),*(undefined4 *)((int)puVar14 + -0x14))
    ;
  }
  else {
    *(double *)(pfVar13 + 2) = dVar22;
    in_xd2 = *(undefined8 *)((int)puVar14 + -0x14);
    dVar22 = *(double *)(param_2 + 0x28);
  }
  if (in_FPSCR_PR == '\0') {
    dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),SUB84(dVar22,0));
  }
  else {
    dVar22 = dVar22 + dVar22;
  }
  if (in_FPSCR_SZ == '\0') {
    pfVar13[3] = (float)((ulonglong)dVar22 >> 0x20);
  }
  else {
    *(double *)(pfVar13 + 3) = dVar22;
  }
  fVar21 = *(float *)(param_3 + 8);
  pfVar13[4] = *(float *)(param_3 + 4);
  pfVar13[5] = fVar21;
  pfVar13[6] = *(float *)(param_3 + 0xc);
  *(undefined4 *)(param_4 + 0x6c) = 0;
  *(undefined4 **)((int)puVar14 + -0x200) = puVar6;
  *(undefined4 *)((int)puVar14 + -0x234) = 0;
  *(uint *)((int)puVar14 + -0x1ec) = (uint)((*(uint *)((int)puVar14 + -0x214) & 3) == 0);
  *(int *)((int)puVar14 + -0x214) =
       *(int *)((int)puVar14 + -0x224) * 0xc + *(int *)((int)puVar14 + -0x200);
  if (in_FPSCR_SZ == '\0') {
    dVar25 = (double)CONCAT44(_DAT_8c0737c0,uVar8);
    piVar16 = (int *)((int)puVar14 + -0x234);
  }
  else {
    dVar25 = (double)CONCAT44(PTR_FUN_8c0737c4,_DAT_8c0737c0);
  }
  while (*piVar16 < piVar16[2]) {
    puVar12 = (uint *)(*piVar16 * 0xc + piVar16[0xd]);
    piVar16[4] = puVar12[2];
    iVar4 = (*(code *)PTR_FUN_8c0737c4)(pfVar13,puVar12[1],*puVar12);
    if (iVar4 != 0) {
      piVar16[1] = 1;
      if (((*(ushort *)(param_2 + 4) & 1) != 0) && (piVar16[0xe] != puVar12[1])) {
        if (*(int *)PTR_DAT_8c0737c8 == 1) {
          if (in_FPSCR_SZ == '\0') {
            dVar22 = (double)CONCAT44(pfVar13[0x15],pfVar13[0x1b]);
          }
          else {
            in_xd2 = *(undefined8 *)(pfVar13 + 0x1b);
            dVar22 = *(double *)(pfVar13 + 0x15);
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                      SUB84(dVar22,0));
          }
          else {
            dVar22 = dVar22 + dVar22;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                      *(undefined4 *)(param_2 + 0x20));
          }
          else {
            in_xd2 = *(undefined8 *)(param_2 + 0x20);
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) - SUB84(dVar22,0),
                                      SUB84(dVar22,0));
          }
          else {
            dVar22 = dVar22 - dVar22;
          }
          if (in_FPSCR_SZ == '\0') {
            piVar16[0x83] = (int)((ulonglong)dVar22 >> 0x20);
            dVar22 = (double)CONCAT44(pfVar13[0x16],pfVar13[0x1c]);
          }
          else {
            *(double *)(piVar16 + 0x83) = dVar22;
            in_xd2 = *(undefined8 *)(pfVar13 + 0x1c);
            dVar22 = *(double *)(pfVar13 + 0x16);
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                      SUB84(dVar22,0));
          }
          else {
            dVar22 = dVar22 + dVar22;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                      *(undefined4 *)(param_2 + 0x24));
          }
          else {
            in_xd2 = *(undefined8 *)(param_2 + 0x24);
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) - SUB84(dVar22,0),
                                      SUB84(dVar22,0));
          }
          else {
            dVar22 = dVar22 - dVar22;
          }
          if (in_FPSCR_SZ == '\0') {
            piVar16[0x84] = (int)((ulonglong)dVar22 >> 0x20);
            dVar22 = (double)CONCAT44(pfVar13[0x17],pfVar13[0x1d]);
          }
          else {
            *(double *)(piVar16 + 0x84) = dVar22;
            in_xd2 = *(undefined8 *)(pfVar13 + 0x1d);
            dVar22 = *(double *)(pfVar13 + 0x17);
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                      SUB84(dVar22,0));
          }
          else {
            dVar22 = dVar22 + dVar22;
          }
        }
        else {
          if (in_FPSCR_SZ == '\0') {
            dVar22 = (double)CONCAT44(pfVar13[0x18],pfVar13[0x1b]);
          }
          else {
            in_xd2 = *(undefined8 *)(pfVar13 + 0x1b);
            dVar22 = *(double *)(pfVar13 + 0x18);
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                      SUB84(dVar22,0));
          }
          else {
            dVar22 = dVar22 + dVar22;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),piVar16[0x89]);
          }
          else {
            in_xd2 = *(undefined8 *)(piVar16 + 0x89);
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) - SUB84(dVar22,0),
                                      SUB84(dVar22,0));
          }
          else {
            dVar22 = dVar22 - dVar22;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                      *(undefined4 *)(param_2 + 0x20));
          }
          else {
            in_xd2 = *(undefined8 *)(param_2 + 0x20);
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) - SUB84(dVar22,0),
                                      SUB84(dVar22,0));
          }
          else {
            dVar22 = dVar22 - dVar22;
          }
          if (in_FPSCR_SZ == '\0') {
            piVar16[0x83] = (int)((ulonglong)dVar22 >> 0x20);
            dVar22 = (double)CONCAT44(pfVar13[0x19],pfVar13[0x1c]);
          }
          else {
            *(double *)(piVar16 + 0x83) = dVar22;
            in_xd2 = *(undefined8 *)(pfVar13 + 0x1c);
            dVar22 = *(double *)(pfVar13 + 0x19);
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                      SUB84(dVar22,0));
          }
          else {
            dVar22 = dVar22 + dVar22;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),piVar16[0x8a]);
          }
          else {
            in_xd2 = *(undefined8 *)(piVar16 + 0x8a);
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) - SUB84(dVar22,0),
                                      SUB84(dVar22,0));
          }
          else {
            dVar22 = dVar22 - dVar22;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                      *(undefined4 *)(param_2 + 0x24));
          }
          else {
            in_xd2 = *(undefined8 *)(param_2 + 0x24);
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) - SUB84(dVar22,0),
                                      SUB84(dVar22,0));
          }
          else {
            dVar22 = dVar22 - dVar22;
          }
          if (in_FPSCR_SZ == '\0') {
            piVar16[0x84] = (int)((ulonglong)dVar22 >> 0x20);
            dVar22 = (double)CONCAT44(pfVar13[0x1a],pfVar13[0x1d]);
          }
          else {
            *(double *)(piVar16 + 0x84) = dVar22;
            in_xd2 = *(undefined8 *)(pfVar13 + 0x1d);
            dVar22 = *(double *)(pfVar13 + 0x1a);
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                      SUB84(dVar22,0));
          }
          else {
            dVar22 = dVar22 + dVar22;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),piVar16[0x8b]);
          }
          else {
            in_xd2 = *(undefined8 *)(piVar16 + 0x8b);
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) - SUB84(dVar22,0),
                                      SUB84(dVar22,0));
          }
          else {
            dVar22 = dVar22 - dVar22;
          }
        }
        if (in_FPSCR_SZ == '\0') {
          dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)(param_2 + 0x28)
                                   );
        }
        else {
          in_xd2 = *(undefined8 *)(param_2 + 0x28);
        }
        if (in_FPSCR_PR == '\0') {
          dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) - SUB84(dVar22,0),
                                    SUB84(dVar22,0));
        }
        else {
          dVar22 = dVar22 - dVar22;
        }
        if (in_FPSCR_SZ == '\0') {
          piVar16[0x85] = (int)((ulonglong)dVar22 >> 0x20);
        }
        else {
          *(double *)(piVar16 + 0x85) = dVar22;
        }
        uVar19 = (*(code *)PTR_FUN_8c0737cc)(piVar16 + 0x83);
        uVar8 = SUB84(param_1,0);
        dVar22 = (double)((ulonglong)dVar22 & 0xffffffff00000000);
        fVar21 = (float)((ulonglong)uVar19 >> 0x20);
        if (in_FPSCR_SZ == '\0') {
          piVar16[9] = (int)fVar21;
        }
        else {
          *(undefined8 *)(piVar16 + 9) = uVar19;
        }
        if (fVar21 != 0.0) {
          (*(code *)PTR_FUN_8c0737d0)(piVar16 + 0x83);
          dVar20 = (double)(*(code *)PTR_FUN_8c0737d4)(piVar16 + 0x83,pfVar13 + 0x12);
          uVar5 = (undefined4)((ulonglong)dVar20 >> 0x20);
          if (in_FPSCR_SZ == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),DAT_8c0737d8);
            param_1 = (double)CONCAT44(uVar5,uVar8);
          }
          else {
            in_xd2 = CONCAT44(uRam8c0737dc,DAT_8c0737d8);
            param_1 = dVar20;
          }
          if (in_FPSCR_PR == '\0') {
            bVar26 = (float)((ulonglong)param_1 >> 0x20) < SUB84(dVar22,0);
          }
          else {
            bVar26 = param_1 < dVar22;
          }
          if (!bVar26) {
            if (in_FPSCR_SZ == '\0') {
              dVar22 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x110),SUB84(dVar22,0));
              dVar20 = (double)CONCAT44(uVar5,piVar16[9]);
            }
            else {
              dVar22 = *(double *)(param_4 + 0x110);
            }
            if (in_FPSCR_PR == '\0') {
              bVar26 = (float)((ulonglong)dVar22 >> 0x20) < SUB84(dVar20,0);
            }
            else {
              bVar26 = dVar22 < dVar20;
            }
            if (!bVar26) goto LAB_8c07377c;
          }
          piVar16[1] = 0;
        }
      }
LAB_8c07377c:
      piVar16[1] = piVar16[1] | 4;
      if (((*(ushort *)(param_2 + 4) & 3) != 0) && ((*(ushort *)(param_2 + 4) & 0x2000) == 0)) {
        if (in_FPSCR_SZ == '\0') {
          dVar22 = (double)CONCAT44(pfVar13[0x15],pfVar13[0x1b]);
        }
        else {
          in_xd2 = *(undefined8 *)(pfVar13 + 0x1b);
          dVar22 = *(double *)(pfVar13 + 0x15);
        }
        if (in_FPSCR_PR == '\0') {
          dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                    SUB84(dVar22,0));
        }
        else {
          dVar22 = dVar22 + dVar22;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),pfVar13[1]);
        }
        else {
          in_xd2 = *(undefined8 *)(pfVar13 + 1);
        }
        if (in_FPSCR_PR == '\0') {
          dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) - SUB84(dVar22,0),
                                    SUB84(dVar22,0));
        }
        else {
          dVar22 = dVar22 - dVar22;
        }
        if (in_FPSCR_SZ == '\0') {
          piVar16[0x1d] = (int)((ulonglong)dVar22 >> 0x20);
        }
        else {
          *(double *)(piVar16 + 0x1d) = dVar22;
        }
        if (*(int *)PTR_DAT_8c0737c8 == 0) {
          if (in_FPSCR_SZ == '\0') {
            dVar22 = (double)CONCAT44(pfVar13[0x16],pfVar13[0x1c]);
          }
          else {
            in_xd2 = *(undefined8 *)(pfVar13 + 0x1c);
            dVar22 = *(double *)(pfVar13 + 0x16);
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                      SUB84(dVar22,0));
          }
          else {
            dVar22 = dVar22 + dVar22;
          }
          if (in_FPSCR_SZ == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),pfVar13[2]);
          }
          else {
            in_xd2 = *(undefined8 *)(pfVar13 + 2);
          }
          if (in_FPSCR_PR == '\0') {
            dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) - SUB84(dVar22,0),
                                      SUB84(dVar22,0));
          }
          else {
            dVar22 = dVar22 - dVar22;
          }
          if (in_FPSCR_SZ == '\0') {
            piVar16[0x1e] = (int)((ulonglong)dVar22 >> 0x20);
          }
          else {
            *(double *)(piVar16 + 0x1e) = dVar22;
          }
        }
        else if (in_FPSCR_SZ == '\0') {
          piVar16[0x1e] = SUB84(dVar25,0);
        }
        else {
          *(undefined8 *)(piVar16 + 0x1e) = in_xd14;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar22 = (double)CONCAT44(pfVar13[0x17],pfVar13[0x1d]);
        }
        else {
          in_xd2 = *(undefined8 *)(pfVar13 + 0x1d);
          dVar22 = *(double *)(pfVar13 + 0x17);
        }
        if (in_FPSCR_PR == '\0') {
          dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                    SUB84(dVar22,0));
        }
        else {
          dVar22 = dVar22 + dVar22;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),pfVar13[3]);
        }
        else {
          in_xd2 = *(undefined8 *)(pfVar13 + 3);
        }
        if (in_FPSCR_PR == '\0') {
          dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) - SUB84(dVar22,0),
                                    SUB84(dVar22,0));
        }
        else {
          dVar22 = dVar22 - dVar22;
        }
        if (in_FPSCR_SZ == '\0') {
          piVar16[0x1f] = (int)((ulonglong)dVar22 >> 0x20);
        }
        else {
          *(double *)(piVar16 + 0x1f) = dVar22;
        }
        fVar21 = (float)(*(code *)PTR_FUN_8c0739e0)(piVar16 + 0x1d);
        dVar22 = (double)((ulonglong)dVar22 & 0xffffffff00000000);
        if (fVar21 != 0.0) {
          (*(code *)PTR_FUN_8c0739e4)(piVar16 + 0x1d);
          if (in_FPSCR_SZ == '\0') {
            dVar22 = (double)((ulonglong)*(uint *)(param_4 + 0x38) << 0x20);
          }
          else {
            dVar22 = *(double *)(param_4 + 0x38);
          }
          fVar21 = SUB84(dVar22,0);
          if (in_FPSCR_PR == '\0' && (float)((ulonglong)dVar22 >> 0x20) < fVar21) {
            if (in_FPSCR_PR == '\0') {
              dVar22 = (double)CONCAT44(0xbf800000,fVar21);
            }
            else {
              dVar22 = -(double)CONCAT44(0x3f800000,fVar21);
            }
            if (in_FPSCR_SZ == '\0') {
              piVar16[0x1a] = (int)((ulonglong)dVar22 >> 0x20);
            }
            else {
              *(double *)(piVar16 + 0x1a) = dVar22;
            }
          }
          else if (in_FPSCR_SZ == '\0') {
            piVar16[0x1a] = SUB84(dVar23,0);
          }
          else {
            *(undefined8 *)(piVar16 + 0x1a) = in_xd12;
          }
          if (in_FPSCR_SZ == '\0') {
            piVar16[0x1b] = SUB84(dVar25,0);
            piVar16[0x1c] = SUB84(dVar25,0);
          }
          else {
            *(undefined8 *)(piVar16 + 0x1b) = in_xd14;
            *(undefined8 *)(piVar16 + 0x1c) = in_xd14;
          }
          (*(code *)PTR_FUN_8c0739e8)(param_2,piVar16 + 0x1a);
          if (*(int *)PTR_DAT_8c0739ec != 0) {
            fVar21 = SUB84(dVar22,0);
            if (in_FPSCR_SZ == '\0') {
              fVar21 = (float)piVar16[0x1b];
            }
            else {
              in_xd2 = *(undefined8 *)(piVar16 + 0x1b);
            }
            if (in_FPSCR_PR == '\0') {
              dVar22 = (double)CONCAT44(0x3f800000,ABS(fVar21));
            }
            else {
              dVar22 = ABS((double)CONCAT44(0x3f800000,fVar21));
            }
            if (SUB84(dVar22,0) == (float)((ulonglong)dVar22 >> 0x20)) goto LAB_8c073880;
          }
          (*(code *)PTR_FUN_8c0739e4)(piVar16 + 0x1a);
          dVar20 = (double)(*(code *)PTR_FUN_8c0739f0)(piVar16 + 0x1d,piVar16 + 0x1a);
          if (in_FPSCR_SZ == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),_DAT_8c0739f4);
          }
          else {
            in_xd2 = CONCAT44(PTR_FUN_8c0739f8,_DAT_8c0739f4);
          }
          if (in_FPSCR_PR == '\0') {
            bVar26 = (float)((ulonglong)dVar20 >> 0x20) < SUB84(dVar22,0);
          }
          else {
            bVar26 = dVar20 < dVar22;
          }
          if (bVar26) {
            piVar16[1] = piVar16[1] & 0xfffffffb;
          }
        }
      }
LAB_8c073880:
      iVar4 = (*(code *)PTR_FUN_8c0739f8)(pfVar13 + 7,pfVar13 + 0x12,0);
      piVar16[6] = iVar4;
      if (0x2c00 < iVar4) {
        FUN_8c0742d4(param_2,pfVar13,puVar12);
        iVar4 = (*(code *)PTR_FUN_8c0739f8)(pfVar13 + 7,pfVar13 + 0x12,0);
        piVar16[6] = iVar4;
        piVar16[1] = piVar16[1] | 2;
      }
      if (((*(ushort *)(param_2 + 4) & 2) == 0) && ((*puVar12 & DAT_8c0739fc) == 0)) {
        if ((0x2c00 < piVar16[6]) || ((piVar16[1] & 5U) != 5)) {
          uVar19 = (*(code *)PTR_FUN_8c0739f0)(DAT_8c073a00,pfVar13 + 7);
          if (in_FPSCR_SZ == '\0') {
            piVar15 = piVar16 + -1;
            piVar16[-1] = (int)((ulonglong)uVar19 >> 0x20);
          }
          else {
            piVar15 = piVar16 + -2;
            *(undefined8 *)(piVar16 + -2) = uVar19;
          }
          dVar20 = (double)(*(code *)PTR_FUN_8c0739f0)(DAT_8c073a00,pfVar13 + 0x12);
          if (in_FPSCR_SZ == '\0') {
            dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*piVar15);
            piVar16 = (int *)((int)piVar15 + 4);
          }
          else {
            in_xd2 = *(undefined8 *)piVar15;
            piVar16 = (int *)((int)piVar15 + 8);
          }
          if (in_FPSCR_PR == '\0') {
            bVar26 = (float)((ulonglong)dVar20 >> 0x20) < SUB84(dVar22,0);
          }
          else {
            bVar26 = dVar20 < dVar22;
          }
          if ((!bVar26) && (((*puVar12 & 0x4000) == 0 || ((*(uint *)((int)piVar16 + 4) & 5) != 5))))
          goto LAB_8c074160;
        }
        if ((*(int *)((int)piVar16 + 0x48) != 0) || ((*(ushort *)(param_4 + 0x10) & 3) != 0)) {
          dVar20 = (double)(*(code *)PTR_FUN_8c0739f0)(DAT_8c073a00,pfVar13 + 0x12);
          dVar22 = (double)((ulonglong)dVar22 & 0xffffffff00000000);
          if (in_FPSCR_PR == '\0') {
            bVar26 = 0.0 < (float)((ulonglong)dVar20 >> 0x20);
          }
          else {
            bVar26 = dVar22 < dVar20;
          }
          if ((((!bVar26) || ((*(ushort *)(param_4 + 0x10) & 3) != 0)) &&
              (((*(uint *)((int)piVar16 + 4) & 2) != 0 ||
               (iVar4 = FUN_8c0742d4(param_2,pfVar13,puVar12), iVar4 != 0)))) &&
             ((param_2[9] != '\a' || (*param_2 != '0')))) {
            dVar20 = (double)(*(code *)PTR_FUN_8c0739e0)(param_3 + 4);
            uVar8 = SUB84(param_1,0);
            if (in_FPSCR_SZ == '\0') {
              dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                        *(undefined4 *)(param_4 + 0xc4));
            }
            else {
              in_xd2 = *(undefined8 *)(param_4 + 0xc4);
            }
            if (in_FPSCR_PR == '\0') {
              bVar26 = (float)((ulonglong)dVar20 >> 0x20) < SUB84(dVar22,0);
            }
            else {
              bVar26 = dVar20 < dVar22;
            }
            if ((!bVar26) && ((*puVar12 & 0x4000) == 0)) {
              uVar8 = *(undefined4 *)(param_3 + 8);
              *(undefined4 *)((int)piVar16 + 0x20c) = *(undefined4 *)(param_3 + 4);
              *(undefined4 *)((int)piVar16 + 0x210) = uVar8;
              *(undefined4 *)((int)piVar16 + 0x214) = *(undefined4 *)(param_3 + 0xc);
              (*(code *)PTR_FUN_8c0739e4)((undefined1 *)((int)piVar16 + 0x20c));
              dVar20 = (double)(*(code *)PTR_FUN_8c0739f0)
                                         ((undefined1 *)((int)piVar16 + 0x20c),pfVar13 + 0x12);
              uVar8 = SUB84(param_1,0);
              if (in_FPSCR_SZ == '\0') {
                dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),(undefined4)DAT_8c073a04)
                ;
              }
              else {
                in_xd2 = CONCAT44(DAT_8c073a04._4_4_,(undefined4)DAT_8c073a04);
              }
              if (in_FPSCR_PR == '\0') {
                bVar26 = SUB84(dVar22,0) < (float)((ulonglong)dVar20 >> 0x20);
              }
              else {
                bVar26 = dVar22 < dVar20;
              }
              if (!bVar26) goto LAB_8c073a20;
              *(uint *)((int)piVar16 + -4) = *puVar12;
              *(undefined4 *)((int)piVar16 + -8) = *(undefined4 *)((int)piVar16 + 0x14);
              *(undefined1 **)((int)piVar16 + -0xc) = (undefined1 *)((int)piVar16 + 0x224);
              *(undefined1 **)((int)piVar16 + -0x10) = (undefined1 *)((int)piVar16 + 0x200);
LAB_8c0740fe:
              uVar8 = FUN_8c0747d2(param_2,param_3,param_4,pfVar13);
              *(undefined4 *)((int)piVar16 + 0x14) = uVar8;
              goto LAB_8c07418a;
            }
LAB_8c073a20:
            dVar22 = (double)((ulonglong)dVar22 & 0xffffffff00000000);
            if (in_FPSCR_SZ == '\0') {
              param_1 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x3c),uVar8);
            }
            else {
              param_1 = *(double *)(param_4 + 0x3c);
            }
            fVar21 = (float)((ulonglong)param_1 >> 0x20);
            if (in_FPSCR_PR == '\0') {
              bVar26 = 0.0 < fVar21;
            }
            else {
              bVar26 = dVar22 < param_1;
            }
            if (bVar26) {
              if (in_FPSCR_PR == '\0') {
                bVar26 = 0.0 < fVar21;
              }
              else {
                bVar26 = dVar22 < param_1;
              }
              if ((!bVar26) || ((*(ushort *)(param_4 + 0x10) & 3) != 0)) {
                *(uint *)((int)piVar16 + -4) = *puVar12;
                *(undefined4 *)((int)piVar16 + -8) = *(undefined4 *)((int)piVar16 + 0x14);
                *(undefined1 **)((int)piVar16 + -0xc) = (undefined1 *)((int)piVar16 + 0x224);
                *(undefined1 **)((int)piVar16 + -0x10) = (undefined1 *)((int)piVar16 + 0x200);
                goto LAB_8c0740fe;
              }
            }
            *(undefined4 *)((int)piVar16 + 0x4c) = 1;
            *(uint *)((int)piVar16 + 0x38) = puVar12[1];
            *(uint *)((int)piVar16 + -4) = *puVar12;
            iVar4 = (*(code *)PTR_FUN_8c073b40)(param_2,param_3,param_4,pfVar13);
            fVar21 = SUB84(param_1,0);
            if (iVar4 != 0) {
              if (*(int *)((int)piVar16 + 0x14) == 0) {
                *(int *)((int)piVar16 + 0x14) = iVar4;
              }
              else if (iVar4 == 2) {
                *(undefined4 *)((int)piVar16 + 0x14) = 2;
              }
            }
            if ((*puVar12 & 0x4000) == 0) {
              if (((*puVar12 & DAT_8c073b44) == 0) && ((*(ushort *)(param_4 + 0x10) & 0x2000) == 0))
              {
                if (in_FPSCR_SZ != '\0') {
                  fVar21 = (float)*(undefined8 *)(pfVar13 + 0x14);
                }
                dVar22 = (double)(*(code *)PTR_FUN_8c073b4c)();
                if (in_FPSCR_PR == '\0') {
                  fVar7 = (float)(int)((float)((ulonglong)dVar22 >> 0x20) *
                                      (float)((ulonglong)dVar25 >> 0x20));
                }
                else {
                  fVar7 = (float)(int)(dVar22 * dVar25);
                }
                pfVar13[0xd] = fVar7;
                if (in_FPSCR_SZ == '\0') {
                  lVar24 = (ulonglong)(uint)pfVar13[0x13] << 0x20;
                }
                else {
                  lVar24 = *(longlong *)(pfVar13 + 0x13);
                }
                if (in_FPSCR_PR == '\0' && (float)((ulonglong)lVar24 >> 0x20) < (float)lVar24) {
                  if (in_FPSCR_SZ != '\0') {
                    fVar21 = (float)*(undefined8 *)(pfVar13 + 0x12);
                  }
                  dVar22 = (double)(*(code *)PTR_FUN_8c073d00)();
                  if (in_FPSCR_PR == '\0') {
                    iVar4 = (int)((float)((ulonglong)dVar22 >> 0x20) *
                                 (float)((ulonglong)dVar25 >> 0x20));
                  }
                  else {
                    iVar4 = (int)(dVar22 * dVar25);
                  }
                  fVar7 = (float)(iVar4 + DAT_8c073d04);
                }
                else {
                  if (in_FPSCR_SZ != '\0') {
                    fVar21 = (float)*(undefined8 *)(pfVar13 + 0x12);
                  }
                  dVar22 = (double)(*(code *)PTR_FUN_8c073b4c)();
                  if (in_FPSCR_PR == '\0') {
                    iVar4 = (int)((float)((ulonglong)dVar22 >> 0x20) *
                                 (float)((ulonglong)dVar25 >> 0x20));
                  }
                  else {
                    iVar4 = (int)(dVar22 * dVar25);
                  }
                  fVar7 = (float)-iVar4;
                }
                pfVar13[0xe] = fVar7;
                if (in_FPSCR_SZ == '\0') {
                  lVar24 = (ulonglong)*(uint *)(param_3 + 4) << 0x20;
                }
                else {
                  lVar24 = *(longlong *)(param_3 + 4);
                }
                if ((float)((ulonglong)lVar24 >> 0x20) == (float)lVar24) {
                  if (in_FPSCR_SZ == '\0') {
                    dVar22 = (double)CONCAT44(*(undefined4 *)(param_3 + 0xc),(float)lVar24);
                  }
                  else {
                    dVar22 = *(double *)(param_3 + 0xc);
                  }
                  if (((float)((ulonglong)dVar22 >> 0x20) == SUB84(dVar22,0)) &&
                     ((*(ushort *)(param_4 + 0x10) & 1) != 0)) {
                    uVar19 = (*(code *)PTR_FUN_8c073d0c)(DAT_8c073d08,pfVar13 + 0x12);
                    if (in_FPSCR_SZ == '\0') {
                      puVar18 = (undefined8 *)((int)piVar16 + -4);
                      *(int *)((int)piVar16 + -4) = (int)((ulonglong)uVar19 >> 0x20);
                    }
                    else {
                      puVar18 = (undefined8 *)((int)piVar16 + -8);
                      *(undefined8 *)((int)piVar16 + -8) = uVar19;
                    }
                    dVar20 = (double)(*(code *)PTR_FUN_8c073d0c)(DAT_8c073d08,pfVar13 + 7);
                    if (in_FPSCR_SZ == '\0') {
                      dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                                *(undefined4 *)puVar18);
                      piVar16 = (int *)((int)puVar18 + 4);
                    }
                    else {
                      in_xd2 = *puVar18;
                      piVar16 = (int *)(puVar18 + 1);
                    }
                    if (in_FPSCR_PR == '\0') {
                      bVar26 = SUB84(dVar22,0) < (float)((ulonglong)dVar20 >> 0x20);
                    }
                    else {
                      bVar26 = dVar22 < dVar20;
                    }
                    if (!bVar26) goto LAB_8c073c1e;
                  }
                }
                if (in_FPSCR_SZ != '\0') {
                  fVar21 = (float)*(undefined8 *)(pfVar13 + 0x14);
                }
                dVar22 = (double)(*(code *)PTR_FUN_8c073d00)();
                if (in_FPSCR_PR == '\0') {
                  iVar4 = (int)((float)((ulonglong)dVar22 >> 0x20) *
                               (float)((ulonglong)dVar25 >> 0x20));
                }
                else {
                  iVar4 = (int)(dVar22 * dVar25);
                }
                *(int *)((int)piVar16 + 0x28) = iVar4;
                if (in_FPSCR_SZ != '\0') {
                  fVar21 = (float)*(undefined8 *)(pfVar13 + 0x12);
                }
                dVar22 = (double)(*(code *)PTR_FUN_8c073d00)();
                if (in_FPSCR_PR == '\0') {
                  iVar4 = (int)((float)((ulonglong)dVar22 >> 0x20) *
                               (float)((ulonglong)dVar25 >> 0x20));
                }
                else {
                  iVar4 = (int)(dVar22 * dVar25);
                }
                *(int *)((int)piVar16 + 0x2c) = -iVar4;
                if (in_FPSCR_SZ == '\0') {
                  dVar22 = (double)((ulonglong)(uint)pfVar13[0x13] << 0x20);
                }
                else {
                  dVar22 = *(double *)(pfVar13 + 0x13);
                }
                if (in_FPSCR_PR == '\0' && (float)((ulonglong)dVar22 >> 0x20) < SUB84(dVar22,0)) {
                  *(int *)((int)piVar16 + 0x18) = DAT_8c073d04 - *(int *)((int)piVar16 + 0x28);
                  *(int *)((int)piVar16 + 4) = DAT_8c073d04 - *(int *)((int)piVar16 + 0x2c);
                  uVar8 = (*(code *)PTR_FUN_8c073d10)
                                    (*(undefined4 *)(param_2 + 0x14),
                                     *(undefined4 *)((int)piVar16 + 0x18));
                  *(undefined4 *)((int)piVar16 + 0x44) = uVar8;
                  iVar4 = (*(code *)PTR_FUN_8c073d10)
                                    (*(undefined4 *)(param_2 + 0x1c),
                                     *(undefined4 *)((int)piVar16 + 4));
                  if (*(int *)((int)piVar16 + 0x44) < iVar4) {
                    pfVar13[0xd] = *(float *)((int)piVar16 + 0x18);
                    fVar7 = *(float *)((int)piVar16 + 0x2c);
                  }
                  else {
                    pfVar13[0xd] = *(float *)((int)piVar16 + 0x28);
                    fVar7 = *(float *)((int)piVar16 + 4);
                  }
                  pfVar13[0xe] = fVar7;
                }
                (*(code *)PTR_FUN_8c073d14)(param_2,param_4,pfVar13);
              }
              else {
                if (in_FPSCR_SZ == '\0') {
                  fVar21 = pfVar13[0x13];
                }
                else {
                  fVar21 = (float)*(undefined8 *)(pfVar13 + 0x12);
                }
                dVar22 = (double)(*(code *)PTR_FUN_8c073b48)();
                puVar2 = PTR_FUN_8c073b4c;
                if (in_FPSCR_PR == '\0') {
                  iVar4 = (int)((float)((ulonglong)dVar22 >> 0x20) *
                               (float)((ulonglong)dVar25 >> 0x20));
                }
                else {
                  iVar4 = (int)(dVar22 * dVar25);
                }
                pfVar13[0xe] = (float)-iVar4;
                if (in_FPSCR_SZ != '\0') {
                  fVar21 = (float)*(undefined8 *)(pfVar13 + 0x14);
                }
                dVar22 = (double)(*(code *)puVar2)();
                if (in_FPSCR_PR == '\0') {
                  fVar7 = (float)(int)((float)((ulonglong)dVar22 >> 0x20) *
                                      (float)((ulonglong)dVar25 >> 0x20));
                }
                else {
                  fVar7 = (float)(int)(dVar22 * dVar25);
                }
                pfVar13[0xd] = fVar7;
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
                  if (((float)((ulonglong)dVar22 >> 0x20) == SUB84(dVar22,0)) &&
                     ((*(ushort *)(param_4 + 0x10) & 1) != 0)) {
                    uVar19 = (*(code *)PTR_FUN_8c073b54)(DAT_8c073b50,pfVar13 + 0x12);
                    if (in_FPSCR_SZ == '\0') {
                      puVar17 = (undefined8 *)((int)piVar16 + -4);
                      *(int *)((int)piVar16 + -4) = (int)((ulonglong)uVar19 >> 0x20);
                    }
                    else {
                      puVar17 = (undefined8 *)((int)piVar16 + -8);
                      *(undefined8 *)((int)piVar16 + -8) = uVar19;
                    }
                    dVar20 = (double)(*(code *)PTR_FUN_8c073b54)(DAT_8c073b50,pfVar13 + 7);
                    if (in_FPSCR_SZ == '\0') {
                      dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                                *(undefined4 *)puVar17);
                      piVar16 = (int *)((int)puVar17 + 4);
                    }
                    else {
                      in_xd2 = *puVar17;
                      piVar16 = (int *)(puVar17 + 1);
                    }
                    if (in_FPSCR_PR == '\0') {
                      bVar26 = SUB84(dVar22,0) < (float)((ulonglong)dVar20 >> 0x20);
                    }
                    else {
                      bVar26 = dVar22 < dVar20;
                    }
                    if (!bVar26) goto LAB_8c073c1e;
                  }
                }
                (*(code *)PTR_FUN_8c073b58)(param_2,param_4,pfVar13);
              }
            }
            else {
              param_2[0x1c] = '\0';
              param_2[0x1d] = '\0';
              param_2[0x1e] = '\0';
              param_2[0x1f] = '\0';
              param_2[0x14] = '\0';
              param_2[0x15] = '\0';
              param_2[0x16] = '\0';
              param_2[0x17] = '\0';
            }
LAB_8c073c1e:
            if (*(int *)((int)piVar16 + 0x14) == 0) {
              if (in_FPSCR_SZ == '\0') {
                pfVar13[9] = SUB84(dVar25,0);
                pfVar13[7] = SUB84(dVar25,0);
                pfVar13[8] = SUB84(dVar23,0);
              }
              else {
                *(undefined8 *)(pfVar13 + 9) = in_xd14;
                *(undefined8 *)(pfVar13 + 7) = in_xd14;
                *(undefined8 *)(pfVar13 + 8) = in_xd12;
              }
              pfVar13[0xd] = *(float *)(param_2 + 0x14);
              pfVar13[0xe] = *(float *)(param_2 + 0x1c);
              if ((*puVar12 & 0x4000) == 0) {
                (*(code *)PTR_FUN_8c073d1c)(param_2,pfVar13 + 7);
              }
              else {
                (*(code *)PTR_FUN_8c073d18)
                          (pfVar13 + 0x12,pfVar13 + 7,*(undefined4 *)(param_2 + 0x18));
              }
              if (in_FPSCR_SZ == '\0') {
                *(int *)((int)piVar16 + 0x22c) = SUB84(dVar25,0);
                *(int *)((int)piVar16 + 0x224) = SUB84(dVar25,0);
                dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                          *(undefined4 *)(param_4 + 0x110));
              }
              else {
                *(undefined8 *)((int)piVar16 + 0x22c) = in_xd14;
                *(undefined8 *)((int)piVar16 + 0x224) = in_xd14;
                in_xd2 = *(undefined8 *)(param_4 + 0x110);
              }
              if (in_FPSCR_PR == '\0') {
                fVar7 = SUB84(dVar22,0) * (float)((ulonglong)dVar23 >> 0x20);
              }
              else {
                fVar7 = SUB84(dVar22 * dVar23,0);
              }
              if (in_FPSCR_SZ == '\0') {
                *(float *)((int)piVar16 + 0x228) = fVar7;
              }
              else {
                *(undefined8 *)((int)piVar16 + 0x228) = in_xd2;
              }
              *(undefined4 *)((int)piVar16 + 0x218) = *(undefined4 *)((int)piVar16 + 0x224);
              *(undefined4 *)((int)piVar16 + 0x21c) = *(undefined4 *)((int)piVar16 + 0x228);
              *(undefined4 *)((int)piVar16 + 0x220) = *(undefined4 *)((int)piVar16 + 0x22c);
              if ((*puVar12 & 0x4000) == 0) {
                (*(code *)PTR_FUN_8c073d1c)(param_2,(undefined1 *)((int)piVar16 + 0x224));
              }
              else {
                (*(code *)PTR_FUN_8c073d18)
                          (pfVar13 + 0x12,(undefined1 *)((int)piVar16 + 0x224),
                           *(undefined4 *)(param_2 + 0x18));
              }
              (*(code *)PTR_FUN_8c073d20)
                        (param_3,param_4,pfVar13,(undefined1 *)((int)piVar16 + 0x218));
              if ((*puVar12 & 0x4000) == 0) {
                (*(code *)PTR_FUN_8c073f80)(param_2,(undefined1 *)((int)piVar16 + 0x218));
              }
              else {
                (*(code *)PTR_FUN_8c073d18)
                          (pfVar13 + 0x12,(undefined1 *)((int)piVar16 + 0x218),
                           *(undefined4 *)(param_2 + 0x18));
              }
            }
            *(float *)(param_3 + 0x1c) = pfVar13[0xd];
            *(float *)(param_3 + 0x24) = pfVar13[0xe];
            fVar7 = pfVar13[0x13];
            *(float *)(param_4 + 0x50) = pfVar13[0x12];
            *(float *)(param_4 + 0x54) = fVar7;
            *(float *)(param_4 + 0x58) = pfVar13[0x14];
            if ((*puVar12 & 0x4000) != 0) {
              fVar7 = pfVar13[0x1c];
              *(float *)((int)piVar16 + 0x20c) = pfVar13[0x1b];
              *(float *)((int)piVar16 + 0x210) = fVar7;
              *(float *)((int)piVar16 + 0x214) = pfVar13[0x1d];
              fVar7 = (float)(*(code *)PTR_FUN_8c073f84)((undefined1 *)((int)piVar16 + 0x20c));
              if (fVar7 != 0.0) {
                uVar19 = (*(code *)PTR_FUN_8c073f84)(pfVar13 + 4);
                if (in_FPSCR_SZ == '\0') {
                  *(int *)((int)piVar16 + 0x1c) = (int)((ulonglong)uVar19 >> 0x20);
                }
                else {
                  *(undefined8 *)((int)piVar16 + 0x1c) = uVar19;
                }
                (*(code *)PTR_FUN_8c073f88)((undefined1 *)((int)piVar16 + 0x20c));
                if (in_FPSCR_SZ == '\0') {
                  dVar22 = (double)CONCAT44(*(undefined4 *)((int)piVar16 + 0x20c),
                                            *(undefined4 *)((int)piVar16 + 0x1c));
                }
                else {
                  in_xd2 = *(undefined8 *)((int)piVar16 + 0x1c);
                  dVar22 = *(double *)((int)piVar16 + 0x20c);
                }
                if (in_FPSCR_PR == '\0') {
                  dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) * SUB84(dVar22,0),
                                            SUB84(dVar22,0));
                }
                else {
                  dVar22 = dVar22 * dVar22;
                }
                if (in_FPSCR_SZ == '\0') {
                  pfVar13[0x1b] = (float)((ulonglong)dVar22 >> 0x20);
                  dVar22 = (double)CONCAT44(*(undefined4 *)((int)piVar16 + 0x210),
                                            *(undefined4 *)((int)piVar16 + 0x1c));
                }
                else {
                  *(double *)(pfVar13 + 0x1b) = dVar22;
                  in_xd2 = *(undefined8 *)((int)piVar16 + 0x1c);
                  dVar22 = *(double *)((int)piVar16 + 0x210);
                }
                if (in_FPSCR_PR == '\0') {
                  dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) * SUB84(dVar22,0),
                                            SUB84(dVar22,0));
                }
                else {
                  dVar22 = dVar22 * dVar22;
                }
                if (in_FPSCR_SZ == '\0') {
                  pfVar13[0x1c] = (float)((ulonglong)dVar22 >> 0x20);
                  dVar22 = (double)CONCAT44(*(undefined4 *)((int)piVar16 + 0x214),
                                            *(undefined4 *)((int)piVar16 + 0x1c));
                }
                else {
                  *(double *)(pfVar13 + 0x1c) = dVar22;
                  in_xd2 = *(undefined8 *)((int)piVar16 + 0x1c);
                  dVar22 = *(double *)((int)piVar16 + 0x214);
                }
                if (in_FPSCR_PR == '\0') {
                  dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) * SUB84(dVar22,0),
                                            SUB84(dVar22,0));
                }
                else {
                  dVar22 = dVar22 * dVar22;
                }
                if (in_FPSCR_SZ == '\0') {
                  pfVar13[0x1d] = (float)((ulonglong)dVar22 >> 0x20);
                }
                else {
                  *(double *)(pfVar13 + 0x1d) = dVar22;
                }
              }
            }
            fVar7 = SUB84(dVar25,0);
            if (in_FPSCR_SZ == '\0') {
              pfVar13[6] = fVar7;
              pfVar13[5] = fVar7;
              pfVar13[4] = fVar7;
              dVar22 = (double)CONCAT44(pfVar13[0x18],pfVar13[0x1b]);
              iVar4 = DAT_8c073f94;
            }
            else {
              *(undefined8 *)(pfVar13 + 6) = in_xd14;
              *(undefined8 *)(pfVar13 + 5) = in_xd14;
              *(undefined8 *)(pfVar13 + 4) = in_xd14;
              in_xd2 = *(undefined8 *)(pfVar13 + 0x1b);
              dVar22 = *(double *)(pfVar13 + 0x18);
              iVar4 = DAT_8c073f94;
            }
            if (in_FPSCR_PR == '\0') {
              dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                        SUB84(dVar22,0));
            }
            else {
              dVar22 = dVar22 + dVar22;
            }
            DAT_8c073f94 = iVar4;
            if (in_FPSCR_SZ == '\0') {
              pfVar13[1] = (float)((ulonglong)dVar22 >> 0x20);
              dVar22 = (double)CONCAT44(pfVar13[0x19],pfVar13[0x1c]);
            }
            else {
              *(double *)(pfVar13 + 1) = dVar22;
              in_xd2 = *(undefined8 *)(pfVar13 + 0x1c);
              dVar22 = *(double *)(pfVar13 + 0x19);
            }
            if (in_FPSCR_PR == '\0') {
              dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                        SUB84(dVar22,0));
            }
            else {
              dVar22 = dVar22 + dVar22;
            }
            if (in_FPSCR_SZ == '\0') {
              pfVar13[2] = (float)((ulonglong)dVar22 >> 0x20);
              dVar22 = (double)CONCAT44(pfVar13[0x1a],pfVar13[0x1d]);
            }
            else {
              *(double *)(pfVar13 + 2) = dVar22;
              in_xd2 = *(undefined8 *)(pfVar13 + 0x1d);
              dVar22 = *(double *)(pfVar13 + 0x1a);
            }
            if (in_FPSCR_PR == '\0') {
              dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                        SUB84(dVar22,0));
            }
            else {
              dVar22 = dVar22 + dVar22;
            }
            if (in_FPSCR_SZ == '\0') {
              pfVar13[3] = (float)((ulonglong)dVar22 >> 0x20);
            }
            else {
              *(double *)(pfVar13 + 3) = dVar22;
            }
            uVar1 = *(ushort *)((int)pfVar13 + 0x2a);
            if (in_FPSCR_SZ == '\0') {
              dVar22 = (double)(ulonglong)(uint)pfVar13[7];
            }
            else {
              in_xd2 = *(undefined8 *)(pfVar13 + 7);
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)piVar16 + 0x74) = SUB84(dVar22,0);
            }
            else {
              *(undefined8 *)((int)piVar16 + 0x74) = in_xd2;
            }
            iVar4 = (uint)uVar1 * 0x58 + iVar4;
            if (in_FPSCR_SZ == '\0') {
              *(float *)((int)piVar16 + 0x78) = pfVar13[8];
              *(float *)((int)piVar16 + 0x7c) = pfVar13[9];
              *(undefined4 *)((int)piVar16 + 0x68) = *(undefined4 *)(iVar4 + 0x20);
              *(undefined4 *)((int)piVar16 + 0x6c) = *(undefined4 *)(iVar4 + 0x24);
              *(undefined4 *)((int)piVar16 + 0x70) = *(undefined4 *)(iVar4 + 0x28);
            }
            else {
              *(undefined8 *)((int)piVar16 + 0x78) = *(undefined8 *)(pfVar13 + 8);
              *(undefined8 *)((int)piVar16 + 0x7c) = *(undefined8 *)(pfVar13 + 9);
              *(undefined8 *)((int)piVar16 + 0x68) = *(undefined8 *)(iVar4 + 0x20);
              *(undefined8 *)((int)piVar16 + 0x6c) = *(undefined8 *)(iVar4 + 0x24);
              in_xd2 = *(undefined8 *)(iVar4 + 0x28);
              *(undefined8 *)((int)piVar16 + 0x70) = in_xd2;
            }
            (*(code *)PTR_FUN_8c073f98)
                      (pfVar13 + 1,(undefined1 *)((int)piVar16 + 0x68),
                       (undefined1 *)((int)piVar16 + 0x5c));
            if (in_FPSCR_SZ == '\0') {
              dVar22 = (double)CONCAT44(*(undefined4 *)((int)piVar16 + 0x224),
                                        *(undefined4 *)((int)piVar16 + 0x5c));
            }
            else {
              in_xd2 = *(undefined8 *)((int)piVar16 + 0x5c);
              dVar22 = *(double *)((int)piVar16 + 0x224);
            }
            if (in_FPSCR_PR == '\0') {
              dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                        SUB84(dVar22,0));
            }
            else {
              dVar22 = dVar22 + dVar22;
            }
            if (in_FPSCR_SZ == '\0') {
              pfVar13[1] = (float)((ulonglong)dVar22 >> 0x20);
              dVar22 = (double)CONCAT44(*(undefined4 *)((int)piVar16 + 0x228),
                                        *(undefined4 *)((int)piVar16 + 0x60));
            }
            else {
              *(double *)(pfVar13 + 1) = dVar22;
              in_xd2 = *(undefined8 *)((int)piVar16 + 0x60);
              dVar22 = *(double *)((int)piVar16 + 0x228);
            }
            if (in_FPSCR_PR == '\0') {
              dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                        SUB84(dVar22,0));
            }
            else {
              dVar22 = dVar22 + dVar22;
            }
            if (in_FPSCR_SZ == '\0') {
              pfVar13[2] = (float)((ulonglong)dVar22 >> 0x20);
              dVar22 = (double)CONCAT44(*(undefined4 *)((int)piVar16 + 0x22c),
                                        *(undefined4 *)((int)piVar16 + 100));
            }
            else {
              *(double *)(pfVar13 + 2) = dVar22;
              in_xd2 = *(undefined8 *)((int)piVar16 + 100);
              dVar22 = *(double *)((int)piVar16 + 0x22c);
            }
            if (in_FPSCR_PR == '\0') {
              dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + SUB84(dVar22,0),
                                        SUB84(dVar22,0));
            }
            else {
              dVar22 = dVar22 + dVar22;
            }
            if (in_FPSCR_SZ == '\0') {
              pfVar13[3] = (float)((ulonglong)dVar22 >> 0x20);
            }
            else {
              *(double *)(pfVar13 + 3) = dVar22;
            }
            if (*(int *)((int)piVar16 + 0x10) == 0) {
              *(undefined4 *)(param_4 + 0x6c) = 0;
            }
            else {
              *(undefined4 *)(param_4 + 0x6c) = *(undefined4 *)((int)piVar16 + 0x10);
              (*(code *)PTR_FUN_8c073f9c)
                        (param_2,param_4,pfVar13,(undefined1 *)((int)piVar16 + 0x224));
            }
            if (in_FPSCR_SZ == '\0') {
              dVar22 = (double)CONCAT44(pfVar13[1],*(undefined4 *)((int)piVar16 + 0x224));
            }
            else {
              in_xd2 = *(undefined8 *)((int)piVar16 + 0x224);
              dVar22 = *(double *)(pfVar13 + 1);
            }
            if (in_FPSCR_PR == '\0') {
              dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) - SUB84(dVar22,0),
                                        SUB84(dVar22,0));
            }
            else {
              dVar22 = dVar22 - dVar22;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)piVar16 + 0x20c) = (int)((ulonglong)dVar22 >> 0x20);
              dVar22 = (double)CONCAT44(pfVar13[2],*(undefined4 *)((int)piVar16 + 0x228));
            }
            else {
              *(double *)((int)piVar16 + 0x20c) = dVar22;
              in_xd2 = *(undefined8 *)((int)piVar16 + 0x228);
              dVar22 = *(double *)(pfVar13 + 2);
            }
            if (in_FPSCR_PR == '\0') {
              dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) - SUB84(dVar22,0),
                                        SUB84(dVar22,0));
            }
            else {
              dVar22 = dVar22 - dVar22;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)piVar16 + 0x210) = (int)((ulonglong)dVar22 >> 0x20);
              dVar22 = (double)CONCAT44(pfVar13[3],*(undefined4 *)((int)piVar16 + 0x22c));
            }
            else {
              *(double *)((int)piVar16 + 0x210) = dVar22;
              in_xd2 = *(undefined8 *)((int)piVar16 + 0x22c);
              dVar22 = *(double *)(pfVar13 + 3);
            }
            if (in_FPSCR_PR == '\0') {
              dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) - SUB84(dVar22,0),
                                        SUB84(dVar22,0));
            }
            else {
              dVar22 = dVar22 - dVar22;
            }
            if (in_FPSCR_SZ == '\0') {
              *(int *)((int)piVar16 + 0x214) = (int)((ulonglong)dVar22 >> 0x20);
            }
            else {
              *(double *)((int)piVar16 + 0x214) = dVar22;
            }
            dVar20 = (double)(*(code *)PTR_FUN_8c074120)
                                       (param_2 + 0x20,(undefined1 *)((int)piVar16 + 0x20c));
            uVar8 = (undefined4)((ulonglong)dVar20 >> 0x20);
            param_1 = dVar20;
            if (in_FPSCR_SZ == '\0') {
              param_1 = (double)CONCAT44(uVar8,fVar21);
            }
            fVar21 = (float)((ulonglong)param_1 >> 0x20);
            if (*(int *)((int)piVar16 + 0x10) == 0) {
              if (in_FPSCR_SZ == '\0') {
                dVar22 = (double)((ulonglong)*(uint *)(param_4 + 0x38) << 0x20);
              }
              else {
                dVar22 = *(double *)(param_4 + 0x38);
              }
              if ((float)((ulonglong)dVar22 >> 0x20) != SUB84(dVar22,0)) goto LAB_8c07401e;
              if (in_FPSCR_SZ == '\0') {
                dVar22 = (double)CONCAT44(*(undefined4 *)(param_4 + 0x40),SUB84(dVar22,0));
              }
              else {
                dVar22 = *(double *)(param_4 + 0x40);
              }
              fVar7 = (float)((ulonglong)dVar22 >> 0x20);
              if (fVar7 != SUB84(dVar22,0)) goto LAB_8c07401e;
              dVar22 = (double)CONCAT44(fVar7,0x3f800000);
              if (in_FPSCR_PR == '\0') {
                bVar26 = 1.0 < fVar21;
              }
              else {
                bVar26 = dVar22 < param_1;
              }
              if ((bVar26) || (*(int *)PTR_DAT_8c074124 != 0)) goto LAB_8c07401e;
            }
            else {
LAB_8c07401e:
              if (in_FPSCR_SZ == '\0') {
                dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),DAT_8c074128);
              }
              else {
                in_xd2 = CONCAT44(_DAT_8c07412c,DAT_8c074128);
              }
              if (in_FPSCR_PR == '\0') {
                bVar26 = SUB84(dVar22,0) < fVar21;
              }
              else {
                bVar26 = dVar22 < param_1;
              }
              if ((bVar26) || (*(int *)PTR_DAT_8c074124 != 0)) {
                uVar5 = *(undefined4 *)((int)piVar16 + 0x210);
                *(undefined4 *)(param_2 + 0x20) = *(undefined4 *)((int)piVar16 + 0x20c);
                *(undefined4 *)(param_2 + 0x24) = uVar5;
                *(undefined4 *)(param_2 + 0x28) = *(undefined4 *)((int)piVar16 + 0x214);
              }
            }
            if (in_FPSCR_SZ == '\0') {
              uVar5 = *(undefined4 *)(param_4 + 0x38);
              *(undefined4 *)((int)piVar16 + 0x3c) = uVar5;
              dVar22 = (double)CONCAT44(_DAT_8c07412c,uVar5);
            }
            else {
              in_xd2 = *(undefined8 *)(param_4 + 0x38);
              *(undefined8 *)((int)piVar16 + 0x3c) = in_xd2;
              dVar22 = (double)CONCAT44(DAT_8c074130,_DAT_8c07412c);
            }
            fVar21 = (float)((ulonglong)dVar22 >> 0x20);
            if (in_FPSCR_PR == '\0' && SUB84(dVar22,0) < fVar21) {
              if (in_FPSCR_SZ == '\0') {
                dVar22 = (double)CONCAT44(fVar21,DAT_8c074130);
                dVar20 = (double)CONCAT44(uVar8,*(undefined4 *)((int)piVar16 + 0x3c));
              }
              else {
                in_xd2 = CONCAT44(DAT_8c074134,DAT_8c074130);
              }
              if (in_FPSCR_PR == '\0') {
                bVar26 = SUB84(dVar22,0) < SUB84(dVar20,0);
              }
              else {
                bVar26 = dVar22 < dVar20;
              }
              if (bVar26) {
                dVar20 = (double)(*(code *)PTR_FUN_8c074138)(DAT_8c074134,pfVar13 + 7);
                if (in_FPSCR_SZ == '\0') {
                  dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                            (undefined4)DAT_8c07413c);
                }
                else {
                  in_xd2 = CONCAT44(DAT_8c07413c._4_4_,(undefined4)DAT_8c07413c);
                }
                fVar21 = (float)((ulonglong)dVar20 >> 0x20);
                if (in_FPSCR_PR == '\0') {
                  bVar26 = fVar21 < SUB84(dVar22,0);
                }
                else {
                  bVar26 = dVar20 < dVar22;
                }
                if ((!bVar26) && ((*puVar12 & 0x4000) == 0)) {
                  if (in_FPSCR_SZ == '\0') {
                    uVar8 = *(undefined4 *)(param_4 + 0x38);
                    *(undefined4 *)((int)piVar16 + 0x40) = uVar8;
                    dVar22 = (double)CONCAT44(DAT_8c07413c._4_4_,uVar8);
                  }
                  else {
                    in_xd2 = *(undefined8 *)(param_4 + 0x38);
                    *(undefined8 *)((int)piVar16 + 0x40) = in_xd2;
                    dVar22 = (double)CONCAT44(DAT_8c074144,DAT_8c07413c._4_4_);
                  }
                  fVar7 = (float)((ulonglong)dVar22 >> 0x20);
                  if (in_FPSCR_PR != '\0' || fVar7 <= SUB84(dVar22,0)) {
LAB_8c074096:
                    dVar20 = (double)(*(code *)PTR_FUN_8c074138)(DAT_8c074134,pfVar13 + 7);
                    if (in_FPSCR_SZ == '\0') {
                      dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                                (undefined4)DAT_8c07413c);
                    }
                    else {
                      in_xd2 = CONCAT44(DAT_8c07413c._4_4_,(undefined4)DAT_8c07413c);
                    }
                    if (in_FPSCR_PR == '\0') {
                      bVar26 = (float)((ulonglong)dVar20 >> 0x20) < SUB84(dVar22,0);
                    }
                    else {
                      bVar26 = dVar20 < dVar22;
                    }
                    if (bVar26) goto LAB_8c0740a8;
                  }
                  else {
                    if (in_FPSCR_SZ == '\0') {
                      dVar22 = (double)CONCAT44(fVar7,DAT_8c074144);
                      dVar20 = (double)CONCAT44(fVar21,*(undefined4 *)((int)piVar16 + 0x40));
                    }
                    else {
                      in_xd2 = CONCAT44(PTR_FUN_8c074148,DAT_8c074144);
                    }
                    if (in_FPSCR_PR == '\0') {
                      bVar26 = SUB84(dVar22,0) < SUB84(dVar20,0);
                    }
                    else {
                      bVar26 = dVar22 < dVar20;
                    }
                    if (!bVar26) goto LAB_8c074096;
                  }
                  (*(code *)PTR_FUN_8c074148)(param_2,param_3,param_4,0);
                  if (in_FPSCR_SZ == '\0') {
                    dVar22 = (double)((ulonglong)*(uint *)(param_4 + 0x38) << 0x20);
                  }
                  else {
                    dVar22 = *(double *)(param_4 + 0x38);
                  }
                  if (in_FPSCR_PR == '\0' && SUB84(dVar22,0) < (float)((ulonglong)dVar22 >> 0x20)) {
                    (*(code *)PTR_FUN_8c07414c)(param_2,param_3,pfVar13);
                  }
                  goto LAB_8c07418a;
                }
              }
            }
LAB_8c0740a8:
            puVar2 = PTR_FUN_8c074148;
            *(ushort *)(param_2 + 4) = *(ushort *)(param_2 + 4) | 1;
            (*(code *)puVar2)(param_2,param_3,param_4,*puVar12);
            goto LAB_8c07418a;
          }
        }
      }
LAB_8c074160:
      *(uint *)((int)piVar16 + -4) = *puVar12;
      *(undefined4 *)((int)piVar16 + -8) = *(undefined4 *)((int)piVar16 + 0x14);
      *(undefined1 **)((int)piVar16 + -0xc) = (undefined1 *)((int)piVar16 + 0x224);
      *(undefined1 **)((int)piVar16 + -0x10) = (undefined1 *)((int)piVar16 + 0x200);
      uVar8 = FUN_8c0747d2(param_2,param_3,param_4,pfVar13);
      *(undefined4 *)((int)piVar16 + 0x14) = uVar8;
    }
LAB_8c07418a:
    *piVar16 = *piVar16 + 1;
  }
  if (piVar16[0x13] == 1) {
    *(int *)(param_4 + 0x74) = piVar16[0xe];
    if (in_FPSCR_SZ == '\0') {
      dVar23 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x20),pfVar13[4]);
    }
    else {
      dVar23 = *(double *)(param_2 + 0x20);
    }
    if (in_FPSCR_PR == '\0') {
      dVar23 = (double)CONCAT44((float)((ulonglong)dVar23 >> 0x20) + SUB84(dVar23,0),SUB84(dVar23,0)
                               );
    }
    else {
      dVar23 = dVar23 + dVar23;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_2 + 0x20) = (int)((ulonglong)dVar23 >> 0x20);
      dVar23 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x24),pfVar13[5]);
    }
    else {
      *(double *)(param_2 + 0x20) = dVar23;
      dVar23 = *(double *)(param_2 + 0x24);
    }
    if (in_FPSCR_PR == '\0') {
      dVar23 = (double)CONCAT44((float)((ulonglong)dVar23 >> 0x20) + SUB84(dVar23,0),SUB84(dVar23,0)
                               );
    }
    else {
      dVar23 = dVar23 + dVar23;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_2 + 0x24) = (int)((ulonglong)dVar23 >> 0x20);
      dVar23 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x28),pfVar13[6]);
    }
    else {
      *(double *)(param_2 + 0x24) = dVar23;
      dVar23 = *(double *)(param_2 + 0x28);
    }
    if (in_FPSCR_PR == '\0') {
      dVar23 = (double)CONCAT44((float)((ulonglong)dVar23 >> 0x20) + SUB84(dVar23,0),SUB84(dVar23,0)
                               );
    }
    else {
      dVar23 = dVar23 + dVar23;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_2 + 0x28) = (int)((ulonglong)dVar23 >> 0x20);
    }
    else {
      *(double *)(param_2 + 0x28) = dVar23;
    }
    goto LAB_8c0742b0;
  }
  if (in_FPSCR_SZ == '\0') {
    piVar16[0xc] = SUB84(dVar25,0);
  }
  else {
    *(undefined8 *)(piVar16 + 0xc) = in_xd14;
  }
  sVar3 = *(short *)(param_4 + 0x10);
  piVar16[2] = (int)sVar3;
  if ((((int)sVar3 & 1U) == 0) || ((piVar16[2] & 0x4000U) != 0)) {
LAB_8c074246:
    if (in_FPSCR_SZ == '\0') {
      dVar23 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x20),pfVar13[4]);
    }
    else {
      dVar23 = *(double *)(param_2 + 0x20);
    }
    if (in_FPSCR_PR == '\0') {
      dVar23 = (double)CONCAT44((float)((ulonglong)dVar23 >> 0x20) + SUB84(dVar23,0),SUB84(dVar23,0)
                               );
    }
    else {
      dVar23 = dVar23 + dVar23;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_2 + 0x20) = (int)((ulonglong)dVar23 >> 0x20);
      dVar23 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x24),pfVar13[5]);
    }
    else {
      *(double *)(param_2 + 0x20) = dVar23;
      dVar23 = *(double *)(param_2 + 0x24);
    }
    if (in_FPSCR_PR == '\0') {
      dVar23 = (double)CONCAT44((float)((ulonglong)dVar23 >> 0x20) + SUB84(dVar23,0),SUB84(dVar23,0)
                               );
    }
    else {
      dVar23 = dVar23 + dVar23;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_2 + 0x24) = (int)((ulonglong)dVar23 >> 0x20);
      dVar23 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x28),pfVar13[6]);
    }
    else {
      *(double *)(param_2 + 0x24) = dVar23;
      dVar23 = *(double *)(param_2 + 0x28);
    }
    if (in_FPSCR_PR == '\0') {
      dVar23 = (double)CONCAT44((float)((ulonglong)dVar23 >> 0x20) + SUB84(dVar23,0),SUB84(dVar23,0)
                               );
    }
    else {
      dVar23 = dVar23 + dVar23;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_2 + 0x28) = (int)((ulonglong)dVar23 >> 0x20);
    }
    else {
      *(double *)(param_2 + 0x28) = dVar23;
    }
  }
  else {
    iVar4 = (*(code *)PTR_FUN_8c074380)(param_2,0,piVar16 + 0xc);
    if (iVar4 != 0) {
      if (in_FPSCR_SZ == '\0') {
        uVar19 = CONCAT44(piVar16[0xc],(undefined4)DAT_8c074384);
      }
      else {
        uVar19 = *(undefined8 *)(piVar16 + 0xc);
      }
      if ((in_FPSCR_PR == '\0' && (float)uVar19 < (float)((ulonglong)uVar19 >> 0x20)) ||
         (*(short *)(param_4 + 10) != 0)) goto LAB_8c074246;
    }
    if (in_FPSCR_SZ == '\0') {
      lVar24 = (ulonglong)*(uint *)(param_4 + 0x3c) << 0x20;
    }
    else {
      lVar24 = *(longlong *)(param_4 + 0x3c);
    }
    if (in_FPSCR_PR == '\0' && (float)lVar24 < (float)((ulonglong)lVar24 >> 0x20))
    goto LAB_8c074246;
    if (in_FPSCR_SZ == '\0') {
      dVar22 = (double)CONCAT44(*(undefined4 *)(param_4 + 0xc4),(float)lVar24);
      dVar20 = (double)CONCAT44((int)((ulonglong)extraout_dr0 >> 0x20),
                                *(undefined4 *)(param_4 + 0x38));
    }
    else {
      dVar22 = *(double *)(param_4 + 0xc4);
      dVar20 = extraout_dr0;
    }
    if (in_FPSCR_PR == '\0') {
      bVar26 = (float)((ulonglong)dVar22 >> 0x20) < SUB84(dVar20,0);
    }
    else {
      bVar26 = dVar22 < dVar20;
    }
    if ((bVar26) || ((*(uint *)(param_4 + 0x60) & 0x1c) != 0)) goto LAB_8c074246;
    if (in_FPSCR_SZ == '\0') {
      dVar22 = (double)CONCAT44(DAT_8c074384._4_4_,SUB84(dVar22,0));
      dVar20 = (double)CONCAT44((int)((ulonglong)dVar20 >> 0x20),*(undefined4 *)(param_4 + 0x1c));
    }
    else {
      dVar22 = (double)CONCAT44(PTR_FUN_8c07438c,DAT_8c074384._4_4_);
    }
    if (in_FPSCR_PR == '\0') {
      bVar26 = (float)((ulonglong)dVar22 >> 0x20) < SUB84(dVar20,0);
    }
    else {
      bVar26 = dVar22 < dVar20;
    }
    if (!bVar26) goto LAB_8c074246;
    if (in_FPSCR_SZ == '\0') {
      dVar22 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x20),*(undefined4 *)(param_3 + 4));
    }
    else {
      dVar22 = *(double *)(param_2 + 0x20);
    }
    fVar21 = (float)((ulonglong)dVar23 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      fVar7 = SUB84(dVar22,0) * fVar21;
      dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) - fVar7,fVar7);
    }
    else {
      dVar22 = dVar22 * dVar23 - dVar22 * dVar23;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_2 + 0x20) = (int)((ulonglong)dVar22 >> 0x20);
      dVar22 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x24),*(undefined4 *)(param_3 + 8));
    }
    else {
      *(double *)(param_2 + 0x20) = dVar22;
      dVar22 = *(double *)(param_2 + 0x24);
    }
    if (in_FPSCR_PR == '\0') {
      fVar7 = SUB84(dVar22,0) * fVar21;
      dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) - fVar7,fVar7);
    }
    else {
      dVar22 = dVar22 * dVar23 - dVar22 * dVar23;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_2 + 0x24) = (int)((ulonglong)dVar22 >> 0x20);
      dVar22 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x28),*(undefined4 *)(param_3 + 0xc));
    }
    else {
      *(double *)(param_2 + 0x24) = dVar22;
      dVar22 = *(double *)(param_2 + 0x28);
    }
    if (in_FPSCR_PR == '\0') {
      fVar21 = SUB84(dVar22,0) * fVar21;
      dVar23 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) - fVar21,fVar21);
    }
    else {
      dVar23 = dVar22 * dVar23 - dVar22 * dVar23;
    }
    uVar8 = SUB84(dVar25,0);
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_2 + 0x28) = (int)((ulonglong)dVar23 >> 0x20);
      *(undefined4 *)(param_3 + 4) = uVar8;
      *(undefined4 *)(param_3 + 8) = uVar8;
      *(undefined4 *)(param_3 + 0xc) = uVar8;
    }
    else {
      *(double *)(param_2 + 0x28) = dVar23;
      *(undefined8 *)(param_3 + 4) = in_xd14;
      *(undefined8 *)(param_3 + 8) = in_xd14;
      *(undefined8 *)(param_3 + 0xc) = in_xd14;
    }
  }
  (*(code *)PTR_FUN_8c07438c)(param_2,param_3,param_4,0);
LAB_8c0742b0:
  return piVar16[5];
}

