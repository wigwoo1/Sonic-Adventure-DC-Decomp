/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c60bff6) */
/* WARNING: Removing unreachable block (ram,0x8c60bfb2) */
/* WARNING: Removing unreachable block (ram,0x8c60bf94) */
/* WARNING: Removing unreachable block (ram,0x8c60bfdc) */
/* WARNING: Removing unreachable block (ram,0x8c60c00c) */

undefined8 callback_8c60bf5e(undefined4 param_1,int param_2,int param_3,int *param_4)

{
  int iVar1;
  int *piVar2;
  int *piVar3;
  double extraout_dr0;
  float fVar4;
  double dVar5;
  double dVar6;
  undefined8 uVar7;
  float fVar9;
  double in_xd0;
  undefined8 in_xd2;
  undefined8 uVar10;
  undefined8 in_xd4;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined4 uStack_14;
  undefined8 uStack_10;
  uint uVar8;
  
  uStack_10._4_4_ = in_PR;
  (*(code *)PTR_FUN_8c60c178)();
  piVar3 = (int *)(param_3 * 0x10 + param_2);
  iVar1 = (*(code *)PTR_FUN_8c60c17c)();
  piVar2 = (int *)(param_2 + iVar1 * 0x10);
  fVar4 = (float)(*piVar2 - *piVar3);
  dVar5 = (double)(ulonglong)(uint)fVar4;
  uVar7 = in_xd2;
  if (in_FPSCR_SZ == '\0') {
    uVar7 = CONCAT44(fVar4,param_1);
  }
  uVar8 = (uint)((ulonglong)uVar7 >> 0x20);
  dVar6 = extraout_dr0;
  uVar10 = uVar7;
  if (in_FPSCR_PR != '\0' || 0.0 <= fVar4) {
    if (in_FPSCR_SZ == '\0') {
      dVar5 = (double)(ulonglong)uVar8;
      uVar10 = in_xd2;
    }
  }
  else {
    if (in_FPSCR_SZ == '\0') {
      dVar5 = (double)(ulonglong)uVar8;
      dVar6 = (double)CONCAT44((int)((ulonglong)extraout_dr0 >> 0x20),DAT_8c60c180);
      uVar10 = in_xd2;
    }
    else {
      in_xd0 = (double)CONCAT44(DAT_8c60c184,DAT_8c60c180);
    }
    if (in_FPSCR_PR == '\0') {
      dVar5 = (double)(ulonglong)(uint)(SUB84(dVar5,0) + SUB84(dVar6,0));
    }
    else {
      dVar5 = dVar5 + dVar6;
    }
  }
  if (in_FPSCR_SZ == '\0') {
    uStack_10 = CONCAT44(uStack_10._4_4_,SUB84(dVar5,0));
  }
  else {
    uStack_10 = uVar10;
  }
  iVar1 = *piVar3;
  fVar4 = (float)iVar1;
  dVar5 = (double)CONCAT44((int)((ulonglong)dVar5 >> 0x20),fVar4);
  if (iVar1 < 0) {
    if (in_FPSCR_PR == '\0') {
      dVar5 = (double)CONCAT44(DAT_8c60c180,fVar4 + DAT_8c60c180);
    }
    else {
      dVar5 = (double)CONCAT44(DAT_8c60c180,fVar4) + (double)CONCAT44(DAT_8c60c180,fVar4);
    }
  }
  if (in_FPSCR_SZ == '\0') {
    dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),*(undefined4 *)DAT_8c60c184);
  }
  else {
    in_xd0 = *DAT_8c60c184;
  }
  if (in_FPSCR_PR == '\0') {
    fVar4 = SUB84(dVar6,0) - SUB84(dVar5,0);
  }
  else {
    fVar4 = SUB84(dVar6 - dVar5,0);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar5 = (double)CONCAT44((int)((ulonglong)dVar5 >> 0x20),(&uStack_14)[0 < *piVar2 - iVar1]);
    in_xd0 = (double)CONCAT44(fVar4,(int)uVar7);
  }
  if (in_FPSCR_PR == '\0') {
    fVar9 = (float)((ulonglong)in_xd0 >> 0x20) / SUB84(dVar5,0);
  }
  else {
    fVar9 = (float)((ulonglong)(in_xd0 / dVar5) >> 0x20);
  }
  if (in_FPSCR_PR == '\0') {
    dVar5 = (double)CONCAT44(fVar9,1.0 - fVar9);
    dVar6 = (double)CONCAT44((float)piVar2[1] * fVar9,(float)piVar3[1]);
  }
  else {
    dVar5 = (double)CONCAT44(fVar9,0x3f800000) - (double)CONCAT44(fVar9,0x3f800000);
    fVar9 = (float)((ulonglong)dVar5 >> 0x20);
    dVar6 = (double)piVar2[1] * dVar5;
  }
  if (in_FPSCR_SZ == '\0') {
    in_xd4 = CONCAT44(SUB84(dVar5,0),fVar4);
  }
  fVar4 = (float)((ulonglong)in_xd4 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    iVar1 = (int)(fVar4 * SUB84(dVar6,0) + (float)((ulonglong)dVar6 >> 0x20));
  }
  else {
    iVar1 = (int)dVar6;
  }
  *param_4 = iVar1;
  if (in_FPSCR_PR == '\0') {
    iVar1 = (int)(fVar4 * (float)piVar3[2] + (float)piVar2[2] * fVar9);
  }
  else {
    iVar1 = (int)((double)piVar2[2] * dVar5);
  }
  param_4[1] = iVar1;
  if (in_FPSCR_PR == '\0') {
    iVar1 = (int)(fVar4 * (float)piVar3[3] + (float)piVar2[3] * fVar9);
  }
  else {
    iVar1 = (int)((double)piVar2[3] * dVar5);
  }
  param_4[2] = iVar1;
  return in_xd4;
}

