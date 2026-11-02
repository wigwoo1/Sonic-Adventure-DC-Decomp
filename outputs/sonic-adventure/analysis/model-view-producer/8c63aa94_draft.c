/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c63aa94(undefined4 param_1,int param_2,int param_3)

{
  ushort uVar1;
  ushort uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  undefined8 *puVar6;
  int iVar7;
  float fVar8;
  double in_dr0;
  undefined4 uVar11;
  double dVar9;
  float fVar12;
  float fVar13;
  double dVar10;
  undefined8 uVar14;
  undefined4 uVar15;
  double dVar16;
  undefined4 uVar17;
  undefined8 in_xd0;
  undefined8 in_xd2;
  undefined8 in_xd8;
  double in_xd10;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  iVar7 = DAT_8c63aaa8;
  iVar3 = (int)DAT_8c63aaa6;
  iVar4 = *(int *)(iVar3 + DAT_8c63aaa8);
  if (0x1f < iVar4) {
    return;
  }
  *(int *)(iVar4 * 4 + DAT_8c63aaa8) = param_2;
  *(int *)(iVar3 + iVar7) = iVar4 + 1;
  iVar3 = DAT_8c63ab98 + param_2;
  iVar7 = DAT_8c63ab9a + param_2;
  dVar16 = in_xd10;
  if (in_FPSCR_SZ == '\0') {
    dVar16 = 0.007812501848093234;
  }
  if (in_FPSCR_PR == '\0') {
    dVar16 = (double)CONCAT44(-(float)((ulonglong)dVar16 >> 0x20),SUB84(dVar16,0));
  }
  else {
    dVar16 = -dVar16;
  }
  *(undefined4 *)(param_2 + 0x58) = 1;
  uVar17 = (undefined4)((ulonglong)dVar16 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(param_2 + 0x54) = uVar17;
    *(undefined4 *)(param_2 + 0x50) = 0;
    *(undefined4 *)(param_2 + 0x4c) = 0;
    *(undefined4 *)(param_2 + 0x48) = 0;
    *(undefined4 *)(param_2 + 0x44) = 0;
    *(undefined4 *)(param_2 + 0x40) = 0;
  }
  else {
    *(double *)(param_2 + 0x54) = dVar16;
    *(undefined8 *)(param_2 + 0x4c) = in_xd8;
    *(undefined8 *)(param_2 + 0x44) = in_xd8;
    *(undefined8 *)(param_2 + 0x3c) = in_xd8;
    *(undefined8 *)(param_2 + 0x34) = in_xd8;
    *(undefined8 *)(param_2 + 0x2c) = in_xd8;
  }
  uVar2 = DAT_8c63ab9e;
  uVar1 = DAT_8c63ab9c;
  iVar5 = (int)(short)DAT_8c63ab9c;
  iVar4 = (int)(short)DAT_8c63ab9e;
  uVar15 = SUB84(dVar16,0);
  if (in_FPSCR_SZ == '\0') {
    dVar9 = (double)CONCAT44(DAT_8c63aba8,DAT_8c63aba4);
    in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),DAT_8c63abac);
    *(undefined4 *)(iVar3 + 0x30) = uVar15;
    *(undefined4 *)(iVar3 + 0x2c) = uVar15;
    *(undefined4 *)(iVar3 + 0x28) = uVar15;
    puVar6 = (undefined8 *)(iVar3 + 0x24);
    *(undefined4 *)puVar6 = 0;
  }
  else {
    in_xd2 = CONCAT44(DAT_8c63aba8,DAT_8c63aba4);
    dVar9 = (double)CONCAT44(DAT_8c63abac,DAT_8c63aba8);
    in_xd0 = CONCAT44(_DAT_8c63abb0,DAT_8c63abac);
    *(double *)(iVar3 + 0x30) = in_xd10;
    *(double *)(iVar3 + 0x28) = in_xd10;
    *(double *)(iVar3 + 0x20) = in_xd10;
    puVar6 = (undefined8 *)(iVar3 + 0x18);
    *puVar6 = in_xd8;
  }
  *(int *)((int)puVar6 + -4) = iVar5;
  *(int *)(puVar6 + -1) = iVar4;
  uVar11 = (undefined4)((ulonglong)dVar9 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    *(int *)(puVar6 + -2) = SUB84(dVar9,0);
    *(undefined4 *)((int)puVar6 + -0x14) = uVar15;
    *(undefined4 *)(puVar6 + -3) = uVar11;
    *(undefined4 *)((int)puVar6 + -0x1c) = uVar15;
    puVar6 = puVar6 + -4;
    *(int *)puVar6 = SUB84(in_dr0,0);
  }
  else {
    *(undefined8 *)((int)puVar6 + -0x14) = in_xd2;
    *(double *)((int)puVar6 + -0x1c) = in_xd10;
    *(double *)((int)puVar6 + -0x24) = dVar9;
    *(double *)((int)puVar6 + -0x2c) = in_xd10;
    puVar6 = (undefined8 *)((int)puVar6 + -0x34);
    *puVar6 = in_xd0;
  }
  *(int *)((int)puVar6 + -4) = param_3;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(iVar7 + 0x44) = uVar17;
    *(undefined4 *)(iVar7 + 0x40) = 0;
    *(undefined4 *)(iVar7 + 0x3c) = 0;
    *(undefined4 *)(iVar7 + 0x38) = 0;
    *(undefined4 *)(iVar7 + 0x34) = 0;
    *(undefined4 *)(iVar7 + 0x30) = 0;
    dVar16 = (double)CONCAT44(*(undefined4 *)(iVar3 + 0x10),param_1);
    dVar9 = (double)CONCAT44(uVar11,*(undefined4 *)(iVar3 + 0x14));
  }
  else {
    *(double *)(iVar7 + 0x44) = dVar16;
    *(undefined8 *)(iVar7 + 0x3c) = in_xd8;
    *(undefined8 *)(iVar7 + 0x34) = in_xd8;
    *(undefined8 *)(iVar7 + 0x2c) = in_xd8;
    *(undefined8 *)(iVar7 + 0x24) = in_xd8;
    *(undefined8 *)(iVar7 + 0x1c) = in_xd8;
    dVar16 = *(double *)(iVar3 + 0x10);
    in_xd2 = *(undefined8 *)(iVar3 + 0x14);
  }
  if (in_FPSCR_PR == '\0') {
    fVar13 = (float)((ulonglong)dVar16 >> 0x20);
    dVar16 = (double)CONCAT44(fVar13 * fVar13,SUB84(dVar16,0));
    dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),SUB84(dVar9,0) * SUB84(dVar9,0));
  }
  else {
    dVar16 = dVar16 * dVar16;
    dVar9 = dVar9 * dVar9;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(iVar7 + 8) = (int)((ulonglong)dVar16 >> 0x20);
    *(int *)(iVar7 + 0xc) = SUB84(dVar9,0);
  }
  else {
    *(double *)(iVar7 + 8) = dVar16;
    *(undefined8 *)(iVar7 + 0xc) = in_xd2;
  }
  if (in_FPSCR_PR == '\0') {
    fVar13 = ((float)uVar2 * 6.2831855) / 65536.0;
    sin(fVar13);
    fVar8 = (float)cos(fVar13);
    fVar13 = ((float)uVar1 * 6.2831855) / 65536.0;
    fVar12 = (float)sin(fVar13);
    fVar13 = (float)cos(fVar13);
    fVar8 = (float)fVar8 * (float)fVar8;
    dVar9 = (double)CONCAT44((float)fVar12,(float)fVar13 * (float)fVar13);
  }
  else {
    fVar8 = SUB84(in_dr0 * in_dr0,0);
    dVar9 = dVar9 * dVar9;
  }
  if (in_FPSCR_SZ == '\0') {
    *(float *)(iVar7 + 0x10) = fVar8;
    *(int *)(iVar7 + 0x14) = SUB84(dVar9,0);
    dVar16 = (double)CONCAT44(*(undefined4 *)(iVar3 + 0x10),SUB84(dVar16,0));
    dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),*(undefined4 *)(iVar3 + 0x14));
  }
  else {
    *(undefined8 *)(iVar7 + 0x10) = in_xd0;
    *(undefined8 *)(iVar7 + 0x14) = in_xd2;
    dVar16 = *(double *)(iVar3 + 0x10);
    in_xd2 = *(undefined8 *)(iVar3 + 0x14);
  }
  fVar13 = SUB84(dVar9,0);
  if (in_FPSCR_PR == '\0') {
    dVar16 = (double)CONCAT44(-((float)((ulonglong)dVar16 >> 0x20) - fVar13),SUB84(dVar16,0));
  }
  else {
    dVar16 = -(dVar16 - dVar9);
  }
  if (in_FPSCR_PR == '\0') {
    fVar12 = 1.0 / (float)((ulonglong)dVar16 >> 0x20);
    dVar9 = (double)CONCAT44(fVar12,fVar13 * fVar12);
  }
  else {
    dVar9 = (double)CONCAT44(0x3f800000,fVar13) / dVar16;
    dVar9 = dVar9 * dVar9;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(iVar7 + 0x18) = (int)((ulonglong)dVar9 >> 0x20);
    *(int *)(iVar7 + 0x1c) = SUB84(dVar9,0);
  }
  else {
    *(double *)(iVar7 + 0x18) = dVar9;
    *(undefined8 *)(iVar7 + 0x1c) = in_xd2;
  }
  *(ushort *)(iVar7 + 0x2c) = uVar1;
  *(ushort *)(iVar7 + 0x2e) = uVar2;
  if (in_FPSCR_SZ == '\0') {
    dVar16 = (double)CONCAT44(*(undefined4 *)_DAT_8c63abb0,SUB84(dVar16,0));
  }
  else {
    dVar16 = *_DAT_8c63abb0;
  }
  if (in_FPSCR_PR == '\0') {
    iVar4 = (int)(float)((ulonglong)dVar16 >> 0x20);
  }
  else {
    iVar4 = (int)dVar16;
  }
  *(int *)(iVar7 + 0x28) = iVar4;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(iVar7 + 0x24) = uVar15;
  }
  else {
    *(double *)(iVar7 + 0x24) = in_xd10;
  }
  if ((param_3 == DAT_8c63aba0) || (param_3 == DAT_8c63aba2)) {
    if (in_FPSCR_SZ == '\0') {
      dVar9 = (double)CONCAT44(*(undefined4 *)(iVar7 + 0x10),*(undefined4 *)(iVar7 + 0x14));
    }
    else {
      dVar9 = *(double *)(iVar7 + 0x10);
    }
    if (in_FPSCR_PR == '\0') {
      fVar13 = (float)((ulonglong)dVar9 >> 0x20) - SUB84(dVar9,0);
      dVar10 = (double)CONCAT44(1.0 / SQRT(fVar13 * fVar13),SUB84(dVar9,0));
    }
    else {
      dVar10 = (dVar9 - dVar9) * (dVar9 - dVar9);
    }
  }
  else {
    if (in_FPSCR_SZ == '\0') {
      dVar16 = (double)CONCAT44(*(undefined4 *)(iVar7 + 0x10),SUB84(dVar16,0));
    }
    else {
      dVar16 = *(double *)(iVar7 + 0x10);
    }
    dVar10 = (double)FUN_8c63aa04(dVar16,*(undefined4 *)(iVar7 + 0x28));
    if (in_FPSCR_SZ == '\0') {
      dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),SUB84(dVar9,0));
    }
    if (in_FPSCR_PR == '\0') {
      fVar13 = (float)((ulonglong)dVar10 >> 0x20);
      dVar10 = (double)CONCAT44(1.0 / SQRT(fVar13 * fVar13),SUB84(dVar10,0));
    }
    else {
      dVar10 = dVar10 * dVar10;
    }
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(iVar7 + 0x20) = (int)((ulonglong)dVar10 >> 0x20);
    uVar14 = CONCAT44(*(undefined4 *)(iVar7 + 0x24),SUB84(dVar16,0));
  }
  else {
    *(double *)(iVar7 + 0x20) = dVar10;
    uVar14 = *(undefined8 *)(iVar7 + 0x24);
  }
  FUN_8c63aa74(uVar14,iVar7 + 0x54,iVar3 + 0x24);
  if (in_FPSCR_SZ == '\0') {
    uVar14 = CONCAT44(*(undefined4 *)(iVar3 + 4),(int)uVar14);
  }
  else {
    uVar14 = *(undefined8 *)(iVar3 + 4);
  }
  FUN_8c63aa74(uVar14,iVar7 + 100);
  if (in_FPSCR_SZ == '\0') {
    uVar14 = CONCAT44(*(undefined4 *)(iVar3 + 8),(int)uVar14);
  }
  else {
    uVar14 = *(undefined8 *)(iVar3 + 8);
  }
  FUN_8c63aa74(uVar14,iVar7 + 0x74);
  if (in_FPSCR_SZ == '\0') {
    uVar14 = CONCAT44(*(undefined4 *)(iVar3 + 0xc),(int)uVar14);
  }
  else {
    uVar14 = *(undefined8 *)(iVar3 + 0xc);
  }
  FUN_8c63aa74(uVar14,iVar7 + 0x84);
  (*(code *)PTR_FUN_8c63ac0c)(param_2);
  return;
}

