/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c040c86) */
/* WARNING: Removing unreachable block (ram,0x8c040c94) */

void FUN_8c040c5c(undefined4 param_1,undefined4 param_2,undefined4 param_3,int *param_4,int param_5,
                 uint param_6,undefined4 *param_7)

{
  double *pdVar1;
  int iVar2;
  double *pdVar3;
  int *piVar4;
  float fVar5;
  undefined8 in_dr2;
  float fVar6;
  double dVar7;
  float fVar8;
  double dVar9;
  double dVar10;
  double in_xd6;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  piVar4 = param_4 + param_5 * 4;
  if (param_5 + 1U < param_6) {
    param_4 = piVar4 + 4;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar7 = (double)CONCAT44(*(undefined4 *)(DAT_8c040ce8 + (char)(*DAT_8c040ce4 * '\x1c')),param_1)
    ;
  }
  else {
    dVar7 = *(double *)(DAT_8c040ce8 + (char)(*DAT_8c040ce4 * '\x1c'));
  }
  fVar5 = (float)*piVar4;
  iVar2 = *param_4 - *piVar4;
  if (in_FPSCR_PR == '\0') {
    fVar5 = (float)((ulonglong)dVar7 >> 0x20) - fVar5;
  }
  else {
    fVar5 = (float)((ulonglong)(dVar7 - (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),fVar5)) >>
                   0x20);
  }
  if (0 < iVar2) {
    fVar6 = (float)iVar2;
    dVar7 = (double)CONCAT44(fVar5,fVar6);
    if (in_FPSCR_PR == '\0') {
      fVar6 = 1.0 / SQRT(fVar6);
      fVar5 = fVar5 * fVar6 * fVar6;
    }
    else {
      dVar7 = dVar7 * dVar7;
      fVar5 = (float)((ulonglong)(dVar7 * dVar7) >> 0x20);
    }
  }
  if (in_FPSCR_PR == '\0') {
    dVar7 = (double)CONCAT44(fVar5,1.0 - fVar5);
  }
  else {
    dVar7 = (double)CONCAT44(fVar5,0x3f800000) - (double)CONCAT44(fVar5,0x3f800000);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar10 = (double)CONCAT44(piVar4[1],param_3);
    pdVar3 = (double *)(piVar4 + 2);
  }
  else {
    dVar10 = *(double *)(piVar4 + 1);
    pdVar3 = (double *)(piVar4 + 3);
  }
  fVar5 = SUB84(dVar7,0);
  if (in_FPSCR_PR == '\0') {
    dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) * fVar5,SUB84(dVar10,0));
  }
  else {
    dVar10 = dVar10 * dVar7;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar9 = (double)CONCAT44(param_4[1],param_2);
    pdVar1 = (double *)(param_4 + 2);
  }
  else {
    dVar9 = *(double *)(param_4 + 1);
    pdVar1 = (double *)(param_4 + 3);
  }
  fVar6 = (float)((ulonglong)dVar7 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) * fVar6,SUB84(dVar9,0));
  }
  else {
    dVar9 = dVar9 * dVar7;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),*(int *)pdVar1);
    pdVar1 = (double *)((int)pdVar1 + 4);
  }
  else {
    in_xd6 = *pdVar1;
    pdVar1 = pdVar1 + 1;
  }
  if (in_FPSCR_PR == '\0') {
    dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) + (float)((ulonglong)dVar9 >> 0x20)
                              ,SUB84(dVar10,0));
  }
  else {
    dVar10 = dVar10 + dVar9;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar9 = (double)CONCAT44(*(int *)pdVar3,SUB84(dVar9,0));
    pdVar3 = (double *)((int)pdVar3 + 4);
  }
  else {
    dVar9 = *pdVar3;
    pdVar3 = pdVar3 + 1;
  }
  if (in_FPSCR_PR == '\0') {
    fVar8 = SUB84(dVar9,0) * fVar6;
    dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) * fVar5 + fVar8,fVar8);
  }
  else {
    dVar9 = dVar9 * dVar7 * dVar7;
    dVar9 = dVar9 + dVar9;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),*(int *)pdVar3);
  }
  else {
    in_xd6 = *pdVar3;
  }
  if (in_FPSCR_PR == '\0') {
    dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),SUB84(dVar9,0) * fVar5);
  }
  else {
    dVar9 = dVar9 * dVar7;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar7 = (double)CONCAT44(fVar6,*(int *)pdVar1);
  }
  if (in_FPSCR_PR == '\0') {
    dVar9 = (double)CONCAT44((int)((ulonglong)dVar9 >> 0x20),
                             SUB84(dVar9,0) + SUB84(dVar7,0) * (float)((ulonglong)dVar7 >> 0x20));
  }
  else {
    dVar9 = dVar9 + dVar7 * dVar7;
  }
  if (in_FPSCR_SZ == '\0') {
    param_7[2] = SUB84(dVar9,0);
    param_7[1] = (int)((ulonglong)dVar9 >> 0x20);
    *param_7 = (int)((ulonglong)dVar10 >> 0x20);
  }
  else {
    *(double *)(param_7 + 1) = in_xd6;
    *(double *)(param_7 + -1) = dVar9;
    *(double *)(param_7 + -3) = dVar10;
  }
  return;
}

