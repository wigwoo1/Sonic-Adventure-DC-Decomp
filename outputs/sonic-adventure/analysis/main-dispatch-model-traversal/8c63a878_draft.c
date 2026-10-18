/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Restarted to delay deadcode elimination for space: register */

undefined8 FUN_8c63a878(float param_1,float param_2,float param_3,undefined8 *param_4)

{
  undefined8 in_dr0;
  undefined8 uVar1;
  undefined8 uVar2;
  float fVar3;
  undefined8 in_xd2;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  if (param_4 == (undefined8 *)0x0) {
    return in_dr0;
  }
  if (in_FPSCR_SZ == '\0') {
    uVar1 = CONCAT44(*(undefined4 *)param_4,*(undefined4 *)(param_4 + 2));
    uVar2 = CONCAT44(*(undefined4 *)(param_4 + 4),*(undefined4 *)(param_4 + 6));
  }
  else {
    uVar1 = *param_4;
    uVar2 = param_4[4];
    in_xd2 = param_4[6];
  }
                    /* WARNING: Ignoring partial resolution of indirect */
  if (in_FPSCR_PR == '\0') {
    fVar3 = (float)uVar1 * param_2 + (float)((ulonglong)uVar1 >> 0x20) * param_1 +
            (float)uVar2 * 1.0 + (float)((ulonglong)uVar2 >> 0x20) * param_3;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_4 + 6) = (int)uVar2;
    uVar1 = CONCAT44(*(undefined4 *)((int)param_4 + 4),*(undefined4 *)((int)param_4 + 0x14));
    uVar2 = CONCAT44(*(undefined4 *)((int)param_4 + 0x24),*(undefined4 *)((int)param_4 + 0x34));
  }
  else {
    param_4[6] = in_xd2;
    uVar1 = *(undefined8 *)((int)param_4 + 4);
    uVar2 = *(undefined8 *)((int)param_4 + 0x24);
    in_xd2 = *(undefined8 *)((int)param_4 + 0x34);
  }
                    /* WARNING: Ignoring partial resolution of indirect */
  if (in_FPSCR_PR == '\0') {
    fVar3 = (float)uVar1 * param_2 + (float)((ulonglong)uVar1 >> 0x20) * param_1 +
            (float)uVar2 * 1.0 + (float)((ulonglong)uVar2 >> 0x20) * param_3;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)param_4 + 0x34) = (int)uVar2;
    uVar1 = CONCAT44(*(undefined4 *)(param_4 + 1),*(undefined4 *)(param_4 + 3));
    uVar2 = CONCAT44(*(undefined4 *)(param_4 + 5),*(undefined4 *)(param_4 + 7));
  }
  else {
    *(undefined8 *)((int)param_4 + 0x34) = in_xd2;
    uVar1 = param_4[1];
    uVar2 = param_4[5];
    in_xd2 = param_4[7];
  }
                    /* WARNING: Ignoring partial resolution of indirect */
  if (in_FPSCR_PR == '\0') {
    fVar3 = (float)uVar1 * param_2 + (float)((ulonglong)uVar1 >> 0x20) * param_1 +
            (float)uVar2 * 1.0 + (float)((ulonglong)uVar2 >> 0x20) * param_3;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_4 + 7) = (int)uVar2;
    uVar1 = CONCAT44(*(undefined4 *)((int)param_4 + 0xc),*(undefined4 *)((int)param_4 + 0x1c));
    uVar2 = CONCAT44(*(undefined4 *)((int)param_4 + 0x2c),*(undefined4 *)((int)param_4 + 0x3c));
  }
  else {
    param_4[7] = in_xd2;
    uVar1 = *(undefined8 *)((int)param_4 + 0xc);
    uVar2 = *(undefined8 *)((int)param_4 + 0x2c);
    in_xd2 = *(undefined8 *)((int)param_4 + 0x3c);
  }
                    /* WARNING: Ignoring partial resolution of indirect */
  if (in_FPSCR_PR == '\0') {
    fVar3 = (float)uVar1 * param_2 + (float)((ulonglong)uVar1 >> 0x20) * param_1 +
            (float)uVar2 * 1.0 + (float)((ulonglong)uVar2 >> 0x20) * param_3;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)param_4 + 0x3c) = (int)uVar2;
  }
  else {
    *(undefined8 *)((int)param_4 + 0x3c) = in_xd2;
  }
  return uVar1;
}

