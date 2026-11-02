/* Ghidra draft only; not buildable/verified source. */

void FUN_8c63a7e4(undefined4 param_1,undefined4 param_2,double *param_3,float *param_4)

{
  float in_fr1;
  undefined4 in_fr0;
  undefined4 in_fr3;
  double dVar1;
  double dVar2;
  double dVar3;
  double in_xd4;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  if (in_FPSCR_SZ == '\0') {
    in_fr1 = *param_4;
    dVar2 = (double)CONCAT44(*(undefined4 *)param_3,param_1);
    param_3 = (double *)((int)param_3 + 4);
    dVar1 = (double)CONCAT44(param_4[1],in_fr3);
    param_4 = param_4 + 2;
  }
  else {
    dVar2 = *param_3;
    param_3 = param_3 + 1;
    dVar1 = *(double *)(param_4 + 2);
    param_4 = param_4 + 4;
  }
  if (in_FPSCR_PR == '\0') {
    dVar2 = (double)CONCAT44((float)((ulonglong)dVar2 >> 0x20) - in_fr1,SUB84(dVar2,0));
  }
  else {
    dVar2 = dVar2 - (double)CONCAT44(in_fr0,in_fr1);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),*(undefined4 *)param_3);
    param_3 = (double *)((int)param_3 + 4);
    dVar1 = (double)CONCAT44((int)((ulonglong)dVar1 >> 0x20),*param_4);
  }
  else {
    in_xd4 = *param_3;
    param_3 = param_3 + 1;
  }
  if (in_FPSCR_PR == '\0') {
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),
                             SUB84(dVar2,0) - (float)((ulonglong)dVar1 >> 0x20));
  }
  else {
    dVar2 = dVar2 - dVar1;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar3 = (double)CONCAT44(*(undefined4 *)param_3,param_2);
  }
  else {
    dVar3 = *param_3;
  }
  if (in_FPSCR_PR == '\0') {
    dVar3 = (double)CONCAT44((float)((ulonglong)dVar3 >> 0x20) - SUB84(dVar1,0),SUB84(dVar3,0));
  }
  else {
    dVar3 = dVar3 - dVar1;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)param_3 = (int)((ulonglong)dVar3 >> 0x20);
    *(int *)((int)param_3 + -4) = SUB84(dVar2,0);
    *(int *)(param_3 + -1) = (int)((ulonglong)dVar2 >> 0x20);
  }
  else {
    *param_3 = dVar3;
    param_3[-1] = in_xd4;
    param_3[-2] = dVar2;
  }
  return;
}

