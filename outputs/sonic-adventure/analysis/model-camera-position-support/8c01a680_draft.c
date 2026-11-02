/* Ghidra draft only; not buildable/verified source. */

void FUN_8c01a680(int param_1)

{
  undefined *puVar1;
  undefined *puVar2;
  int iVar3;
  undefined1 *puVar4;
  double dVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  double in_dr2;
  double dVar8;
  undefined8 uVar9;
  double dVar10;
  float in_fr13;
  undefined4 in_fr12;
  float fVar11;
  double in_dr14;
  double dVar12;
  double in_xd2;
  undefined8 in_xd14;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_20 [12];
  undefined1 local_14 [4];
  undefined8 uStack_10;
  
  puVar1 = PTR_DAT_8c01a8a0;
  uVar6 = (undefined4)((ulonglong)in_dr14 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    uStack_10 = CONCAT44(SUB84(in_dr14,0),(undefined4)uStack_10);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_10._0_4_ = uVar6;
    puVar4 = local_14;
  }
  else {
    puVar4 = auStack_20;
    uStack_10 = in_xd14;
  }
  *(undefined4 *)(puVar4 + -4) = in_PR;
  if (in_FPSCR_SZ == '\0') {
    uVar7 = *(undefined4 *)(*(int *)PTR_DAT_8c01a8a0 + 0xc);
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),uVar7);
    *(undefined4 *)(param_1 + 0x20) = uVar7;
  }
  else {
    in_xd2 = *(double *)(*(int *)PTR_DAT_8c01a8a0 + 0xc);
    *(double *)(param_1 + 0x20) = in_xd2;
  }
  if (in_FPSCR_SZ == '\0') {
    uVar7 = *(undefined4 *)(*(int *)puVar1 + 0x10);
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),uVar7);
    *(undefined4 *)(param_1 + 0x24) = uVar7;
  }
  else {
    in_xd2 = *(double *)(*(int *)puVar1 + 0x10);
    *(double *)(param_1 + 0x24) = in_xd2;
  }
  if (in_FPSCR_SZ == '\0') {
    uVar7 = *(undefined4 *)(*(int *)puVar1 + 0x14);
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),uVar7);
    *(undefined4 *)(param_1 + 0x28) = uVar7;
  }
  else {
    in_xd2 = *(double *)(*(int *)puVar1 + 0x14);
    *(double *)(param_1 + 0x28) = in_xd2;
  }
  iVar3 = *(int *)puVar1;
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(iVar3 + 0x18));
    in_dr14 = (double)CONCAT44(uVar6,*(undefined4 *)(iVar3 + 0xc));
  }
  else {
    in_xd2 = *(double *)(iVar3 + 0x18);
    in_xd14 = *(undefined8 *)(iVar3 + 0xc);
  }
  if (in_FPSCR_PR == '\0') {
    fVar11 = SUB84(in_dr14,0) - SUB84(in_dr2,0);
  }
  else {
    fVar11 = SUB84(in_dr14 - in_dr2,0);
  }
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(iVar3 + 0x20));
    dVar12 = (double)CONCAT44(*(undefined4 *)(iVar3 + 0x14),fVar11);
  }
  else {
    in_xd2 = *(double *)(iVar3 + 0x20);
    dVar12 = *(double *)(iVar3 + 0x14);
  }
  if (in_FPSCR_PR == '\0') {
    dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) - SUB84(in_dr2,0),SUB84(dVar12,0));
  }
  else {
    dVar12 = dVar12 - in_dr2;
  }
  uVar9 = in_xd14;
  if (in_FPSCR_SZ == '\0') {
    uVar9 = CONCAT44(SUB84(dVar12,0),(int)((ulonglong)dVar12 >> 0x20));
    in_fr13 = DAT_8c01a8a4;
  }
  dVar5 = (double)(*(code *)PTR_FUN_8c01a8a8)(uVar9);
  puVar2 = PTR_FUN_8c01a8ac;
  if (in_FPSCR_PR == '\0') {
    dVar5 = (double)CONCAT44((float)((ulonglong)dVar5 >> 0x20) * in_fr13,SUB84(dVar5,0));
  }
  else {
    dVar5 = dVar5 * (double)CONCAT44(in_fr12,in_fr13);
  }
  fVar11 = (float)((ulonglong)dVar12 >> 0x20);
  dVar10 = dVar12;
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),fVar11);
    dVar10 = in_xd2;
  }
  if (in_FPSCR_PR == '\0') {
    dVar8 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),SUB84(in_dr2,0) * fVar11);
    iVar3 = (int)(float)((ulonglong)dVar5 >> 0x20);
  }
  else {
    dVar8 = in_dr2 * dVar12;
    iVar3 = (int)dVar5;
  }
  fVar11 = SUB84(dVar12,0);
  if (in_FPSCR_SZ != '\0') {
    fVar11 = (float)((ulonglong)in_xd14 >> 0x20);
  }
  if (in_FPSCR_PR == '\0') {
    dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),
                             fVar11 * SUB84(dVar12,0) + SUB84(dVar8,0));
  }
  *(int *)(param_1 + 0x18) = iVar3;
  if (in_FPSCR_SZ == '\0') {
    dVar10 = (double)CONCAT44(SUB84(dVar8,0),(int)uVar9);
  }
  uVar6 = (*(code *)puVar2)(dVar10);
  iVar3 = *(int *)puVar1;
  if (in_FPSCR_SZ == '\0') {
    dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),*(undefined4 *)(iVar3 + 0x10));
    dVar12 = (double)CONCAT44(*(undefined4 *)(iVar3 + 0x1c),uVar6);
  }
  else {
    dVar12 = *(double *)(iVar3 + 0x1c);
  }
  if (in_FPSCR_PR == '\0') {
    dVar12 = (double)CONCAT44((float)((ulonglong)dVar12 >> 0x20) - SUB84(dVar8,0),SUB84(dVar12,0));
  }
  else {
    dVar12 = dVar12 - dVar8;
  }
  dVar12 = (double)(*(code *)PTR_FUN_8c01a8a8)(dVar12);
  if (in_FPSCR_PR == '\0') {
    iVar3 = (int)((float)((ulonglong)dVar12 >> 0x20) * in_fr13);
  }
  else {
    iVar3 = (int)(dVar12 * (double)CONCAT44(in_fr12,in_fr13));
  }
  *(int *)(param_1 + 0x14) = iVar3;
  *(undefined4 *)(param_1 + 0x1c) = *(undefined4 *)(*(int *)puVar1 + 0x2c);
  return;
}

