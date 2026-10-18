/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c60ee0c) */

void FUN_8c60ee0a(undefined8 param_1)

{
  double *pdVar1;
  double *pdVar2;
  double *pdVar3;
  undefined4 in_fr3;
  double dVar4;
  float fVar5;
  float fVar7;
  double dVar6;
  double in_xd2;
  double in_xd4;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  pdVar3 = DAT_8c60ee50;
  pdVar2 = DAT_8c60ee4c;
  pdVar1 = DAT_8c60ee44._4_4_;
  fVar7 = (float)((ulonglong)param_1 >> 0x20);
  fVar5 = -(float)param_1;
  dVar4 = in_xd4;
  if (in_FPSCR_SZ == '\0') {
    dVar4 = (double)CONCAT44(fVar5,in_fr3);
  }
  if (in_FPSCR_PR == '\0') {
    dVar6 = (double)CONCAT44(-fVar7,fVar5);
  }
  else {
    dVar6 = -(double)CONCAT44(fVar7,fVar5);
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)DAT_8c60ee44._4_4_ = (int)((ulonglong)dVar6 >> 0x20);
    *(int *)pdVar2 = SUB84(dVar6,0);
    dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),*(undefined4 *)pdVar1);
  }
  else {
    *DAT_8c60ee44._4_4_ = dVar6;
    *pdVar2 = in_xd4;
    in_xd2 = *pdVar1;
  }
  if (in_FPSCR_PR == '\0') {
    fVar5 = (float)((ulonglong)dVar4 >> 0x20) - SUB84(dVar4,0);
  }
  else {
    fVar5 = (float)((ulonglong)(dVar4 - dVar4) >> 0x20);
  }
  if (in_FPSCR_PR == '\0') {
    fVar5 = 1.0 / fVar5;
  }
  else {
    fVar5 = SUB84((double)CONCAT44(fVar5,0x3f800000) / (double)CONCAT44(fVar5,0x3f800000),0);
  }
  if (in_FPSCR_SZ == '\0') {
    *(float *)pdVar3 = fVar5;
  }
  else {
    *pdVar3 = in_xd2;
  }
  return;
}

