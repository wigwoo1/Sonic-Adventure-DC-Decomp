/* Ghidra draft only; not buildable/verified source. */

double FUN_8c01a718(int param_1)

{
  undefined *puVar1;
  int iVar2;
  undefined1 *puVar3;
  double dVar4;
  double dVar5;
  float fVar7;
  undefined8 uVar6;
  float extraout_fr3;
  float extraout_fr3_00;
  float fVar8;
  undefined8 in_dr12;
  double dVar9;
  undefined8 uVar10;
  float in_fr15;
  undefined8 in_xd12;
  double in_xd14;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_20 [8];
  undefined8 local_18;
  
  puVar1 = PTR_DAT_8c01a8a0;
  if (in_FPSCR_SZ == '\0') {
    puVar3 = (undefined1 *)((int)&local_18 + 4);
    local_18 = CONCAT44((int)((ulonglong)in_dr12 >> 0x20),(undefined4)local_18);
  }
  else {
    puVar3 = auStack_20;
    local_18 = in_xd12;
  }
  *(undefined4 *)(puVar3 + -4) = in_PR;
  iVar2 = *(int *)PTR_DAT_8c01a8a0;
  if (in_FPSCR_SZ == '\0') {
    dVar9 = (double)CONCAT44(*(undefined4 *)(iVar2 + 8),(int)in_dr12);
  }
  else {
    dVar9 = *(double *)(iVar2 + 8);
  }
  dVar4 = (double)(*(code *)PTR_FUN_8c01a8b0)(*(undefined4 *)(iVar2 + 0x24));
  if (in_FPSCR_PR == '\0') {
    dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) * (float)((ulonglong)dVar4 >> 0x20),
                             SUB84(dVar9,0));
  }
  else {
    dVar9 = dVar9 * dVar4;
  }
  iVar2 = *(int *)puVar1;
  if (in_FPSCR_SZ == '\0') {
    in_fr15 = (float)((ulonglong)dVar9 >> 0x20);
    dVar4 = (double)CONCAT44(*(undefined4 *)(iVar2 + 8),*(undefined4 *)(iVar2 + 0x1c));
    dVar9 = in_xd14;
  }
  else {
    in_xd12 = *(undefined8 *)(iVar2 + 0x1c);
    dVar4 = *(double *)(iVar2 + 8);
  }
  dVar5 = (double)(*(code *)PTR_FUN_8c01a8b4)(*(undefined4 *)(iVar2 + 0x24));
  if (in_FPSCR_PR == '\0') {
    fVar8 = SUB84(dVar4,0) - (float)((ulonglong)dVar4 >> 0x20) * (float)((ulonglong)dVar5 >> 0x20);
  }
  else {
    fVar8 = SUB84(dVar4 * dVar5 - dVar4 * dVar5,0);
  }
  if (in_FPSCR_SZ == '\0') {
    *(float *)(param_1 + 0x24) = fVar8;
  }
  else {
    *(undefined8 *)(param_1 + 0x24) = in_xd12;
  }
  iVar2 = *(int *)puVar1;
  if (in_FPSCR_SZ == '\0') {
    uVar10 = CONCAT44(*(undefined4 *)(iVar2 + 0x18),fVar8);
  }
  else {
    uVar10 = *(undefined8 *)(iVar2 + 0x18);
  }
  fVar8 = (float)(*(code *)PTR_FUN_8c01a8b4)(*(undefined4 *)(iVar2 + 0x28));
  fVar7 = in_fr15;
  if (in_FPSCR_SZ != '\0') {
    fVar7 = (float)((ulonglong)dVar9 >> 0x20);
    fVar8 = extraout_fr3;
  }
  if (in_FPSCR_PR == '\0') {
    uVar10 = CONCAT44(fVar7 * fVar8 + (float)((ulonglong)uVar10 >> 0x20),(int)uVar10);
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_1 + 0x20) = (int)((ulonglong)uVar10 >> 0x20);
  }
  else {
    *(undefined8 *)(param_1 + 0x20) = uVar10;
  }
  iVar2 = *(int *)puVar1;
  if (in_FPSCR_SZ == '\0') {
    uVar10 = CONCAT44(*(undefined4 *)(iVar2 + 0x20),(int)uVar10);
  }
  else {
    uVar10 = *(undefined8 *)(iVar2 + 0x20);
  }
  uVar6 = (*(code *)PTR_FUN_8c01a8b0)(*(undefined4 *)(iVar2 + 0x28));
  fVar8 = extraout_fr3_00;
  if (in_FPSCR_SZ == '\0') {
    fVar8 = (float)((ulonglong)uVar6 >> 0x20);
    dVar9 = (double)CONCAT44(in_fr15,(int)uVar6);
  }
  if (in_FPSCR_PR == '\0') {
    uVar10 = CONCAT44((float)((ulonglong)dVar9 >> 0x20) * fVar8 + (float)((ulonglong)uVar10 >> 0x20)
                      ,(int)uVar10);
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_1 + 0x28) = (int)((ulonglong)uVar10 >> 0x20);
  }
  else {
    *(undefined8 *)(param_1 + 0x28) = uVar10;
  }
  *(undefined4 *)(param_1 + 0x14) = *(undefined4 *)(*(int *)puVar1 + 0x24);
  *(undefined4 *)(param_1 + 0x18) = *(undefined4 *)(*(int *)puVar1 + 0x28);
  *(undefined4 *)(param_1 + 0x1c) = *(undefined4 *)(*(int *)puVar1 + 0x2c);
  return dVar9;
}

