/* Ghidra draft only; not buildable/verified source. */

void FUN_8c640128(undefined4 param_1,undefined8 *param_2,double *param_3)

{
  undefined4 in_fr1;
  double dVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  double in_dr2;
  float fVar5;
  double in_dr12;
  undefined4 in_fr15;
  double dVar6;
  double in_xd2;
  undefined8 in_xd12;
  undefined8 in_xd14;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  fVar2 = (float)((ulonglong)in_xd14 >> 0x20);
  fVar3 = (float)((ulonglong)in_xd12 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
                    /* WARNING: Ignoring partial resolution of indirect */
    dVar6 = (double)CONCAT44(*(undefined4 *)((int)param_2 + 4),in_fr15);
    dVar1 = (double)CONCAT44(*(undefined4 *)((int)param_3 + 4),in_fr1);
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)param_3);
  }
  else {
    dVar6 = *(double *)((int)param_2 + 4);
    dVar1 = *(double *)((int)param_3 + 4);
    in_xd2 = *param_3;
  }
  if (in_FPSCR_PR == '\0') {
    dVar6 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) - (float)((ulonglong)dVar1 >> 0x20),
                             SUB84(dVar6,0));
  }
  else {
    dVar6 = dVar6 - dVar1;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),*(undefined4 *)param_2);
    in_dr12 = (double)CONCAT44((int)((ulonglong)in_dr12 >> 0x20),*(undefined4 *)(param_2 + 1));
  }
  else {
    fVar2 = (float)((ulonglong)*param_2 >> 0x20);
    fVar3 = (float)((ulonglong)param_2[1] >> 0x20);
  }
  if (in_FPSCR_PR == '\0') {
    dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),SUB84(dVar6,0) - SUB84(in_dr2,0));
  }
  else {
    dVar6 = dVar6 - in_dr2;
  }
  fVar4 = (float)((ulonglong)dVar6 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    dVar1 = (double)CONCAT44(*(undefined4 *)(param_3 + 1),fVar4);
  }
  else {
    dVar1 = param_3[1];
    in_xd2 = dVar6;
  }
  if (in_FPSCR_PR == '\0') {
    fVar5 = (float)((ulonglong)dVar1 >> 0x20);
    dVar1 = (double)CONCAT44(fVar5,SUB84(dVar1,0) * fVar4);
    fVar5 = SUB84(in_dr12,0) - fVar5;
  }
  else {
    dVar1 = dVar1 * dVar6;
    fVar5 = SUB84(in_dr12 - dVar1,0);
  }
  fVar4 = SUB84(dVar1,0);
  if (in_FPSCR_SZ == '\0') {
    fVar2 = SUB84(dVar6,0);
  }
  if (in_FPSCR_PR == '\0') {
    fVar4 = fVar2 * SUB84(dVar6,0) + fVar4;
  }
  if (in_FPSCR_SZ == '\0') {
    fVar3 = fVar5;
  }
  if (in_FPSCR_PR == '\0') {
    fVar4 = fVar3 * fVar5 + fVar4;
  }
  if (in_FPSCR_SZ == '\0') {
    in_xd2 = (double)CONCAT44(fVar4,param_1);
  }
  (*(code *)PTR_FUN_8c640168)(in_xd2);
  return;
}

