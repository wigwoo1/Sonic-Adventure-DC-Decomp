/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c65065c) */

bool FUN_8c650614(undefined4 param_1,undefined4 param_2,double param_3,undefined4 param_4,
                 int param_5,int param_6)

{
  undefined4 in_fr1;
  float fVar1;
  undefined4 uVar2;
  float fVar3;
  double in_dr2;
  double dVar4;
  double dVar5;
  double dVar6;
  double dVar7;
  double dVar8;
  double dVar9;
  float fVar11;
  double in_dr14;
  double in_xd14;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined4 uVar10;
  
  if (in_FPSCR_SZ == '\0') {
    uVar10 = *(undefined4 *)(param_5 + 0xc);
    param_1 = *(undefined4 *)(param_5 + 0x10);
    dVar7 = (double)CONCAT44(*(undefined4 *)(param_6 + 0x10),param_2);
    param_3 = (double)CONCAT44((int)((ulonglong)param_3 >> 0x20),*(undefined4 *)(param_6 + 0xc));
    dVar9 = (double)CONCAT44(uVar10,in_fr1);
    dVar5 = (double)CONCAT44(uVar10,param_4);
  }
  else {
    dVar9 = *(double *)(param_5 + 0xc);
    dVar7 = *(double *)(param_6 + 0x10);
    uVar10 = (undefined4)((ulonglong)dVar9 >> 0x20);
    dVar5 = dVar9;
  }
  fVar11 = (float)((ulonglong)dVar7 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) * fVar11,SUB84(dVar9,0));
  }
  else {
    dVar9 = dVar9 * dVar7;
  }
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),param_1);
  }
  if (in_FPSCR_PR == '\0') {
    dVar4 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),SUB84(in_dr2,0) * SUB84(param_3,0));
  }
  else {
    dVar4 = in_dr2 * param_3;
  }
  uVar2 = (undefined4)((ulonglong)dVar9 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    dVar8 = (double)CONCAT44(*(undefined4 *)(param_5 + 0x14),SUB84(param_3,0));
    dVar5 = (double)CONCAT44(uVar10,*(undefined4 *)(param_6 + 0x14));
    dVar6 = (double)CONCAT44(uVar2,param_1);
  }
  else {
    dVar8 = *(double *)(param_5 + 0x14);
    dVar6 = dVar9;
  }
  if (in_FPSCR_PR == '\0') {
    dVar6 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) - SUB84(dVar4,0),SUB84(dVar6,0));
  }
  else {
    dVar6 = dVar6 - dVar4;
  }
  uVar10 = (undefined4)((ulonglong)dVar8 >> 0x20);
  dVar4 = dVar8;
  if (in_FPSCR_SZ == '\0') {
    dVar4 = (double)CONCAT44(uVar10,SUB84(dVar6,0));
  }
  if (in_FPSCR_PR == '\0') {
    dVar4 = (double)CONCAT44((float)((ulonglong)dVar4 >> 0x20) * fVar11,
                             SUB84(dVar4,0) * SUB84(dVar5,0));
  }
  else {
    dVar4 = dVar4 * dVar5 * dVar7;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar9 = (double)CONCAT44(uVar2,(int)((ulonglong)dVar5 >> 0x20));
  }
  if (in_FPSCR_PR == '\0') {
    fVar1 = SUB84(dVar9,0) * SUB84(dVar5,0);
  }
  else {
    fVar1 = SUB84(dVar9 * dVar5,0);
  }
  fVar3 = (float)((ulonglong)dVar6 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    dVar6 = (double)CONCAT44(fVar3,fVar1);
    dVar7 = (double)CONCAT44(fVar11,SUB84(dVar4,0));
  }
  if (in_FPSCR_PR == '\0') {
    dVar7 = (double)CONCAT44((int)((ulonglong)dVar7 >> 0x20),
                             SUB84(dVar7,0) - (float)((ulonglong)dVar4 >> 0x20));
  }
  else {
    dVar7 = dVar7 - dVar4;
  }
  dVar5 = dVar8;
  if (in_FPSCR_SZ == '\0') {
    dVar5 = (double)CONCAT44(uVar10,SUB84(dVar4,0));
  }
  if (in_FPSCR_PR == '\0') {
    dVar5 = (double)CONCAT44((float)((ulonglong)dVar5 >> 0x20) * SUB84(dVar8,0),SUB84(dVar5,0));
  }
  else {
    dVar5 = dVar5 * dVar8;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar5 = (double)CONCAT44((int)((ulonglong)dVar5 >> 0x20),SUB84(dVar7,0));
  }
  if (in_FPSCR_PR == '\0') {
    dVar5 = (double)CONCAT44((int)((ulonglong)dVar5 >> 0x20),SUB84(dVar5,0) * SUB84(dVar7,0));
  }
  else {
    dVar5 = dVar5 * dVar7;
  }
  dVar7 = dVar5;
  if (in_FPSCR_SZ == '\0') {
    in_dr14 = (double)CONCAT44((int)((ulonglong)in_dr14 >> 0x20),(int)((ulonglong)dVar5 >> 0x20));
    dVar7 = in_xd14;
  }
  if (in_FPSCR_PR == '\0') {
    dVar5 = (double)(ulonglong)(uint)((float)((ulonglong)dVar6 >> 0x20) * fVar3 + SUB84(dVar5,0));
    fVar11 = SUB84(in_dr14,0) - SUB84(dVar6,0);
  }
  else {
    fVar11 = SUB84(in_dr14 - dVar6,0);
  }
  fVar3 = SUB84(dVar5,0);
  fVar1 = fVar11;
  if (in_FPSCR_SZ != '\0') {
    fVar1 = (float)((ulonglong)dVar7 >> 0x20);
  }
  if (in_FPSCR_PR == '\0') {
    fVar3 = fVar1 * fVar11 + fVar3;
  }
  return fVar3 == 0.0;
}

