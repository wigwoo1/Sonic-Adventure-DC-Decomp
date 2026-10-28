/* Ghidra draft only; not buildable/verified source. */

void FUN_8c027360(undefined4 param_1,undefined4 param_2,double param_3,double *param_4,
                 double *param_5,undefined4 *param_6,double *param_7)

{
  float fVar1;
  double in_dr2;
  double dVar3;
  double dVar4;
  float fVar5;
  float fVar6;
  double dVar7;
  double in_xd6;
  double in_xd8;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  double dVar2;
  
  if (in_FPSCR_SZ == '\0') {
    param_3 = (double)CONCAT44((int)((ulonglong)param_3 >> 0x20),*(undefined4 *)param_5);
    param_5 = (double *)((int)param_5 + 4);
    dVar3 = (double)CONCAT44(*(undefined4 *)param_4,param_1);
    param_4 = (double *)((int)param_4 + 4);
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*param_6);
    param_6 = param_6 + 1;
  }
  else {
    in_xd8 = *param_5;
    param_5 = param_5 + 1;
    dVar3 = *param_4;
    param_4 = param_4 + 1;
    param_6 = param_6 + 2;
  }
  fVar6 = (float)((ulonglong)dVar3 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    fVar5 = SUB84(param_3,0) - fVar6;
  }
  else {
    fVar5 = SUB84(param_3 - dVar3,0);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar7 = (double)CONCAT44(*(undefined4 *)param_5,fVar5);
    param_5 = (double *)((int)param_5 + 4);
  }
  else {
    dVar7 = *param_5;
    param_5 = param_5 + 1;
  }
  if (in_FPSCR_PR == '\0') {
    dVar3 = (double)CONCAT44(fVar6 - SUB84(in_dr2,0),SUB84(dVar3,0));
  }
  else {
    dVar3 = dVar3 - in_dr2;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar4 = (double)CONCAT44(*(undefined4 *)param_4,param_2);
    param_4 = (double *)((int)param_4 + 4);
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*param_6);
    param_6 = param_6 + 1;
  }
  else {
    dVar4 = *param_4;
    param_4 = param_4 + 1;
    param_6 = param_6 + 2;
  }
  fVar6 = (float)((ulonglong)dVar4 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar7 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) - fVar6,SUB84(dVar7,0));
  }
  else {
    dVar7 = dVar7 - dVar4;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar3 = (double)CONCAT44((int)((ulonglong)dVar3 >> 0x20),*(undefined4 *)param_4);
  }
  if (in_FPSCR_PR == '\0') {
    dVar4 = (double)CONCAT44(fVar6 - SUB84(in_dr2,0),SUB84(dVar4,0));
  }
  else {
    dVar4 = dVar4 - in_dr2;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),*(undefined4 *)param_5);
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*param_6);
  }
  else {
    in_xd6 = *param_5;
  }
  if (in_FPSCR_PR == '\0') {
    dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),SUB84(dVar4,0) - SUB84(dVar3,0));
    dVar3 = (double)CONCAT44((int)((ulonglong)dVar3 >> 0x20),SUB84(dVar3,0) - SUB84(in_dr2,0));
  }
  else {
    dVar4 = dVar4 - dVar3;
    dVar3 = dVar3 - in_dr2;
  }
  fVar6 = (float)((ulonglong)dVar7 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)(ulonglong)(uint)fVar6;
  }
  fVar5 = (float)((ulonglong)dVar3 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar7 = (double)CONCAT44(fVar6 * fVar5,SUB84(dVar7,0));
  }
  else {
    dVar7 = dVar7 * dVar3;
  }
  if (in_FPSCR_SZ == '\0') {
    in_xd6 = (double)CONCAT44(SUB84(dVar4,0),SUB84(in_dr2,0));
  }
  fVar6 = (float)((ulonglong)dVar4 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    fVar1 = SUB84(in_xd6,0) * SUB84(dVar3,0);
    dVar4 = (double)CONCAT44(fVar6,SUB84(dVar4,0) * fVar5);
    dVar2 = (double)CONCAT44((float)((ulonglong)in_xd6 >> 0x20) * fVar6 - fVar1,fVar1);
  }
  else {
    dVar2 = in_xd6 * dVar4 * dVar3;
    dVar4 = dVar4 * dVar3;
    dVar2 = dVar2 - dVar2;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)param_7 = (int)((ulonglong)dVar2 >> 0x20);
    in_xd8 = (double)CONCAT44(SUB84(dVar7,0),SUB84(dVar2,0));
  }
  else {
    *param_7 = dVar2;
  }
  if (in_FPSCR_PR == '\0') {
    fVar6 = SUB84(dVar7,0) * (float)((ulonglong)dVar4 >> 0x20);
    dVar3 = (double)CONCAT44((float)((ulonglong)in_xd8 >> 0x20) * SUB84(dVar3,0) - SUB84(dVar4,0),
                             SUB84(in_xd8,0));
    dVar7 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) - fVar6,fVar6);
  }
  else {
    dVar3 = in_xd8 * dVar3 - dVar4;
    dVar7 = dVar7 * dVar4 - dVar7 * dVar4;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)param_7 + 4) = (int)((ulonglong)dVar3 >> 0x20);
    *(int *)(param_7 + 1) = (int)((ulonglong)dVar7 >> 0x20);
  }
  else {
    *(double *)((int)param_7 + 4) = dVar3;
    param_7[1] = dVar7;
  }
  return;
}

