/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c0373fa) */
/* WARNING: Removing unreachable block (ram,0x8c037450) */
/* WARNING: Removing unreachable block (ram,0x8c037410) */
/* WARNING: Restarted to delay deadcode elimination for space: register */

undefined8
FUN_8c037350(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,int param_5
            )

{
  int iVar1;
  undefined4 *puVar2;
  undefined8 *puVar3;
  float *pfVar4;
  int iVar5;
  undefined4 *puVar6;
  undefined4 *puVar7;
  int iVar8;
  int iVar9;
  undefined4 in_fr1;
  undefined4 in_fr3;
  undefined8 uVar10;
  float fVar12;
  undefined4 uVar14;
  double dVar13;
  float fVar15;
  undefined8 uVar16;
  float fVar17;
  float fVar18;
  undefined8 in_xd0;
  undefined8 in_xd2;
  undefined8 in_xd4;
  double in_xd6;
  undefined8 in_xd8;
  undefined8 in_xd10;
  int in_GBR;
  int iVar19;
  int iVar20;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  float fVar11;
  
  puVar7 = (undefined4 *)PTR_DAT_8c0373b0;
  puVar3 = *(undefined8 **)(param_5 + 4);
  iVar5 = *(int *)(param_5 + 8);
  puVar6 = (undefined4 *)(*(int *)(in_GBR + 0x3c) + 0xc);
  *(undefined **)(in_GBR + 0x40) = PTR_DAT_8c0373b0;
  if (in_FPSCR_SZ == '\0') {
    uVar16 = CONCAT44(*(undefined4 *)PTR_DAT_8c0373b4,*(undefined4 *)(PTR_DAT_8c0373b4 + 4));
    fVar18 = *(float *)(PTR_DAT_8c0373b4 + 8);
    in_xd0 = CONCAT44(in_fr1,in_fr1);
    uVar10 = CONCAT44(in_fr3,in_fr3);
  }
  else {
    uVar16 = *(undefined8 *)PTR_DAT_8c0373b4;
    fVar18 = (float)((ulonglong)*(undefined8 *)(PTR_DAT_8c0373b4 + 0x10) >> 0x20);
    uVar10 = in_xd2;
  }
                    /* WARNING: Ignoring partial resolution of indirect */
  fVar15 = (float)uVar16;
  fVar17 = (float)((ulonglong)uVar16 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    fVar11 = (float)in_xd0 * fVar15 + (float)((ulonglong)in_xd0 >> 0x20) * fVar17 +
             (float)uVar10 * 0.0 + (float)((ulonglong)uVar10 >> 0x20) * fVar18;
  }
  if (in_FPSCR_SZ == '\0') {
    in_xd4 = CONCAT44(param_1,param_1);
    in_xd6 = (double)CONCAT44(param_2,param_2);
  }
                    /* WARNING: Ignoring partial resolution of indirect */
  if (in_FPSCR_PR == '\0') {
    fVar11 = (float)in_xd4 * fVar15 + (float)((ulonglong)in_xd4 >> 0x20) * fVar17 +
             SUB84(in_xd6,0) * 0.0 + (float)((ulonglong)in_xd6 >> 0x20) * fVar18;
  }
  uVar16 = in_xd10;
  if (in_FPSCR_SZ == '\0') {
    in_xd8 = CONCAT44(param_3,param_3);
    uVar16 = CONCAT44(param_4,param_4);
  }
                    /* WARNING: Ignoring partial resolution of indirect */
  if (in_FPSCR_PR == '\0') {
    fVar18 = (float)in_xd8 * fVar15 + (float)((ulonglong)in_xd8 >> 0x20) * fVar17 +
             (float)uVar16 * 0.0 + (float)((ulonglong)uVar16 >> 0x20) * fVar18;
  }
  if (in_FPSCR_SZ == '\0') {
    in_xd2 = CONCAT44((int)uVar10,SUB84(in_xd6,0));
    fVar18 = (float)uVar16;
    in_xd6 = (double)CONCAT44((int)((ulonglong)in_xd6 >> 0x20),*(undefined4 *)PTR_DAT_8c0373b8);
  }
  else {
    fVar18 = (float)((ulonglong)in_xd10 >> 0x20);
  }
  iVar9 = (int)DAT_8c0373ac;
  if (((DAT_8c0373bc & *(uint *)(in_GBR + 0x2c)) == 0) &&
     (*(int *)(*(int *)(in_GBR + 0x34) + 4) != 0)) {
    iVar1 = *(int *)(in_GBR + 0x58);
    iVar8 = (int)DAT_8c0373ae;
  }
  else {
    iVar1 = *(int *)(in_GBR + 0x58);
    iVar8 = 0;
  }
  iVar5 = iVar5 + -1;
  if (in_FPSCR_SZ == '\0') {
    uVar16 = CONCAT44(*(undefined4 *)puVar3,*(undefined4 *)((int)puVar3 + 4));
    uVar14 = *(undefined4 *)(puVar3 + 1);
    pfVar4 = (float *)((int)puVar3 + 0xc);
  }
  else {
    uVar16 = *puVar3;
    uVar14 = (undefined4)((ulonglong)puVar3[2] >> 0x20);
    pfVar4 = (float *)(puVar3 + 3);
  }
  dVar13 = (double)CONCAT44(uVar14,0x3f800000);
  do {
                    /* WARNING: Ignoring partial resolution of indirect */
    fVar15 = (float)in_xd2;
    fVar17 = (float)((ulonglong)in_xd2 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      fVar11 = (float)uVar16 * fVar15 + (float)((ulonglong)uVar16 >> 0x20) * fVar17 +
               SUB84(dVar13,0) * 0.0 + (float)((ulonglong)dVar13 >> 0x20) * fVar18;
    }
    if (in_FPSCR_SZ == '\0') {
      in_xd0 = CONCAT44(*pfVar4,(int)in_xd0);
      pfVar4 = pfVar4 + 1;
    }
    else {
      in_xd0 = *(undefined8 *)pfVar4;
      pfVar4 = pfVar4 + 2;
    }
    fVar11 = SUB84(in_xd6,0);
    if (in_FPSCR_PR == '\0') {
      dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),SUB84(dVar13,0) * fVar11);
    }
    else {
      dVar13 = dVar13 * in_xd6;
    }
    if (in_FPSCR_SZ == '\0') {
      in_xd0 = CONCAT44((int)((ulonglong)in_xd0 >> 0x20),*pfVar4);
      pfVar4 = pfVar4 + 1;
    }
    else {
      pfVar4 = pfVar4 + 2;
    }
    if (in_FPSCR_PR == '\0') {
      dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),SUB84(dVar13,0) + fVar11);
    }
    else {
      dVar13 = dVar13 + in_xd6;
    }
    if (in_FPSCR_SZ == '\0') {
      fVar12 = *pfVar4;
      pfVar4 = pfVar4 + 1;
    }
    else {
      fVar12 = (float)((ulonglong)*(undefined8 *)pfVar4 >> 0x20);
      pfVar4 = pfVar4 + 2;
    }
    if (in_FPSCR_PR == '\0') {
      iVar19 = (int)SUB84(dVar13,0);
    }
    else {
      iVar19 = (int)dVar13;
    }
    dVar13 = (double)CONCAT44(fVar12,0x3f800000);
                    /* WARNING: Ignoring partial resolution of indirect */
    if (in_FPSCR_PR == '\0') {
      fVar12 = (float)in_xd0 * fVar15 + (float)((ulonglong)in_xd0 >> 0x20) * fVar17 + 0.0 +
               fVar12 * fVar18;
    }
    if (iVar19 < 0) {
      iVar19 = 0;
    }
    if (in_FPSCR_PR == '\0') {
      dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),SUB84(dVar13,0) * fVar11);
    }
    else {
      dVar13 = dVar13 * in_xd6;
    }
    if (in_FPSCR_PR == '\0') {
      dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),SUB84(dVar13,0) + fVar11);
    }
    else {
      dVar13 = dVar13 + in_xd6;
    }
    if (iVar9 < iVar19) {
      iVar19 = iVar9;
    }
    if (in_FPSCR_PR == '\0') {
      iVar20 = (int)SUB84(dVar13,0);
    }
    else {
      iVar20 = (int)dVar13;
    }
    puVar2 = (undefined4 *)(iVar19 * 8 + iVar1);
    if (iVar20 < 0) {
      iVar20 = 0;
    }
    uVar14 = *(undefined4 *)((int)puVar2 + iVar8 + 4);
    if (iVar9 < iVar20) {
      iVar20 = iVar9;
    }
    *puVar6 = *puVar2;
    *puVar7 = uVar14;
    puVar2 = (undefined4 *)(iVar20 * 8 + iVar1);
    uVar14 = *(undefined4 *)((int)puVar2 + iVar8 + 4);
    if (in_FPSCR_SZ == '\0') {
      uVar16 = CONCAT44(*pfVar4,(float)uVar16);
      pfVar4 = pfVar4 + 1;
    }
    else {
      uVar16 = *(undefined8 *)pfVar4;
      pfVar4 = pfVar4 + 2;
    }
    puVar6[4] = *puVar2;
    if (in_FPSCR_SZ == '\0') {
      uVar16 = CONCAT44((int)((ulonglong)uVar16 >> 0x20),*pfVar4);
      pfVar4 = pfVar4 + 1;
    }
    else {
      pfVar4 = pfVar4 + 2;
    }
    puVar7[1] = uVar14;
    if (in_FPSCR_SZ == '\0') {
      fVar12 = *pfVar4;
      pfVar4 = pfVar4 + 1;
    }
    else {
      fVar12 = (float)((ulonglong)*(undefined8 *)pfVar4 >> 0x20);
      pfVar4 = pfVar4 + 2;
    }
    puVar6 = puVar6 + 8;
    dVar13 = (double)CONCAT44(fVar12,0x3f800000);
    iVar5 = iVar5 + -2;
    puVar7 = puVar7 + 2;
  } while (0 < iVar5);
  if (iVar5 == 0) {
                    /* WARNING: Ignoring partial resolution of indirect */
    if (in_FPSCR_PR == '\0') {
      fVar18 = (float)uVar16 * fVar15 + (float)((ulonglong)uVar16 >> 0x20) * fVar17 + 0.0 +
               fVar12 * fVar18;
      iVar5 = (int)(SUB84(dVar13,0) * fVar11 + fVar11);
    }
    else {
      iVar5 = (int)(dVar13 * in_xd6 + in_xd6);
    }
    if (iVar5 < 0) {
      iVar5 = 0;
    }
    if (iVar9 < iVar5) {
      iVar5 = iVar9;
    }
    puVar2 = (undefined4 *)(iVar5 * 8 + iVar1);
    uVar14 = *(undefined4 *)((int)puVar2 + iVar8 + 4);
    *puVar6 = *puVar2;
    *puVar7 = uVar14;
  }
  return in_xd0;
}

