/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void callback_8c60c526(undefined4 param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  double in_dr2;
  float fVar7;
  double dVar8;
  bool bVar9;
  uint uVar10;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined4 uStack_18;
  undefined4 uStack_14;
  undefined4 uStack_10;
  
  iVar1 = DAT_8c60c714;
  iVar3 = *(int *)(DAT_8c60c714 + 0x14) * 4;
  iVar2 = *(int *)(*(int *)(DAT_8c60c714 + 4) + iVar3);
  if (iVar2 == 0) {
    (*(code *)PTR_FUN_8c60c728)
              (*(undefined4 *)(DAT_8c60c714 + 0x18),*(undefined4 *)(param_2 + 0x14),
               *(undefined4 *)(param_2 + 0x18),*(undefined4 *)(param_2 + 0x1c));
  }
  else {
    uVar5 = *(uint *)(*(int *)(DAT_8c60c714 + 8) + iVar3);
    if (in_FPSCR_SZ == '\0') {
      dVar8 = (double)CONCAT44(*(undefined4 *)(DAT_8c60c714 + 0xc),param_1);
      in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),DAT_8c60c718);
    }
    else {
      dVar8 = *(double *)(DAT_8c60c714 + 0xc);
    }
    uVar4 = 0;
    fVar7 = (float)((ulonglong)dVar8 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      bVar9 = SUB84(in_dr2,0) < fVar7;
    }
    else {
      bVar9 = in_dr2 < dVar8;
    }
    if (bVar9) {
      if (in_FPSCR_SZ == '\0') {
        dVar8 = (double)CONCAT44(DAT_8c60c71c,fVar7);
      }
      else {
        dVar8 = (double)CONCAT44(_DAT_8c60c720,DAT_8c60c71c);
      }
      if (in_FPSCR_PR == '\0') {
        fVar7 = (float)((ulonglong)dVar8 >> 0x20);
        in_dr2 = (double)CONCAT44(fVar7,SUB84(dVar8,0) + fVar7);
      }
      else {
        in_dr2 = dVar8 + dVar8;
      }
    }
    else if (in_FPSCR_SZ == '\0') {
      in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),fVar7);
    }
    if (in_FPSCR_PR == '\0') {
      uVar10 = (uint)SUB84(in_dr2,0);
    }
    else {
      uVar10 = (uint)in_dr2;
    }
    while (uVar6 = uVar5, 1 < uVar6 - uVar4) {
      uVar5 = uVar4 + uVar6 >> 1;
      if (*(uint *)(uVar5 * 0x10 + iVar2) <= uVar10) {
        uVar4 = uVar5;
        uVar5 = uVar6;
      }
    }
    (*(code *)*_DAT_8c60c720)(&uStack_18);
    (*(code *)PTR_FUN_8c60c728)(*(undefined4 *)(iVar1 + 0x18),uStack_18,uStack_14,uStack_10);
  }
  *(int *)(iVar1 + 0x14) = *(int *)(iVar1 + 0x14) + 1;
  return;
}

