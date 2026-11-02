/* Ghidra draft only; not buildable/verified source. */

void FUN_8c64065c(double param_1,double param_2,int param_3)

{
  double *pdVar1;
  double dVar2;
  undefined4 in_fr3;
  double dVar3;
  undefined8 in_xd4;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  if (param_3 == 0) {
    param_3 = *DAT_8c64067c;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar2 = (double)CONCAT44(*(undefined4 *)(param_3 + 0x5c),*(undefined4 *)(param_3 + 0x60));
    pdVar1 = (double *)(param_3 + 100);
  }
  else {
    dVar2 = *(double *)(param_3 + 0x5c);
    pdVar1 = (double *)(param_3 + 0x6c);
  }
  if (in_FPSCR_PR == '\0') {
    param_1 = (double)CONCAT44((float)((ulonglong)param_1 >> 0x20) +
                               (float)((ulonglong)dVar2 >> 0x20),SUB84(param_1,0));
  }
  else {
    param_1 = param_1 + dVar2;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar3 = (double)CONCAT44(*(undefined4 *)pdVar1,in_fr3);
    pdVar1 = (double *)((int)pdVar1 + 4);
  }
  else {
    dVar3 = *pdVar1;
    pdVar1 = pdVar1 + 1;
  }
  if (in_FPSCR_PR == '\0') {
    param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),SUB84(param_1,0) + SUB84(dVar2,0));
    param_2 = (double)CONCAT44((float)((ulonglong)param_2 >> 0x20) +
                               (float)((ulonglong)dVar3 >> 0x20),SUB84(param_2,0));
  }
  else {
    param_1 = param_1 + dVar2;
    param_2 = param_2 + dVar3;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(pdVar1 + -4) = (int)((ulonglong)param_2 >> 0x20);
    *(int *)((int)pdVar1 + -0x24) = SUB84(param_1,0);
    *(int *)(pdVar1 + -5) = (int)((ulonglong)param_1 >> 0x20);
  }
  else {
    *(double *)((int)pdVar1 + -0x24) = param_2;
    *(undefined8 *)((int)pdVar1 + -0x2c) = in_xd4;
    *(double *)((int)pdVar1 + -0x34) = param_1;
  }
  return;
}

