/* Ghidra draft only; not buildable/verified source. */

void FUN_8c638eac(undefined4 param_1,double *param_2,double *param_3)

{
  float fVar1;
  double in_dr0;
  undefined4 in_fr3;
  double dVar2;
  double dVar3;
  double dVar4;
  double in_xd0;
  double in_xd2;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  if (in_FPSCR_SZ == '\0') {
    in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),*(undefined4 *)param_3);
    param_3 = (double *)((int)param_3 + 4);
    dVar3 = (double)CONCAT44(*(undefined4 *)param_2,*(undefined4 *)((int)param_2 + 4));
    param_2 = param_2 + 1;
  }
  else {
    dVar3 = *param_2;
    in_xd0 = *param_3;
    param_3 = param_3 + 1;
    param_2 = param_2 + 2;
  }
  if (in_FPSCR_PR == '\0') {
    fVar1 = SUB84(in_dr0,0) + (float)((ulonglong)dVar3 >> 0x20);
  }
  else {
    fVar1 = SUB84(in_dr0 + dVar3,0);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar2 = (double)CONCAT44(*(undefined4 *)param_3,in_fr3);
    param_3 = (double *)((int)param_3 + 4);
    dVar4 = (double)CONCAT44(*(undefined4 *)param_2,param_1);
  }
  else {
    dVar2 = *param_3;
    param_3 = param_3 + 1;
    dVar4 = *param_2;
  }
  if (in_FPSCR_PR == '\0') {
    dVar2 = (double)CONCAT44((float)((ulonglong)dVar2 >> 0x20) + SUB84(dVar3,0),SUB84(dVar2,0));
  }
  else {
    dVar2 = dVar2 + dVar3;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),*(undefined4 *)param_3);
  }
  else {
    in_xd2 = *param_3;
  }
  if (in_FPSCR_PR == '\0') {
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),
                             SUB84(dVar2,0) + (float)((ulonglong)dVar4 >> 0x20));
  }
  else {
    dVar2 = dVar2 + dVar4;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)param_2 = SUB84(dVar2,0);
    *(int *)((int)param_2 + -4) = (int)((ulonglong)dVar2 >> 0x20);
    *(float *)(param_2 + -1) = fVar1;
  }
  else {
    *param_2 = in_xd2;
    param_2[-1] = dVar2;
    param_2[-2] = in_xd0;
  }
  return;
}

