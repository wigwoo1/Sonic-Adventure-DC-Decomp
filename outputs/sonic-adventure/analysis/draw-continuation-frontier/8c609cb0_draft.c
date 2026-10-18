/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c609ce6) */
/* WARNING: Removing unreachable block (ram,0x8c609d0e) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void callback_8c609cb0(undefined4 param_1,int param_2,int param_3,undefined8 *param_4)

{
  int iVar1;
  int *piVar2;
  int *piVar3;
  double extraout_dr0;
  double dVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  double dVar8;
  uint uVar9;
  double in_xd0;
  undefined8 in_xd2;
  undefined8 uVar10;
  undefined8 in_xd4;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_14 [8];
  undefined4 uStack_c;
  
  fVar7 = (float)((ulonglong)in_xd4 >> 0x20);
  uStack_c = in_PR;
  (*(code *)PTR_FUN_8c609eb0)();
  piVar3 = (int *)(param_3 * 0x10 + param_2);
  iVar1 = (*(code *)PTR_FUN_8c609eb4)();
  piVar2 = (int *)(param_2 + iVar1 * 0x10);
  fVar5 = (float)(*piVar2 - *piVar3);
  dVar8 = (double)(ulonglong)(uint)fVar5;
  uVar10 = in_xd2;
  if (in_FPSCR_SZ == '\0') {
    uVar10 = CONCAT44(fVar5,param_1);
  }
  uVar9 = (uint)((ulonglong)uVar10 >> 0x20);
  dVar4 = extraout_dr0;
  if (in_FPSCR_PR != '\0' || 0.0 <= fVar5) {
    if (in_FPSCR_SZ == '\0') {
      dVar8 = (double)(ulonglong)uVar9;
      uVar10 = in_xd2;
    }
  }
  else {
    if (in_FPSCR_SZ == '\0') {
      dVar8 = (double)(ulonglong)uVar9;
      dVar4 = (double)CONCAT44((int)((ulonglong)extraout_dr0 >> 0x20),DAT_8c609eb8);
      uVar10 = in_xd2;
    }
    else {
      in_xd0 = (double)CONCAT44(_DAT_8c609ebc,DAT_8c609eb8);
    }
    if (in_FPSCR_PR == '\0') {
      dVar8 = (double)(ulonglong)(uint)(SUB84(dVar8,0) + SUB84(dVar4,0));
    }
    else {
      dVar8 = dVar8 + dVar4;
    }
  }
  if (in_FPSCR_SZ == '\0') {
    stack0xfffffff0 = CONCAT44(uStack_c,SUB84(dVar8,0));
  }
  else {
    stack0xfffffff0 = uVar10;
  }
  iVar1 = *piVar3;
  fVar5 = (float)iVar1;
  dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),fVar5);
  if (iVar1 < 0) {
    if (in_FPSCR_PR == '\0') {
      dVar8 = (double)CONCAT44(DAT_8c609eb8,fVar5 + DAT_8c609eb8);
    }
    else {
      dVar8 = (double)CONCAT44(DAT_8c609eb8,fVar5) + (double)CONCAT44(DAT_8c609eb8,fVar5);
    }
  }
  if (in_FPSCR_SZ == '\0') {
    dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),
                             *(undefined4 *)(DAT_8c609ec0 + (uint)*_DAT_8c609ebc * 0x1c));
  }
  else {
    in_xd0 = *(double *)(DAT_8c609ec0 + (uint)*_DAT_8c609ebc * 0x1c);
  }
  if (in_FPSCR_PR == '\0') {
    fVar5 = SUB84(dVar4,0) - SUB84(dVar8,0);
  }
  else {
    fVar5 = SUB84(dVar4 - dVar8,0);
  }
  iVar1 = (uint)(0 < *piVar2 - iVar1) * 4;
  if (in_FPSCR_SZ == '\0') {
    in_xd0 = (double)CONCAT44(fVar5,0x3f800000);
    dVar8 = (double)CONCAT44(piVar3[1],*(undefined4 *)(auStack_14 + iVar1));
  }
  else {
    uVar10 = *(undefined8 *)(auStack_14 + iVar1);
    dVar8 = *(double *)(piVar3 + 1);
  }
  if (in_FPSCR_PR == '\0') {
    in_xd0 = (double)CONCAT44((float)((ulonglong)in_xd0 >> 0x20) / SUB84(dVar8,0),SUB84(in_xd0,0));
  }
  else {
    in_xd0 = in_xd0 / dVar8;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),piVar2[1]);
  }
  else {
    uVar10 = *(undefined8 *)(piVar2 + 1);
  }
  if (in_FPSCR_PR == '\0') {
    fVar5 = (float)((ulonglong)in_xd0 >> 0x20);
    in_xd0 = (double)CONCAT44(fVar5,SUB84(in_xd0,0) - fVar5);
    dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),SUB84(dVar8,0) * fVar5);
  }
  else {
    in_xd0 = in_xd0 - in_xd0;
    fVar5 = (float)((ulonglong)in_xd0 >> 0x20);
    dVar8 = dVar8 * in_xd0;
  }
  if (in_FPSCR_SZ == '\0') {
    fVar7 = SUB84(in_xd0,0);
  }
  if (in_FPSCR_PR == '\0') {
    dVar8 = (double)(ulonglong)(uint)(fVar7 * (float)((ulonglong)dVar8 >> 0x20) + SUB84(dVar8,0));
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)param_4 = SUB84(dVar8,0);
    dVar8 = (double)CONCAT44(piVar3[2],piVar2[2]);
  }
  else {
    *param_4 = uVar10;
    uVar10 = *(undefined8 *)(piVar2 + 2);
    dVar8 = *(double *)(piVar3 + 2);
  }
  if (in_FPSCR_PR == '\0') {
    fVar6 = fVar7 * (float)((ulonglong)dVar8 >> 0x20) + SUB84(dVar8,0) * fVar5;
  }
  else {
    fVar6 = SUB84(dVar8 * in_xd0,0);
  }
  if (in_FPSCR_SZ == '\0') {
    *(float *)((int)param_4 + 4) = fVar6;
    dVar8 = (double)CONCAT44(piVar3[3],piVar2[3]);
  }
  else {
    *(undefined8 *)((int)param_4 + 4) = uVar10;
    uVar10 = *(undefined8 *)(piVar2 + 3);
    dVar8 = *(double *)(piVar3 + 3);
  }
  if (in_FPSCR_PR == '\0') {
    fVar7 = fVar7 * (float)((ulonglong)dVar8 >> 0x20) + SUB84(dVar8,0) * fVar5;
  }
  else {
    fVar7 = SUB84(dVar8 * in_xd0,0);
  }
  if (in_FPSCR_SZ == '\0') {
    *(float *)(param_4 + 1) = fVar7;
  }
  else {
    param_4[1] = uVar10;
  }
  return;
}

