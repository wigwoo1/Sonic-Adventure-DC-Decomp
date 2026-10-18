/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c03ff46) */
/* WARNING: Removing unreachable block (ram,0x8c03ff56) */

void FUN_8c03ff2c(undefined4 param_1,undefined4 param_2,undefined4 param_3,int *param_4,int param_5,
                 uint param_6,undefined4 *param_7)

{
  int *piVar1;
  double *pdVar2;
  int iVar3;
  double *pdVar4;
  float fVar5;
  undefined8 in_dr2;
  float fVar6;
  double dVar7;
  double dVar8;
  float fVar9;
  double dVar10;
  double in_xd6;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  piVar1 = param_4 + param_5 * 4;
  if (param_5 + 1U < param_6) {
    param_4 = piVar1 + 4;
  }
  fVar5 = (float)*piVar1;
  if (in_FPSCR_SZ == '\0') {
    dVar7 = (double)CONCAT44(*(undefined4 *)DAT_8c04007c,param_1);
  }
  else {
    dVar7 = *DAT_8c04007c;
  }
  iVar3 = *param_4 - *piVar1;
  if (in_FPSCR_PR == '\0') {
    fVar5 = (float)((ulonglong)dVar7 >> 0x20) - fVar5;
  }
  else {
    fVar5 = (float)((ulonglong)(dVar7 - (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),fVar5)) >>
                   0x20);
  }
  if (0 < iVar3) {
    fVar6 = (float)iVar3;
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
    dVar10 = (double)CONCAT44(piVar1[1],param_3);
    pdVar4 = (double *)(piVar1 + 2);
    dVar8 = (double)CONCAT44(param_4[1],param_2);
    pdVar2 = (double *)(param_4 + 2);
  }
  else {
    dVar10 = *(double *)(piVar1 + 1);
    pdVar4 = (double *)(piVar1 + 3);
    dVar8 = *(double *)(param_4 + 1);
    pdVar2 = (double *)(param_4 + 3);
  }
  fVar5 = SUB84(dVar7,0);
  if (in_FPSCR_PR == '\0') {
    dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) * fVar5,SUB84(dVar10,0));
  }
  else {
    dVar10 = dVar10 * dVar7;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),*(int *)pdVar2);
    pdVar2 = (double *)((int)pdVar2 + 4);
  }
  else {
    in_xd6 = *pdVar2;
    pdVar2 = pdVar2 + 1;
  }
  fVar6 = (float)((ulonglong)dVar7 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    fVar9 = (float)((ulonglong)dVar8 >> 0x20) * fVar6;
    dVar8 = (double)CONCAT44(fVar9,SUB84(dVar8,0));
    dVar10 = (double)CONCAT44((float)((ulonglong)dVar10 >> 0x20) + fVar9,SUB84(dVar10,0));
  }
  else {
    dVar8 = dVar8 * dVar7;
    dVar10 = dVar10 + dVar8;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar8 = (double)CONCAT44(*(int *)pdVar4,SUB84(dVar8,0));
    pdVar4 = (double *)((int)pdVar4 + 4);
  }
  else {
    dVar8 = *pdVar4;
    pdVar4 = pdVar4 + 1;
  }
  if (in_FPSCR_PR == '\0') {
    fVar9 = SUB84(dVar8,0) * fVar6;
    dVar8 = (double)CONCAT44((float)((ulonglong)dVar8 >> 0x20) * fVar5 + fVar9,fVar9);
  }
  else {
    dVar8 = dVar8 * dVar7 * dVar7;
    dVar8 = dVar8 + dVar8;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),*(int *)pdVar4);
  }
  else {
    in_xd6 = *pdVar4;
  }
  if (in_FPSCR_PR == '\0') {
    dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),SUB84(dVar8,0) * fVar5);
  }
  else {
    dVar8 = dVar8 * dVar7;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar7 = (double)CONCAT44(fVar6,*(int *)pdVar2);
  }
  if (in_FPSCR_PR == '\0') {
    dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),
                             SUB84(dVar8,0) + SUB84(dVar7,0) * (float)((ulonglong)dVar7 >> 0x20));
  }
  else {
    dVar8 = dVar8 + dVar7 * dVar7;
  }
  if (in_FPSCR_SZ == '\0') {
    param_7[2] = SUB84(dVar8,0);
    param_7[1] = (int)((ulonglong)dVar8 >> 0x20);
    *param_7 = (int)((ulonglong)dVar10 >> 0x20);
  }
  else {
    *(double *)(param_7 + 1) = in_xd6;
    *(double *)(param_7 + -1) = dVar8;
    *(double *)(param_7 + -3) = dVar10;
  }
  return;
}

