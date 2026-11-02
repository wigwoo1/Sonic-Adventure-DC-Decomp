/* Ghidra draft only; not buildable/verified source. */

void FUN_8c63adc0(int param_1,uint param_2,uint param_3)

{
  float fVar1;
  double in_dr0;
  float fVar2;
  double in_dr2;
  undefined8 in_xd0;
  undefined8 in_xd2;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  *(uint *)(DAT_8c63adf8 + param_1) = param_2;
  *(uint *)(DAT_8c63adfa + param_1) = param_3;
  if (in_FPSCR_PR == '\0') {
    fVar2 = ((float)(param_2 & 0xffff) * 6.2831855) / 65536.0;
    fVar1 = (float)sin(fVar2);
    fVar2 = (float)cos(fVar2);
    in_dr0 = (double)CONCAT44((float)fVar1,(float)fVar2);
  }
  if (in_FPSCR_PR == '\0') {
    fVar2 = ((float)(param_3 & 0xffff) * 6.2831855) / 65536.0;
    sin(fVar2);
    fVar2 = (float)cos(fVar2);
    fVar1 = SUB84(in_dr0,0) * SUB84(in_dr0,0);
    fVar2 = (float)fVar2 * (float)fVar2;
  }
  else {
    fVar1 = SUB84(in_dr0 * in_dr0,0);
    fVar2 = SUB84(in_dr2 * in_dr2,0);
  }
  if (in_FPSCR_SZ == '\0') {
    *(float *)(DAT_8c63adfc + param_1) = fVar1;
  }
  else {
    *(undefined8 *)(DAT_8c63adfc + param_1) = in_xd0;
  }
  if (in_FPSCR_SZ == '\0') {
    *(float *)(DAT_8c63adfe + param_1) = fVar2;
  }
  else {
    *(undefined8 *)(DAT_8c63adfe + param_1) = in_xd2;
  }
  *(uint *)(DAT_8c63ae00 + param_1) =
       (*(int *)(DAT_8c63adf8 + param_1) + DAT_8c63ae04) * 0x10000 |
       *(int *)(DAT_8c63adfa + param_1) + DAT_8c63ae04 & 0xffffU;
  return;
}

