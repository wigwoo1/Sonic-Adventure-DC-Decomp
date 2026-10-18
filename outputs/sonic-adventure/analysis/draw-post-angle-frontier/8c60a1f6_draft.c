/* Ghidra draft only; not buildable/verified source. */

void callback_8c60a1f6(ulonglong param_1,ulonglong *param_2,float *param_3)

{
  float fVar1;
  float fVar2;
  double in_dr2;
  double dVar3;
  double dVar4;
  float fVar5;
  uint uVar7;
  double dVar6;
  ulonglong in_xd2;
  ulonglong uVar8;
  ulonglong in_xd4;
  ulonglong uVar9;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  uVar7 = (uint)(param_1 >> 0x20);
  uVar9 = param_1;
  uVar8 = param_1;
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),uVar7);
    param_1 = (ulonglong)uVar7;
    uVar9 = in_xd4;
    uVar8 = in_xd2;
  }
  fVar1 = (float)(uVar9 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar6 = (double)CONCAT44(1.0 - SUB84(in_dr2,0),(int)param_1);
  }
  else {
    dVar6 = (double)CONCAT44(0x3f800000,(int)param_1) - in_dr2;
  }
  fVar5 = SUB84(dVar6,0);
  if (in_FPSCR_SZ == '\0') {
    dVar3 = (double)CONCAT44(*(undefined4 *)((int)param_2 + 0xc),(int)*param_2);
    fVar1 = fVar5;
  }
  else {
    uVar8 = *param_2;
    dVar3 = *(double *)((int)param_2 + 0xc);
  }
  fVar2 = (float)((ulonglong)dVar6 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    fVar1 = fVar1 * (float)((ulonglong)dVar3 >> 0x20) + SUB84(dVar3,0) * fVar2;
  }
  else {
    fVar1 = SUB84(dVar3 * dVar6,0);
  }
  if (in_FPSCR_SZ == '\0') {
    *param_3 = fVar1;
    dVar4 = (double)CONCAT44(*(undefined4 *)((int)param_2 + 4),(int)param_2[2]);
    dVar3 = (double)((ulonglong)(uint)fVar2 << 0x20);
  }
  else {
    *(ulonglong *)param_3 = uVar8;
    uVar8 = param_2[2];
    dVar4 = *(double *)((int)param_2 + 4);
    dVar3 = dVar6;
  }
  fVar1 = (float)((ulonglong)dVar3 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    fVar2 = fVar1 * (float)((ulonglong)dVar4 >> 0x20) + SUB84(dVar4,0) * fVar5;
  }
  else {
    fVar2 = SUB84(dVar4 * dVar6,0);
  }
  if (in_FPSCR_SZ == '\0') {
    param_3[1] = fVar2;
    dVar3 = (double)CONCAT44((int)param_2[1],*(undefined4 *)((int)param_2 + 0x14));
  }
  else {
    *(ulonglong *)(param_3 + 1) = uVar8;
    uVar8 = *(ulonglong *)((int)param_2 + 0x14);
    dVar3 = (double)param_2[1];
  }
  if (in_FPSCR_PR == '\0') {
    fVar1 = fVar1 * (float)((ulonglong)dVar3 >> 0x20) + SUB84(dVar3,0) * fVar5;
  }
  else {
    fVar1 = SUB84(dVar3 * dVar6,0);
  }
  if (in_FPSCR_SZ == '\0') {
    param_3[2] = fVar1;
  }
  else {
    *(ulonglong *)(param_3 + 2) = uVar8;
  }
  return;
}

