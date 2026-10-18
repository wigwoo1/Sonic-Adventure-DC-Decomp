/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x0c905644) */
/* WARNING: Removing unreachable block (ram,0x0c90562e) */
/* WARNING: Removing unreachable block (ram,0x0c90565c) */

void FUN_0c905618(undefined4 param_1,int param_2,int param_3)

{
  float fVar1;
  double in_dr0;
  undefined4 uVar2;
  undefined8 in_dr2;
  double dVar3;
  double dVar4;
  undefined8 in_xd0;
  undefined8 in_xd2;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  if (in_FPSCR_SZ == '\0') {
    dVar4 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x30),param_1);
    in_dr0 = (double)CONCAT44((int)((ulonglong)in_dr0 >> 0x20),*(undefined4 *)(param_3 + 0xd4));
  }
  else {
    dVar4 = *(double *)(param_2 + 0x30);
    in_xd0 = *(undefined8 *)(param_3 + 0xd4);
  }
  if (in_FPSCR_PR == '\0') {
    dVar4 = (double)CONCAT44((float)((ulonglong)dVar4 >> 0x20) + -1.0,SUB84(dVar4,0));
  }
  else {
    dVar4 = dVar4 + (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),0xbf800000);
  }
  dVar3 = (double)CONCAT44(DAT_0c905744,0xbf800000);
  if (in_FPSCR_PR == '\0') {
    fVar1 = SUB84(in_dr0,0) * DAT_0c905744;
  }
  else {
    fVar1 = SUB84(in_dr0 * dVar3,0);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar3 = (double)CONCAT44(DAT_0c905744,*(undefined4 *)(param_3 + 0xd4));
  }
  else {
    in_xd2 = *(undefined8 *)(param_3 + 0xd4);
  }
  if (in_FPSCR_PR == '\0') {
    dVar4 = (double)CONCAT44((float)((ulonglong)dVar4 >> 0x20) * DAT_0c905748,SUB84(dVar4,0));
  }
  else {
    dVar4 = dVar4 * (double)CONCAT44(DAT_0c905748,fVar1);
  }
  if (in_FPSCR_PR == '\0') {
    fVar1 = -fVar1 - (float)((ulonglong)dVar4 >> 0x20);
    dVar4 = (double)CONCAT44(DAT_0c905748,fVar1);
    dVar3 = (double)CONCAT44((int)((ulonglong)dVar3 >> 0x20),SUB84(dVar3,0) + fVar1);
  }
  else {
    dVar4 = (double)CONCAT44(DAT_0c905748,-fVar1) - dVar4;
    dVar3 = dVar3 + dVar4;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_3 + 0xd4) = SUB84(dVar3,0);
    dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),*(undefined4 *)(param_2 + 0x30));
    dVar3 = (double)CONCAT44((int)((ulonglong)dVar3 >> 0x20),*(undefined4 *)(param_3 + 0xd4));
  }
  else {
    *(undefined8 *)(param_3 + 0xd4) = in_xd2;
    in_xd0 = *(undefined8 *)(param_2 + 0x30);
    in_xd2 = *(undefined8 *)(param_3 + 0xd4);
  }
  if (in_FPSCR_PR == '\0') {
    dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),SUB84(dVar4,0) + SUB84(dVar3,0));
  }
  else {
    dVar4 = dVar4 + dVar3;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar3 = (double)CONCAT44((int)((ulonglong)dVar3 >> 0x20),SUB84(dVar4,0));
    *(int *)(param_2 + 0x30) = SUB84(dVar4,0);
    in_xd0 = in_xd2;
  }
  else {
    *(undefined8 *)(param_2 + 0x30) = in_xd0;
  }
  uVar2 = (undefined4)((ulonglong)dVar4 >> 0x20);
  dVar4 = (double)CONCAT44(uVar2,0x3f800000);
  if (in_FPSCR_PR == '\0') {
    dVar4 = (double)CONCAT44(uVar2,0x40000000);
  }
  else {
    dVar4 = dVar4 + dVar4;
  }
  if (in_FPSCR_PR == '\0') {
    fVar1 = -SUB84(dVar3,0) + SUB84(dVar4,0);
  }
  else {
    fVar1 = SUB84((double)CONCAT44((int)((ulonglong)dVar3 >> 0x20),-SUB84(dVar3,0)) + dVar4,0);
  }
  if (in_FPSCR_SZ == '\0') {
    *(float *)(param_2 + 0x2c) = fVar1;
  }
  else {
    *(undefined8 *)(param_2 + 0x2c) = in_xd0;
  }
  return;
}

