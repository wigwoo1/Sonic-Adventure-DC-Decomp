/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c639b72) */
/* WARNING: Restarted to delay deadcode elimination for space: register */

double FUN_8c639b50(undefined4 param_1,double *param_2,double *param_3,double *param_4)

{
  double dVar1;
  double dVar2;
  double dVar3;
  double dVar5;
  float fVar6;
  undefined4 uVar7;
  float fVar8;
  double dVar9;
  float fVar10;
  double in_xd8;
  double in_xd10;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  uint uVar4;
  
  if (in_FPSCR_SZ == '\0') {
    dVar5 = (double)CONCAT44(*(undefined4 *)param_3,param_1);
    uVar4 = *(uint *)((int)param_2 + 4);
    dVar3 = (double)CONCAT44(uVar4,*(undefined4 *)(param_2 + 1));
    dVar9 = (double)CONCAT44(*(undefined4 *)param_2,*(undefined4 *)(param_3 + 1));
    dVar2 = (double)CONCAT44(uVar4,*(undefined4 *)((int)param_3 + 4));
  }
  else {
    dVar9 = *param_2;
    dVar5 = *param_3;
    dVar3 = *(double *)((int)param_2 + 4);
    uVar4 = (uint)((ulonglong)dVar3 >> 0x20);
    in_xd8 = param_2[1];
    in_xd10 = param_3[1];
    dVar2 = dVar3;
  }
  fVar8 = SUB84(dVar9,0);
  if (in_FPSCR_PR == '\0') {
    dVar3 = (double)CONCAT44((float)((ulonglong)dVar3 >> 0x20) * fVar8,SUB84(dVar3,0));
  }
  else {
    dVar3 = dVar3 * dVar9;
  }
  dVar1 = in_xd8;
  if (in_FPSCR_SZ == '\0') {
    dVar1 = (double)((longlong)dVar3 << 0x20);
  }
  if (in_FPSCR_PR == '\0') {
    dVar3 = (double)CONCAT44(-(float)((ulonglong)dVar1 >> 0x20) * SUB84(dVar2,0) +
                             (float)((ulonglong)dVar3 >> 0x20),SUB84(dVar3,0));
  }
  uVar7 = (undefined4)((ulonglong)dVar3 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)param_4 = uVar7;
  }
  else {
    *param_4 = dVar3;
  }
  fVar6 = (float)((ulonglong)dVar5 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar3 = (double)CONCAT44(uVar7,SUB84(dVar3,0) * fVar6);
  }
  else {
    dVar3 = dVar3 * dVar5;
  }
  fVar10 = (float)((ulonglong)dVar9 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    dVar9 = (double)((ulonglong)(uint)fVar10 << 0x20);
  }
  fVar8 = -fVar8;
  if (in_FPSCR_PR == '\0') {
    dVar3 = (double)(ulonglong)(uint)((float)((ulonglong)dVar9 >> 0x20) * fVar8 + SUB84(dVar3,0));
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)param_4 + 4) = SUB84(dVar3,0);
  }
  else {
    *(double *)((int)param_4 + 4) = in_xd8;
  }
  if (in_FPSCR_PR == '\0') {
    dVar9 = (double)CONCAT44(fVar10 * SUB84(dVar2,0),fVar8);
    fVar6 = -fVar6;
  }
  else {
    dVar9 = (double)CONCAT44(fVar10,fVar8) * dVar2;
    fVar6 = (float)((ulonglong)-dVar5 >> 0x20);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar2 = (double)((ulonglong)uVar4 << 0x20);
  }
  if (in_FPSCR_PR == '\0') {
    dVar9 = (double)CONCAT44((float)((ulonglong)dVar2 >> 0x20) * fVar6 +
                             (float)((ulonglong)dVar9 >> 0x20),SUB84(dVar9,0));
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_4 + 1) = (int)((ulonglong)dVar9 >> 0x20);
    in_xd10 = (double)((ulonglong)dVar3 & 0xffffffff);
  }
  else {
    param_4[1] = dVar9;
  }
  if (in_FPSCR_PR == '\0') {
    dVar3 = (double)CONCAT44(SQRT((float)((ulonglong)in_xd10 >> 0x20)),SUB84(in_xd10,0));
  }
  else {
    dVar3 = SQRT(in_xd10);
  }
  return dVar3;
}

