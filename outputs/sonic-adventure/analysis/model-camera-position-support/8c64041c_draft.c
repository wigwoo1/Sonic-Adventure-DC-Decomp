/* Ghidra draft only; not buildable/verified source. */

double FUN_8c64041c(undefined4 param_1,int param_2,ushort param_3)

{
  float fVar1;
  double in_dr0;
  float fVar2;
  float fVar3;
  double dVar4;
  float fVar5;
  undefined8 uVar6;
  double dVar7;
  double in_xd2;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  if (in_FPSCR_PR == '\0') {
    fVar3 = ((float)param_3 * 6.2831855) / 65536.0;
    fVar1 = (float)sin(fVar3);
    fVar3 = (float)cos(fVar3);
    in_dr0 = (double)CONCAT44((float)fVar1,(float)fVar3);
    if (param_2 != 0) goto LAB_8c640428;
  }
  param_2 = *DAT_8c64044c;
LAB_8c640428:
  if (in_FPSCR_SZ == '\0') {
    uVar6 = CONCAT44(*(undefined4 *)(param_2 + 0x68),param_1);
    dVar4 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x6c),*(undefined4 *)(param_2 + 0x70));
    param_2 = param_2 + 0x74;
  }
  else {
    uVar6 = *(undefined8 *)(param_2 + 0x68);
    dVar4 = *(double *)(param_2 + 0x70);
    in_xd2 = *(double *)(param_2 + 0x78);
    param_2 = param_2 + 0x80;
  }
  fVar3 = SUB84(dVar4,0);
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_2 + -0x28) = (int)((ulonglong)uVar6 >> 0x20);
    in_xd2 = (double)CONCAT44(fVar3,(int)uVar6);
  }
  else {
    *(undefined8 *)(param_2 + -0x28) = uVar6;
  }
  fVar1 = SUB84(in_dr0,0);
  fVar2 = (float)((ulonglong)in_dr0 >> 0x20);
  fVar5 = (float)((ulonglong)dVar4 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar7 = (double)CONCAT44(fVar2 * fVar5 + (float)((ulonglong)in_xd2 >> 0x20) * fVar1,
                             SUB84(in_xd2,0));
  }
  else {
    dVar7 = in_xd2 * in_dr0;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_2 + -0x20) = (int)((ulonglong)dVar7 >> 0x20);
  }
  else {
    *(double *)(param_2 + -0x20) = dVar7;
  }
  if (in_FPSCR_PR == '\0') {
    fVar2 = -fVar2;
    dVar7 = (double)CONCAT44(fVar2,fVar1);
    dVar4 = (double)CONCAT44(fVar2 * fVar3 + fVar5 * fVar1,fVar3);
  }
  else {
    dVar4 = dVar4 * in_dr0;
    dVar7 = -in_dr0;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_2 + -0x24) = (int)((ulonglong)dVar4 >> 0x20);
  }
  else {
    *(double *)(param_2 + -0x24) = dVar4;
  }
  return dVar7;
}

