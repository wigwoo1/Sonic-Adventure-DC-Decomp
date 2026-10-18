/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c639046) */

double FUN_8c638fe4(int param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  int *piVar3;
  undefined *puVar4;
  undefined8 *puVar5;
  double *pdVar6;
  undefined8 uVar7;
  double dVar8;
  undefined4 uVar10;
  undefined8 uVar11;
  undefined4 uVar12;
  float fVar13;
  undefined8 uVar14;
  undefined4 uVar15;
  undefined8 uVar16;
  undefined4 uVar18;
  undefined8 in_xd0;
  undefined8 in_xd2;
  undefined8 uVar19;
  undefined8 in_xd4;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  float fVar9;
  undefined4 uVar17;
  
  puVar1 = DAT_8c639078;
  *DAT_8c639074 = 1;
  puVar2 = DAT_8c63907c;
  *puVar1 = param_3;
  piVar3 = DAT_8c639084;
  *DAT_8c639080 = param_1;
  *puVar2 = param_2;
  *piVar3 = param_1;
  if (in_FPSCR_SZ == '\0') {
    uVar7 = CONCAT44(*(undefined4 *)(PTR_DAT_8c639088 + 0x20),
                     *(undefined4 *)(PTR_DAT_8c639088 + 0x24));
    uVar11 = CONCAT44(*(undefined4 *)(PTR_DAT_8c639088 + 0x28),
                      *(undefined4 *)(PTR_DAT_8c639088 + 0x2c));
    uVar14 = CONCAT44(*(undefined4 *)(PTR_DAT_8c639088 + 0x30),
                      *(undefined4 *)(PTR_DAT_8c639088 + 0x34));
    uVar17 = *(undefined4 *)(PTR_DAT_8c639088 + 0x38);
    puVar4 = PTR_DAT_8c639088 + 0x40;
    *(undefined4 *)(param_1 + 0x3c) = *(undefined4 *)(PTR_DAT_8c639088 + 0x3c);
    puVar5 = (undefined8 *)(param_1 + 0x38);
    *(undefined4 *)puVar5 = uVar17;
  }
  else {
    uVar7 = *(undefined8 *)(PTR_DAT_8c639088 + 0x20);
    in_xd0 = *(undefined8 *)(PTR_DAT_8c639088 + 0x28);
    uVar11 = *(undefined8 *)(PTR_DAT_8c639088 + 0x30);
    in_xd2 = *(undefined8 *)(PTR_DAT_8c639088 + 0x38);
    uVar14 = *(undefined8 *)(PTR_DAT_8c639088 + 0x40);
    in_xd4 = *(undefined8 *)(PTR_DAT_8c639088 + 0x48);
    uVar16 = *(undefined8 *)(PTR_DAT_8c639088 + 0x50);
    puVar4 = PTR_DAT_8c639088 + 0x60;
    *(undefined8 *)(param_1 + 0x38) = *(undefined8 *)(PTR_DAT_8c639088 + 0x58);
    puVar5 = (undefined8 *)(param_1 + 0x30);
    *puVar5 = uVar16;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)puVar5 + -4) = (int)uVar14;
    *(int *)(puVar5 + -1) = (int)((ulonglong)uVar14 >> 0x20);
    *(int *)((int)puVar5 + -0xc) = (int)uVar11;
    *(int *)(puVar5 + -2) = (int)((ulonglong)uVar11 >> 0x20);
    *(int *)((int)puVar5 + -0x14) = (int)uVar7;
    *(int *)(puVar5 + -3) = (int)((ulonglong)uVar7 >> 0x20);
    fVar9 = *(float *)(puVar4 + -0x40);
    uVar17 = *(undefined4 *)(puVar4 + -0x3c);
    dVar8 = (double)CONCAT44(fVar9,uVar17);
    uVar12 = *(undefined4 *)(puVar4 + -0x38);
    uVar10 = *(undefined4 *)(puVar4 + -0x34);
    uVar15 = *(undefined4 *)(puVar4 + -0x30);
    fVar13 = *(float *)(puVar4 + -0x2c);
    uVar18 = *(undefined4 *)(puVar4 + -0x28);
    *(undefined4 *)((int)puVar5 + -0x1c) = *(undefined4 *)(puVar4 + -0x24);
    *(undefined4 *)(puVar5 + -4) = uVar18;
    *(float *)((int)puVar5 + -0x24) = fVar13;
    *(undefined4 *)(puVar5 + -5) = uVar15;
    *(undefined4 *)((int)puVar5 + -0x2c) = uVar10;
    *(undefined4 *)(puVar5 + -6) = uVar12;
    *(undefined4 *)((int)puVar5 + -0x34) = uVar17;
    pdVar6 = (double *)(puVar5 + -7);
    *(float *)pdVar6 = fVar9;
  }
  else {
    puVar5[-1] = in_xd4;
    puVar5[-2] = uVar14;
    puVar5[-3] = in_xd2;
    puVar5[-4] = uVar11;
    puVar5[-5] = in_xd0;
    puVar5[-6] = uVar7;
    dVar8 = *(double *)(puVar4 + -0x40);
    uVar16 = *(undefined8 *)(puVar4 + -0x38);
    uVar14 = *(undefined8 *)(puVar4 + -0x30);
    uVar19 = *(undefined8 *)(puVar4 + -0x28);
    uVar11 = *(undefined8 *)(puVar4 + -0x20);
    in_xd4 = *(undefined8 *)(puVar4 + -0x18);
    uVar7 = *(undefined8 *)(puVar4 + -0x10);
    puVar5[-7] = *(undefined8 *)(puVar4 + -8);
    puVar5[-8] = uVar7;
    fVar13 = (float)uVar11;
    puVar5[-9] = in_xd4;
    puVar5[-10] = uVar11;
    puVar5[-0xb] = uVar19;
    puVar5[-0xc] = uVar14;
    uVar17 = SUB84(dVar8,0);
    puVar5[-0xd] = uVar16;
    fVar9 = (float)((ulonglong)dVar8 >> 0x20);
    pdVar6 = (double *)(puVar5 + -0xe);
    *pdVar6 = dVar8;
  }
  if (in_FPSCR_PR == '\0') {
    dVar8 = (double)CONCAT44(-fVar9,uVar17);
  }
  else {
    dVar8 = -dVar8;
  }
  if (in_FPSCR_SZ == '\0') {
    *(float *)pdVar6 = (float)((ulonglong)dVar8 >> 0x20);
    *(float *)((int)pdVar6 + 0x14) = -fVar13;
    dVar8 = (double)CONCAT44(*(float *)pdVar6,*(float *)((int)pdVar6 + 4));
  }
  else {
    *pdVar6 = dVar8;
    *(undefined8 *)((int)pdVar6 + 0x14) = in_xd4;
    dVar8 = *pdVar6;
  }
  return dVar8;
}

