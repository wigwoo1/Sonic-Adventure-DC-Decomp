/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c640610) */

void FUN_8c640540(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                 undefined8 *param_5)

{
  double dVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined *puVar5;
  undefined8 *puVar6;
  undefined1 *puVar7;
  undefined4 in_fr1;
  undefined8 uVar8;
  undefined4 in_fr3;
  float fVar10;
  double dVar11;
  undefined8 uVar13;
  undefined8 uVar15;
  undefined8 uVar17;
  double dVar19;
  undefined8 in_dr12;
  double dVar21;
  undefined8 in_dr14;
  double dVar23;
  undefined8 in_xd0;
  undefined8 in_xd2;
  undefined8 in_xd4;
  undefined8 in_xd6;
  undefined8 in_xd8;
  undefined8 in_xd10;
  undefined8 in_xd12;
  undefined8 in_xd14;
  bool bVar24;
  undefined4 in_PR;
  uint uVar25;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined4 auStack_28 [5];
  undefined1 auStack_14 [8];
  undefined8 uStack_c;
  undefined8 *puVar4;
  undefined4 uVar9;
  undefined4 uVar12;
  undefined4 uVar14;
  undefined4 uVar16;
  undefined4 uVar18;
  undefined4 uVar20;
  float fVar22;
  
  if (in_FPSCR_SZ == '\0') {
    uStack_c = CONCAT44((int)in_dr14,(undefined4)uStack_c);
    puVar6 = &uStack_c;
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_c._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
  }
  else {
    puVar6 = (undefined8 *)auStack_14;
    uStack_c = in_xd14;
  }
  iVar2 = *(int *)(param_5 + 0xb);
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)puVar6 + -4) = (int)in_dr12;
    puVar7 = (undefined1 *)((int)puVar6 + -8);
    *(int *)((int)puVar6 + -8) = (int)((ulonglong)in_dr12 >> 0x20);
  }
  else {
    *(undefined8 *)((int)puVar6 + -8) = in_xd12;
    puVar7 = (undefined1 *)((int)puVar6 + -0x10);
    *(undefined8 *)((int)puVar6 + -0x10) = in_dr12;
  }
  *(undefined4 *)(puVar7 + -4) = in_PR;
  if (in_FPSCR_SZ == '\0') {
    uVar9 = *(undefined4 *)(PTR_DAT_8c640630 + 0x20);
    uVar12 = *(undefined4 *)(PTR_DAT_8c640630 + 0x24);
    dVar11 = (double)CONCAT44(uVar12,in_fr3);
    uVar14 = *(undefined4 *)(PTR_DAT_8c640630 + 0x28);
    uVar16 = *(undefined4 *)(PTR_DAT_8c640630 + 0x2c);
    puVar5 = PTR_DAT_8c640630 + 0x30;
    uVar18 = *(undefined4 *)PTR_DAT_8c640630;
    uVar20 = *(undefined4 *)(PTR_DAT_8c640630 + 4);
    dVar19 = (double)CONCAT44(uVar20,param_4);
    puVar3 = (undefined4 *)(PTR_DAT_8c640630 + 8);
    *(undefined4 *)((int)param_5 + 0x3c) = param_2;
    *(undefined4 *)(param_5 + 7) = uVar16;
    *(undefined4 *)((int)param_5 + 0x34) = param_1;
    *(undefined4 *)(param_5 + 6) = uVar14;
    *(undefined4 *)((int)param_5 + 0x2c) = in_fr3;
    *(undefined4 *)(param_5 + 5) = uVar12;
    *(undefined4 *)((int)param_5 + 0x24) = in_fr1;
    puVar4 = param_5 + 4;
    *(undefined4 *)puVar4 = uVar9;
    uVar12 = *puVar3;
    uVar13 = CONCAT44(uVar12,param_2);
    uVar9 = puVar3[1];
    uVar8 = CONCAT44(uVar9,param_1);
    *(undefined4 *)((int)puVar4 + -4) = param_1;
    *(undefined4 *)(puVar4 + -1) = uVar9;
    *(undefined4 *)((int)puVar4 + -0xc) = param_2;
    *(undefined4 *)(puVar4 + -2) = uVar12;
    *(undefined4 *)((int)puVar4 + -0x14) = param_4;
    *(undefined4 *)(puVar4 + -3) = uVar20;
    *(undefined4 *)((int)puVar4 + -0x1c) = param_3;
    *(undefined4 *)(puVar4 + -4) = uVar18;
  }
  else {
    uVar8 = *(undefined8 *)(PTR_DAT_8c640630 + 0x20);
    dVar11 = *(double *)(PTR_DAT_8c640630 + 0x28);
    uVar13 = *(undefined8 *)(PTR_DAT_8c640630 + 0x30);
    uVar15 = *(undefined8 *)(PTR_DAT_8c640630 + 0x38);
    puVar5 = PTR_DAT_8c640630 + 0x40;
    uVar17 = *(undefined8 *)PTR_DAT_8c640630;
    dVar19 = *(double *)(PTR_DAT_8c640630 + 8);
    puVar4 = (undefined8 *)(PTR_DAT_8c640630 + 0x10);
    param_5[7] = in_xd6;
    param_5[6] = uVar15;
    param_5[5] = in_xd4;
    param_5[4] = uVar13;
    param_5[3] = in_xd2;
    param_5[2] = dVar11;
    param_5[1] = in_xd0;
    *param_5 = uVar8;
    uVar13 = *puVar4;
    uVar8 = puVar4[1];
    param_5[-1] = in_xd4;
    param_5[-2] = uVar8;
    param_5[-3] = in_xd6;
    param_5[-4] = uVar13;
    param_5[-5] = in_xd10;
    param_5[-6] = dVar19;
    param_5[-7] = in_xd8;
    param_5[-8] = uVar17;
  }
  uVar9 = (undefined4)uVar8;
  uVar12 = (undefined4)uVar13;
  (*(code *)PTR_FUN_8c640634)(param_5,-iVar2,puVar5);
  if (in_FPSCR_SZ == '\0') {
    fVar22 = *(float *)((int)param_5 + 0x4c);
    dVar21 = (double)CONCAT44(fVar22,*(undefined4 *)(param_5 + 10));
    fVar10 = (float)((ulonglong)in_xd14 >> 0x20);
    dVar23 = (double)CONCAT44(*(undefined4 *)((int)param_5 + 0x54),fVar22);
  }
  else {
    dVar21 = *(double *)((int)param_5 + 0x4c);
    in_xd12 = *(undefined8 *)((int)param_5 + 0x54);
    fVar10 = (float)((ulonglong)dVar21 >> 0x20);
    dVar23 = *(double *)((int)param_5 + 0x5c);
    fVar22 = fVar10;
  }
  if (in_FPSCR_PR == '\0') {
    dVar23 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),SUB84(dVar23,0) * fVar22);
  }
  else {
    dVar23 = dVar23 * dVar21;
  }
  fVar22 = (float)((ulonglong)dVar23 >> 0x20);
  dVar1 = dVar23;
  if (in_FPSCR_SZ == '\0') {
    dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),SUB84(dVar21,0));
    in_xd12 = in_xd10;
    dVar1 = (double)((ulonglong)(uint)fVar22 << 0x20);
  }
  if (in_FPSCR_PR == '\0') {
    dVar23 = (double)CONCAT44(fVar22,SQRT((float)((ulonglong)dVar1 >> 0x20) * fVar22 +
                                          SUB84(dVar23,0)));
    dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),SUB84(dVar19,0) * SUB84(dVar21,0));
  }
  else {
    dVar23 = SQRT(dVar23);
    dVar19 = dVar19 * dVar21;
  }
  fVar22 = SUB84(dVar23,0);
  if (in_FPSCR_SZ == '\0') {
    fVar10 = fVar22;
  }
  if (in_FPSCR_PR == '\0') {
    fVar22 = (1.0 / SQRT(fVar10 * fVar22 + SUB84(dVar19,0))) * fVar22;
  }
  else {
    fVar22 = SUB84(dVar19 * dVar23,0);
  }
  if (in_FPSCR_SZ == '\0') {
    in_xd12 = CONCAT44(fVar22,uVar9);
  }
  dVar19 = (double)(*(code *)PTR_FUN_8c640638)(in_xd12);
  uVar9 = (undefined4)in_xd12;
  if (in_FPSCR_SZ == '\0') {
    dVar11 = (double)CONCAT44((int)((ulonglong)dVar11 >> 0x20),DAT_8c640654);
  }
  if (in_FPSCR_PR == '\0') {
    fVar10 = SUB84(dVar11,0) * (float)((ulonglong)dVar19 >> 0x20);
  }
  else {
    fVar10 = SUB84(dVar11 * dVar19,0);
  }
  dVar19 = (double)(ulonglong)(uint)fVar10;
  if (in_FPSCR_PR == '\0') {
    uVar25 = (uint)fVar10;
    bVar24 = SUB84(dVar21,0) < 0.0;
  }
  else {
    uVar25 = (uint)dVar19;
    bVar24 = dVar21 < dVar19;
  }
  uVar25 = uVar25 & DAT_8c64063c;
  if (!bVar24) {
    uVar25 = -uVar25;
  }
  (*(code *)PTR_FUN_8c640640)(param_5,-uVar25);
  dVar11 = dVar23;
  if (in_FPSCR_SZ == '\0') {
    dVar11 = (double)CONCAT44((int)((ulonglong)dVar23 >> 0x20),uVar9);
  }
  if (in_FPSCR_PR == '\0') {
    dVar11 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) / SUB84(dVar23,0),SUB84(dVar11,0));
  }
  else {
    dVar11 = dVar11 / dVar23;
  }
  dVar23 = (double)(*(code *)PTR_FUN_8c640638)(dVar11);
  fVar10 = SUB84(dVar11,0);
  if (in_FPSCR_SZ == '\0') {
    dVar19 = (double)CONCAT44((int)((ulonglong)dVar19 >> 0x20),DAT_8c640654);
  }
  if (in_FPSCR_PR == '\0') {
    fVar22 = SUB84(dVar19,0) * (float)((ulonglong)dVar23 >> 0x20);
  }
  else {
    fVar22 = SUB84(dVar19 * dVar23,0);
  }
  if (in_FPSCR_PR == '\0') {
    uVar25 = (uint)fVar22;
    bVar24 = (float)((ulonglong)dVar21 >> 0x20) < 0.0;
  }
  else {
    uVar25 = (uint)(double)(ulonglong)(uint)fVar22;
    bVar24 = dVar21 < (double)(ulonglong)(uint)fVar22;
  }
  uVar25 = uVar25 & DAT_8c64063c;
  if (bVar24) {
    uVar25 = -uVar25;
  }
  (*(code *)PTR_FUN_8c640644)(param_5,-uVar25);
  if (in_FPSCR_SZ == '\0') {
    dVar19 = (double)CONCAT44(*(undefined4 *)(param_5 + 9),uVar12);
    fVar10 = *(float *)((int)param_5 + 0x44);
  }
  else {
    dVar19 = (double)param_5[9];
  }
  if (in_FPSCR_PR == '\0') {
    dVar19 = (double)CONCAT44(-(float)((ulonglong)dVar19 >> 0x20),SUB84(dVar19,0));
  }
  else {
    dVar19 = -dVar19;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar11 = (double)CONCAT44(*(undefined4 *)(param_5 + 8),-fVar10);
  }
  else {
    dVar11 = (double)param_5[8];
  }
  if (in_FPSCR_PR == '\0') {
    dVar11 = (double)CONCAT44(-(float)((ulonglong)dVar11 >> 0x20),SUB84(dVar11,0));
  }
  else {
    dVar11 = -dVar11;
  }
  (*(code *)PTR_FUN_8c640648)(dVar11,dVar19,param_5);
  *DAT_8c64064c = (int)param_5;
  (*(code *)PTR_FUN_8c640650)();
  return;
}

