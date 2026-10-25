/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c604ba0(undefined4 param_1)

{
  code *pcVar1;
  undefined4 *puVar2;
  undefined8 *puVar3;
  undefined8 *puVar4;
  undefined8 *puVar5;
  undefined8 *puVar6;
  ulonglong *puVar7;
  uint *puVar8;
  undefined *puVar9;
  int iVar10;
  ulonglong *puVar11;
  undefined1 *puVar12;
  undefined1 *puVar13;
  ulonglong *puVar14;
  undefined4 uVar15;
  uint uVar16;
  undefined4 *puVar17;
  uint uVar18;
  uint uVar19;
  float fVar20;
  double in_dr2;
  double dVar21;
  ulonglong uVar22;
  undefined4 uVar23;
  double dVar24;
  undefined8 uVar25;
  ulonglong in_xd2;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 local_1c [4];
  
  puVar3 = DAT_8c604ce0;
  puVar2 = DAT_8c604cdc;
  puVar17 = DAT_8c604cd8;
  iVar10 = (int)DAT_8c604ccc;
  puVar12 = local_1c + iVar10;
  puVar13 = local_1c + iVar10;
  if (in_FPSCR_PR == '\0') {
    dVar24 = (double)CONCAT44(0x40000000,param_1);
  }
  else {
    dVar24 = (double)CONCAT44(0x3f800000,param_1) + (double)CONCAT44(0x3f800000,param_1);
  }
  *DAT_8c604cd4 = 0;
  *puVar17 = 0;
  *puVar2 = 0;
  puVar6 = DAT_8c604cf0;
  puVar5 = DAT_8c604ce8;
  puVar4 = DAT_8c604ce4;
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)((int)puVar3 + 4));
  }
  else {
    in_xd2 = *(ulonglong *)((int)puVar3 + 4);
  }
  fVar20 = (float)((ulonglong)dVar24 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar21 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),SUB84(in_dr2,0) / fVar20);
  }
  else {
    dVar21 = in_dr2 / dVar24;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)puVar3 + 0xc) = SUB84(dVar21,0);
    dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),*(undefined4 *)(puVar3 + 1));
  }
  else {
    *(ulonglong *)((int)puVar3 + 0xc) = in_xd2;
    in_xd2 = puVar3[1];
  }
  if (in_FPSCR_PR == '\0') {
    dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),SUB84(dVar21,0) / fVar20);
  }
  else {
    dVar21 = dVar21 / dVar24;
  }
  uVar15 = SUB84(dVar24,0);
  uVar25 = CONCAT44(0x3f800000,uVar15);
  if (in_FPSCR_SZ == '\0') {
    *(int *)(puVar3 + 2) = SUB84(dVar21,0);
    *(undefined4 *)puVar4 = 0x3f800000;
    *(undefined4 *)puVar5 = 0x3f800000;
    *(undefined4 *)puVar3 = DAT_8c604cec;
    *(undefined4 *)puVar6 = 0x3f800000;
    dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),DAT_8c604cf4);
  }
  else {
    puVar3[2] = in_xd2;
    *puVar4 = uVar25;
    *puVar5 = uVar25;
    *puVar3 = CONCAT44(DAT_8c604cf0,DAT_8c604cec);
    *puVar6 = uVar25;
    in_xd2 = CONCAT44(DAT_8c604cf8,DAT_8c604cf4);
  }
  puVar14 = (ulonglong *)(local_1c + DAT_8c604cce + iVar10);
  if (in_FPSCR_SZ == '\0') {
    *(int *)DAT_8c604cf8 = SUB84(dVar21,0);
  }
  else {
    *DAT_8c604cf8 = in_xd2;
  }
  puVar7 = DAT_8c604d08;
  puVar2 = DAT_8c604d04;
  puVar17 = DAT_8c604d00;
  puVar11 = puVar14 + 1;
  *DAT_8c604cfc = 0;
  *puVar17 = 0;
  *puVar2 = 0;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)puVar14 = 0;
    *(undefined4 *)((int)puVar14 + 4) = 0;
    dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),*(undefined4 *)((int)puVar3 + 4));
  }
  else {
    *puVar14 = (ulonglong)dVar24 & 0xffffffff;
    *(ulonglong *)((int)puVar14 + 4) = (ulonglong)dVar24 & 0xffffffff;
    in_xd2 = *(ulonglong *)((int)puVar3 + 4);
  }
  if (in_FPSCR_PR == '\0') {
    dVar24 = (double)CONCAT44(0xbf800000,uVar15);
    fVar20 = -1.0;
    dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),SUB84(dVar21,0) + -1.0);
  }
  else {
    dVar24 = -(double)CONCAT44(0x3f800000,uVar15);
    fVar20 = (float)((ulonglong)dVar24 >> 0x20);
    dVar21 = dVar21 + dVar24;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)puVar11 = SUB84(dVar21,0);
    dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),*(undefined4 *)(puVar3 + 1));
  }
  else {
    *puVar11 = in_xd2;
    in_xd2 = puVar3[1];
  }
  if (in_FPSCR_PR == '\0') {
    fVar20 = SUB84(dVar21,0) + fVar20;
  }
  else {
    fVar20 = SUB84(dVar21 + dVar24,0);
  }
  if (in_FPSCR_SZ == '\0') {
    *(float *)((int)puVar14 + 0xc) = fVar20;
    *(int *)puVar7 = (int)*puVar14;
    *(undefined4 *)((int)puVar7 + 4) = *(undefined4 *)((int)puVar14 + 4);
    pcVar1 = (code *)PTR_FUN_8c604d0c;
    uVar22 = *puVar11;
    *(int *)(puVar7 + 1) = (int)uVar22;
    uVar22 = CONCAT44(*(undefined4 *)((int)puVar14 + 0xc),(int)uVar22);
    *(undefined4 *)((int)puVar7 + 0xc) = *(undefined4 *)((int)puVar14 + 0xc);
  }
  else {
    *(ulonglong *)((int)puVar14 + 0xc) = in_xd2;
    *puVar7 = *puVar14;
    *(undefined8 *)((int)puVar7 + 4) = *(undefined8 *)((int)puVar14 + 4);
    pcVar1 = (code *)PTR_FUN_8c604d0c;
    in_xd2 = *puVar11;
    puVar7[1] = in_xd2;
    uVar22 = *(ulonglong *)((int)puVar14 + 0xc);
    *(ulonglong *)((int)puVar7 + 0xc) = uVar22;
  }
  (*pcVar1)();
  uVar15 = DAT_8c604d18;
  uVar19 = (uint)DAT_8c604cce;
  uVar16 = (int)DAT_8c604cd0 + DAT_8c604d14;
  puVar17 = DAT_8c604d10;
  uVar18 = DAT_8c604d14;
  do {
    FUN_8c605006(uVar18);
    uVar23 = SUB84(dVar24,0);
    uVar18 = uVar18 + uVar19;
    *puVar17 = uVar15;
    puVar17 = puVar17 + 1;
  } while (uVar18 < uVar16);
  FUN_8c605006(DAT_8c604d1c);
  puVar8 = DAT_8c604d1c;
  *DAT_8c604d1c = uVar19;
  puVar8[0xb] = 10;
  puVar9 = PTR_FUN_8c604d20;
  puVar8[0xc] = 0xb;
  (*(code *)puVar9)();
  (*(code *)PTR_FUN_8c604d24)(DAT_8c604d1c);
  (*(code *)PTR_FUN_8c604d28)();
  if (in_FPSCR_SZ == '\0') {
    uVar25 = CONCAT44(_DAT_8c604d2c,uVar23);
  }
  else {
    uVar25 = CONCAT44(PTR_FUN_8c604d30,_DAT_8c604d2c);
  }
  (*(code *)PTR_FUN_8c604d30)(uVar25);
  FUN_8c604fd0(0);
  (*(code *)PTR_FUN_8c604d38)();
  iVar10 = DAT_8c604d3c;
  uVar18 = 0;
  do {
    uVar16 = uVar18 + 4;
    if (in_FPSCR_SZ == '\0') {
      uVar22 = (ulonglong)*(uint *)(puVar12 + uVar18);
    }
    else {
      in_xd2 = *(ulonglong *)(puVar12 + uVar18);
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(uVar18 + iVar10) = (int)uVar22;
    }
    else {
      *(ulonglong *)(uVar18 + iVar10) = in_xd2;
    }
    puVar9 = PTR_FUN_8c604d38;
    uVar18 = uVar16;
  } while (uVar16 < uVar19);
  uVar15 = 3;
  *(undefined4 *)(iVar10 + 0x40) = 3;
  (*(code *)puVar9)();
  iVar10 = DAT_8c604d44;
  uVar18 = 0;
  do {
    uVar16 = uVar18 + 4;
    if (in_FPSCR_SZ == '\0') {
      uVar22 = (ulonglong)*(uint *)(puVar13 + uVar18);
    }
    else {
      in_xd2 = *(ulonglong *)(puVar13 + uVar18);
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(uVar18 + iVar10) = (int)uVar22;
    }
    else {
      *(ulonglong *)(uVar18 + iVar10) = in_xd2;
    }
    puVar17 = DAT_8c604e34;
    uVar18 = uVar16;
  } while (uVar16 < uVar19);
  *(undefined4 *)(iVar10 + 0x40) = uVar15;
  *puVar17 = 0;
  return;
}

