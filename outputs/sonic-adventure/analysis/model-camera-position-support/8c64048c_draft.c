/* Ghidra draft only; not buildable/verified source. */

void FUN_8c64048c(undefined4 param_1,int param_2,ushort param_3)

{
  double dVar1;
  double in_dr0;
  float fVar2;
  double dVar3;
  undefined8 in_xd2;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  float fVar4;
  
  if (in_FPSCR_PR == '\0') {
    fVar4 = ((float)param_3 * 6.2831855) / 65536.0;
    fVar2 = (float)sin(fVar4);
    fVar4 = (float)cos(fVar4);
    in_dr0 = (double)CONCAT44((float)fVar2,(float)fVar4);
    if (param_2 != 0) goto LAB_8c64049a;
  }
  param_2 = *DAT_8c6404bc;
LAB_8c64049a:
  if (in_FPSCR_SZ == '\0') {
    fVar4 = *(float *)(DAT_8c6404b8 + param_2);
    dVar3 = (double)CONCAT44(fVar4,param_1);
    dVar1 = (double)CONCAT44(fVar4,*(undefined4 *)(DAT_8c6404c0 + param_2));
  }
  else {
    dVar3 = *(double *)(DAT_8c6404b8 + param_2);
    fVar4 = (float)((ulonglong)dVar3 >> 0x20);
    in_xd2 = *(undefined8 *)(DAT_8c6404c0 + param_2);
    dVar1 = dVar3;
  }
  fVar2 = (float)((ulonglong)in_dr0 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar3 = (double)CONCAT44(fVar2 * SUB84(dVar1,0) +
                             (float)((ulonglong)dVar3 >> 0x20) * SUB84(in_dr0,0),SUB84(dVar3,0));
  }
  else {
    dVar3 = dVar3 * in_dr0;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(DAT_8c6404b8 + param_2) = (int)((ulonglong)dVar3 >> 0x20);
  }
  else {
    *(double *)(DAT_8c6404b8 + param_2) = dVar3;
  }
  if (in_FPSCR_PR == '\0') {
    fVar4 = -fVar2 * fVar4 + SUB84(dVar1,0) * SUB84(in_dr0,0);
  }
  else {
    fVar4 = SUB84(dVar1 * in_dr0,0);
  }
  if (in_FPSCR_SZ == '\0') {
    *(float *)(DAT_8c6404c0 + param_2) = fVar4;
  }
  else {
    *(undefined8 *)(DAT_8c6404c0 + param_2) = in_xd2;
  }
  return;
}

