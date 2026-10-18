/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c60a278) */
/* WARNING: Removing unreachable block (ram,0x8c60a25e) */
/* WARNING: Removing unreachable block (ram,0x8c60a28e) */

double callback_8c60a250(undefined8 param_1,int *param_2,int *param_3)

{
  undefined4 in_fr1;
  undefined8 uVar1;
  double dVar2;
  double in_dr2;
  double dVar3;
  float fVar4;
  undefined4 uVar6;
  double dVar5;
  float fVar7;
  undefined8 in_xd4;
  int iVar8;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  uVar1 = param_1;
  if (in_FPSCR_SZ == '\0') {
    uVar6 = (undefined4)((ulonglong)param_1 >> 0x20);
    param_1 = CONCAT44(uVar6,uVar6);
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),uVar6);
    uVar1 = in_xd4;
  }
  if (in_FPSCR_PR == '\0') {
    dVar5 = (double)CONCAT44(1.0 - SUB84(in_dr2,0),(int)param_1);
  }
  else {
    dVar5 = (double)CONCAT44(0x3f800000,(int)param_1) - in_dr2;
  }
  fVar4 = SUB84(dVar5,0);
  if (in_FPSCR_SZ == '\0') {
    uVar1 = CONCAT44(fVar4,in_fr1);
  }
  fVar7 = (float)((ulonglong)dVar5 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar3 = (double)CONCAT44((float)((ulonglong)uVar1 >> 0x20) * (float)param_2[3] +
                             (float)*param_2 * fVar7,(float)param_2[3]);
  }
  else {
    dVar3 = (double)*param_2 * dVar5;
  }
  dVar2 = dVar5;
  if (in_FPSCR_SZ == '\0') {
    dVar2 = (double)CONCAT44(fVar7,(int)uVar1);
  }
  if (in_FPSCR_PR == '\0') {
    iVar8 = (int)(float)((ulonglong)dVar3 >> 0x20);
  }
  else {
    iVar8 = (int)dVar3;
  }
  *param_3 = iVar8;
  fVar7 = (float)((ulonglong)dVar2 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    iVar8 = (int)(fVar7 * (float)param_2[1] + (float)param_2[4] * fVar4);
  }
  else {
    iVar8 = (int)((double)param_2[4] * dVar5);
  }
  param_3[1] = iVar8;
  if (in_FPSCR_PR == '\0') {
    iVar8 = (int)(fVar7 * (float)param_2[2] + (float)param_2[5] * fVar4);
  }
  else {
    iVar8 = (int)((double)param_2[5] * dVar5);
  }
  param_3[2] = iVar8;
  return dVar2;
}

