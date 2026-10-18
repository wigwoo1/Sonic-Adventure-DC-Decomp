/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void callback_8c60c3b8(undefined4 param_1,undefined4 param_2,int param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  undefined8 *puVar7;
  double in_dr2;
  float fVar8;
  double dVar9;
  undefined8 uVar10;
  undefined8 uVar11;
  bool bVar12;
  uint uVar13;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined8 auStack_18 [2];
  
  iVar1 = _DAT_8c60c470;
  iVar3 = *(int *)(_DAT_8c60c470 + 0x14) * 4;
  iVar2 = *(int *)(*(int *)(_DAT_8c60c470 + 4) + iVar3);
  if (iVar2 == 0) {
    puVar7 = (undefined8 *)(param_3 + 0x20);
  }
  else {
    uVar5 = *(uint *)(*(int *)(_DAT_8c60c470 + 8) + iVar3);
    if (in_FPSCR_SZ == '\0') {
      dVar9 = (double)CONCAT44(*(undefined4 *)(_DAT_8c60c470 + 0xc),param_1);
      in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),DAT_8c60c474);
    }
    else {
      dVar9 = *(double *)(_DAT_8c60c470 + 0xc);
    }
    uVar4 = 0;
    fVar8 = (float)((ulonglong)dVar9 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      bVar12 = SUB84(in_dr2,0) < fVar8;
    }
    else {
      bVar12 = in_dr2 < dVar9;
    }
    if (bVar12) {
      if (in_FPSCR_SZ == '\0') {
        dVar9 = (double)CONCAT44(DAT_8c60c478,fVar8);
      }
      else {
        dVar9 = (double)CONCAT44(_DAT_8c60c47c,DAT_8c60c478);
      }
      if (in_FPSCR_PR == '\0') {
        fVar8 = (float)((ulonglong)dVar9 >> 0x20);
        in_dr2 = (double)CONCAT44(fVar8,SUB84(dVar9,0) + fVar8);
      }
      else {
        in_dr2 = dVar9 + dVar9;
      }
    }
    else if (in_FPSCR_SZ == '\0') {
      in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),fVar8);
    }
    if (in_FPSCR_PR == '\0') {
      uVar13 = (uint)SUB84(in_dr2,0);
    }
    else {
      uVar13 = (uint)in_dr2;
    }
    while (uVar6 = uVar5, 1 < uVar6 - uVar4) {
      uVar5 = uVar4 + uVar6 >> 1;
      if (*(uint *)(uVar5 * 0x10 + iVar2) <= uVar13) {
        uVar4 = uVar5;
        uVar5 = uVar6;
      }
    }
    puVar7 = auStack_18;
    (*(code *)*_DAT_8c60c47c)(puVar7);
  }
  if (in_FPSCR_SZ == '\0') {
    uVar11 = CONCAT44(*(undefined4 *)(puVar7 + 1),param_2);
    uVar10 = CONCAT44(*(undefined4 *)puVar7,*(undefined4 *)((int)puVar7 + 4));
  }
  else {
    uVar11 = puVar7[1];
    uVar10 = *puVar7;
  }
  (*(code *)PTR_FUN_8c60c484)(uVar10,uVar11,*(undefined4 *)(iVar1 + 0x18));
  *(int *)(iVar1 + 0x14) = *(int *)(iVar1 + 0x14) + 1;
  return;
}

