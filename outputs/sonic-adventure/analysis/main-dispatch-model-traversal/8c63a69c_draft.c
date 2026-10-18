/* Ghidra draft only; not buildable/verified source. */

double FUN_8c63a69c(float param_1,float param_2,float param_3,float param_4,undefined4 param_5,
                   undefined4 param_6,double *param_7)

{
  double *pdVar1;
  double in_dr0;
  float fVar4;
  double dVar2;
  double dVar3;
  double in_dr2;
  undefined4 uVar5;
  float fVar6;
  double in_xd0;
  double in_xd2;
  double in_xd4;
  double in_xd6;
  double in_xd8;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  if (param_7 == (double *)0x0) {
    if (in_FPSCR_SZ == '\0') {
      in_xd0 = (double)CONCAT44(SUB84(in_dr0,0),SUB84(in_dr0,0));
      in_xd4 = (double)CONCAT44(param_2,param_5);
      in_xd6 = (double)CONCAT44(param_4,param_6);
    }
    if (in_FPSCR_PR == '\0') {
      fVar4 = (float)((ulonglong)in_xd0 >> 0x20) * param_1;
      fVar6 = SUB84(in_xd4,0) * param_2;
      param_2 = (float)((ulonglong)in_xd6 >> 0x20) * param_2;
    }
    else {
      fVar4 = (float)((ulonglong)
                      (in_xd0 * (double)CONCAT44(param_1,param_2) *
                      (double)CONCAT44(param_1,param_2)) >> 0x20);
      fVar6 = SUB84(in_xd4 * (double)CONCAT44(param_1,param_2) * (double)CONCAT44(param_1,param_2),0
                   );
      param_2 = (float)((ulonglong)
                        (in_xd6 * (double)CONCAT44(param_1,param_2) *
                        (double)CONCAT44(param_1,param_2)) >> 0x20);
    }
    if (in_FPSCR_SZ == '\0') {
      in_xd8 = (double)CONCAT44(fVar6,fVar4);
      param_4 = param_2;
    }
    if (in_FPSCR_PR == '\0') {
      dVar2 = (double)CONCAT44((float)((ulonglong)in_xd8 >> 0x20) * param_3,
                               SUB84(in_xd8,0) * param_3);
    }
    else {
      dVar2 = in_xd8 * (double)CONCAT44(param_3,param_4) * (double)CONCAT44(param_3,param_4);
    }
    return dVar2;
  }
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)param_7);
    pdVar1 = (double *)((int)param_7 + 4);
  }
  else {
    in_xd2 = *param_7;
    pdVar1 = param_7 + 1;
  }
  if (in_FPSCR_PR == '\0') {
    fVar6 = SUB84(in_dr2,0) * param_1;
  }
  else {
    fVar6 = SUB84(in_dr2 * (double)CONCAT44(param_1,param_2),0);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar2 = (double)CONCAT44(*(undefined4 *)pdVar1,fVar6);
    pdVar1 = (double *)((int)pdVar1 + 4);
  }
  else {
    dVar2 = *pdVar1;
    pdVar1 = pdVar1 + 1;
  }
  if (in_FPSCR_PR == '\0') {
    dVar2 = (double)CONCAT44((float)((ulonglong)dVar2 >> 0x20) * param_1,SUB84(dVar2,0));
  }
  else {
    dVar2 = dVar2 * (double)CONCAT44(param_1,param_2);
  }
  uVar5 = (undefined4)((ulonglong)dVar2 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    *(int *)param_7 = SUB84(dVar2,0);
    in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),*(undefined4 *)pdVar1);
    pdVar1 = (double *)((int)pdVar1 + 4);
    *(undefined4 *)((int)param_7 + 4) = uVar5;
  }
  else {
    *param_7 = in_xd2;
    in_xd0 = *pdVar1;
    pdVar1 = pdVar1 + 1;
    *(double *)((int)param_7 + 4) = dVar2;
  }
  if (in_FPSCR_PR == '\0') {
    dVar3 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),SUB84(in_dr0,0) * param_1);
  }
  else {
    dVar3 = in_dr0 * (double)CONCAT44(param_1,param_2);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar2 = (double)CONCAT44(uVar5,*(undefined4 *)pdVar1);
    pdVar1 = (double *)((int)pdVar1 + 4);
  }
  else {
    in_xd2 = *pdVar1;
    pdVar1 = pdVar1 + 1;
  }
  if (in_FPSCR_PR == '\0') {
    fVar6 = SUB84(dVar2,0) * param_1;
  }
  else {
    fVar6 = SUB84(dVar2 * (double)CONCAT44(param_1,param_2),0);
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_7 + 1) = SUB84(dVar3,0);
    dVar2 = (double)CONCAT44(*(undefined4 *)pdVar1,fVar6);
    pdVar1 = (double *)((int)pdVar1 + 4);
  }
  else {
    param_7[1] = in_xd0;
    dVar2 = *pdVar1;
    pdVar1 = pdVar1 + 1;
  }
  if (in_FPSCR_PR == '\0') {
    dVar2 = (double)CONCAT44((float)((ulonglong)dVar2 >> 0x20) * param_2,SUB84(dVar2,0));
  }
  else {
    dVar2 = dVar2 * (double)CONCAT44(param_1,param_2);
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)param_7 + 0xc) = SUB84(dVar2,0);
    dVar3 = (double)CONCAT44((int)((ulonglong)dVar3 >> 0x20),*(undefined4 *)pdVar1);
    pdVar1 = (double *)((int)pdVar1 + 4);
  }
  else {
    *(double *)((int)param_7 + 0xc) = in_xd2;
    in_xd0 = *pdVar1;
    pdVar1 = pdVar1 + 1;
  }
  if (in_FPSCR_PR == '\0') {
    dVar3 = (double)CONCAT44((int)((ulonglong)dVar3 >> 0x20),SUB84(dVar3,0) * param_2);
  }
  else {
    dVar3 = dVar3 * (double)CONCAT44(param_1,param_2);
  }
  uVar5 = (undefined4)((ulonglong)dVar2 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(param_7 + 2) = uVar5;
    dVar2 = (double)CONCAT44(uVar5,*(undefined4 *)pdVar1);
    pdVar1 = (double *)((int)pdVar1 + 4);
  }
  else {
    param_7[2] = dVar2;
    in_xd2 = *pdVar1;
    pdVar1 = pdVar1 + 1;
  }
  if (in_FPSCR_PR == '\0') {
    fVar6 = SUB84(dVar2,0) * param_2;
  }
  else {
    fVar6 = SUB84(dVar2 * (double)CONCAT44(param_1,param_2),0);
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)param_7 + 0x14) = SUB84(dVar3,0);
    dVar2 = (double)CONCAT44(*(undefined4 *)pdVar1,fVar6);
    pdVar1 = (double *)((int)pdVar1 + 4);
  }
  else {
    *(double *)((int)param_7 + 0x14) = in_xd0;
    dVar2 = *pdVar1;
    pdVar1 = pdVar1 + 1;
  }
  if (in_FPSCR_PR == '\0') {
    dVar2 = (double)CONCAT44((float)((ulonglong)dVar2 >> 0x20) * param_2,SUB84(dVar2,0));
  }
  else {
    dVar2 = dVar2 * (double)CONCAT44(param_1,param_2);
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_7 + 3) = SUB84(dVar2,0);
    dVar3 = (double)CONCAT44((int)((ulonglong)dVar3 >> 0x20),*(undefined4 *)pdVar1);
    pdVar1 = (double *)((int)pdVar1 + 4);
  }
  else {
    param_7[3] = in_xd2;
    in_xd0 = *pdVar1;
    pdVar1 = pdVar1 + 1;
  }
  if (in_FPSCR_PR == '\0') {
    dVar3 = (double)CONCAT44((int)((ulonglong)dVar3 >> 0x20),SUB84(dVar3,0) * param_3);
  }
  else {
    dVar3 = dVar3 * (double)CONCAT44(param_3,param_4);
  }
  uVar5 = (undefined4)((ulonglong)dVar2 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)((int)param_7 + 0x1c) = uVar5;
    dVar2 = (double)CONCAT44(uVar5,*(undefined4 *)pdVar1);
    pdVar1 = (double *)((int)pdVar1 + 4);
  }
  else {
    *(double *)((int)param_7 + 0x1c) = dVar2;
    in_xd2 = *pdVar1;
    pdVar1 = pdVar1 + 1;
  }
  if (in_FPSCR_PR == '\0') {
    fVar6 = SUB84(dVar2,0) * param_3;
  }
  else {
    fVar6 = SUB84(dVar2 * (double)CONCAT44(param_3,param_4),0);
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_7 + 4) = SUB84(dVar3,0);
    dVar2 = (double)CONCAT44(*(undefined4 *)pdVar1,fVar6);
    pdVar1 = (double *)((int)pdVar1 + 4);
  }
  else {
    param_7[4] = in_xd0;
    dVar2 = *pdVar1;
    pdVar1 = pdVar1 + 1;
  }
  if (in_FPSCR_PR == '\0') {
    dVar2 = (double)CONCAT44((float)((ulonglong)dVar2 >> 0x20) * param_3,SUB84(dVar2,0));
  }
  else {
    dVar2 = dVar2 * (double)CONCAT44(param_3,param_4);
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)param_7 + 0x24) = SUB84(dVar2,0);
    dVar3 = (double)CONCAT44((int)((ulonglong)dVar3 >> 0x20),*(undefined4 *)pdVar1);
  }
  else {
    *(double *)((int)param_7 + 0x24) = in_xd2;
    in_xd0 = *pdVar1;
  }
  if (in_FPSCR_PR == '\0') {
    dVar3 = (double)CONCAT44((int)((ulonglong)dVar3 >> 0x20),SUB84(dVar3,0) * param_3);
  }
  else {
    dVar3 = dVar3 * (double)CONCAT44(param_3,param_4);
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_7 + 5) = (int)((ulonglong)dVar2 >> 0x20);
    *(int *)pdVar1 = SUB84(dVar3,0);
  }
  else {
    param_7[5] = dVar2;
    *pdVar1 = in_xd0;
  }
  return dVar3;
}

