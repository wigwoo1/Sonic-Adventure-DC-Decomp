/* Ghidra draft only; not buildable/verified source. */

double FUN_8c63a5ec(undefined4 param_1,undefined4 param_2,undefined4 param_3,double *param_4,
                   double *param_5)

{
  double *pdVar1;
  double in_dr0;
  float fVar3;
  double dVar2;
  double in_dr2;
  double dVar4;
  undefined4 uVar5;
  double dVar6;
  double dVar7;
  float fVar9;
  float fVar10;
  double in_xd0;
  double in_xd2;
  double in_xd4;
  double in_xd6;
  double in_xd8;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  float fVar8;
  
  if (in_FPSCR_SZ == '\0') {
    dVar6 = (double)CONCAT44(*(undefined4 *)param_5,*(undefined4 *)((int)param_5 + 4));
    dVar7 = (double)CONCAT44(*(undefined4 *)(param_5 + 1),param_1);
  }
  else {
    dVar6 = *param_5;
    in_xd4 = param_5[1];
    dVar7 = param_5[2];
  }
  fVar10 = SUB84(dVar6,0);
  fVar3 = (float)((ulonglong)dVar6 >> 0x20);
  fVar8 = (float)((ulonglong)dVar7 >> 0x20);
  if (param_4 == (double *)0x0) {
    if (in_FPSCR_SZ == '\0') {
      in_xd0 = (double)CONCAT44(SUB84(in_dr0,0),SUB84(in_dr0,0));
      in_xd4 = (double)CONCAT44(fVar10,param_2);
      in_xd6 = (double)CONCAT44(SUB84(dVar7,0),param_3);
    }
    if (in_FPSCR_PR == '\0') {
      fVar3 = (float)((ulonglong)in_xd0 >> 0x20) * fVar3;
      fVar9 = SUB84(in_xd4,0) * fVar10;
      fVar10 = (float)((ulonglong)in_xd6 >> 0x20) * fVar10;
    }
    else {
      fVar3 = (float)((ulonglong)(in_xd0 * dVar6 * dVar6) >> 0x20);
      fVar9 = SUB84(in_xd4 * dVar6 * dVar6,0);
      fVar10 = (float)((ulonglong)(in_xd6 * dVar6 * dVar6) >> 0x20);
    }
    if (in_FPSCR_SZ == '\0') {
      dVar7 = (double)CONCAT44(fVar8,fVar10);
      in_xd8 = (double)CONCAT44(fVar9,fVar3);
    }
    fVar10 = (float)((ulonglong)dVar7 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      dVar7 = (double)CONCAT44((float)((ulonglong)in_xd8 >> 0x20) * fVar10,SUB84(in_xd8,0) * fVar10)
      ;
    }
    else {
      dVar7 = in_xd8 * dVar7 * dVar7;
    }
    return dVar7;
  }
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)param_4);
    pdVar1 = (double *)((int)param_4 + 4);
  }
  else {
    in_xd2 = *param_4;
    pdVar1 = param_4 + 1;
  }
  if (in_FPSCR_PR == '\0') {
    fVar9 = SUB84(in_dr2,0) * fVar3;
  }
  else {
    fVar9 = SUB84(in_dr2 * dVar6,0);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar4 = (double)CONCAT44(*(undefined4 *)pdVar1,fVar9);
    pdVar1 = (double *)((int)pdVar1 + 4);
  }
  else {
    dVar4 = *pdVar1;
    pdVar1 = pdVar1 + 1;
  }
  if (in_FPSCR_PR == '\0') {
    dVar4 = (double)CONCAT44((float)((ulonglong)dVar4 >> 0x20) * fVar3,SUB84(dVar4,0));
  }
  else {
    dVar4 = dVar4 * dVar6;
  }
  uVar5 = (undefined4)((ulonglong)dVar4 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    *(int *)param_4 = SUB84(dVar4,0);
    in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),*(undefined4 *)pdVar1);
    pdVar1 = (double *)((int)pdVar1 + 4);
    *(undefined4 *)((int)param_4 + 4) = uVar5;
  }
  else {
    *param_4 = in_xd2;
    in_xd0 = *pdVar1;
    pdVar1 = pdVar1 + 1;
    *(double *)((int)param_4 + 4) = dVar4;
  }
  if (in_FPSCR_PR == '\0') {
    dVar2 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),SUB84(in_dr0,0) * fVar3);
  }
  else {
    dVar2 = in_dr0 * dVar6;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar4 = (double)CONCAT44(uVar5,*(undefined4 *)pdVar1);
    pdVar1 = (double *)((int)pdVar1 + 4);
  }
  else {
    in_xd2 = *pdVar1;
    pdVar1 = pdVar1 + 1;
  }
  if (in_FPSCR_PR == '\0') {
    fVar3 = SUB84(dVar4,0) * fVar3;
  }
  else {
    fVar3 = SUB84(dVar4 * dVar6,0);
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_4 + 1) = SUB84(dVar2,0);
    dVar4 = (double)CONCAT44(*(undefined4 *)pdVar1,fVar3);
    pdVar1 = (double *)((int)pdVar1 + 4);
  }
  else {
    param_4[1] = in_xd0;
    dVar4 = *pdVar1;
    pdVar1 = pdVar1 + 1;
  }
  if (in_FPSCR_PR == '\0') {
    dVar4 = (double)CONCAT44((float)((ulonglong)dVar4 >> 0x20) * fVar10,SUB84(dVar4,0));
  }
  else {
    dVar4 = dVar4 * dVar6;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)param_4 + 0xc) = SUB84(dVar4,0);
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),*(undefined4 *)pdVar1);
    pdVar1 = (double *)((int)pdVar1 + 4);
  }
  else {
    *(double *)((int)param_4 + 0xc) = in_xd2;
    in_xd0 = *pdVar1;
    pdVar1 = pdVar1 + 1;
  }
  if (in_FPSCR_PR == '\0') {
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),SUB84(dVar2,0) * fVar10);
  }
  else {
    dVar2 = dVar2 * dVar6;
  }
  uVar5 = (undefined4)((ulonglong)dVar4 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(param_4 + 2) = uVar5;
    dVar4 = (double)CONCAT44(uVar5,*(undefined4 *)pdVar1);
    pdVar1 = (double *)((int)pdVar1 + 4);
  }
  else {
    param_4[2] = dVar4;
    in_xd2 = *pdVar1;
    pdVar1 = pdVar1 + 1;
  }
  if (in_FPSCR_PR == '\0') {
    fVar3 = SUB84(dVar4,0) * fVar10;
  }
  else {
    fVar3 = SUB84(dVar4 * dVar6,0);
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)param_4 + 0x14) = SUB84(dVar2,0);
    dVar4 = (double)CONCAT44(*(undefined4 *)pdVar1,fVar3);
    pdVar1 = (double *)((int)pdVar1 + 4);
  }
  else {
    *(double *)((int)param_4 + 0x14) = in_xd0;
    dVar4 = *pdVar1;
    pdVar1 = pdVar1 + 1;
  }
  if (in_FPSCR_PR == '\0') {
    dVar4 = (double)CONCAT44((float)((ulonglong)dVar4 >> 0x20) * fVar10,SUB84(dVar4,0));
  }
  else {
    dVar4 = dVar4 * dVar6;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_4 + 3) = SUB84(dVar4,0);
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),*(undefined4 *)pdVar1);
    pdVar1 = (double *)((int)pdVar1 + 4);
  }
  else {
    param_4[3] = in_xd2;
    in_xd0 = *pdVar1;
    pdVar1 = pdVar1 + 1;
  }
  if (in_FPSCR_PR == '\0') {
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),SUB84(dVar2,0) * fVar8);
  }
  else {
    dVar2 = dVar2 * dVar7;
  }
  uVar5 = (undefined4)((ulonglong)dVar4 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)((int)param_4 + 0x1c) = uVar5;
    dVar4 = (double)CONCAT44(uVar5,*(undefined4 *)pdVar1);
    pdVar1 = (double *)((int)pdVar1 + 4);
  }
  else {
    *(double *)((int)param_4 + 0x1c) = dVar4;
    in_xd2 = *pdVar1;
    pdVar1 = pdVar1 + 1;
  }
  if (in_FPSCR_PR == '\0') {
    fVar10 = SUB84(dVar4,0) * fVar8;
  }
  else {
    fVar10 = SUB84(dVar4 * dVar7,0);
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_4 + 4) = SUB84(dVar2,0);
    dVar6 = (double)CONCAT44(*(undefined4 *)pdVar1,fVar10);
    pdVar1 = (double *)((int)pdVar1 + 4);
  }
  else {
    param_4[4] = in_xd0;
    dVar6 = *pdVar1;
    pdVar1 = pdVar1 + 1;
  }
  if (in_FPSCR_PR == '\0') {
    dVar6 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) * fVar8,SUB84(dVar6,0));
  }
  else {
    dVar6 = dVar6 * dVar7;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)param_4 + 0x24) = SUB84(dVar6,0);
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),*(undefined4 *)pdVar1);
  }
  else {
    *(double *)((int)param_4 + 0x24) = in_xd2;
    in_xd0 = *pdVar1;
  }
  if (in_FPSCR_PR == '\0') {
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),SUB84(dVar2,0) * fVar8);
  }
  else {
    dVar2 = dVar2 * dVar7;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_4 + 5) = (int)((ulonglong)dVar6 >> 0x20);
    *(int *)pdVar1 = SUB84(dVar2,0);
  }
  else {
    param_4[5] = dVar6;
    *pdVar1 = in_xd0;
  }
  return dVar2;
}

