/* Ghidra draft only; not buildable/verified source. */

double FUN_8c0751c0(float param_1,int param_2,int param_3,int param_4)

{
  float in_fr1;
  float fVar1;
  undefined8 uVar2;
  double dVar3;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  if (in_FPSCR_SZ == '\0') {
    uVar2 = CONCAT44(*(undefined4 *)(param_2 + 0x20),*(undefined4 *)(param_4 + 0x3c));
    fVar1 = (float)DAT_8c0753a0;
  }
  else {
    uVar2 = *(undefined8 *)(param_2 + 0x20);
    in_fr1 = (float)DAT_8c0753a0;
    fVar1 = DAT_8c0753a0._4_4_;
    param_1 = (float)DAT_8c0753a0;
  }
  if (in_FPSCR_PR == '\0') {
    uVar2 = CONCAT44(fVar1 * (float)uVar2 + (float)((ulonglong)uVar2 >> 0x20),(float)uVar2);
  }
  if (in_FPSCR_PR == '\0') {
    dVar3 = (double)CONCAT44(0x40000000,param_1);
  }
  else {
    dVar3 = (double)CONCAT44(0x3f800000,param_1) + (double)CONCAT44(0x3f800000,param_1);
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_2 + 0x20) = (int)((ulonglong)uVar2 >> 0x20);
    uVar2 = CONCAT44(*(undefined4 *)(param_2 + 0x24),*(undefined4 *)(param_4 + 0x40));
  }
  else {
    *(undefined8 *)(param_2 + 0x20) = uVar2;
    uVar2 = *(undefined8 *)(param_2 + 0x24);
  }
  if (in_FPSCR_PR == '\0') {
    uVar2 = CONCAT44(fVar1 * (float)uVar2 + (float)((ulonglong)uVar2 >> 0x20),(float)uVar2);
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_2 + 0x24) = (int)((ulonglong)uVar2 >> 0x20);
    uVar2 = CONCAT44(*(undefined4 *)(param_2 + 0x28),*(undefined4 *)(param_4 + 0x44));
  }
  else {
    *(undefined8 *)(param_2 + 0x24) = uVar2;
    uVar2 = *(undefined8 *)(param_2 + 0x28);
  }
  if (in_FPSCR_PR == '\0') {
    uVar2 = CONCAT44(fVar1 * (float)uVar2 + (float)((ulonglong)uVar2 >> 0x20),(float)uVar2);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar3 = (double)CONCAT44((int)((ulonglong)dVar3 >> 0x20),in_fr1);
    *(int *)(param_2 + 0x28) = (int)((ulonglong)uVar2 >> 0x20);
    uVar2 = CONCAT44(*(undefined4 *)(param_3 + 4),*(undefined4 *)(param_4 + 0x3c));
  }
  else {
    *(undefined8 *)(param_2 + 0x28) = uVar2;
    uVar2 = *(undefined8 *)(param_3 + 4);
  }
  fVar1 = (float)((ulonglong)dVar3 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    uVar2 = CONCAT44(fVar1 * (float)uVar2 + (float)((ulonglong)uVar2 >> 0x20),(float)uVar2);
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_3 + 4) = (int)((ulonglong)uVar2 >> 0x20);
    uVar2 = CONCAT44(*(undefined4 *)(param_3 + 0xc),*(undefined4 *)(param_4 + 0x44));
  }
  else {
    *(undefined8 *)(param_3 + 4) = uVar2;
    uVar2 = *(undefined8 *)(param_3 + 0xc);
  }
  if (in_FPSCR_PR == '\0') {
    uVar2 = CONCAT44(fVar1 * (float)uVar2 + (float)((ulonglong)uVar2 >> 0x20),(float)uVar2);
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_3 + 0xc) = (int)((ulonglong)uVar2 >> 0x20);
  }
  else {
    *(undefined8 *)(param_3 + 0xc) = uVar2;
  }
  return dVar3;
}

