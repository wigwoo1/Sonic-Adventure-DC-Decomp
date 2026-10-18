/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c03ffe2) */
/* WARNING: Removing unreachable block (ram,0x8c03ffba) */
/* WARNING: Removing unreachable block (ram,0x8c03ffaa) */
/* WARNING: Removing unreachable block (ram,0x8c03ffca) */
/* WARNING: Removing unreachable block (ram,0x8c03fffa) */

void FUN_8c03ff90(undefined4 param_1,int *param_2,int param_3,uint param_4,int *param_5)

{
  int *piVar1;
  int iVar2;
  float fVar3;
  undefined8 in_dr2;
  double dVar4;
  float fVar5;
  double dVar6;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  piVar1 = param_2 + param_3 * 4;
  if (param_3 + 1U < param_4) {
    param_2 = piVar1 + 4;
  }
  fVar3 = (float)*piVar1;
  if (in_FPSCR_SZ == '\0') {
    dVar6 = (double)CONCAT44(*(undefined4 *)DAT_8c04007c,param_1);
  }
  else {
    dVar6 = *DAT_8c04007c;
  }
  iVar2 = *param_2 - *piVar1;
  if (in_FPSCR_PR == '\0') {
    fVar3 = (float)((ulonglong)dVar6 >> 0x20) - fVar3;
  }
  else {
    fVar3 = (float)((ulonglong)(dVar6 - (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),fVar3)) >>
                   0x20);
  }
  if (0 < iVar2) {
    fVar5 = (float)iVar2;
    dVar6 = (double)CONCAT44(fVar3,fVar5);
    if (in_FPSCR_PR == '\0') {
      fVar5 = 1.0 / SQRT(fVar5);
      fVar3 = fVar3 * fVar5 * fVar5;
    }
    else {
      dVar6 = dVar6 * dVar6;
      fVar3 = (float)((ulonglong)(dVar6 * dVar6) >> 0x20);
    }
  }
  if (in_FPSCR_PR == '\0') {
    dVar6 = (double)CONCAT44(fVar3,1.0 - fVar3);
  }
  else {
    dVar6 = (double)CONCAT44(fVar3,0x3f800000) - (double)CONCAT44(fVar3,0x3f800000);
  }
  fVar3 = SUB84(dVar6,0);
  fVar5 = (float)((ulonglong)dVar6 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    iVar2 = (int)((float)piVar1[1] * fVar3 + (float)param_2[1] * fVar5);
  }
  else {
    dVar4 = (double)param_2[1] * dVar6;
    iVar2 = (int)(dVar4 + dVar4);
  }
  *param_5 = iVar2;
  if (in_FPSCR_PR == '\0') {
    iVar2 = (int)((float)piVar1[2] * fVar3 + (float)param_2[2] * fVar5);
  }
  else {
    dVar4 = (double)param_2[2] * dVar6;
    iVar2 = (int)(dVar4 + dVar4);
  }
  param_5[1] = iVar2;
  if (in_FPSCR_PR == '\0') {
    iVar2 = (int)((float)piVar1[3] * fVar3 + (float)param_2[3] * fVar5);
  }
  else {
    dVar6 = (double)param_2[3] * dVar6;
    iVar2 = (int)(dVar6 + dVar6);
  }
  param_5[2] = iVar2;
  return;
}

