/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c01226e) */
/* WARNING: Removing unreachable block (ram,0x8c012352) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c012240(uint param_1,undefined4 param_2)

{
  double *pdVar1;
  int *piVar2;
  int *piVar3;
  undefined *puVar4;
  undefined8 *puVar5;
  undefined8 *puVar6;
  ulonglong *puVar7;
  uint uVar8;
  int iVar9;
  undefined4 *puVar10;
  undefined4 *puVar11;
  uint uVar12;
  int iVar13;
  int iVar14;
  int iVar15;
  undefined8 uVar16;
  undefined4 in_fr3;
  double dVar17;
  float fVar18;
  float fVar19;
  undefined8 uVar20;
  undefined4 uVar21;
  double dVar22;
  double in_xd2;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined4 local_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined4 local_1c;
  
  puVar4 = PTR_FUN_8c012464;
  *DAT_8c012460 = 0;
  local_1c = in_PR;
  uVar8 = (*(code *)puVar4)();
  iVar15 = -1;
  iVar14 = -1;
  uVar12 = uVar8 & 0xff;
  iVar13 = (int)(uVar8 & 0xffff & DAT_8c012468) >> 8;
  (*(code *)PTR_FUN_8c01246c)(0);
  iVar9 = (*(code *)PTR_thunk_FUN_8c64826e_8c012474)(*(undefined4 *)PTR_DAT_8c012470);
  if (iVar9 != 0) {
    iVar15 = FUN_8c012014(iVar13,uVar12);
    iVar14 = FUN_8c011d46(iVar13,uVar12);
  }
  if ((iVar15 != 0) || (iVar14 != 0)) {
    puVar11 = DAT_8c012478 + 0x168;
    pdVar1 = _DAT_8c012484;
    for (puVar10 = DAT_8c012478; _DAT_8c012484 = pdVar1, puVar10 < puVar11; puVar10 = puVar10 + 0x24
        ) {
      *puVar10 = 0;
      puVar10[0xc] = 0;
      puVar10[0x18] = 0;
      pdVar1 = _DAT_8c012484;
    }
    uVar20 = CONCAT44(0x3f800000,param_1);
    *DAT_8c01247c = 0;
    puVar5 = DAT_8c01248c;
    uVar21 = DAT_8c012480;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)pdVar1 = DAT_8c012480;
      puVar6 = DAT_8c012490;
      *(undefined4 *)DAT_8c012488 = 0x3f800000;
      *(undefined4 *)puVar5 = 0x3f800000;
      *(undefined4 *)puVar6 = 0x3f800000;
      piVar3 = DAT_8c01249c;
      piVar2 = DAT_8c012498;
      *(undefined4 *)DAT_8c012494 = 0x3f800000;
      in_fr3 = uVar21;
    }
    else {
      in_xd2 = (double)CONCAT44(_DAT_8c012484,DAT_8c012480);
      *pdVar1 = in_xd2;
      puVar6 = DAT_8c012490;
      *DAT_8c012488 = uVar20;
      *puVar5 = uVar20;
      *puVar6 = uVar20;
      piVar3 = DAT_8c01249c;
      piVar2 = DAT_8c012498;
      *DAT_8c012494 = uVar20;
    }
    *piVar2 = 0;
    *piVar3 = 0;
    puVar6 = DAT_8c0124a8;
    puVar5 = DAT_8c0124a4;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)DAT_8c0124a0 = 0x3f800000;
      *(undefined4 *)puVar5 = 0x3f800000;
      *(undefined4 *)puVar6 = 0x3f800000;
      puVar5 = DAT_8c0124b4;
      *(undefined4 *)DAT_8c0124b0 = DAT_8c0124ac;
      *(undefined4 *)puVar5 = 0x3f800000;
      in_fr3 = DAT_8c0124b8;
      uVar21 = DAT_8c0124c0;
    }
    else {
      *DAT_8c0124a0 = uVar20;
      *puVar5 = uVar20;
      *puVar6 = uVar20;
      puVar5 = DAT_8c0124b4;
      *DAT_8c0124b0 = CONCAT44(DAT_8c0124b0,DAT_8c0124ac);
      *puVar5 = uVar20;
      in_xd2 = (double)CONCAT44(DAT_8c0124bc,DAT_8c0124b8);
      uVar21 = DAT_8c0124c0;
    }
    iVar9 = 0;
    DAT_8c0124c0 = uVar21;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)DAT_8c0124bc = in_fr3;
    }
    else {
      *DAT_8c0124bc = in_xd2;
    }
    do {
      puVar4 = PTR_DAT_8c0124c4;
      uVar8 = 0;
      iVar15 = 4;
      do {
        *(undefined4 *)(puVar4 + uVar8 + iVar9 * 0x800) = 0xffffffff;
        *(undefined4 *)(puVar4 + iVar15 + iVar9 * 0x800) = uVar21;
        *(undefined4 *)(puVar4 + uVar8 + 8 + iVar9 * 0x800) = 0xffffffff;
        uVar8 = uVar8 + 0x10;
        *(undefined4 *)(puVar4 + iVar15 + 8 + iVar9 * 0x800) = uVar21;
        iVar15 = iVar15 + 0x10;
      } while (uVar8 < 0x800);
      iVar9 = iVar9 + 1;
    } while (iVar9 < 6);
    (*(code *)PTR_FUN_8c0124c8)(iVar13,uVar12);
    param_1 = (uint)uVar20;
    (*(code *)PTR_FUN_8c0124cc)(iVar13,uVar12);
  }
  puVar4 = PTR_FUN_8c0124d4;
  puVar7 = DAT_8c0124d0;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)DAT_8c0124d0 = 0;
    *(undefined4 *)((int)puVar7 + 4) = 0xbf800000;
    *(undefined4 *)(puVar7 + 1) = 0;
  }
  else {
    *DAT_8c0124d0 = (ulonglong)param_1;
    *(double *)((int)puVar7 + 4) = in_xd2;
    puVar7[1] = (ulonglong)param_1;
  }
  (*(code *)puVar4)(0);
  (*(code *)PTR_FUN_8c0124d8)(0);
  if (*DAT_8c012498 != 0) {
    (*(code *)PTR_FUN_8c0124dc)(0,*DAT_8c012498);
  }
  if (*DAT_8c01249c != 0) {
    (*(code *)PTR_FUN_8c0124e0)(0,*DAT_8c01249c);
  }
  (*(code *)PTR_FUN_8c0124e8)(0,DAT_8c0124d0,DAT_8c0124e4);
  (*(code *)PTR_FUN_8c0124ec)(1);
  (*(code *)PTR_FUN_8c0124d4)(0);
  (*(code *)PTR_FUN_8c0124e8)(0,DAT_8c0124e4,&local_28);
  (*(code *)PTR_FUN_8c0124ec)(1);
  if (in_FPSCR_SZ == '\0') {
    uVar20 = CONCAT44(uStack_20,param_2);
    uVar16 = CONCAT44(local_28,uStack_24);
  }
  else {
    uVar20 = CONCAT44(local_1c,uStack_20);
    uVar16 = CONCAT44(uStack_24,local_28);
  }
  (*(code *)PTR_FUN_8c0124f0)(uVar16,uVar20);
  if (in_FPSCR_SZ == '\0') {
    uVar20 = CONCAT44(uStack_20,(int)uVar20);
    dVar22 = (double)CONCAT44(local_28,uStack_24);
  }
  else {
    uVar20 = CONCAT44(local_1c,uStack_20);
    dVar22 = (double)CONCAT44(uStack_24,local_28);
  }
  uVar16 = (*(code *)PTR_FUN_8c0124f4)(dVar22,uVar20);
  fVar19 = (float)uVar16;
  uVar21 = (undefined4)uVar20;
  if (in_FPSCR_SZ == '\0') {
    dVar17 = (double)CONCAT44(*(undefined4 *)DAT_8c0124bc,DAT_8c0124f8);
    fVar19 = DAT_8c0124fc;
  }
  else {
    dVar17 = *DAT_8c0124bc;
  }
  if (in_FPSCR_PR == '\0') {
    fVar18 = (float)((ulonglong)dVar17 >> 0x20) + SUB84(dVar17,0);
  }
  else {
    fVar18 = (float)((ulonglong)(dVar17 + dVar17) >> 0x20);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),fVar18);
  }
  if (in_FPSCR_PR == '\0') {
    fVar19 = SUB84(dVar22,0) * fVar19;
  }
  else {
    fVar19 = SUB84(dVar22 * (double)CONCAT44((int)((ulonglong)uVar16 >> 0x20),fVar19),0);
  }
  if (in_FPSCR_SZ == '\0') {
    uVar20 = CONCAT44(*(undefined4 *)DAT_8c0124b4,fVar19);
  }
  else {
    uVar20 = *DAT_8c0124b4;
  }
  (*(code *)PTR_FUN_8c012500)(uVar20);
  if (in_FPSCR_SZ == '\0') {
    uVar20 = CONCAT44(*(undefined4 *)DAT_8c0124a0,uVar21);
    uVar16 = CONCAT44(*(undefined4 *)DAT_8c0124a8,*(undefined4 *)DAT_8c0124a4);
  }
  else {
    uVar20 = *DAT_8c0124a0;
    uVar16 = *DAT_8c0124a8;
  }
  (*(code *)PTR_FUN_8c012504)(uVar16,uVar20);
  uVar21 = (undefined4)uVar20;
  (*(code *)PTR_FUN_8c01250c)(DAT_8c012508,0x13);
  if (in_FPSCR_SZ == '\0') {
    dVar22 = (double)CONCAT44(*(undefined4 *)DAT_8c0124bc,uVar21);
    uVar20 = CONCAT44(*(undefined4 *)DAT_8c0124b0,*(undefined4 *)DAT_8c0124b4);
  }
  else {
    dVar22 = *DAT_8c0124bc;
    uVar20 = *DAT_8c0124b0;
  }
  (*(code *)PTR_FUN_8c012510)(uVar20,dVar22,DAT_8c012508);
  if (in_FPSCR_SZ == '\0') {
    uVar20 = CONCAT44(*(undefined4 *)DAT_8c0124a0,SUB84(dVar22,0));
    uVar16 = CONCAT44(*(undefined4 *)DAT_8c0124a8,*(undefined4 *)DAT_8c0124a4);
  }
  else {
    uVar20 = *DAT_8c0124a0;
    uVar16 = *DAT_8c0124a8;
  }
  (*(code *)PTR_FUN_8c012514)(uVar16,uVar20,DAT_8c012508);
  uVar21 = (undefined4)uVar20;
  (*(code *)PTR_FUN_8c012518)(DAT_8c012508,0xaaa,0x31c7);
  if (in_FPSCR_SZ == '\0') {
    uVar20 = CONCAT44(*(undefined4 *)(DAT_8c0124e4 + 1),uVar21);
    uVar16 = CONCAT44(*(undefined4 *)DAT_8c0124e4,*(undefined4 *)((int)DAT_8c0124e4 + 4));
  }
  else {
    uVar20 = DAT_8c0124e4[1];
    uVar16 = *DAT_8c0124e4;
  }
  (*(code *)PTR_FUN_8c01251c)(uVar16,uVar20,DAT_8c012508);
  return;
}

