/* Ghidra draft only; not buildable/verified source. */

void FUN_8c63ae0c(float param_1,undefined4 param_2,float *param_3,undefined8 *param_4)

{
  float fVar1;
  double in_dr2;
  double dVar2;
  undefined8 in_xd2;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)param_4);
  }
  else {
    in_xd2 = *param_4;
  }
  if (in_FPSCR_PR == '\0') {
    fVar1 = SUB84(in_dr2,0) * param_1;
  }
  else {
    fVar1 = SUB84(in_dr2 * (double)CONCAT44(param_1,param_2),0);
  }
  if (in_FPSCR_SZ == '\0') {
    *param_3 = fVar1;
    dVar2 = (double)CONCAT44(*(undefined4 *)((int)param_4 + 4),fVar1);
  }
  else {
    *(undefined8 *)param_3 = in_xd2;
    dVar2 = *(double *)((int)param_4 + 4);
  }
  if (in_FPSCR_PR == '\0') {
    dVar2 = (double)CONCAT44((float)((ulonglong)dVar2 >> 0x20) * param_1,SUB84(dVar2,0));
  }
  else {
    dVar2 = dVar2 * (double)CONCAT44(param_1,param_2);
  }
  fVar1 = (float)((ulonglong)dVar2 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    param_3[1] = fVar1;
    dVar2 = (double)CONCAT44(fVar1,*(undefined4 *)(param_4 + 1));
  }
  else {
    *(double *)(param_3 + 1) = dVar2;
    in_xd2 = param_4[1];
  }
  if (in_FPSCR_PR == '\0') {
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),SUB84(dVar2,0) * param_1);
  }
  else {
    dVar2 = dVar2 * (double)CONCAT44(param_1,param_2);
  }
  if (in_FPSCR_SZ == '\0') {
    param_3[2] = SUB84(dVar2,0);
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),*(undefined4 *)((int)param_4 + 0xc));
  }
  else {
    *(undefined8 *)(param_3 + 2) = in_xd2;
    in_xd2 = *(undefined8 *)((int)param_4 + 0xc);
  }
  if (in_FPSCR_PR == '\0') {
    param_1 = SUB84(dVar2,0) * param_1;
  }
  else {
    param_1 = SUB84(dVar2 * (double)CONCAT44(param_1,param_2),0);
  }
  if (in_FPSCR_SZ == '\0') {
    param_3[3] = param_1;
  }
  else {
    *(undefined8 *)(param_3 + 3) = in_xd2;
  }
  return;
}

