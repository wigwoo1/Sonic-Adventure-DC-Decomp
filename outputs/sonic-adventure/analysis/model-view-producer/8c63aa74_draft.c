/* Ghidra draft only; not buildable/verified source. */

void FUN_8c63aa74(float param_1,undefined4 param_2,undefined8 *param_3,undefined8 *param_4)

{
  double *pdVar1;
  double in_dr0;
  double dVar2;
  float fVar3;
  double in_dr2;
  double dVar4;
  double in_xd0;
  undefined8 in_xd2;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)param_4);
    pdVar1 = (double *)((int)param_4 + 4);
  }
  else {
    in_xd2 = *param_4;
    pdVar1 = (double *)(param_4 + 1);
  }
  if (in_FPSCR_PR == '\0') {
    fVar3 = SUB84(in_dr2,0) * param_1;
  }
  else {
    fVar3 = SUB84(in_dr2 * (double)CONCAT44(param_1,param_2),0);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar4 = (double)CONCAT44(*(undefined4 *)pdVar1,fVar3);
    pdVar1 = (double *)((int)pdVar1 + 4);
  }
  else {
    dVar4 = *pdVar1;
    pdVar1 = pdVar1 + 1;
  }
  if (in_FPSCR_PR == '\0') {
    dVar4 = (double)CONCAT44((float)((ulonglong)dVar4 >> 0x20) * param_1,SUB84(dVar4,0));
  }
  else {
    dVar4 = dVar4 * (double)CONCAT44(param_1,param_2);
  }
  if (in_FPSCR_SZ == '\0') {
    in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),*(undefined4 *)pdVar1);
    pdVar1 = (double *)((int)pdVar1 + 4);
  }
  else {
    in_xd0 = *pdVar1;
    pdVar1 = pdVar1 + 1;
  }
  if (in_FPSCR_PR == '\0') {
    fVar3 = SUB84(in_dr0,0) * param_1;
  }
  else {
    fVar3 = SUB84(in_dr0 * (double)CONCAT44(param_1,param_2),0);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar2 = (double)CONCAT44(*(undefined4 *)pdVar1,fVar3);
  }
  else {
    dVar2 = *pdVar1;
  }
  if (in_FPSCR_PR == '\0') {
    dVar2 = (double)CONCAT44((float)((ulonglong)dVar2 >> 0x20) * param_1,SUB84(dVar2,0));
  }
  else {
    dVar2 = dVar2 * (double)CONCAT44(param_1,param_2);
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)param_3 = SUB84(dVar4,0);
    *(int *)((int)param_3 + 4) = (int)((ulonglong)dVar4 >> 0x20);
    *(int *)(param_3 + 1) = SUB84(dVar2,0);
    *(int *)((int)param_3 + 0xc) = (int)((ulonglong)dVar2 >> 0x20);
  }
  else {
    *param_3 = in_xd2;
    *(double *)((int)param_3 + 4) = dVar4;
    param_3[1] = in_xd0;
    *(double *)((int)param_3 + 0xc) = dVar2;
  }
  return;
}

