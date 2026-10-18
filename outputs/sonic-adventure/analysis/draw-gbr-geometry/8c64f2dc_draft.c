/* Ghidra draft only; not buildable/verified source. */

double FUN_8c64f2dc(undefined4 param_1,undefined4 param_2,undefined4 param_3,double *param_4)

{
  undefined4 in_fr1;
  double dVar1;
  double dVar2;
  double dVar3;
  float fVar4;
  float fVar5;
  double in_xd0;
  double in_xd4;
  double in_xd6;
  double in_xd8;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  if (in_FPSCR_SZ == '\0') {
    dVar2 = (double)CONCAT44(*(undefined4 *)param_4,*(undefined4 *)((int)param_4 + 4));
    dVar3 = (double)CONCAT44(*(undefined4 *)(param_4 + 1),param_1);
    in_xd0 = (double)CONCAT44(in_fr1,in_fr1);
  }
  else {
    dVar2 = *param_4;
    in_xd4 = param_4[1];
    dVar3 = param_4[2];
  }
  fVar5 = (float)((ulonglong)dVar2 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar1 = (double)CONCAT44((float)((ulonglong)in_xd0 >> 0x20) * fVar5,SUB84(in_xd0,0) * fVar5);
  }
  else {
    dVar1 = in_xd0 * dVar2 * dVar2;
  }
  fVar5 = SUB84(dVar2,0);
  if (in_FPSCR_SZ == '\0') {
    in_xd4 = (double)CONCAT44(fVar5,param_2);
    in_xd6 = (double)CONCAT44(SUB84(dVar3,0),param_3);
    dVar1 = (double)((ulonglong)dVar1 >> 0x20);
  }
  if (in_FPSCR_PR == '\0') {
    fVar4 = SUB84(in_xd4,0) * fVar5;
  }
  else {
    fVar4 = SUB84(in_xd4 * dVar2 * dVar2,0);
  }
  if (in_FPSCR_SZ == '\0') {
    in_xd8 = (double)CONCAT44(fVar4,SUB84(dVar1,0));
  }
  fVar4 = (float)((ulonglong)dVar3 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    fVar5 = (float)((ulonglong)in_xd6 >> 0x20) * fVar5;
    dVar2 = (double)CONCAT44((float)((ulonglong)in_xd8 >> 0x20) * fVar4,SUB84(in_xd8,0));
  }
  else {
    fVar5 = (float)((ulonglong)(in_xd6 * dVar2 * dVar2) >> 0x20);
    dVar2 = in_xd8 * dVar3;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar3 = (double)CONCAT44(fVar4,fVar5);
  }
  if (in_FPSCR_PR == '\0') {
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),
                             SUB84(dVar2,0) * (float)((ulonglong)dVar3 >> 0x20));
  }
  else {
    dVar2 = dVar2 * dVar3;
  }
  return dVar2;
}

